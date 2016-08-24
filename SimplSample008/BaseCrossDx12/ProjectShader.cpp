/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	IMPLEMENT_DX12SHADER(VSPTSprite, App::GetApp()->m_wstrRelativeShadersPath + L"VSPTSprite.cso")
	IMPLEMENT_DX12SHADER(PSPTSprite, App::GetApp()->m_wstrRelativeShadersPath + L"PSPTSprite.cso")

	IMPLEMENT_DX12SHADER(VSPCTSprite, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCTSprite.cso")
	IMPLEMENT_DX12SHADER(PSPCTSprite, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCTSprite.cso")


}
//end basecross


