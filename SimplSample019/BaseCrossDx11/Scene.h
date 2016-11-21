/*!
@file Scene.h
@brief シーン
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	struct GameParamaters {
		//プレイヤーのライフ
		int m_PlayerLife;
		//吹き出しを表示させるタスク
		bool m_BalloonOpenTask;
		//頭のタスク
		bool m_IsHeadTaskClear;
		GameParamaters() :
			m_PlayerLife(5),
			m_BalloonOpenTask(false),
			m_IsHeadTaskClear(false)
		{}
	};


	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{
		//パラメータ
		GameParamaters m_GameParamaters;
		//リソースの作成
		void CreateResourses();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(), m_GameParamaters(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ゲームパラメータを得る
		@return	ゲームパラメータ
		*/
		//--------------------------------------------------------------------------------------
		GameParamaters& GetGameParamaters() {
			return m_GameParamaters;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief イベント取得
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;
	};

}

//end basecross
