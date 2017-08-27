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

	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	void GameStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�I�u�W�F�N�g�̃O���[�v���쐬����
			auto Group = CreateSharedObjectGroup(L"EnemyGroup");
			//�Q�[���I�u�W�F�N�g�r���_�[
			GameObjecttCSVBuilder Builder;
			//�Q�[���I�u�W�F�N�g�̓o�^
			Builder.Register<TilingPlate>(L"TilingPlate");
			Builder.Register<TilingFixedBox>(L"TilingFixedBox");
			Builder.Register<Enemy1>(L"Enemy1");
			Builder.Register<Enemy2>(L"Enemy2");
			Builder.Register<Enemy3>(L"Enemy3");
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			//CSV����Q�[���I�u�W�F�N�g�̍\�z
			wstring CSVStr = DataDir + L"GameStage";
			CSVStr += Util::IntToWStr(m_StageNum);
			CSVStr += L".csv";
			Builder.Build(GetThis<Stage>(), CSVStr);
			//�v���[���[�̍쐬
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
