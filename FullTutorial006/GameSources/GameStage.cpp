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
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);

		auto ModelMesh = MeshResource::CreateBoneModelMesh(DataDir,L"Chara_R.bmf");
		App::GetApp()->RegisterResource(L"Chara_R_MESH", ModelMesh);
		auto StaticModelMesh = MeshResource::CreateStaticModelMesh(DataDir, L"Character_01.bmf");
		App::GetApp()->RegisterResource(L"MODEL_MESH", StaticModelMesh);
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
		PtrTrans->SetScale(50.0f, 50.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);

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

	//�ǂ�������I�u�W�F�N�g�̍쐬
	void GameStage::CreateSeekObject() {
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//�z��̏�����
		vector<Vector3> Vec = {
			{ 0, 2.0f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<SeekObject>(v);
		}
	}



	//�Œ�̃{�b�N�X�̍쐬
	void GameStage::CreateFixedBox() {
		//�z��̏�����
		vector< vector<Vector3> > Vec = {
			{
				Vector3(5.0f, 0.5f, 5.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(10.0f, 0.25f, 10.0f)
			},
			{
				Vector3(5.0f, 0.5f, 5.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(14.0f, 0.25f, 10.0f)
			},

			{
				Vector3(2.0f, 1.0f, 2.0f),
				Vector3(0, 0, 0),
				Vector3(10.0f, 0.5f, 10.0f)
			},
			{
				Vector3(4.0f, 1.0f, 4.0f),
				Vector3(0, 0, 0),
				Vector3(-10.0f, 0.5f, 10.0f)
			},
			{
				Vector3(10.0f, 0.5f, 10.0f),
				Vector3(-0.5f, 0.0f, -0.5f),
				Vector3(-10.0f, 0.25f, 10.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}


	//�㉺�ړ����Ă���{�b�N�X�̍쐬
	void GameStage::CreateMoveBox() {
		CreateSharedObjectGroup(L"MoveBox");
		AddGameObject<MoveBox>(
			Vector3(5.0f, 0.5f, 5.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, -0.5f, 20.0f)
			);
	}

	//�q�b�g���鋅�̂̍쐬
	void GameStage::CreateSphere() {
		//�z��̏�����
		vector<Vector3> Vec = {
			{ 20.0f, 0, 20.0f },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<SphereObject>(v);
		}
	}

	//�q�b�g����J�v�Z���̍쐬
	void GameStage::CreateCapsule() {
		//�z��̏�����
		vector<Vector3> Vec = {
			{ 20.0f, 0, 0.0f },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<CapsuleObject>(v);
		}

	}

	//�ł��ڂ����̍쐬
	void GameStage::CreateUnevenGround() {
		//�ł��ڂ����̃f�[�^�̍쐬
		AddGameObject<UnevenGroundData>();
		//�z��̏�����
		vector< vector<Vector3> > Vec = {
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 5.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 5.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 7.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 7.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 9.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 9.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 11.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 11.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 13.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 13.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 15.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 15.0f)
			},


			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-2.0f, 0.0f, 5.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-1.0f, 0.0f, 5.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-2.0f, 0.0f, 7.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-1.0f, 0.0f, 7.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-2.0f, 0.0f, 9.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-1.0f, 0.0f, 9.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-2.0f, 0.0f, 11.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-1.0f, 0.0f, 11.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-2.0f, 0.0f, 13.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-1.0f, 0.0f, 13.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-2.0f, 0.0f, 15.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-1.0f, 0.0f, 15.0f)
			},


			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(0.0f, 0.0f, 5.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(1.0f, 0.0f, 5.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(0.0f, 0.0f, 7.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(1.0f, 0.0f, 7.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(0.0f, 0.0f, 9.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(1.0f, 0.0f, 9.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(0.0f, 0.0f, 11.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(1.0f, 0.0f, 11.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(0.0f, 0.0f, 13.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(1.0f, 0.0f, 13.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(0.0f, 0.0f, 15.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(1.0f, 0.0f, 15.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(2.0f, 0.0f, 5.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(3.0f, 0.0f, 5.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(2.0f, 0.0f, 7.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(3.0f, 0.0f, 7.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(2.0f, 0.0f, 9.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(3.0f, 0.0f, 9.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(2.0f, 0.0f, 11.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(3.0f, 0.0f, 11.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(2.0f, 0.0f, 13.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(3.0f, 0.0f, 13.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(2.0f, 0.0f, 15.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(3.0f, 0.0f, 15.0f)
			},



		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<UnevenGround>(v[0], v[1], v[2]);
		}

	}


	//�Œ�̃��f���̍쐬
	void GameStage::CreateStaticModel() {
		AddGameObject<StaticModel>(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(5.0f, 0.25f, 5.0f)
			);

	}
	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}


	void GameStage::OnCreate() {
		try {
			//���\�[�X�̍쐬
			CreateResourses();
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
			//�J�v�Z���̍쐬
			CreateCapsule();
			//�Œ�̃��f���̍쐬
			CreateStaticModel();
			//�ǂ�������I�u�W�F�N�g�̍쐬
			CreateSeekObject();
			//�ł��ڂ����̍쐬
			CreateUnevenGround();
			//�v���[���[�̍쐬
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
