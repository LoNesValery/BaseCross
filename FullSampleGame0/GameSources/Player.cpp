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
	Player::Player(const shared_ptr<Stage>& StagePtr,
		const Vector3 StartPos, const Vector3 StartVelocity) :
		GameObject(StagePtr),
		m_MaxSpeed(10.0f),	//�ō����x
		m_Decel(0.95f),	//�����l
		m_Mass(1.0f),	//����
		m_IsThrough(false),
		m_StartPos(StartPos),
		m_StartVelocity(StartVelocity)
	{}

	//������
	void Player::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//���a25�Z���`�̋���
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);
		//Rigidbody������
		auto PtrRedid = AddComponent<Rigidbody>();
		//�����W����0.5
		PtrRedid->SetReflection(0.5f);
		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();
		//�ŉ��n�_
		PtrGravity->SetBaseY(0.125f);
		//�Փ˔���
		auto PtrColl = AddComponent<CollisionSphere>();
		//�������̂ݔ���
		PtrColl->SetIsHitAction(IsHitAction::AutoOnObjectRepel);

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

		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetFont(L"�l�r�S�V�b�N", 32);
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


		//��������
		SetAlphaActive(true);
		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetView()->GetTargetCamera());
		if (PtrCamera) {
			//LookAtCamera�ɒ��ڂ���I�u�W�F�N�g�i�v���C���[�j�̐ݒ�
			PtrCamera->SetTargetObject(GetThis<GameObject>());
		}
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< StateMachine<Player> >(GetThis<Player>());
		//�ŏ��̃X�e�[�g��DefaultState�ɐݒ�
		m_StateMachine->ChangeState(DefaultState::Instance());
	}

	//�ړ��̌����𓾂�
	Vector3 Player::GetAngle() {
		Vector3 Angle(0, 0, 0);
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//���������̃X�s�[�h
				auto PtrTransform = GetComponent<Transform>();
				auto PtrCamera = GetStage()->GetView()->GetTargetCamera();
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


	//�X�V
	void Player::OnUpdate() {
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̍X�V���s����(Execute()�֐����Ă΂��)
		m_StateMachine->Update();
	}

	void Player::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
		for (auto &v : OtherVec) {
			auto shenemy = dynamic_pointer_cast<EnemyInterface>(v);
			auto shCylinder = dynamic_pointer_cast<FixedCylinder>(v);
			auto shWindow = dynamic_pointer_cast<RoomWindow>(v);
			auto shDoor = dynamic_pointer_cast<RoomDoor>(v);
			auto sh = dynamic_pointer_cast<GameObject>(v);
			if (shenemy) {
				auto PtrRedid = GetComponent<Rigidbody>();
				auto Pos = GetComponent<Transform>()->GetPosition();
				auto EnemyPos = sh->GetComponent<Transform>()->GetPosition();

				auto Coll = GetComponent<CollisionSphere>();
				auto Sp = Coll->GetSphere();
				auto EnColl = v->GetComponent<CollisionSphere>();
				auto EnSp = EnColl->GetSphere();
				float Esc = Sp.m_Radius + EnSp.m_Radius;
				auto Span = Pos - EnemyPos;
				Span.y = 0;
				Span.Normalize();
				Span *= Esc;
				Pos = EnemyPos + Span;
				GetComponent<Transform>()->SetPosition(Pos);
				GetComponent<Transform>()->SetToBefore();

				auto Velo = Pos - EnemyPos;
				Velo *= 10.0f;
				PtrRedid->SetVelocity(Velo);
				//���C�t�����炷
				App::GetApp()->GetScene<Scene>()->GetGameParamaters().m_PlayerLife--;
				if (App::GetApp()->GetScene<Scene>()->GetGameParamaters().m_PlayerLife <= 0) {
					auto ScenePtr = App::GetApp()->GetScene<Scene>();
					PostEvent(0.0f, GetThis<ObjectInterface>(), ScenePtr, L"ToGameOver");
				}
				return;
			}
			if (shCylinder) {
				auto NowStage = dynamic_pointer_cast<NowScienceRoom>(GetStage());
				if (NowStage) {
					shCylinder->BalloonObOff();
					return;
				}
			}
			if (shWindow) {
				auto PastStage = dynamic_pointer_cast<PastScienceRoom>(GetStage());
				if (PastStage) {
					shWindow->WindowClear();
					return;
				}
			}
			if (shDoor) {
				auto& param = App::GetApp()->GetScene<Scene>()->GetGameParamaters();
				if (param.m_IsHeadTaskClear) {
					auto NowStage = dynamic_pointer_cast<NowScienceRoom>(GetStage());
					if (NowStage) {
						auto ScenePtr = App::GetApp()->GetScene<Scene>();
						PostEvent(0.0f, GetThis<ObjectInterface>(), ScenePtr, L"ToClear");
						return;
					}
				}
			}
		}

		if (GetStateMachine()->GetCurrentState() == JumpState::Instance()) {
			GetStateMachine()->ChangeState(DefaultState::Instance());
		}
	}
	//�^�[���̍ŏI�X�V��
	void Player::OnLastUpdate() {

		auto PtrTransform = GetComponent<Transform>();
		auto Coll = GetComponent<CollisionSphere>();
		auto Sp = Coll->GetSphere();
		//�S�[���Ƃ̌����𔻒�
		auto ShGoal = GetStage()->GetSharedGameObject<TileWall>(L"Goal");
		if (ShGoal && !m_IsThrough) {
			auto PtrCylinder = GetStage()->GetSharedGameObject<FixedCylinder>(L"FixedCylinder");
			//�o���[���^�X�N���N���A�������ǂ���
			if (App::GetApp()->GetScene<Scene>()->GetGameParamaters().m_BalloonOpenTask) {
				Vector3 RetVec, Normal;
				if (ShGoal->HitTestSphere(Sp, RetVec, Normal)) {
					m_IsThrough = true;
					return;
				}
			}
		}
		//���ʂƂ̌����𔻒�
		if (!m_IsThrough) {
			auto Group = GetStage()->GetSharedObjectGroup(L"WallGroup");
			for (auto Ptr : Group->GetGroupVector()) {
				if (!Ptr.expired()) {
					auto SquarePtr = dynamic_pointer_cast< TileWall>(Ptr.lock());
					if (SquarePtr) {
						Vector3 RetVec, Normal;
						if (SquarePtr->HitTestSphere(Sp, RetVec, Normal)) {
							//�q�b�g�����̂ňʒu���q�b�g�ʒu�ɏC������
							Vector3 SetPos = RetVec + -Normal * Sp.m_Radius;
							PtrTransform->SetPosition(SetPos);
							auto PtrGrav = GetComponent<Gravity>();
							//�~�ς��������̉����x��0�ɂ���
							PtrGrav->SetGravityVelocityZero();
							auto PtrRidit = GetComponent<Rigidbody>();
							//����
							Vector3 Refalct = Vector3EX::Reflect(PtrRidit->GetVelocity(), Normal);
							Refalct *= 0.5f;
							PtrRidit->SetVelocity(Refalct);

						}
					}
				}
			}
		}

		//�Q�[���X�e�[�W�͈͂���̗��E�`�F�b�N
		Rect2D<float> StageRect(-5.05f, -5.05f, 5.05f, 5.05f);
		auto Pos = PtrTransform->GetPosition();
		Point2D<float> PlayerPoint(Pos.x, Pos.z);
		if (!StageRect.PtInRect(PlayerPoint)) {
			auto ScenePtr = App::GetApp()->GetScene<Scene>();
			PostEvent(0.0f, GetThis<ObjectInterface>(), ScenePtr, L"ChangeScienceRoom");
			return;

		}


		//������\��
		wstring STAGE_NAME(L"���݂̗��Ȏ�");
		auto PastStagePtr = dynamic_pointer_cast<PastScienceRoom>(GetStage());
		if (PastStagePtr) {
			STAGE_NAME = L"���ɂ����̗��Ȏ�";
		}
		STAGE_NAME += L"\n";


		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\n";

		wstring LIFE(L"LIFE: ");
		LIFE += Util::IntToWStr(App::GetApp()->GetScene<Scene>()->GetGameParamaters().m_PlayerLife);
		LIFE += L"\n";
		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetView()->GetTargetCamera());
		if (PtrCamera) {
			PtrCamera->SetTargetObject(GetThis<GameObject>());
		}

		wstring str = STAGE_NAME + FPS + LIFE;
		//�����������
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
		
	}

	//���[�V��������������֐��Q
	//�ړ����Č������ړ������ɂ���
	void Player::MoveRotationMotion() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vector3 Angle = GetAngle();
		//Transform
		auto PtrTransform = GetComponent<Transform>();
		//Rigidbody�����o��
		auto PtrRedit = GetComponent<Rigidbody>();
		//���݂̑��x�����o��
		auto Velo = PtrRedit->GetVelocity();
		//�ړI�n���ō����x���|���ċ��߂�
		auto Target = Angle * m_MaxSpeed;
		//�ړI�n�Ɍ��������߂ɗ͂̂�����������v�Z����
		//Force�̓t�H�[�X�ł���
		auto Force = Target - Velo;
		//y��0�ɂ���
		Force.y = 0;
		//�����x�����߂�
		auto Accel = Force / m_Mass;
		//�^�[�����Ԃ��|�������̂𑬓x�ɉ��Z����
		Velo += (Accel * ElapsedTime);
		//��������
		Velo *= m_Decel;
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


	//A�{�^���ŃW�����v����ǂ����𓾂�
	bool Player::IsJumpMotion() {
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//A�{�^���������ꂽ�u�ԂȂ�W�����v
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
				return true;
			}
		}
		return false;
	}
	//A�{�^���ŃW�����v����u�Ԃ̏���
	void Player::JumpMotion() {
		auto PtrTrans = GetComponent<Transform>();
		//�d��
		auto PtrGravity = GetComponent<Gravity>();
		//�W�����v�X�^�[�g
		Vector3 JumpVec(0.0f, 4.0f, 0);
		PtrGravity->StartJump(JumpVec);
	}
	//A�{�^���ŃW�����v���Ă���Ԃ̏���
	//�W�����v�I��������true��Ԃ�
	bool Player::JumpMoveMotion() {
		auto PtrTransform = GetComponent<Transform>();
		//�d��
		auto PtrGravity = GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0) {
			return true;
		}
		return false;
	}


	//--------------------------------------------------------------------------------------
	//	class DefaultState : public ObjState<Player>;
	//	�p�r: �ʏ�ړ�
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<DefaultState> DefaultState::Instance() {
		static shared_ptr<DefaultState> instance;
		if (!instance) {
			instance = shared_ptr<DefaultState>(new DefaultState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void DefaultState::Enter(const shared_ptr<Player>& Obj) {
		//�������Ȃ�
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void DefaultState::Execute(const shared_ptr<Player>& Obj) {
		Obj->MoveRotationMotion();
		if (Obj->IsJumpMotion()) {
			//Jump�{�^���ŃX�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(JumpState::Instance());
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void DefaultState::Exit(const shared_ptr<Player>& Obj) {
		//�������Ȃ�
	}


	//--------------------------------------------------------------------------------------
	//	class JumpState : public ObjState<Player>;
	//	�p�r: �W�����v���
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<JumpState> JumpState::Instance() {
		static shared_ptr<JumpState> instance;
		if (!instance) {
			instance = shared_ptr<JumpState>(new JumpState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void JumpState::Enter(const shared_ptr<Player>& Obj) {
		//�W�����v�����ړ��\�Ƃ���
		Obj->MoveRotationMotion();
		Obj->JumpMotion();
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void JumpState::Execute(const shared_ptr<Player>& Obj) {
		//�W�����v�����ړ��\�Ƃ���
		Obj->MoveRotationMotion();
		if (Obj->JumpMoveMotion()) {
			//�ʏ��Ԃɖ߂�
			Obj->GetStateMachine()->ChangeState(DefaultState::Instance());
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void JumpState::Exit(const shared_ptr<Player>& Obj) {
		//�������Ȃ�
	}


}
//end basecross

