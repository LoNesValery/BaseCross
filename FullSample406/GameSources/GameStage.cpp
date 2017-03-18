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
	}


	//�v���[�g�̍쐬
	void GameStage::CreatePlate() {
		//�X�e�[�W�ւ̃Q�[���I�u�W�F�N�g�̒ǉ�
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quaternion Qt(Vector3(1.0f, 0, 0), XM_PIDIV2);
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(50.0f, 50.0f, 1.0f);
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
		DrawComp->SetTextureResource(L"SKY_TX");
	}



	//�Œ�̉�]�{�b�N�X�̍쐬
	void GameStage::CreateRollingBox() {
		auto GroupPtr = AddGameObject<RollingBoxGroup>();
		//�V�F�A�I�u�W�F�N�g�ɐݒ�
		SetSharedGameObject(L"RollingBoxGroup", GroupPtr);
		Vector3 Scale(1.25f, 0.5f, 1.25f);
		Vector3 Rot(0.0f, 0.0f, 0.0f);
		Vector3 Pos;

		//�z��̏�����
		vector< vector<Vector3> > Vec;
		//�����̃I�u�W�F�N�g
		for (int x = -20; x < 20; x+= 2) {
			for (int z = 5; z < 20; z+= 2) {
				Pos = Vector3((float)x, 0.25f, (float)z);
				vector<Vector3> temp = { Scale, Rot, Pos };
				Vec.push_back(temp);
			}
		}
		//��O���̃I�u�W�F�N�g
		for (int x = -20; x < 20; x += 2) {
			for (int z = -5; z > -20; z -= 2) {
				Pos = Vector3((float)x, 0.25f, (float)z);
				vector<Vector3> temp = { Scale, Rot, Pos };
				Vec.push_back(temp);
			}
		}
		//�I�u�W�F�N�g�̍쐬
		float RotParam = 1.0f;
		for (auto v : Vec) {
			//�����ŉ�]�W����0.5����1.5�̊Ԃɐݒ�
			RotParam = Util::RandZeroToOne(true) + 0.5f;
			AddGameObject<RollingBox>(v[0], v[1], v[2], RotParam);
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



	void GameStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�v���[�g�̍쐬
			CreatePlate();
			//�Œ�̉�]�{�b�N�X�̍쐬
			CreateRollingBox();
			//�v���[���[�̍쐬
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
