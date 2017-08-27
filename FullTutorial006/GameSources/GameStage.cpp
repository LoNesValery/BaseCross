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


	//�v���[�g�̍쐬
	void GameStage::CreatePlate() {
		//�X�e�[�W�ւ̃Q�[���I�u�W�F�N�g�̒ǉ�
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quat Qt(Vec3(1.0f, 0, 0), XM_PIDIV2);
		Qt.rotationRollPitchYawFromVector(Vec3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(50.0f, 50.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);

		auto ColPtr = Ptr->AddComponent<CollisionRect>();
		//�`��R���|�[�l���g�̒ǉ�
		auto DrawComp = Ptr->AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		DrawComp->SetFogEnabled(true);
		//�����ɉe���f�肱�ނ悤�ɂ���
		DrawComp->SetOwnShadowActive(true);

		//�`��R���|�[�l���g�e�N�X�`���̐ݒ�
		DrawComp->SetTextureResource(L"SKY_TX");
	}

	//�ǂ�������I�u�W�F�N�g�̍쐬
	void GameStage::CreateSeekObject() {
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//�z��̏�����
		vector<Vec3> Vec = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
		};

		//�z�u�I�u�W�F�N�g�̍쐬
		//�i���o�[�X�N�G�A�̍쐬
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<SeekObject>(Vec[count]);
			//�i���o�[�X�N�G�A���쐬���Ċ֘A������
			AddGameObject<NumberSquare>(Ptr, count);
		}

	}



	//�Œ�̃{�b�N�X�̍쐬
	void GameStage::CreateFixedBox() {
		//�z��̏�����
		vector< vector<Vec3> > Vec = {
			{
				Vec3(5.0f, 0.5f, 5.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(10.0f, 0.25f, 10.0f)
			},
			{
				Vec3(5.0f, 0.5f, 5.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(14.0f, 0.25f, 10.0f)
			},

			{
				Vec3(2.0f, 1.0f, 2.0f),
				Vec3(0, 0, 0),
				Vec3(10.0f, 0.5f, 10.0f)
			},
			{
				Vec3(4.0f, 1.0f, 4.0f),
				Vec3(0, 0, 0),
				Vec3(-10.0f, 0.5f, 10.0f)
			},
			{
				Vec3(10.0f, 0.5f, 10.0f),
				Vec3(-0.5f, 0.0f, -0.5f),
				Vec3(-10.0f, 0.25f, 10.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
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
		auto PlayerPtr = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	//�㉺�ړ����Ă���{�b�N�X�̍쐬
	void GameStage::CreateMoveBox() {
		CreateSharedObjectGroup(L"MoveBox");
		AddGameObject<MoveBox>(
			Vec3(5.0f, 0.5f, 5.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 20.0f)
			);
	}

	//�q�b�g���鋅�̂̍쐬
	void GameStage::CreateSphere() {
		//�z��̏�����
		vector<Vec3> Vec = {
			{ 20.0f, 0, 25.0f },
			{ 20.0f, 0, 0.0f },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<SphereObject>(v);
		}
	}


	//�������̃X�v���C�g�쐬
	void GameStage::CreateTraceSprite() {
		AddGameObject<TraceSprite>( true,
			Vec2(200.0f, 200.0f), Vec3(-500.0f, -280.0f,0.0f));
	}


	//�ǖ͗l�̃X�v���C�g�쐬
	void GameStage::CreateWallSprite() {
		AddGameObject<WallSprite>(L"WALL_TX", false,
			Vec2(200.0f, 200.0f), Vec2(500.0f, -280.0f));
	}


	//�X�N���[������X�v���C�g�쐬
	void GameStage::CreateScrollSprite() {
		AddGameObject<ScrollSprite>(L"TRACE_TX",true,
			Vec2(160.0f, 40.0f),Vec2(500.0f,-280.0f));
	}

	//����ŉ�]���闧����
	void GameStage::CreateRollingCube() {
		Quat Qt(Vec3(0.0f, 0.0, 1.0), XM_PIDIV4);
		AddGameObject<RollingCube>(true,
			Vec3(64.0f,64.0f,64.0f), 
			Qt,
			Vec3(-440,320,100.0f)
			);

	}

	//����ŉ�]����Wall������
	void GameStage::CreateRollingWallCube() {
		Quat Qt(Vec3(0.0f, 0.0, 1.0), XM_PIDIV4);
		AddGameObject<RollingWallCube>(
			L"WALL_TX",
			false,
			Vec3(64.0f, 64.0f, 64.0f),
			Qt,
			Vec3(-320, 320, 100.0f)
			);

	}
	//����������
	void GameStage::CreateWhiteCube() {
		Quat Qt(Vec3(0.0f, 1.0, 1.0), 0);
		AddGameObject<WhiteCube>(
			Vec3(1.0f, 1.0f, 1.0f),
			Qt,
			Vec3(0.0f,1.0f, 10.0f)
			);

	}
	//�`�󂪕ς�鋅��
	void GameStage::CreateTransSphere() {
		AddGameObject<TransSphere>(
			L"WALL_TX",
			false,
			Vec3(1.0f, 1.0f, 1.0f),
			Quat(),
			Vec3(10.0f, 2.0f, 10.0f)
			);
	}

	//�ł��ڂ����̍쐬
	void GameStage::CreateUnevenGround() {
		//�ł��ڂ����̃f�[�^�̍쐬
		AddGameObject<UnevenGroundData>();
		//�z��̏�����
		vector< vector<Vec3> > Vec = {
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(-4.0f, 0.0f, 5.0f)
			},
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-3.0f, 0.0f, 5.0f)
			},

			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(-4.0f, 0.0f, 7.0f)
			},
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-3.0f, 0.0f, 7.0f)
			},

			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(-4.0f, 0.0f, 9.0f)
			},
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-3.0f, 0.0f, 9.0f)
			},

			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(-4.0f, 0.0f, 11.0f)
			},
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-3.0f, 0.0f, 11.0f)
			},

			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(-4.0f, 0.0f, 13.0f)
			},
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-3.0f, 0.0f, 13.0f)
			},

			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(-4.0f, 0.0f, 15.0f)
			},
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-3.0f, 0.0f, 15.0f)
			},


		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<UnevenGround>(v[0], v[1], v[2]);
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





	void GameStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�v���[�g�̍쐬
			CreatePlate();
			//�Œ�̃{�b�N�X�̍쐬
			CreateFixedBox();
			//�㉺�ړ����Ă���{�b�N�X�̍쐬
			CreateMoveBox();
			//���̍쐬
			CreateSphere();
			//�ǂ�������I�u�W�F�N�g�̍쐬
			CreateSeekObject();
			//�������̃X�v���C�g�쐬
			CreateTraceSprite();
			//�ǖ͗l�̃X�v���C�g�쐬
			CreateWallSprite();
			//�X�N���[������X�v���C�g�쐬
			CreateScrollSprite();
			//����ŉ�]���闧����
			CreateRollingCube();
			//����ŉ�]����Wall������
			CreateRollingWallCube();
			//����������
			CreateWhiteCube();
			//����Ō`�󂪕ς�鋅��
			CreateTransSphere();
			//�ł��ڂ����̍쐬
			CreateUnevenGround();
			//�X�p�[�N�̍쐬
			CreateSpark();
			//���̍쐬
			CreateFire();
			//�v���[���[�̍쐬
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
