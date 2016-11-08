/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		//���\�[�X�̍쐬
		void CreateResourses();
		//�r���[�̍쐬
		void CreateViewLight();
		//�v���[�g�̍쐬
		void CreatePlate();
		//�ǂ̍쐬
		void CreateTileWall();
		//������ǂ̍쐬
		void CreateThroughWall();
		//�Œ�̃{�b�N�X�̍쐬
		void CreateFixedBox();
		//���̓G�̍쐬
		void CreateHead();
		//�����G�̍쐬
		void CreateMozo();
		//�Œ�̃V�����_�[�̍쐬
		void CreateFixedCylinder();
		//�v���C���[�̍쐬
		void CreatePlayer();
		//�J�����}���̍쐬
		void CreateCameraman();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
	};


}
//end basecross

