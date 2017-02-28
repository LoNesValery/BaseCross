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


	//���\�[�X�̍쐬
	void GameStage::CreateResourses() {
		wstring DataDir;
		//�T���v���̂��߃A�Z�b�g�f�B���N�g�����擾
		App::GetApp()->GetAssetsDirectory(DataDir);
		//�e�Q�[���͈ȉ��̂悤�Ƀf�[�^�f�B���N�g�����擾���ׂ�
		//App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"trace2.png";
		App::GetApp()->RegisterTexture(L"TRACE2_TX", strTexture);
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
	}



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
	}


	//�v���[�g�̍쐬
	void GameStage::CreatePlate() {
		//�X�e�[�W�ւ̃Q�[���I�u�W�F�N�g�̒ǉ�
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(20.0f, 40.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 20.0f);
		auto ColPtr = Ptr->AddComponent<CollisionRect>();
		//�`��R���|�[�l���g�̒ǉ�
		auto DrawComp = Ptr->AddComponent<BcPNTStaticDraw>();
		DrawComp->SetFogEnabled(true);
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//�����ɉe���f�肱�ނ悤�ɂ���
		DrawComp->SetOwnShadowActive(true);
		//�`��R���|�[�l���g�e�N�X�`���̐ݒ�
		DrawComp->SetTextureResource(L"SKY_TX");
	}

	//�Z���}�b�v�̍쐬
	void GameStage::CreateStageCellMap() {
		auto Group = CreateSharedObjectGroup(L"CellMap");
		float  PieceSize = 1.0f;
		auto Ptr = AddGameObject<StageCellMap>(Vector3(-10.0f, 0, 4.0f),PieceSize,20,7);
		//�Z���}�b�v�̋���\������ꍇ�͈ȉ��̐ݒ�
		Ptr->SetDrawActive(true);
		//����ɃZ���̃C���f�b�N�X�ƃR�X�g��\������ꍇ�͈ȉ��̐ݒ�
		Ptr->SetCellStringActive(true);
		SetSharedGameObject(L"StageCellMap1", Ptr);
		//�O���[�v�ɒǉ�
		Group->IntoGroup(Ptr);

		Ptr = AddGameObject<StageCellMap>(Vector3(-10.0f, 0, 16.0f), PieceSize, 20, 7);
		//�Z���}�b�v�̋���\������ꍇ�͈ȉ��̐ݒ�
		Ptr->SetDrawActive(true);
		//����ɃZ���̃C���f�b�N�X�ƃR�X�g��\������ꍇ�͈ȉ��̐ݒ�
		//Ptr->SetCellStringActive(true);
		SetSharedGameObject(L"StageCellMap2", Ptr);
		//�O���[�v�ɒǉ�
		Group->IntoGroup(Ptr);

		//�ȉ�3�ڂ̃Z���}�b�v�̓O���[�v��ʂɂ���
		//���I�ɃZ���}�b�v��ύX����G�p
		auto Group2 = CreateSharedObjectGroup(L"CellMap2");

		Ptr = AddGameObject<StageCellMap>(Vector3(-10.0f, 0, 28.0f), PieceSize, 20, 7);
		//�Z���}�b�v�̋���\������ꍇ�͈ȉ��̐ݒ�
		Ptr->SetDrawActive(true);
		//����ɃZ���̃C���f�b�N�X�ƃR�X�g��\������ꍇ�͈ȉ��̐ݒ�
		Ptr->SetCellStringActive(true);
		SetSharedGameObject(L"StageCellMap3", Ptr);
		//�O���[�v�ɒǉ�
		Group2->IntoGroup(Ptr);


	}



	//�Œ�̃{�b�N�X�̍쐬
	void GameStage::CreateFixedBox() {
		//�z��̏�����
		vector< vector<Vector3> > Vec = {
			{
				Vector3(1.0f, 0.5f, 40.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(9.5f, 0.25f, 20.0f)
			},
			{
				Vector3(1.0f, 0.5f, 40.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(-9.5f, 0.25f, 20.0f)
			},

			{
				Vector3(20.0f, 0.5f, 1.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(0.0f, 0.25f, 0.5f)
			},

			{
				Vector3(20.0f, 0.5f, 1.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(0.0f, 0.25f, 39.5f)
			},

			///////


			{
				Vector3(2.0f, 0.5f, 1.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(-8.0f, 0.25f, 6.5f)
			},

			{
				Vector3(1.0f, 0.5f, 2.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(-4.5f, 0.25f, 7.0f)
			},

			{
				Vector3(2.0f, 0.5f, 1.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(0.0f, 0.25f, 7.5f)
			},

			{
				Vector3(1.0f, 0.5f, 2.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(4.5f, 0.25f, 7.0f)
			},


			{
				Vector3(2.0f, 0.5f, 1.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(8.0f, 0.25f, 6.5f)
			},


			///////


			{
				Vector3(2.0f, 0.5f, 1.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(-8.0f, 0.25f,18.5f)
			},

			{
				Vector3(1.0f, 0.5f, 2.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(-4.5f, 0.25f, 19.0f)
			},

			{
				Vector3(2.0f, 0.5f, 1.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(0.0f, 0.25f, 19.5f)
			},

			{
				Vector3(1.0f, 0.5f, 2.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(4.5f, 0.25f, 19.0f)
			},


			{
				Vector3(2.0f, 0.5f, 1.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(8.0f, 0.25f, 18.5f)
			},

			///////


			{
				Vector3(2.0f, 0.5f, 1.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(-8.0f, 0.25f, 30.5f)
			},

			{
				Vector3(1.0f, 0.5f, 2.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(-4.5f, 0.25f, 31.0f)
			},

			{
				Vector3(2.0f, 0.5f, 1.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(0.0f, 0.25f, 31.5f)
			},

			{
				Vector3(1.0f, 0.5f, 2.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(4.5f, 0.25f, 31.0f)
			},


			{
				Vector3(2.0f, 0.5f, 1.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(8.0f, 0.25f, 30.5f)
			},
		};

		//�{�b�N�X�̃O���[�v���쐬
		auto BoxGroup = CreateSharedObjectGroup(L"FixedBoxes");
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			auto BoxPtr = AddGameObject<FixedBox>(v[0], v[1], v[2]);
			//�{�b�N�X�̃O���[�v�ɒǉ�
			BoxGroup->IntoGroup(BoxPtr);
		}
		//�ŏ���2�̃Z���}�b�v�ւ̃{�b�N�X�̃R�X�g�ݒ�
		SetCellMapCost(L"CellMap");
		//���̃Z���}�b�v�ւ̃{�b�N�X�̃R�X�g�ݒ�
		SetCellMapCost(L"CellMap2");
	}

	//�Œ�̃{�b�N�X�̃R�X�g���Z���}�b�v�ɔ��f
	void GameStage::SetCellMapCost(const wstring& CellMapGroupName) {
		//�Z���}�b�v����FixedBox�̏����Z�b�g
		auto Group = GetSharedObjectGroup(CellMapGroupName);
		auto BoxGroup = GetSharedObjectGroup(L"FixedBoxes");

		//�Z���}�b�v�O���[�v���擾
		for (auto& gv : Group->GetGroupVector()) {
			auto MapPtr = dynamic_pointer_cast<StageCellMap>(gv.lock());
			if (MapPtr) {
				//�Z���}�b�v����Z���̔z����擾
				auto& CellVec = MapPtr->GetCellVec();
				//�{�b�N�X�O���[�v����{�b�N�X�̔z����擾
				auto& BoxVec = BoxGroup->GetGroupVector();
				vector<AABB> ObjectsAABBVec;
				for (auto& v : BoxVec) {
					auto FixedBoxPtr = dynamic_pointer_cast<FixedBox>(v.lock());
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
		}
	}


	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		CreateSharedObjectGroup(L"AttackBall");
		//�A�^�b�N�{�[����10�p�ӂ���
		for (int i = 0; i < 10; i++) {
			AddGameObject<AttackBall>();
		}
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>(Vector3(0.0f, 0.125f, 2.0f));
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	//�G�̍쐬
	void GameStage::CreateEnemy() {
		auto MapPtr = GetSharedGameObject<StageCellMap>(L"StageCellMap1");
		vector< vector<Vector3> > Vec1 = {
			{
				Vector3(0.25f, 0.25f, 0.25f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(-2.5f, 0.125f, 9.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec1) {
			AddGameObject<Enemy>(MapPtr,v[0], v[1], v[2]);
		}

		MapPtr = GetSharedGameObject<StageCellMap>(L"StageCellMap2");
		vector< vector<Vector3> > Vec2 = {
			{
				Vector3(0.25f, 0.25f, 0.25f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(4.5f, 0.125f, 21.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec2) {
			AddGameObject<Enemy>(MapPtr, v[0], v[1], v[2]);
		}

		//3�ڂ̓G

		MapPtr = GetSharedGameObject<StageCellMap>(L"StageCellMap3");
		vector< vector<Vector3> > Vec3 = {
			{
				Vector3(0.25f, 0.25f, 0.25f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(6.5f, 0.125f, 33.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec3) {
			//�Z���}�b�v��ύX����G
			AddGameObject<TestCellChangeEnemy>(MapPtr, v[0], v[1], v[2]);
		}
	}


	void GameStage::OnCreate() {
		try {
			//���\�[�X�̍쐬
			CreateResourses();
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�v���[�g�̍쐬
			CreatePlate();
			//�Z���}�b�v�̍쐬
			CreateStageCellMap();
			//�Œ�̃{�b�N�X�̍쐬
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