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
	}

	void GameStage::OnCreate() {
		try {
			//���\�[�X�̍쐬
			CreateResourses();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
