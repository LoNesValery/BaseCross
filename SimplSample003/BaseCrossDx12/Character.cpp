/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	�l�p�`�X�v���C�g����
	//--------------------------------------------------------------------------------------
	SquareSprite::SquareSprite() :
		ObjectInterface(),
		ShapeInterface(),
		m_Scale(128.0f, 128.0f),
		m_Rot(0),
		m_Pos(0, 0),
		m_PosSpan(3.0f, 0)
	{}
	SquareSprite::~SquareSprite() {}
	void SquareSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��
		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(Vector3(-HelfSize, HelfSize, 0), Color4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, HelfSize, 0), Color4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(-HelfSize, -HelfSize, 0), Color4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, -HelfSize, 0), Color4(1.0f, 0.0f, 1.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//���b�V���̍쐬
		m_SquareMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		m_DrawContext = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateCbv);
		m_DrawContext->CreateConstantBuffer(sizeof(m_SpriteConstantBuffer));
		m_DrawContext->CreateDefault2DPipelineCmdList<VertexPositionColor, VSPCSprite, PSPCSprite>();

	}

	void SquareSprite::OnUpdate() {
		if (m_PosSpan.x > 0) {
			m_Rot -= 0.05f;
		}
		else {
			m_Rot += 0.05f;
		}
		m_Pos += m_PosSpan;
		if (abs(m_Pos.x) > 400.0f) {
			m_PosSpan *= -1.0f;
		}
	}

	void SquareSprite::OnDraw() {
		//�s��̒�`
		Matrix4X4 World, Proj;
		//���[���h�s��̌���
		World.AffineTransformation2D(
			m_Scale,			//�X�P�[�����O
			Vector2(0, 0),		//��]�̒��S�i�d�S�j
			m_Rot,				//��]�p�x
			m_Pos				//�ʒu
		);
		//�ˉe�s��̌���
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj.OrthographicLH(w, h, -1.0, 1.0f);
		//�s��̍���
		World *= Proj;
		//�R���X�^���g�o�b�t�@�̏���
		m_SpriteConstantBuffer.Emissive = Color4(0.0f, 0.0f, 0, 1.0f);
		m_SpriteConstantBuffer.World = World;
		//�X�V
		m_DrawContext->UpdateConstantBuffer(&m_SpriteConstantBuffer, sizeof(m_SpriteConstantBuffer));
		//�`��
		m_DrawContext->DrawIndexed<VertexPositionColor>(m_SquareMesh);
	}


}
//end basecross
