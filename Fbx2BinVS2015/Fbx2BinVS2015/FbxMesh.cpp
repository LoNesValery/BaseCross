/*!
@file FbxMesh.cpp
@brief FbxMesh関連実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	struct VertexParams {
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//頂点バッファ
		ComPtr<ID3D11Buffer> m_IndexBuffer;	//インデックスバッファ
		UINT m_NumVertices;				//頂点の数
		UINT m_NumIndicis;				//インデックスの数
	};

	//--------------------------------------------------------------------------------------
	//	Implイディオム
	//--------------------------------------------------------------------------------------
	struct FbxMeshResource2::Impl {
		unique_ptr<FbxMesh, FbxMeshDeleter> m_FbxMesh;
		//FBXシーン
		weak_ptr<FbxSceneResource> m_FbxSceneResource;
		//FBX名（空白の場合あり）
		string m_FbxName;
		//以下、スキンメッシュ用
		//スキンメッシュの場合nullptr以外になる
		unique_ptr<FbxSkin, FbxSkinDeleter> m_FbxSkin;
		//１フレームの基準となる時間
		FbxTime m_timePeriod;
		//スキンアニメーションに使用するボーン
		//ボーンの配列
		vector< Bone > m_vecBones;
		//ボーンを名前で照会するする際に使用するインデックステーブル
		map< string, UINT > m_mapBoneList;
		//スキンメッシュでも強制的にスタティックで読むかどうか
		bool m_NeedStatic;
		Impl(shared_ptr<FbxSceneResource> FbxSceneResourcePtr, FbxMesh* pFbxMesh, bool NeedStatic) :
			m_FbxSceneResource(FbxSceneResourcePtr),
			m_FbxSkin(nullptr),
			m_timePeriod(0),
			m_FbxMesh(pFbxMesh),
			m_NeedStatic(NeedStatic)
		{}
		~Impl() {
		}
	};
	//--------------------------------------------------------------------------------------
	///	FBXメッシュリソース(MeshResource派生版)
	//--------------------------------------------------------------------------------------
	FbxMeshResource2::FbxMeshResource2(shared_ptr<FbxSceneResource> FbxSceneResourcePtr, FbxMesh* pFbxMesh, bool NeedStatic):
		MeshResource(),
		pImpl(new Impl(FbxSceneResourcePtr, pFbxMesh, NeedStatic))
	{}
	FbxMeshResource2::~FbxMeshResource2() {}

	void FbxMeshResource2::OnCreate() {
		try {
			//スキンメッシュかどうかを判断
			pImpl->m_FbxSkin.reset(FbxCast< FbxSkin >(pImpl->m_FbxMesh->GetDeformer(0, FbxDeformer::eSkin)));
			//スキンメッシュの場合
			if (pImpl->m_FbxSkin && !pImpl->m_NeedStatic) {
				CreateInstanceFromSkinFbx();
			}
			//スタティックメッシュもしくは強制的にスタティックで読む場合
			else {
				pImpl->m_FbxSkin = nullptr;
				CreateInstanceFromStaticFbx();
			}
		}
		catch (...) {
			throw;
		}
	}



	void FbxMeshResource2::GetMaterialVec(vector<MaterialEx>& materials) {

		DWORD MaterialCount = pImpl->m_FbxMesh->GetNode()->GetMaterialCount();

		//マテリアルの設定
		//テクスチャファイル名作成のためのワーク配列
		wchar_t Buff[MAX_PATH];
		setlocale(LC_CTYPE, "jpn");

		if (pImpl->m_FbxSceneResource.expired()) {
			//失敗した
			throw BaseException(
				L"FbxSceneResourceが無効です",
				L"if (pImpl->m_FbxSceneResource.expired())",
				L"FbxMeshResource::GetMaterialVec()");

		}
		auto FbxSceneResourcePtr = pImpl->m_FbxSceneResource.lock();

		for (DWORD i = 0; i < MaterialCount; i++) {
			//マテリアル取得
			MaterialEx material;
			::ZeroMemory(&material, sizeof(MaterialEx));

			FbxSurfaceMaterial*			pMaterial = pImpl->m_FbxMesh->GetNode()->GetMaterial(i);
			FbxSurfacePhong*			pPhong = (FbxSurfacePhong*)pMaterial;
			FbxPropertyT<FbxDouble3>	color;
			color = pPhong->Ambient;
			//アンビエントは使用しない
			material.m_Ambient = Color4(0.5f, 0.5f, 0.5f, 1.0f);
			//			material.m_Ambient = Color4((float)color.Get()[ 0 ], (float)color.Get()[ 1 ], (float)color.Get()[ 2 ], 1.0f);
			color = pPhong->Diffuse;
			//デフィーズはテクスチャを使うのでフル
			material.m_Diffuse = Color4((float)color.Get()[0], (float)color.Get()[1], (float)color.Get()[2], 1.0f);
			//			material.m_Specular = Color4(0.0f,0.0f,0.0f,0.4f);
			material.m_Specular = Color4((float)color.Get()[0] * 0.5f, (float)color.Get()[1] * 0.5f, (float)color.Get()[2] * 0.5f, 0.4f);
			color = pPhong->Emissive;
			//エミッシブはDirectXTKデフォルト
			//material.m_Emissive = Color4(0.05333332f,0.09882354f,0.1819608f,1.0f);
			material.m_Emissive = Color4((float)color.Get()[0], (float)color.Get()[1], (float)color.Get()[2], 1.0f);

			//マテリアルに関連付けられているテクスチャを読み込む
			const FbxProperty	fbxProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
			//テクスチャからファイル情報を抜き出す
			FbxFileTexture*	pFbxFileTexture = fbxProperty.GetSrcObject< FbxFileTexture >(i);
			TextureResource* pTexture = 0;
			if (pFbxFileTexture) {
				//テクスチャファイル名からパスを排除しファイル名+拡張子として合成する
				char szTextureFilename[256], szFileExt[8];
				_splitpath_s(pFbxFileTexture->GetFileName(), nullptr, 0, nullptr, 0, szTextureFilename, 256, szFileExt, 8);
				//ファイル名の合成
				string sTextureFilename(szTextureFilename);
				sTextureFilename += szFileExt;
				//UNIコードに変換
				size_t size = 0;
				mbstowcs_s(&size, Buff, sTextureFilename.c_str(), MAX_PATH - 1);
				wstring strWork = FbxSceneResourcePtr->GetDataDir() + Buff;
				//pFbxFileTextureからラップモードを取得してテクスチャを作成
				auto PtrTexture = TextureResource::CreateTextureResource(strWork.c_str());
				material.m_TextureResource = PtrTexture;
			}
			else {
				//テクスチャがない場合はnullptrを設定
				material.m_TextureResource = nullptr;
			}
			//マテリアル配列に追加
			materials.push_back(material);
		}
	}


	void FbxMeshResource2::GetStaticVerticesIndicesMaterials(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices,
		vector<MaterialEx>& materials) {
		vertices.clear();
		indices.clear();
		materials.clear();
		GetMaterialVec(materials);
		auto MaterialCount = materials.size();
		UINT NumVertices = 0;
		//メッシュ単体の読み込み
		DWORD dwNumPolygons = 0;	//ポリゴン数
									//頂点がない	
		if ((NumVertices = pImpl->m_FbxMesh->GetControlPointsCount()) <= 0) {
			//失敗した
			throw BaseException(L"Fbxに頂点がありません",
				L"m_FbxMesh->GetControlPointsCount() <= 0",
				L"FbxMeshResource::GetStaticVerticesIndicesMaterials()");
		}
		//ポリゴン数の取得
		dwNumPolygons = pImpl->m_FbxMesh->GetPolygonCount();
		//頂点を作成するための配列
		vertices.resize(NumVertices);
		FbxStringList sUVSetNames;
		pImpl->m_FbxMesh->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		//頂点座標・法線・テクスチャ座標の取得
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//ポリゴンのサイズを得る（通常３）
			const DWORD dwPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = pImpl->m_FbxMesh->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				//Fbxから頂点を得る
				vPos = pImpl->m_FbxMesh->GetControlPointAt(iIndex);
				//法線を得る
				pImpl->m_FbxMesh->GetPolygonVertexNormal(i, j, vNormal);
				//UV値を得る
				pImpl->m_FbxMesh->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				vertices[iIndex] =
					VertexPositionNormalTexture(
						//頂点の設定
						//Z座標がFbxとは符号が逆になる（DirectXは左手座標系）
						XMFLOAT3(static_cast< float >(vPos[0]), static_cast< float >(vPos[1]), -static_cast< float >(vPos[2])),
						//法線の設定
						//Z座標がFbxとは符号が逆になる（DirectXは左手座標系）
//						XMFLOAT3(static_cast< float >(-vNormal[0]), -static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
						XMFLOAT3(static_cast< float >(vNormal[0]), static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
						//UV値の設定
						//Vの値が、1.0から引いた値になる
						XMFLOAT2(static_cast< float >(vUV[0]), 1.0f - static_cast< float >(vUV[1]))
					);

				int tangentCount = pImpl->m_FbxMesh->GetElementTangentCount();
				int binormalCount = pImpl->m_FbxMesh->GetElementBinormalCount();

			}
		}
		//インデックス
		//マテリアルのポインタを取得する
		const FbxLayerElementMaterial*	fbxMaterial = pImpl->m_FbxMesh->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			//頂点インデックスを最適化する(同じマテリアルを使用するポリゴンをまとめて描画できるように並べ、
			//描画時にマテリアルの切り替え回数を減らす)
			for (DWORD j = 0; j < dwNumPolygons; j++) {
				DWORD	dwMaterialId = fbxMaterial->GetIndexArray().GetAt(j);
				if (dwMaterialId == i) {
					int iPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(j);
					for (int k = 0; k < iPolygonSize; k++) {
						indices.push_back(static_cast< uint16_t >(pImpl->m_FbxMesh->GetPolygonVertex(j, 2 - k)));
						materials[i].m_IndexCount++;
					}
				}
			}
		}
		//マテリアル配列にスタート地点を設定
		UINT StarIndex = 0;
		for (DWORD i = 0; i < materials.size(); i++) {
			materials[i].m_StartIndex = StarIndex;
			StarIndex += materials[i].m_IndexCount;
		}
	}

	void FbxMeshResource2::GetStaticVerticesIndicesMaterialsWithTangent(vector<VertexPositionNormalTangentTexture>& vertices,
		vector<uint16_t>& indices, vector<MaterialEx>& materials) {
		vertices.clear();
		indices.clear();
		materials.clear();
		GetMaterialVec(materials);
		auto MaterialCount = materials.size();
		UINT NumVertices = 0;
		//メッシュ単体の読み込み
		DWORD dwNumPolygons = 0;	//ポリゴン数
		//頂点がない	
		if ((NumVertices = pImpl->m_FbxMesh->GetControlPointsCount()) <= 0) {
			//失敗した
			throw BaseException(L"Fbxに頂点がありません",
				L"m_FbxMesh->GetControlPointsCount() <= 0",
				L"FbxMeshResource::GetStaticVerticesIndicesMaterialsWithTangent()");
		}
		//ポリゴン数の取得
		dwNumPolygons = pImpl->m_FbxMesh->GetPolygonCount();
		//頂点を作成するための配列
		vertices.resize(NumVertices);
		FbxStringList sUVSetNames;
		pImpl->m_FbxMesh->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		//頂点座標・法線・テクスチャ座標の取得
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//ポリゴンのサイズを得る（通常３）
			const DWORD dwPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = pImpl->m_FbxMesh->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				FbxVector4 fbxTangent;
				//Fbxから頂点を得る
				vPos = pImpl->m_FbxMesh->GetControlPointAt(iIndex);
				//法線を得る
				pImpl->m_FbxMesh->GetPolygonVertexNormal(i, j, vNormal);
				//UV値を得る
				pImpl->m_FbxMesh->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				vertices[iIndex] =
					VertexPositionNormalTangentTexture(
						//頂点の設定
						//Z座標がFbxとは符号が逆になる（DirectXは左手座標系）
						XMFLOAT3(static_cast< float >(vPos[0]), static_cast< float >(vPos[1]), -static_cast< float >(vPos[2])),
						//法線の設定
						//Z座標がFbxとは符号が逆になる（DirectXは左手座標系）
						//XMFLOAT3(static_cast< float >(-vNormal[0]), -static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
						XMFLOAT3(static_cast< float >(vNormal[0]), static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
						//タンジェントの設定
						XMFLOAT4(0,0,0,0),
						//UV値の設定
						//Vの値が、1.0から引いた値になる
						XMFLOAT2(static_cast< float >(vUV[0]), 1.0f - static_cast< float >(vUV[1]))
					);

				int tangentCount = pImpl->m_FbxMesh->GetElementTangentCount();
				int binormalCount = pImpl->m_FbxMesh->GetElementBinormalCount();

			}
		}
		//インデックス
		//マテリアルのポインタを取得する
		const FbxLayerElementMaterial*	fbxMaterial = pImpl->m_FbxMesh->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			//頂点インデックスを最適化する(同じマテリアルを使用するポリゴンをまとめて描画できるように並べ、
			//描画時にマテリアルの切り替え回数を減らす)
			for (DWORD j = 0; j < dwNumPolygons; j++) {
				DWORD	dwMaterialId = fbxMaterial->GetIndexArray().GetAt(j);
				if (dwMaterialId == i) {
					int iPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(j);
					for (int k = 0; k < iPolygonSize; k++) {
						indices.push_back(static_cast< uint16_t >(pImpl->m_FbxMesh->GetPolygonVertex(j, 2 - k)));
						materials[i].m_IndexCount++;
					}
				}
			}
		}
		//マテリアル配列にスタート地点を設定
		UINT StarIndex = 0;
		for (DWORD i = 0; i < materials.size(); i++) {
			materials[i].m_StartIndex = StarIndex;
			StarIndex += materials[i].m_IndexCount;
		}
	}


	void FbxMeshResource2::CreateInstanceFromStaticFbx() {
		//このFBXに名前があればそれを保持
		if (pImpl->m_FbxMesh->GetName()) {
			pImpl->m_FbxName = pImpl->m_FbxMesh->GetName();
		}
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		GetStaticVerticesIndicesMaterials(vertices, indices,GetMaterialExVec());
		//配列をもとに頂点とインデックスを作成
		ComPtr<ID3D11Buffer> VertexBuffer;	//頂点バッファ
		ComPtr<ID3D11Buffer> IndexBuffer;	//インデックスバッファ
		MeshResource::CreateVertexBuffer(VertexBuffer, vertices);
		MeshResource::CreateIndexBuffer(IndexBuffer, indices);
		SetVertexBuffer(VertexBuffer);
		SetIndexBuffer(IndexBuffer);
		SetNumVertices(vertices.size());
		SetNumIndicis(indices.size());
		SetVertexType<VertexPositionNormalTexture>();
	}

	void FbxMeshResource2::GetSkinVerticesIndicesMaterials(vector<VertexPositionNormalTextureSkinning>& vertices,
		vector<uint16_t>& indices, vector<MaterialEx>& materials,
		vector<Bone>& Bones, map< string, UINT >& mapBoneList) {
		vertices.clear();
		indices.clear();
		materials.clear();
		Bones.clear();
		mapBoneList.clear();
		GetMaterialVec(materials);
		auto MaterialCount = materials.size();
		UINT NumVertices = 0;
		//メッシュ単体の読み込み
		DWORD dwNumPolygons = 0;	//ポリゴン数
		//頂点がない	
		if ((NumVertices = pImpl->m_FbxMesh->GetControlPointsCount()) <= 0) {
			//失敗した
			throw BaseException(L"Fbxに頂点がありません",
				L"m_pFbxMesh->GetControlPointsCount() <= 0",
				L"FbxMeshResource::GetSkinVerticesIndicesMaterials()");
		}
		//ポリゴン数の取得
		dwNumPolygons = pImpl->m_FbxMesh->GetPolygonCount();

		//頂点を作成するための配列
		vertices.resize(NumVertices);
		FbxStringList sUVSetNames;
		pImpl->m_FbxMesh->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		//頂点座標・法線・テクスチャ座標の取得
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//ポリゴンのサイズを得る（通常３）
			const DWORD dwPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = pImpl->m_FbxMesh->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				//Fbxから頂点を得る
				vPos = pImpl->m_FbxMesh->GetControlPointAt(iIndex);
				//法線を得る
				pImpl->m_FbxMesh->GetPolygonVertexNormal(i, j, vNormal);
				//UV値を得る
				pImpl->m_FbxMesh->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				uint32_t temp[4] = { 0, 0, 0, 0 };
				float tempf[4] = { 0, 0, 0, 0 };
				vertices[iIndex] =
					VertexPositionNormalTextureSkinning(
						//頂点の設定
						//Z座標がFbxとは符号が逆になる（DirectXは左手座標系）
						XMFLOAT3(static_cast< float >(vPos[0]), static_cast< float >(vPos[1]), -static_cast< float >(vPos[2])),
						//法線の設定
						//Z座標がFbxとは符号が逆になる（DirectXは左手座標系）
						XMFLOAT3(static_cast< float >(vNormal[0]), static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
						//UV値の設定
						//Vの値が、1.0から引いた値になる
						XMFLOAT2(static_cast< float >(vUV[0]), 1.0f - static_cast< float >(vUV[1])),
						//ブレンドインデックスはとりあえず0
						temp,
						//ブレンドウエイトはとりあえず0
						tempf
					);
			}
		}

		//ブレンドウェイトとブレンドインデックスの読み込み
		const int	iNumCluster = pImpl->m_FbxSkin->GetClusterCount();
		// 変換した FbxSkin から クラスター(ボーン)の数を取得する

		for (int i = 0; i < iNumCluster; i++) {
			int		iNumBlendIndices = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointIndicesCount();	// i 番目のクラスターに影響を受ける頂点の数を取得する
			int*	piBlendIndices = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointIndices();		// i 番目のクラスターに影響を受ける頂点の番号を配列で取得する
			double*	pdBlendWeights = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointWeights();		// i 番目のクラスターに影響を受ける頂点に対応した重みデータを配列で取得する

			for (int j = 0; j < iNumBlendIndices; j++) {
				int	idx = piBlendIndices[j];

				//頂点に登録する４つのブレンドウェイトのうち、最少の値をもつ要素を検索する（DirectX9の固定シェーダでは４つのボーンからしかブレンドできない）
				int	iMinIndex = 0;
				for (int k = 0; k < 4 - 1; k++) {
					for (int l = k + 1; l < 4; l++) {
						if (vertices[idx].weights[k] < vertices[idx].weights[l]) {
							iMinIndex = k;
						}
						else {
							iMinIndex = l;
							k = l;
							break;
						}
					}
				}
				//すでに登録されている中で最小のブレンドウェイトよりも大きい値を持つデータを登録する
				if (static_cast< float >(pdBlendWeights[j]) > vertices[idx].weights[iMinIndex]) {
					vertices[idx].indices[iMinIndex] = static_cast< BYTE >(i);
					vertices[idx].weights[iMinIndex] = static_cast< float >(pdBlendWeights[j]);
				}
			}
		}
		//ウエイトのチェック
		//各頂点ごとにウェイトを足して1.0fにならないとスキンがうまくできない
		for (size_t i = 0; i < vertices.size(); i++) {
			float f = vertices[i].weights[0] + vertices[i].weights[1] + vertices[i].weights[2] + vertices[i].weights[3];
			if (f > 0 && f < 1.0f) {
				float k = 1.0f / f;
				vertices[i].weights[0] *= k;
				vertices[i].weights[1] *= k;
				vertices[i].weights[2] *= k;
				vertices[i].weights[3] *= k;
			}
		}

		//基準タイマーの設定
		FbxGlobalSettings&	globalTimeSettings = pImpl->m_FbxMesh->GetScene()->GetGlobalSettings();
		FbxTime::EMode timeMode = globalTimeSettings.GetTimeMode();
		pImpl->m_timePeriod.SetTime(0, 0, 0, 1, 0, timeMode);

		//インデックスバッファの作成
		//マテリアルのポインタを取得する
		const FbxLayerElementMaterial*	fbxMaterial = pImpl->m_FbxMesh->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			//頂点インデックスを最適化する(同じマテリアルを使用するポリゴンをまとめて描画できるように並べ、
			//描画時にマテリアルの切り替え回数を減らす)
			for (DWORD j = 0; j < dwNumPolygons; j++) {
				DWORD	dwMaterialId = fbxMaterial->GetIndexArray().GetAt(j);
				if (dwMaterialId == i) {
					int iPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(j);
					for (int k = 0; k < iPolygonSize; k++) {
						indices.push_back(static_cast< uint16_t >(pImpl->m_FbxMesh->GetPolygonVertex(j, 2 - k)));
						materials[i].m_IndexCount++;
					}
				}
			}
		}
		//マテリアル配列にスタート地点を設定
		UINT StarIndex = 0;
		for (DWORD i = 0; i < materials.size(); i++) {
			materials[i].m_StartIndex = StarIndex;
			StarIndex += materials[i].m_IndexCount;
		}

		//ボーン数を得る
		auto NumBones = (UINT)pImpl->m_FbxSkin->GetClusterCount();
		for (UINT i = 0; i < NumBones; i++) {
			Bone	bone;

			FbxAMatrix	mBindPose, mCurrentPose;
			pImpl->m_FbxSkin->GetCluster(i)->GetTransformLinkMatrix(mBindPose);
			mCurrentPose = pImpl->m_FbxSkin->GetCluster(i)->GetLink()->EvaluateGlobalTransform(pImpl->m_timePeriod * 0);

			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					bone.m_BindPose(r, c) = static_cast< float >(mBindPose.Get(r, c));
					bone.m_CurrentPose(r, c) = static_cast< float >(mCurrentPose.Get(r, c));
				}
			}

			Matrix4X4	mMirror, mBindInverse;
			mMirror.Identity();
			mMirror(2, 2) = -1.0f;

			bone.m_BindPose *= mMirror;
			bone.m_CurrentPose *= mMirror;
			Vector4 temp4;
			mBindInverse = Matrix4X4EX::Inverse(&temp4, bone.m_BindPose);
			bone.m_ConbinedPose = mBindInverse * bone.m_CurrentPose;

			Bones.push_back(bone);
			//マップの登録
			mapBoneList[pImpl->m_FbxSkin->GetCluster(i)->GetName()] = i;
		}
	}

	void FbxMeshResource2::CreateInstanceFromSkinFbx() {
		//このFBXに名前があればそれを保持
		if (pImpl->m_FbxMesh->GetName()) {
			pImpl->m_FbxName = pImpl->m_FbxMesh->GetName();
		}
		vector<VertexPositionNormalTextureSkinning> vertices;
		vector<uint16_t> indices;
		GetSkinVerticesIndicesMaterials(vertices, indices, GetMaterialExVec(),
			pImpl->m_vecBones,pImpl->m_mapBoneList);

		//配列をもとに頂点とインデックスを作成
		ComPtr<ID3D11Buffer> VertexBuffer;	//頂点バッファ
		ComPtr<ID3D11Buffer> IndexBuffer;	//インデックスバッファ
		MeshResource::CreateVertexBuffer(VertexBuffer, vertices);
		MeshResource::CreateIndexBuffer(IndexBuffer, indices);
		SetVertexBuffer(VertexBuffer);
		SetIndexBuffer(IndexBuffer);
		SetNumVertices(vertices.size());
		SetNumIndicis(indices.size());
		SetVertexType<VertexPositionNormalTextureSkinning>();

	}
	shared_ptr<FbxMeshResource2> FbxMeshResource2::CreateFbxMeshResource(shared_ptr<FbxSceneResource> FbxSceneResourcePtr,
		FbxMesh* pFbxMesh, bool NeedStatic) {
		return ObjectFactory::Create<FbxMeshResource2>(FbxSceneResourcePtr, pFbxMesh, NeedStatic);
	}

	bool FbxMeshResource2::IsSkining() const {
		return (pImpl->m_FbxSkin && !pImpl->m_NeedStatic);
	}

	const unique_ptr<FbxMesh, FbxMeshResource2::FbxMeshDeleter>& FbxMeshResource2::GetFbxMesh()const {
		return pImpl->m_FbxMesh;
	}
	const unique_ptr<FbxSkin, FbxMeshResource2::FbxSkinDeleter>& FbxMeshResource2::GetFbxSkin() const {
		return pImpl->m_FbxSkin;
	}

	const vector<Bone>& FbxMeshResource2::GetBonesVec() const {
		return pImpl->m_vecBones;
	}


	//
	bool FbxMeshResource2::GenerateCurrentPose(vector<Bone>& tgtBones, AnimationData& rAnimationData, float CurrentTime) {
		//経過時間からフレームを求める
		float fCurrentFrame = CurrentTime * rAnimationData.m_SamplesParSecond;
		//アニメーションが最後までいってるかどうかを確認
		bool ret = false;
		if (!rAnimationData.m_IsLoop && ((UINT)fCurrentFrame >= rAnimationData.m_SampleLength)) {
			//ループしないで、アニメが最後に到達したら
			ret = true;
		}
		int iAnimationFrame = static_cast< int >(fCurrentFrame * 10000.0f);

		if (rAnimationData.m_SampleLength > 0) {
			if (rAnimationData.m_IsLoop) {
				//ループする
				iAnimationFrame %= rAnimationData.m_SampleLength * 10000;
				iAnimationFrame += rAnimationData.m_StartSample * 10000;
			}
			else {
				//ループしない
				if (ret) {
					//最後のフレームにポーズを設定する
					iAnimationFrame = (rAnimationData.m_StartSample + rAnimationData.m_SampleLength) * 10000;
				}
				else {
					iAnimationFrame += rAnimationData.m_StartSample * 10000;
				}
			}
		}
		FbxAMatrix	mCurrentPose;
		Matrix4X4	mMirror, mBindInverse;
		mMirror.Identity();
		mMirror(2, 2) = -1.0f;	//座標系の関係でボーンが反転しているので、それをDirectXに合わせるために使用する行列

								//カレント行列と合成行列を再計算する
		for (UINT i = 0; i < GetNumBones(); i++) {
			mCurrentPose = pImpl->m_FbxSkin->GetCluster(i)->GetLink()->EvaluateGlobalTransform((pImpl->m_timePeriod * iAnimationFrame) / 10000);
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					tgtBones[i].m_CurrentPose(r, c) = static_cast< float >(mCurrentPose.Get(r, c));
				}
			}
			tgtBones[i].m_CurrentPose *= mMirror;

			Vector4 temp;
			mBindInverse = Matrix4X4EX::Inverse(&temp, tgtBones[i].m_BindPose);

			tgtBones[i].m_ConbinedPose = mBindInverse * tgtBones[i].m_CurrentPose;
		}
		rAnimationData.m_IsAnimeEnd = ret;
		return ret;
	}


	//--------------------------------------------------------------------------------------
	//	Implイディオム
	//--------------------------------------------------------------------------------------
	struct FbxSceneResource::Impl {
		//データディレクトリ名
		wstring m_DataDir;
		//FBXファイル名
		wstring m_FileName;
		//FBXシーン名
		string m_FbxSceneName;
		//FBXシーン
		shared_ptr<FbxScene> m_FbxScene;
		//強制的にstaticで読むかどうか
		bool m_NeedStatic;
		//単一のFBXメッシュの配列
		vector< shared_ptr<FbxMeshResource2> > m_FbxMeshResourceVec;
		Impl(const wstring& DataDir,
			const wstring& FileName, const string& SceneName, bool  NeedStatic):
			m_DataDir(DataDir),
			m_FileName(FileName),
			m_FbxSceneName(SceneName),
			m_NeedStatic(NeedStatic)
		{}
		~Impl() {}

	};

	//--------------------------------------------------------------------------------------
	///	FBXシーンリソース
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FbxSceneResource::FbxSceneResource(const wstring& DataDir,
		const wstring& FileName, const string& SceneName, bool NeedStatic):
		BaseResource(),
		pImpl(new Impl(DataDir, FileName, SceneName, NeedStatic))
	{
		try {
			if (FileName == L"") {
				//失敗した
				throw BaseException(
					L"Fbxファイル名が無効です",
					L"if(FileName == \"\")",
					L"FbxSceneResource::FbxSceneResource()");
			}

			if (pImpl->m_FbxSceneName == "") {
				if (FileName == L"") {
					//失敗した
					throw BaseException(
						L"ファイル名が無効です",
						L"if(FileName == \"\")",
						L"FbxSceneResource::FbxSceneResource()");
				}
				wstring strWork;
				strWork = pImpl->m_FileName;
				//ファイル名をマルチバイトに変換しシーン名を作成
				Util::WStoMB(strWork, pImpl->m_FbxSceneName);
				pImpl->m_FbxSceneName += ".scene";
			}
		}
		catch (...) {
			throw;
		}
	}
	FbxSceneResource::~FbxSceneResource() {}

	//static構築
	shared_ptr<FbxSceneResource> FbxSceneResource::CreateFbxScene(const wstring& DataDir,
		const wstring& FileName, const string& SceneName, bool NeedStatic) {
		try {
			return ObjectFactory::Create<FbxSceneResource>(DataDir, FileName, SceneName, NeedStatic);
		}
		catch (...) {
			throw;
		}
	}

	//再帰的にシーンを読み込む
	void FbxSceneResource::CreateFbxMeshVector(FbxNode* pFbxNode) {
		//ノードが有効でなければリターン
		if (!pFbxNode) {
			return;
		}
		//FBXシーンの取得
		FbxScene* pScene = pFbxNode->GetScene();
		//ノードの属性を取得
		FbxNodeAttribute* pFbxNodeAttribute = pFbxNode->GetNodeAttribute();
		//ノードの属性が有効であれば、属性のタイプを判定し、タイプがメッシュであればvectorに登録する
		if (pFbxNodeAttribute) {
			if (pFbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
				//三角分割
				FbxGeometryConverter converter(pFbxNode->GetFbxManager());
				converter.Triangulate(pScene, true);
				//メッシュを取り出す
				FbxMesh* pFbxMesh = pFbxNode->GetMesh();
				//シーンのグローバル行列を取得
				FbxAnimEvaluator* pFbxAnimEvaluator = pScene->GetAnimationEvaluator();
				FbxMatrix mGlobal = pFbxAnimEvaluator->GetNodeGlobalTransform(pFbxNode);
				//グローバル行列をすべての頂点に適用
				int	iNumControlPoints = pFbxMesh->GetControlPointsCount();
				FbxVector4*	v = pFbxMesh->GetControlPoints();
				for (int i = 0; i < iNumControlPoints; i++) {
					v[i] = mGlobal.MultNormalize(v[i]);
				}
				//Fbxメッシュの配列に登録
				auto FbxMeshPtr = FbxMeshResource2::CreateFbxMeshResource(GetThis<FbxSceneResource>(), pFbxMesh, pImpl->m_NeedStatic);
				pImpl->m_FbxMeshResourceVec.push_back(
					FbxMeshPtr
				);
			}
		}
		//子ノードの数を取得する
		int iNumChild = pFbxNode->GetChildCount();
		for (int i = 0; i < iNumChild; i++) {
			//子ノードすべてを再帰的にメッシュを登録する
			CreateFbxMeshVector(pFbxNode->GetChild(i));
		}
	}

	void FbxSceneResource::OnCreate() {
		try {

			//インポーターを作成する
			FbxImporter* lImporter = FbxImporter::Create(App::GetApp()->GetScene<Scene>()->GetFbxManager().get(), "");
			//ファイル名をマルチバイトにする
			wstring strWork;
			strWork = pImpl->m_DataDir + pImpl->m_FileName;
			//マルチバイトに変換
			string sFileName;
			Util::WStoMB(strWork, sFileName);
			//インポーターの初期化
			if (!lImporter->Initialize(sFileName.c_str(), -1, App::GetApp()->GetScene<Scene>()->GetFbxManager().get()->GetIOSettings())) {
				//失敗した
				throw BaseMBException(
					"インポータの初期化に失敗しました",
					lImporter->GetStatus().GetErrorString(),
					"FbxSceneResource::FbxSceneResource()");
			}
			//シーンの作成
			pImpl->m_FbxScene = shared_ptr<FbxScene>(
				FbxScene::Create(
					App::GetApp()->GetScene<Scene>()->GetFbxManager().get(), 
					pImpl->m_FbxSceneName.c_str()),	//生ポインタ
					FbxSceneDeleter()	//カスタムデリータ
				);
			//インポータによるシーンへの読み込み
			lImporter->Import(pImpl->m_FbxScene.get());
			//インポータはもう必要ない
			lImporter->Destroy();


			//再帰的に読み込む
			CreateFbxMeshVector(pImpl->m_FbxScene->GetRootNode());


		}
		catch (...) {
			throw;
		}
	}

	//アクセサ
	wstring FbxSceneResource::GetDataDir() const {
		return pImpl->m_DataDir;
	}
	wstring FbxSceneResource::GetFileName() const {
		return pImpl->m_FileName;
	}
	string FbxSceneResource::GetFbxSceneName() const {
		return pImpl->m_FbxSceneName;
	}
	shared_ptr<FbxMeshResource2> FbxSceneResource::GetFbxMeshResource(size_t Index) const {
		if (Index >= pImpl->m_FbxMeshResourceVec.size()) {
			auto str = Util::UintToWStr(Index);
			str += L" >= m_FbxMeshResourceVec.size()";
			throw BaseException(
				L"インデックスが範囲外です",
				str,
				L"FbxSceneResource::GetMeshResource()"
			);
		}
		return pImpl->m_FbxMeshResourceVec[Index];
	}
	size_t FbxSceneResource::GetFbxMeshResourceSize() const {
		return pImpl->m_FbxMeshResourceVec.size();
	}
	vector< shared_ptr<FbxMeshResource2> >& FbxSceneResource::GetFbxMeshResourceVec() const {
		return pImpl->m_FbxMeshResourceVec;
	}

	//--------------------------------------------------------------------------------------
	//	struct BasicFbxPNTBoneDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct BasicFbxPNTBoneDraw::Impl {
		string m_sCurrentAnimationName;	//現在処理対象になっているアニメーションの名前
		float m_CurrentTime;			//アニメーションの現在の経過時間（秒）
		bool m_IsAnimeEnd;				//現在のアニメーションが終了したかどうか
		vector< Bone > m_vecLocalBones;	//各オブジェクトごとにボーンを所持しておく
										//シェーダに渡すボーン行列
		vector<Matrix4X4> m_LocalBonesMatrix;	//シャドウマップに渡すポインタ

		//アニメーションを名前で照会するする際に使用するインデックステーブル
		map< string, AnimationData > m_AnimationMap;
		Impl() :
			m_sCurrentAnimationName(""),
			m_CurrentTime(0),
			m_IsAnimeEnd(false)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class BasicFbxPNTBoneDraw : public BasicFbxPNTDraw;
	//	用途: BasicFbxPNTBoneDraw描画コンポーネント
	//--------------------------------------------------------------------------------------

	//構築と破棄
	BasicFbxPNTBoneDraw::BasicFbxPNTBoneDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		PNTStaticModelDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}
	BasicFbxPNTBoneDraw::~BasicFbxPNTBoneDraw() {}

	//ボーン行列をリソースから読み込む
	void BasicFbxPNTBoneDraw::SetBoneVec() {
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		if (PtrMeshResource->GetNumBones() > 0) {
			//ローカルボーンのオリジナルからのコピー
			pImpl->m_vecLocalBones.resize(PtrMeshResource->GetBonesVec().size());
			pImpl->m_vecLocalBones = PtrMeshResource->GetBonesVec();

			pImpl->m_LocalBonesMatrix.resize(PtrMeshResource->GetNumBones());
		}
		else {
			throw BaseException(
				L"ボーンが見つかりません",
				L"if (PtrMeshResource->GetNumBones() <= 0)",
				L"BasicFbxPNTBoneDraw::SetBoneVec()"
			);
		}
	}
	void BasicFbxPNTBoneDraw::GetBoneVec(vector< Bone >& Bones) {
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		if (PtrMeshResource->GetNumBones() > 0) {
			//ローカルボーンのオリジナルからのコピー
			Bones.resize(PtrMeshResource->GetBonesVec().size());
			Bones = PtrMeshResource->GetBonesVec();
		}
		else {
			throw BaseException(
				L"ボーンが見つかりません",
				L"if (PtrMeshResource->GetNumBones() <= 0)",
				L"BasicFbxPNTBoneDraw::GetBoneVec()"
			);
		}
	}


	//アクセサ
	//各オブジェクトごとのボーン
	const vector< Bone >& BasicFbxPNTBoneDraw::GetVecLocalFbxBones() const {
		return pImpl->m_vecLocalBones;
	}
	const vector< Bone >* BasicFbxPNTBoneDraw::GetVecLocalFbxBonesPtr() const {
		return &pImpl->m_vecLocalBones;
	}

	const vector< Matrix4X4 >* BasicFbxPNTBoneDraw::GetVecLocalBonesPtr() const {
		return &pImpl->m_LocalBonesMatrix;
	}


	void BasicFbxPNTBoneDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		PNTStaticModelDraw::SetMeshResource(MeshRes);
		//ボーンがあれば読み込む
		SetBoneVec();
	}
	void BasicFbxPNTBoneDraw::SetMeshResource(const wstring& MeshKey) {
		PNTStaticModelDraw::SetMeshResource(MeshKey);
		//ボーンがあれば読み込む
		SetBoneVec();
	}


	void BasicFbxPNTBoneDraw::AddAnimation(const char* Name, UINT StartFrame, UINT FrameLength, bool Loop,
		float FramesParSecond) {
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		if (PtrMeshResource->IsSkining()) {
			map< string, AnimationData >::iterator it
				= pImpl->m_AnimationMap.find(Name);
			if (it != pImpl->m_AnimationMap.end()) {
				//指定の名前が見つかった
				//そのデータに差し替え
				it->second.m_StartSample = StartFrame;
				it->second.m_SampleLength = FrameLength;
				it->second.m_SamplesParSecond = FramesParSecond;
				it->second.m_IsLoop = Loop;
			}
			else {
				//見つからない
				//アニメーション定義の追加
				pImpl->m_AnimationMap[Name] = AnimationData(StartFrame, FrameLength,
					Loop, FramesParSecond);
			}
		}
	}
	const AnimationData& BasicFbxPNTBoneDraw::GetAnimationData(const string& AnimeName) const {
		auto it = pImpl->m_AnimationMap.find(AnimeName);
		if (it != pImpl->m_AnimationMap.end()) {
			return it->second;
		}
		else {
			throw BaseMBException(
				"指定のアニメーションが見つかりません",
				AnimeName,
				"AnimationData& FbxMeshResource::GetAnimationData()");
		}
	}


	const string& BasicFbxPNTBoneDraw::GetCurrentAnimation() const {
		return pImpl->m_sCurrentAnimationName;
	}
	void BasicFbxPNTBoneDraw::SetCurrentAnimation(const string& AnemationName, float StartTime) {
		//指定のアニメーションがあるかどうかチェック
		//無ければ例外が出る
		AnimationData animData = GetAnimationData(AnemationName);
		pImpl->m_sCurrentAnimationName = AnemationName;
		pImpl->m_CurrentTime = StartTime;
		pImpl->m_IsAnimeEnd = false;
	}

	float BasicFbxPNTBoneDraw::GetCurrentTime() const {
		return pImpl->m_CurrentTime;
	}


	//現在のアニメーションが終了しているかどうか
	bool BasicFbxPNTBoneDraw::IsTargetAnimeEnd() const {
		return pImpl->m_IsAnimeEnd;
	}

	//指定したIDのボーンの現在の行列を取得する
	void BasicFbxPNTBoneDraw::GetBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const {
		auto PtrGameObject = GetGameObject();
		auto PtrT = PtrGameObject->GetComponent<Transform>();
		Matrix4X4 MeshMatrix = GetMeshToTransformMatrix() * PtrT->GetWorldMatrix();
		if (pImpl->m_vecLocalBones.size() <= BoneId) {
			throw BaseException(
				L"ボーンIDが範囲外です",
				L"if (pImpl->m_vecLocalBones.size() <= BoneId)",
				L"BasicFbxPNTBoneDraw::GetBoneMatrix()"
			);
		}
		Matrix = pImpl->m_vecLocalBones[BoneId].m_CurrentPose * MeshMatrix;
	}

	//指定したIDのボーンの現在のローカル行列を取得する（親子関係を構築するなど用）
	void BasicFbxPNTBoneDraw::GetLocalBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const {
		auto PtrGameObject = GetGameObject();
		auto PtrT = PtrGameObject->GetComponent<Transform>();
		Matrix4X4 MeshMatrix = GetMeshToTransformMatrix();
		if (pImpl->m_vecLocalBones.size() <= BoneId) {
			throw BaseException(
				L"ボーンIDが範囲外です",
				L"if (pImpl->m_vecLocalBones.size() <= BoneId)",
				L"BasicFbxPNTBoneDraw::GetLocalBoneMatrix()"
			);
		}
		Matrix = pImpl->m_vecLocalBones[BoneId].m_CurrentPose * MeshMatrix;
	}

	bool BasicFbxPNTBoneDraw::UpdateAnimation(float ElapsedTime) {
		//メッシュリソースの取得
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		//ボーンが登録されていたらボーンを探す
		if (PtrMeshResource->IsSkining()) {
			if(!pImpl->m_IsAnimeEnd){
				pImpl->m_CurrentTime += ElapsedTime;
			}
			//アニメーションデータの取得
			//チェックするだけなのでテンポラリでよい
			AnimationData animData = GetAnimationData(pImpl->m_sCurrentAnimationName);
			//ボーンを現在の時間に更新する
			//戻り値は終了してるかどうか
			pImpl->m_IsAnimeEnd = PtrMeshResource->GenerateCurrentPose(pImpl->m_vecLocalBones, animData, pImpl->m_CurrentTime);
			if (animData.m_IsLoop) {
				float TotalTime = (float)animData.m_SampleLength / (float)animData.m_SamplesParSecond;
				if (TotalTime <= pImpl->m_CurrentTime) {
					pImpl->m_CurrentTime = 0.0f;
				}
			}
			//シャドウマップ用の行列にコピー
			size_t BoneSz = pImpl->m_vecLocalBones.size();
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				pImpl->m_LocalBonesMatrix[b] = pImpl->m_vecLocalBones[b].m_ConbinedPose;
			}
			return pImpl->m_IsAnimeEnd;
		}
		return false;
	}


	void  BasicFbxPNTBoneDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();

		auto GameObjectPtr = GetGameObject();
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//行列の定義
		Matrix4X4 World, ViewMat, ProjMat;
		//ワールド行列の決定
		World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		//転置する
		World.Transpose();
		//カメラを得る
		auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
		//ビューと射影行列を得る
		ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.Transpose();
		//転置する
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.Transpose();

		//コンスタントバッファの設定
		SimpleConstants cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = World;
		cb1.View = ViewMat;
		cb1.Projection = ProjMat;
		auto StageLight = GameObjectPtr->OnGetDrawLight();
		cb1.LightDir = StageLight.m_Directional;
		cb1.LightDir.w = 1.0f;
		cb1.ActiveFlg.x = 1;

		//ボーンの設定
		size_t BoneSz = pImpl->m_vecLocalBones.size();
		UINT cb_count = 0;
		for (size_t b = 0; b < BoneSz; b++) {
			Matrix4X4 mat = pImpl->m_vecLocalBones[b].m_ConbinedPose;
			mat.Transpose();
			cb1.Bones[cb_count] = ((XMMATRIX)mat).r[0];
			cb1.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
			cb1.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
			cb_count += 3;
		}


/*
		size_t BoneSz = pImpl->m_LocalBonesMatrix.size();
		UINT cb_count = 0;
		for (size_t b = 0; b < BoneSz; b++) {
			Matrix4X4 mat = pImpl->m_LocalBonesMatrix[b];
			mat.Transpose();
			cb1.Bones[cb_count] = ((XMMATRIX)mat).r[0];
			cb1.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
			cb1.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
			cb_count += 3;
		}
*/

		auto MeshPtr = GetMeshResource();
		auto& MatVec = MeshPtr->GetMaterialExVec();

		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSPNTBone::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSPNTBone::GetPtr()->GetInputLayout());

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTextureSkinning);
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshPtr->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(MeshPtr->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//デプスステンシル
		switch (GetDepthStencilState()) {
		case DepthStencilState::None:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
			break;
		case DepthStencilState::Default:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			break;
		case DepthStencilState::Read:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthRead(), 0);
			break;
		}
		//サンプラー
		//nullに初期化
		ID3D11SamplerState* pSampler = nullptr;
		switch (GetSamplerState()) {
		case SamplerState::SamplerNone:
			break;
		case SamplerState::PointWrap:
			pSampler = RenderStatePtr->GetPointWrap();
			break;
		case SamplerState::PointClamp:
			pSampler = RenderStatePtr->GetPointClamp();
			break;
		case SamplerState::LinearWrap:
			pSampler = RenderStatePtr->GetLinearWrap();
			break;
		case SamplerState::LinearClamp:
			pSampler = RenderStatePtr->GetLinearClamp();
			break;
		case SamplerState::AnisotropicWrap:
			pSampler = RenderStatePtr->GetAnisotropicWrap();
			break;
		case SamplerState::AnisotropicClamp:
			pSampler = RenderStatePtr->GetAnisotropicClamp();
			break;
		case SamplerState::ComparisonLinear:
			pSampler = RenderStatePtr->GetComparisonLinear();
			break;
		}
		//サンプラーを設定
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		for (auto& m : MatVec) {
			cb1.Emissive = GetEmissive();
			cb1.Diffuse = GetDiffuse();
//			cb1.Emissive = m.m_Emissive;
//			cb1.Diffuse = m.m_Diffuse;
			//コンスタントバッファの更新
			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb1, 0, 0);
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//テクスチャを設定
			pID3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
			if (GetRasterizerState() == RasterizerState::Wireframe) {
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetWireframe());
				//描画
				pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else {
				//ブレンドステートとラスタライザを設定して描画
				//もし、透明描画ならAlphaBlendExに設定し、そうでなければ、指定に従う。
				if (GameObjectPtr->GetAlphaActive()) {
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				else {
					switch (GetBlendState()) {
					case BlendState::Opaque:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
						break;
					case BlendState::AlphaBlend:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
						break;
					case BlendState::Additive:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAdditive(), nullptr, 0xffffffff);
						break;
					case BlendState::NonPremultiplied:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetNonPremultiplied(), nullptr, 0xffffffff);
						break;
					}
					switch (GetRasterizerState()) {
					case RasterizerState::CullBack:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
						break;
					case RasterizerState::CullFront:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
						break;
					case RasterizerState::CullNone:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
						break;
					}
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
			}
		}
		Dev->InitializeStates();
	}




	//--------------------------------------------------------------------------------------
	//	class FbxMeshObject : public GameObject;
	//	用途: FBXメッシュのオブジェクト
	//--------------------------------------------------------------------------------------
	FbxMeshObject::FbxMeshObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_DataDir(L""),
		m_FbxFileName(L""),
		m_FbxResName(L""),
		m_MeshIndex(0),
		m_FbxMeshResName(L""),
		m_CharaLocalScale(1.0f),
		m_CharaLocalPosition(0,0,0),
		m_IsReadStaticMesh(false),
		m_IsAnimeRun(false)
	{}
	void FbxMeshObject::OnCreate() {
		//初期位置などの設定
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(1.0f, 1.0f, 1.0f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.0f, 0);

		//文字列をつける
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


	}

	void FbxMeshObject::OnUpdate() {
		auto PtrBoneDraw = GetComponent<BasicFbxPNTBoneDraw>(false);
		if (PtrBoneDraw && m_IsAnimeRun) {
			if (PtrBoneDraw->IsTargetAnimeEnd()) {
				m_IsAnimeRun = false;
			}
			else {
				PtrBoneDraw->UpdateAnimation(App::GetApp()->GetElapsedTime());
			}
		}
	}

	void FbxMeshObject::OnUpdate2() {
		//文字列表示
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\n";
		wstring CurrentTime = L"";
		wstring AnimeRun = L"";
		auto PtrBoneDraw = GetComponent<BasicFbxPNTBoneDraw>(false);
		if (PtrBoneDraw) {
			CurrentTime += Util::FloatToWStr(PtrBoneDraw->GetCurrentTime(), 6, Util::FloatModify::Fixed) + L"\n";
			if (m_IsAnimeRun) {
				AnimeRun = L"ANIME: RUN\n";
			}
			else {
				AnimeRun = L"ANIME: STOP\n";
			}
		}
		wstring str = FPS + CurrentTime + AnimeRun;
		//文字列をつける
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}


	void FbxMeshObject::ClearFbxMesh() {
		try {
			if (m_FbxMeshResName != L"") {
				App::GetApp()->UnRegisterResource<FbxMeshResource2>(m_FbxMeshResName);
				m_FbxMeshResName = L"";
			}
			if (m_FbxResName != L"") {
				App::GetApp()->UnRegisterResource<FbxSceneResource>(m_FbxResName);
				m_FbxResName = L"";
			}
		}
		catch (...) {
			throw;
		}
	}

	void FbxMeshObject::ResetFbxMesh(const wstring& DirName, const wstring& FbxName, size_t MeshIndex, float Scale, const Vector3& Position,
		bool IsReadStatic) {
		try {
			if (m_FbxMeshResName != L"") {
				App::GetApp()->UnRegisterResource<FbxMeshResource2>(m_FbxMeshResName);
				m_FbxMeshResName = L"";
			}
			if (m_FbxResName != L"") {
				App::GetApp()->UnRegisterResource<FbxSceneResource>(m_FbxResName);
				m_FbxResName = L"";
			}
			m_DataDir = DirName;
			m_FbxFileName = FbxName;
			m_FbxResName = FbxName;
			m_MeshIndex = MeshIndex;
			m_FbxMeshResName = m_FbxResName + Util::UintToWStr(m_MeshIndex);
			m_CharaLocalScale = Scale;
			m_CharaLocalPosition = Position;
			m_IsReadStaticMesh = IsReadStatic;

			shared_ptr<FbxSceneResource> PtrFbxScene;
			if (App::GetApp()->CheckResource<FbxSceneResource>(m_FbxResName)) {
				PtrFbxScene = App::GetApp()->GetResource<FbxSceneResource>(m_FbxResName);
			}
			else {
				PtrFbxScene = FbxSceneResource::CreateFbxScene(DirName, FbxName,"", m_IsReadStaticMesh);
				App::GetApp()->RegisterResource(m_FbxResName, PtrFbxScene);
			}

			shared_ptr<FbxMeshResource2> PtrFbxMesh;
			if (App::GetApp()->CheckResource<FbxMeshResource2>(m_FbxMeshResName)) {
				PtrFbxMesh = App::GetApp()->GetResource<FbxMeshResource2>(m_FbxMeshResName);
			}
			else {
				PtrFbxMesh = PtrFbxScene->GetFbxMeshResource(m_MeshIndex);
				App::GetApp()->RegisterResource(m_FbxMeshResName, PtrFbxMesh);
			}
			//モデルの行列
			auto Ptr = GetComponent<Transform>();
			Ptr->SetScale(m_CharaLocalScale, m_CharaLocalScale, m_CharaLocalScale);
			Ptr->SetRotation(0.0f, 0.0f, 0.0f);
			Ptr->SetPosition(m_CharaLocalPosition);

			auto ShadowPtr = AddComponent<Shadowmap>();
			ShadowPtr->SetMeshResource(PtrFbxMesh);


			if (PtrFbxMesh->IsSkining()) {
				RemoveComponent<PNTStaticModelDraw>();
				auto PtrDraw = AddComponent<BasicFbxPNTBoneDraw>();
				PtrDraw->SetMeshResource(PtrFbxMesh);
				PtrDraw->AddAnimation("start", 0, 1, true, 30);
				PtrDraw->SetCurrentAnimation("start");
			}
			else {
				RemoveComponent<BasicFbxPNTBoneDraw>();
				auto PtrDraw = AddComponent<PNTStaticModelDraw>();
				PtrDraw->SetMeshResource(PtrFbxMesh);
			}
		}
		catch (...) {
			throw;
		}
	}

	bool FbxMeshObject::CheckSkinMesh() {
		auto PtrBoneDraw = GetComponent<BasicFbxPNTBoneDraw>(false);
		if (PtrBoneDraw) {
			auto FbxMeshPtr = PtrBoneDraw->GetMeshResource();
			if (FbxMeshPtr->IsSkining()) {
				return true;
			}
		}
		return false;
	}

	bool FbxMeshObject::CheckMesh() {
		auto PtrDraw = GetComponent<PNTStaticModelDraw>(false);
		if (PtrDraw && PtrDraw->GetMeshResource()) {
			return true;
		}
		auto PtrBoneDraw = GetComponent<BasicFbxPNTBoneDraw>(false);
		if (PtrBoneDraw) {
			auto FbxMeshPtr = PtrBoneDraw->GetMeshResource();
			if (FbxMeshPtr->IsSkining()) {
				return true;
			}
		}
		return false;
	}



	void FbxMeshObject::MoveFbxMesh(UINT FrameRate, UINT StartTime, UINT EndTime, bool IsLoop) {

		m_IsAnimeRun = false;
		auto PtrBoneDraw = GetComponent<BasicFbxPNTBoneDraw>(false);
		if (PtrBoneDraw) {
			auto FbxMeshPtr = PtrBoneDraw->GetMeshResource();
			if (!FbxMeshPtr) {
				AfxMessageBox(L"FbxMeshが読み込まれてません。");
				return;
			}
			UINT FrameLength = (EndTime - StartTime) * FrameRate;
			PtrBoneDraw->AddAnimation("run", StartTime, FrameLength, IsLoop,(float)FrameRate);
			PtrBoneDraw->SetCurrentAnimation("run");
			m_IsAnimeRun = true;
		}
		else {
			AfxMessageBox(L"読み込んだメッシュはスキンメッシュではありません。");
		}

	}

	void FbxMeshObject::AnimePoseStart() {

		auto PtrBoneDraw = GetComponent<BasicFbxPNTBoneDraw>(false);
		if (PtrBoneDraw) {
			if (m_IsAnimeRun) {
				m_IsAnimeRun = false;
			}
			else {
				m_IsAnimeRun = true;
			}
		}
		else {
			AfxMessageBox(L"読み込んだメッシュはスキンメッシュではありません。");
		}
	}

	void FbxMeshObject::SaveStaticBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale) {
		try {
			string header("BDV1.0");
			if (header.size() < 16) {
				header.resize(16, '\0');
			}
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			vector<MaterialEx> materials;
			vector< shared_ptr<TextureResource> > textures;
			auto PtrDraw = GetComponent<PNTStaticModelDraw>();
			auto Mesh = dynamic_pointer_cast<FbxMeshResource2>(PtrDraw->GetMeshResource());
			Mesh->GetStaticVerticesIndicesMaterials(vertices, indices,materials);
			for (auto& v : vertices) {
				v.position *= Scale;
			}

			wstring filename = Dir + FileName;

			ofstream ofs(filename, ios::out | ios::binary);
			ofs.write(header.c_str(), 16);
			//頂点の保存
			BlockHeader VerTexHeader;
			VerTexHeader.m_Type = BlockType::Vertex;
			VerTexHeader.m_Size = (UINT)vertices.size() * sizeof(VertexPositionNormalTexture);
			ofs.write((const char*)&VerTexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&vertices.front(), VerTexHeader.m_Size);
			//インデックスの保存
			BlockHeader IndexHeader;
			IndexHeader.m_Type = BlockType::Index;
			IndexHeader.m_Size = (UINT)indices.size() * sizeof(uint16_t);
			ofs.write((const char*)&IndexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&indices.front(), IndexHeader.m_Size);
			//マテリアルの保存
			//マテリアル数のヘッダの保存
			BlockHeader MaterialCountHeader;
			MaterialCountHeader.m_Type = BlockType::MaterialCount;
			MaterialCountHeader.m_Size = (UINT)materials.size();
			ofs.write((const char*)&MaterialCountHeader, sizeof(BlockHeader));
			//マテリアル本体の保存
			wchar_t Drivebuff[_MAX_DRIVE];
			wchar_t Dirbuff[_MAX_DIR];
			wchar_t FileNamebuff[_MAX_FNAME];
			wchar_t Extbuff[_MAX_EXT];
			BlockHeader MaterialHeader;
			MaterialHeader.m_Type = BlockType::Material;
			for (auto mat : materials) {
				wstring TextureFileName = mat.m_TextureResource->GetTextureFileName();
				::ZeroMemory(Drivebuff, sizeof(Drivebuff));
				::ZeroMemory(Dirbuff, sizeof(Dirbuff));
				::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
				::ZeroMemory(Extbuff, sizeof(Extbuff));
				//モジュール名から、各ブロックに分ける
				_wsplitpath_s(TextureFileName.c_str(),
					Drivebuff, _MAX_DRIVE,
					Dirbuff, _MAX_DIR,
					FileNamebuff, _MAX_FNAME,
					Extbuff, _MAX_EXT);
				TextureFileName = FileNamebuff;
				TextureFileName += Extbuff;

				SaveMaterialEx SaveMat;
				SaveMat.m_StartIndex = mat.m_StartIndex;
				SaveMat.m_IndexCount = mat.m_IndexCount;
				SaveMat.m_Diffuse = mat.m_Diffuse;
				SaveMat.m_Specular = mat.m_Specular;
				SaveMat.m_Ambient = mat.m_Ambient;
				SaveMat.m_Emissive = mat.m_Emissive;
				UINT TextureStrSize = (TextureFileName.size() + 1) * sizeof(wchar_t);
				MaterialHeader.m_Size = sizeof(SaveMaterialEx) + TextureStrSize;
				ofs.write((const char*)&MaterialHeader, sizeof(BlockHeader));
				ofs.write((const char*)&SaveMat, sizeof(SaveMaterialEx));
				ofs.write((const char*)TextureFileName.c_str(), TextureStrSize);

			}
			//End(ヘッダのみ)
			BlockHeader EndHeader;
			EndHeader.m_Type = BlockType::End;
			EndHeader.m_Size = 0;
			ofs.write((const char*)&EndHeader, sizeof(BlockHeader));
			ofs.close();





		}
		catch (...) {
			throw;
		}

	}


	void  FbxMeshObject::SaveSkinBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale,
		UINT FrameParSec, UINT Start, UINT End) {
		try {
			string header("BDV1.0");
			if (header.size() < 16) {
				header.resize(16, '\0');
			}
			vector<VertexPositionNormalTextureSkinning> vertices;
			vector<uint16_t> indices;
			vector<MaterialEx> materials;
			vector<Bone> bones;
			map< string, UINT > mapBoneList;
			auto PtrDraw = GetComponent<BasicFbxPNTBoneDraw>();
			auto Mesh = dynamic_pointer_cast<FbxMeshResource2>(PtrDraw->GetMeshResource());

			Mesh->GetSkinVerticesIndicesMaterials(vertices, indices, materials,
				bones, mapBoneList);


			for (auto& v : vertices) {
				v.position *= Scale;
			}

			vector<Matrix4X4> animematrix;

			//ボーンを現在の時間に更新する
			vector<Bone> AnimeBones;
			AnimeBones.resize(bones.size());
			AnimeBones = bones;

			float start = (float)Start * (float)FrameParSec;
			float end = (float)End * (float)FrameParSec;

			float SampleSpan = 1.0f / (float)FrameParSec;

			//アニメデータを得る
			PtrDraw->AddAnimation("save_run", (UINT)start, (UINT)(end - start), true,(float) FrameParSec);
			PtrDraw->SetCurrentAnimation("save_run");
			AnimationData animData = PtrDraw->GetAnimationData("save_run");

			for (float f = (float)Start; f < (float)End; f += SampleSpan) {
				Mesh->GenerateCurrentPose(AnimeBones, animData, f);
				for (auto b : AnimeBones) {
					Matrix4X4 Mat = b.m_ConbinedPose;
					Mat._41 *= Scale;
					Mat._42 *= Scale;
					Mat._43 *= Scale;
					animematrix.push_back(Mat);
				}
			}


			wstring filename = Dir + FileName;
			ofstream ofs(filename, ios::out | ios::binary);
			ofs.write(header.c_str(), 16);
			//頂点の保存
			BlockHeader VerTexHeader;
			VerTexHeader.m_Type = BlockType::SkinedVertex;
			VerTexHeader.m_Size = (UINT)vertices.size() * sizeof(VertexPositionNormalTextureSkinning);
			ofs.write((const char*)&VerTexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&vertices.front(), VerTexHeader.m_Size);
			//インデックスの保存
			BlockHeader IndexHeader;
			IndexHeader.m_Type = BlockType::Index;
			IndexHeader.m_Size = (UINT)indices.size() * sizeof(uint16_t);
			ofs.write((const char*)&IndexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&indices.front(), IndexHeader.m_Size);
			//マテリアルの保存
			//マテリアル数のヘッダの保存
			BlockHeader MaterialCountHeader;
			MaterialCountHeader.m_Type = BlockType::MaterialCount;
			MaterialCountHeader.m_Size = (UINT)materials.size();
			ofs.write((const char*)&MaterialCountHeader, sizeof(BlockHeader));
			//マテリアル本体の保存
			wchar_t Drivebuff[_MAX_DRIVE];
			wchar_t Dirbuff[_MAX_DIR];
			wchar_t FileNamebuff[_MAX_FNAME];
			wchar_t Extbuff[_MAX_EXT];
			BlockHeader MaterialHeader;
			MaterialHeader.m_Type = BlockType::Material;
			for (auto mat : materials) {
				wstring TextureFileName = mat.m_TextureResource->GetTextureFileName();
				::ZeroMemory(Drivebuff, sizeof(Drivebuff));
				::ZeroMemory(Dirbuff, sizeof(Dirbuff));
				::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
				::ZeroMemory(Extbuff, sizeof(Extbuff));
				//モジュール名から、各ブロックに分ける
				_wsplitpath_s(TextureFileName.c_str(),
					Drivebuff, _MAX_DRIVE,
					Dirbuff, _MAX_DIR,
					FileNamebuff, _MAX_FNAME,
					Extbuff, _MAX_EXT);
				TextureFileName = FileNamebuff;
				TextureFileName += Extbuff;

				SaveMaterialEx SaveMat;
				SaveMat.m_StartIndex = mat.m_StartIndex;
				SaveMat.m_IndexCount = mat.m_IndexCount;
				SaveMat.m_Diffuse = mat.m_Diffuse;
				SaveMat.m_Specular = mat.m_Specular;
				SaveMat.m_Ambient = mat.m_Ambient;
				SaveMat.m_Emissive = mat.m_Emissive;
				UINT TextureStrSize = (TextureFileName.size() + 1) * sizeof(wchar_t);
				MaterialHeader.m_Size = sizeof(SaveMaterialEx) + TextureStrSize;
				ofs.write((const char*)&MaterialHeader, sizeof(BlockHeader));
				ofs.write((const char*)&SaveMat, sizeof(SaveMaterialEx));
				ofs.write((const char*)TextureFileName.c_str(), TextureStrSize);

			}
			//ボーンカウント（ヘッダのみ）
			BlockHeader BoneCountHeader;
			BoneCountHeader.m_Type = BlockType::BoneCount;
			//ボーンカウントの場合m_Sizeはボーン数になる
			BoneCountHeader.m_Size = (UINT)bones.size();
			ofs.write((const char*)&BoneCountHeader, sizeof(BlockHeader));

			//アニメーション行列
			BlockHeader AnimeMatrixHeader;
			AnimeMatrixHeader.m_Type = BlockType::AnimeMatrix;
			AnimeMatrixHeader.m_Size = (UINT)animematrix.size() * sizeof(Matrix4X4);
			ofs.write((const char*)&AnimeMatrixHeader, sizeof(BlockHeader));
			ofs.write((const char*)&animematrix.front(), AnimeMatrixHeader.m_Size);

			//End(ヘッダのみ)
			BlockHeader EndHeader;
			EndHeader.m_Type = BlockType::End;
			EndHeader.m_Size = 0;
			ofs.write((const char*)&EndHeader, sizeof(BlockHeader));


			ofs.close();
		}
		catch (...) {
			throw;
		}

	}


}
//basecross
