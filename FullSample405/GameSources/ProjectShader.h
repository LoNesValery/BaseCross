/*!
@file ProjectShader.h
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�R���X�^���g�o�b�t�@�\����
	//--------------------------------------------------------------------------------------
	struct TestConstants
	{
		/// ���[���h�s��
		Matrix4X4 World;
		/// �r���[�s��
		Matrix4X4 View;
		/// �ˉe�s��
		Matrix4X4 Projection;
		/// �G�~�b�V�u�F
		Color4 Emissive;
		/// �f�t���[�Y�F
		Color4 Diffuse;
		/// ���C�C�g����
		Vector4 LightDir;
		TestConstants() {
			memset(this, 0, sizeof(TestConstants));
			Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	//�R���X�^���g�o�b�t�@�p�}�N��
	DECLARE_DX11_CONSTANT_BUFFER(CBTest, TestConstants)
	//���_�V�F�[�_�p�}�N��
	DECLARE_DX11_VERTEX_SHADER(VSTest, VertexPositionNormalTexture)
	//�s�N�Z���V�F�[�_�p�}�N��
	DECLARE_DX11_PIXEL_SHADER(PSTest)


}
//end basecross

