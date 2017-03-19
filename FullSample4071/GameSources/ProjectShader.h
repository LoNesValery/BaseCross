/*!
@file ProjectShader.h
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	//�J�X�^���V���h�E�}�b�v�p�R���X�^���g�o�b�t�@�\����
	struct CustomShadowmapConstantBuffer
	{
		XMMATRIX mWorld[3];
		XMMATRIX mView;
		XMMATRIX mProj;
		CustomShadowmapConstantBuffer() {
			memset(this, 0, sizeof(CustomShadowmapConstantBuffer));
		};
	};
	//�V�F�[�_�錾�i�}�N���g�p�j
	DECLARE_DX11_CONSTANT_BUFFER(CBCustomShadowmap, CustomShadowmapConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSCustomShadowmap, VertexPositionNormalColor)
	DECLARE_DX11_GEOMETRY_SHADER(GSCustomShadowmap)
	//�J�X�^���`��R���X�^���g�o�b�t�@�\����
	struct CustomDrawConstantBuffer
	{
		Matrix4X4 World[3];
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Vector4 Param;	//�ėp�p�����[�^
		CustomDrawConstantBuffer() {
			memset(this, 0, sizeof(CustomDrawConstantBuffer));
		};
	};
	//�V�F�[�_�錾�i�}�N���g�p�j
	DECLARE_DX11_CONSTANT_BUFFER(CBCustomDraw, CustomDrawConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSCustomDraw, VertexPositionNormalColor)
	DECLARE_DX11_GEOMETRY_SHADER(GSCustomDraw)
	DECLARE_DX11_PIXEL_SHADER(PSCustomDraw)


}
//end basecross

