/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�`��p�̃V�F�[�_�N���X�̎��́i�}�N���g�p�j
	//--------------------------------------------------------------------------------------
	//Shadowmap
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBShadow)
	IMPLEMENT_DX11_VERTEX_SHADER(VSShadowmap, App::GetApp()->GetShadersPath() + L"VSShadowmap.cso")

	//PNTStatic
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPNTStatic)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStatic, App::GetApp()->GetShadersPath() + L"VSPNTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStatic, App::GetApp()->GetShadersPath() + L"PSPNTStatic.cso")

	//CBPNTStaticShadow
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPNTStaticShadow)
	//PNTStaticShadow
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStaticShadow, App::GetApp()->GetShadersPath() + L"VSPNTStaticShadow.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticShadow, App::GetApp()->GetShadersPath() + L"PSPNTStaticShadow.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticShadow2, App::GetApp()->GetShadersPath() + L"PSPNTStaticShadow2.cso")


	//PCTSprite
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBSprite)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTSprite, App::GetApp()->GetShadersPath() + L"VSPCTSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTSprite, App::GetApp()->GetShadersPath() + L"PSPCTSprite.cso")


}
//end basecross


