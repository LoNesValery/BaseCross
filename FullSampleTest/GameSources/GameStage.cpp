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

	//���\�[�X�̍쐬
	void GameStage::CreateResourses() {
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
	}

	//�r���[�ƃ��C�g�̍쐬
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vector3(0, 2.0, -5.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
		//�V���O�����C�g�̍쐬
		auto PtrSingleLight = CreateLight<SingleLight>();
		//���C�g�̐ݒ�
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}



	//�{�b�N�X�̍쐬
	void GameStage::CreateNormalTextureBox() {
		AddGameObjectWithParam<NormalTextureBox>(Vector3(0.0f, 0.0f, .0f));
	}


	void GameStage::OnCreate() {
		try {
			//���\�[�X�̍쐬
			CreateResourses();
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�{�b�N�X�̍쐬
			CreateNormalTextureBox();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
