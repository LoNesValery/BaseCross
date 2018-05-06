/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------

	//�r���[�ƃ��C�g�̍쐬
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrLookAtCamera = ObjectFactory::Create<LookAtCamera>();
		PtrView->SetCamera(PtrLookAtCamera);
		PtrLookAtCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		PtrLookAtCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}


	//�^�C�����O�v���[�g�̍쐬
	void GameStage::CreatePlate() {
		//CSV�̍s�P�ʂ̔z��
		vector<wstring> LineVec;
		//0�Ԗڂ̃J������L"TilingPlate"�ł���s�𔲂��o��
		m_GameStageCsvB.GetSelect(LineVec, 0, L"TilingPlate");
		//1�s�����o�ł��Ȃ���΍쐬���Ȃ�
		if (!LineVec.empty()) {
			//�g�[�N���i�J�����j�̔z��
			vector<wstring> Tokens;
			//0�s�ڂ��g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
			Util::WStrToTokenVector(Tokens, LineVec[0], L',');
			//�e�g�[�N���i�J�����j���X�P�[���A��]�A�ʒu�ɓǂݍ���
			Vec3 Scale(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);
			Vec3 Rot;
			//��]�́uXM_PIDIV2�v�̕�����ɂȂ��Ă���ꍇ������
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());
			Vec3 Pos(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			//�v���[�g�̉�]�̈����̓N�I�[�^�j�I���ɂȂ��Ă���̂ŕϊ�
			Quat Qt;
			Qt.rotationRollPitchYawFromVector(Rot);
			//�X�e�[�W�ւ̃Q�[���I�u�W�F�N�g�̒ǉ�
			AddGameObject<TilingPlate>(Scale, Qt, Pos, 1.0f, 1.0f);
		}
	}

	//�{�b�N�X�̍쐬
	void GameStage::CreateFixedBox() {
		//CSV�̍s�P�ʂ̔z��
		vector<wstring> LineVec;
		//0�Ԗڂ̃J������L"TilingFixedBox"�ł���s�𔲂��o��
		m_GameStageCsvB.GetSelect(LineVec, 0, L"TilingFixedBox");
		for (auto& v : LineVec) {
			//�g�[�N���i�J�����j�̔z��
			vector<wstring> Tokens;
			//�g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
			Util::WStrToTokenVector(Tokens, v, L',');
			//�e�g�[�N���i�J�����j���X�P�[���A��]�A�ʒu�ɓǂݍ���
			Vec3 Scale(
				(float)_wtof(Tokens[1].c_str()), 
				(float)_wtof(Tokens[2].c_str()), 
				(float)_wtof(Tokens[3].c_str())
			);
			Vec3 Rot;
			//��]�́uXM_PIDIV2�v�̕�����ɂȂ��Ă���ꍇ������
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());
			Vec3 Pos(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			//�e�l����������̂ŃI�u�W�F�N�g�쐬
			AddGameObject<TilingFixedBox>(Scale, Rot, Pos, 1.0f, 1.0f);
		}
	}



	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	//�G�̍쐬
	void GameStage::CreateEnemy() {
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto Group = CreateSharedObjectGroup(L"EnemyGroup");
		//CSV�̑S�̂̔z��
		//CSV���炷�ׂĂ̍s�𔲂��o��
		vector<wstring>& LineVec = m_GameStageCsv.GetCsvVec();
		for (size_t i = 0; i < LineVec.size();i++) {
			//�g�[�N���i�J�����j�̔z��
			vector<wstring> Tokens;
			//�g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
			Util::WStrToTokenVector(Tokens, LineVec[i], L',');
			for (size_t j = 0; j < Tokens.size(); j++) {
				//X��Z�̈ʒu���v�Z
				float XPos = (float)((int)j - 19);
				float ZPos = (float)(19 - (int)i);
				if (Tokens[j] == L"1") {
					AddGameObject<Enemy1>(Vec3(XPos,0.25f,ZPos));
				}
				else if (Tokens[j] == L"2") {
					AddGameObject<Enemy2>(Vec3(XPos, 0.25f, ZPos));

				}
				else if (Tokens[j] == L"3") {
					AddGameObject<Enemy3>(Vec3(XPos, 0.25f, ZPos));

				}
			}
		}
	}


	void GameStage::OnCreate() {
		try {
			//�V�[���̎擾
			auto PtrScene = App::GetApp()->GetScene<Scene>();
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			switch (PtrScene->GetStageNum()) {
			case 1:
				//CSV�t�@�C���̊m��
				m_GameStageCsv.SetFileName(DataDir + L"GameStage1.csv");
				break;
			case 2:
				//CSV�t�@�C���̊m��
				m_GameStageCsv.SetFileName(DataDir + L"GameStage2.csv");
				break;
			case 3:
				//CSV�t�@�C���̊m��
				m_GameStageCsv.SetFileName(DataDir + L"GameStage3.csv");
				break;
			default:
				//����ȊO�̓X�e�[�W1
				//CSV�t�@�C���̊m��
				m_GameStageCsv.SetFileName(DataDir + L"GameStage1.csv");
				break;
			}
			m_GameStageCsv.ReadCsv();
			////CSV�t�@�C������B�̓ǂݍ���
			m_GameStageCsvB.SetFileName(DataDir + L"GameStageB.csv");
			m_GameStageCsvB.ReadCsv();
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�v���[�g�̍쐬
			CreatePlate();
			//�{�b�N�X�̍쐬
			CreateFixedBox();
			//�v���[���[�̍쐬
			CreatePlayer();
			//�G�̍쐬
			CreateEnemy();
		}
		catch (...) {
			throw;
		}
	}

	//�X�V
	void GameStage::OnUpdate() {
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//B�{�^��
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				//�V�[���̎擾
				auto PtrScene = App::GetApp()->GetScene<Scene>();
				PostEvent(0.0f, GetThis<GameStage>(), PtrScene, L"ToSelectStage");
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	�Z���N�g�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	void SelectStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(Vec3(0.0f, 2.0f, -3.0f));
		PtrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();

	}

	//�X�v���C�g�̍쐬
	void SelectStage::CreateAnimeSprite() {
		auto PtrSp = AddGameObject<AnimeSprite>(L"STAGE1_TX", true,
			Vec2(256.0f, 64.0f), Vec2(0.0f, 100.0f));
		PtrSp->SetSelect(true);
		m_SpVec[0] = PtrSp;
		PtrSp = AddGameObject<AnimeSprite>(L"STAGE2_TX", true,
			Vec2(256.0f, 64.0f), Vec2(0.0f, 0.0f));
		m_SpVec[1] = PtrSp;
		PtrSp = AddGameObject<AnimeSprite>(L"STAGE3_TX", true,
			Vec2(256.0f, 64.0f), Vec2(0.0f, -100.0f));
		m_SpVec[2] = PtrSp;
	}

	//������
	void SelectStage::OnCreate() {
		CreateViewLight();
		//�X�v���C�g�̍쐬
		CreateAnimeSprite();
	}

	void SelectStage::ChangeSelect(int num) {
		for (int i = 0; i < 3; i++) {
			shared_ptr<AnimeSprite> shptr = m_SpVec[i].lock();
			if (shptr) {
				if ((i + 1) == num) {
					shptr->SetSelect(true);
				}
				else {
					shptr->SetSelect(false);
				}
			}
		}
	}


	//�X�V
	void SelectStage::OnUpdate() {
		//�V�[���̎擾
		auto PtrScene = App::GetApp()->GetScene<Scene>();
		int StageNum = PtrScene->GetStageNum();
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//�Q�[���X�e�[�W��
			//A�{�^��
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
				PostEvent(0.0f, GetThis<SelectStage>(), PtrScene, L"ToGameStage");
				return;
			}
			if (!m_CntrolLock) {
				if (CntlVec[0].fThumbLY >= 0.8f) {
					StageNum--;
					if (StageNum < 1) {
						StageNum = 3;
					}
					m_CntrolLock = true;
					PtrScene->SetStageNum(StageNum);
					ChangeSelect(StageNum);
				}
				else if (CntlVec[0].fThumbLY <= -0.8f) {
					StageNum++;
					if (StageNum > 3) {
						StageNum = 1;
					}
					m_CntrolLock = true;
					PtrScene->SetStageNum(StageNum);
					ChangeSelect(StageNum);
				}
			}
			else {
				if (CntlVec[0].fThumbLY == 0.0f) {
					m_CntrolLock = false;
				}
			}
		}
	}


}
//end basecross
