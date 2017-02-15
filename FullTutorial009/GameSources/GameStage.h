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
		//�Z���}�b�v�̍쐬
		void CreateStageCellMap();
		//�Œ�̃{�b�N�X�̍쐬
		void CreateFixedBox();
		//�v���C���[�̍쐬
		void CreatePlayer();
		//�G�̍쐬
		void CreateEnemy();
	public:
		//�Œ�̃{�b�N�X�̃R�X�g���Z���}�b�v�ɔ��f
		void SetCellMapCost(const wstring& CellMapGroupName);
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
	};


}
//end basecross

