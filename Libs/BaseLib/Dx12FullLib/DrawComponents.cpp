/*!
@file DrawComponents.cpp
@brief �`��R���|�[�l���g����
*/

#include "stdafx.h"

namespace basecross {


	//������ʃV�F�[�_�[
	IMPLEMENT_DX12SHADER(VSPCDirect, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCDirect.cso")
	IMPLEMENT_DX12SHADER(PSPCDirect, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCDirect.cso")

	IMPLEMENT_DX12SHADER(VSPTDirect, App::GetApp()->m_wstrRelativeShadersPath + L"VSPTDirect.cso")
	IMPLEMENT_DX12SHADER(PSPTDirect, App::GetApp()->m_wstrRelativeShadersPath + L"PSPTDirect.cso")

	IMPLEMENT_DX12SHADER(VSPCStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCStatic.cso")
	IMPLEMENT_DX12SHADER(PSPCStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCStatic.cso")

	IMPLEMENT_DX12SHADER(VSPTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPTStatic.cso")
	IMPLEMENT_DX12SHADER(PSPTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPTStatic.cso")

	IMPLEMENT_DX12SHADER(VSPNTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTStatic.cso")
	IMPLEMENT_DX12SHADER(PSPNTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStatic.cso")


	//--------------------------------------------------------------------------------------
	//	struct DrawComponent::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct DrawComponent::Impl {
		BlendState m_BlendState;
		DepthStencilState m_DepthStencilState;
		RasterizerState m_RasterizerState;
		SamplerState m_SamplerState;
		Matrix4X4 m_MeshToTransformMatrix;
		Impl() :
			m_MeshToTransformMatrix() {}
	};

	//--------------------------------------------------------------------------------------
	///	�`��R���|�[�l���g�̐e�N���X����
	//--------------------------------------------------------------------------------------
	DrawComponent::DrawComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	DrawComponent::~DrawComponent() {}


	BlendState DrawComponent::GetBlendState() const {
		return pImpl->m_BlendState;
	}
	DepthStencilState DrawComponent::GetDepthStencilState() const {
		return pImpl->m_DepthStencilState;
	}
	RasterizerState DrawComponent::GetRasterizerState() const {
		return pImpl->m_RasterizerState;
	}
	SamplerState DrawComponent::GetSamplerState() const {
		return pImpl->m_SamplerState;
	}


	void DrawComponent::SetBlendState(const BlendState state) {
		pImpl->m_BlendState = state;
	}
	void DrawComponent::SetDepthStencilState(const DepthStencilState state) {
		pImpl->m_DepthStencilState = state;

	}
	void DrawComponent::SetRasterizerState(const RasterizerState state) {
		pImpl->m_RasterizerState = state;
	}
	void DrawComponent::SetSamplerState(const SamplerState state) {
		pImpl->m_SamplerState = state;
	}

	const Matrix4X4& DrawComponent::GetMeshToTransformMatrix() const {
		return pImpl->m_MeshToTransformMatrix;
	}
	void DrawComponent::SetMeshToTransformMatrix(const Matrix4X4& Mat) {
		pImpl->m_MeshToTransformMatrix = Mat;
	}


	//--------------------------------------------------------------------------------------
	//	struct PNTStaticDraw::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PNTStaticDraw::Impl {
		//�`��R���e�L�X�g
		shared_ptr<VSPSDrawContext> m_DrawContext;
		//�R���X�^���g�o�b�t�@�\����
		struct ConstantBuffer
		{
			Matrix4X4 World;
			Matrix4X4 View;
			Matrix4X4 Projection;
			Vector4 LightDir;
			Color4 Emissive;
			Color4 Diffuse;
			ConstantBuffer() {
				memset(this, 0, sizeof(ConstantBuffer));
			};
		};
		//�R���X�^���g�o�b�t�@�f�[�^
		ConstantBuffer m_ConstantBufferData;
		//���b�V�����\�[�X
		weak_ptr<MeshResource> m_MeshResource;
		//�e�N�X�`��
		weak_ptr<TextureResource> m_TextureResource;
		Color4 m_Emissive;	//�F
		Color4 m_Diffuse;	//�f�t�B�[�Y���C�g
		bool m_OwnShadowActive;
		Impl() :
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0f),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_OwnShadowActive(false)
		{
			m_DrawContext = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateSrvSmpCbv);
			m_DrawContext->CreateConstantBuffer(sizeof(m_ConstantBufferData));
			m_DrawContext->CreateDefault3DPipelineCmdList<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>();
			//�e�s���Identity�ɏ�����
			ZeroMemory(&m_ConstantBufferData, sizeof(m_ConstantBufferData));
			m_ConstantBufferData.World = Matrix4X4EX::Identity();
			m_ConstantBufferData.View = Matrix4X4EX::Identity();
			m_ConstantBufferData.Projection = Matrix4X4EX::Identity();
			//�����l�X�V
			m_DrawContext->UpdateConstantBuffer(reinterpret_cast<void**>(&m_ConstantBufferData), sizeof(m_ConstantBufferData));
		}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	//	class PNTStaticDraw : public DrawComponent;
	//	�p�r: PNTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PNTStaticDraw::PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl()) {
		//�p�C�v���C���X�e�[�g���f�t�H���g�̂RD
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PNTStaticDraw::~PNTStaticDraw() {}

	void PNTStaticDraw::OnCreate() {
	}

	shared_ptr<MeshResource> PNTStaticDraw::GetMeshResource() const {
		//���b�V�����Ȃ���΃��^�[��
		auto shptr = pImpl->m_MeshResource.lock();
		if (!shptr) {
			throw BaseException(
				L"���b�V�����ݒ肳��Ă܂���",
				L"if (!shptr)",
				L"PNTStaticDraw::GetMeshResource()"
			);
		}
		return shptr;
	}

	void PNTStaticDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_MeshResource = MeshRes;
	}
	void PNTStaticDraw::SetMeshResource(const wstring& MeshKey) {
		pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(MeshKey);
	}
	void PNTStaticDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_TextureResource = TextureRes;
		pImpl->m_DrawContext->SetTextureResource(TextureRes);
	}
	void PNTStaticDraw::SetTextureResource(const wstring& TextureKey) {
		auto Res = App::GetApp()->GetResource<TextureResource>(TextureKey);
		pImpl->m_TextureResource = Res;
		pImpl->m_DrawContext->SetTextureResource(Res);
	}
	shared_ptr<TextureResource> PNTStaticDraw::GetTextureResource() const {
		//�e�N�X�`�����Ȃ����null��Ԃ�
		auto shptr = pImpl->m_TextureResource.lock();
		if (!shptr){
			return nullptr;
		}
		return pImpl->m_TextureResource.lock();
	}

	Color4 PNTStaticDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}
	void PNTStaticDraw::SetEmissive(const Color4& col) {
		pImpl->m_Emissive = col;
	}

	Color4 PNTStaticDraw::GetDiffuse() const {
		return pImpl->m_Diffuse;
	}
	void PNTStaticDraw::SetDiffuse(const Color4& col) {
		pImpl->m_Diffuse = col;
	}




	void PNTStaticDraw::DrawNotShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		auto PtrMeshResource = GetMeshResource();
		//�s��̒�`
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//���[���h�s��̌���
		pImpl->m_ConstantBufferData.World.AffineTransformation(
			PtrTrans->GetScale(),			//�X�P�[�����O
			PtrTrans->GetPivot(),		//��]�̒��S�i�d�S�j
			PtrTrans->GetQuaternion(),				//��]�p�x
			PtrTrans->GetPosition()				//�ʒu
		);
		//�]�u����
		pImpl->m_ConstantBufferData.World.Transpose();
		//�r���[�s��̌���
		pImpl->m_ConstantBufferData.View.LookAtLH(Vector3(0, 2.0, -5.0f), Vector3(0, 0, 0), Vector3(0, 1.0f, 0));
		//�]�u����
		pImpl->m_ConstantBufferData.View.Transpose();
		//�ˉe�s��̌���
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		pImpl->m_ConstantBufferData.Projection.PerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);
		//�]�u����
		pImpl->m_ConstantBufferData.Projection.Transpose();
		//���C�e�B���O
		pImpl->m_ConstantBufferData.LightDir = Vector4(0.5f, -1.0f, 0.5f, 1.0f);
		//�G�~�b�V�u
		pImpl->m_ConstantBufferData.Emissive = Color4(0, 0, 0, 0);
		//�f�B�t���[�Y
		pImpl->m_ConstantBufferData.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		//�X�V
		pImpl->m_DrawContext->UpdateConstantBuffer(&pImpl->m_ConstantBufferData, sizeof(pImpl->m_ConstantBufferData));
		//�`��
		pImpl->m_DrawContext->DrawIndexed<VertexPositionNormalTexture>(PtrMeshResource);
	}

	void PNTStaticDraw::OnDraw() {
		DrawNotShadow();
	}



}
//end basecross
