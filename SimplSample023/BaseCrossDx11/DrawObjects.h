/*!
@file DrawObjects.h
@brief 描画オブジェクト
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Scene;
	class Stage;
	class GameObject;

	//--------------------------------------------------------------------------------------
	///	シャドウマップ描画に使用する構造体
	//--------------------------------------------------------------------------------------
	struct ShadowmapObject {
		shared_ptr<MeshResource> m_MeshRes;
		Mat4x4 m_WorldMatrix;
		ShadowmapObject() :
			m_MeshRes(nullptr),
			m_WorldMatrix()
		{}
	};

	//--------------------------------------------------------------------------------------
	///	シャドウマップの描画クラス
	//--------------------------------------------------------------------------------------
	class ShadowmapDrawObject : public GameObject {
		vector<ShadowmapObject> m_ShadowmapObjectVec;
	public:
		static float m_LightHeight;	//ライトの高さ（向きをこの値で掛ける）
		static float m_LightNear;	//ライトのNear
		static float m_LightFar;		//ライトのFar
		static float m_ViewWidth;
		static float m_ViewHeight;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ShadowmapDrawObject(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ShadowmapDrawObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画するオブジェクトを追加する
		@param[in]	MeshRes	メッシュ
		@param[in]	WorldMat ワールド行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddDrawMesh(const shared_ptr<MeshResource>& MeshRes, const Mat4x4& WorldMat);
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
		@brief	シャドウマップの描画処理(仮想関数)
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}
	};

	//--------------------------------------------------------------------------------------
	///	PNT頂点描画に使用する構造体(影対応)
	//--------------------------------------------------------------------------------------
	struct DrawObject {
		shared_ptr<MeshResource> m_MeshRes;
		shared_ptr<TextureResource> m_TextureRes;
		Mat4x4 m_WorldMatrix;
		bool m_Trace;
		bool m_Wrap;
		bool m_OwnShadowmapActive;
		DrawObject() :
			m_MeshRes(nullptr),
			m_TextureRes(nullptr),
			m_WorldMatrix(),
			m_Trace(false),
			m_Wrap(false),
			m_OwnShadowmapActive(false)
		{}
	};


	//--------------------------------------------------------------------------------------
	///	PNT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	class PNTDrawObject : public GameObject {
		vector<DrawObject> m_DrawObjectVec;
		vector<DrawObject> m_TraceDrawObjectVec;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画サブ処理
		@param[in]	ObjectVec	描画する配列
		@param[in]	sb	コンスタントバッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void OnDrawSub(vector<DrawObject>& ObjectVec, PNTStaticConstantBuffer& sb);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		*/
		//--------------------------------------------------------------------------------------
		PNTDrawObject(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTDrawObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画するオブジェクトを追加する
		@param[in]	MeshRes	メッシュ
		@param[in]	TextureRes テクスチャ
		@param[in]	WorldMat ワールド行列
		@param[in]	Trace 透明処理するかどうか
		@param[in]	Wrap ラッピング処理するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddDrawMesh(const shared_ptr<MeshResource>& MeshRes,
			const shared_ptr<TextureResource>& TextureRes,
			const Mat4x4& WorldMat,
			bool Trace, bool Wrap = false);
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
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	影付きPNT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	class PNTShadowDrawObject : public GameObject {
		vector<DrawObject> m_DrawObjectVec;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンスタントバッファの設定
		@param[out]	DrawObj	設定するデータ
		@param[out]	Cb	設定する構造体
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetConstants(DrawObject& DrawObj, PNTStaticShadowConstantBuffer& Cb);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		*/
		//--------------------------------------------------------------------------------------
		PNTShadowDrawObject(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTShadowDrawObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画するオブジェクトを追加する
		@param[in]	MeshRes	メッシュ
		@param[in]	TextureRes テクスチャ
		@param[in]	WorldMat ワールド行列
		@param[in]	Trace 透明処理するかどうか
		@param[in]	Wrap ラッピング処理するかどうか
		@param[in]	OwnShadowmapActive 自己影を出すかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddDrawMesh(const shared_ptr<MeshResource>& MeshRes,
			const shared_ptr<TextureResource>& TextureRes,
			const Mat4x4& WorldMat,
			bool Trace, bool Wrap = false, bool OwnShadowmapActive = false);
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
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


}
//end basecross
