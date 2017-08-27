/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	RollingBox�O���[�v(�C���X�^���X�`��̂��߂����̃I�u�W�F�N�g)
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	RollingBoxGroup::RollingBoxGroup(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{
	}
	RollingBoxGroup::~RollingBoxGroup() {}

	//������
	void RollingBoxGroup::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat Qt;
		Qt.identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(Vec3(0, 0, 0));
		//�e������(���ȉe�p�̃V�F�[�_���g�����߂̗��R)
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<PNTStaticInstanceDraw>();
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		//���^�[�����ɍs����N���A����
		PtrDraw->SetAutoClearMatrixVec(true);
	}


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̉�]�{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	RollingBox::RollingBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position,
		float RotParam
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position),
		m_RotParam(RotParam)
	{
	}
	RollingBox::~RollingBox() {}

	//������
	void RollingBox::OnCreate() {
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

		auto GroupPtr = GetStage()->GetSharedGameObject<RollingBoxGroup>(L"RollingBoxGroup");
		auto DrawPtr = GroupPtr->GetComponent<PNTStaticInstanceDraw>();
		DrawPtr->AddMatrix(PtrTransform->GetWorldMatrix());

		AddTag(L"MoveBox");

	}

	void RollingBox::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		auto PtrTransform = GetComponent<Transform>();
		auto Qt = PtrTransform->GetQuaternion();
		Quat QtSpan(Vec3(0, 1, 0), ElapsedTime * m_RotParam);
		Qt *= QtSpan;
		PtrTransform->SetQuaternion(Qt);
		auto GroupPtr = GetStage()->GetSharedGameObject<RollingBoxGroup>(L"RollingBoxGroup");
		auto DrawPtr = GroupPtr->GetComponent<PNTStaticInstanceDraw>();
		DrawPtr->AddMatrix(PtrTransform->GetWorldMatrix());
	}




}
//end basecross
