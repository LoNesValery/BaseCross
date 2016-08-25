/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	IMPLEMENT_DX12SHADER(VSPNStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNStatic.cso")
	IMPLEMENT_DX12SHADER(PSPNStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNStatic.cso")

}
//end basecross


