/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		//ビューの作成
		void CreateViewLight();
		//プレートのメッシュ作成
		void CreatePlateMesh();
		//プレートの作成
		void CreatePlate();
		//追いかけるオブジェクトの作成
		void CreateSeekObject();
		//固定の法線マップ処理ボックスの作成
		void CreateFixedNormalBox();
		//上下移動しているボックスの作成
		void CreateMoveBox();
		//ヒットする球体の作成
		void CreateSphere();
		//でこぼこ床の作成
		void CreateUnevenGround();
		//プレイヤーの作成
		void CreatePlayer();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
	};


}
//end basecross

