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
	Scene::Scene() :
		SceneInterface()
	{
	}


	void Scene::OnCreate() {
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"sky.jpg";
		//�����̃O���[�v�̍쐬
		m_CubeObjectGroup = ObjectFactory::Create<CubeObjectGroup>(strTexture);
	}

	void Scene::OnUpdate() {
		m_CubeObjectGroup->OnUpdate();
	}
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefultViews(Color4(0, 0, 0, 1.0f));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefultDraw();
		m_CubeObjectGroup->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefultDraw();
	}

}
//end basecross
