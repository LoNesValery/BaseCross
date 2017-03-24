
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
		wstring strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);

		//�f�[�^�f�B���N�g�����擾
		wstring MediaDir;
		App::GetApp()->GetDataDirectory(MediaDir);
		//�X�^�e�B�b�N���f���̃��\�[�X
		auto StaticModelMesh = MeshResource::CreateStaticModelMesh(MediaDir + L"Chara1\\", L"Character_01.bmf");
		App::GetApp()->RegisterResource(L"MODEL_MESH", StaticModelMesh);
		//�{�[�����f���̃��\�[�X
		auto BoneModelMesh = MeshResource::CreateBoneModelMesh(MediaDir + L"Chara_R\\", L"Chara_R.bmf");
		App::GetApp()->RegisterResource(L"BONE_MESH", BoneModelMesh);

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
