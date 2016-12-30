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
		//�r���[�ƃ��C�g�̍쐬
		void CreateViewLight();
		//�v���[�g�̍쐬
		void CreatePlate();
		//�{�b�N�X�̍쐬
		void CreateBox();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
	};


}
//end basecross

