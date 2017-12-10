/*!
@file BasePhysics.cpp
@brief �����v�Z�C���^�[�t�@�C�X����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"


namespace basecross {

	using namespace sce::PhysicsEffects;


	namespace ps {

#define NUM_RIGIDBODIES 500
#define NUM_JOINTS    500
#define NUM_CONTACTS  4000

		const float timeStep = 0.016f;
		const float separateBias = 0.1f;
		int iteration = 5;

		//�d��
		PfxVector3 PsGravity(0.0f, -9.8f, 0.0f);

		//J ���[���h�T�C�Y
		//E World size
		PfxVector3 worldCenter(0.0f);
		PfxVector3 worldExtent(500.0f);

		//J ����
		//E Rigid body
		PfxRigidState states[NUM_RIGIDBODIES];
		PfxRigidBody  bodies[NUM_RIGIDBODIES];
		PfxCollidable collidables[NUM_RIGIDBODIES];
		PfxSolverBody solverBodies[NUM_RIGIDBODIES];
		int numRigidBodies = 0;

		//J �v���L�V
		//E Proxies
		PfxBroadphaseProxy proxies[NUM_RIGIDBODIES];

		//J �W���C���g
		//E Joint
		PfxConstraintPair jointPairs[NUM_JOINTS];
		PfxJoint joints[NUM_JOINTS];
		int numJoints = 0;

		//J �y�A
		//E Pairs
		unsigned int pairSwap;
		unsigned int numPairs[2];
		PfxBroadphasePair pairsBuff[2][NUM_CONTACTS];

		//J �R���^�N�g
		//E Contacts
		PfxContactManifold contacts[NUM_CONTACTS];
		int numContacts;

		PfxUInt32 contactIdPool[NUM_CONTACTS];
		int numContactIdPool;

		//J �ꎞ�o�b�t�@
		//E Temporary buffers
#define POOL_BYTES (5*1024*1024)
		unsigned char SCE_PFX_ALIGNED(128) poolBuff[POOL_BYTES];

		//J �ꎞ�o�b�t�@�p�X�^�b�N�A���P�[�^
		//E Stack allocator for temporary buffers
		PfxHeapManager pool(poolBuff, POOL_BYTES);


//		int frame = 0;

		void broadphase()
		{
			pairSwap = 1 - pairSwap;

			unsigned int &numPreviousPairs = numPairs[1 - pairSwap];
			unsigned int &numCurrentPairs = numPairs[pairSwap];
			PfxBroadphasePair *previousPairs = pairsBuff[1 - pairSwap];
			PfxBroadphasePair *currentPairs = pairsBuff[pairSwap];

			//J ���̂��ł����U���Ă��鎲��������
			//E Find the axis along which all rigid bodies are most widely positioned
			int axis = 0;
			{
				PfxVector3 s(0.0f), s2(0.0f);
				for (int i = 0; i<numRigidBodies; i++) {
					PfxVector3 c = states[i].getPosition();
					s += c;
					s2 += mulPerElem(c, c);
				}
				PfxVector3 v = s2 - mulPerElem(s, s) / (float)numRigidBodies;
				if (v[1] > v[0]) axis = 1;
				if (v[2] > v[axis]) axis = 2;
			}

			//J �u���[�h�t�F�[�Y�v���L�V�̍X�V
			//E Create broadpahse proxies
			{
				for (int i = 0; i<numRigidBodies; i++) {
					pfxUpdateBroadphaseProxy(proxies[i], states[i], collidables[i], worldCenter, worldExtent, axis);
				}

				int workBytes = sizeof(PfxBroadphaseProxy) * numRigidBodies;
				void *workBuff = pool.allocate(workBytes);

				pfxSort(proxies, (PfxSortData32*)workBuff, numRigidBodies);

				pool.deallocate(workBuff);
			}

			//J �����y�A�T��
			//E Find overlapped pairs
			{
				PfxFindPairsParam findPairsParam;
				findPairsParam.pairBytes = pfxGetPairBytesOfFindPairs(NUM_CONTACTS);
				findPairsParam.pairBuff = pool.allocate(findPairsParam.pairBytes);
				findPairsParam.workBytes = pfxGetWorkBytesOfFindPairs(NUM_CONTACTS);
				findPairsParam.workBuff = pool.allocate(findPairsParam.workBytes);
				findPairsParam.proxies = proxies;
				findPairsParam.numProxies = numRigidBodies;
				findPairsParam.maxPairs = NUM_CONTACTS;
				findPairsParam.axis = axis;

				PfxFindPairsResult findPairsResult;

				int ret = pfxFindPairs(findPairsParam, findPairsResult);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxFindPairs failed %d\n", ret);

				pool.deallocate(findPairsParam.workBuff);

				//J �����y�A����
				//E Decompose overlapped pairs into 3 arrays
				PfxDecomposePairsParam decomposePairsParam;
				decomposePairsParam.pairBytes = pfxGetPairBytesOfDecomposePairs(numPreviousPairs, findPairsResult.numPairs);
				decomposePairsParam.pairBuff = pool.allocate(decomposePairsParam.pairBytes);
				decomposePairsParam.workBytes = pfxGetWorkBytesOfDecomposePairs(numPreviousPairs, findPairsResult.numPairs);
				decomposePairsParam.workBuff = pool.allocate(decomposePairsParam.workBytes);
				decomposePairsParam.previousPairs = previousPairs;
				decomposePairsParam.numPreviousPairs = numPreviousPairs;
				decomposePairsParam.currentPairs = findPairsResult.pairs; // Set pairs from pfxFindPairs()
				decomposePairsParam.numCurrentPairs = findPairsResult.numPairs; // Set the number of pairs from pfxFindPairs()

				PfxDecomposePairsResult decomposePairsResult;

				ret = pfxDecomposePairs(decomposePairsParam, decomposePairsResult);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxDecomposePairs failed %d\n", ret);

				pool.deallocate(decomposePairsParam.workBuff);

				PfxBroadphasePair *outNewPairs = decomposePairsResult.outNewPairs;
				PfxBroadphasePair *outKeepPairs = decomposePairsResult.outKeepPairs;
				PfxBroadphasePair *outRemovePairs = decomposePairsResult.outRemovePairs;
				PfxUInt32 numOutNewPairs = decomposePairsResult.numOutNewPairs;
				PfxUInt32 numOutKeepPairs = decomposePairsResult.numOutKeepPairs;
				PfxUInt32 numOutRemovePairs = decomposePairsResult.numOutRemovePairs;

				//J �p���y�A�̃R���^�N�g���v�[���ɖ߂�
				//E Put removed contacts into the contact pool
				for (PfxUInt32 i = 0; i<numOutRemovePairs; i++) {
					contactIdPool[numContactIdPool++] = pfxGetContactId(outRemovePairs[i]);
				}

				//J �V�K�y�A�̃R���^�N�g�̃����N�Ə�����
				//E Add new contacts and initialize
				for (PfxUInt32 i = 0; i<numOutNewPairs; i++) {
					int cId = 0;
					if (numContactIdPool > 0) {
						cId = contactIdPool[--numContactIdPool];
					}
					else {
						cId = numContacts++;
					}
					if (cId >= NUM_CONTACTS) {
						cId = 0;
					}
					SCE_PFX_ASSERT(cId < NUM_CONTACTS);
					pfxSetContactId(outNewPairs[i], cId);
					PfxContactManifold &contact = contacts[cId];
					contact.reset(pfxGetObjectIdA(outNewPairs[i]), pfxGetObjectIdB(outNewPairs[i]));
				}

				//J �V�K�y�A�ƈێ��y�A������
				//E Merge 'new' and 'keep' pairs
				numCurrentPairs = 0;
				for (PfxUInt32 i = 0; i<numOutKeepPairs; i++) {
					currentPairs[numCurrentPairs++] = outKeepPairs[i];
				}
				for (PfxUInt32 i = 0; i<numOutNewPairs; i++) {
					currentPairs[numCurrentPairs++] = outNewPairs[i];
				}

				pool.deallocate(decomposePairsParam.pairBuff);
				pool.deallocate(findPairsParam.pairBuff);
			}

			{
				int workBytes = sizeof(PfxBroadphasePair) * numCurrentPairs;
				void *workBuff = pool.allocate(workBytes);

				pfxSort(currentPairs, (PfxSortData16*)workBuff, numCurrentPairs);

				pool.deallocate(workBuff);
			}
		}

		void collision()
		{
			unsigned int numCurrentPairs = numPairs[pairSwap];
			PfxBroadphasePair *currentPairs = pairsBuff[pairSwap];

			//J ���t���b�V��
			//E Refresh contacts
			{
				PfxRefreshContactsParam param;
				param.contactPairs = currentPairs;
				param.numContactPairs = numCurrentPairs;
				param.offsetContactManifolds = contacts;
				param.offsetRigidStates = states;
				param.numRigidBodies = numRigidBodies;

				int ret = pfxRefreshContacts(param);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxRefreshContacts failed %d\n", ret);
			}

			//J �Փˌ��o
			//E Detect collisions
			{
				PfxDetectCollisionParam param;
				param.contactPairs = currentPairs;
				param.numContactPairs = numCurrentPairs;
				param.offsetContactManifolds = contacts;
				param.offsetRigidStates = states;
				param.offsetCollidables = collidables;
				param.numRigidBodies = numRigidBodies;

				int ret = pfxDetectCollision(param);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxDetectCollision failed %d\n", ret);
			}
		}

		void constraintSolver()
		{
//			PfxPerfCounter pc;

			unsigned int numCurrentPairs = numPairs[pairSwap];
			PfxBroadphasePair *currentPairs = pairsBuff[pairSwap];

//			pc.countBegin("setup solver bodies");
			{
				PfxSetupSolverBodiesParam param;
				param.states = states;
				param.bodies = bodies;
				param.solverBodies = solverBodies;
				param.numRigidBodies = numRigidBodies;

				int ret = pfxSetupSolverBodies(param);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxSetupSolverBodies failed %d\n", ret);
			}
//			pc.countEnd();

//			pc.countBegin("setup contact constraints");
			{
				PfxSetupContactConstraintsParam param;
				param.contactPairs = currentPairs;
				param.numContactPairs = numCurrentPairs;
				param.offsetContactManifolds = contacts;
				param.offsetRigidStates = states;
				param.offsetRigidBodies = bodies;
				param.offsetSolverBodies = solverBodies;
				param.numRigidBodies = numRigidBodies;
				param.timeStep = timeStep;
				param.separateBias = separateBias;

				int ret = pfxSetupContactConstraints(param);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxSetupContactConstraints failed %d\n", ret);
			}
//			pc.countEnd();

//			pc.countBegin("setup joint constraints");
			{
				PfxSetupJointConstraintsParam param;
				param.jointPairs = jointPairs;
				param.numJointPairs = numJoints;
				param.offsetJoints = joints;
				param.offsetRigidStates = states;
				param.offsetRigidBodies = bodies;
				param.offsetSolverBodies = solverBodies;
				param.numRigidBodies = numRigidBodies;
				param.timeStep = timeStep;

				for (int i = 0; i<numJoints; i++) {
					pfxUpdateJointPairs(jointPairs[i], i, joints[i], states[joints[i].m_rigidBodyIdA], states[joints[i].m_rigidBodyIdB]);
				}

				int ret = pfxSetupJointConstraints(param);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxSetupJointConstraints failed %d\n", ret);
			}
//			pc.countEnd();

//			pc.countBegin("solve constraints");
			{
				PfxSolveConstraintsParam param;
				param.workBytes = pfxGetWorkBytesOfSolveConstraints(numRigidBodies, numCurrentPairs, numJoints);
				param.workBuff = pool.allocate(param.workBytes);
				param.contactPairs = currentPairs;
				param.numContactPairs = numCurrentPairs;
				param.offsetContactManifolds = contacts;
				param.jointPairs = jointPairs;
				param.numJointPairs = numJoints;
				param.offsetJoints = joints;
				param.offsetRigidStates = states;
				param.offsetSolverBodies = solverBodies;
				param.numRigidBodies = numRigidBodies;
				param.iteration = iteration;

				int ret = pfxSolveConstraints(param);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxSolveConstraints failed %d\n", ret);

				pool.deallocate(param.workBuff);
			}
//			pc.countEnd();

			//pc.printCount();
		}

		void integrate()
		{
			PfxUpdateRigidStatesParam param;
			param.states = states;
			param.bodies = bodies;
			param.numRigidBodies = numRigidBodies;
			param.timeStep = timeStep;

			pfxUpdateRigidStates(param);
		}


	}

	using namespace basecross::ps;

	struct PxBoxParam {
		PsMotionType m_MotionType;
		PfxVector3 m_Size;
		PfxQuat m_Quat;
		PfxVector3 m_Pos;
		float m_Mass;
		PxBoxParam() {}
		PxBoxParam(const PsBoxParam& param) {
			m_MotionType = param.m_MotionType;
			m_Size.setX(param.m_HalfSize.x);
			m_Size.setY(param.m_HalfSize.y);
			m_Size.setZ(param.m_HalfSize.z);
			m_Quat.setX(param.m_Quat.x);
			m_Quat.setY(param.m_Quat.y);
			m_Quat.setZ(param.m_Quat.z);
			m_Quat.setW(param.m_Quat.w);
			m_Pos.setX(param.m_Pos.x);
			m_Pos.setY(param.m_Pos.y);
			m_Pos.setZ(param.m_Pos.z);
			m_Mass = param.m_Mass;
		}
	};

	struct PxSphereParam {
		PsMotionType m_MotionType;
		float m_Radius;
		PfxQuat m_Quat;
		PfxVector3 m_Pos;
		float m_Mass;
		PxSphereParam() {}
		PxSphereParam(const PsSphereParam& param) {
			m_MotionType = param.m_MotionType;
			m_Radius = param.m_Radius;
			m_Quat.setX(param.m_Quat.x);
			m_Quat.setY(param.m_Quat.y);
			m_Quat.setZ(param.m_Quat.z);
			m_Quat.setW(param.m_Quat.w);
			m_Pos.setX(param.m_Pos.x);
			m_Pos.setY(param.m_Pos.y);
			m_Pos.setZ(param.m_Pos.z);
			m_Mass = param.m_Mass;
		}
	};


	//--------------------------------------------------------------------------------------
	///	�����I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	PhysicsObject::PhysicsObject() {}
	PhysicsObject::~PhysicsObject() {}

	//--------------------------------------------------------------------------------------
	//	Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PhysicsBox::Impl {
		//�������p�����[�^
		PxBoxParam m_PxBoxParam;
		Impl(const PsBoxParam& param):
			m_PxBoxParam(param)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	�{�b�N�X�����I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	PhysicsBox::PhysicsBox(const PsBoxParam& param):
		pImpl(new Impl(param))
	{

	}

	PhysicsBox::~PhysicsBox() {}

	void PhysicsBox::OnCreate() {
		m_Index = numRigidBodies++;
		PfxBox box(pImpl->m_PxBoxParam.m_Size);
		PfxShape shape;
		shape.reset();
		shape.setBox(box);
		collidables[m_Index].reset();
		collidables[m_Index].addShape(shape);
		collidables[m_Index].finish();
		bodies[m_Index].reset();
		bodies[m_Index].setMass(pImpl->m_PxBoxParam.m_Mass);
		bodies[m_Index].setInertia(pfxCalcInertiaBox(pImpl->m_PxBoxParam.m_Size, pImpl->m_PxBoxParam.m_Mass));
		states[m_Index].reset();
		states[m_Index].setPosition(pImpl->m_PxBoxParam.m_Pos);
		states[m_Index].setOrientation(pImpl->m_PxBoxParam.m_Quat);
		states[m_Index].setMotionType((ePfxMotionType)pImpl->m_PxBoxParam.m_MotionType);
		states[m_Index].setRigidBodyId(m_Index);
	}

	//--------------------------------------------------------------------------------------
	//	Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PhysicsSphere::Impl {
		//�������p�����[�^
		PxSphereParam m_PxSphereParam;
		Impl(const PsSphereParam& param) :
			m_PxSphereParam(param)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	���̕����I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	PhysicsSphere::PhysicsSphere(const PsSphereParam& param) :
		pImpl(new Impl(param))
	{

	}

	PhysicsSphere::~PhysicsSphere() {}

	void PhysicsSphere::OnCreate() {
		m_Index = numRigidBodies++;
		PfxSphere sphere(pImpl->m_PxSphereParam.m_Radius);
		PfxShape shape;
		shape.reset();
		shape.setSphere(sphere);
		collidables[m_Index].reset();
		collidables[m_Index].addShape(shape);
		collidables[m_Index].finish();
		bodies[m_Index].reset();
		bodies[m_Index].setMass(pImpl->m_PxSphereParam.m_Mass);
		bodies[m_Index].setInertia(pfxCalcInertiaSphere(pImpl->m_PxSphereParam.m_Radius, pImpl->m_PxSphereParam.m_Mass));
		states[m_Index].reset();
		states[m_Index].setPosition(pImpl->m_PxSphereParam.m_Pos);
		states[m_Index].setOrientation(pImpl->m_PxSphereParam.m_Quat);
		states[m_Index].setMotionType((ePfxMotionType)pImpl->m_PxSphereParam.m_MotionType);
		states[m_Index].setRigidBodyId(m_Index);
	}


	//--------------------------------------------------------------------------------------
	///	�����v�Z�p�̃C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	BasePhysics::BasePhysics()
	{
	}
	BasePhysics::~BasePhysics() {}

	shared_ptr<PhysicsBox> BasePhysics::AddSingleBox(const PsBoxParam& param) {
		return ObjectFactory::Create<PhysicsBox>(param);
	}

	shared_ptr<PhysicsSphere> BasePhysics::AddSingleSphere(const PsSphereParam& param) {
		return ObjectFactory::Create<PhysicsSphere>(param);
	}

	uint16_t BasePhysics::GetNumBodies() const {
		return numRigidBodies;
	}

	void BasePhysics::GetBodyWorldQuatPos(uint16_t index, bsm::Quat& qt, bsm::Vec3& pos) {
		auto& srcQt = states[index].getOrientation();
		qt.x = srcQt.getX();
		qt.y = srcQt.getY();
		qt.z = srcQt.getZ();
		qt.w = srcQt.getW();
		auto& srcPos = states[index].getPosition();
		pos.x = srcPos.getX();
		pos.y = srcPos.getY();
		pos.z = srcPos.getZ();
	}

	uint16_t BasePhysics::GetNumShapes(uint16_t body_index) {
		return (uint16_t)collidables[body_index].getNumShapes();
	}

	void BasePhysics::GetShapeOffsetQuatPos(uint16_t body_index, uint16_t shape_index, bsm::Quat& qt, bsm::Vec3& pos) {
		auto& shape = collidables[body_index].getShape(shape_index);
		auto offpos = shape.getOffsetPosition();
		pos = bsm::Vec3(offpos.getX(), offpos.getY(), offpos.getZ());
		auto offor = shape.getOffsetOrientation();
		qt = bsm::Quat(offor.getX(), offor.getY(), offor.getZ(), offor.getW());
	}

	ePfxShapeType BasePhysics::GetShapeType(uint16_t body_index, uint16_t shape_index)const {
		auto& shape = collidables[body_index].getShape(shape_index);
		return (ePfxShapeType)shape.getType();
	}

	bsm::Vec3 BasePhysics::GetShapeBoxScale(uint16_t body_index, uint16_t shape_index) const {
		auto& shape = collidables[body_index].getShape(shape_index);
		bsm::Vec3 scale;
		scale.x = shape.getBox().m_half.getX();
		scale.y = shape.getBox().m_half.getY();
		scale.z = shape.getBox().m_half.getZ();
		return scale;
	}

	float BasePhysics::GetShapeSphereRadius(uint16_t body_index, uint16_t shape_index) const {
		auto& shape = collidables[body_index].getShape(shape_index);
		return (float)shape.getSphere().m_radius;
	}




	void BasePhysics::Reset() {
		numRigidBodies = 0;
		numJoints = 0;
		numContacts = 0;
		pairSwap = 0;
		numPairs[0] = 0;
		numPairs[1] = 0;
		numContactIdPool = 0;
		::ZeroMemory(states, sizeof(PfxRigidState) * NUM_RIGIDBODIES);
		::ZeroMemory(bodies, sizeof(PfxRigidBody) * NUM_RIGIDBODIES);
		::ZeroMemory(collidables, sizeof(PfxCollidable) * NUM_RIGIDBODIES);
		::ZeroMemory(solverBodies, sizeof(PfxSolverBody) * NUM_RIGIDBODIES);
		::ZeroMemory(proxies, sizeof(PfxBroadphaseProxy) * NUM_RIGIDBODIES);
		::ZeroMemory(jointPairs, sizeof(PfxConstraintPair) * NUM_JOINTS);
		::ZeroMemory(joints, sizeof(PfxJoint) * NUM_JOINTS);
		::ZeroMemory(pairsBuff, sizeof(PfxBroadphasePair) * NUM_CONTACTS * 2);

		::ZeroMemory(contacts, sizeof(PfxContactManifold) * NUM_CONTACTS);
		::ZeroMemory(contactIdPool, sizeof(PfxUInt32) * NUM_CONTACTS);
		::ZeroMemory(poolBuff, sizeof(poolBuff));
	}






	void BasePhysics::Update() {

//		PfxPerfCounter pc;

		for (int i = 1; i<numRigidBodies; i++) {
			pfxApplyExternalForce(
				states[i], bodies[i], 
				bodies[i].getMass() * PsGravity,
				PfxVector3(0.0f),
				timeStep
			);
		}

//		perfPushMarker("broadphase");
//		pc.countBegin("broadphase");
		broadphase();
//		pc.countEnd();
//		perfPopMarker();

//		perfPushMarker("collision");
//		pc.countBegin("collision");
		collision();
//		pc.countEnd();
//		perfPopMarker();

//		perfPushMarker("solver");
//		pc.countBegin("solver");
		constraintSolver();
//		pc.countEnd();
//		perfPopMarker();

//		perfPushMarker("integrate");
//		pc.countBegin("integrate");
		integrate();
//		pc.countEnd();
//		perfPopMarker();

//		frame++;

		//if (frame % 100 == 0) {
		//	float broadphaseTime = pc.getCountTime(0);
		//	float collisionTime = pc.getCountTime(2);
		//	float solverTime = pc.getCountTime(4);
		//	float integrateTime = pc.getCountTime(6);
		//	SCE_PFX_PRINTF("frame %3d broadphase %.2f collision %.2f solver %.2f integrate %.2f | total %.2f\n", frame,
		//		broadphaseTime, collisionTime, solverTime, integrateTime,
		//		broadphaseTime + collisionTime + solverTime + integrateTime);
		//}


	}





}
//end basecross