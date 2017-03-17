/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	//�R���X�^���g�o�b�t�@���̃}�N��
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBTest)
	//���_�V�F�[�_���̃}�N��
	IMPLEMENT_DX11_VERTEX_SHADER(VSTest, App::GetApp()->GetShadersPath() + L"VSTest.cso")
	//�s�N�Z���V�F�[�_���̃}�N��
	IMPLEMENT_DX11_PIXEL_SHADER(PSTest, App::GetApp()->GetShadersPath() + L"PSTest.cso")


}
//end basecross


