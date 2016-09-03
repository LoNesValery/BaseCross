/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	class NormalTextureBox  : public ObjectInterface;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	NormalTextureBox::NormalTextureBox(const Vector3& Pos) :
		ObjectInterface(),
		ShapeInterface(),
		m_Scale(1.0f, 1.0f, 1.0f),
		m_Qt(),
		m_Pos(Pos)
	{
		m_Qt.Identity();
	}
	NormalTextureBox::~NormalTextureBox() {}

	///���[�g�V�O�l�`���쐬
	void NormalTextureBox::CreateRootSignature() {
		//���[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
	}
	///�f�X�N�v���^�q�[�v�쐬
	void NormalTextureBox::CreateDescriptorHeap() {
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
	void NormalTextureBox::CreateSampler() {
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
	}
	///�R���X�^���g�o�b�t�@�쐬
	void NormalTextureBox::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(StaticConstantBuffer)),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"NormalTextureBox::CreateConstantBuffer()"
		);
		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		cbvDesc.SizeInBytes = (sizeof(StaticConstantBuffer) + 255) & ~255;
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
			L"NormalTextureBox::CreateConstantBuffer()"
		);
	}
	///�V�F�[�_�[���\�[�X�r���[�i�e�N�X�`���j�쐬
	void NormalTextureBox::CreateShaderResourceView() {
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
		srvDesc.Format = m_WallTex->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = m_WallTex->GetTextureResDesc().MipLevels;
		//�V�F�[�_���\�[�X�r���[
		Dev->GetDevice()->CreateShaderResourceView(
			m_WallTex->GetTexture().Get(),
			&srvDesc,
			Handle);
	}
	///�p�C�v���C���X�e�[�g�쐬
	void NormalTextureBox::CreatePipelineState() {
		//�p�C�v���C���X�e�[�g�̍쐬
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>(m_RootSignature, PineLineDesc);
		PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
		PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		m_PipelineState = PipelineState::CreateDirect(PineLineDesc);
	}
	///�R�}���h���X�g�쐬
	void NormalTextureBox::CreateCommandList() {
		//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	void NormalTextureBox::UpdateConstantBuffer() {
		//�s��̒�`
		//���[���h�s��̌���
		m_ConstantBufferData.World.AffineTransformation(
			m_Scale,			//�X�P�[�����O
			Vector3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		//�]�u����
		m_ConstantBufferData.World.Transpose();
		//�r���[�s��̌���
		m_ConstantBufferData.View.LookAtLH(Vector3(0, 2.0, -5.0f), Vector3(0, 0, 0), Vector3(0, 1.0f, 0));
		//�]�u����
		m_ConstantBufferData.View.Transpose();
		//�ˉe�s��̌���
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		m_ConstantBufferData.Projection.PerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);
		//�]�u����
		m_ConstantBufferData.Projection.Transpose();
		//���C�e�B���O
		m_ConstantBufferData.LightDir = Vector4(0.5f, -1.0f, 0.5f, 1.0f);
		//�G�~�b�V�u
		m_ConstantBufferData.Emissive = Color4(0, 0, 0, 0);
		//�f�B�t���[�Y
		m_ConstantBufferData.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_ConstantBufferData),
			sizeof(m_ConstantBufferData));
	}


	///�`�揈��
	void NormalTextureBox::DrawObject() {
		CommandList::Reset(m_PipelineState, m_CommandList);
		m_CubeMesh->UpdateResources<VertexPositionNormalTexture>(m_CommandList);
		m_WallTex->UpdateResources(m_CommandList);

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
		m_CommandList->IASetIndexBuffer(&m_CubeMesh->GetIndexBufferView());
		m_CommandList->IASetVertexBuffers(0, 1, &m_CubeMesh->GetVertexBufferView());
		m_CommandList->DrawIndexedInstanced(m_CubeMesh->GetNumIndicis(), 1, 0, 0, 0);

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}



	void NormalTextureBox::OnCreate() {
		m_CubeMesh = MeshResource::CreateCube(1.0f);
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"wall.jpg";
		m_WallTex = TextureResource::CreateTextureResource(strTexture);
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
		//�R���X�^���g�o�b�t�@�̍X�V
		UpdateConstantBuffer();
	}
	void NormalTextureBox::OnUpdate() {
		Quaternion QtSpan;
		QtSpan.RotationAxis(Vector3(0, 1.0f, 0), 0.02f);
		m_Qt *= QtSpan;
		m_Qt.Normalize();
	}



	void NormalTextureBox::OnDraw() {
		UpdateConstantBuffer();
		DrawObject();

	}


}
//end basecross
