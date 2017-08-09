/*!
@file Scene.h
@brief シーンなど
*/
#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	class Scene : public SceneInterface {
		shared_ptr<SquareObject> m_SquareObject;				///<平面オブジェクト
		shared_ptr<SphereObject> m_SphereObject;				///<球オブジェクト
		vector<shared_ptr<BoxBase>> m_BoxVec;		///<ボックスの配列

		shared_ptr<PNTDrawObject>	m_PNTDrawObject;				///<描画オブジェクト
		shared_ptr<WrappedSprite> m_WallSprite;		///<スプライト

		Vector3 m_CamerEye;			///<カメラ位置
		Vector3 m_CamerAt;			///<カメラ視点
		Vector3 m_CamerUp;			///<カメラ傾き
		float m_FovY;				///<画角
		float m_CameraXZRad;			//カメラのXZラジアン
		float m_CameraYRad;				//カメラのYラジアン
		float m_CameraArmLen;		//カメラの腕の長さ

		Vector4 m_LightDir;			///<ライト向き


	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 球オブジェクトの取得
		@return	球オブジェクト
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<SphereObject> GetSphereObject() const {
			return m_SphereObject;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ボックスの配列の取得
		@return	ボックスの配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<shared_ptr<BoxBase>>& GetBoxVec() const {
			return m_BoxVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 壁スプライトの取得
		@return	壁スプライト
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<WrappedSprite> GetWallSprite() const {
			return m_WallSprite;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画オブジェクトの取得
		@return	描画オブジェクト
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<PNTDrawObject> GetPNTDrawObject() const {
			return m_PNTDrawObject;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ビュー行列と射影行列の取得（各参照パラメータに代入）
		@param[out]	View	ビュー行列を受け取る参照
		@param[out]	Proj	射影行列を受け取る参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetViewProjMatrix(Matrix4X4& View, Matrix4X4& Proj)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラのEyeとAtを取得
		@param[out]	Eye	Eyeを受け取る参照
		@param[out]	At	Atを受け取る参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetCameraEyeAt(Vector3& Eye, Vector3& At)const {
			Eye = m_CamerEye;
			At = m_CamerAt;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ライト向きの取得
		@param[out]	LightDir	ライト向き受け取る参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetLightDir(Vector4& LightDir)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


}
// end basecross
