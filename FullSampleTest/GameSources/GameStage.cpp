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

	//�{�b�N�X�̍쐬
	void GameStage::CreateNormalTextureBox() {
		AddGameObjectWithParam<NormalTextureBox>(Vector3(0.0f, 0.0f, .0f));
	}


	void GameStage::OnCreate() {
		try {
			//���\�[�X�̍쐬
			CreateResourses();
			//�{�b�N�X�̍쐬
			CreateNormalTextureBox();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
