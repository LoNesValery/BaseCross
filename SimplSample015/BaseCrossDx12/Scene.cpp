/*!
@file Scene.cpp
@brief �V�[���Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate() {
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"sky.jpg";
		//�����̂̍쐬
		m_CubeObject1 = ObjectFactory::Create<CubeObject>(strTexture, false, Vector3(-1.0f, 0, 2.0f), true);
		m_CubeObject2 = ObjectFactory::Create<CubeObject>(strTexture, false, Vector3(1.0f, 0, 2.0f), false);
		strTexture = DataDir + L"trace.png";
		//�����̂̍쐬
		m_CubeObject3 = ObjectFactory::Create<CubeObject>(strTexture, true, Vector3(-2.0f, 0, 0), true);
		m_CubeObject4 = ObjectFactory::Create<CubeObject>(strTexture, true, Vector3(2.0f, 0, 0), false);
	}

	void Scene::OnUpdate() {
		m_CubeObject1->OnUpdate();
		m_CubeObject2->OnUpdate();
		m_CubeObject3->OnUpdate();
		m_CubeObject4->OnUpdate();
	}
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefultViews(Color4(0, 0, 0, 1.0));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefultDraw();
		m_CubeObject1->OnDraw();
		m_CubeObject2->OnDraw();
		m_CubeObject3->OnDraw();
		m_CubeObject4->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefultDraw();
	}

}
//end basecross
