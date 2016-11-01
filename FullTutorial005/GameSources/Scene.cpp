
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
			wstring strMusic = App::GetApp()->m_wstrRelativeDataPath + L"nanika .wav";
			App::GetApp()->RegisterWav(L"Nanika", strMusic);

			//�I�[�f�B�I�̏�����
			m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
			m_AudioObjectPtr->AddAudioResource(L"Nanika");
			m_AudioObjectPtr->Start(L"Nanika", XAUDIO2_LOOP_INFINITE, 0.1f);

			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
			ResetActiveStage<GameStage>();
		}
		catch (...) {
			throw;
		}
	}

	Scene::~Scene() {
		m_AudioObjectPtr->Stop(L"Nanika");
	}




}
//end basecross
