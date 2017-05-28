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
		//CSV�t�@�C��
		CsvFile m_GameStageCsv;
		//�r���[�̍쐬
		void CreateViewLight();
		//�v���[�g�̍쐬
		void CreatePlate();
		//�Z���}�b�v�̍쐬
		void CreateStageCellMap();
		//�{�b�N�X�̍쐬
		void CreateFixedBox();
		//�X�p�[�N�̍쐬
		void CreateSpark();
		//���̍쐬
		void CreateFire();
		//�v���C���[�̍쐬
		void CreatePlayer();
		//�G�̍쐬
		void CreateEnemy();
	public:
		//�Œ�̃{�b�N�X�̃R�X�g���Z���}�b�v�ɔ��f
		void SetCellMapCost();
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
	};


}
//end basecross

