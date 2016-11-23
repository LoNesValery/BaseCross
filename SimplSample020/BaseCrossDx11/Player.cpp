/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�W�����v�R�}���h
	//--------------------------------------------------------------------------------------
	void JumpCommand::Excute(const shared_ptr<Player>& Obj) {
		//�W�����v�X�^�[�g
		Obj->GetStateMachine()->Push(PlayerJump::Instance());
	}
	//--------------------------------------------------------------------------------------
	///	�s���؂�ւ��R�}���h
	//--------------------------------------------------------------------------------------
	void BehaviorChangeCommand::Excute(const shared_ptr<Player>& Obj) {
		//�s���؂�ւ�
		Obj->ChangePlayerBehavior();
	}


	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	�p�r: �v���C���[
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{
	}
	Player::~Player() {}

	//�R���g���[���̃`�F�b�N
	void Player::InputCheck() {
		//�R���g���[���{�^���`�F�b�N
		auto pCom = m_Handler.HandleInput();
		if (pCom) {
			pCom->Excute(GetThis<Player>());
		}
	}
	//�s���̐؂�ւ�
	void Player::ChangePlayerBehavior() {
		//���̍s�����`�F���W����i���z�֐��ō쐬����j
		m_PlayerBehavior = m_PlayerBehavior->CreateNextBehavior(GetThis<Player>());
	}

	//������
	void Player::OnCreate() {
		//�R�}���h�̓o�^
		//A�{�^��
		m_Handler.SetButtonA(&m_Jump);
		//B�{�^��
		m_Handler.SetButtonB(&m_BehaviorChange);

		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(0.5f, 0.5, 0.5);	//���a50�Z���`�̋���
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.25f, 0);

		//Rigidbody������
		auto PtrRedid = AddComponent<Rigidbody>();
		//�����W����0.5
		PtrRedid->SetReflection(0.5f);

		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();
		//�ŉ��n�_
		PtrGravity->SetBaseY(0.25f);

		//�Փ˔���
		auto PtrColl = AddComponent<CollisionSphere>();
		//�������̂ݔ���
		PtrColl->SetIsHitAction(IsHitAction::AutoOnObjectRepel);
//		PtrColl->SetDrawActive(true);

		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetFont(L"�l�r�S�V�b�N", 32);
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


		Matrix4X4 SpanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		SpanMat.DefTransformation(
			Vector3(0.8, 0.8, 0.8),
			Vector3(0.0f, XM_PI, 0.0f),
			Vector3(0.0f, -0.5f, 0.0f)
		);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"PLAYER_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<PNTStaticModelDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"PLAYER_MESH");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);

		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetView()->GetTargetCamera());
		if (PtrCamera) {
			//LookAtCamera�ɒ��ڂ���I�u�W�F�N�g�i�v���C���[�j�̐ݒ�
			PtrCamera->SetTargetObject(GetThis<GameObject>());
		}
		//�s���N���X�̍\�z(�V���v���s��)
		m_PlayerBehavior = SimplePlayerBehavior::Instance();
		PtrString->SetText(L"�f�t�H���g�W�����v: B�{�^���Ő؂�ւ�");
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< LayeredStateMachine<Player> >(GetThis<Player>());
		//�ŏ��̃X�e�[�g��PlayerDefault�ɐݒ�
		m_StateMachine->Reset(PlayerDefault::Instance());
	}

	void Player::OnUpdate() {
		//�R���g���[���`�F�b�N
		InputCheck();
		//�X�e�[�g�}�V���X�V
		m_StateMachine->Update();
	}

	//--------------------------------------------------------------------------------------
	///	�v���C���[�s���e
	//--------------------------------------------------------------------------------------
	Vector3 PlayerBehavior::GetAngle(const shared_ptr<Player>& Obj) {
		Vector3 Angle(0, 0, 0);
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//���������̃X�s�[�h
				auto PtrTransform = Obj->GetComponent<Transform>();
				auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(Obj->GetStage()->GetView()->GetTargetCamera());
				if (PtrCamera) {
					//�i�s�����̌������v�Z
					Vector3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
					Front.y = 0;
					Front.Normalize();
					//�i�s������������̊p�x���Z�o
					float FrontAngle = atan2(Front.z, Front.x);
					//�R���g���[���̌����v�Z
					float MoveX = CntlVec[0].fThumbLX;
					float MoveZ = CntlVec[0].fThumbLY;
					MoveLength = sqrt(MoveX * MoveX + MoveZ * MoveZ);
					//�R���g���[���̌�������p�x���v�Z
					float CntlAngle = atan2(-MoveX, MoveZ);
					//�g�[�^���̊p�x���Z�o
					float TotalAngle = FrontAngle + CntlAngle;
					//�p�x����x�N�g�����쐬
					Angle = Vector3(cos(TotalAngle), 0, sin(TotalAngle));
					//���K������
					Angle.Normalize();
					//Y���͕ω������Ȃ�
					Angle.y = 0;
					Angle *= MoveLength;
				}
			}
		}
		return Angle;
	}

	//--------------------------------------------------------------------------------------
	///	�V���v���v���C���[�s��
	//--------------------------------------------------------------------------------------
	//�C���X�^���X�擾
	IMPLEMENT_SINGLETON_INSTANCE(SimplePlayerBehavior)
	shared_ptr<PlayerBehavior> SimplePlayerBehavior::CreateNextBehavior(const shared_ptr<Player>& Obj) {
		Obj->GetComponent<StringSprite>()->SetText(L"�n�C�W�����v: B�{�^���Ő؂�ւ�");
		//�n�C�W�����v�s����Ԃ�
		return HighJumpPlayerBehavior::Instance();
	}


	void SimplePlayerBehavior::Move(const shared_ptr<Player>& Obj) {
		auto Angle = GetAngle(Obj);
		auto PtrTransform = Obj->GetComponent<Transform>();
		auto PtrRedid = Obj->GetComponent<Rigidbody>();
		PtrRedid->SetVelocity(Angle * 10.0f);
		//��]�̌v�Z
		float YRot = PtrTransform->GetRotation().y;
		Quaternion Qt;
		Qt.Identity();
		if (Angle.Length() > 0.0f) {
			//�x�N�g����Y����]�ɕϊ�
			float PlayerAngle = atan2(Angle.x, Angle.z);
			Qt.RotationRollPitchYaw(0, PlayerAngle, 0);
			Qt.Normalize();
		}
		else {
			Qt.RotationRollPitchYaw(0, YRot, 0);
			Qt.Normalize();
		}
		//Transform
		PtrTransform->SetQuaternion(Qt);
	}

	void SimplePlayerBehavior::StartJump(const shared_ptr<Player>& Obj) {
		auto PtrTrans = Obj->GetComponent<Transform>();
		//�d��
		auto PtrGravity = Obj->GetComponent<Gravity>();
		//�W�����v�X�^�[�g
		Vector3 JumpVec(0.0f, 4.0f, 0);
		PtrGravity->StartJump(JumpVec);
	}
	void SimplePlayerBehavior::JumpExcute(const shared_ptr<Player>& Obj) {
		auto PtrTransform = Obj->GetComponent<Transform>();
		//�d��
		auto PtrGravity = Obj->GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0) {
			//�W�����v�I��
			Obj->GetStateMachine()->Pop();
		}
	}

	//--------------------------------------------------------------------------------------
	///	�n�C�W�����v�v���C���[�s��
	//--------------------------------------------------------------------------------------
	//�C���X�^���X�擾
	IMPLEMENT_SINGLETON_INSTANCE(HighJumpPlayerBehavior)
	shared_ptr<PlayerBehavior> HighJumpPlayerBehavior::CreateNextBehavior(const shared_ptr<Player>& Obj) {
		Obj->GetComponent<StringSprite>()->SetText(L"�f�t�H���g�W�����v: B�{�^���Ő؂�ւ�");
		//�V���v���s����Ԃ�
		return SimplePlayerBehavior::Instance();
	}
	void HighJumpPlayerBehavior::StartJump(const shared_ptr<Player>& Obj) {
		auto PtrTrans = Obj->GetComponent<Transform>();
		//�d��
		auto PtrGravity = Obj->GetComponent<Gravity>();
		//�W�����v�X�^�[�g
		Vector3 JumpVec(0.0f, 12.0f, 0);
		PtrGravity->StartJump(JumpVec);
	}

	//--------------------------------------------------------------------------------------
	///	�ʏ�X�e�[�g
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	IMPLEMENT_SINGLETON_INSTANCE(PlayerDefault)
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void PlayerDefault::Execute(const shared_ptr<Player>& Obj) {
		Obj->GetPlayerBehavior()->Move(Obj);
	}

	//--------------------------------------------------------------------------------------
	///	�W�����v�X�e�[�g
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	IMPLEMENT_SINGLETON_INSTANCE(PlayerJump)
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void PlayerJump::Enter(const shared_ptr<Player>& Obj) {
		Obj->GetPlayerBehavior()->StartJump(Obj);
	}

	//�X�e�[�g���s���ɖ�Update�ɌĂ΂��֐�
	void PlayerJump::Execute(const shared_ptr<Player>& Obj) {
		Obj->GetPlayerBehavior()->Move(Obj);
		Obj->GetPlayerBehavior()->JumpExcute(Obj);
	}









}
//end basecross

