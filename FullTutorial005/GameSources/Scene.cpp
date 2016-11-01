
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------

	void Scene::OnCreate(){
		try {
			wstring strMusic = App::GetApp()->m_wstrRelativeDataPath + L"nanika .wav";
			App::GetApp()->RegisterWav(L"Nanika", strMusic);

			//オーディオの初期化
			m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
			m_AudioObjectPtr->AddAudioResource(L"Nanika");
			m_AudioObjectPtr->Start(L"Nanika", XAUDIO2_LOOP_INFINITE, 0.1f);

			//最初のアクティブステージの設定
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
