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
		m_LightDir.Normalize();
	}

	void Scene::GetViewProjMatrix(Matrix4X4& View, Matrix4X4& Proj)const {
		View.LookAtLH(m_CamerEye, m_CamerAt, m_CamerUp);
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj.PerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);

	}

	void Scene::GetLightDir(Vector4& LightDir)const {
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
		Quaternion(Vector3(1.0f, 0, 0), XM_PIDIV2);
		m_SquareObject = ObjectFactory::Create<SquareObject>(
			GetThis<Scene>(),
			strTexture,
			Vector3(20.0f, 20.0f, 1.0f),
			Quaternion(Vector3(1.0f, 0, 0), XM_PIDIV2),
			Vector3(0.0f, 0.0f, 0.0f)
			);
		wstring strTexture2 = DataDir + L"trace.png";
		//���̍쐬
		m_SphereObject = ObjectFactory::Create<SphereObject>(
			GetThis<Scene>(),
			18, strTexture2, true, Vector3(0.0f, 0.5f, 0.0f));

		m_BoxObjectVec.push_back(
			ObjectFactory::Create<BoxObject>(
				GetThis<Scene>(), strTexture, false,
				Vector3(3.0f, 1.0f, 3.0f),
				Quaternion(),
				Vector3(-3.0f, 0.5f, 0.0f)
				)
		);

		m_BoxObjectVec.push_back(
			ObjectFactory::Create<BoxObject>(
				GetThis<Scene>(), strTexture, false,
				Vector3(3.0f, 1.0f, 3.0f),
				Quaternion(Vector3(0, 0, 1), XM_PIDIV4),
				Vector3(3.0f, 0.5f, 0.0f)
				)
		);

		//PNT�`��I�u�W�F�N�g�̍쐬
		m_PNTDrawObject = ObjectFactory::Create<PNTDrawObject>(GetThis<Scene>());

		strTexture = DataDir + L"trace.png";
		m_WallSprite = ObjectFactory::Create<WrappedSprite>(strTexture, true,
			Vector2(160, 160),
			Vector2(-480, 260),
			4,4);


	}

	void Scene::OnUpdate() {
		m_SquareObject->OnUpdate();
		for (auto& v : m_BoxObjectVec) {
			v->OnUpdate();
		}
		m_SphereObject->OnUpdate();
		m_PNTDrawObject->OnUpdate();
		m_WallSprite->OnUpdate();
	}
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Color4(0, 0, 0, 1.0f));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefaultDraw();
		m_SquareObject->OnDraw();
		m_SphereObject->OnDraw();
		for (auto& v : m_BoxObjectVec) {
			v->OnDraw();
		}
		m_PNTDrawObject->OnDraw();
		m_WallSprite->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefaultDraw();
	}

}
//end basecross
