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
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"trace2.png";
		App::GetApp()->RegisterTexture(L"TRACE2_TX", strTexture);
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		strTexture = DataDir + L"mozo.png";
		App::GetApp()->RegisterTexture(L"MOZO_TX", strTexture);

	}



	//�r���[�ƃ��C�g�̍쐬
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtCamera = ObjectFactory::Create<Camera>();
//		auto PtCamera = ObjectFactory::Create<LookAtCamera>();
		PtrView->SetCamera(PtCamera);
		{
			//LookAtCamera�̏ꍇ
			//PtCamera->SetEye(Vector3(0.0f, 5.0f, -5.0f));
			//PtCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
			//PtCamera->SetMaxArm(20.0f);
			//PtCamera->SetMinArm(1.5f);
			//PtCamera->SetToTargetLerp(0.1f);
			//PtCamera->SetTargetToAt(Vector3(0, 1.0f, 0));
		}
		{
			//�J�����}���ɂ��J�����̏ꍇ
			PtCamera->SetEye(Vector3(0.0f, 2.0f, -2.0f));
			PtCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
		}
		//�V���O�����C�g�̍쐬
		auto PtrSingleLight = CreateLight<SingleLight>();
		//���C�g�̐ݒ�
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}


	//�v���[�g�̍쐬
	void GameStage::CreatePlate() {


		//�X�e�[�W�ւ̃Q�[���I�u�W�F�N�g�̒ǉ�
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		Matrix4X4 WorldMat;
		WorldMat.DefTransformation(
			Vector3(200.0f, 200.0f, 1.0f),
			Qt,
			Vector3(0.0f, 0.0f, 0.0f)
		);
		PtrTrans->SetScale(200.0f, 200.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);

		//�`��R���|�[�l���g�̒ǉ�
		auto DrawComp = Ptr->AddComponent<PNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//�����ɉe���f�肱�ނ悤�ɂ���
		DrawComp->SetOwnShadowActive(true);

		//�`��R���|�[�l���g�e�N�X�`���̐ݒ�
		DrawComp->SetTextureResource(L"SKY_TX");


	}

	//�Œ�̃{�b�N�X�̍쐬
	void GameStage::CreateFixedBox() {
		//�z��̏�����
		vector< vector<Vector3> > Vec = {
			{
				Vector3(1.0f, 0.25f, 1.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(0.0f, 1.5f, 4.0f)
			},
			{
				Vector3(1.5f, 1.0f, 1.5f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(-2.0f, 0.5f, 3.0f)
			},
			{
				Vector3(1.5f, 0.5f, 1.5f),
				Vector3(0.0f,0, 0),
				Vector3(-3.0f, 0.25f, 2.5f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//���̓G�̍쐬
	void GameStage::CreateHead() {
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//�z��̏�����
		vector<Vector3> Vec = {
			{ 3.0f, 0.125f, -2.0f },
		};

		//�z�u�I�u�W�F�N�g�̍쐬
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<HeadEnemy>(Vec[count]);
		}

	}

	//�����G�̍쐬
	void GameStage::CreateMozo() {
		//�z��̏�����
		vector<Vector3> Vec = {
			{ -2.6f, 5.0f, 2.25f },
			{ -2.5f, 5.0f, 3.25f },
			{ -1.5f, 5.0f, 3.0f },
			{ -1.3f, 5.0f, 3.25f },
			{ -1.7f, 5.0f, 2.6f },
			{ -2.0f, 5.0f, 2.3f },
			{ -2.4f, 5.0f, 2.8f },
			{ -2.2f, 5.0f, 2.7f },
			{ -3.5f, 5.0f, 2.0f },
			{ 0.0f, 5.0f, 4.0f },
			{ 0.2f, 5.0f, 4.2f },
			{ -0.2f, 5.0f, 3.8f },
		};

		//�z�u�I�u�W�F�N�g�̍쐬
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<MozoEnemy>(Vec[count]);
		}

	}


	//�Œ�̃V�����_�[�̍쐬
	void GameStage::CreateFixedCylinder() {
		//�z��̏�����
		vector< vector<Vector3> > Vec = {
			{
				Vector3(1.0f, 0.5f, 0.5f),
				Vector3(0, XM_PIDIV4,0),
				Vector3(3.0f, 0.25f, -3.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<FixedCylinder>(v[0], v[1], v[2]);
		}
	}


	//�ǂ̍쐬
	void GameStage::CreateTileWall() {
		auto Grpup = CreateSharedObjectGroup(L"WallGroup");

		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(0, 0, 0));
		auto Ptr = AddGameObject<TileWall>(
			L"WALL_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, 5.0f)
			);
		Grpup->IntoGroup(Ptr);
		Qt.RotationRollPitchYawFromVector(Vector3(0, XM_PIDIV2, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(5.0f, 2.5f, 0.0f)
			);
		Grpup->IntoGroup(Ptr);

		Qt.RotationRollPitchYawFromVector(Vector3(0, -XM_PIDIV2, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(-5.0f, 2.5f, 0.0f)
			);
		Grpup->IntoGroup(Ptr);

		Qt.RotationRollPitchYawFromVector(Vector3(0, XM_PI, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, -5.0f)
			);
		Grpup->IntoGroup(Ptr);

		Qt.RotationRollPitchYawFromVector(Vector3(-XM_PIDIV2, 0, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL_TX",
			10, 10,
			Vector3(10.0f, 10.0f, 1.0f),
			Qt,
			Vector3(0.0f, 5.0f, 0.0f)
			);
		Grpup->IntoGroup(Ptr);


	}

	//������ǂ̍쐬
	void GameStage::CreateThroughWall() {
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(0, 0, 0));
		auto Ptr = AddGameObject<ThroughWall>(
			L"SKY_TX",
			50, 50,
			false,
			Vector3(2.0f, 2.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, 4.9f)
			);

	}


	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	//�J�����}���̍쐬
	void GameStage::CreateCameraman() {
		//�J�����}���̍쐬
		AddGameObject<Cameraman>();
	}



	void GameStage::OnCreate() {
		try {
			//���\�[�X�̍쐬
			CreateResourses();
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�v���[�g�̍쐬
			CreatePlate();
			//�ǂ̍쐬
			CreateTileWall();
			//������ǂ̍쐬
			CreateThroughWall();
			//�Œ�̃{�b�N�X�̍쐬
			CreateFixedBox();
			//���̓G�̍쐬
			CreateHead();
			//�����G�̍쐬
			CreateMozo();
			//�Œ�̃V�����_�[�̍쐬
			CreateFixedCylinder();
			//�v���[���[�̍쐬
			CreatePlayer();
			//�J�����}���̍쐬
			CreateCameraman();

		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
