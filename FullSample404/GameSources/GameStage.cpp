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
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrLookAtCamera = ObjectFactory::Create<LookAtCamera>(10.0f);
		PtrView->SetCamera(PtrLookAtCamera);
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	//�v���[�g�̍쐬
	void GameStage::CreatePlate() {
		//�X�e�[�W�ւ̃Q�[���I�u�W�F�N�g�̒ǉ�
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(200.0f, 200.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);

		//�`��R���|�[�l���g�̒ǉ�
		auto DrawComp = Ptr->AddComponent<BcPNTStaticDraw>();
		//�X�y�L�����[�Ȃ�
		DrawComp->DisableSpecular();
		DrawComp->SetFogEnabled(true);
		DrawComp->SetDiffuse(Color4(0.3f, 0.7f, 1.0f, 1.0f));

		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//�����ɉe���f�肱�ނ悤�ɂ���
		DrawComp->SetOwnShadowActive(true);

		//�`��R���|�[�l���g�e�N�X�`���̐ݒ�
		DrawComp->SetTextureResource(L"SKY_TX");
	}


	//���̍쐬
	void GameStage::CreateSphere() {
		AddGameObject<PcSphere>(Vector3(-7.5f, 0.5f, 5.0f));
		AddGameObject<PtSphere>(Vector3(-5.0f, 0.5f, 5.0f));
		AddGameObject<PctSphere>(Vector3(-2.5f, 0.5f, 5.0f));
		AddGameObject<PntSphere>(Vector3(0.0f, 0.5f, 5.0f), true);
		AddGameObject<PntSphere>(Vector3(2.5f, 0.5f, 5.0f), false);
		AddGameObject<PntSpSphere>(Vector3(5.0f, 0.5f, 5.0f), true);
		AddGameObject<PntSpSphere>(Vector3(7.5f, 0.5f, 5.0f), false);

	}


	//�L�����N�^�[�̍쐬
	void GameStage::CreateChara() {
		AddGameObject<StaticChara>(Vector3(2.5f, 0.0f, 0.0f),false);
		AddGameObject<StaticChara>(Vector3(5.0f, 0.0f, 0.0f),true);
		AddGameObject<BoneChara>(Vector3(-5.0f, 0.0f, 0.0f),false);
		AddGameObject<BoneChara>(Vector3(-2.5f, 0.0f, 0.0f),true);
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
			//���̍쐬
			CreateSphere();
			//�L�����N�^�[�̍쐬
			CreateChara();
			//�v���C���[�̍쐬
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}



}
//end basecross
