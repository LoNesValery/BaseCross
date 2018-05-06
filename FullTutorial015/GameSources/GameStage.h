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
		//CSV�t�@�C������B
		CsvFile m_GameStageCsvB;
		//�r���[�̍쐬
		void CreateViewLight();
		//�v���[�g�̍쐬
		void CreatePlate();
		//�{�b�N�X�̍쐬
		void CreateFixedBox();
		//�v���C���[�̍쐬
		void CreatePlayer();
		//�G�̍쐬
		void CreateEnemy();
	public:
		//�\�z�Ɣj��
		//�X�e�[�W����3��
		GameStage() :Stage(){}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
		//�X�V
		virtual void OnUpdate()override;
	};

	class AnimeSprite;
	//--------------------------------------------------------------------------------------
	//	�Z���N�g�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class SelectStage : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
		//�X�v���C�g�̍쐬
		void CreateAnimeSprite();
		bool m_CntrolLock;
		//�X�v���C�g��weak_ptr�̔z��
		vector<weak_ptr<AnimeSprite>> m_SpVec;
		//�I�����`�F���W���ꂽ
		void ChangeSelect(int num);
	public:
		//�\�z�Ɣj��
		SelectStage() :Stage(),  m_CntrolLock(false),
			m_SpVec(3)
		{
		}
		virtual ~SelectStage() {}
		//������
		virtual void OnCreate()override;
		//�X�V
		virtual void OnUpdate()override;
	};



}
//end basecross

