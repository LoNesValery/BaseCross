/*!
@file ProjectShader.cpp
@brief プロジェクトで使用するシェーダー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	描画用のシェーダクラスの実体（マクロ使用）
	//--------------------------------------------------------------------------------------
	//PNTStatic
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPNTStatic)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStatic, App::GetApp()->GetShadersPath() + L"VSPNTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStatic, App::GetApp()->GetShadersPath() + L"PSPNTStatic.cso")

	//CBPCTStatic
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPCTStatic)
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTStatic, App::GetApp()->GetShadersPath() + L"PSPCTStatic.cso")
	//PCTStaticInstance
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTStaticInstance, App::GetApp()->GetShadersPath() + L"VSPCTStaticInstance.cso")

	//PCTSprite
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBSprite)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTSprite, App::GetApp()->GetShadersPath() + L"VSPCTSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTSprite, App::GetApp()->GetShadersPath() + L"PSPCTSprite.cso")


}
//end basecross


