/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	Tree::Tree(const shared_ptr<Stage>& StagePtr, const Vec3& Pos):
		GameObject(StagePtr),
		m_Pos(Pos)
	{}


	void Tree::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_Pos);
		PtrTransform->SetScale(0.5f, 1.0f, 0.5f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//capsule�̏Փ˔��������
		auto PtrColl = AddComponent<CollisionCapsule>();
		PtrColl->SetFixed(true);
//		PtrColl->SetDrawActive(true);

		shared_ptr<MeshResource> TreeMesh, LeafMesh1, LeafMesh2;
		if (!App::GetApp()->CheckResource<MeshResource>(L"TREE_MESH")) {
			TreeMesh =MeshResource::CreateCylinder(1.0f, 1.0f, 18, false);
			Mat4x4 TransToMat;
			TransToMat.affineTransformation(
				Vec3(1.0f,1.0f,1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f)
				);
			TreeMesh->SetMeshToTransformMatrix(TransToMat);
			TreeMesh->SetUseMeshToTransformMatrix(true);
			App::GetApp()->RegisterResource(L"TREE_MESH", TreeMesh);
		}
		else {
			TreeMesh = App::GetApp()->GetResource<MeshResource>(L"TREE_MESH");
		}
		//���̃��b�V�����L�̃e�N�X�`���Ƃ��ēo�^
		TreeMesh->SetTextureResource(L"BROWN_TX");


		if (!App::GetApp()->CheckResource<MeshResource>(L"LEAF_MESH_1")) {
			LeafMesh1 = MeshResource::CreateCone(1.0f, 1.0f,18,false);
			Mat4x4 TransToMat;
			TransToMat.affineTransformation(
				Vec3(3.0f, 0.5f, 3.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.4f, 0.0f)
			);
			LeafMesh1->SetMeshToTransformMatrix(TransToMat);
			LeafMesh1->SetUseMeshToTransformMatrix(true);
			App::GetApp()->RegisterResource(L"LEAF_MESH_1", LeafMesh1);
		}
		else {
			LeafMesh1 = App::GetApp()->GetResource<MeshResource>(L"LEAF_MESH_1");
		}
		//���̃��b�V�����L�̃e�N�X�`���Ƃ��ēo�^
		LeafMesh1->SetTextureResource(L"GREEN_TX");

		if (!App::GetApp()->CheckResource<MeshResource>(L"LEAF_MESH_2")) {
			LeafMesh2 = MeshResource::CreateCone(1.0f, 1.0f, 18, false);
			Mat4x4 TransToMat;
			TransToMat.affineTransformation(
				Vec3(2.0f, 0.5f, 2.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.7f, 0.0f)
			);
			LeafMesh2->SetMeshToTransformMatrix(TransToMat);
			LeafMesh2->SetUseMeshToTransformMatrix(true);
			App::GetApp()->RegisterResource(L"LEAF_MESH_2", LeafMesh2);
		}
		else {
			LeafMesh2 = App::GetApp()->GetResource<MeshResource>(L"LEAF_MESH_2");
		}
		//���̃��b�V�����L�̃e�N�X�`���Ƃ��ēo�^
		LeafMesh2->SetTextureResource(L"GREEN_TX");

		//���̃I�u�W�F�N�g�̓}���`���b�V�����g��
		m_MultiMeshResource = ObjectFactory::Create<MultiMeshResource>();
		m_MultiMeshResource->AddMesh(TreeMesh);
		m_MultiMeshResource->AddMesh(LeafMesh1);
		m_MultiMeshResource->AddMesh(LeafMesh2);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMultiMeshResource(m_MultiMeshResource);

		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷��}���`���b�V����ݒ�
		PtrDraw->SetMultiMeshResource(m_MultiMeshResource);

	}



	//--------------------------------------------------------------------------------------
	///	�G�P
	//--------------------------------------------------------------------------------------
	Enemy1::Enemy1(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos):
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_NearFarChange(5.0f)
	{}

	//������
	void Enemy1::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.125f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//���ǌn�̍s��������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();

		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<GameObject>());
		//�����s��������
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->SetGameObjectGroup(Group);
		//Obb�̏Փ˔��������
		auto PtrColl = AddComponent<CollisionObb>();
		//�������̂ݔ���
		PtrColl->SetIsHitAction(IsHitAction::Auto);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//�`�悷��e�N�X�`����ݒ�
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//��������
		SetAlphaActive(true);

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<Enemy1>(GetThis<Enemy1>()));
		//�ŏ��̃X�e�[�g��Enemy1FarState�ɐݒ�
		m_StateMachine->ChangeState(Enemy1FarState::Instance());
	}

	void Enemy1::OnUpdate() {
		//�X�e�[�g�ɂ���ĕς��Ȃ��s�������s
		auto PtrGrav = GetBehavior<Gravity>();
		PtrGrav->Execute();
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->Execute();
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	void Enemy1::OnUpdate2() {
		auto PtrUtil = GetBehavior<UtilBehavior>();
		PtrUtil->RotToHead(0.2f);
	}


	//--------------------------------------------------------------------------------------
	///	Enemy1��Far�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy1FarState)

	void Enemy1FarState::Enter(const shared_ptr<Enemy1>& Obj) {
	}

	void Enemy1FarState::Execute(const shared_ptr<Enemy1>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekSteering>();
		float f = PtrSeek->Execute(L"Player");
		if (f < Obj->GetNearFarChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy1NearState::Instance());
		}
	}

	void Enemy1FarState::Exit(const shared_ptr<Enemy1>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	Enemy1��Near�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy1NearState)

	void Enemy1NearState::Enter(const shared_ptr<Enemy1>& Obj) {
	}

	void Enemy1NearState::Execute(const shared_ptr<Enemy1>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveSteering>();
		if (PtrArrive->Execute(L"Player") >= Obj->GetNearFarChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy1FarState::Instance());
		}
	}

	void Enemy1NearState::Exit(const shared_ptr<Enemy1>& Obj) {
	}


	//--------------------------------------------------------------------------------------
	///	�G2
	//--------------------------------------------------------------------------------------
	Enemy2::Enemy2(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_LongMediumChange(10.0f),
		m_MediumShortChange(5.0f)
	{}

	//������
	void Enemy2::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.25f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//���ǌn�̍s��������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();

		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<GameObject>());
		//�����s��������
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->SetGameObjectGroup(Group);

		auto PtrColl = AddComponent<CollisionSphere>();
		//�������̂ݔ���
		PtrColl->SetIsHitAction(IsHitAction::Auto);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//�`�悷��e�N�X�`����ݒ�
		PtrDraw->SetTextureResource(L"TRACE2_TX");
		//��������
		SetAlphaActive(true);


		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<Enemy2>(GetThis<Enemy2>()));
		//�ŏ��̃X�e�[�g��Enemy2longState�ɐݒ�
		m_StateMachine->ChangeState(Enemy2longState::Instance());
	}

	void Enemy2::OnUpdate() {
		//�X�e�[�g�ɂ���ĕς��Ȃ��s�������s
		auto PtrGrav = GetBehavior<Gravity>();
		PtrGrav->Execute();
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->Execute();
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	void Enemy2::OnUpdate2() {
		auto PtrUtil = GetBehavior<UtilBehavior>();
		PtrUtil->RotToHead(0.1f);
	}

	//�Փˎ�
	void Enemy2::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
		if (m_StateMachine->GetCurrentState() == Enemy2MediumState::Instance()) {
			auto PtrGrav = GetBehavior<Gravity>();
			PtrGrav->StartJump(Vec3(0, 4.0f, 0));
		}
	}



	//--------------------------------------------------------------------------------------
	///	Enemy2��long�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy2longState)

	void Enemy2longState::Enter(const shared_ptr<Enemy2>& Obj) {
	}

	void Enemy2longState::Execute(const shared_ptr<Enemy2>& Obj) {
		auto PtrWait = Obj->GetBehavior<WaitBehavior>();
		float f = PtrWait->Execute(L"Player");
		if (f < Obj->GetLongMediumChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy2MediumState::Instance());
		}
	}

	void Enemy2longState::Exit(const shared_ptr<Enemy2>& Obj) {
		//�������Ȃ�
	}



	//--------------------------------------------------------------------------------------
	///	Enemy2��Medium�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy2MediumState)

	void Enemy2MediumState::Enter(const shared_ptr<Enemy2>& Obj) {
		auto PtrGrav = Obj->GetBehavior<Gravity>();
		PtrGrav->StartJump(Vec3(0, 4.0f, 0));
	}

	void Enemy2MediumState::Execute(const shared_ptr<Enemy2>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekSteering>();
		float f = PtrSeek->Execute(L"Player");
		if (f < Obj->GetMediumShortChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy2ShortState::Instance());
		}
		else if (f >= Obj->GetLongMediumChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy2longState::Instance());
		}
	}

	void Enemy2MediumState::Exit(const shared_ptr<Enemy2>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	Enemy2��Short�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy2ShortState)

	void Enemy2ShortState::Enter(const shared_ptr<Enemy2>& Obj) {
	}

	void Enemy2ShortState::Execute(const shared_ptr<Enemy2>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveSteering>();
		if (PtrArrive->Execute(L"Player") >= Obj->GetMediumShortChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy2MediumState::Instance());
		}
	}

	void Enemy2ShortState::Exit(const shared_ptr<Enemy2>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	�G�R
	//--------------------------------------------------------------------------------------
	Enemy3::Enemy3(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_DefaultNearChange(7.0f)
	{}

	//������
	void Enemy3::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.25f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//���ǌn�̍s��������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();

		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<GameObject>());
		//�����s��������
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->SetGameObjectGroup(Group);

		auto PtrColl = AddComponent<CollisionSphere>();
		//�������̂ݔ���
		PtrColl->SetIsHitAction(IsHitAction::Auto);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//�`�悷��e�N�X�`����ݒ�
		PtrDraw->SetTextureResource(L"WALL_TX");
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<Enemy3>(GetThis<Enemy3>()));
		//�ŏ��̃X�e�[�g��Enemy3DefaultState�ɐݒ�
		m_StateMachine->ChangeState(Enemy3DefaultState::Instance());
	}

	void Enemy3::OnUpdate() {
		//�X�e�[�g�ɂ���ĕς��Ȃ��s�������s
		auto PtrGrav = GetBehavior<Gravity>();
		PtrGrav->Execute();
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->Execute();
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	void Enemy3::OnUpdate2() {
		auto PtrUtil = GetBehavior<UtilBehavior>();
		PtrUtil->RotToHead(0.1f);
	}


	//--------------------------------------------------------------------------------------
	///	Enemy3��Default�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy3DefaultState)

	void Enemy3DefaultState::Enter(const shared_ptr<Enemy3>& Obj) {
		//�������Ȃ�
	}

	void Enemy3DefaultState::Execute(const shared_ptr<Enemy3>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekSteering>();
		float f = PtrSeek->Execute(L"Player");
		if (f < Obj->GetDefaultNearChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy3NearState::Instance());
		}
	}

	void Enemy3DefaultState::Exit(const shared_ptr<Enemy3>& Obj) {
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	///	Enemy3��Near�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy3NearState)

	void Enemy3NearState::Enter(const shared_ptr<Enemy3>& Obj) {
		auto PtrGrav = Obj->GetBehavior<Gravity>();
		PtrGrav->StartJump(Vec3(0, 4.0f, 0));
	}

	void Enemy3NearState::Execute(const shared_ptr<Enemy3>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveSteering>();
		if (PtrArrive->Execute(L"Player") >= Obj->GetDefaultNearChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy3DefaultState::Instance());
		}
	}

	void Enemy3NearState::Exit(const shared_ptr<Enemy3>& Obj) {
		//�������Ȃ�
	}


}
//end basecross
