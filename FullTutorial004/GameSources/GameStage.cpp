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
		Quat Qt;
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
		//�z��̏�����
		vector<Vec3> Vec = {
			{ 0, 0.25f, 10.0f },
			{ 10.0f, 0.25f, 0.0f },
			{ -10.0f, 0.25f, 0.0f },
			{ 0, 0.25f, -10.0f },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<Enemy1>(v);
		}

		//�z��̏�����
		vector<Vec3> Vec2 = {
			{ 15.0f, 0.25f, 0.0f },
			{ -15.0f, 0.25f, 0.0f },
			{ 0.0f, 0.25f, 15.0f },
			{ 0.0f, 0.25f, -15.0f },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto v : Vec2) {
			AddGameObject<Enemy2>(v);
		}

		//�z��̏�����
		vector<Vec3> Vec3 = {
			{ 10.0f, 0.25f, 10.0f },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto v : Vec3) {
			AddGameObject<Enemy3>(v);
		}



	}





	void GameStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�v���[�g�̍쐬
			CreatePlate();
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
