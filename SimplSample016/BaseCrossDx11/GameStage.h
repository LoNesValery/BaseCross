/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	���j���[�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class MenuStage : public Stage {
		//���\�[�X�̍쐬
		void CreateResourses();
		//�r���[�̍쐬
		void CreateViewLight();
		//�ǖ͗l�̃X�v���C�g�쐬
		void CreateWallSprite();
		//�X�N���[������X�v���C�g�쐬
		void CreateScrollSprite();
	public:
		//�\�z�Ɣj��
		MenuStage() :Stage() {}
		virtual ~MenuStage() {}
		//������
		virtual void OnCreate()override;
		//�X�V
		virtual void OnUpdate() override;
	};


	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
		//�v���[�g�̍쐬
		void CreatePlate();
		//�Z���}�b�v�̍쐬
		void CreateStageCellMap();
		//�Œ�̃{�b�N�X�̍쐬
		void CreateFixedBox();
		//�v���C���[�̍쐬
		void CreatePlayer();
		//�G�̍쐬
		void CreateEnemy();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
	};


}
//end basecross

