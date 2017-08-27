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
		auto PlateNode = m_XmlDocReader->GetSelectSingleNode(L"GameStage/GameObjects/TilingPlate");
		if (!PlateNode) {
			throw BaseException(
				L"GameStage/GameObjects/TilingPlate��������܂���",
				L"if (!PlateNode)",
				L"GameStage::CreatePlate()"
			);
		}
		//�g�[�N���i�J�����j�̔z��
		vector<wstring> Tokens;
		auto ScaleNode = XmlDocReader::GetSelectSingleNode(PlateNode, L"Scale");
		auto RotNode = XmlDocReader::GetSelectSingleNode(PlateNode, L"Rot");
		auto PosNode = XmlDocReader::GetSelectSingleNode(PlateNode, L"Pos");
		wstring ScaleStr = XmlDocReader::GetText(ScaleNode);
		wstring RotStr = XmlDocReader::GetText(RotNode);
		wstring PosStr = XmlDocReader::GetText(PosNode);
		//�g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
		Tokens.clear();
		Util::WStrToTokenVector(Tokens, ScaleStr, L',');
		//�e�g�[�N���i�J�����j���X�P�[���A��]�A�ʒu�ɓǂݍ���
		Vec3 Scale(
			(float)_wtof(Tokens[0].c_str()),
			(float)_wtof(Tokens[1].c_str()),
			(float)_wtof(Tokens[2].c_str())
		);
		Tokens.clear();
		Util::WStrToTokenVector(Tokens, RotStr, L',');
		Vec3 Rot;
		//��]�́uXM_PIDIV2�v�̕�����ɂȂ��Ă���ꍇ������
		Rot.x = (Tokens[0] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[0].c_str());
		Rot.y = (Tokens[1] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[1].c_str());
		Rot.z = (Tokens[2] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[2].c_str());
		Tokens.clear();
		Util::WStrToTokenVector(Tokens, PosStr, L',');
		Vec3 Pos(
			(float)_wtof(Tokens[0].c_str()),
			(float)_wtof(Tokens[1].c_str()),
			(float)_wtof(Tokens[2].c_str())
		);
		//�v���[�g�̉�]�̈����̓N�I�[�^�j�I���ɂȂ��Ă���̂ŕϊ�
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Rot);
		//�X�e�[�W�ւ̃Q�[���I�u�W�F�N�g�̒ǉ�
		AddGameObject<TilingPlate>(Scale, Qt, Pos, 1.0f, 1.0f);
	}

	//�{�b�N�X�̍쐬
	void GameStage::CreateFixedBox() {
		auto BoxNodes = m_XmlDocReader->GetSelectNodes(L"GameStage/GameObjects/TilingFixedBox");
		if (!BoxNodes) {
			throw BaseException(
				L"GameStage/GameObjects/TilingFixedBox��������܂���",
				L"if (!BoxNodes)",
				L"GameStage::CreateFixedBox()"
			);
		}
		long CountNode = XmlDocReader::GetLength(BoxNodes);
		for (long i = 0; i < CountNode; i++) {
			auto Node = XmlDocReader::GetItem(BoxNodes, i);
			auto ScaleNode = XmlDocReader::GetSelectSingleNode(Node, L"Scale");
			auto RotNode = XmlDocReader::GetSelectSingleNode(Node, L"Rot");
			auto PosNode = XmlDocReader::GetSelectSingleNode(Node,L"Pos");
			wstring ScaleStr = XmlDocReader::GetText(ScaleNode);
			wstring RotStr = XmlDocReader::GetText(RotNode);
			wstring PosStr = XmlDocReader::GetText(PosNode);
			//�g�[�N���i�J�����j�̔z��
			vector<wstring> Tokens;
			//�g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
			Util::WStrToTokenVector(Tokens, ScaleStr, L',');
			//�e�g�[�N���i�J�����j���X�P�[���A��]�A�ʒu�ɓǂݍ���
			Vec3 Scale(
				(float)_wtof(Tokens[0].c_str()),
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str())
			);
			Tokens.clear();
			Util::WStrToTokenVector(Tokens, RotStr, L',');
			Vec3 Rot;
			//��]�́uXM_PIDIV2�v�̕�����ɂȂ��Ă���ꍇ������
			Rot.x = (Tokens[0] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[0].c_str());
			Rot.y = (Tokens[1] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[1].c_str());
			Rot.z = (Tokens[2] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[2].c_str());
			Tokens.clear();
			Util::WStrToTokenVector(Tokens, PosStr, L',');
			Vec3 Pos(
				(float)_wtof(Tokens[0].c_str()),
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str())
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
		//�Z���}�b�v�̃m�[�h���擾
		auto CellmapNode = m_XmlDocReader->GetSelectSingleNode(L"GameStage/CellMap");
		if (!CellmapNode) {
			throw BaseException(
				L"GameStage/CellMap��������܂���",
				L"if (!CellmapNode)",
				L"GameStage::CreateEnemy()"
			);
		}
		//���e�̕�������擾
		wstring MapStr = XmlDocReader::GetText(CellmapNode);
		vector<wstring> LineVec;
		//�ŏ��Ɂu���s�v���f���~�^�Ƃ���������̔z��ɂ���
		Util::WStrToTokenVector(LineVec, MapStr, L'\n');
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
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			//XML�̓ǂݍ���
			m_XmlDocReader.reset(new XmlDocReader(DataDir + L"GameStage.xml"));
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

}
//end basecross
