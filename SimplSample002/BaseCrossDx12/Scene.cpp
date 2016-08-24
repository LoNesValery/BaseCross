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
		//�O�p�`�̍쐬
		m_TriangleSprite = ObjectFactory::Create<TriangleSprite>();
	}

	void Scene::OnUpdate() {
		m_TriangleSprite->OnUpdate();
	}
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefultViews(Color4(0, 0, 0, 1.0));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefultDraw();
		//�O�p�`�̕`��
		m_TriangleSprite->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefultDraw();
	}

}
//end basecross