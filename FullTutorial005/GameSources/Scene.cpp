
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

		//�v���[�g�p
		wstring strTexture = DataDir + L"gray.png";
		App::GetApp()->RegisterTexture(L"GRAY_TX", strTexture);
		strTexture = DataDir + L"normal3.png";
		App::GetApp()->RegisterTexture(L"NORMAL3_TX", strTexture);
		//�v���C���[�p
		strTexture = DataDir + L"Brown.png";
		App::GetApp()->RegisterTexture(L"BROWN_TX", strTexture);
		strTexture = DataDir + L"normal1.png";
		App::GetApp()->RegisterTexture(L"NORMAL1_TX", strTexture);
		//Seek�I�u�W�F�N�g�p
		strTexture = DataDir + L"Green.png";
		App::GetApp()->RegisterTexture(L"GREEN_TX", strTexture);
		strTexture = DataDir + L"normal2.png";
		App::GetApp()->RegisterTexture(L"NORMAL2_TX", strTexture);
		//Fixed�p
		strTexture = DataDir + L"Blue.png";
		App::GetApp()->RegisterTexture(L"BLUE_TX", strTexture);
		//L"NORMAL2_TX"
		strTexture = DataDir + L"Wall2.png";
		App::GetApp()->RegisterTexture(L"WALL2_TX", strTexture);
		strTexture = DataDir + L"Wall2_normal.png";
		App::GetApp()->RegisterTexture(L"WALL2_NORMAL_TX", strTexture);
		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		strTexture = DataDir + L"wall_normal.png";
		App::GetApp()->RegisterTexture(L"WALL_NORMAL_TX", strTexture);
		//L"BROWN_TX"
		strTexture = DataDir + L"Floor_normal.png";
		App::GetApp()->RegisterTexture(L"FLOOR_NORMAL_TX", strTexture);
		//MoveBox�p
		strTexture = DataDir + L"Red.png";
		App::GetApp()->RegisterTexture(L"RED_TX", strTexture);
		//FLOOR_NORMAL_TX
		//UnevenGround�p
		strTexture = DataDir + L"Brown2.png";
		App::GetApp()->RegisterTexture(L"BROWN2_TX", strTexture);
		//WALL2_NORMAL_TX

		//���̑�
		strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"Floor.png";
		App::GetApp()->RegisterTexture(L"FLOOR_TX", strTexture);

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

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
			ResetActiveStage<GameStage>();
		}
	}


}
//end basecross
