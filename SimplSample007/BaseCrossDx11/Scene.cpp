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
		//�T���v���̂��߃A�Z�b�g�f�B���N�g�����擾
		App::GetApp()->GetAssetsDirectory(DataDir);
		//�e�Q�[���͈ȉ��̂悤�Ƀf�[�^�f�B���N�g�����擾���ׂ�
		//App::GetApp()->GetDataDirectory(DataDir);

		//�l�p�`�̍쐬
		wstring strTexture = DataDir + L"wall.jpg";
		m_WallSprite = ObjectFactory::Create<WallSprite>(strTexture, false, Vec2(0, 0));
		strTexture = DataDir + L"trace.png";
		m_SquareSprite = ObjectFactory::Create<SquareSprite>(strTexture, true, Vec2(0, 0));
	}

	void Scene::OnUpdate() {
		m_WallSprite->OnUpdate();
		m_SquareSprite->OnUpdate();
	}
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefaultDraw();
		m_WallSprite->OnDraw();
		m_SquareSprite->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefaultDraw();
	}

}
//end basecross
