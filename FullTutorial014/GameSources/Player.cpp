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
	//�\�z
	Player::Player(const shared_ptr<Stage>& StagePtr, const Vec3& StatPos) :
		GameObject(StagePtr),
		m_StartPos(StatPos),
		m_Scale(0.25f)
	{}

	Vec3 Player::GetMoveVector() const {
		Vec3 Angle(0, 0, 0);
		//�L�[�{�[�h�̎擾
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		float MoveLength = 0;	//���������̃X�s�[�h
		auto PtrTransform = GetComponent<Transform>();
		auto PtrCamera = OnGetDrawCamera();
		//�i�s�����̌������v�Z
		Vec3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
		Front.y = 0;
		Front.normalize();
		//�i�s������������̊p�x���Z�o
		float FrontAngle = atan2(Front.z, Front.x);
		float MoveX = 0.0f;
		float MoveZ = 0.0f;
		if (KeyState.m_bPushKeyTbl['W']) {
			//�O
			MoveZ = 1.0f;
		}
		else if (KeyState.m_bPushKeyTbl['A']) {
			//��
			MoveX = -1.0f;
		}
		else if (KeyState.m_bPushKeyTbl['S']) {
			//���
			MoveZ = -1.0f;
		}
		else if (KeyState.m_bPushKeyTbl['D']) {
			//�E
			MoveX = 1.0f;
		}
		if (MoveX == 0.0f && MoveZ == 0.0f) {
			return Angle;
		}

		float KeyAngle = atan2(-MoveX, MoveZ);
		//�g�[�^���̊p�x���Z�o
		float TotalAngle = FrontAngle + KeyAngle;
		//�p�x����x�N�g�����쐬
		Angle = Vec3(cos(TotalAngle), 0, sin(TotalAngle));
		//���K������
		Angle.normalize();
		//Y���͕ω������Ȃ�
		Angle.y = 0;
		return Angle;
	}


	//������
	void Player::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(Vec3(m_Scale));	//���a25�Z���`�̋���
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionSphere>();
		//���肷�邾���Ȃ̂ŃA�N�V������None
		PtrCol->SetIsHitAction(IsHitAction::None);


		PsSphereParam param;
		//basecross�̃X�P�[�����O�͒��a��Ȃ̂ŁA���a��ɂ���
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		//�����e���\���̌v�Z
		param.m_Inertia = BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
		//�v���C���[�Ȃ̂ŃX���[�v���Ȃ�
		param.m_UseSleep = false;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat.identity();
		param.m_Pos = m_StartPos;
		param.m_LinearVelocity = Vec3(0);
		auto PsPtr = AddComponent<PsSphereBody>(param);
		PsPtr->SetAutoTransform(false);
		PsPtr->SetDrawActive(true);

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
	}

	//�X�V
	void Player::OnUpdate() {

		auto Vec = GetMoveVector();
		auto PtrPs = GetComponent<PsSphereBody>();
		auto Velo = PtrPs->GetLinearVelocity();
		Velo.x = Vec.x * 5.0f;
		Velo.z = Vec.z * 5.0f;
		PtrPs->SetLinearVelocity(Velo);

		//�L�[�{�[�h�̎擾
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (KeyState.m_bPressedKeyTbl['X']) {
			OnPushX();
		}
		else if (KeyState.m_bPressedKeyTbl['Z']) {
			OnPushA();
		}
	}

	//��X�V
	void Player::OnUpdate2() {
		auto PtrPs = GetComponent<PsSphereBody>();
		auto Ptr = GetComponent<Transform>();
		Ptr->SetPosition(PtrPs->GetPosition());
		//��]�̌v�Z
		Vec3 Angle = GetMoveVector();
		if (Angle.length() > 0.0f) {
			auto UtilPtr = GetBehavior<UtilBehavior>();
			//��ԏ������s��Ȃ���]�B��ԏ�������ɂ͈ȉ�1.0��0.1�Ȃǂɂ���
			UtilPtr->RotToHead(Angle, 1.0f);
		}
		//������̕\��
		DrawStrings();
	}

	//A�{�^���n���h��
	void  Player::OnPushA() {
		auto Ptr = GetComponent<Transform>();
		if (Ptr->GetPosition().y > 0.125f) {
			return;
		}
		auto PtrPs = GetComponent<PsSphereBody>();
		auto Velo = PtrPs->GetLinearVelocity();
		Velo += Vec3(0, 4.0f, 0.0);
		PtrPs->SetLinearVelocity(Velo);
	}


	//X�{�^���n���h��
	void Player::OnPushX() {
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		XAPtr->Start(L"cursor");

		auto Ptr = GetComponent<Transform>();
		Vec3 Pos = Ptr->GetPosition();
		Pos.y += 0.3f;
		Quat Qt = Ptr->GetQuaternion();
		Vec3 Rot = Qt.toRotVec();
		float RotY = Rot.y;
		Vec3 velo(sin(RotY), 0.05f, cos(RotY));
		velo.normalize();
		velo *= 20.0f;
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellGroup");
		for (size_t i = 0; i < Group->size(); i++) {
			auto shptr = dynamic_pointer_cast<ShellSphere>(Group->at(i));
			if (shptr && !shptr->IsUpdateActive()) {
				//�󂫂���������
				shptr->Reset(Pos, velo);
				return;
			}
		}
		//�����܂ŗ��Ă�΋󂫂��Ȃ�
		GetStage()->AddGameObject<ShellSphere>(Pos, velo);
	}


	//������̕\��
	void Player::DrawStrings() {

		//������\��
		wstring Mess(L"A�ŃW�����v�AX�{�^���Ŕ��ˁAB�ŃX�e�[�W�ؑ�\n");
		//�I�u�W�F�N�g��
		auto ObjCount = GetStage()->GetGameObjectVec().size();
		wstring OBJ_COUNT(L"OBJ_COUNT: ");
		OBJ_COUNT += Util::UintToWStr(ObjCount);
		OBJ_COUNT += L"\n";
		//�����I�u�W�F�N�g��
		wstring PS_OBJ_COUNT(L"PS_OBJ_COUNT: ");
		PS_OBJ_COUNT += Util::UintToWStr(GetStage()->GetBasePhysics().GetNumBodies());
		PS_OBJ_COUNT += L"\n";
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

		wstring MousePosStr(L"MousePos:\t");
		auto MousePos = GetTypeStage<GameStage>()->GetMousePoint();
		MousePosStr += L"X=" + Util::IntToWStr(MousePos.x) + L",\t";
		MousePosStr += L"Y=" + Util::IntToWStr(MousePos.y) + L"\n";

		Vec3 Near; Vec3 Far;
		GetTypeStage<GameStage>()->GetMouseRay(Near, Far);

		wstring MouseRayNearStr(L"MouseRayNear:\t");
		MouseRayNearStr += L"X=" + Util::FloatToWStr(Near.x, 6, Util::FloatModify::Fixed) + L",\t";
		MouseRayNearStr += L"Y=" + Util::FloatToWStr(Near.y, 6, Util::FloatModify::Fixed) + L",\t";
		MouseRayNearStr += L"Z=" + Util::FloatToWStr(Near.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring MouseRayFarStr(L"MouseRayFar:\t");
		MouseRayFarStr += L"X=" + Util::FloatToWStr(Far.x, 6, Util::FloatModify::Fixed) + L",\t";
		MouseRayFarStr += L"Y=" + Util::FloatToWStr(Far.y, 6, Util::FloatModify::Fixed) + L",\t";
		MouseRayFarStr += L"Z=" + Util::FloatToWStr(Far.z, 6, Util::FloatModify::Fixed) + L"\n";



		wstring str = Mess + OBJ_COUNT + PS_OBJ_COUNT + FPS + PositionStr + MousePosStr + MouseRayNearStr + MouseRayFarStr;
		//�����������
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}

}
//end basecross

