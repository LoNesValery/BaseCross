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
	//�r���[�̍쐬
	void CreateViewLight();
	//�^�C�����O�v���[�g�̍쐬
	void CreateTilingPlate();
	//���C���̍쐬
	void CreateActionLine();
	//�v���C���[�̍쐬
	void CreatePlayer();
public:
	//�\�z�Ɣj��
	GameStage() :Stage(){}
	virtual ~GameStage() {}
	//������
	virtual void OnCreate()override;
};


}
//end basecross
