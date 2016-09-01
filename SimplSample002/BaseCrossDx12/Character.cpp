/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O�p�`�X�v���C�g
	//--------------------------------------------------------------------------------------
	TriangleSprite::TriangleSprite() :
		ObjectInterface(),
		ShapeInterface(),
		m_Pos(0, 0, 0),
		m_MoveSpan(0.01f, 0, 0)
	{
	}
	TriangleSprite::~TriangleSprite() {}

	///���[�g�V�O�l�`���쐬
	void TriangleSprite::CreateRootSignature() {
		//�R���X�^���g�o�b�t�@�t���[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateCbv();
	}

	///�f�X�N�v���^�q�[�v�쐬
	void TriangleSprite::CreateDescriptorHeap() {
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CbvSrvDescriptorHandleIncrementSize
			= Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CbvSrv�f�X�N�v���^�q�[�v(�R���X�^���g�o�b�t�@�̂�)
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1);
		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}

	///�R���X�^���g�o�b�t�@�쐬
	void TriangleSprite::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(SpriteConstantBuffer)),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"TriangleSprite::CreateConstantBuffer()"
		);
		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		cbvDesc.SizeInBytes = (sizeof(SpriteConstantBuffer) + 255) & ~255;
		//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
		//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
		//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"TriangleSprite::CreateConstantBuffer()"
		);
	}

	///�p�C�v���C���X�e�[�g�쐬
	void TriangleSprite::CreatePipelineState() {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		m_PipelineState 
		= PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(m_RootSignature, PineLineDesc);
	}
	///�R�}���h���X�g�쐬
	void TriangleSprite::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	void TriangleSprite::OnCreate() {
		//���_���쐬���邽�߂̔z��
		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(Vector3(0.0f, 0.5f, 0.0f), Color4(1.0f,0.0f,0.0f,1.0f)) },
			{ VertexPositionColor(Vector3(0.5f, -0.5f, 0.0f), Color4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(-0.5f, -0.5f, 0.0f), Color4(0.0f, 0.0f, 1.0f, 1.0f)) },
		};
		m_TriangleMesh = MeshResource::CreateMeshResource(vertices, false);

		///���[�g�V�O�l�`���쐬
		CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		CreateDescriptorHeap();
		///�R���X�^���g�o�b�t�@�쐬
		CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		CreatePipelineState();
		///�R�}���h���X�g�쐬
		CreateCommandList();
		//�R���X�^���g�o�b�t�@�̍X�V
		UpdateConstantBuffer();
	}

	void TriangleSprite::UpdateConstantBuffer() {
		//�R���X�^���g�o�b�t�@�̏���
		m_SpriteConstantBuffer.Emissive = Color4(0.0f, 0.0f, 0, 1.0f);
		Matrix4X4 mat;
		mat.TranslationFromVector(m_Pos);
		m_SpriteConstantBuffer.World = mat;
		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_SpriteConstantBuffer),
			sizeof(m_SpriteConstantBuffer));

	}

	void TriangleSprite::OnUpdate() {
		m_Pos += m_MoveSpan;
		if (abs(m_Pos.x) >= 0.5f) {
			m_MoveSpan *= -1.0f;
		}
	}

	///�`�揈��
	void TriangleSprite::DrawObject() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R�}���h���X�g�̃��Z�b�g
		CommandList::Reset(m_PipelineState, m_CommandList);
		//���b�V�����X�V����Ă���΃��\�[�X�X�V
		m_TriangleMesh->UpdateResources<VertexPositionColor>(m_CommandList);
		//���[�g�V�O�l�`���̃Z�b�g
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		//�f�X�N�v���^�q�[�v�̃Z�b�g
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		//GPU�f�X�N�v���^�q�[�v�n���h���̃Z�b�g
		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}

		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

		//�����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = Dev->GetRtvHandle();
		//�f�v�X�X�e���V���r���[�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = Dev->GetDsvHandle();
		//�擾�����n���h�����Z�b�g
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetVertexBuffers(0, 1, &m_TriangleMesh->GetVertexBufferView());
		m_CommandList->DrawInstanced(m_TriangleMesh->GetNumVertices(), 1, 0, 0);

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());


	}

	void TriangleSprite::OnDraw() {
		UpdateConstantBuffer();
		//�`��
		DrawObject();
	}



}
//end basecross
