/*!
@file Scene.cpp
@brief シーンなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	Scene::Scene() :
		SceneInterface()
	{
	}

	void Scene::OnCreate() {
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"sky.jpg";
		//立方体グループの作成
		m_CubeObjectGroup = ObjectFactory::Create<CubeObjectGroup>(strTexture);
	}

	void Scene::OnUpdate() {
		m_CubeObjectGroup->OnUpdate();
	}
	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefultViews(Color4(0, 0, 0, 1.0f));
		//デフォルト描画の開始
		Dev->StartDefultDraw();
		m_CubeObjectGroup->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefultDraw();
	}

}
//end basecross
