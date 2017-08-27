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

		wstring strTexture = DataDir + L"sky.jpg";
		//���̍쐬
		m_SphereObject1 = ObjectFactory::Create<SphereObject>(18, strTexture, false, Vec3(0.0f, 0, 2.0f));
		strTexture = DataDir + L"trace.png";
		//���̍쐬
		m_SphereObject2 = ObjectFactory::Create<SphereObject>(18, strTexture, true, Vec3(0.0f, 0, 0.0f));
	}

	void Scene::OnUpdate() {
		m_SphereObject1->OnUpdate();
		m_SphereObject2->OnUpdate();
	}
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefaultDraw();
		m_SphereObject1->OnDraw();
		m_SphereObject2->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefaultDraw();
	}

}
//end basecross
