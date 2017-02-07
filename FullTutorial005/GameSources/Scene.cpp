
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
			wstring DataDir;
			//�T���v���̂��߃A�Z�b�g�f�B���N�g�����擾
			App::GetApp()->GetAssetsDirectory(DataDir);
			//�e�Q�[���͈ȉ��̂悤�Ƀf�[�^�f�B���N�g�����擾���ׂ�
			//App::GetApp()->GetDataDirectory(DataDir);

			wstring strMusic = DataDir + L"nanika .wav";
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
	}




}
//end basecross
