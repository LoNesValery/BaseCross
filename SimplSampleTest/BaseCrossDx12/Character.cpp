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

	void NormalTextureBox::OnCreate() {
		m_CubeMesh = MeshResource::CreateCube(1.0f);
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"wall.jpg";
		m_WallTex = TextureResource::CreateTextureResource(strTexture);
		m_DrawContext = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateSrvSmpCbv);
		m_DrawContext->CreateConstantBuffer(sizeof(m_ConstantBufferData));
		m_DrawContext->CreateDefault3DPipelineCmdList<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>();
		ZeroMemory(&m_ConstantBufferData, sizeof(m_ConstantBufferData));
		//�e�s���Identity�ɏ�����
		m_ConstantBufferData.World = Matrix4X4EX::Identity();
		m_ConstantBufferData.View = Matrix4X4EX::Identity();
		m_ConstantBufferData.Projection = Matrix4X4EX::Identity();
		//�����l�X�V
		m_DrawContext->UpdateConstantBuffer(reinterpret_cast<void**>(&m_ConstantBufferData), sizeof(m_ConstantBufferData));
		//�e�N�X�`���ݒ�
		m_DrawContext->SetTextureResource(m_WallTex);
	}
	void NormalTextureBox::OnUpdate() {
		Quaternion QtSpan;
		QtSpan.RotationAxis(Vector3(0, 1.0f, 0), 0.02f);
		m_Qt *= QtSpan;
		m_Qt.Normalize();
	}
	void NormalTextureBox::OnDraw() {
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
		m_DrawContext->UpdateConstantBuffer(&m_ConstantBufferData, sizeof(m_ConstantBufferData));
		//�`��
		m_DrawContext->DrawIndexed<VertexPositionNormalTexture>(m_CubeMesh);

	}


}
//end basecross
