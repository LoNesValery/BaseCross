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
		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();
		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionSphere>();
		PtrCol->SetIsHitAction(IsHitAction::AutoOnParentRepel);

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
			PtrCamera->SetTargetToAt(Vector3(0, 0.25f, 0));
		}
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


	//������̕\��
	void Player::DrawStrings() {

		//������\��
		//�s��
		wstring BEHAVIOR;


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

		wstring GravStr(L"Gravity:\t");

		auto Grav = GetComponent<Gravity>()->GetGravity();
		GravStr += L"X=" + Util::FloatToWStr(Grav.x, 6, Util::FloatModify::Fixed) + L",\t";
		GravStr += L"Y=" + Util::FloatToWStr(Grav.y, 6, Util::FloatModify::Fixed) + L",\t";
		GravStr += L"Z=" + Util::FloatToWStr(Grav.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring GravityStr(L"GravityVelocity:\t");
		auto GravityVelocity = GetComponent<Gravity>()->GetGravityVelocity();
		GravityStr += L"X=" + Util::FloatToWStr(GravityVelocity.x, 6, Util::FloatModify::Fixed) + L",\t";
		GravityStr += L"Y=" + Util::FloatToWStr(GravityVelocity.y, 6, Util::FloatModify::Fixed) + L",\t";
		GravityStr += L"Z=" + Util::FloatToWStr(GravityVelocity.z, 6, Util::FloatModify::Fixed) + L"\n";

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
		wstring str = BEHAVIOR + FPS + PositionStr + RididStr + GravStr + GravityStr + HitObjectStr;
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
		auto PtrDefault = Obj->GetBehavior<PlayerBehavior>();
		PtrDefault->MovePlayer();
	}

	void PlayerDefaultState::Exit(const shared_ptr<Player>& Obj) {
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	///	�W�����v�X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerJumpState)

	void PlayerJumpState::Enter(const shared_ptr<Player>& Obj) {
		auto PtrJump = Obj->GetBehavior<JumpBehavior>();
		PtrJump->StartJump(Vector3(0, 4.0f, 0));
	}

	void PlayerJumpState::Execute(const shared_ptr<Player>& Obj) {
		//�W�����v���������ύX�\
		auto PtrDefault = Obj->GetBehavior<PlayerBehavior>();
		PtrDefault->MovePlayer();
		auto PtrJump = Obj->GetBehavior<JumpBehavior>();
		if (PtrJump->Execute()) {
			Obj->GetStateMachine()->ChangeState(PlayerDefaultState::Instance());
		}
	}

	void PlayerJumpState::Exit(const shared_ptr<Player>& Obj) {
		//�������Ȃ�
	}


}
//end basecross

