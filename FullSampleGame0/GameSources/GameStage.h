/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	���Ȏ��X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class ScienceRoom : public Stage {
	protected:
		//�ǂ̍쐬
		void CreateTileWall();
		//�Œ�̃{�b�N�X�̍쐬
		void CreateFixedBox();
		//�����h�A�̍쐬
		void CreateRoomDoor();
		//�E�C���h�E�̍쐬
		void CreateRoomWindow();
		//���̍쐬
		void CreateFixHead();
		//�\�z�Ɣj��
		ScienceRoom() :Stage()
		{}
		virtual ~ScienceRoom() {}
	public:
		//������
	};

	//--------------------------------------------------------------------------------------
	//	���݂̗��Ȏ��X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class NowScienceRoom : public ScienceRoom {
		//�v���[�g�̍쐬
		void CreatePlate();
		//�r���[�̍쐬
		void CreateViewLight();
		//������ǂ̍쐬
		void CreateThroughWall();
		//���̓G�̍쐬
		void CreateHead();
		//�����G�̍쐬
		void CreateMozo();
		//�Œ�̃V�����_�[�̍쐬
		void CreateFixedCylinder();
		//���a�̌Œ�̃V�����_�[�̍쐬
		void CreatteClearFixedCylinder();
		//�V�����_�[�ɕt���̐��������̍쐬
		void CreatePopupBalloon();
		//�v���C���[�̍쐬
		void CreatePlayer();
	public:
		//�\�z�Ɣj��
		NowScienceRoom() :ScienceRoom()
		{}
		virtual ~NowScienceRoom() {}
		//������
		virtual void OnCreate()override;
	};

	//--------------------------------------------------------------------------------------
	//	�ߋ��̗��Ȏ��X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class PastScienceRoom : public ScienceRoom {
		//�v���[�g�̍쐬
		void CreatePlate();
		//�r���[�̍쐬
		void CreateViewLight();
		//������ǂ̍쐬
		void CreateThroughWall();
		//���̓G�̍쐬
		void CreateHead();
		//�Œ�̃V�����_�[�̍쐬
		void CreateFixedCylinder();
		//�v���C���[�̍쐬
		void CreatePlayer();
	public:
		//�\�z�Ɣj��
		PastScienceRoom() :ScienceRoom()
		{}
		virtual ~PastScienceRoom() {}
		//������
		virtual void OnCreate()override;
	};

	//--------------------------------------------------------------------------------------
	//	�N���A�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class ClearStage : public Stage {
	public:
		//�\�z�Ɣj��
		ClearStage() :Stage()
		{}
		virtual ~ClearStage() {}
		//������
		virtual void OnCreate()override;
		//�X�V
		virtual void OnUpdate()override;
	};

	//--------------------------------------------------------------------------------------
	//	�Q�[���I�[�o�[�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameOverStage : public Stage {
	public:
		//�\�z�Ɣj��
		GameOverStage() :Stage()
		{}
		virtual ~GameOverStage() {}
		//������
		virtual void OnCreate()override;
		//�X�V
		virtual void OnUpdate()override;
	};

}
//end basecross

