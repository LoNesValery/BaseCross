/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	立方体実体
	//--------------------------------------------------------------------------------------
	CubeObject::CubeObject(const wstring& TextureFileName, bool Trace, const Vector3& Pos, bool Flat) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(Pos),
		m_Flat(Flat)
	{}
	CubeObject::~CubeObject() {}

	void CubeObject::CreateBuffers() {
		float HelfSize = 0.5f;
		vector<Vector3> PosVec = {
			{ Vector3(-HelfSize, HelfSize, -HelfSize) },
			{ Vector3(HelfSize, HelfSize, -HelfSize) },
			{ Vector3(-HelfSize, -HelfSize, -HelfSize) },
			{ Vector3(HelfSize, -HelfSize, -HelfSize) },
			{ Vector3(HelfSize, HelfSize, HelfSize) },
			{ Vector3(-HelfSize, HelfSize, HelfSize) },
			{ Vector3(HelfSize, -HelfSize, HelfSize) },
			{ Vector3(-HelfSize, -HelfSize, HelfSize) },
		};
		vector<UINT> PosIndeces = {
			0, 1, 2, 3,
			1, 4, 3, 6,
			4, 5, 6, 7,
			5, 0, 7, 2,
			5, 4, 0, 1,
			2, 3, 7, 6,
		};


		vector<Vector3> FaceNormalVec = {
			{ Vector3(0, 0, -1.0f) },
			{ Vector3(1.0f, 0, 0) },
			{ Vector3(0, 0, 1.0f) },
			{ Vector3(-1.0f, 0, 0) },
			{ Vector3(0, 1.0f, 0) },
			{ Vector3(0, -1.0f, 0) }
		};

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		UINT BasePosCount = 0;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				VertexPositionNormalTexture Data;
				Data.position = PosVec[PosIndeces[BasePosCount + j]];
				if (m_Flat) {
					//フラット表示の場合は法線は頂点方向にする
					Data.normal = Data.position;
					Data.normal.Normalize();
				}
				else {
					//フラット表示しない場合は、法線は面の向き
					Data.normal = FaceNormalVec[i];
				}
				switch (j) {
				case 0:
					Data.textureCoordinate = Vector2(0, 0);
					break;
				case 1:
					Data.textureCoordinate = Vector2(1.0f, 0);
					break;
				case 2:
					Data.textureCoordinate = Vector2(0, 1.0f);
					break;
				case 3:
					Data.textureCoordinate = Vector2(1.0f, 1.0f);
					break;
				}
				vertices.push_back(Data);
			}

			indices.push_back((uint16_t)BasePosCount + 0);
			indices.push_back((uint16_t)BasePosCount + 1);
			indices.push_back((uint16_t)BasePosCount + 2);
			indices.push_back((uint16_t)BasePosCount + 1);
			indices.push_back((uint16_t)BasePosCount + 3);
			indices.push_back((uint16_t)BasePosCount + 2);

			BasePosCount += 4;
		}

		//メッシュの作成（変更できない）
		m_CubeMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}

	void CubeObject::UpdateConstantBuffer() {
		//行列の定義
		Matrix4X4 World, View, Proj;
		//ワールド行列の決定
		World.AffineTransformation(
			m_Scale,			//スケーリング
			Vector3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		//転置する
		World.Transpose();
		//ビュー行列の決定
		View.LookAtLH(Vector3(0, 2.0, -5.0f), Vector3(0, 0, 0), Vector3(0, 1.0f, 0));
		//転置する
		View.Transpose();
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj.PerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);
		//転置する
		Proj.Transpose();
		//ライティング
		Vector4 LightDir(0.5f, -1.0f, 0.5f, 0.0f);
		LightDir.Normalize();
		m_StaticConstantBuffer.World = World;
		m_StaticConstantBuffer.View = View;
		m_StaticConstantBuffer.Projection = Proj;
		m_StaticConstantBuffer.LightDir = LightDir;
		m_StaticConstantBuffer.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		m_StaticConstantBuffer.Emissive = Color4(0, 0, 0, 0);
		//更新
		m_DrawContextCullBack->UpdateConstantBuffer(reinterpret_cast<void**>(&m_StaticConstantBuffer), sizeof(m_StaticConstantBuffer));
		m_DrawContextCullFront->UpdateConstantBuffer(reinterpret_cast<void**>(&m_StaticConstantBuffer), sizeof(m_StaticConstantBuffer));
	}



	void CubeObject::OnCreate() {
		CreateBuffers();
		//テクスチャの作成
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		m_Scale = Vector3(1.0f, 1.0f, 1.0f);
		m_Qt.Identity();
		//描画コンテキスト1
		m_DrawContextCullBack = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateSrvSmpCbv);
		m_DrawContextCullBack->CreateConstantBuffer(sizeof(m_StaticConstantBuffer));
		m_DrawContextCullBack->CreateDefault3DPipelineCmdList<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>();
		//テクスチャ設定
		m_DrawContextCullBack->SetTextureResource(m_TextureResource);
		//描画コンテキスト2
		m_DrawContextCullFront = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateSrvSmpCbv);
		m_DrawContextCullFront->CreateConstantBuffer(sizeof(m_StaticConstantBuffer));
		m_DrawContextCullFront->CreateDefault3DPipelineCmdList<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>();
		//テクスチャ設定
		m_DrawContextCullFront->SetTextureResource(m_TextureResource);
		//CullFront設定
		m_DrawContextCullFront->SetRasterizerState(RasterizerState::CullFront);
		if (m_Trace) {
			m_DrawContextCullBack->SetBlendState(BlendState::AlphaBlend);
			m_DrawContextCullFront->SetBlendState(BlendState::AlphaBlend);
		}
		//コンスタントバッファの更新
		UpdateConstantBuffer();
	}
	void CubeObject::OnUpdate() {
		Quaternion QtSpan;
		QtSpan.RotationAxis(Vector3(0, 1.0f, 0), 0.02f);
		m_Qt *= QtSpan;
		m_Qt.Normalize();
	}
	void CubeObject::OnDraw() {
		//コンスタントバッファの更新
		UpdateConstantBuffer();
		//描画
		if (m_Trace) {
			m_DrawContextCullFront->DrawIndexed<VertexPositionNormalTexture>(m_CubeMesh);
			m_DrawContextCullBack->DrawIndexed<VertexPositionNormalTexture>(m_CubeMesh);
		}
		else {
			m_DrawContextCullBack->DrawIndexed<VertexPositionNormalTexture>(m_CubeMesh);
		}
	}




}
//end basecross
