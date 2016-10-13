
/*!
@file Scene.cpp
@brief �V�[������
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------

	void Scene::OnCreate(){
		try {
			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
			ResetActiveStage<MenuStage>();
		}
		catch (...) {
			throw;
		}
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
			ResetActiveStage<GameStage>();
		}
		else if (event->m_MsgStr == L"ToMenuStage") {
			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
			ResetActiveStage<MenuStage>();
		}

	}




}
//end basecross
