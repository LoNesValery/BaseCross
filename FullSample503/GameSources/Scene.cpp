
/*!
@file Scene.cpp
@brief �V�[������
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	void Scene::CreateResourses() {
		wstring DataDir;
		//�T���v���̂��߃A�Z�b�g�f�B���N�g�����擾
		App::GetApp()->GetAssetsDirectory(DataDir);
		//�e�Q�[���͈ȉ��̂悤�Ƀf�[�^�f�B���N�g�����擾���ׂ�
		//App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);

		//�f�[�^�f�B���N�g�����擾
		wstring MediaDir;
		App::GetApp()->GetDataDirectory(MediaDir);
		//�X�^�e�B�b�N���f���̃^���W�F���g�t�����\�[�X
		auto StaticModelMesh = MeshResource::CreateStaticModelMeshWithTangent(MediaDir + L"TestFbx\\", L"TangentTestNonAnimation.bmf");
		App::GetApp()->RegisterResource(L"MODEL_MESH_WITH_TAN", StaticModelMesh);
		//�{�[�����f���̃^���W�F���g�t�����\�[�X
		auto BoneModelMesh = MeshResource::CreateBoneModelMeshWithTangent(MediaDir + L"TestFbx\\", L"TangentTest.bmf");
		App::GetApp()->RegisterResource(L"BONE_MESH_WITH_TAN", BoneModelMesh);

		//�@���}�b�v
		strTexture = MediaDir + L"TestFbx\\" + L"NormalMap.png";
		App::GetApp()->RegisterTexture(L"MODEL_NORMAL_TX", strTexture);


	}

	void Scene::OnCreate(){
		try {
			//���\�[�X�쐬
			CreateResourses();


			//�������g�ɃC�x���g�𑗂�
			//����ɂ��e�X�e�[�W��I�u�W�F�N�g��Create���ɃV�[���ɃA�N�Z�X�ł���
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
		}
		catch (...) {
			throw;
		}
	}

	Scene::~Scene() {
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
			ResetActiveStage<GameStage>();
		}
	}



}
//end basecross
