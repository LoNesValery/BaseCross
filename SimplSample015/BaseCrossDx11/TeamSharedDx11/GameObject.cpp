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
	void PNTStaticDraw::OnDraw(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

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
		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBPNTStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBPNTStatic::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//���_�V�F�[�_�ɓn��
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�s�N�Z���V�F�[�_�ɓn��
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());

		//�u�����h�X�e�[�g
		if (m_Trace){
			//��������
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		}
		else{
			//�����������Ȃ�
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		}

		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);

		//�e�N�X�`���ƃT���v���[�̐ݒ�
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureResource->GetShaderResourceView().GetAddressOf());
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		if (m_Trace){
			//���������̏ꍇ�́A���X�^���C�U�X�e�[�g��ύX����2��`��
			//���X�^���C�U�X�e�[�g�i���ʕ`��j
			pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
			//�`��
			pD3D11DeviceContext->DrawIndexed(m_NumIndicis, 0, 0);
			//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
			pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
			//�`��
			pD3D11DeviceContext->DrawIndexed(m_NumIndicis, 0, 0);
		}
		else{
			//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
			pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
			//�`��
			pD3D11DeviceContext->DrawIndexed(m_NumIndicis, 0, 0);
		}
		//��n��
		Dev->InitializeStates();

	}



}
//end basecross
