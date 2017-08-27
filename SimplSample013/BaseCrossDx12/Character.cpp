/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	PNT�X�^�e�B�b�N�`��iPNT���_�̃v���~�e�B�u�`��p�j
	//--------------------------------------------------------------------------------------
	PNTStaticDraw::PNTStaticDraw(const wstring& TextureFileName, bool Trace) :
		m_TextureFileName(TextureFileName),
		m_Trace(Trace)
	{}

	void PNTStaticDraw::OnCreate() {
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
	void PNTStaticDraw::CreateRootSignature() {
		//���[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
	}
	///�f�X�N�v���^�q�[�v�쐬
	void PNTStaticDraw::CreateDescriptorHeap() {
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
	void PNTStaticDraw::CreateSampler() {
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
	}
	///�R���X�^���g�o�b�t�@�쐬
	void PNTStaticDraw::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		UINT ConstBuffSize = (sizeof(PNTStaticConstantBuffer) + 255) & ~255;
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
			L"PNTStaticDraw::CreateConstantBuffer()"
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
			L"PNTStaticDraw::CreateConstantBuffer()"
		);
	}
	///�V�F�[�_�[���\�[�X�r���[�i�e�N�X�`���j�쐬
	void PNTStaticDraw::CreateShaderResourceView() {
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
	void PNTStaticDraw::CreatePipelineState() {
		//�p�C�v���C���X�e�[�g�̍쐬
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>(m_RootSignature, PineLineDesc);
		//�u�����h�X�e�[�g�ƃ��X�^���C�U�����ւ�
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
		}

		PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
		PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;

		m_CullFrontPipelineState = PipelineState::CreateDirect(PineLineDesc);

		PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		m_CullBackPipelineState = PipelineState::CreateDirect(PineLineDesc);
	}
	///�R�}���h���X�g�쐬
	void PNTStaticDraw::CreateCommandList() {
		//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
		m_CommandList = CommandList::CreateDefault(m_CullBackPipelineState);
		CommandList::Close(m_CommandList);
	}

	void PNTStaticDraw::UpdateConstantBuffer(PNTStaticConstantBuffer & Data) {
		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&Data),
			sizeof(Data));
	}


	///�`�揈��
	void PNTStaticDraw::DrawObject(const shared_ptr<MeshResource>& Mesh) {
		//�R�}���h���X�g�̃��Z�b�g
		if (m_Trace) {
			CommandList::Reset(m_CullFrontPipelineState, m_CommandList);
		}
		else {
			CommandList::Reset(m_CullBackPipelineState, m_CommandList);
		}

		Mesh->UpdateResources<VertexPositionNormalTexture>(m_CommandList);
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
		if (m_Trace) {
			m_CommandList->SetPipelineState(m_CullBackPipelineState.Get());
			m_CommandList->DrawIndexedInstanced(Mesh->GetNumIndicis(), 1, 0, 0, 0);
		}

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}


	//--------------------------------------------------------------------------------------
	///	���ʎ���
	//--------------------------------------------------------------------------------------
	SquareObject::SquareObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos) :
		m_Scene(PtrScene),
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	SquareObject::~SquareObject() {}

	void SquareObject::CreateBuffers() {
		float HelfSize = 0.5f;
		vector<VertexPositionNormalTexture> vertices = {
			{ VertexPositionNormalTexture(Vec3(-HelfSize, HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vec3(HelfSize, HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vec3(-HelfSize, -HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionNormalTexture(Vec3(HelfSize, -HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(1.0f, 1.0f)) },

		};

		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2,
		};
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_SquareMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}


	void SquareObject::OnCreate() {
		CreateBuffers();
		m_PNTStaticDraw = ObjectFactory::Create<PNTStaticDraw>(m_TextureFileName, false);
	}
	void SquareObject::OnUpdate() {
	}
	void SquareObject::OnDraw() {
		//�s��̒�`
		Mat4x4 World, View, Proj;
		//���C�e�B���O
		Vec4 LightDir;
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//���[���h�s��̌���
		World.affineTransformation(
			m_Scale,			//�X�P�[�����O
			Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		//�]�u����
		World.transpose();
		ShPtrScene->GetViewProjMatrix(View, Proj);
		ShPtrScene->GetLightDir(LightDir);
		//�r���[�s��̌���
		//�]�u����
		View.transpose();
		//�ˉe�s��̌���
		//�]�u����
		Proj.transpose();
		//�R���X�^���g�o�b�t�@�̏���
		PNTStaticConstantBuffer sb;
		sb.World = World;
		sb.View = View;
		sb.Projection = Proj;
		//���C�e�B���O
		sb.LightDir = LightDir;
		//�f�B�t���[�Y
		sb.Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		//�G�~�b�V�u���Z�B
		sb.Emissive = Col4(0.4f, 0.4f, 0.4f, 0);
		m_PNTStaticDraw->UpdateConstantBuffer(sb);
		m_PNTStaticDraw->DrawObject(m_SquareMesh);
	}



	//--------------------------------------------------------------------------------------
	///	������
	//--------------------------------------------------------------------------------------
	SphereObject::SphereObject(const shared_ptr<Scene> PtrScene,
		UINT Division, const wstring& TextureFileName, bool Trace, const Vec3& Pos) :
		m_Scene(PtrScene),
		ObjectInterface(),
		ShapeInterface(),
		m_Division(Division),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(Pos)
	{}
	SphereObject::~SphereObject() {}

	void SphereObject::CreateBuffers() {
		float Radius = 0.5f;
		//�ܓx����
		UINT Vertical = m_Division;
		//�o�x����
		UINT Horizontal = m_Division * 2;

		vector<VertexPositionNormalTexture> vertices;
		for (UINT i = 0; i <= Vertical; i++)
		{
			float v = 1 - (float)i / Vertical;

			float Latitude = (i * XM_PI / Vertical) - XM_PIDIV2;
			float Dy = sin(Latitude);
			float Dxz = cos(Latitude);

			for (UINT j = 0; j <= Horizontal; j++)
			{
				float u = 1 - (float)j / Horizontal;

				float Longitude = j * XM_2PI / Horizontal;
				float Dx = sin(Longitude) * Dxz;
				float Dz = cos(Longitude) * Dxz;
				Vec3 normal(Dx, Dy, Dz);
				VertexPositionNormalTexture Data;
				Data.position = normal * Radius;
				Data.normal = normal;
				Data.normal.normalize();
				Data.textureCoordinate = Vec2(u, v);
				vertices.push_back(Data);
			}
		}
		UINT Stride = Horizontal + 1;
		vector<uint16_t> indices;
		for (UINT i = 0; i < Vertical; i++)
		{
			for (UINT j = 0; j <= Horizontal; j++)
			{
				UINT NextI = i + 1;
				UINT NextJ = (j + 1) % Stride;
				indices.push_back((uint16_t)(i * Stride + NextJ));
				indices.push_back((uint16_t)(NextI * Stride + j));
				indices.push_back((uint16_t)(i * Stride + j));

				indices.push_back((uint16_t)(NextI * Stride + NextJ));
				indices.push_back((uint16_t)(NextI * Stride + j));
				indices.push_back((uint16_t)(i * Stride + NextJ));
			}
		}
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}


	void SphereObject::OnCreate() {
		CreateBuffers();
		m_Scale = Vec3(1.0f, 1.0f, 1.0f);
		m_Qt.identity();
		m_PNTStaticDraw = ObjectFactory::Create<PNTStaticDraw>(m_TextureFileName, m_Trace);
	}
	void SphereObject::OnUpdate() {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		//�L�[�{�[�h�ƃ}�E�X�̎擾
		auto Key = App::GetApp()->GetInputDevice().GetKeyState();
		//�ʒu���̑ޔ�
		Vec3 TempPos = m_Pos;
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0) {
				m_Pos.x += (CntlVec[0].fThumbLX * ElapsedTime * 5.0f);
			}
			if (CntlVec[0].fThumbLY != 0) {
				m_Pos.z += (CntlVec[0].fThumbLY * ElapsedTime * 5.0f);
			}
		}
		if (Key.m_bPushKeyTbl['F'] || Key.m_bPushKeyTbl[VK_LBUTTON]) {
			m_Pos.x -= ElapsedTime * 5.0f;
		}
		else if (Key.m_bPushKeyTbl['G'] || Key.m_bPushKeyTbl[VK_RBUTTON]) {
			m_Pos.x += ElapsedTime * 5.0f;
		}
		if (Key.m_bPushKeyTbl['T']) {
			m_Pos.z += ElapsedTime * 5.0f;
		}
		else if (Key.m_bPushKeyTbl['V']) {
			m_Pos.z -= ElapsedTime * 5.0f;
		}
		TempPos = m_Pos - TempPos;
		if (TempPos.length() > 0) {
			//�ړ�����
			TempPos.normalize();
			float Angle = atan2(TempPos.x, TempPos.z);
			m_Qt.rotation(Angle, Vec3(0, 1.0f, 0));
			m_Qt.normalize();
		}
	}
	void SphereObject::OnDraw() {
		//�s��̒�`
		Mat4x4 World, View, Proj;
		//���C�e�B���O
		Vec4 LightDir;
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//���[���h�s��̌���
		World.affineTransformation(
			m_Scale,			//�X�P�[�����O
			Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		//�]�u����
		World.transpose();
		ShPtrScene->GetViewProjMatrix(View, Proj);
		ShPtrScene->GetLightDir(LightDir);
		//�r���[�s��̌���
		//�]�u����
		View.transpose();
		//�ˉe�s��̌���
		//�]�u����
		Proj.transpose();
		//�R���X�^���g�o�b�t�@�̏���
		PNTStaticConstantBuffer sb;
		sb.World = World;
		sb.View = View;
		sb.Projection = Proj;
		sb.LightDir = LightDir;
		//�f�B�t���[�Y
		sb.Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		//�G�~�b�V�u���Z�B
		sb.Emissive = Col4(0.4f, 0.4f, 0.4f, 0);
		m_PNTStaticDraw->UpdateConstantBuffer(sb);
		m_PNTStaticDraw->DrawObject(m_SphereMesh);
	}




}
//end basecross
