/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------




	//�r���[�ƃ��C�g�̍쐬
	void GameStage::CreateViewLight() {


		auto PtrView = CreateView<MultiView>();
		//���C���r���[�̃r���[�|�[�g�ƃJ�����̐ݒ�
		auto PtrLookAtCamera = ObjectFactory::Create<LookAtCamera>();
		PtrLookAtCamera->SetEye(Vector3(0.0f, 5.0f, -5.0f));
		PtrLookAtCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
		Viewport MainViewport;
		float Height = 200;
		MainViewport.Width = static_cast<float>(App::GetApp()->GetGameWidth());
		MainViewport.Height = static_cast<float>(App::GetApp()->GetGameHeight()) - Height;
		MainViewport.TopLeftX = 0;
		MainViewport.TopLeftY = 0;
		MainViewport.MinDepth = 0.5f;
		MainViewport.MaxDepth = 1.0f;
		//�䗦���ς��̂ŃJ�����Ƀr���[�|�[�g��`����
		PtrLookAtCamera->SetViewPort(MainViewport);
		PtrView->AddView(MainViewport, PtrLookAtCamera);
		
		//�T�u(�Œ�J����)
		auto PtrSubCamera = ObjectFactory::Create<Camera>();
		PtrSubCamera->SetEye(Vector3(0.0f, 5.0f,-5.0f));
		PtrSubCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));

		Viewport SubViewport;
		SubViewport.Width = static_cast<float>(320.0f);
		SubViewport.Height = static_cast<float>(256.0f);
		SubViewport.TopLeftX = 32;
		SubViewport.TopLeftY = 32;
		SubViewport.MinDepth = 0.0f;
		SubViewport.MaxDepth = 0.5f;
		//�䗦���ς��̂ŃJ�����Ƀr���[�|�[�g��`����
		PtrSubCamera->SetViewPort(SubViewport);
		PtrView->AddView(SubViewport, PtrSubCamera);
		//�V���O�����C�g�̍쐬
		auto PtrSingleLight = CreateLight<SingleLight>();
		//���C�g�̐ݒ�
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}


	//�v���[�g�̍쐬
	void GameStage::CreatePlate() {
		//�X�e�[�W�ւ̃Q�[���I�u�W�F�N�g�̒ǉ�
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		Matrix4X4 WorldMat;
		WorldMat.DefTransformation(
			Vector3(200.0f, 200.0f, 1.0f),
			Qt,
			Vector3(0.0f, 0.0f, 0.0f)
		);
		PtrTrans->SetScale(200.0f, 200.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);

		//�`��R���|�[�l���g�̒ǉ�
		auto DrawComp = Ptr->AddComponent<PNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//�����ɉe���f�肱�ނ悤�ɂ���
		DrawComp->SetOwnShadowActive(true);

		//�`��R���|�[�l���g�e�N�X�`���̐ݒ�
		DrawComp->SetTextureResource(L"SKY_TX");
	}

	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}


	void GameStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�v���[�g�̍쐬
			CreatePlate();
			//�v���[���[�̍쐬
			CreatePlayer();
			//�q���X�e�[�W�̍쐬
			AddChileStage<ChildStage>();

		}
		catch (...) {
			throw;
		}
	}


	//--------------------------------------------------------------------------------------
	//	�`���C���h�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	void ChildStage::OnCreate() {
		try {
			auto PtrView = CreateView<SingleView>();

			Viewport ChileViewport;
			float Height = 200;
			ChileViewport.Width = static_cast<float>(App::GetApp()->GetGameWidth());
			ChileViewport.Height = Height;
			ChileViewport.TopLeftX = 0;
			ChileViewport.TopLeftY = static_cast<float>(App::GetApp()->GetGameHeight()) - Height;
			ChileViewport.MinDepth = 0.0f;
			ChileViewport.MaxDepth = 1.0f;
			PtrView->SetViewport(ChileViewport);

			auto PtrStr = AddGameObject<StringGameObject>();
			wstring ClearStr = L"�`���C���h�X�e�[�W";
			PtrStr->SetText(ClearStr);
		}
		catch (...) {
			throw;
		}
	}

	void ChildStage::OnUpdate() {
	}


}
//end basecross
