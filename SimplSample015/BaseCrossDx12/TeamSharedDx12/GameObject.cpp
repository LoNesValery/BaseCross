/*!
@file GameObject.cpp
@brief �R���|�[�l���g�A�Q�[���I�u�W�F�N�g����
*/

#include "stdafx.h"
#include "../Project.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	�Q�[���I�u�W�F�N�g�N���X
	//--------------------------------------------------------------------------------------
	shared_ptr<Component> GameObject::SearchComponent(type_index TypeIndex)const{
		auto it = m_CompMap.find(TypeIndex);
		if (it != m_CompMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	void GameObject::AddMakedComponent(type_index TypeIndex, const shared_ptr<Component>& Ptr){
		if (!SearchComponent(TypeIndex)){
			//���̃R���|�[�l���g���܂��Ȃ���ΐV�K�o�^
			m_CompOrder.push_back(TypeIndex);
		}
		//map�ɒǉ��������͍X�V
		m_CompMap[TypeIndex] = Ptr;
		Ptr->SetGameObject(GetThis<GameObject>());
	}

	GameObject::GameObject(const shared_ptr<Scene>& ScenePtr) :
		m_Scene(ScenePtr)
	{}

	GameObject::~GameObject(){}

	void GameObject::ComponentPreparation(){
		auto TMptr = GetComponent<Transform>();
		TMptr->SetToBefore();
	}


	void GameObject::ComponentUpdate(){
		auto TMptr = GetComponent<Transform>();
		auto GravityPtr = GetComponent<Gravity>(false);
		//�}�b�v�����؂���Update
		auto it = m_CompOrder.begin();
		while (it != m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = m_CompMap.find(*it);
			if (it2 != m_CompMap.end()) {
				//�w��̌^�̃R���|�[�l���g����������
				it2->second->OnUpdate();
			}
			it++;
		}
		if (GravityPtr) {
			//GravityPtr�������Update()
			GravityPtr->OnUpdate();
		}
		//Transform��Update
		TMptr->OnUpdate();
	}

	void GameObject::ComponentDraw(){
		//Gravity��Transform��Draw���Ă΂Ȃ�
		//�}�b�v�����؂���Update
		auto it = m_CompOrder.begin();
		while (it != m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = m_CompMap.find(*it);
			if (it2 != m_CompMap.end()) {
				//�w��̌^�̃R���|�[�l���g����������
				it2->second->OnDraw();
			}
			it++;
		}

	}




	//--------------------------------------------------------------------------------------
	///	�R���|�[�l���g�e�N���X
	//--------------------------------------------------------------------------------------
	Component::Component(const shared_ptr<GameObject>& GameObjectPtr) :
		m_GameObject(GameObjectPtr)
	{}

	Component::~Component(){}

	shared_ptr<GameObject> Component::GetGameObject() const{
		if (m_GameObject.expired()){
			throw BaseException(
				L"GameObject�͗L���ł͂���܂���",
				L"if (pImpl->m_GameObject.expired())",
				L"Component::GetGameObject()"
				);
		}
		else{
			return m_GameObject.lock();
		}
	}

	void Component::SetGameObject(const shared_ptr<GameObject>& GameObjectPtr){
		m_GameObject = GameObjectPtr;
	}

	//--------------------------------------------------------------------------------------
	///	�d�̓R���|�[�l���g
	//--------------------------------------------------------------------------------------
	void Gravity::StartJump(const Vector3& StartVec, float EscapeSpan){
		auto Position = GetGameObject()->GetComponent<Transform>()->GetPosition();
		SetGravityVelocity(StartVec);
		SetGravityDefault();
		//�W�����v���Đe�I�u�W�F�N�g�{�����[������E�o�ł��Ȃ��Ƃ��Ή�
		Vector3 EscapeVec = StartVec;
		EscapeVec *= EscapeSpan;
		Position += EscapeVec;
		//�ʒu��ݒ�
		GetGameObject()->GetComponent<Transform>()->SetPosition(Position);
	}

	void Gravity::UpdateFromTime(float CalcTime){
		auto Position = GetGameObject()->GetComponent<Transform>()->GetPosition();
		//���R���������x���v�Z
		m_GravityVelocity += m_Gravity * CalcTime;
		Position += m_GravityVelocity * CalcTime;
		if (Position.y <= m_BaseY){
			Position.y = m_BaseY;
			SetGravityVelocityZero();
			SetGravityZero();
		}
		//�ʒu��ݒ�
		GetGameObject()->GetComponent<Transform>()->SetPosition(Position);
	}

	void Gravity::OnUpdate(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateFromTime(ElapsedTime);
	}

	//--------------------------------------------------------------------------------------
	///	PNTStaticDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
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
			L"PNTStaticDraw::CreateConstantBuffer()"
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
			L"PNTStaticDraw::CreateConstantBuffer()"
		);
	}
	///�V�F�[�_�[���\�[�X�r���[�i�e�N�X�`���j�쐬
	void PNTStaticDraw::CreateShaderResourceView() {
		auto ShPtr = m_TextureResource.lock();
		if (!ShPtr) {
			return;
		}
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
		srvDesc.Format = ShPtr->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = ShPtr->GetTextureResDesc().MipLevels;
		//�V�F�[�_���\�[�X�r���[
		Dev->GetDevice()->CreateShaderResourceView(
			ShPtr->GetTexture().Get(),
			&srvDesc,
			Handle);
	}
	///�p�C�v���C���X�e�[�g�쐬
	void PNTStaticDraw::CreatePipelineState() {
		//�p�C�v���C���X�e�[�g�̍쐬
		PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>(m_RootSignature, m_PineLineDesc);
		m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
		m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		m_CullFrontPipelineState = PipelineState::CreateDirect(m_PineLineDesc);
		m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		m_CullBackPipelineState = PipelineState::CreateDirect(m_PineLineDesc);
	}

	void PNTStaticDraw::RefreshTrace() {
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
			m_PineLineDesc.BlendState = blend_desc;
		}
		else {
			m_PineLineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		}
		m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		m_CullFrontPipelineState = PipelineState::CreateDirect(m_PineLineDesc);

		m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		m_CullBackPipelineState = PipelineState::CreateDirect(m_PineLineDesc);

	}


	///�R�}���h���X�g�쐬
	void PNTStaticDraw::CreateCommandList() {
		//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
		m_CommandList = CommandList::CreateDefault(m_CullBackPipelineState);
		CommandList::Close(m_CommandList);
	}

	void PNTStaticDraw::UpdateConstantBuffer(PNTStaticConstantBuffer& Buff) {
		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&Buff),
			sizeof(Buff));
	}
	void PNTStaticDraw::DrawObject() {
		auto ShMesh = m_MeshResource.lock();
		auto ShTex = m_TextureResource.lock();
		if (!ShMesh || !ShTex) {
			return;
		}

		//�R�}���h���X�g�̃��Z�b�g
		if (m_Trace) {
			CommandList::Reset(m_CullFrontPipelineState, m_CommandList);
		}
		else {
			CommandList::Reset(m_CullBackPipelineState, m_CommandList);
		}

		ShMesh->UpdateResources<VertexPositionNormalTexture>(m_CommandList);
		ShTex->UpdateResources(m_CommandList);

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
		m_CommandList->IASetIndexBuffer(&ShMesh->GetIndexBufferView());
		m_CommandList->IASetVertexBuffers(0, 1, &ShMesh->GetVertexBufferView());
		m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
		if (m_Trace) {
			m_CommandList->SetPipelineState(m_CullBackPipelineState.Get());
			m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
		}

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}




	void PNTStaticDraw::OnCreate() {
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

		//�e�s���Identity�ɏ�����
		PNTStaticConstantBuffer sb;
		sb.World = Matrix4X4EX::Identity();
		sb.View = Matrix4X4EX::Identity();
		sb.Projection = Matrix4X4EX::Identity();
		sb.Emissive = Color4(0.0f, 0.0f, 0.0f, 0.0f);
		sb.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);

		//�R���X�^���g�o�b�t�@�̍X�V
		UpdateConstantBuffer(sb);
	}

	void PNTStaticDraw::OnDraw(){
		auto GObject = GetGameObject();

		//�s��̒�`
		Matrix4X4 World, View, Proj;
		//���C�e�B���O
		Vector4 LightDir;
		auto PtrTrans = GObject->GetComponent<Transform>();
		World = PtrTrans->GetWorldMatrix();
		//�]�u����
		World.Transpose();
		GObject->GetScene()->GetViewProjMatrix(View, Proj);
		GObject->GetScene()->GetLightDir(LightDir);
		//�r���[�s��̌���
		//�]�u����
		View.Transpose();
		//�ˉe�s��̌���
		//�]�u����
		Proj.Transpose();
		//�R���X�^���g�o�b�t�@�̏���
		PNTStaticConstantBuffer sb;
		sb.World = World;
		sb.View = View;
		sb.Projection = Proj;
		sb.LightDir = LightDir;
		//�f�B�t���[�Y
		sb.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		//�G�~�b�V�u���Z�͍s��Ȃ��B
		sb.Emissive = Color4(0, 0, 0, 0);

		UpdateConstantBuffer(sb);
		DrawObject();

	}

	shared_ptr<MeshResource> PNTStaticDraw::GetMeshResource() const {
		//���b�V�����Ȃ���΃��^�[��
		auto shptr = m_MeshResource.lock();
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
		m_MeshResource = MeshRes;
	}
	void PNTStaticDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		m_TextureResource = TextureRes;
		CreateShaderResourceView();
	}
	shared_ptr<TextureResource> PNTStaticDraw::GetTextureResource() const {
		//�e�N�X�`�����Ȃ����null��Ԃ�
		auto shptr = m_TextureResource.lock();
		if (!shptr) {
			return nullptr;
		}
		return m_TextureResource.lock();
	}

	void PNTStaticDraw::SetTrace(bool b) {
		if (b != m_Trace) {
			m_Trace = b;
			RefreshTrace();
		}
	}





}
//end basecross
