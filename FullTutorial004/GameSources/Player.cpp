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
		PtrCol->SetIsHitAction(IsHitAction::AutoOnObjectRepel);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
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
	/// �A�N�V�����R�}���h�iA�{�^���j
	//--------------------------------------------------------------------------------------
	void ActionCommand::Excute(const shared_ptr<Player>& Obj) {
		//�A�N�V�����X�^�[�g
		if (Obj->GetStateMachine()->GetTopState() == PlayerDefault::Instance()) {
			Obj->GetStateMachine()->Push(PlayerAction::Instance());
		}
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
		GameObject(StagePtr),
		m_MaxSpeed(30.0f),	//�ō����x
		m_Decel(0.95f),	//�����l
		m_Mass(1.0f)	//����
	{}

	//�s���̐؂�ւ�
	void Player::ChangePlayerBehavior() {
		//���̍s�����`�F���W����i���z�֐��Ăяo���Ő؂�ւ���j
		m_PlayerBehavior = m_PlayerBehavior->ChangeNextBehavior(GetThis<Player>());
	}

	//������
	void Player::OnCreate() {
		//�R�}���h�̓o�^
		//A�{�^���ɃA�N�V�����R�}���h��o�^
		m_Handler.SetButtonA(&m_Action);
		//B�{�^���ɍs���؂�ւ��R�}���h��o�^
		m_Handler.SetButtonB(&m_BehaviorChange);


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
		PtrCol->SetIsHitAction(IsHitAction::Slide);
		PtrCol->SetDrawActive(true);

		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//�`�悷��e�N�X�`����ݒ�
		PtrDraw->SetTextureResource(L"TRACE_TX");

		//��������
		SetAlphaActive(true);
		//0�Ԗڂ̃r���[�̃J�����𓾂�
		//LookAtCamera�ł���
		
		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetView()->GetTargetCamera());
		if (PtrCamera) {
			//LookAtCamera�ɒ��ڂ���I�u�W�F�N�g�i�v���C���[�j�̐ݒ�
			PtrCamera->SetTargetObject(GetThis<GameObject>());
		}
		//�s���N���X�̍\�z(�f�t�H���g�s��)
		m_PlayerBehavior = DefaultPlayerBehavior::Instance();
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< LayeredStateMachine<Player> >(GetThis<Player>());
		//�ŏ��̃X�e�[�g��PlayerDefault�ɐݒ�
		m_StateMachine->Reset(PlayerDefault::Instance());
	}

	//�X�V
	void Player::OnUpdate() {
		//�R���g���[���`�F�b�N
		InputCheck();
		//�X�e�[�g�}�V���X�V
		m_StateMachine->Update();
	}

	void Player::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
	}

	//�^�[���̍ŏI�X�V��
	void Player::OnLastUpdate() {
		//������\��
		//�s��
		wstring BEHAVIOR;

		if (m_PlayerBehavior == DefaultPlayerBehavior::Instance()) {
			BEHAVIOR = L"DEFAULT�s��: A�{�^���ŃW�����v�BB�{�^���ōs���؂�ւ�\n";
		}
		else {
			BEHAVIOR = L"ATTACK�s��: A�{�^���ŃA�^�b�N�BB�{�^���ōs���؂�ւ�\n";
		}


		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
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
		wstring str = BEHAVIOR + FPS + PositionStr + RididStr + GravStr + GravityStr + HitObjectStr ;
		//�����������
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
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
				auto PtrCamera = Obj->GetStage()->GetView()->GetTargetCamera();
				//�i�s�����̌������v�Z
				Vector3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
				Front.y = 0;
				Front.Normalize();
				//�i�s������������̊p�x���Z�o
				float FrontAngle = atan2(Front.z, Front.x);
				//�R���g���[���̌����v�Z
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
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
			}
		}
		return Angle;
	}

	void PlayerBehavior::Move(const shared_ptr<Player>& Obj) {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vector3 Angle = GetAngle(Obj);
		//Transform
		auto PtrTransform = Obj->GetComponent<Transform>();
		//Rigidbody�����o��
		auto PtrRedit = Obj->GetComponent<Rigidbody>();
		//���݂̑��x�����o��
		auto Velo = PtrRedit->GetVelocity();
		//�ړI�n���ō����x���|���ċ��߂�
		auto Target = Angle * Obj->GetMaxSpeed();
		//�ړI�n�Ɍ��������߂ɗ͂̂�����������v�Z����
		//Force�̓t�H�[�X�ł���
		auto Force = Target - Velo;
		//y��0�ɂ���
		Force.y = 0;
		//�����x�����߂�
		auto Accel = Force / Obj->GetMass();
		//�^�[�����Ԃ��|�������̂𑬓x�ɉ��Z����
		Velo += (Accel * ElapsedTime);
		//��������
		Velo *= Obj->GetDecel();
		//���x��ݒ肷��
		PtrRedit->SetVelocity(Velo);
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


	//--------------------------------------------------------------------------------------
	///	�f�t�H���g�v���C���[�s��
	//--------------------------------------------------------------------------------------
	//�C���X�^���X�擾
	IMPLEMENT_SINGLETON_INSTANCE(DefaultPlayerBehavior)

	shared_ptr<PlayerBehavior> DefaultPlayerBehavior::ChangeNextBehavior(const shared_ptr<Player>& Obj) {
		//�U���s����Ԃ�
		return AttackPlayerBehavior::Instance();
	}


	void DefaultPlayerBehavior::StartAction(const shared_ptr<Player>& Obj) {
		//�d��
		auto PtrGravity = Obj->GetComponent<Gravity>();
		//�W�����v�X�^�[�g
		Vector3 JumpVec(0.0f, 4.0f, 0);
		PtrGravity->StartJump(JumpVec);
	}
	void DefaultPlayerBehavior::ExcuteAction(const shared_ptr<Player>& Obj) {
		//�d��
		auto PtrGravity = Obj->GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0) {
			//�W�����v�I��
			Obj->GetStateMachine()->Pop();
		}
	}

	//--------------------------------------------------------------------------------------
	///	�U���v���C���[�s��
	//--------------------------------------------------------------------------------------
	//�C���X�^���X�擾
	IMPLEMENT_SINGLETON_INSTANCE(AttackPlayerBehavior)

	shared_ptr<PlayerBehavior> AttackPlayerBehavior::ChangeNextBehavior(const shared_ptr<Player>& Obj) {
		//�ʏ�s����Ԃ�
		return DefaultPlayerBehavior::Instance();
	}

	void AttackPlayerBehavior::StartAction(const shared_ptr<Player>& Obj) {
		auto PtrCamera = Obj->GetStage()->GetView()->GetTargetCamera();
		auto PtrTrans = Obj->GetComponent<Transform>();
		auto Group = Obj->GetStage()->GetSharedObjectGroup(L"AttackBall");
		for (auto& v : Group->GetGroupVector()) {
			auto shptr = v.lock();
			if (shptr) {
				auto AttackPtr = dynamic_pointer_cast<AttackBall>(shptr);
				if (AttackPtr && !AttackPtr->IsUpdateActive()) {

					//��]�̌v�Z
					auto RotY = PtrTrans->GetRotation().y;
					auto Angle = Vector3(sin(RotY), 0, cos(RotY));
					Angle.Normalize();

					auto Span = Angle * 0.5f;



					AttackPtr->Weakup(PtrTrans->GetPosition() + Span, Angle * 10.0f);
					return;
				}
			}
		}
	}
	void AttackPlayerBehavior::ExcuteAction(const shared_ptr<Player>& Obj) {
		//�A�^�b�N�I��
		Obj->GetStateMachine()->Pop();
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
	///	�A�N�V�����X�e�[�g
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	IMPLEMENT_SINGLETON_INSTANCE(PlayerAction)

	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void PlayerAction::Enter(const shared_ptr<Player>& Obj) {
		Obj->GetPlayerBehavior()->StartAction(Obj);
	}

	//�X�e�[�g���s���ɖ�Update�ɌĂ΂��֐�
	void PlayerAction::Execute(const shared_ptr<Player>& Obj) {
		Obj->GetPlayerBehavior()->Move(Obj);
		Obj->GetPlayerBehavior()->ExcuteAction(Obj);
	}





}
//end basecross

