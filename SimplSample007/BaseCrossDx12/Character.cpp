/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	PT�X�v���C�g�`��
	//--------------------------------------------------------------------------------------
	PTSpriteDraw::PTSpriteDraw(const wstring& TextureFileName, bool Trace):
		m_TextureFileName(TextureFileName),
		m_Trace(Trace)
	{}

	void PTSpriteDraw::OnCreate() {
		//�e�N�X�`���̍쐬
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		///�e�������֐��Ăяo��
		///���[�g�V�O�l�`���쐬
		CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		CreateDescriptorHeap();
		///�T���v���[�쐬
		CreateSampler();
		///�V�F�[�_�[���\�[�X�r���[�쐬
		CreateShaderResourceView();
		///�R���X�^���g�o�b�t�@�쐬
		CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		CreatePipelineState();
		///�R�}���h���X�g�쐬
		CreateCommandList();
	}

	///���[�g�V�O�l�`���쐬
	void PTSpriteDraw::CreateRootSignature() {
		//���[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
	}


	///�f�X�N�v���^�q�[�v�쐬
	void PTSpriteDraw::CreateDescriptorHeap() {
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CbvSrvDescriptorHandleIncrementSize =
			Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CbvSrv�f�X�N�v���^�q�[�v
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
		//�T���v���[�f�X�N�v���^�q�[�v
		m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SrvHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SamplerHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}
	///�T���v���[�쐬
	void PTSpriteDraw::CreateSampler() {
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		//���b�s���O�T���v���[
		DynamicSampler::CreateSampler(SamplerState::LinearWrap, SamplerDescriptorHandle);
	}
	///�V�F�[�_�[���\�[�X�r���[�쐬
	void PTSpriteDraw::CreateShaderResourceView() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�e�N�X�`���n���h�����쐬
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//�e�N�X�`���̃V�F�[�_���\�[�X�r���[���쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//�t�H�[�}�b�g
		srvDesc.Format = m_TextureResource->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = m_TextureResource->GetTextureResDesc().MipLevels;
		//�V�F�[�_���\�[�X�r���[
		Dev->GetDevice()->CreateShaderResourceView(
			m_TextureResource->GetTexture().Get(),
			&srvDesc,
			Handle);
	}
	///�R���X�^���g�o�b�t�@�쐬
	void PTSpriteDraw::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		UINT ConstBuffSize = (sizeof(DiffuseSpriteConstantBuffer) + 255) & ~255;
		//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"PTSpriteDraw::CreateConstantBuffer()"
		);
		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = ConstBuffSize;
		//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
		//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
		//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"PTSpriteDraw::CreateConstantBuffer()"
		);
	}
	///�p�C�v���C���X�e�[�g�쐬
	void PTSpriteDraw::CreatePipelineState() {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		m_PipelineState
			= PipelineState::CreateDefault2D<VertexPositionTexture, VSPTSprite, PSPTSprite>(m_RootSignature, PineLineDesc);
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
	///�R�}���h���X�g�쐬
	void PTSpriteDraw::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	///�R���X�^���g�o�b�t�@�X�V
	void PTSpriteDraw::UpdateConstantBuffer(DiffuseSpriteConstantBuffer& CBuff) {
		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&CBuff),
			sizeof(CBuff));

	}
	///�`�揈��
	void PTSpriteDraw::DrawObject(const shared_ptr<MeshResource>& Mesh) {
		//�R�}���h���X�g�̃��Z�b�g
		CommandList::Reset(m_PipelineState, m_CommandList);

		Mesh->UpdateResources<VertexPositionTexture>(m_CommandList);
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



	//--------------------------------------------------------------------------------------
	///	�ǃX�v���C�g����
	//--------------------------------------------------------------------------------------
	WallSprite::WallSprite(const wstring& TextureFileName, bool Trace, const Vector2& StartPos) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(StartPos)
	{}
	WallSprite::~WallSprite() {}
	void WallSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��(�c��10���\��)
		vector<VertexPositionTexture> vertices = {
			{ VertexPositionTexture(Vector3(-HelfSize, HelfSize, 0), Vector2(0.0f, 0.0f)) },
			{ VertexPositionTexture(Vector3(HelfSize, HelfSize, 0), Vector2(10, 0.0f)) },
			{ VertexPositionTexture(Vector3(-HelfSize, -HelfSize, 0), Vector2(0.0f, 10)) },
			{ VertexPositionTexture(Vector3(HelfSize, -HelfSize, 0), Vector2(10, 10)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_SquareMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//�`��I�u�W�F�N�g�̍쐬
		m_PTSpriteDraw = ObjectFactory::Create<PTSpriteDraw>(m_TextureFileName,m_Trace);
		//��`�̑傫��
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		m_Scale = Vector2(w, h);
		m_DiffuseSpriteConstantBuffer.World = Matrix4X4EX::Identity();
		//�R���X�^���g�o�b�t�@�X�V
		m_PTSpriteDraw->UpdateConstantBuffer(m_DiffuseSpriteConstantBuffer);
	}


	void WallSprite::OnDraw() {
		//�s��̒�`
		Matrix4X4 World, Proj;
		//���[���h�s��̌���
		World.AffineTransformation2D(
			m_Scale,			//�X�P�[�����O
			Vector2(0, 0),		//��]�̒��S�i�d�S�j
			0,				//��]�p�x
			m_Pos				//�ʒu
		);
		//�ˉe�s��̌���
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj.OrthographicLH(w, h, -1.0, 1.0f);
		//�s��̍���
		World *= Proj;

		//�R���X�^���g�o�b�t�@�̏���
		m_DiffuseSpriteConstantBuffer.Emissive = Color4(0, 0, 0, 0.0f);
		m_DiffuseSpriteConstantBuffer.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		m_DiffuseSpriteConstantBuffer.World = World;
		//�R���X�^���g�o�b�t�@�X�V
		m_PTSpriteDraw->UpdateConstantBuffer(m_DiffuseSpriteConstantBuffer);
		//�`��
		m_PTSpriteDraw->DrawObject(m_SquareMesh);
	}



	//--------------------------------------------------------------------------------------
	///	�l�p�`�X�v���C�g����
	//--------------------------------------------------------------------------------------
	SquareSprite::SquareSprite(const wstring& TextureFileName, bool Trace, const Vector2& StartPos) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(StartPos),
		m_TotalTime(0)
	{}
	SquareSprite::~SquareSprite() {}
	void SquareSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��
		m_BackupVertices = {
			{ VertexPositionTexture(Vector3(-HelfSize, HelfSize, 0), Vector2(0.0f, 0.0f)) },
			{ VertexPositionTexture(Vector3(HelfSize, HelfSize, 0), Vector2(4.0f, 0.0f)) },
			{ VertexPositionTexture(Vector3(-HelfSize, -HelfSize, 0), Vector2(0.0f, 1.0f)) },
			{ VertexPositionTexture(Vector3(HelfSize, -HelfSize, 0), Vector2(4.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//���b�V���̍쐬�i�ύX�ł���j
		m_SquareMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);
		//�`��I�u�W�F�N�g�̍쐬
		m_PTSpriteDraw = ObjectFactory::Create<PTSpriteDraw>(m_TextureFileName, m_Trace);
		//��`�̑傫��
		m_Scale = Vector2(512.0f, 128.0f);
		//�s��̒�`
		Matrix4X4 World, Proj;
		//���[���h�s��̌���
		World.AffineTransformation2D(
			m_Scale,			//�X�P�[�����O
			Vector2(0, 0),		//��]�̒��S�i�d�S�j
			0,				//��]�p�x
			m_Pos				//�ʒu
		);
		//�ˉe�s��̌���
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj.OrthographicLH(w, h, -1.0, 1.0f);
		//�s��̍���
		World *= Proj;
		//�R���X�^���g�o�b�t�@�̏���
		m_DiffuseSpriteConstantBuffer.Emissive = Color4(0, 0, 0, 0.0f);
		m_DiffuseSpriteConstantBuffer.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		m_DiffuseSpriteConstantBuffer.World = World;
		//�R���X�^���g�o�b�t�@�X�V
		m_PTSpriteDraw->UpdateConstantBuffer(m_DiffuseSpriteConstantBuffer);

	}

	//���_�̕ύX
	void SquareSprite::UpdateVertex(float ElapsedTime) {
		m_TotalTime += ElapsedTime;
		if (m_TotalTime > 1.0f) {
			m_TotalTime = 0;
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		//���_�̕ύX
		vector<VertexPositionTexture> new_vertices;
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			Vector2 UV = m_BackupVertices[i].textureCoordinate;
			if (UV.x == 0.0f) {
				UV.x = m_TotalTime;
			}
			else if (UV.x == 4.0f) {
				UV.x += m_TotalTime;
			}
			VertexPositionTexture v = VertexPositionTexture(
				m_BackupVertices[i].position,
				UV
			);
			new_vertices.push_back(v);
		}
		//���b�V���̒��_�̕ύX
		m_SquareMesh->UpdateVirtex(new_vertices);
	}


	void SquareSprite::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateVertex(ElapsedTime);
	}

	void SquareSprite::OnDraw() {

		//�s��̒�`
		Matrix4X4 World, Proj;
		//���[���h�s��̌���
		World.AffineTransformation2D(
			m_Scale,			//�X�P�[�����O
			Vector2(0, 0),		//��]�̒��S�i�d�S�j
			0,				//��]�p�x
			m_Pos				//�ʒu
		);
		//�ˉe�s��̌���
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj.OrthographicLH(w, h, -1.0, 1.0f);
		//�s��̍���
		World *= Proj;

		//�R���X�^���g�o�b�t�@�̏���
		m_DiffuseSpriteConstantBuffer.Emissive = Color4(0, 0, 0, 0.0f);
		m_DiffuseSpriteConstantBuffer.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		m_DiffuseSpriteConstantBuffer.World = World;
		//�R���X�^���g�o�b�t�@�X�V
		m_PTSpriteDraw->UpdateConstantBuffer(m_DiffuseSpriteConstantBuffer);
		//�`��
		m_PTSpriteDraw->DrawObject(m_SquareMesh);
	}


}
//end basecross
