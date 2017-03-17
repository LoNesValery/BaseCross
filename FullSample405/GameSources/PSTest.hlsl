
//���ʂ̍��ڂ��C���N���[�h
#include "INCTest.hlsli"

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSPNTInput input) : SV_TARGET
{
	//���C�e�B���O
	float3 lightdir = normalize(LightDir.xyz);
	float4 Light = (saturate(dot(input.norm, -lightdir)) * Diffuse) + Emissive;
	Light.a = Diffuse.a;
	Light = g_texture.Sample(g_sampler, input.tex) * Light;
	return Light;
}
