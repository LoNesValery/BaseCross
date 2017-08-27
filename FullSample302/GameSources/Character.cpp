/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//�\�z�Ɣj��
	ActionObject::ActionObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos) {
	}
	ActionObject::~ActionObject() {}


	//������
	void ActionObject::OnCreate() {
		auto Ptr = GetComponent<Transform>();
		Ptr->SetPosition(m_StartPos);
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//���a25�Z���`�̋���
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);

		//�A�N�V�����̓o�^
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddRotateBy(1.0f, Vec3(0, XM_PI, 0));
		PtrAction->AddRotateInterval(1.0f);
		PtrAction->AddMoveBy(1.0f, Vec3(2.0f, 0, 0));
		PtrAction->AddMoveBy(0.5f, Vec3(0, 1.0f, -1.0f), Lerp::Linear, Lerp::EaseOut, Lerp::Linear);
		PtrAction->AddMoveBy(0.5f, Vec3(0, -1.0f, -1.0f), Lerp::Linear, Lerp::Easein, Lerp::Linear);
		PtrAction->AddMoveBy(2.0f, Vec3(-4.0f, 0, 0));
		PtrAction->AddMoveBy(1.0f, Vec3(0, 0, 2.0f));
		PtrAction->AddMoveBy(1.0f, Vec3(2.0f, 0, 0));

		//���[�v����
		PtrAction->SetLooped(true);
		//�A�N�V�����J�n
		PtrAction->Run();

		//�e�̍쐬
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`��
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		//���b�V���̓o�^
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//�e�N�X�`���̓o�^
		PtrDraw->SetTextureResource(L"TRACE_TX");


		//���ߏ�������
		SetAlphaActive(true);
	}



}
//end basecross
