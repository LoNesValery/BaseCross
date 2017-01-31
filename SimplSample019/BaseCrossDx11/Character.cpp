/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox() {}

	//������
	void FixedBox::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//�Փ˔���
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
	}



	//--------------------------------------------------------------------------------------
	//	class RoomDoor : public GameObject;
	//	�p�r: �����̃h�A
	//--------------------------------------------------------------------------------------
	RoomDoor::RoomDoor(const shared_ptr<Stage>& StagePtr,
		const wstring& TexResName,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	):
		GameObject(StagePtr),
		m_TexResName(TexResName),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{}
	RoomDoor::~RoomDoor() {}

	//������
	void RoomDoor::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//�Փ˔���
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//�^�O������
		AddTag(L"RoomDoor");


		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(m_TexResName);
	}

	//--------------------------------------------------------------------------------------
	//	�����̑�
	//--------------------------------------------------------------------------------------
	RoomWindow::RoomWindow(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{}
	RoomWindow::~RoomWindow() {}

	//������
	void RoomWindow::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//�Փ˔���
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//�^�O������
		AddTag(L"RoomWindow");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");

		//�p�����[�^�̎擾
		auto PtrParam = App::GetApp()->GetScene<Scene>()->GetGameParamaters();
		if (PtrParam.m_IsHeadTaskClear) {
			PtrDraw->SetTextureResource(L"SKY_TX");
		}
		else {
			PtrDraw->SetTextureResource(L"WALL_TX");
		}

	}

	void RoomWindow::WindowClear() {
		auto PtrDraw = GetComponent<BcPNTStaticDraw>();
		PtrDraw->SetTextureResource(L"SKY_TX");
		//�p�����[�^�̎擾
		auto& PtrParam = App::GetApp()->GetScene<Scene>()->GetGameParamaters();
		PtrParam.m_IsHeadTaskClear = true;
	}




	//--------------------------------------------------------------------------------------
	//	class PopupBalloon : public GameObject;
	//	�p�r:�@�����o��
	//--------------------------------------------------------------------------------------
	PopupBalloon::PopupBalloon(const shared_ptr<Stage>& StagePtr,
		const wstring& TexResName,
		const Vector3& Scale
	) :
		GameObject(StagePtr),
		m_TexResName(TexResName),
		m_Scale(Scale),
		m_EmitterPosition(0,0,0)
	{}
	PopupBalloon::~PopupBalloon() {}

	//������
	void PopupBalloon::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(0,0,0);
		PtrTransform->SetPosition(0,0,0);

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SQUARE");
		PtrDraw->SetTextureResource(m_TexResName);
		SetDrawActive(false);
	}

	void PopupBalloon::OnUpdate() {
		if (IsDrawActive()) {
			auto PtrTransform = GetComponent<Transform>();
			Quaternion Qt;
			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();
			Vector3 Line = PtrCamera->GetAt() - PtrCamera->GetEye();
			Qt.FacingY(Line);
			PtrTransform->SetQuaternion(Qt);
		}

	}


	//�\��
	void PopupBalloon::PopupOpen(const Vector3& EmitterPosition) {
		m_EmitterPosition = EmitterPosition;
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale);
		Quaternion Qt;
		auto PtrCamera = GetStage()->GetView()->GetTargetCamera();
		Vector3 Line = PtrCamera->GetAt() - PtrCamera->GetEye();
		Qt.FacingY(Line);
		PtrTransform->SetQuaternion(Qt);
		Vector3 Pos = m_EmitterPosition;
		Pos.y +=  m_Scale.y;
		PtrTransform->SetPosition(Pos);
		SetDrawActive(true);

	}
	//�N���[�Y
	void PopupBalloon::PopupClose() {
		SetDrawActive(false);
	}

	//--------------------------------------------------------------------------------------
	//	�������ێ����邾���̃I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	StringGameObject::StringGameObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	StringGameObject::~StringGameObject() {}

	//������
	void StringGameObject::OnCreate() {
		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetFont(L"�l�r�S�V�b�N", 64);
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(320.0f, 320.0f, 1200.0f, 750.0f));

	}

	void StringGameObject::SetText(const wstring& str) {
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}



}
//end basecross
