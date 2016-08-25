/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�����̎���
	//--------------------------------------------------------------------------------------
	CubeObject::CubeObject() :
		ObjectInterface(),
		ShapeInterface()
	{}
	CubeObject::~CubeObject() {}

	void CubeObject::CreateBuffers() {
		float HelfSize = 0.5f;
		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(Vector3(-HelfSize, HelfSize, -HelfSize), Color4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, HelfSize, -HelfSize), Color4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(-HelfSize, -HelfSize, -HelfSize), Color4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, -HelfSize, -HelfSize), Color4(1.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, HelfSize, HelfSize), Color4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(-HelfSize, HelfSize, HelfSize), Color4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, -HelfSize, HelfSize), Color4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(-HelfSize, -HelfSize, HelfSize), Color4(1.0f, 0.0f, 1.0f, 1.0f)) }
		};
		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2,
			1, 4, 3,
			4, 6, 3,
			4, 5, 6,
			5, 7, 6,
			5, 0, 7,
			0, 2, 7,
			5, 4, 0,
			4, 1, 0,
			6, 7, 3,
			7, 2, 3
		};
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_CubeMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}

	void CubeObject::UpdateConstantBuffer() {
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

		m_StaticConstantBuffer.World = World;
		m_StaticConstantBuffer.View = View;
		m_StaticConstantBuffer.Projection = Proj;
		m_StaticConstantBuffer.Emissive = Color4(0, 0, 0, 0);
		//�����l�X�V
		m_DrawContext->UpdateConstantBuffer(reinterpret_cast<void**>(&m_StaticConstantBuffer), sizeof(m_StaticConstantBuffer));
	}



	void CubeObject::OnCreate() {
		CreateBuffers();
		m_Scale = Vector3(1.0f, 1.0f, 1.0f);
		m_Qt.Identity();
		m_Pos = Vector3(0, 0, 0.0);
		//�`��R���e�L�X�g
		m_DrawContext = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateSrvSmpCbv);
		m_DrawContext->CreateConstantBuffer(sizeof(m_StaticConstantBuffer));
		m_DrawContext->CreateDefault3DPipelineCmdList<VertexPositionColorTexture, VSPCStatic, PSPCStatic>();
		//�R���X�^���g�o�b�t�@�̍X�V
		UpdateConstantBuffer();
	}
	void CubeObject::OnUpdate() {
		Quaternion QtSpan;
		QtSpan.RotationAxis(Vector3(0, 1.0f, 0), 0.02f);
		m_Qt *= QtSpan;
		m_Qt.Normalize();
	}
	void CubeObject::OnDraw() {
		//�R���X�^���g�o�b�t�@�̍X�V
		UpdateConstantBuffer();
		//�`��
		m_DrawContext->DrawIndexed<VertexPositionColor>(m_CubeMesh);
	}



}
//end basecross
