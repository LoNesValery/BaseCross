/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	IMPLEMENT_DX12SHADER(VSPCDirect, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCDirect.cso")
	IMPLEMENT_DX12SHADER(PSPCDirect, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCDirect.cso")


}
//end basecross


