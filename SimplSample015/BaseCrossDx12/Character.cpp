/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	���ʎ���
	//--------------------------------------------------------------------------------------
	SquareObject::SquareObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, const Vector3& Scale, const Quaternion& Qt, const Vector3& Pos) :
		GameObject(PtrScene),
		m_TextureFileName(TextureFileName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	SquareObject::~SquareObject() {}

	void SquareObject::OnCreate() {
		m_SquareMesh = MeshResource::CreateSquare(1.0f);
		//�e�N�X�`���̍쐬
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetQuaternion(m_Qt);
		PtrTrans->SetPosition(m_Pos);

		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetTrace(false);
		DrawComp->SetMeshResource(m_SquareMesh);
		DrawComp->SetTextureResource(m_TextureResource);
	}
	void SquareObject::OnUpdate() {
	}

	//--------------------------------------------------------------------------------------
	///	�����̎���
	//--------------------------------------------------------------------------------------
	CubeObject::CubeObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, const Vector3& Scale, const Quaternion& Qt, const Vector3& Pos) :
		GameObject(PtrScene),
		m_TextureFileName(TextureFileName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	CubeObject::~CubeObject() {}

	void CubeObject::OnCreate() {
		m_CubeMesh = MeshResource::CreateCube(1.0f);
		//�e�N�X�`���̍쐬
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetQuaternion(m_Qt);
		PtrTrans->SetPosition(m_Pos);

		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetTrace(false);
		DrawComp->SetMeshResource(m_CubeMesh);
		DrawComp->SetTextureResource(m_TextureResource);
	}
	void CubeObject::OnUpdate() {
	}

	OBB CubeObject::GetObb()const {
		auto PtrTrans = GetComponent<Transform>();
		OBB obb(Vector3(1.0f, 1.0f, 1.0f), PtrTrans->GetWorldMatrix());
		return obb;
	}



	//--------------------------------------------------------------------------------------
	///	���̎���
	//--------------------------------------------------------------------------------------
	SphereObject::SphereObject(const shared_ptr<Scene> PtrScene,
		UINT Division, const wstring& TextureFileName, bool Trace, const Vector3& Pos) :
		GameObject(PtrScene),
		m_Division(Division),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Scale(Vector3(1.0f, 1.0f, 1.0f)),
		m_Qt(),
		m_Pos(Pos),
		m_MoveXDir(4.0f)
	{
		m_Qt.Identity();
	}
	SphereObject::~SphereObject() {}


	void SphereObject::OnCreate() {
		m_SphereMesh = MeshResource::CreateSphere(1.0f, m_Division);
		//�e�N�X�`���̍쐬
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetQuaternion(m_Qt);
		PtrTrans->SetPosition(m_Pos);

		auto PtrMoveBy = AddComponent<MoveBy>();
		PtrMoveBy->SetParams(2.0f, Vector3(m_MoveXDir, 0, 0));
		PtrMoveBy->Run();

		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetTrace(true);
		DrawComp->SetMeshResource(m_SphereMesh);
		DrawComp->SetTextureResource(m_TextureResource);

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< StateMachine<SphereObject> >(GetThis<SphereObject>());
		//�ŏ��̃X�e�[�g��DefaultState�ɐݒ�
		m_StateMachine->ChangeState(RightState::Instance());

	}
	void SphereObject::OnUpdate() {
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̍X�V���s����(Execute()�֐����Ă΂��)
		m_StateMachine->Update();
	}

	void SphereObject::SetActionMotion(SphereMotion motion) {
		auto PtrMoveBy = GetComponent<MoveBy>();
		switch (motion) {
		case SphereMotion::RightMotion:
			PtrMoveBy->SetParams(2.0f, Vector3(4.0f, 0, 0));
			PtrMoveBy->Run();
			break;
		case SphereMotion::LeftMotion:
			PtrMoveBy->SetParams(2.0f, Vector3(-4.0f, 0, 0));
			PtrMoveBy->Run();
			break;
		}
	}

	bool SphereObject::IstActionArrived() {
		auto PtrMoveBy = GetComponent<MoveBy>();
		if (PtrMoveBy->IsArrived()) {
			return true;
		}
		return false;

	}




	//--------------------------------------------------------------------------------------
	//	class RightState : public ObjState<SphereObject>;
	//	�p�r: �E�����ړ�
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<RightState> RightState::Instance() {
		static shared_ptr<RightState> instance;
		if (!instance) {
			instance = shared_ptr<RightState>(new RightState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void RightState::Enter(const shared_ptr<SphereObject>& Obj) {
		Obj->SetActionMotion(SphereMotion::RightMotion);
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void RightState::Execute(const shared_ptr<SphereObject>& Obj) {
		if (Obj->IstActionArrived()) {
			Obj->GetStateMachine()->ChangeState(LeftState::Instance());
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void RightState::Exit(const shared_ptr<SphereObject>& Obj) {
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	//	class LeftState : public ObjState<Enemy>;
	//	�p�r: �������ړ�
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<LeftState> LeftState::Instance() {
		static shared_ptr<LeftState> instance;
		if (!instance) {
			instance = shared_ptr<LeftState>(new LeftState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void LeftState::Enter(const shared_ptr<SphereObject>& Obj) {
		Obj->SetActionMotion(SphereMotion::LeftMotion);
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void LeftState::Execute(const shared_ptr<SphereObject>& Obj) {
		if (Obj->IstActionArrived()) {
			Obj->GetStateMachine()->ChangeState(RightState::Instance());
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void LeftState::Exit(const shared_ptr<SphereObject>& Obj) {
		//�������Ȃ�
	}




}
//end basecross
