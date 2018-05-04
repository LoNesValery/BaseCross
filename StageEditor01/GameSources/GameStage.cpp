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
		auto PtrLookAtCamera = ObjectFactory::Create<LookAtCamera>();
		PtrView->SetCamera(PtrLookAtCamera);
		PtrLookAtCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		PtrLookAtCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		PtrLookAtCamera->SetMaxArm(100.0f);
		//�V���O�����C�g�̍쐬
		auto PtrSingleLight = CreateLight<SingleLight>();
		//���C�g�̐ݒ�
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}

	//���C���̍쐬
	void GameStage::CreateActionLine() {
		//X��
		AddGameObject<ActionLine>(Vec3(-100.0f,0,0), Vec3(100.0f, 0.0f, 0.0f),Col4(0.0f,1.0f,0.0f,1.0f));
		//Y��
		AddGameObject<ActionLine>(Vec3(0.0f, -100.0f, 0), Vec3(0.0f, 100.0f, 0.0f), Col4(1.0f, 0.0f, 0.0f, 1.0f));
		//Z��
		AddGameObject<ActionLine>(Vec3(0.0f, 0.0f, -100.0f), Vec3(0.0f, 0.0f, 100.0f), Col4(0.0f, 0.0f, 1.0f, 1.0f));
	}

	//�^�C�����O�v���[�g�̍쐬
	void GameStage::CreateTilingPlate() {
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(XM_PIDIV2, 0, 0));
		Vec3 Scale(200.0f, 200.0f, 1.0f);
		Vec3 Pos(0.0f, 0.0f, 0.0f);
		AddGameObject<TilingPlate>(Scale,Qt,Pos,1.0f,1.0f);
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
			//���C���̍쐬
			CreateActionLine();
			//�^�C�����O�v���[�g�̍쐬
			CreateTilingPlate();
			//�v���[���[�̍쐬
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}


}
//end basecross
