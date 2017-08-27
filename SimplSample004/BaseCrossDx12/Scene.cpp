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
		//�l�p�`�X�v���C�g�O���[�v�̍쐬
		m_SquareSpriteGroup = ObjectFactory::Create<SquareSpriteGroup>();
	}

	void Scene::OnUpdate() {
		m_SquareSpriteGroup->OnUpdate();
	}
	
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefaultDraw();
		m_SquareSpriteGroup->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefaultDraw();
	}

}
//end basecross
