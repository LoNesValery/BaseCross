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
		SceneInterface(),
		m_CamerEye(0, 5.0, -10.0f),
		m_CamerAt(0, 0, 0),
		m_CamerUp(0, 1.0f, 0),
		m_LightDir(0.5f, -1.0f, 0.5f, 1.0f)
	{
		m_LightDir.normalize();
	}

	void Scene::GetViewProjMatrix(Mat4x4& View, Mat4x4& Proj)const {
		View = XMMatrixLookAtLH(m_CamerEye, m_CamerAt, m_CamerUp);
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);

	}

	void Scene::GetLightDir(Vec4& LightDir)const {
		LightDir = m_LightDir;
	}


	void Scene::OnCreate() {
		wstring DataDir;
		//�T���v���̂��߃A�Z�b�g�f�B���N�g�����擾
		App::GetApp()->GetAssetsDirectory(DataDir);
		//�e�Q�[���͈ȉ��̂悤�Ƀf�[�^�f�B���N�g�����擾���ׂ�
		//App::GetApp()->GetDataDirectory(DataDir);

		wstring strTexture = DataDir + L"sky.jpg";
		//���ʂ̍쐬
		Quat(Vec3(1.0f, 0, 0), XM_PIDIV2);
		m_SquareObject = ObjectFactory::Create<SquareObject>(
			GetThis<Scene>(),
			strTexture,
			Vec3(20.0f, 20.0f, 1.0f),
			Quat(Vec3(1.0f, 0, 0), XM_PIDIV2),
			Vec3(0.0f, 0.0f, 0.0f)
			);
		strTexture = DataDir + L"trace.png";
		//���̍쐬
		m_SphereObject = ObjectFactory::Create<SphereObject>(
			GetThis<Scene>(),
			18, strTexture, true, Vec3(0.0f, 0.5f, 0.0f));
	}

	void Scene::OnUpdate() {
		m_SquareObject->OnUpdate();
		m_SphereObject->OnUpdate();
	}
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefaultDraw();
		m_SquareObject->OnDraw();
		m_SphereObject->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefaultDraw();
	}

}
//end basecross
