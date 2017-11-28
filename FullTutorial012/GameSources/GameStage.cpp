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
		auto PtrLookAtCamera = ObjectFactory::Create<LookAtCamera>(10.0f);
		PtrView->SetCamera(PtrLookAtCamera);
		PtrLookAtCamera->SetEye(Vec3(0.0f, 5.0f, -20.0f));
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

	//�����v�Z�{�b�N�X�̍쐬
	void GameStage::CreatePhysicsBox() {
		//���̑�
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeStatic, Vec3(6.0f, 0.5f, 6.0f), Quat(), Vec3(0.0f, 0.25f, 5.0f));
		//�e�����󂯂�{�b�N�X
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive,Vec3(2.0f, 0.25f, 1.0f), Quat(), Vec3(0.0f, 0.625f, 5.0f));
		Quat Qt1,Qt2, Qt3, Qt4;
		Qt1.rotationZ(2.0f);
		Qt2.rotationY(0.7f);
		Qt3.rotationZ(-2.0f);
		Qt4.rotationY(-0.7f);
		//�ォ��~���Ă���{�b�N�X
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(2.0f, 0.25f, 1.0f), Qt2 * Qt1, Vec3(0.0f, 3.5f, 5.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt1 * Qt2, Vec3(0.0f, 6.0f, 5.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt1, Vec3(1.0f, 4.0f, 6.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt2, Vec3(-2.0f, 4.0f, 4.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt3, Vec3(2.0f, 4.0f, 3.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt4, Vec3(1.0f, 6.0f, 3.0f));

		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt1, Vec3(2.5f, 8.0f, 6.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt2, Vec3(-3.0f, 8.0f, 4.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt3, Vec3(-3.0f, 8.0f, 3.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt4, Vec3(2.5f, 8.0f, 7.0f));

	}

	void GameStage::OnCreate() {
		try {
			SetPhysicsManager(true);
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�v���[�g�̍쐬
			CreatePlate();
			//�����v�Z�{�b�N�X�̍쐬
			CreatePhysicsBox();
			//�v���[���[�̍쐬
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
