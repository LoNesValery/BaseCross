/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	�p�r: �v���C���[
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_StartPosition(0.0f, 0.5f, 0.0f)
	{}

	//������
	void Player::OnCreate() {

		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(0.5f, 0.5f, 0.5f);	//���a25�Z���`�̋���
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPosition);
		AddTag(L"Player");

		//Rigidbody������
		auto PtrRedid = AddComponent<Rigidbody>();
		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionCapsule>();
		PtrCol->SetIsHitAction(IsHitAction::Slide);
		PtrCol->AddExcludeCollisionTag(L"PlayerSword");

		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_CAPSULE");
		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_CAPSULE");
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
		//�킢�̍s���̍\�z
		m_FightBehavior.reset(new FightBehavior<Player>(GetThis<Player>()));
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		//�ŏ��̃X�e�[�g��PlayerDefault�ɐݒ�
		m_StateMachine->ChangeState(PlayerDefaultState::Instance());
	}

	//�X�V
	void Player::OnUpdate() {
		//�R���g���[���`�F�b�N���ē��͂�����΃R�}���h�Ăяo��
		m_InputHandler.PushHandle(GetThis<Player>());
		//�X�e�[�g�}�V���X�V
		m_StateMachine->Update();

		Vec3 StartPos, EndPos;
		GetFightBehavior()->GetSowdStartEndLine(StartPos, EndPos);

		auto EnemyPtr = GetStage()->GetSharedGameObject<EnemyBox>(L"EnemyBox");
		Vec3 HitPoint;
		if (EnemyPtr->IsHitSegmentTriangles(StartPos, EndPos, HitPoint)) {
			//�X�p�[�N�̕��o
			auto PtrSpark = GetStage()->GetSharedGameObject<MultiSpark>(L"MultiSpark");
			PtrSpark->InsertSpark(HitPoint);
		}

		auto BonePtr = GetStage()->GetSharedGameObject<BoneChara>(L"BoneChara");
		if (BonePtr->IsHitSegmentTriangles(StartPos, EndPos, HitPoint)) {
			//�X�p�[�N�̕��o
			auto PtrSpark = GetStage()->GetSharedGameObject<MultiSpark>(L"MultiSpark");
			PtrSpark->InsertSpark(HitPoint);
		}
	}

	//��X�V
	void Player::OnUpdate2() {
		//������̕\��
		DrawStrings();
	}

	void Player::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
		//�v���C���[�������ɓ�������
		if (GetStateMachine()->GetCurrentState() == PlayerJumpState::Instance()) {
			//���݂��W�����v�X�e�[�g�Ȃ�PlayerDefault�ɐݒ�
			GetStateMachine()->ChangeState(PlayerDefaultState::Instance());
		}
	}

	//A�{�^���n���h��
	void  Player::OnPushA() {
		if (GetStateMachine()->GetCurrentState() == PlayerDefaultState::Instance()) {
			//�ʏ�X�e�[�g�Ȃ�W�����v�X�e�[�g�Ɉڍs
			GetStateMachine()->ChangeState(PlayerJumpState::Instance());
		}
	}

	//X�{�^���n���h��
	void  Player::OnPushX() {
		//����U��X�e�[�g�Ɉڍs
		GetStateMachine()->ChangeState(PlayerSwordState::Instance());
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

		wstring str = FPS + PositionStr + RididStr ;
		//�����������
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}


	//--------------------------------------------------------------------------------------
	///	�ʏ�X�e�[�g
	//--------------------------------------------------------------------------------------

	IMPLEMENT_SINGLETON_INSTANCE(PlayerDefaultState)

	void PlayerDefaultState::Enter(const shared_ptr<Player>& Obj) {
		Obj->GetFightBehavior()->HaveSword(L"PlayerSword");
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
	///	�W�����v�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerJumpState)

	void PlayerJumpState::Enter(const shared_ptr<Player>& Obj) {
		auto PtrGrav = Obj->GetBehavior<Gravity>();
		PtrGrav->StartJump(Vec3(0, 4.0f, 0));
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
	///	����U��X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerSwordState)

	void PlayerSwordState::Enter(const shared_ptr<Player>& Obj) {
		Obj->GetFightBehavior()->StartShakeSword();
	}

	void PlayerSwordState::Execute(const shared_ptr<Player>& Obj) {
		auto PtrBehavior = Obj->GetBehavior<PlayerBehavior>();
		PtrBehavior->MovePlayer();
		auto PtrGrav = Obj->GetBehavior<Gravity>();
		PtrGrav->Execute();
		if (Obj->GetFightBehavior()->RotationShakeSword()) {
			//������]��������f�t�H���g�X�e�[�g�Ɉڍs
			Obj->GetStateMachine()->ChangeState(PlayerDefaultState::Instance());
		}
	}

	void PlayerSwordState::Exit(const shared_ptr<Player>& Obj) {
		//�������Ȃ�
	}


}
//end basecross

