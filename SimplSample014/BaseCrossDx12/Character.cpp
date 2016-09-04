/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�����̃O���[�v����
	//--------------------------------------------------------------------------------------
	CubeObjectGroup::CubeObjectGroup(const wstring& TextureFileName) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_MaxInstance(2000)
	{}
	CubeObjectGroup::~CubeObjectGroup() {}

	void CubeObjectGroup::CreateBuffers() {
		//Cube�̍쐬
		float HelfSize = 0.5f;
		vector<Vector3> PosVec = {
			{ Vector3(-HelfSize, HelfSize, -HelfSize) },
			{ Vector3(HelfSize, HelfSize, -HelfSize) },
			{ Vector3(-HelfSize, -HelfSize, -HelfSize) },
			{ Vector3(HelfSize, -HelfSize, -HelfSize) },
			{ Vector3(HelfSize, HelfSize, HelfSize) },
			{ Vector3(-HelfSize, HelfSize, HelfSize) },
			{ Vector3(HelfSize, -HelfSize, HelfSize) },
			{ Vector3(-HelfSize, -HelfSize, HelfSize) },
		};
		vector<UINT> PosIndeces = {
			0, 1, 2, 3,
			1, 4, 3, 6,
			4, 5, 6, 7,
			5, 0, 7, 2,
			5, 4, 0, 1,
			2, 3, 7, 6,
		};


		vector<Vector3> FaceNormalVec = {
			{ Vector3(0, 0, -1.0f) },
			{ Vector3(1.0f, 0, 0) },
			{ Vector3(0, 0, 1.0f) },
			{ Vector3(-1.0f, 0, 0) },
			{ Vector3(0, 1.0f, 0) },
			{ Vector3(0, -1.0f, 0) }
		};

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		UINT BasePosCount = 0;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				VertexPositionNormalTexture Data;
				Data.position = PosVec[PosIndeces[BasePosCount + j]];
				Data.normal = FaceNormalVec[i];
				switch (j) {
				case 0:
					Data.textureCoordinate = Vector2(0, 0);
					break;
				case 1:
					Data.textureCoordinate = Vector2(1.0f, 0);
					break;
				case 2:
					Data.textureCoordinate = Vector2(0, 1.0f);
					break;
				case 3:
					Data.textureCoordinate = Vector2(1.0f, 1.0f);
					break;
				}
				vertices.push_back(Data);
			}

			indices.push_back((uint16_t)BasePosCount + 0);
			indices.push_back((uint16_t)BasePosCount + 1);
			indices.push_back((uint16_t)BasePosCount + 2);
			indices.push_back((uint16_t)BasePosCount + 1);
			indices.push_back((uint16_t)BasePosCount + 3);
			indices.push_back((uint16_t)BasePosCount + 2);

			BasePosCount += 4;
		}
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_CubeMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//�C���X�^���X�s��o�b�t�@�̍쐬
		//Max�l�ō쐬����
		vector<Matrix4X4> matrices(m_MaxInstance);
		for (auto& m : matrices) {
			m = Matrix4X4();
		}
		//�C���X�^���X�`��p�̍s��̃��b�V���i���e�ύX�ł���j
		m_InstanceMatrixMesh = MeshResource::CreateMeshResource(matrices, true);
	}

	///���[�g�V�O�l�`���쐬
	void CubeObjectGroup::CreateRootSignature() {
		//���[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
	}
	///�f�X�N�v���^�q�[�v�쐬
	void CubeObjectGroup::CreateDescriptorHeap() {
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
	void CubeObjectGroup::CreateSampler() {
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
	}
	///�R���X�^���g�o�b�t�@�쐬
	void CubeObjectGroup::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(PNTStaticConstantBuffer)),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"CubeObjectGroup::CreateConstantBuffer()"
		);
		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		cbvDesc.SizeInBytes = (sizeof(PNTStaticConstantBuffer) + 255) & ~255;
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
			L"CubeObjectGroup::CreateConstantBuffer()"
		);
	}
	///�V�F�[�_�[���\�[�X�r���[�i�e�N�X�`���j�쐬
	void CubeObjectGroup::CreateShaderResourceView() {
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
	///�p�C�v���C���X�e�[�g�쐬
	void CubeObjectGroup::CreatePipelineState() {
		//�p�C�v���C���X�e�[�g�̍쐬
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		PipelineState::CreateDefault3D<VertexPositionNormalTextureMatrix, VSPNTInstance, PSPNTStatic>(m_RootSignature, PineLineDesc);
		PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
		PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		m_PipelineState = PipelineState::CreateDirect(PineLineDesc);
	}
	///�R�}���h���X�g�쐬
	void CubeObjectGroup::CreateCommandList() {
		//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	void CubeObjectGroup::UpdateConstantBuffer() {
		//�r���[�s��̌���
		Matrix4X4 View, Proj;
		View.LookAtLH(Vector3(0, 2.0, -5.0f), Vector3(0, 0, 0), Vector3(0, 1.0f, 0));
		//�]�u����
		View.Transpose();
		//�ˉe�s��̌���
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj.PerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);
		//�]�u����
		Proj.Transpose();
		//�R���X�^���g�o�b�t�@�̏���
		PNTStaticConstantBuffer sb;
		sb.World = Matrix4X4();	//���[���h�s��̓_�~�[
		sb.View = View;
		sb.Projection = Proj;
		//���C�e�B���O
		Vector4 LightDir(0.5f, -1.0f, 0.5f, 0.0f);
		LightDir.Normalize();
		sb.LightDir = LightDir;
		//�f�B�t���[�Y
		sb.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		//�G�~�b�V�u���Z�͍s��Ȃ��B
		sb.Emissive = Color4(0, 0, 0, 0);

		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&sb),
			sizeof(sb));
	}



	///�`�揈��
	void CubeObjectGroup::DrawObject() {
		CommandList::Reset(m_PipelineState, m_CommandList);

		m_CubeMesh->UpdateResources<VertexPositionNormalTexture>(m_CommandList);
		m_InstanceMatrixMesh->UpdateResources<Matrix4X4>(m_CommandList);
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
		m_CommandList->IASetIndexBuffer(&m_CubeMesh->GetIndexBufferView());
		//�C���X�^���X�`��́A���_�o�b�t�@�𕡐��o�^����
		const D3D12_VERTEX_BUFFER_VIEW Buf[2] = { 
			m_CubeMesh->GetVertexBufferView(),
			m_InstanceMatrixMesh->GetVertexBufferView() 
		};
		m_CommandList->IASetVertexBuffers(0, 2, Buf);

		m_CommandList->DrawIndexedInstanced(m_CubeMesh->GetNumIndicis(), m_CubeObjectVec.size(), 0, 0, 0);

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}




	void CubeObjectGroup::OnCreate() {
		CreateBuffers();
		//�e�N�X�`���̍쐬
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		//�C���X�^���X�z��̍쐬
		for (UINT count = 0; count < 500; count++) {
			CubeObject Data;
			Data.Refresh();
			m_CubeObjectVec.push_back(Data);
		}
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
	void CubeObjectGroup::OnUpdate() {
		if (m_CubeObjectVec.size() >= m_MaxInstance) {
			throw BaseException(
				L"�C���X�^���X����𒴂��Ă܂�",
				L"if(m_CubeObjectVec.size() >= m_MaxInstance)",
				L"CubeObjectGroup::OnUpdate()"
			);

		}

		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto& v : m_CubeObjectVec) {
			v.m_Posision += v.m_Velocity * ElapsedTime;
			Quaternion QtSpan(v.m_QuaternionRot, v.m_QuaternionVelocity * ElapsedTime);
			v.m_Quaternion *= QtSpan;
			v.m_Quaternion.Normalize();
			if (v.m_Posision.Length() >= 2.0f) {
				v.Refresh();
			}
		}
		vector<Matrix4X4> MatVec;
		for (size_t i = 0; i < m_CubeObjectVec.size(); i++) {
			Matrix4X4 World;
			//���[���h�s��̌���
			World.AffineTransformation(
				m_CubeObjectVec[i].m_Scale,			//�X�P�[�����O
				Vector3(0, 0, 0),		//��]�̒��S�i�d�S�j
				m_CubeObjectVec[i].m_Quaternion,		//��]�p�x
				m_CubeObjectVec[i].m_Posision		//�ʒu
			);
			//�]�u����
			World.Transpose();
			MatVec.push_back(World);
		}
		//���b�V���̒��_�̕ύX
		m_InstanceMatrixMesh->UpdateVirtex(MatVec);
	}
	void CubeObjectGroup::OnDraw() {
		UpdateConstantBuffer();
		DrawObject();
	}

}
//end basecross
