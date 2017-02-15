/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	�G�P
	//--------------------------------------------------------------------------------------
	Enemy1::Enemy1(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos):
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
		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//Seek����
		auto PtrSeek = AddComponent<SeekSteering>();
		//Seek�͖����ɂ��Ă���
		PtrSeek->SetUpdateActive(false);
		//Arrive����
		auto PtrArrive = AddComponent<ArriveSteering>();
		//Arrive�͖����ɂ��Ă���
		PtrArrive->SetUpdateActive(false);

		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<GameObject>());
		//�����s��������
		AddComponent<SeparationSteering>(Group);
		//Obb�̏Փ˔��������
		auto PtrColl = AddComponent<CollisionObb>();
		//�������̂ݔ���
		PtrColl->SetIsHitAction(IsHitAction::AutoOnParentRepel);

		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();

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
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}



	//--------------------------------------------------------------------------------------
	///	Enemy1��Far�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy1FarState)

	void Enemy1FarState::Enter(const shared_ptr<Enemy1>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		PtrSeek->Enter(L"Player");
	}

	void Enemy1FarState::Execute(const shared_ptr<Enemy1>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		float f = PtrSeek->Execute(L"Player",true,0.1f);
		if (f < Obj->GetNearFarChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy1NearState::Instance());
		}
	}

	void Enemy1FarState::Exit(const shared_ptr<Enemy1>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		PtrSeek->Exit();
	}

	//--------------------------------------------------------------------------------------
	///	Enemy1��Near�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy1NearState)

	void Enemy1NearState::Enter(const shared_ptr<Enemy1>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveBehavior>();
		PtrArrive->Enter(L"Player");
	}

	void Enemy1NearState::Execute(const shared_ptr<Enemy1>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveBehavior>();
		if (PtrArrive->Execute(L"Player",true,0.1f) >= Obj->GetNearFarChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy1FarState::Instance());
		}
	}

	void Enemy1NearState::Exit(const shared_ptr<Enemy1>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveBehavior>();
		PtrArrive->Exit();
	}


	//--------------------------------------------------------------------------------------
	///	�G2
	//--------------------------------------------------------------------------------------
	Enemy2::Enemy2(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
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
		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//Seek����
		auto PtrSeek = AddComponent<SeekSteering>();
		//Seek�͖����ɂ��Ă���
		PtrSeek->SetUpdateActive(false);
		//Arrive����
		auto PtrArrive = AddComponent<ArriveSteering>();
		//Arrive�͖����ɂ��Ă���
		PtrArrive->SetUpdateActive(false);

		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<GameObject>());
		//�����s��������
		AddComponent<SeparationSteering>(Group);
		auto PtrColl = AddComponent<CollisionSphere>();
		//�������̂ݔ���
		PtrColl->SetIsHitAction(IsHitAction::AutoOnParentRepel);

		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();

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
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	//--------------------------------------------------------------------------------------
	///	Enemy2��long�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy2longState)

	void Enemy2longState::Enter(const shared_ptr<Enemy2>& Obj) {
		auto PtrWait = Obj->GetBehavior<WaitBehavior>();
		PtrWait->Enter();
	}

	void Enemy2longState::Execute(const shared_ptr<Enemy2>& Obj) {
		auto PtrWait = Obj->GetBehavior<WaitBehavior>();
		float f = PtrWait->Execute();
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
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		PtrSeek->Enter(L"Player");
		auto PtrJump = Obj->GetBehavior<JumpBehavior>();
		PtrJump->StartJump(Vector3(0, 4.0f, 0));
	}

	void Enemy2MediumState::Execute(const shared_ptr<Enemy2>& Obj) {
		auto PtrUtil = Obj->GetBehavior<UtilBehavior>();
		auto PtrJump = Obj->GetBehavior<JumpBehavior>();
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		if (PtrJump->Execute()) {
			PtrJump->StartJump(Vector3(0, 4.0f, 0));
		}
		float f = PtrSeek->Execute(L"Player",true,0.1f);
		if (f < Obj->GetMediumShortChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy2ShortState::Instance());
		}
		else if (f >= Obj->GetLongMediumChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy2longState::Instance());
		}
	}

	void Enemy2MediumState::Exit(const shared_ptr<Enemy2>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		PtrSeek->Exit();
	}

	//--------------------------------------------------------------------------------------
	///	Enemy2��Short�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy2ShortState)

	void Enemy2ShortState::Enter(const shared_ptr<Enemy2>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveBehavior>();
		PtrArrive->Enter(L"Player");
	}

	void Enemy2ShortState::Execute(const shared_ptr<Enemy2>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveBehavior>();
		if (PtrArrive->Execute(L"Player",true,0.1f) >= Obj->GetMediumShortChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy2MediumState::Instance());
		}
	}

	void Enemy2ShortState::Exit(const shared_ptr<Enemy2>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveBehavior>();
		PtrArrive->Exit();
	}

	//--------------------------------------------------------------------------------------
	///	�G�R
	//--------------------------------------------------------------------------------------
	Enemy3::Enemy3(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
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
		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//Seek����
		auto PtrSeek = AddComponent<SeekSteering>();

		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<GameObject>());
		//�����s��������
		AddComponent<SeparationSteering>(Group);
		auto PtrColl = AddComponent<CollisionSphere>();
		//�������̂ݔ���
		PtrColl->SetIsHitAction(IsHitAction::AutoOnParentRepel);

		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();

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
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	//--------------------------------------------------------------------------------------
	///	Enemy3��Default�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy3DefaultState)

	void Enemy3DefaultState::Enter(const shared_ptr<Enemy3>& Obj) {
		//�������Ȃ�
	}

	void Enemy3DefaultState::Execute(const shared_ptr<Enemy3>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		float f = PtrSeek->Execute(L"Player", true, 0.1f);
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
		auto PtrJump = Obj->GetBehavior<JumpBehavior>();
		PtrJump->StartJump(Vector3(0, 3.0f, 0));
	}

	void Enemy3NearState::Execute(const shared_ptr<Enemy3>& Obj) {
		auto PtrJump = Obj->GetBehavior<JumpBehavior>();
		if (PtrJump->Execute()) {
			PtrJump->StartJump(Vector3(0, 3.0f, 0));
		}
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		float f = PtrSeek->Execute(L"Player", true, 0.1f);
		if (f >= Obj->GetDefaultNearChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy3DefaultState::Instance());
		}
	}

	void Enemy3NearState::Exit(const shared_ptr<Enemy3>& Obj) {
		//�������Ȃ�
	}



}
//end basecross
