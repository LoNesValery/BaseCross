/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	壁スプライト
	//--------------------------------------------------------------------------------------
	class WallSprite : public ObjectInterface, public ShapeInterface {
		//描画コンテキスト
		shared_ptr<VSPSDrawContext> m_DrawContext;
		// Diffuse入りコンスタントバッファ
		struct DiffuseSpriteConstantBuffer
		{
			Matrix4X4 World;
			Color4 Emissive;
			Color4 Diffuse;
			DiffuseSpriteConstantBuffer() {
				memset(this, 0, sizeof(DiffuseSpriteConstantBuffer));
			};
		};
		DiffuseSpriteConstantBuffer m_DiffuseSpriteConstantBuffer;
		//メッシュ
		shared_ptr<MeshResource> m_SquareMesh;
		wstring m_TextureFileName;		///<テクスチャファイル名
		bool m_Trace;					///<透明処理するかどうか
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		Vector2 m_Scale;				///<スケーリング
		Vector2 m_Pos;				///<位置
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		WallSprite(const wstring& TextureFileName, bool Trace, const Vector2& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~WallSprite();
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
	///	四角形スプライト
	//--------------------------------------------------------------------------------------
	class SquareSprite : public ObjectInterface, public ShapeInterface {
		//描画コンテキスト
		shared_ptr<VSPSDrawContext> m_DrawContext;
		// Diffuse入りコンスタントバッファ
		struct DiffuseSpriteConstantBuffer
		{
			Matrix4X4 World;
			Color4 Emissive;
			Color4 Diffuse;
			DiffuseSpriteConstantBuffer() {
				memset(this, 0, sizeof(DiffuseSpriteConstantBuffer));
			};
		};
		DiffuseSpriteConstantBuffer m_DiffuseSpriteConstantBuffer;
		//メッシュ
		shared_ptr<MeshResource> m_SquareMesh;
		//バックアップしておく頂点データ
		vector<VertexPositionTexture> m_BackupVertices;
		wstring m_TextureFileName;		///<テクスチャファイル名
		bool m_Trace;					///<透明処理するかどうか
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		Vector2 m_Scale;				///<スケーリング
		Vector2 m_Pos;				///<位置
		float m_TotalTime;
		//頂点の変更
		void UpdateVertex(float ElapsedTime);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		SquareSprite(const wstring& TextureFileName, bool Trace, const Vector2& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareSprite();
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
//end basecross
