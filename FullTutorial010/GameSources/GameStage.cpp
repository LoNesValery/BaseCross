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
		PtrLookAtCamera->SetEye(Vector3(0.0f, 5.0f, -5.0f));
		PtrLookAtCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
		//���C�e�B���O�C��
	}


	//�^�C�����O�v���[�g�̍쐬
	void GameStage::CreatePlate() {
		//CSV�̍s�P�ʂ̔z��
		vector<wstring> LineVec;
		//0�Ԗڂ̃J������L"TilingPlate"�ł���s�𔲂��o��
		m_GameStageCsv.GetSelect(LineVec, 0, L"TilingPlate");
		//1�s�����o�ł��Ȃ���΍쐬���Ȃ�
		if (!LineVec.empty()) {
			//�g�[�N���i�J�����j�̔z��
			vector<wstring> Tokens;
			//0�s�ڂ��g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
			Util::WStrToTokenVector(Tokens, LineVec[0], L',');
			//�e�g�[�N���i�J�����j���X�P�[���A��]�A�ʒu�ɓǂݍ���
			Vector3 Scale(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);
			Vector3 Rot;
			//��]�́uXM_PIDIV2�v�̕�����ɂȂ��Ă���ꍇ������
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());
			Vector3 Pos(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			//�v���[�g�̉�]�̈����̓N�I�[�^�j�I���ɂȂ��Ă���̂ŕϊ�
			Quaternion Qt;
			Qt.RotationRollPitchYawFromVector(Rot);
			//�X�e�[�W�ւ̃Q�[���I�u�W�F�N�g�̒ǉ�
			AddGameObject<TilingPlate>(Scale, Qt, Pos, 1.0f, 1.0f);
		}
	}

	//�Z���}�b�v�̍쐬
	void GameStage::CreateStageCellMap() {
		float  PieceSize = 1.0f;
		auto Ptr = AddGameObject<StageCellMap>(Vector3(-5.0f, 0, -5.0f), PieceSize, 10, 10);
		//�Z���}�b�v�̋���\������ꍇ�͈ȉ��̐ݒ�
		//Ptr->SetDrawActive(true);
		//����ɃZ���̃C���f�b�N�X�ƃR�X�g��\������ꍇ�͈ȉ��̐ݒ�
		//Ptr->SetCellStringActive(true);
		SetSharedGameObject(L"StageCellMap", Ptr);
	}


	//�{�b�N�X�̍쐬
	void GameStage::CreateFixedBox() {
		//�{�b�N�X�̃O���[�v���쐬
		auto BoxGroup = CreateSharedObjectGroup(L"FixedBoxes");
		//CSV�̍s�P�ʂ̔z��
		vector<wstring> LineVec;
		//0�Ԗڂ̃J������L"TilingFixedBox"�ł���s�𔲂��o��
		m_GameStageCsv.GetSelect(LineVec, 0, L"TilingFixedBox");
		for (auto& v : LineVec) {
			//�g�[�N���i�J�����j�̔z��
			vector<wstring> Tokens;
			//�g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
			Util::WStrToTokenVector(Tokens, v, L',');
			//�e�g�[�N���i�J�����j���X�P�[���A��]�A�ʒu�ɓǂݍ���
			Vector3 Scale(
				(float)_wtof(Tokens[1].c_str()), 
				(float)_wtof(Tokens[2].c_str()), 
				(float)_wtof(Tokens[3].c_str())
			);
			Vector3 Rot;
			//��]�́uXM_PIDIV2�v�̕�����ɂȂ��Ă���ꍇ������
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());
			Vector3 Pos(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			//�e�l����������̂ŃI�u�W�F�N�g�쐬
			auto BoxPtr = AddGameObject<TilingFixedBox>(Scale, Rot, Pos, 1.0f, 1.0f);
			//�{�b�N�X�̃O���[�v�ɒǉ�
			BoxGroup->IntoGroup(BoxPtr);
		}
		//�ŏ���2�̃Z���}�b�v�ւ̃{�b�N�X�̃R�X�g�ݒ�
		SetCellMapCost();
	}

	//�Œ�̃{�b�N�X�̃R�X�g���Z���}�b�v�ɔ��f
	void GameStage::SetCellMapCost() {
		//�Z���}�b�v����FixedBox�̏����Z�b�g
		auto PtrCellmap = GetSharedGameObject<StageCellMap>(L"StageCellMap");
		auto BoxGroup = GetSharedObjectGroup(L"FixedBoxes");
		//�Z���}�b�v����Z���̔z����擾
		auto& CellVec = PtrCellmap->GetCellVec();
		//�{�b�N�X�O���[�v����{�b�N�X�̔z����擾
		auto& BoxVec = BoxGroup->GetGroupVector();
		vector<AABB> ObjectsAABBVec;
		for (auto& v : BoxVec) {
			auto FixedBoxPtr = dynamic_pointer_cast<TilingFixedBox>(v.lock());
			if (FixedBoxPtr) {
				auto ColPtr = FixedBoxPtr->GetComponent<CollisionObb>();
				//�{�b�N�X�̏Փ˔��肩���b�s���O����AABB���擾���ĕۑ�
				ObjectsAABBVec.push_back(ColPtr->GetWrappingAABB());
			}
		}
		//�Z���z�񂩂�Z�����X�L����
		for (auto& v : CellVec) {
			for (auto& v2 : v) {
				for (auto& vObj : ObjectsAABBVec) {
					if (HitTest::AABB_AABB_NOT_EQUAL(v2.m_PieceRange, vObj)) {
						//�{�b�N�X��ABB��NOT_EQUAL�ŏՓ˔���
						v2.m_Cost = -1;
						break;
					}
				}
			}
		}
	}


	//�X�p�[�N�̍쐬
	void GameStage::CreateSpark() {
		auto MultiSparkPtr = AddGameObject<MultiSpark>();
		//���L�I�u�W�F�N�g�ɃX�p�[�N��o�^
		SetSharedGameObject(L"MultiSpark", MultiSparkPtr);
	}
	//���̍쐬
	void GameStage::CreateFire() {
		auto MultiFirePtr = AddGameObject<MultiFire>();
		//���L�I�u�W�F�N�g�ɉ���o�^
		SetSharedGameObject(L"MultiFire", MultiFirePtr);
	}


	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		auto SwordPtr = AddGameObject<Sword>(
			Vector3(0.2, 0.5, 0.2),
			L"PlayerSword"
			);
		SetSharedGameObject(L"PlayerSword", SwordPtr);
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
		//HP�o�[
		AddGameObject<HPManeger<Player>>(PlayerPtr,Color4(0,1,0,1));
	}

	//�G�̍쐬
	void GameStage::CreateEnemy() {
		auto MapPtr = GetSharedGameObject<StageCellMap>(L"StageCellMap");
		vector< vector<Vector3> > Vec1 = {
			{
				Vector3(0.5f, 0.5f, 0.5f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(-3.5f, 0.25f, 3.5f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec1) {
			auto SwordPtr = AddGameObject<Sword>(
				Vector3(0.2, 0.7, 0.2),
				L"EnemySword"
				);
			SetSharedGameObject(L"EnemySword", SwordPtr);
			auto EnemyPtr = AddGameObject<Enemy>(MapPtr, v[0], v[1], v[2]);
			//HP�o�[
			AddGameObject<HPManeger<Enemy>>(EnemyPtr, Color4(1, 0, 0, 1));
		}
	}


	void GameStage::OnCreate() {
		try {
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			////CSV�t�@�C���̓ǂݍ���
			m_GameStageCsv.SetFileName(DataDir + L"GameStage.csv");
			m_GameStageCsv.ReadCsv();

			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�v���[�g�̍쐬
			CreatePlate();
			//�Z���}�b�v�̍쐬
			CreateStageCellMap();
			//�{�b�N�X�̍쐬
			CreateFixedBox();
			//�X�p�[�N�̍쐬
			CreateSpark();
			//���̍쐬
			CreateFire();
			//�v���[���[�̍쐬
			CreatePlayer();
			//�G�̍쐬
			CreateEnemy();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
