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
		m_Scale(1.0f, 1.0f, 1.0f),
		m_Qt(),
		m_Pos(Pos)
	{
		m_Qt.Identity();
	}
	NormalTextureBox::~NormalTextureBox() {}

	void NormalTextureBox::OnCreate() {
		m_CubeMesh = MeshResource::CreateCube(1.0f);
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"wall.jpg";
		m_WallTex = TextureResource::CreateTextureResource(strTexture);
	}
	void NormalTextureBox::OnUpdate() {
		Quaternion QtSpan;
		QtSpan.RotationAxis(Vector3(0, 1.0f, 0), 0.02f);
		m_Qt *= QtSpan;
		m_Qt.Normalize();
	}
	void NormalTextureBox::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//�s��̒�`
		Matrix4X4 World, View, Proj;
		//���[���h�s��̌���
		World.AffineTransformation(
			m_Scale,			//�X�P�[�����O
			Vector3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		//�]�u����
		World.Transpose();
		//�r���[�s��̌���
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
		sb.World = World;
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
		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBPNTStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_CubeMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(m_CubeMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

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
		//�����������Ȃ�
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);

		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);

		//�e�N�X�`���ƃT���v���[�̐ݒ�
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, m_WallTex->GetShaderResourceView().GetAddressOf());
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//�`��
		pD3D11DeviceContext->DrawIndexed(m_CubeMesh->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates();

	}


}
//end basecross
