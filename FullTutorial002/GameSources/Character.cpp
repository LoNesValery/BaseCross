/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Box::Box(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{
	}
	Box::~Box() {}

	//������
	void Box::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		Quaternion Qt;
		Qt.Identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(m_StartPos);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//���ߏ���
		SetAlphaActive(true);
	}

	//�X�V
	void Box::OnUpdate() {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0;
		}
		auto Pos = GetComponent<Transform>()->GetPosition();
		Pos.x = sin(m_TotalTime);
		GetComponent<Transform>()->SetPosition(Pos);
	}



}
//end basecross
