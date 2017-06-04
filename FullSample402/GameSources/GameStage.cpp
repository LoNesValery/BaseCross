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
	//�r���[�̍쐬
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�V���h�E�}�b�v�͎g�p���Ȃ�
		SetShadowmapDraw(false);
	}

	//�������̃X�v���C�g�쐬
	void GameStage::CreateTraceSprite() {
		AddGameObject<TraceSprite>(true,
			Vector2(320.0f, 320.0f), Vector3(-400.0f, 0.0f, 0.0f));
	}


	//�ǖ͗l�̃X�v���C�g�쐬
	void GameStage::CreateWallSprite() {
		AddGameObject<WallSprite>(L"WALL_TX", false,
			Vector2(320.0f, 320.0f), Vector3(400.0f, 0.0f ,0.1f));
	}


	//�X�N���[������X�v���C�g�쐬
	void GameStage::CreateScrollSprite() {
		AddGameObject<ScrollSprite>(L"TRACE_TX", true,
			Vector2(240.0f, 60.0f), Vector3(400.0f, 0.0f,0.0f));
	}

	//�X�R�A�X�v���C�g�쐬
	void GameStage::CreateScoreSprite() {
		AddGameObject<ScoreSprite>(4,
			L"NUMBER_TX",
			true,
			Vector2(320.0f, 80.0f),
			Vector3(0.0f, 0.0f,0.0f));
	}



	void GameStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�������̃X�v���C�g�쐬
			CreateTraceSprite();
			//�ǖ͗l�̃X�v���C�g�쐬
			CreateWallSprite();
			//�X�N���[������X�v���C�g�쐬
			CreateScrollSprite();
			//�X�R�A�X�v���C�g�쐬
			CreateScoreSprite();
		}
		catch (...) {
			throw;
		}
	}

	void GameStage::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= 10000.0f) {
			m_TotalTime = 0.0f;
		}
		//�X�R�A���X�V����
		auto ScorPtr = GetSharedGameObject<ScoreSprite>(L"ScoreSprite");
		ScorPtr->SetScore(m_TotalTime);
	}


}
//end basecross