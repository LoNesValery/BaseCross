/*!
@file DrawComponentsEx.cpp
@brief 拡張描画コンポーネント実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	DirectXTKのEffect関連移植
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	///	Basicシェーダー
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	/// コンスタントバッファ
	//--------------------------------------------------------------------------------------
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBBasicEffect)

	//--------------------------------------------------------------------------------------
	/// PT頂点シェーダ
	//--------------------------------------------------------------------------------------
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPTStatic.cso")

	//--------------------------------------------------------------------------------------
	/// PTピクセルシェーダ
	//--------------------------------------------------------------------------------------
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"BcPSPTStatic.cso")



	//--------------------------------------------------------------------------------------
	/// PNT頂点シェーダ(頂点ライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticVL, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTStaticVL.cso")
	//影無しBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBoneVL, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTBoneVL.cso")
	//影付き
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticVLShadow, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTStaticVLShadow.cso")
	//影付きBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBoneVLShadow, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTBoneVLShadow.cso")


		

	//バイアス付き影無し
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticVLBn, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTStaticVLBn.cso")
	//バイアス付き影無しBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBoneVLBn, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTBoneVLBn.cso")

	//バイアス付き影付き
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticVLBnShadow, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTStaticVLBnShadow.cso")

	//バイアス付き影付きBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBoneVLBnShadow, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTBoneVLBnShadow.cso")


		
	//--------------------------------------------------------------------------------------
	/// PNTピクセルシェーダ(頂点ライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPNTVL, App::GetApp()->m_wstrRelativeShadersPath + L"BcPSPNTVL.cso")
	//影付き
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPNTVLShadow, App::GetApp()->m_wstrRelativeShadersPath + L"BcPSPNTVLShadow.cso")


	//--------------------------------------------------------------------------------------
	/// PNT頂点シェーダ(ピクセルライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticPL, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTStaticPL.cso")
	//影無しBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBonePL, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTBonePL.cso")
	//影付き
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticPLShadow, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTStaticPLShadow.cso")
	//影付きBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBonePLShadow, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTBonePLShadow.cso")
		
	//バイス付き影無し
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticPLBn, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTStaticPLBn.cso")
	//バイス付き影無しBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBonePLBn, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTBonePLBn.cso")

	//バイス付き影付き
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticPLBnShadow, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTStaticPLBnShadow.cso")
	//バイス付き影付きBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBonePLBnShadow, App::GetApp()->m_wstrRelativeShadersPath + L"BcVSPNTBonePLBnShadow.cso")


		

	//--------------------------------------------------------------------------------------
	/// PNTピクセルシェーダ(ピクセルライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPNTPL, App::GetApp()->m_wstrRelativeShadersPath + L"BcPSPNTPL.cso")
	//影付き
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPNTPLShadow, App::GetApp()->m_wstrRelativeShadersPath + L"BcPSPNTPLShadow.cso")


	//--------------------------------------------------------------------------------------
	//	Bc3DDraw::Impl
	//--------------------------------------------------------------------------------------
	struct Bc3DDraw::Impl {
		//エミッシブ色
		Color4 m_Emissive;
		//デフューズ色
		Color4 m_Diffuse;
		//透明度
		float m_Alpha;
		//スペキュラーとパワー色
		Color4 m_SpecularColorAndPower;
		//ライティングするかどうか
		bool m_LightingEnabled;
		//ピクセルライティングをするかどうか
		bool m_PreferPerPixelLighting;
		//法線にバイアスをかけるかどうか
		bool m_BiasedNormals;
		//アンビエント
		Color4 m_AmbientLightColor;
		//ライトのMAX
		static const int MaxDirectionalLights = 3;
		//ライトが有効かどうか
		bool m_LightEnabled[MaxDirectionalLights];
		//ライトの向き
		Vector3 m_LightDirection[MaxDirectionalLights];
		//ライトのデフューズ色
		Color4 m_LightDiffuseColor[MaxDirectionalLights];
		//ライトのスペキュラー色
		Color4 m_LightSpecularColor[MaxDirectionalLights];
		//デフォルトのライティングを設定する
		static void EnableDefaultLighting(Bc3DDraw* Draw);
		//フォグが有効かどうか
		bool m_FogEnabled;
		//フォグの開始位置
		float m_FogStart;
		//フォグの終了位置
		float m_FogEnd;
		//フォグ色
		Color4 m_FogColor;
		//フォグベクトル
		Vector3 m_FogVector;
		Impl() :
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_Alpha(1.0f),
			m_SpecularColorAndPower(0.0f, 0.0f, 0.0f, 1.0f),
			m_LightingEnabled(false),
			m_PreferPerPixelLighting(false),
			m_BiasedNormals(false),
			m_AmbientLightColor(0, 0, 0, 0),
			m_FogEnabled(false),
			m_FogStart(-25.0f),
			m_FogEnd(-40.0f),
			m_FogColor(0.8f, 0.8f, 0.8f, 1.0f),
			m_FogVector(0.0, 0.0, 1.0f)
		{
			static const XMVECTORF32 defaultSpecular = { 1, 1, 1, 16 };
			static const Vector3 defaultLightDirection = { 0, -1, 0 };
			m_SpecularColorAndPower = defaultSpecular;
			for (int i = 0; i < MaxDirectionalLights; i++)
			{
				m_LightDirection[i] = defaultLightDirection;
				m_LightDiffuseColor[i] =  g_XMZero;
				m_LightSpecularColor[i] =  g_XMZero;
			}
		}
	};

	//--------------------------------------------------------------------------------------
	///	Basic描画コンポーネントの親(3D描画)
	//--------------------------------------------------------------------------------------
	Bc3DDraw::Bc3DDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl()) {}

	Bc3DDraw::~Bc3DDraw() {}

	Color4 Bc3DDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}
	void Bc3DDraw::SetEmissive(const Color4& col) {
		pImpl->m_Emissive = col;
	}
	Color4 Bc3DDraw::GetDiffuse() const {
		return pImpl->m_Diffuse;
	}
	void Bc3DDraw::SetDiffuse(const Color4& col) {
		pImpl->m_Diffuse = col;
	}

	Color4 Bc3DDraw::GetSpecularColor() const {
		Color4 ret = pImpl->m_SpecularColorAndPower;
		ret.w = 0.0f;
		return ret;
	}
	void Bc3DDraw::SetSpecularColor(const Color4& col) {
		pImpl->m_SpecularColorAndPower = XMVectorSelect(pImpl->m_SpecularColorAndPower, col, g_XMSelect1110);
	}

	void Bc3DDraw::DisableSpecular() {
		pImpl->m_SpecularColorAndPower = g_XMIdentityR3;
	}

	float Bc3DDraw::GetSpecularPower() const {
		return pImpl->m_SpecularColorAndPower.w;
	}


	void Bc3DDraw::SetSpecularPower(float pow) {
		pImpl->m_SpecularColorAndPower.w = pow;
	}



	Color4 Bc3DDraw::GetSpecularColorAndPower() const {
		return pImpl->m_SpecularColorAndPower;
	}
	void Bc3DDraw::SetSpecularColorAndPower(const Color4& col) {
		pImpl->m_SpecularColorAndPower = col;
	}

	float Bc3DDraw::GetAlpha() const {
		return pImpl->m_Alpha;
	}

	void Bc3DDraw::SetAlpha(float a) {
		pImpl->m_Alpha = a;
	}

	Color4 Bc3DDraw::GetColorAndAlpha() const {
		Color4 ret = pImpl->m_Diffuse;
		ret.w = pImpl->m_Alpha;
		return ret;
	}

	void Bc3DDraw::SetColorAndAlpha(const Color4& col) {
		pImpl->m_Diffuse = col;
		pImpl->m_Alpha = col.w;
	}
	bool Bc3DDraw::IsLightingEnabled() const {
		return pImpl->m_LightingEnabled;
	}
	bool Bc3DDraw::GetLightingEnabled() const {
		return pImpl->m_LightingEnabled;

	}
	void Bc3DDraw::SetLightingEnabled(bool value) {
		pImpl->m_LightingEnabled = value;
	}
	bool Bc3DDraw::GetPerPixelLighting() const {
		return pImpl->m_PreferPerPixelLighting;
	}

	bool Bc3DDraw::IsPerPixelLighting() const {
		return pImpl->m_PreferPerPixelLighting;

	}
	void Bc3DDraw::SetPerPixelLighting(bool value) {
		pImpl->m_PreferPerPixelLighting = value;
	}

	bool Bc3DDraw::GetBiasedNormals() const {
		return pImpl->m_BiasedNormals;
	}

	bool Bc3DDraw::IsBiasedNormals() const {
		return pImpl->m_BiasedNormals;
	}

	void Bc3DDraw::SetBiasedNormals(bool value) {
		pImpl->m_BiasedNormals = value;
	}

	Color4 Bc3DDraw::GetAmbientLightColor()const {
		return pImpl->m_AmbientLightColor;
	}

	void Bc3DDraw::SetAmbientLightColor(const Color4& value) {
		pImpl->m_AmbientLightColor = value;
	}
	void Bc3DDraw::ValidateLightIndex(int whichLight)
	{
		if (whichLight < 0 || whichLight >= Impl::MaxDirectionalLights)
		{
			throw BaseException(
				L"ライトのインデックスが範囲外です",
				L"if (whichLight < 0 || whichLight >= Impl::MaxDirectionalLights)",
				L"Bc3DDraw::ValidateLightIndex()"
			);
		}
	}

	int Bc3DDraw::GetMaxDirectionalLights() {
		return Impl::MaxDirectionalLights;
	}

	bool Bc3DDraw::GetLightEnabled(int whichLight)const {
		ValidateLightIndex(whichLight);
		return pImpl->m_LightEnabled[whichLight];
	}

	bool Bc3DDraw::IsLightEnabled(int whichLight)const {
		ValidateLightIndex(whichLight);
		return pImpl->m_LightEnabled[whichLight];
	}
	void Bc3DDraw::SetLightEnabled(int whichLight, bool value) {
		ValidateLightIndex(whichLight);
		pImpl->m_LightEnabled[whichLight] = value;
	}

	Vector3 Bc3DDraw::GetLightDirection(int whichLight) const {
		ValidateLightIndex(whichLight);
		return pImpl->m_LightDirection[whichLight];
	}

	void Bc3DDraw::SetLightDirection(int whichLight, const Vector3& value) {
		ValidateLightIndex(whichLight);
		pImpl->m_LightDirection[whichLight] = value;
	}

	Color4 Bc3DDraw::GetLightDiffuseColor(int whichLight) const {
		ValidateLightIndex(whichLight);
		return pImpl->m_LightDiffuseColor[whichLight];
	}


	void Bc3DDraw::SetLightDiffuseColor(int whichLight, const Color4& value) {
		ValidateLightIndex(whichLight);
		pImpl->m_LightDiffuseColor[whichLight] = value;
	}

	Color4 Bc3DDraw::GetLightSpecularColor(int whichLight) const {
		ValidateLightIndex(whichLight);
		return pImpl->m_LightSpecularColor[whichLight];
	}

	void Bc3DDraw::SetLightSpecularColor(int whichLight, const Color4& value) {
		ValidateLightIndex(whichLight);
		pImpl->m_LightSpecularColor[whichLight] = value;
	}


	bool Bc3DDraw::GetFogEnabled() const {
		return pImpl->m_FogEnabled;

	}
	bool Bc3DDraw::IsFogEnabled() const {
		return pImpl->m_FogEnabled;
	}
	void Bc3DDraw::SetFogEnabled(bool b) {
		pImpl->m_FogEnabled = b;
	}

	float Bc3DDraw::GetFogStart() const {
		return pImpl->m_FogStart;
	}

	void Bc3DDraw::SetFogStart(float value) {
		//常にマイナス値
		pImpl->m_FogStart = -(abs(value));
	}

	float Bc3DDraw::GetFogEnd() const {
		return pImpl->m_FogEnd;
	}
	void Bc3DDraw::SetFogEnd(float value) {
		//常にマイナス値
		pImpl->m_FogEnd = -(abs(value));
	}



	Color4 Bc3DDraw::GetFogColor() const {
		return pImpl->m_FogColor;
	}
	void Bc3DDraw::SetFogColor(const Color4& col) {
		pImpl->m_FogColor = col;
	}

	void Bc3DDraw::SetConstants(BasicEffectConstants& BcCb, bool shadowUse) {
		//行列の定義
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//カメラを得る
		auto CameraPtr = GetGameObject()->OnGetDrawCamera();
		//ライトを得る
		auto PtrLightObj = GetGameObject()->GetStage()->GetLight();
		auto PtrMultiLight = dynamic_pointer_cast<MultiLight>(PtrLightObj);
		if (PtrMultiLight) {
			//マルチライトだった
			BcCb.activeFlg.x = 3;
		}
		else {
			//そうではない
			BcCb.activeFlg.x = 1;
		}
		auto StageLight = GetGameObject()->OnGetDrawLight();
		//ワールド行列
		Matrix4X4 world = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		//ビュー行列
		Matrix4X4 view = CameraPtr->GetViewMatrix();
		//射影行列
		Matrix4X4 projection = CameraPtr->GetProjMatrix();

		//行列の設定
		auto worldView = world * view;
		BcCb.worldViewProj = XMMatrixTranspose(XMMatrixMultiply(worldView, projection));

		//フォグの設定
		if (IsFogEnabled())
		{
			auto start = GetFogStart();
			auto end = GetFogEnd();
			if (start == end)
			{
				// Degenerate case: force everything to 100% fogged if start and end are the same.
				static const XMVECTORF32 fullyFogged = { 0, 0, 0, 1 };
				BcCb.fogVector = fullyFogged;
			}
			else
			{
				XMMATRIX worldViewTrans = worldView;
				// _13, _23, _33, _43
				XMVECTOR worldViewZ = XMVectorMergeXY(XMVectorMergeZW(worldViewTrans.r[0], worldViewTrans.r[2]),
					XMVectorMergeZW(worldViewTrans.r[1], worldViewTrans.r[3]));
				XMVECTOR wOffset = XMVectorSwizzle<1, 2, 3, 0>(XMLoadFloat(&start));
				BcCb.fogVector = (worldViewZ + wOffset) / (start - end);
			}
			BcCb.fogColor = GetFogColor();
		}
		else
		{
			// When fog is disabled, make sure the fog vector is reset to zero.
			BcCb.fogVector = g_XMZero;
			BcCb.fogColor = g_XMZero;

		}
		//ライトの設定
		if (IsLightingEnabled())
		{
			if (BcCb.activeFlg.x == 1) {
				BcCb.lightDirection[0] = GetLightDirection(0);
				BcCb.lightDiffuseColor[0] = GetLightDiffuseColor(0);
				BcCb.lightSpecularColor[0] = GetLightSpecularColor(0);
			}
			else {
				for (int i = 0; i < GetMaxDirectionalLights(); i++) {
					if (IsLightEnabled(i)) {
						BcCb.lightDirection[i] = GetLightDirection(i);
						BcCb.lightDiffuseColor[i] = GetLightDiffuseColor(i);
						BcCb.lightSpecularColor[i] = GetLightSpecularColor(i);
					}
				}
			}
			BcCb.world = XMMatrixTranspose(world);

			XMMATRIX worldInverse = XMMatrixInverse(nullptr, world);

			BcCb.worldInverseTranspose[0] = worldInverse.r[0];
			BcCb.worldInverseTranspose[1] = worldInverse.r[1];
			BcCb.worldInverseTranspose[2] = worldInverse.r[2];


			XMMATRIX viewInverse = XMMatrixInverse(nullptr, view);

			BcCb.eyePosition = viewInverse.r[3];
		}

		// Material color parameters. The desired lighting model is:
		//
		//     ((ambientLightColor + sum(diffuse directional light)) * diffuseColor) + emissiveColor
		//
		// When lighting is disabled, ambient and directional lights are ignored, leaving:
		//
		//     diffuseColor + emissiveColor
		//
		// For the lighting disabled case, we can save one shader instruction by precomputing
		// diffuse+emissive on the CPU, after which the shader can use diffuseColor directly,
		// ignoring its emissive parameter.
		//
		// When lighting is enabled, we can merge the ambient and emissive settings. If we
		// set our emissive parameter to emissive+(ambient*diffuse), the shader no longer
		// needs to bother adding the ambient contribution, simplifying its computation to:
		//
		//     (sum(diffuse directional light) * diffuseColor) + emissiveColor
		//
		// For futher optimization goodness, we merge material alpha with the diffuse
		// color parameter, and premultiply all color values by this alpha.

		XMVECTOR diffuse = GetDiffuse();
		XMVECTOR alphaVector = XMVectorReplicate(GetAlpha());
		XMVECTOR emissiveColor = GetEmissive();
		XMVECTOR ambientLightColor = GetAmbientLightColor();
		if (IsLightingEnabled())
		{
			// Merge emissive and ambient light contributions.
			BcCb.emissiveColor = (emissiveColor + ambientLightColor * diffuse) * alphaVector;
			BcCb.specularColorAndPower = GetSpecularColorAndPower();
		}
		else
		{
			BcCb.activeFlg.x = 0;
			// Merge diffuse and emissive light contributions.
			diffuse += emissiveColor;
			BcCb.specularColorAndPower = GetSpecularColorAndPower();
		}
		// xyz = diffuse * alpha, w = alpha.
		BcCb.diffuseColor = XMVectorSelect(alphaVector, diffuse * alphaVector, g_XMSelect1110);

		//影用
		if (shadowUse) {
			Vector3 CalcLightDir = -1.0 * StageLight.m_Directional;
			Vector3 LightAt = CameraPtr->GetAt();
			Vector3 LightEye = CalcLightDir;
			LightEye *= Shadowmap::GetLightHeight();
			LightEye = LightAt + LightEye;
			Vector4 LightEye4 = LightEye;
			LightEye4.w = 1.0f;
			BcCb.lightPos = LightEye4;
			Vector4 eyePos4 = CameraPtr->GetEye();
			eyePos4.w = 1.0f;
			BcCb.eyePos = eyePos4;
			Matrix4X4 LightView, LightProj;
			//ライトのビューと射影を計算
			LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
			LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
				Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
			BcCb.lightView = Matrix4X4EX::Transpose(LightView);
			BcCb.lightProjection = Matrix4X4EX::Transpose(LightProj);
		}
	}



	//--------------------------------------------------------------------------------------
	//	BcPNTStaticDraw::Impl
	//--------------------------------------------------------------------------------------
	struct BcPNTStaticDraw::Impl {
		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		bool m_OwnShadowActive;
		Impl():
			m_OwnShadowActive(false)
		{}
	};
	//--------------------------------------------------------------------------------------
	///	BasicPNT描画コンポーネント
	//--------------------------------------------------------------------------------------
	BcPNTStaticDraw::BcPNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		Bc3DDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}
	BcPNTStaticDraw::~BcPNTStaticDraw() {}


	bool BcPNTStaticDraw::GetOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	bool BcPNTStaticDraw::IsOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	void BcPNTStaticDraw::SetOwnShadowActive(bool b) {
		pImpl->m_OwnShadowActive = b;
	}



	shared_ptr<MeshResource> BcPNTStaticDraw::GetMeshResource() const {
		auto shptr = pImpl->m_MeshResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}
	void BcPNTStaticDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_MeshResource = MeshRes;
	}
	void BcPNTStaticDraw::SetMeshResource(const wstring& MeshKey) {
		pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(MeshKey);
	}

	void BcPNTStaticDraw::OnCreate() {
		SetLightingEnabled(true);
		//マルチライトの設定
		for (int i = 0; i < GetMaxDirectionalLights(); i++) {
			SetLightEnabled(i, true);
		}
	}

	void BcPNTStaticDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"BcPNTStaticDraw::OnDraw()"
			);
		}
		auto shTex = GetTextureResource();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
		//ライトを得る
		auto PtrLightObj = GetGameObject()->GetStage()->GetLight();
		auto PtrMultiLight = dynamic_pointer_cast<MultiLight>(PtrLightObj);
		if (PtrMultiLight) {
			//マルチライトだった
			for (int i = 0; i < GetMaxDirectionalLights(); i++) {
				if (IsLightEnabled(i)) {
					SetLightDirection(i, PtrMultiLight->GetLight(i).m_Directional);
					SetLightDiffuseColor(i, PtrMultiLight->GetLight(i).m_DiffuseColor);
					SetLightSpecularColor(i, PtrMultiLight->GetLight(i).m_SpecularColor);
				}
			}
		}
		else {
			//そうではない
			auto LightPtr = GetGameObject()->OnGetDrawLight();
			SetLightEnabled(0, true);
			SetLightDirection(0, LightPtr.m_Directional);
			SetLightDiffuseColor(0, LightPtr.m_DiffuseColor);
			SetLightSpecularColor(0, LightPtr.m_SpecularColor);
			for (int i = 1; i < GetMaxDirectionalLights(); i++) {
				SetLightEnabled(i, false);
			}
		}
		SetAmbientLightColor(PtrLightObj->GetAmbientLightColor());

		//コンスタントバッファの設定
		BasicEffectConstants BcCb;
		ZeroMemory(&BcCb, sizeof(BcCb));
		if (IsOwnShadowActive()) {
			SetConstants(BcCb,true);
		}
		else {
			SetConstants(BcCb);
		}
		if (shTex) {
			//テクスチャがある
			BcCb.activeFlg.y = 1;
		}
		else {
			//テクスチャがない
			BcCb.activeFlg.y = 0;
		}
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBBasicEffect::GetPtr()->GetBuffer(), 0, nullptr, &BcCb, 0, 0);
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBBasicEffect::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//シェーダの設定
		if (IsPerPixelLighting()) {
			//ピクセルライティング
			if (IsOwnShadowActive()) {
				//影付き
				if (IsBiasedNormals()) {
					//バイアス付き
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticPLBnShadow::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticPLBnShadow::GetPtr()->GetInputLayout());
				}
				else {
					//バイアス無し
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticPLShadow::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticPLShadow::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTPLShadow::GetPtr()->GetShader(), nullptr, 0);
			}
			else {
				//影無し
				if (IsBiasedNormals()) {
					//バイアス付き
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticPLBn::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticPLBn::GetPtr()->GetInputLayout());
				}
				else {
					//バイアス無し
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticPL::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticPL::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTPL::GetPtr()->GetShader(), nullptr, 0);
			}
		}
		else {
			//頂点ライティング
			if (IsOwnShadowActive()) {
				//影付き
				if (IsBiasedNormals()) {
					//バイアス付き
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticVLBnShadow::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticVLBnShadow::GetPtr()->GetInputLayout());
				}
				else {
					//バイアス無し
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticVLShadow::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticVLShadow::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTVLShadow::GetPtr()->GetShader(), nullptr, 0);
			}
			else {
				//影無し
				if (IsBiasedNormals()) {
					//バイアス付き
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticVLBn::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticVLBn::GetPtr()->GetInputLayout());
				}
				else {
					//バイアス無し
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticVL::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticVL::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTVL::GetPtr()->GetShader(), nullptr, 0);
			}
		}

		//テクスチャとサンプラー
		if (shTex) {
			pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
			//サンプラーは設定に任せる
			SetDeviceSamplerState();
		}
		else {
			//シェーダーリソースもクリア
			pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
			//サンプラーもクリア
			pD3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
		}

		//影とサンプラー
		if (IsOwnShadowActive()) {
			//シャドウマップのレンダラーターゲット
			auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
			ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
			pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
			//シャドウマップサンプラー
			ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
			pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
		}

		//ストライドとオフセット
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//デプスステンシルステートは設定に任せる
		SetDeviceDepthStencilState();
		//透明処理なら
		if (GetGameObject()->GetAlphaActive()) {
			//ブレンドステート
			//透明処理
			if (GetBlendState() == BlendState::Additive) {
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
			}
			else {
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			}
			//ラスタライザステート(裏描画)
			pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
			//描画
			pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			//ラスタライザステート（表描画）
			pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
			//描画
			pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		}
		else {
			//透明処理しない
			//ブレンドステートは設定に任せる
			SetDeviceBlendState();
			//ラスタライザステートは設定に任せる
			SetDeviceRasterizerState();
			//描画
			pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		}
		//後始末
		Dev->InitializeStates();

	}


	//--------------------------------------------------------------------------------------
	//	BcPNTStaticModelDraw::Impl
	//--------------------------------------------------------------------------------------
	struct BcPNTStaticModelDraw::Impl {
		bool m_ModelDiffusePriority;
		bool m_ModelEmissivePriority;
		bool m_ModelTextureEnabled;
		Impl() :
			m_ModelDiffusePriority(true),
			m_ModelEmissivePriority(true),
			m_ModelTextureEnabled(true)
		{}
	};


	//--------------------------------------------------------------------------------------
	///	BasicPNTモデル描画コンポーネント
	//--------------------------------------------------------------------------------------
	BcPNTStaticModelDraw::BcPNTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		BcPNTStaticDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}
	BcPNTStaticModelDraw::~BcPNTStaticModelDraw() {}


	bool BcPNTStaticModelDraw::GetModelDiffusePriority() const {
		return pImpl->m_ModelDiffusePriority;

	}
	bool BcPNTStaticModelDraw::IsModelDiffusePriority() const {
		return pImpl->m_ModelDiffusePriority;

	}
	void BcPNTStaticModelDraw::SetModelDiffusePriority(bool b) {
		pImpl->m_ModelDiffusePriority = b;
	}
	bool BcPNTStaticModelDraw::GetModelEmissivePriority() const {
		return pImpl->m_ModelEmissivePriority;
	}
	bool BcPNTStaticModelDraw::IsModelEmissivePriority() const {
		return pImpl->m_ModelEmissivePriority;
	}
	void BcPNTStaticModelDraw::SetModelEmissivePriority(bool b) {
		pImpl->m_ModelEmissivePriority = b;
	}


	bool BcPNTStaticModelDraw::GetModelTextureEnabled() const {
		return pImpl->m_ModelTextureEnabled;

	}
	bool BcPNTStaticModelDraw::IsModelTextureEnabled() const {
		return pImpl->m_ModelTextureEnabled;
	}
	void BcPNTStaticModelDraw::SeModelTextureEnabled(bool b) {
		pImpl->m_ModelTextureEnabled = b;
	}




	void BcPNTStaticModelDraw::OnCreate() {
		SetLightingEnabled(true);
		//マルチライトの設定
		for (int i = 0; i < GetMaxDirectionalLights(); i++) {
			SetLightEnabled(i, true);
		}
	}

	void BcPNTStaticModelDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"BcPNTStaticModelDraw::OnDraw()"
			);
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };


		//ライトを得る
		auto PtrLightObj = GetGameObject()->GetStage()->GetLight();
		auto PtrMultiLight = dynamic_pointer_cast<MultiLight>(PtrLightObj);
		if (PtrMultiLight) {
			//マルチライトだった
			for (int i = 0; i < GetMaxDirectionalLights(); i++) {
				if (IsLightEnabled(i)) {
					SetLightDirection(i, PtrMultiLight->GetLight(i).m_Directional);
					SetLightDiffuseColor(i, PtrMultiLight->GetLight(i).m_DiffuseColor);
					SetLightSpecularColor(i, PtrMultiLight->GetLight(i).m_SpecularColor);
				}
			}
		}
		else {
			//そうではない
			auto LightPtr = GetGameObject()->OnGetDrawLight();
			SetLightEnabled(0, true);
			SetLightDirection(0, LightPtr.m_Directional);
			SetLightDiffuseColor(0, LightPtr.m_DiffuseColor);
			SetLightSpecularColor(0, LightPtr.m_SpecularColor);
			for (int i = 1; i < GetMaxDirectionalLights(); i++) {
				SetLightEnabled(i, false);
			}
		}
		SetAmbientLightColor(PtrLightObj->GetAmbientLightColor());

		//シェーダの設定
		if (IsPerPixelLighting()) {
			//ピクセルライティング
			if (IsOwnShadowActive()) {
				//影付き
				if (IsBiasedNormals()) {
					//バイアス付き
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticPLBnShadow::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticPLBnShadow::GetPtr()->GetInputLayout());
				}
				else {
					//バイアス無し
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticPLShadow::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticPLShadow::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTPLShadow::GetPtr()->GetShader(), nullptr, 0);
			}
			else {
				//影無し
				if (IsBiasedNormals()) {
					//バイアス付き
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticPLBn::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticPLBn::GetPtr()->GetInputLayout());
				}
				else {
					//バイアス無し
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticPL::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticPL::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTPL::GetPtr()->GetShader(), nullptr, 0);
			}
		}
		else {
			//頂点ライティング
			if (IsOwnShadowActive()) {
				//影付き
				if (IsBiasedNormals()) {
					//バイアス付き
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticVLBnShadow::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticVLBnShadow::GetPtr()->GetInputLayout());
				}
				else {
					//バイアス無し
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticVLShadow::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticVLShadow::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTVLShadow::GetPtr()->GetShader(), nullptr, 0);
			}
			else {
				//影無し
				if (IsBiasedNormals()) {
					//バイアス付き
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticVLBn::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticVLBn::GetPtr()->GetInputLayout());
				}
				else {
					//バイアス無し
					pD3D11DeviceContext->VSSetShader(BcVSPNTStaticVL::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTStaticVL::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTVL::GetPtr()->GetShader(), nullptr, 0);
			}
		}
		//影とサンプラー
		if (IsOwnShadowActive()) {
			//シャドウマップのレンダラーターゲット
			auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
			ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
			pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
			//シャドウマップサンプラー
			ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
			pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
		}
		//ストライドとオフセット
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//デプスステンシルステートは設定に任せる
		SetDeviceDepthStencilState();

		//コンスタントバッファの設定
		BasicEffectConstants BcCb;
		ZeroMemory(&BcCb, sizeof(BcCb));
		if (IsOwnShadowActive()) {
			SetConstants(BcCb, true);
		}
		else {
			SetConstants(BcCb);
		}

		//これよりマテリアルごとの描画
		auto& MatVec = MeshRes->GetMaterialExVec();
		for (auto& m : MatVec) {
			if (m.m_TextureResource && IsModelTextureEnabled()) {
				//テクスチャがありテクスチャが有効である
				BcCb.activeFlg.y = 1;
			}
			else {
				//テクスチャがない
				BcCb.activeFlg.y = 0;
			}
			if (IsModelDiffusePriority()) {
				BcCb.diffuseColor = m.m_Diffuse;
			}
			if (IsModelEmissivePriority()) {
				Color4 Em4 = m.m_Emissive;
				Em4.w = 0.0f;
				BcCb.emissiveColor = Em4;
			}
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBBasicEffect::GetPtr()->GetBuffer(), 0, nullptr, &BcCb, 0, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBBasicEffect::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//テクスチャとサンプラー
			if (m.m_TextureResource) {
				pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
				//サンプラーは設定に任せる
				SetDeviceSamplerState();
			}
			else {
				//シェーダーリソースもクリア
				pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
				//サンプラーもクリア
				pD3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
			}
			//透明処理なら
			if (GetGameObject()->GetAlphaActive()) {
				//ブレンドステート
				//透明処理
				if (GetBlendState() == BlendState::Additive) {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
				}
				else {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				}
				//ラスタライザステート(裏描画)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				//ラスタライザステート（表描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else {
				//透明処理しない
				//ブレンドステートは設定に任せる
				SetDeviceBlendState();
				//ラスタライザステートは設定に任せる
				SetDeviceRasterizerState();
				//描画
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
		}
		//後始末
		Dev->InitializeStates();
	}


	//--------------------------------------------------------------------------------------
	//	BcPNTBoneModelDraw::Impl
	//--------------------------------------------------------------------------------------
	struct BcPNTBoneModelDraw::Impl {
		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		vector<Matrix4X4> m_LocalBonesMatrix;
		map<wstring, AnimationData> m_AnimationMap;
		wstring m_CurrentAnimeName;
		float m_CurrentAnimeTime;
		Impl() :
			m_CurrentAnimeName(L""),
			m_CurrentAnimeTime(0)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	BcPNTBoneModelDraw描画コンポーネント（ボーンモデル描画用）
	//--------------------------------------------------------------------------------------
	BcPNTBoneModelDraw::BcPNTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		BcPNTStaticModelDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}
	BcPNTBoneModelDraw::~BcPNTBoneModelDraw() {}

	shared_ptr<MeshResource> BcPNTBoneModelDraw::GetMeshResource() const {
		//メッシュがなければリターン
		if (pImpl->m_MeshResource.expired()) {
			throw BaseException(
				L"メッシュが設定されてません",
				L"if (pImpl->m_MeshResource.expired())",
				L"BcPNTBoneModelDraw::GetMeshResource()"
			);
		}
		return pImpl->m_MeshResource.lock();
	}

	void BcPNTBoneModelDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		if (!MeshRes->IsSkining() || MeshRes->GetBoneCount() == 0 || MeshRes->GetSampleCount() == 0) {
			throw BaseException(
				L"メッシュがボーンメッシュではありません",
				L"if (!MeshRes->IsSkining() || MeshRes->GetBoneCount() == 0 || MeshRes->GetSampleCount() == 0)",
				L"BcPNTBoneModelDraw::SetMeshResource()"
			);

		}
		//先頭のボーン数の行列で初期化
		pImpl->m_LocalBonesMatrix.resize(MeshRes->GetBoneCount());
		auto& SampleMatrixVec = MeshRes->GetSampleMatrixVec();
		for (UINT i = 0; i < pImpl->m_LocalBonesMatrix.size(); i++) {
			pImpl->m_LocalBonesMatrix[i] = SampleMatrixVec[i];
		}
		pImpl->m_MeshResource = MeshRes;
	}
	void BcPNTBoneModelDraw::SetMeshResource(const wstring& MeshKey) {
		auto MeshRes = App::GetApp()->GetResource<MeshResource>(MeshKey);
		if (!MeshRes->IsSkining() || MeshRes->GetBoneCount() == 0 || MeshRes->GetSampleCount() == 0) {
			throw BaseException(
				L"メッシュがボーンメッシュではありません",
				L"if (!MeshRes->IsSkining() || MeshRes->GetBoneCount() == 0 || MeshRes->GetSampleCount() == 0)",
				L"BcPNTBoneModelDraw::SetMeshResource()"
			);

		}
		//先頭のボーン数の行列で初期化
		pImpl->m_LocalBonesMatrix.resize(MeshRes->GetBoneCount());
		auto& SampleMatrixVec = MeshRes->GetSampleMatrixVec();
		for (UINT i = 0; i < pImpl->m_LocalBonesMatrix.size(); i++) {
			pImpl->m_LocalBonesMatrix[i] = SampleMatrixVec[i];
		}
		pImpl->m_MeshResource = MeshRes;
	}


	void BcPNTBoneModelDraw::OnCreate() {
		SetLightingEnabled(true);
		//マルチライトの設定
		for (int i = 0; i < GetMaxDirectionalLights(); i++) {
			SetLightEnabled(i, true);
		}
	}

	void BcPNTBoneModelDraw::AddAnimation(const wstring& Name, int StartSample, int SampleLength, bool Loop,
		float SamplesParSecond) {
		if (Name == L"") {
			throw BaseException(
				L"アニメーション名が空白です",
				L"if (Name == L\"\")",
				L"BcPNTBoneModelDraw::AddAnimation()"
			);
		}
		if (StartSample < 0 || SampleLength < 0) {
			throw BaseException(
				L"開始サンプルかサンプル数が0未満です",
				L"if (StartSample < 0 || SampleLength < 0)",
				L"BcPNTBoneModelDraw::AddAnimation()"
			);
		}
		if (SamplesParSecond <= 0.0f) {
			throw BaseException(
				L"サンプル毎秒が0以下です",
				L"if (SamplesParSecond <= 0.0f)",
				L"BcPNTBoneModelDraw::AddAnimation()"
			);
		}
		//重複キーがあれば差し替える
		AnimationData Data((UINT)StartSample, (UINT)SampleLength, Loop, SamplesParSecond);
		pImpl->m_AnimationMap[Name] = Data;
		if (pImpl->m_AnimationMap.size() == 1) {
			//1つしか登録がなかったら、カレントアニメは該当アニメとなる
			ChangeCurrentAnimation(Name, 0);
		}


	}

	void BcPNTBoneModelDraw::ChangeCurrentAnimation(const wstring& AnemationName, float StartTime) {
		if (AnemationName == L"") {
			throw BaseException(
				L"アニメーション名が空白です",
				L"if (AnemationName == L\"\")",
				L"BcPNTBoneModelDraw::SetCurrentAnimation()"
			);
		}
		auto it = pImpl->m_AnimationMap.find(AnemationName);
		if (it != pImpl->m_AnimationMap.end()) {
			//指定の名前が見つかった
			pImpl->m_CurrentAnimeName = AnemationName;
			pImpl->m_CurrentAnimeTime = StartTime;
			//アニメーションは終了していない
			it->second.m_IsAnimeEnd = false;
		}
		else {
			//見つからない
			throw BaseException(
				L"指定のアニメーションは登録されてません",
				AnemationName,
				L"BcPNTBoneModelDraw::SetCurrentAnimation()"
			);
		}
	}

	const wstring& BcPNTBoneModelDraw::GetCurrentAnimation() const {
		return pImpl->m_CurrentAnimeName;
	}

	bool BcPNTBoneModelDraw::UpdateAnimation(float ElapsedTime) {
		if (ElapsedTime < 0.0f) {
			throw BaseException(
				L"アニメーション更新にマイナスは設定できません",
				L"if (ElapsedTime < 0.0f)",
				L"BcPNTBoneModelDraw::UpdateAnimation()"
			);
		}
		if (pImpl->m_CurrentAnimeName == L"") {
			//見つからない
			throw BaseException(
				L"カレントアニメーションが設定されてません",
				L"if (pImpl->m_CurrentAnimeName == L\"\")",
				L"BcPNTBoneModelDraw::UpdateAnimation()"
			);
		}
		auto PtrMesh = GetMeshResource();
		UINT SampleCount = PtrMesh->GetSampleCount();
		auto& SampleMatrixVec = PtrMesh->GetSampleMatrixVec();
		UINT BoneCount = PtrMesh->GetBoneCount();
		auto& TgtAnimeData = pImpl->m_AnimationMap[pImpl->m_CurrentAnimeName];
		if (TgtAnimeData.m_StartSample >= SampleCount) {
			//スタートのサンプルが最後のサンプル以降だった
			TgtAnimeData.m_StartSample = SampleCount - 1;
			TgtAnimeData.m_SampleLength = 0;
			UINT UITgtSample = TgtAnimeData.m_StartSample;
			//最後のサンプルを表示
			for (UINT i = 0; i < pImpl->m_LocalBonesMatrix.size(); i++) {
				pImpl->m_LocalBonesMatrix[i] = SampleMatrixVec[BoneCount * UITgtSample + i];
			}
			pImpl->m_CurrentAnimeTime = 0;
			if (TgtAnimeData.m_IsLoop) {
				TgtAnimeData.m_IsAnimeEnd = false;
				return false;
			}
			else {
				TgtAnimeData.m_IsAnimeEnd = true;
				return true;
			}
		}
		//すでにアニメが終了している
		if (TgtAnimeData.m_IsAnimeEnd) {
			//現在のローカル行列を使用
			return true;
		}
		//カレントタイムを更新
		pImpl->m_CurrentAnimeTime += ElapsedTime;
		//スタート位置を計算
		auto FLOATTgtSample = (float)TgtAnimeData.m_StartSample + pImpl->m_CurrentAnimeTime * TgtAnimeData.m_SamplesParSecond;
		UINT UITgtSample = (UINT)FLOATTgtSample;
		UINT UILastSample = TgtAnimeData.m_StartSample + TgtAnimeData.m_SampleLength;
		if (UILastSample >= SampleCount) {
			UILastSample = SampleCount - 1;
		}
		if (UITgtSample >= UILastSample) {
			UITgtSample = UILastSample - 1;
			//最後のサンプルを表示
			for (UINT i = 0; i < pImpl->m_LocalBonesMatrix.size(); i++) {
				pImpl->m_LocalBonesMatrix[i] = SampleMatrixVec[BoneCount * UITgtSample + i];
			}
			if (TgtAnimeData.m_IsLoop) {
				TgtAnimeData.m_IsAnimeEnd = false;
				//ループするのでカレントタイムを0にする
				pImpl->m_CurrentAnimeTime = 0;
				return false;
			}
			else {
				pImpl->m_CurrentAnimeTime = TgtAnimeData.m_SampleLength / TgtAnimeData.m_SamplesParSecond;
				TgtAnimeData.m_IsAnimeEnd = true;
				return true;
			}
		}
		else {
			//サンプルとサンプルの間の割合を計算
			FLOATTgtSample -= (float)UITgtSample;
			UINT UINextSample = UITgtSample + 1;
			for (UINT i = 0; i < pImpl->m_LocalBonesMatrix.size(); i++) {
				InterpolationMatrix(
					SampleMatrixVec[BoneCount * UITgtSample + i],
					SampleMatrixVec[BoneCount * UINextSample + i],
					FLOATTgtSample, pImpl->m_LocalBonesMatrix[i]);
			}
			//アニメは終わってない
			return false;
		}
	}


	void BcPNTBoneModelDraw::InterpolationMatrix(const Matrix4X4& m1, const Matrix4X4& m2, float t, Matrix4X4& out) {
		Vector3 Scale1, Pos1;
		Quaternion Qt1;
		m1.Decompose(Scale1, Qt1, Pos1);
		Qt1.Normalize();

		Vector3 Scale2, Pos2;
		Quaternion Qt2;

		m2.Decompose(Scale2, Qt2, Pos2);
		Qt2.Normalize();

		Vector3 ScaleOut, PosOut;
		Quaternion QtOut;

		ScaleOut = Lerp::CalculateLerp(Scale1, Scale2, 0.0f, 1.0f, t, Lerp::Linear);
		PosOut = Lerp::CalculateLerp(Pos1, Pos2, 0.0f, 1.0f, t, Lerp::Linear);
		QtOut = QuaternionEX::Slerp(Qt1, Qt2, t);
		out.DefTransformation(ScaleOut, QtOut, PosOut);
	}


	const vector< Matrix4X4 >* BcPNTBoneModelDraw::GetVecLocalBonesPtr() const {
		return &pImpl->m_LocalBonesMatrix;
	}


	void BcPNTBoneModelDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"BcPNTStaticModelDraw::OnDraw()"
			);
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };


		//ライトを得る
		auto PtrLightObj = GetGameObject()->GetStage()->GetLight();
		auto PtrMultiLight = dynamic_pointer_cast<MultiLight>(PtrLightObj);
		if (PtrMultiLight) {
			//マルチライトだった
			for (int i = 0; i < GetMaxDirectionalLights(); i++) {
				if (IsLightEnabled(i)) {
					SetLightDirection(i, PtrMultiLight->GetLight(i).m_Directional);
					SetLightDiffuseColor(i, PtrMultiLight->GetLight(i).m_DiffuseColor);
					SetLightSpecularColor(i, PtrMultiLight->GetLight(i).m_SpecularColor);
				}
			}
		}
		else {
			//そうではない
			auto LightPtr = GetGameObject()->OnGetDrawLight();
			SetLightEnabled(0, true);
			SetLightDirection(0, LightPtr.m_Directional);
			SetLightDiffuseColor(0, LightPtr.m_DiffuseColor);
			SetLightSpecularColor(0, LightPtr.m_SpecularColor);
			for (int i = 1; i < GetMaxDirectionalLights(); i++) {
				SetLightEnabled(i, false);
			}
		}
		SetAmbientLightColor(PtrLightObj->GetAmbientLightColor());

		//シェーダの設定
		if (IsPerPixelLighting()) {
			//ピクセルライティング
			if (IsOwnShadowActive()) {
				//影付き
				if (IsBiasedNormals()) {
					//バイアス付き
					pD3D11DeviceContext->VSSetShader(BcVSPNTBonePLBnShadow::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBonePLBnShadow::GetPtr()->GetInputLayout());
				}
				else {
					//バイアス無し
					pD3D11DeviceContext->VSSetShader(BcVSPNTBonePLShadow::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBonePLShadow::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTPLShadow::GetPtr()->GetShader(), nullptr, 0);
			}
			else {
				//影無し
				if (IsBiasedNormals()) {
					//バイアス付き
					pD3D11DeviceContext->VSSetShader(BcVSPNTBonePLBn::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBonePLBn::GetPtr()->GetInputLayout());
				}
				else {
					//バイアス無し
					pD3D11DeviceContext->VSSetShader(BcVSPNTBonePL::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBonePL::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTPL::GetPtr()->GetShader(), nullptr, 0);
			}
		}
		else {
			//頂点ライティング
			if (IsOwnShadowActive()) {
				//影付き
				if (IsBiasedNormals()) {
					//バイアス付き
					pD3D11DeviceContext->VSSetShader(BcVSPNTBoneVLBnShadow::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBoneVLBnShadow::GetPtr()->GetInputLayout());
				}
				else {
					//バイアス無し
					pD3D11DeviceContext->VSSetShader(BcVSPNTBoneVLShadow::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBoneVLShadow::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTVLShadow::GetPtr()->GetShader(), nullptr, 0);
			}
			else {
				//影無し
				if (IsBiasedNormals()) {
					//バイアス付き
					pD3D11DeviceContext->VSSetShader(BcVSPNTBoneVLBn::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBoneVLBn::GetPtr()->GetInputLayout());
				}
				else {
					//バイアス無し
					pD3D11DeviceContext->VSSetShader(BcVSPNTBoneVL::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBoneVL::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTVL::GetPtr()->GetShader(), nullptr, 0);
			}
		}
		//影とサンプラー
		if (IsOwnShadowActive()) {
			//シャドウマップのレンダラーターゲット
			auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
			ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
			pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
			//シャドウマップサンプラー
			ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
			pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
		}
		//ストライドとオフセット
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//デプスステンシルステートは設定に任せる
		SetDeviceDepthStencilState();

		//コンスタントバッファの設定
		BasicEffectConstants BcCb;
		ZeroMemory(&BcCb, sizeof(BcCb));
		if (IsOwnShadowActive()) {
			SetConstants(BcCb, true);
		}
		else {
			SetConstants(BcCb);
		}

		//これよりマテリアルごとの描画
		auto& MatVec = MeshRes->GetMaterialExVec();
		for (auto& m : MatVec) {
			if (m.m_TextureResource && IsModelTextureEnabled()) {
				//テクスチャがありテクスチャが有効である
				BcCb.activeFlg.y = 1;
			}
			else {
				//テクスチャがない
				BcCb.activeFlg.y = 0;
			}
			if (IsModelDiffusePriority()) {
				BcCb.diffuseColor = m.m_Diffuse;
			}
			if (IsModelEmissivePriority()) {
				Color4 Em4 = m.m_Emissive;
				Em4.w = 0.0f;
				BcCb.emissiveColor = Em4;
			}
			//ボーンの設定
			size_t BoneSz = pImpl->m_LocalBonesMatrix.size();
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				Matrix4X4 mat = pImpl->m_LocalBonesMatrix[b];
				mat.Transpose();
				BcCb.bones[cb_count] = ((XMMATRIX)mat).r[0];
				BcCb.bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
				BcCb.bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
				cb_count += 3;
			}

			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBBasicEffect::GetPtr()->GetBuffer(), 0, nullptr, &BcCb, 0, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBBasicEffect::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//テクスチャとサンプラー
			if (m.m_TextureResource) {
				pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
				//サンプラーは設定に任せる
				SetDeviceSamplerState();
			}
			else {
				//シェーダーリソースもクリア
				pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
				//サンプラーもクリア
				pD3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
			}
			//透明処理なら
			if (GetGameObject()->GetAlphaActive()) {
				//ブレンドステート
				//透明処理
				if (GetBlendState() == BlendState::Additive) {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
				}
				else {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				}
				//ラスタライザステート(裏描画)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				//ラスタライザステート（表描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else {
				//透明処理しない
				//ブレンドステートは設定に任せる
				SetDeviceBlendState();
				//ラスタライザステートは設定に任せる
				SetDeviceRasterizerState();
				//描画
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
		}
		//後始末
		Dev->InitializeStates();
	}





}

//end basecross
