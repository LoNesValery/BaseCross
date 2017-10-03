/*!
@file ProjectShader.h
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	Shadow�R���X�^���g�o�b�t�@�\����
	//--------------------------------------------------------------------------------------
	struct ShadowConstants
	{
		/// ���[���h�s��
		bsm::Mat4x4 mWorld;
		/// �r���[�s��
		bsm::Mat4x4 mView;
		/// �ˉe�s��
		bsm::Mat4x4 mProj;
		/// Bone�p
		bsm::Vec4 Bones[3 * 72];
		ShadowConstants() {
			memset(this, 0, sizeof(ShadowConstants));
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBShadow, ShadowConstants)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmap, VertexPositionNormalTexture)


	//--------------------------------------------------------------------------------------
	///	�`��p�̃V�F�[�_�N���X�̐錾�i�}�N���g�p�j
	//--------------------------------------------------------------------------------------
	//PNTStatic
	struct PNTStaticConstantBuffer
	{
		Mat4x4 World;
		Mat4x4 View;
		Mat4x4 Projection;
		Vec4 LightDir;
		Col4 Emissive;
		Col4 Diffuse;
		PNTStaticConstantBuffer() {
			memset(this, 0, sizeof(PNTStaticConstantBuffer));
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBPNTStatic, PNTStaticConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNTStatic, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStatic)


	struct PNTStaticShadowConstantBuffer
	{
		bsm::Mat4x4 World;
		bsm::Mat4x4 View;
		bsm::Mat4x4 Projection;
		bsm::Col4 Emissive;
		bsm::Col4 Diffuse;
		bsm::Vec4 LightDir;
		bsm::Vec4 LightPos;
		bsm::Vec4 EyePos;
		bsm::Mat4x4 LightView;
		bsm::Mat4x4 LightProjection;
		PNTStaticShadowConstantBuffer() {
			memset(this, 0, sizeof(PNTStaticShadowConstantBuffer));
			Diffuse = bsm::Col4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	//PNTStaticShadowConstantBuffer
	DECLARE_DX11_CONSTANT_BUFFER(CBPNTStaticShadow, PNTStaticShadowConstantBuffer)
	///PNTStaticShadow
	DECLARE_DX11_VERTEX_SHADER(VSPNTStaticShadow, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow2)


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

	//PCTSprite
	DECLARE_DX11_VERTEX_SHADER(VSPCTSprite, VertexPositionColorTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPCTSprite)


}
//end basecross

