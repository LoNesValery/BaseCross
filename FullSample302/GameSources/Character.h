/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	アクションする配置オブジェクト
	//--------------------------------------------------------------------------------------
	class ActionObject : public GameObject {
		Vec3 m_StartPos;
	public:
		//構築と破棄
		ActionObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~ActionObject();
		//初期化
		virtual void OnCreate() override;
	};


}
//end basecross
