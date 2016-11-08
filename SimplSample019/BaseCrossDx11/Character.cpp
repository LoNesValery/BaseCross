/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	�J�����}��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Cameraman::Cameraman(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{
	}
	Cameraman::~Cameraman() {}

	//������
	void Cameraman::OnCreate() {

		//Rigidbody������
		auto PtrRigid = AddComponent<Rigidbody>();

		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");

		Vector3 TargetBase(0, 1.0f, -2.0f);
		auto PlayerFront = PlayerPtr->GetComponent<Transform>()->GetQuaternion();
		auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
		Matrix4X4 m;
		m.RotationQuaternion(PlayerFront);
		TargetBase.Transform(m);
		TargetBase += PlayerPos;

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(0.1, 0.1, 0.1);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(TargetBase);

		//Arrive����
		auto PtrArrive = AddComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(TargetBase);

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
	}
	//����
	void Cameraman::OnUpdate() {
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		Vector3 TargetBase(0, 1.0f, -2.0f);
		auto PlayerQt = PlayerPtr->GetComponent<Transform>()->GetQuaternion();
		auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
		auto Pos = GetComponent<Transform>()->GetPosition();
		Matrix4X4 m;
		m.DefTransformation(Vector3(1, 1, 1), PlayerQt, PlayerPos);
		TargetBase.Transform(m);

		auto PtrCamera = GetStage()->GetView()->GetTargetCamera();
		auto PtrLookAtCamera = dynamic_pointer_cast<LookAtCamera>(PtrCamera);
		if (PtrCamera && !PtrLookAtCamera) {
			Vector2 Vec2Ply(PlayerPos.x, PlayerPos.z);
			Vector2 Vec2(Pos.x, Pos.z);
			float Span = Vector2EX::Length(Vec2Ply - Vec2);
			auto PtrRigid = GetComponent<Rigidbody>();
			auto Velo = PtrRigid->GetVelocity();
			if (Span < 1.5f) {
				TargetBase = Pos - PlayerPos;
				TargetBase.Normalize();
				TargetBase *= 2.0f;
				TargetBase = PlayerPos + TargetBase;
				TargetBase.y = PlayerPos.y + 1.0f;
			}
		}
		//Arrive����
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(TargetBase);


	}

	void Cameraman::OnLastUpdate() {
		auto PtrCamera = GetStage()->GetView()->GetTargetCamera();
		auto PtrLookAtCamera = dynamic_pointer_cast<LookAtCamera>(PtrCamera);
		if (PtrCamera && !PtrLookAtCamera) {
			auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
			auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
			auto Pos = GetComponent<Transform>()->GetPosition();
			PtrCamera->SetEye(Pos);
			PtrCamera->SetAt(PlayerPos);
		}
	}


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

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
	}



}
//end basecross
