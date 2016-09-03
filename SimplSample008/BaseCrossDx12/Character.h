/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	�X�v���C�g�`��(�e�N���X)
	//--------------------------------------------------------------------------------------
	class SpriteDraw : public ObjectInterface {
	protected:
		bool m_Trace;					///<�����������邩�ǂ���
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		shared_ptr<TextureResource> m_TextureResource;	///<�e�N�X�`�����\�[�X
														///���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		///GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		///�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };
		///�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_PipelineState;
		///�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		///�e�������֐�
		///���[�g�V�O�l�`���쐬
		void CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		void CreateDescriptorHeap();
		///�T���v���[�쐬
		void CreateSampler();
		///�V�F�[�_�[���\�[�X�r���[�쐬
		void CreateShaderResourceView();
		///�R���X�^���g�o�b�t�@�쐬
		void CreateConstantBufferBase(UINT BuffSize);
		virtual void CreateConstantBuffer() = 0;
		///�p�C�v���C���X�e�[�g�쐬
		template<typename Vertex, typename VS, typename PS>
		void CreatePipelineStateBase() {
			D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
			m_PipelineState
				= PipelineState::CreateDefault2D<Vertex, VS, PS>(m_RootSignature, PineLineDesc);
			//�����̏ꍇ�̓u�����h�X�e�[�g�����ւ�
			if (m_Trace) {
				D3D12_BLEND_DESC blend_desc;
				D3D12_RENDER_TARGET_BLEND_DESC Target;
				ZeroMemory(&blend_desc, sizeof(blend_desc));
				blend_desc.AlphaToCoverageEnable = false;
				blend_desc.IndependentBlendEnable = false;
				ZeroMemory(&Target, sizeof(Target));
				Target.BlendEnable = true;
				Target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
				Target.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
				Target.BlendOp = D3D12_BLEND_OP_ADD;
				Target.SrcBlendAlpha = D3D12_BLEND_ONE;
				Target.DestBlendAlpha = D3D12_BLEND_ZERO;
				Target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
				Target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
					blend_desc.RenderTarget[i] = Target;
				}
				PineLineDesc.BlendState = blend_desc;
				m_PipelineState = PipelineState::CreateDirect(PineLineDesc);
			}
		}
		virtual void CreatePipelineState() = 0;
		///�R�}���h���X�g�쐬
		void CreateCommandList();
		//�`��e���v���[�g�֐�
		template<typename Vertex>
		void DrawObjectBase(const shared_ptr<MeshResource>& Mesh) {
			//�R�}���h���X�g�̃��Z�b�g
			CommandList::Reset(m_PipelineState, m_CommandList);

			Mesh->UpdateResources<Vertex>(m_CommandList);
			m_TextureResource->UpdateResources(m_CommandList);

			//�`��
			m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
			ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get(), m_SamplerDescriptorHeap.Get() };
			m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

			for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
				m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
			}
			auto Dev = App::GetApp()->GetDeviceResources();
			m_CommandList->RSSetViewports(1, &Dev->GetViewport());
			m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
				Dev->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
				Dev->GetFrameIndex(),
				Dev->GetRtvDescriptorSize());
			CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
				Dev->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
			);
			m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

			m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_CommandList->IASetIndexBuffer(&Mesh->GetIndexBufferView());
			m_CommandList->IASetVertexBuffers(0, 1, &Mesh->GetVertexBufferView());
			m_CommandList->DrawIndexedInstanced(Mesh->GetNumIndicis(), 1, 0, 0, 0);

			//�R�}���h���X�g�̃N���[�Y
			CommandList::Close(m_CommandList);
			//�f�o�C�X�ɃR�}���h���X�g�𑗂�
			Dev->InsertDrawCommandLists(m_CommandList.Get());
		}


		//�v���e�N�g�R���X�g���N�^
		SpriteDraw(const wstring& TextureFileName, bool Trace);
		//�v���e�N�g�f�X�g���N�^
		virtual ~SpriteDraw() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		///�`�揈��
		virtual void DrawObject(const shared_ptr<MeshResource>& Mesh) = 0;
	};

	//--------------------------------------------------------------------------------------
	// Diffuse�����R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	struct SpriteConstantBuffer
	{
		Matrix4X4 World;
		Color4 Emissive;
		SpriteConstantBuffer() {
			memset(this, 0, sizeof(SpriteConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	///	PCT�X�v���C�g�`��
	//--------------------------------------------------------------------------------------
	class PCTSpriteDraw : public SpriteDraw {
	protected:
		///�R���X�^���g�o�b�t�@�쐬
		virtual void CreateConstantBuffer()override;
		///�p�C�v���C���X�e�[�g�쐬
		virtual void CreatePipelineState()override;
	public:
		PCTSpriteDraw(const wstring& TextureFileName, bool Trace);
		virtual ~PCTSpriteDraw() {}
		///�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer(SpriteConstantBuffer& CBuff);
		///�`�揈��
		virtual void DrawObject(const shared_ptr<MeshResource>& Mesh)override;
	};


	//--------------------------------------------------------------------------------------
	// Diffuse����R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	struct DiffuseSpriteConstantBuffer
	{
		Matrix4X4 World;
		Color4 Emissive;
		Color4 Diffuse;
		DiffuseSpriteConstantBuffer() {
			memset(this, 0, sizeof(DiffuseSpriteConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	///	PT�X�v���C�g�`��
	//--------------------------------------------------------------------------------------
	class PTSpriteDraw : public SpriteDraw {
	protected:
		///�R���X�^���g�o�b�t�@�쐬
		virtual void CreateConstantBuffer()override;
		///�p�C�v���C���X�e�[�g�쐬
		virtual void CreatePipelineState()override;
	public:
		PTSpriteDraw(const wstring& TextureFileName, bool Trace);
		virtual ~PTSpriteDraw() {}
		///�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer(DiffuseSpriteConstantBuffer& CBuff);
		///�`�揈��
		virtual void DrawObject(const shared_ptr<MeshResource>& Mesh)override;
	};


	//--------------------------------------------------------------------------------------
	///	�ǃX�v���C�g
	//--------------------------------------------------------------------------------------
	class WallSprite : public ObjectInterface, public ShapeInterface {
		//PT�X�v���C�g�`��
		shared_ptr<PCTSpriteDraw> m_PCTSpriteDraw;
		//Diffuse�����R���X�^���g�o�b�t�@
		SpriteConstantBuffer m_SpriteConstantBuffer;
		//���b�V��
		shared_ptr<MeshResource> m_SquareMesh;
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		bool m_Trace;					///<�����������邩�ǂ���
		Vector2 m_Scale;				///<�X�P�[�����O
		Vector2 m_Pos;				///<�ʒu
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	TextureFileName	�e�N�X�`���t�@�C����
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		WallSprite(const wstring& TextureFileName, bool Trace, const Vector2& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~WallSprite();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};



	//--------------------------------------------------------------------------------------
	///	�l�p�`�X�v���C�g
	//--------------------------------------------------------------------------------------
	class SquareSprite : public ObjectInterface, public ShapeInterface {
		//PT�X�v���C�g�`��
		shared_ptr<PTSpriteDraw> m_PTSpriteDraw;
		//Diffuse����R���X�^���g�o�b�t�@
		DiffuseSpriteConstantBuffer m_DiffuseSpriteConstantBuffer;
		//���b�V��
		shared_ptr<MeshResource> m_SquareMesh;
		//�o�b�N�A�b�v���Ă������_�f�[�^
		vector<VertexPositionTexture> m_BackupVertices;
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		bool m_Trace;					///<�����������邩�ǂ���
		Vector2 m_Scale;				///<�X�P�[�����O
		Vector2 m_Pos;				///<�ʒu
		float m_TotalTime;
		//���_�̕ύX
		void UpdateVertex(float ElapsedTime);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	TextureFileName	�e�N�X�`���t�@�C����
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		SquareSprite(const wstring& TextureFileName, bool Trace, const Vector2& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareSprite();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

}
//end basecross
