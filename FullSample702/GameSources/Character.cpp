/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�����v�Z����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedPsBox::FixedPsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	):
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	FixedPsBox::~FixedPsBox(){}
	//������
	void FixedPsBox::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//�����v�Z�{�b�N�X
		PsBoxParam param;
		//DEFAULT_CUBE�̃X�P�[�����O�͊e�ӊ�Ȃ̂ŁA�n�[�t�T�C�Y�ɂ���
		param.m_HalfSize = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeFixed;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<PsSingleBoxBody>(param);
		PsPtr->SetDrawActive(true);
	}

	//--------------------------------------------------------------------------------------
	///	�ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_StateChangeSize(3.0f),
		m_MaxForce(30.0f),
		m_MaxSpeed(10.0f),
		m_Decl(3.0f)
	{
	}
	SeekObject::~SeekObject() {}

	//������
	void SeekObject::OnCreate() {
		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"SeekObjectGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<SeekObject>());
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<SeekObject>(GetThis<SeekObject>()));
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		m_StateMachine->ChangeState(FarState::Instance());
	}

	//����
	void SeekObject::OnUpdate() {
		//�t�H�[�X�̏�����
		m_Force = Vec3(0);
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	float SeekObject::GetTargetToLen() const {
		auto Pos = GetComponent<Transform>()->GetPosition();
		auto TargetTrans = GetStage()->GetSharedObject(L"Player")->GetComponent<Transform>();
		auto TargetPos = TargetTrans->GetPosition();
		return length(TargetPos - Pos);
	}

	void SeekObject::SeekBehavior() {
		auto Pos = GetComponent<Transform>()->GetPosition();
		auto TargetTrans = GetStage()->GetSharedObject(L"Player")->GetComponent<Transform>();
		auto TargetPos = TargetTrans->GetPosition();
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Seek(GetVelocity(), TargetPos, Pos, m_MaxSpeed);
		Steering::AccumulateForce(m_Force, WorkForce, m_MaxForce);
	}

	void SeekObject::ArriveBehavior() {
		auto Pos = GetComponent<Transform>()->GetPosition();
		auto TargetTrans = GetStage()->GetSharedObject(L"Player")->GetComponent<Transform>();
		auto TargetPos = TargetTrans->GetPosition();
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Arrive(GetVelocity(), TargetPos,
			Pos, m_MaxSpeed, m_Decl);
		Steering::AccumulateForce(m_Force, WorkForce, m_MaxForce);
	}


	void SeekObject::SeparationBehavior() {
		auto Group = GetStage()->GetSharedObjectGroup(L"SeekObjectGroup");
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Separation(Group, GetThis<GameObject>());
		Steering::AccumulateForce(m_Force, WorkForce, m_MaxForce);
	}



	//--------------------------------------------------------------------------------------
	///	�v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<FarState> FarState::Instance() {
		static shared_ptr<FarState> instance(new FarState);
		return instance;
	}
	void FarState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj) {
		Obj->SeparationBehavior();
		Obj->SeekBehavior();
		if (Obj->GetTargetToLen() < Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void FarState::Exit(const shared_ptr<SeekObject>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	�v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<NearState> NearState::Instance() {
		static shared_ptr<NearState> instance(new NearState);
		return instance;
	}
	void NearState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj) {
		Obj->SeparationBehavior();
		Obj->ArriveBehavior();
		if (Obj->GetTargetToLen() >= Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȓʖʃI�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//static�ϐ��̏�����
	shared_ptr<MeshResource> ActivePsConvex::m_ConvexMesh = nullptr;
	shared_ptr<PsConvexMeshResource> ActivePsConvex::m_PsConvexMesh = nullptr;

	ActivePsConvex::ActivePsConvex(const shared_ptr<Stage>& StagePtr,
		const Vec3& Position
	) :
		SeekObject(StagePtr),
		m_Position(Position)
	{}
	ActivePsConvex::~ActivePsConvex() {}

	void ActivePsConvex::OnCreate() {
		if (!m_ConvexMesh || !m_PsConvexMesh) {
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateDodecahedron(0.5, vertices, indices);
			m_ConvexMesh = MeshResource::CreateMeshResource(vertices, indices, false);
			m_PsConvexMesh = ObjectFactory::Create<PsConvexMeshResource>(vertices, indices, 0.5f);
		}

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(Vec3(1.0f));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Position);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(m_ConvexMesh);

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(m_ConvexMesh);
		PtrDraw->SetTextureResource(L"WALL_TX");

		//�����v�Z�ʖ�
		PsConvexParam param;
		//���a�ɂ���
		param.m_ConvexMeshResource = m_PsConvexMesh;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = Quat();
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<PsConvexBody>(param);
		PsPtr->SetDrawActive(true);
		//�e�N���X��OnCreate���Ă�
		SeekObject::OnCreate();
	}

	Vec3 ActivePsConvex::GetVelocity()const {
		auto PtrPs = GetComponent<PsConvexBody>();
		return PtrPs->GetLinearVelocity();
	}

	//�X�V
	void ActivePsConvex::OnUpdate() {
		//�e�N���X��OnUpdate���Ă�
		SeekObject::OnUpdate();
		auto PtrPs = GetComponent<PsConvexBody>();
		//���݂̃t�H�[�X��ݒ�
		PtrPs->ApplyForce(GetForce());
	}




	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȋ���
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ActivePsSphere::ActivePsSphere(const shared_ptr<Stage>& StagePtr,
		float Scale,
		const Vec3& Position
	) :
		SeekObject(StagePtr),
		m_Scale(Scale),
		m_Position(Position)
	{}

	ActivePsSphere::~ActivePsSphere() {}
	//������
	void ActivePsSphere::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Position);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"WALL_TX");

		//�����v�Z����
		PsSphereParam param;
		//DEFAULT_SPHERE�̃X�P�[�����O�͒��a��Ȃ̂ŁA���a�ɂ���
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = Quat();
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<PsSingleSphereBody>(param);
		PsPtr->SetDrawActive(true);
		//�e�N���X��OnCreate���Ă�
		SeekObject::OnCreate();
	}

	Vec3 ActivePsSphere::GetVelocity()const {
		auto PtrPs = GetComponent<PsSingleSphereBody>();
		return PtrPs->GetLinearVelocity();
	}

	//�X�V
	void ActivePsSphere::OnUpdate() {
		//�e�N���X��OnUpdate���Ă�
		SeekObject::OnUpdate();
		auto PtrPs = GetComponent<PsSingleSphereBody>();
		//���݂̃t�H�[�X��ݒ�
		PtrPs->ApplyForce(GetForce());
	}


	//--------------------------------------------------------------------------------------
	///	�����v�Z���锭�˂��鋅��
	//--------------------------------------------------------------------------------------
	FirePsSphere::FirePsSphere(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity):
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity),
		m_Scale(0.25f)
	{}
	FirePsSphere::~FirePsSphere() {}

	void FirePsSphere::CreateDefParam(PsSphereParam& param) {
		//DEFAULT_SPHERE�̃X�P�[�����O�͒��a��Ȃ̂ŁA���a�ɂ���
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		//�X���[�v���Ȃ�
		param.m_UseSleep = false;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat.identity();
	}

	//������
	void FirePsSphere::OnCreate() {
		//���L�I�u�W�F�N�g�ɃZ�b�g
		GetStage()->SetSharedGameObject(L"FirePsSphere",GetThis<FirePsSphere>());

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Emitter);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"SKY_TX");

		PsSphereParam param;
		CreateDefParam(param);
		param.m_Pos = m_Emitter;
		param.m_LinearVelocity = m_Velocity;
		auto PsPtr = AddComponent<PsSingleSphereBody>(param);
		PsPtr->SetDrawActive(true);
	}


	void FirePsSphere::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		auto PsPtr = GetComponent<PsSingleSphereBody>();
		PsSphereParam param;
		CreateDefParam(param);
		param.m_Pos = Emitter;
		param.m_LinearVelocity = Velocity;
		PsPtr->Reset(param, PsPtr->GetIndex());
	}






}
//end basecross
