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

		//�l�p�`�̍쐬
		wstring strTexture = DataDir + L"sky.jpg";
		m_SquareSprite1 = ObjectFactory::Create<SquareSprite>(strTexture, false, Vector2(-100.0f, 0));
		strTexture = DataDir + L"trace.png";
		m_SquareSprite2 = ObjectFactory::Create<SquareSprite>(strTexture, true, Vector2(100.0f, 0));
	}

	void Scene::OnUpdate() {
		m_SquareSprite1->OnUpdate();
		m_SquareSprite2->OnUpdate();
	}
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefultViews(Color4(0, 0, 0, 1.0));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefultDraw();
		m_SquareSprite1->OnDraw();
		m_SquareSprite2->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefultDraw();
	}

}
//end basecross
