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

	void AttackBall::Weakup(const Vec3& Position, const Vec3& Velocity) {
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

		//CollisionSphere�Փ˔����t����
		auto PtrColl = AddComponent<CollisionSphere>();


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

		//CollisionSphere�Փ˔����t����
		auto PtrColl = AddComponent<CollisionSphere>();
		PtrColl->SetIsHitAction(IsHitAction::Auto);


		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//�`�悷��e�N�X�`����ݒ�
		PtrDraw->SetTextureResource(L"TRACE_TX");

		//��������
		SetAlphaActive(true);

		//�J�����𓾂�
		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(OnGetDrawCamera());
		if (PtrCamera) {
			//LookAtCamera�ł���
			//LookAtCamera�ɒ��ڂ���I�u�W�F�N�g�i�v���C���[�j�̐ݒ�
			PtrCamera->SetTargetObject(GetThis<GameObject>());
			PtrCamera->SetTargetToAt(Vec3(0, 0.25f, 0));
		}

		//A�{�^���̓A�^�b�N
		m_PlayerAction = PlayerAction::Attack;

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
		//������̕\��
		DrawStrings();
	}


	//A�{�^��
	void  Player::OnPushA() {
		if (GetStateMachine()->GetTopState() == PlayerDefaultState::Instance()) {
				GetStateMachine()->Push(PlayerAttackState::Instance());
		}
	}

	//������̕\��
	void Player::DrawStrings() {

		//������\��

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

		wstring RididStr(L"Velocity:\t");
		auto Velocity = GetComponent<Rigidbody>()->GetVelocity();
		RididStr += L"X=" + Util::FloatToWStr(Velocity.x, 6, Util::FloatModify::Fixed) + L",\t";
		RididStr += L"Y=" + Util::FloatToWStr(Velocity.y, 6, Util::FloatModify::Fixed) + L",\t";
		RididStr += L"Z=" + Util::FloatToWStr(Velocity.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring str = FPS + PositionStr + RididStr;
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
	}

	void PlayerDefaultState::Exit(const shared_ptr<Player>& Obj) {
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	///	�A�^�b�N�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerAttackState)

	void PlayerAttackState::Enter(const shared_ptr<Player>& Obj) {
		auto PtrBehavior = Obj->GetBehavior<PlayerBehavior>();
		PtrBehavior->FireAttackBall();
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

