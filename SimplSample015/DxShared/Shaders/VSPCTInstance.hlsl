
#include "INCStructs.hlsli"


cbuffer ConstantBuffer : register(b0)
{
	float4x4 World	: packoffset(c0);	//���̃��[���h�s��͎g�p���Ȃ�
	float4x4 View	: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 Emissive : packoffset(c12);
	float4 Diffuse : packoffset(c13);
};


struct VSPCTInstanceInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float4x4 mat : MATRIX;          // �C���X�^���X���Ƃɐݒ肳���s��
	uint InstanceId           : SV_InstanceID;   // �C���X�^���X�h�c
};


PSPCTInput main(VSPCTInstanceInput input)
{
	PSPCTInput result;
	//���_�̈ʒu��ϊ�
	float4 pos = float4(input.position.xyz, 1.0f);
	//���[���h�ϊ�
	pos = mul(pos, input.mat);
	//�r���[�ϊ�
	pos = mul(pos, View);
	//�ˉe�ϊ�
	pos = mul(pos, Projection);
	//�s�N�Z���V�F�[�_�ɓn���ϐ��ɐݒ�
	result.position = pos;
	result.color = input.color;
	result.tex = input.tex;
	return result;
}