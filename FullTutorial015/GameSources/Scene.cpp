
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
		wstring strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"trace2.png";
		App::GetApp()->RegisterTexture(L"TRACE2_TX", strTexture);

		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		//���f�B�A�f�B���N�g�����擾
		App::GetApp()->GetDataDirectory(DataDir);
		strTexture = DataDir + L"stage1.png";
		App::GetApp()->RegisterTexture(L"STAGE1_TX", strTexture);
		strTexture = DataDir + L"stage2.png";
		App::GetApp()->RegisterTexture(L"STAGE2_TX", strTexture);
		strTexture = DataDir + L"stage3.png";
		App::GetApp()->RegisterTexture(L"STAGE3_TX", strTexture);


	}

	void Scene::OnCreate() {
		try {
			//���\�[�X�쐬
			CreateResourses();
			//�������g�ɃC�x���g�𑗂�
			//����ɂ��e�X�e�[�W��I�u�W�F�N�g��Create���ɃV�[���ɃA�N�Z�X�ł���
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToSelectStage");
		}
		catch (...) {
			throw;
		}
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToSelectStage") {
			m_SrageNum = 1;
			//�Z���N�g�X�e�[�W�̐ݒ�
			ResetActiveStage<SelectStage>();
		}
		else if (event->m_MsgStr == L"ToGameStage") {
			//�Q�[���X�e�[�W�̐ݒ�
			ResetActiveStage<GameStage>();
		}
	}


}
//end basecross
