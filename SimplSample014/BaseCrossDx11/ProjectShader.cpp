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
	//PNTStatic
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPNTStatic)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTInstance, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTInstance.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStatic.cso")

}
//end basecross


