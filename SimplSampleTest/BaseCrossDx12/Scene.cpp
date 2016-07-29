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
		//�����̂̍쐬
		m_NormalTextureBox = ObjectFactory::Create<NormalTextureBox>(Vector3(0, 0, 0));
	}

	void Scene::OnUpdate() {
		m_NormalTextureBox->OnUpdate();
	}
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefultViews(Color4(0, 0, 0, 1.0));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefultDraw();
		m_NormalTextureBox->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefultDraw();
	}

}
//end basecross
