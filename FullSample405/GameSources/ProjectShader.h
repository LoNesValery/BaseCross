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
		Mat4x4 World;
		/// �r���[�s��
		Mat4x4 View;
		/// �ˉe�s��
		Mat4x4 Projection;
		/// �G�~�b�V�u�F
		Col4 Emissive;
		/// �f�t���[�Y�F
		Col4 Diffuse;
		/// ���C�C�g����
		Vec4 LightDir;
		TestConstants() {
			memset(this, 0, sizeof(TestConstants));
			Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
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

