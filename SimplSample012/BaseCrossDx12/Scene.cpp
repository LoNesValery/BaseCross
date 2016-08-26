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
		//���̍쐬
		m_SphereObject1 = ObjectFactory::Create<SphereObject>(18, strTexture, false, Vector3(0.0f, 0, 2.0f));
		strTexture = DataDir + L"trace.png";
		//���̍쐬
		m_SphereObject2 = ObjectFactory::Create<SphereObject>(18, strTexture, true, Vector3(0.0f, 0, 0.0f));
	}

	void Scene::OnUpdate() {
		m_SphereObject1->OnUpdate();
		m_SphereObject2->OnUpdate();
	}
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefultViews(Color4(0, 0, 0, 1.0));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefultDraw();
		m_SphereObject1->OnDraw();
		m_SphereObject2->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefultDraw();
	}

}
//end basecross
