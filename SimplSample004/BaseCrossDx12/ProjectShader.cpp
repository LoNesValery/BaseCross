/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	IMPLEMENT_DX12SHADER(VSPCSprite, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCSprite.cso")
	IMPLEMENT_DX12SHADER(PSPCSprite, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCSprite.cso")


}
//end basecross

