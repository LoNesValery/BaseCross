/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{



	//--------------------------------------------------------------------------------------
	//	class AttackBall : public GameObject;
	//	�p�r: ���ł����{�[��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	AttackBall::AttackBall(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr) {}

	AttackBall::~AttackBall() {}

	void AttackBall::Weakup(const Vector3& Position, const Vector3& Velocity) {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(0.1f, 0.1f, 0.1f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->ResetPosition(Position);
		auto PtrRedid = GetComponent<Rigidbody>();
		PtrRedid->SetVelocity(Velocity);
		SetDrawActive(true);
		SetUpdateActive(true);
	}


	//������
	void AttackBall::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(0.1f, 0.1f, 0.1f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(0, 0, 0);

		//Rigidbody������
		auto PtrRedid = AddComponent<Rigidbody>();


		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionSphere>();
		PtrCol->SetIsHitAction(IsHitAction::Repel);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");

		auto Group = GetStage()->GetSharedObjectGroup(L"AttackBall");
		Group->IntoGroup(GetThis<AttackBall>());

		//�ŏ��͖����ɂ��Ă���
		SetDrawActive(false);
		SetUpdateActive(false);

	}

	void AttackBall::OnUpdate() {
		Rect2D<float> rect(-25.0f, -25.0f, 25.0f, 25.0f);
		Point2D<float> point;
		auto PtrTransform = GetComponent<Transform>();
		point.x = PtrTransform->GetPosition().x;
		point.y = PtrTransform->GetPosition().z;
		if (!rect.PtInRect(point) || abs(PtrTransform->GetPosition().y) > 10.0f) {
			PtrTransform->SetScale(0.1f, 0.1f, 0.1f);
			PtrTransform->SetRotation(0, 0, 0);
			PtrTransform->SetPosition(0, 0, 0);
			SetDrawActive(false);
			SetUpdateActive(false);
		}
	}

	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	�p�r: �v���C���[
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}

	//������
	void Player::OnCreate() {

		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//���a25�Z���`�̋���
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.125f, 0);

		//Rigidbody������
		auto PtrRedid = AddComponent<Rigidbody>();
		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionSphere>();
		PtrCol->SetIsHitAction(IsHitAction::Auto);
		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

		Matrix4X4 SpanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		SpanMat.DefTransformation(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, XM_PI, 0.0f),
			Vector3(0.0f, -0.5f, 0.0f)
		);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();

		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"Chara_R_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTBoneModelDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"Chara_R_MESH");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);

		PtrDraw->AddAnimation(L"Default", 60, 20, true, 20.0f);
		PtrDraw->AddAnimation(L"Hit", 30, 30, false, 30.0f);
		PtrDraw->ChangeCurrentAnimation(L"Default");

		//��������
		SetAlphaActive(true);

		//�J�����𓾂�
		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(OnGetDrawCamera());
		if (PtrCamera) {
			//LookAtCamera�ł���
			//LookAtCamera�ɒ��ڂ���I�u�W�F�N�g�i�v���C���[�j�̐ݒ�
			PtrCamera->SetTargetObject(GetThis<GameObject>());
			PtrCamera->SetTargetToAt(Vector3(0, 0.25f, 0));
		}
		//�ŏ���A�{�^���̓W�����v
		m_PlayerAction = PlayerAction::Jump;
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new LayeredStateMachine<Player>(GetThis<Player>()));
		//�ŏ��̃X�e�[�g��PlayerDefault�Ƀ��Z�b�g
		m_StateMachine->Reset(PlayerDefaultState::Instance());
	}

	//�X�V
	void Player::OnUpdate() {
		//�R���g���[���`�F�b�N���ē��͂�����΃R�}���h�Ăяo��
		m_InputHandler.PushHandle(GetThis<Player>());
		//�X�e�[�g�}�V���X�V
		m_StateMachine->Update();
		//�A�j���[�V�������X�V����
		auto PtrDraw = GetComponent<BcPNTBoneModelDraw>();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		if (PtrDraw->UpdateAnimation(ElapsedTime) &&
			PtrDraw->GetCurrentAnimation() == L"Hit") {
			PtrDraw->ChangeCurrentAnimation(L"Default");
		}
	}

	//��X�V
	void Player::OnUpdate2() {
		//������̕\��
		DrawStrings();
	}


	void Player::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {

		auto PtrBehavior = GetBehavior<PlayerBehavior>();
		shared_ptr<GameObject> v;
		if (PtrBehavior->OnHitObjMoveBox(OtherVec, v)) {
			GetStateMachine()->Reset(PlayerOnMoveboxState::Instance());
			GetComponent<Transform>()->SetParent(v);
			return;
		}

		//�v���C���[�������ɓ�������
		if (GetStateMachine()->GetTopState() == PlayerJumpState::Instance()) {
			//���݂��W�����v�X�e�[�g���ړ��{�b�N�X�X�e�[�g�Ȃ�PlayerDefault�Ƀ��Z�b�g
			GetStateMachine()->Reset(PlayerDefaultState::Instance());
		}
	}

	//A�{�^��
	void  Player::OnPushA() {
		if (GetStateMachine()->GetTopState() == PlayerDefaultState::Instance() ||
			GetStateMachine()->GetTopState() == PlayerOnMoveboxState::Instance()) {
			switch (m_PlayerAction) {
			case PlayerAction::Jump:
				GetStateMachine()->Push(PlayerJumpState::Instance());
				break;
			case PlayerAction::Attack:
				GetStateMachine()->Push(PlayerAttackState::Instance());
				break;
			}
		}
	}

	//B�{�^��
	void  Player::OnPushB() {
		switch (m_PlayerAction) {
		case PlayerAction::Jump:
			m_PlayerAction = PlayerAction::Attack;
			break;
		case PlayerAction::Attack:
			m_PlayerAction = PlayerAction::Jump;
			break;
		}
	}



	//������̕\��
	void Player::DrawStrings() {

		//������\��
		//�s��
		wstring BEHAVIOR;
		if (m_PlayerAction == PlayerAction::Jump) {
			BEHAVIOR = L"DEFAULT�s��: A�{�^���ŃW�����v�BB�{�^���ōs���؂�ւ�\n";
		}
		else {
			BEHAVIOR = L"ATTACK�s��: A�{�^���ŃA�^�b�N�BB�{�^���ōs���؂�ւ�\n";
		}


		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\nElapsedTime: ";
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		FPS += Util::FloatToWStr(ElapsedTime);
		FPS += L"\n";

		auto Pos = GetComponent<Transform>()->GetPosition();
		wstring PositionStr(L"Position:\t");
		PositionStr += L"X=" + Util::FloatToWStr(Pos.x, 6, Util::FloatModify::Fixed) + L",\t";
		PositionStr += L"Y=" + Util::FloatToWStr(Pos.y, 6, Util::FloatModify::Fixed) + L",\t";
		PositionStr += L"Z=" + Util::FloatToWStr(Pos.z, 6, Util::FloatModify::Fixed) + L"\n";
		if (GetComponent<Transform>()->GetParent()) {
			PositionStr += L"OnParent\n";
		}
		else {
			PositionStr += L"NotParent\n";
		}

		wstring RididStr(L"Velocity:\t");
		auto Velocity = GetComponent<Rigidbody>()->GetVelocity();
		RididStr += L"X=" + Util::FloatToWStr(Velocity.x, 6, Util::FloatModify::Fixed) + L",\t";
		RididStr += L"Y=" + Util::FloatToWStr(Velocity.y, 6, Util::FloatModify::Fixed) + L",\t";
		RididStr += L"Z=" + Util::FloatToWStr(Velocity.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring StateStr(L"State: ");
		StateStr += GetStateMachine()->GetTopState()->GetStateName() + L"\n";

		wstring HitObjectStr(L"HitObject: ");
		if (GetComponent<Collision>()->GetHitObjectVec().size() > 0) {
			for (auto&v : GetComponent<Collision>()->GetHitObjectVec()) {
				HitObjectStr += Util::UintToWStr((UINT)v.get()) + L",";
			}
			HitObjectStr += L"\n";
		}
		else {
			HitObjectStr += L"NULL\n";
		}
		wstring str = BEHAVIOR + FPS + PositionStr + RididStr + StateStr + HitObjectStr;
		//�����������
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}


	//--------------------------------------------------------------------------------------
	///	�ʏ�X�e�[�g
	//--------------------------------------------------------------------------------------

	IMPLEMENT_SINGLETON_INSTANCE(PlayerDefaultState)

	void PlayerDefaultState::Enter(const shared_ptr<Player>& Obj) {
		//�������Ȃ�
	}

	void PlayerDefaultState::Execute(const shared_ptr<Player>& Obj) {
		auto PtrBehavior = Obj->GetBehavior<PlayerBehavior>();
		PtrBehavior->MovePlayer();
		auto PtrGrav = Obj->GetBehavior<Gravity>();
		PtrGrav->Execute();
	}

	void PlayerDefaultState::Exit(const shared_ptr<Player>& Obj) {
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	///	MoveBox�ɏ���Ă���X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerOnMoveboxState)
	void PlayerOnMoveboxState::Enter(const shared_ptr<Player>& Obj) {
		Obj->GetComponent<CollisionSphere>()->SetUpdateActive(false);
	}

	void PlayerOnMoveboxState::Execute(const shared_ptr<Player>& Obj) {
		auto PtrBehavior = Obj->GetBehavior<PlayerBehavior>();
		PtrBehavior->MovePlayer();
		if (!PtrBehavior->OnMoveBox()) {
			Obj->GetStateMachine()->Push(PlayerJumpState::Instance());
		}
	}

	void PlayerOnMoveboxState::Sleep(const shared_ptr<Player>& Obj) {
		Obj->GetComponent<CollisionSphere>()->SetUpdateActive(true);
	}

	void PlayerOnMoveboxState::Exit(const shared_ptr<Player>& Obj) {
		Obj->GetComponent<CollisionSphere>()->SetUpdateActive(true);
		Obj->GetComponent<Transform>()->SetParent(nullptr);
	}

	//--------------------------------------------------------------------------------------
	///	�W�����v�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerJumpState)

	void PlayerJumpState::Enter(const shared_ptr<Player>& Obj) {
		auto PtrGrav = Obj->GetBehavior<Gravity>();
		PtrGrav->StartJump(Vector3(0, 4.0f, 0));
	}

	void PlayerJumpState::Execute(const shared_ptr<Player>& Obj) {
		//�W�����v���������ύX�\
		auto PtrBehavior = Obj->GetBehavior<PlayerBehavior>();
		PtrBehavior->MovePlayer();
		auto PtrGrav = Obj->GetBehavior<Gravity>();
		PtrGrav->Execute();
	}

	void PlayerJumpState::Exit(const shared_ptr<Player>& Obj) {
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	///	�A�^�b�N�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerAttackState)

	void PlayerAttackState::Enter(const shared_ptr<Player>& Obj) {
		auto PtrBehavior = Obj->GetBehavior<PlayerBehavior>();
		PtrBehavior->FireAttackBall();
		//�A�j���[�V������ύX����
		auto PtrDraw = Obj->GetComponent<BcPNTBoneModelDraw>();
		PtrDraw->ChangeCurrentAnimation(L"Hit");
	}

	void PlayerAttackState::Execute(const shared_ptr<Player>& Obj) {
		//�����ɃX�e�[�g��߂�
		Obj->GetStateMachine()->Pop();
	}

	void PlayerAttackState::Exit(const shared_ptr<Player>& Obj) {
		//�������Ȃ�
	}



}
//end basecross

