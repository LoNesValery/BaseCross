/*!
@file ProjectShader.h
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//�X�v���C�g�p�R���X�^���g�o�b�t�@�\����
	struct SpriteConstantBuffer
	{
		Mat4x4 World;
		Col4 Emissive;
		SpriteConstantBuffer() {
			memset(this, 0, sizeof(SpriteConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBSprite, SpriteConstantBuffer)

	// Diffuse����R���X�^���g�o�b�t�@
	struct DiffuseSpriteConstantBuffer
	{
		Mat4x4 World;
		Col4 Emissive;
		Col4 Diffuse;
		DiffuseSpriteConstantBuffer() {
			memset(this, 0, sizeof(DiffuseSpriteConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBDiffuseSprite, DiffuseSpriteConstantBuffer)
	//PTSprite
	DECLARE_DX11_VERTEX_SHADER(VSPTSprite, VertexPositionTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPTSprite)

	//PCTSprite
	DECLARE_DX11_VERTEX_SHADER(VSPCTSprite, VertexPositionColorTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPCTSprite)


}
//end basecross

