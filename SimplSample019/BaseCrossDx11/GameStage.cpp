/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	���݂̗��Ȏ��X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	//�Œ�̃{�b�N�X�̍쐬
	void ScienceRoom::CreateFixedBox() {
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

	//�����h�A�̍쐬
	void ScienceRoom::CreateRoomDoor() {
		//�I�u�W�F�N�g�̍쐬
		AddGameObject<RoomDoor>(
			L"TRACE_TX",
			Vector3(0.5f, 1.0f, 0.1f), 
			Vector3(0.0f, XM_PI, 0.0f), 
			Vector3(0.0f, 0.5f, -5.0f)
			);

	}

	//�E�C���h�E�̍쐬
	void ScienceRoom::CreateRoomWindow() {
		//�I�u�W�F�N�g�̍쐬
		AddGameObject<RoomWindow>(
			Vector3(1.0f, 1.0f, 0.1f),
			Vector3(0.0f, XM_PIDIV2, 0.0f),
			Vector3(5.0f, 1.0f, -2.0f)
			);


	}

	//���̍쐬
	void ScienceRoom::CreateFixHead() {
		//�I�u�W�F�N�g�̍쐬
		AddGameObject<FixedHead>(
			Vector3(3.0f, 1.0f, -3.0f)
			);

	}


	//�ǂ̍쐬
	void ScienceRoom::CreateTileWall() {
		auto Grpup = CreateSharedObjectGroup(L"WallGroup");

		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(0, 0, 0));
		auto Ptr = AddGameObject<TileWall>(
			L"WALL2_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, 5.0f)
			);
		Grpup->IntoGroup(Ptr);
		Qt.RotationRollPitchYawFromVector(Vector3(0, XM_PIDIV2, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL2_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(5.0f, 2.5f, 0.0f)
			);
		Grpup->IntoGroup(Ptr);

		Qt.RotationRollPitchYawFromVector(Vector3(0, -XM_PIDIV2, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL2_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(-5.0f, 2.5f, 0.0f)
			);
		Grpup->IntoGroup(Ptr);

		Qt.RotationRollPitchYawFromVector(Vector3(0, XM_PI, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL2_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, -5.0f)
			);
		Grpup->IntoGroup(Ptr);

		Qt.RotationRollPitchYawFromVector(Vector3(-XM_PIDIV2, 0, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL2_TX",
			10, 10,
			Vector3(10.0f, 10.0f, 1.0f),
			Qt,
			Vector3(0.0f, 5.0f, 0.0f)
			);
		Grpup->IntoGroup(Ptr);


	}



	//--------------------------------------------------------------------------------------
	//	���݂̗��Ȏ��X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	//�v���[�g�̍쐬
	void NowScienceRoom::CreatePlate() {

		//�X�e�[�W�ւ̃Q�[���I�u�W�F�N�g�̒ǉ�
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(10.0f, 10.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);

		auto ColPtr = Ptr->AddComponent<CollisionRect>();
		//�`��R���|�[�l���g�̒ǉ�
		auto DrawComp = Ptr->AddComponent<PNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//�����ɉe���f�肱�ނ悤�ɂ���
		DrawComp->SetOwnShadowActive(true);

		//�`��R���|�[�l���g�e�N�X�`���̐ݒ�
		DrawComp->SetTextureResource(L"WALL_TX");

	}

	//�r���[�ƃ��C�g�̍쐬
	void NowScienceRoom::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtCamera = ObjectFactory::Create<LookAtCamera>();
		PtrView->SetCamera(PtCamera);
		{
			//LookAtCamera�̏ꍇ
			PtCamera->SetMaxArm(2.0f);
			PtCamera->SetMinArm(1.0f);
			PtCamera->SetAt(Vector3(0.0f, 0.125f, -3.0f));
			PtCamera->SetEye(Vector3(0.0f, 0.75f, -4.5f));
			PtCamera->UpdateArmLengh();
			PtCamera->SetToTargetLerp(0.1f);
			PtCamera->SetTargetToAt(Vector3(0, 0.125f, 0));
		}
		//�V���O�����C�g�̍쐬
		auto PtrSingleLight = CreateLight<SingleLight>();
		//���C�g�̐ݒ�
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}

	//������ǂ̍쐬
	void NowScienceRoom::CreateThroughWall() {
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(0, 0, 0));

		auto Ptr = AddGameObject<TileWall>(
			L"SKY_TX",
			1, 1,
			Vector3(2.0f, 2.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, 4.99f)
			);
		SetSharedGameObject(L"Goal", Ptr);


		auto ThroughPtr = AddGameObject<ThroughWall>(
			L"SKY_TX",
			50, 50,
			false,
			Vector3(2.0f, 2.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, 4.8f)
			);

	}

	//���̓G�̍쐬
	void NowScienceRoom::CreateHead() {
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//�z��̏�����
		vector<Vector3> Vec = {
			{ 0.0f, 0.125f, 0.0f },
		};

		//�z�u�I�u�W�F�N�g�̍쐬
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<HeadEnemy>(Vec[count]);
		}

	}

	//�����G�̍쐬
	void NowScienceRoom::CreateMozo() {
		//�z��̏�����
		vector<Vector3> Vec = {
			{ -2.6f, 5.0f, 2.25f },
			{ -2.5f, 5.0f, 3.25f },
			{ -1.5f, 5.0f, 3.0f },
			{ -1.3f, 5.0f, 3.25f },
			{ -1.7f, 5.0f, 2.6f },
			{ -2.0f, 5.0f, 2.3f },
			{ -2.4f, 5.0f, 2.8f },
			{ -3.3f, 5.0f, 2.2f },
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
	void NowScienceRoom::CreateFixedCylinder() {
		//�I�u�W�F�N�g�̍쐬
		auto PtrCylinder = AddGameObject<FixedCylinder>(
			Vector3(1.0f, 0.5f, 0.5f),
			Vector3(0, XM_PIDIV4, 0),
			Vector3(3.0f, 0.25f, -3.0f)
			);
		//�V�F�A�z��ɒǉ�
		SetSharedGameObject(L"FixedCylinder", PtrCylinder);
	}

	//���a�̌Œ�̃V�����_�[�̍쐬
	void NowScienceRoom::CreatteClearFixedCylinder() {
		//�I�u�W�F�N�g�̍쐬
		auto PtrCylinder = AddGameObject<FixedCylinder>(
			Vector3(0.5f, 1.0f, 0.5f),
			Vector3(0, XM_PIDIV4, 0),
			Vector3(3.0f, 0.5f, -3.0f)
			);
		//�V�F�A�z��ɒǉ�
		SetSharedGameObject(L"FixedCylinder", PtrCylinder);

	}


	//�V�����_�[�ɕt���̐��������̍쐬
	void NowScienceRoom::CreatePopupBalloon() {
		auto PtrBalloon = AddGameObject<PopupBalloon>(L"BALLOON_0_TX", Vector3(1.0f, 0.5f, 1.0f));
		//�V�F�A�z��ɒǉ�
		SetSharedGameObject(L"PopupBalloon", PtrBalloon);
	}

	//�v���C���[�̍쐬
	void NowScienceRoom::CreatePlayer() {
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>(
			Vector3(0, 0.125, -3.0f), Vector3(0, 0, 0));
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	void NowScienceRoom::OnCreate() {
		try {
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
			//�����h�A�̍쐬
			CreateRoomDoor();
			//�E�C���h�E�̍쐬
			CreateRoomWindow();
			auto PtrParam = App::GetApp()->GetScene<Scene>()->GetGameParamaters();
			if (!PtrParam.m_IsHeadTaskClear) {
				//�����G�̍쐬
				CreateMozo();
				//���̓G�̍쐬
				CreateHead();
				//�Œ�̃V�����_�[�̍쐬
				CreateFixedCylinder();
			}
			else {
				//�G�ł͂Ȃ���
				CreateFixHead();
				//���a�ȌŒ�̃V�����_�[�̍쐬
				CreatteClearFixedCylinder();
			}
			//�V�����_�[�ɕt���̐��������̍쐬
			CreatePopupBalloon();
			//�v���[���[�̍쐬
			CreatePlayer();

		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	�ߋ��̗��Ȏ��X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	//�v���[�g�̍쐬
	void PastScienceRoom::CreatePlate() {

		//�X�e�[�W�ւ̃Q�[���I�u�W�F�N�g�̒ǉ�
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(10.0f, 10.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);

		auto ColPtr = Ptr->AddComponent<CollisionRect>();
		//�`��R���|�[�l���g�̒ǉ�
		auto DrawComp = Ptr->AddComponent<PNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//�����ɉe���f�肱�ނ悤�ɂ���
		DrawComp->SetOwnShadowActive(true);

		//�`��R���|�[�l���g�e�N�X�`���̐ݒ�
		DrawComp->SetTextureResource(L"FLOOR_TX");

	}
	//�r���[�ƃ��C�g�̍쐬
	void PastScienceRoom::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtCamera = ObjectFactory::Create<LookAtCamera>();
		PtrView->SetCamera(PtCamera);
		{
			//LookAtCamera�̏ꍇ
			PtCamera->SetMaxArm(2.0f);
			PtCamera->SetMinArm(1.0f);
			PtCamera->SetAt(Vector3(0.0f, 0.125f, -3.0f));
			PtCamera->SetEye(Vector3(0.0f, 0.75f, -4.5f));
			PtCamera->UpdateArmLengh();
			PtCamera->SetToTargetLerp(0.1f);
			PtCamera->SetTargetToAt(Vector3(0, 0.125f, 0));
		}
		//�V���O�����C�g�̍쐬
		auto PtrSingleLight = CreateLight<SingleLight>();
		//���C�g�̐ݒ�
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}

	//������ǂ̍쐬
	void PastScienceRoom::CreateThroughWall() {
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(0, 0, 0));

		auto Ptr = AddGameObject<TileWall>(
			L"SKY_TX",
			1, 1,
			Vector3(2.0f, 2.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, 4.99f)
			);
		SetSharedGameObject(L"Goal", Ptr);


		auto ThroughPtr = AddGameObject<ThroughWall>(
			L"SKY_TX",
			50, 50,
			false,
			Vector3(2.0f, 2.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, 4.8f)
			);

	}

	//���̓G�̍쐬
	void PastScienceRoom::CreateHead() {
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//�z��̏�����
		vector<Vector3> Vec = {
			{ 0.0f, 0.125f, 0.0f },
		};

		//�z�u�I�u�W�F�N�g�̍쐬
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<HeadEnemy>(Vec[count]);
		}

	}

	//�Œ�̃V�����_�[�̍쐬
	void PastScienceRoom::CreateFixedCylinder() {
		//�I�u�W�F�N�g�̍쐬
		auto PtrCylinder = AddGameObject<FixedCylinder>(
			Vector3(0.5f, 1.0f, 0.5f),
			Vector3(0, XM_PIDIV4, 0),
			Vector3(3.0f, 0.5f, -3.0f)
			);
		//�V�F�A�z��ɒǉ�
		SetSharedGameObject(L"FixedCylinder", PtrCylinder);
	}

	//�v���C���[�̍쐬
	void PastScienceRoom::CreatePlayer() {
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>(
			Vector3(0, 0.125, -3.0f), Vector3(0, 0, 0));
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	void PastScienceRoom::OnCreate() {
		try {
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
			//�����h�A�̍쐬
			CreateRoomDoor();
			//�E�C���h�E�̍쐬
			CreateRoomWindow();
			//���̍쐬
			CreateFixHead();
			//�Œ�̃V�����_�[�̍쐬
			CreateFixedCylinder();
			//�v���[���[�̍쐬
			CreatePlayer();

		}
		catch (...) {
			throw;
		}
	}


	//--------------------------------------------------------------------------------------
	//	�N���A�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	void ClearStage::OnCreate() {
		try {
			auto PtrView = CreateView<SingleView>();
			auto PtrStr = AddGameObject<StringGameObject>();
			auto& param = App::GetApp()->GetScene<Scene>()->GetGameParamaters();
			auto LifesStr = Util::UintToWStr(param.m_PlayerLife);
			wstring ClearStr = L"�N���A\n�c��̃��C�t: ";
			ClearStr += LifesStr;
			ClearStr += L"\n������x���ɂ�A�{�^��";

			PtrStr->SetText(ClearStr);
		}
		catch (...) {
			throw;
		}
	}

	void ClearStage::OnUpdate() {
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//A�{�^���������ꂽ�u�ԂȂ�A�������ɖ߂�
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
				auto& param = App::GetApp()->GetScene<Scene>()->GetGameParamaters();
				param.m_BalloonOpenTask = false;
				param.m_IsHeadTaskClear = false;
				param.m_PlayerLife = 5;

				auto ScenePtr = App::GetApp()->GetScene<Scene>();
				PostEvent(0.0f, GetThis<ObjectInterface>(), ScenePtr, L"ToNowScienceRoom");
			}
		}

	}

	//--------------------------------------------------------------------------------------
	//	�Q�[���I�[�o�[�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	void GameOverStage::OnCreate() {
		try {
			auto PtrView = CreateView<SingleView>();
			auto PtrStr = AddGameObject<StringGameObject>();
			PtrStr->SetText(L"�Q�[���I�[�o�[\n������x���ɂ�A�{�^��");
		}
		catch (...) {
			throw;
		}
	}

	void GameOverStage::OnUpdate() {
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//A�{�^���������ꂽ�u�ԂȂ�A�������ɖ߂�
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
				auto& param = App::GetApp()->GetScene<Scene>()->GetGameParamaters();
				param.m_BalloonOpenTask = false;
				param.m_IsHeadTaskClear = false;
				param.m_PlayerLife = 5;

				auto ScenePtr = App::GetApp()->GetScene<Scene>();
				PostEvent(0.0f, GetThis<ObjectInterface>(), ScenePtr, L"ToNowScienceRoom");
			}
		}

	}


}
//end basecross
