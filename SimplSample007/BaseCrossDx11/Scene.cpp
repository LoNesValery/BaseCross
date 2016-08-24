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
		wstring strTexture = DataDir + L"wall.jpg";
		m_WallSprite = ObjectFactory::Create<WallSprite>(strTexture, false, Vector2(0, 0));
		strTexture = DataDir + L"trace.png";
		m_SquareSprite = ObjectFactory::Create<SquareSprite>(strTexture, true, Vector2(0, 0));
	}

	void Scene::OnUpdate() {
		m_WallSprite->OnUpdate();
		m_SquareSprite->OnUpdate();
	}
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefultViews(Color4(0, 0, 0, 1.0f));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefultDraw();
		m_WallSprite->OnDraw();
		m_SquareSprite->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefultDraw();
	}

}
//end basecross