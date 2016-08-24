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
	SquareSprite::SquareSprite(const wstring& TextureFileName, bool Trace, const Vector2& StartPos) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(StartPos)
	{}
	SquareSprite::~SquareSprite() {}
	void SquareSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��
		vector<VertexPositionTexture> vertices = {
			{ VertexPositionTexture(Vector3(-HelfSize, HelfSize, 0), Vector2(0.0f, 0.0f)) },
			{ VertexPositionTexture(Vector3(HelfSize, HelfSize, 0), Vector2(1.0f, 0.0f)) },
			{ VertexPositionTexture(Vector3(-HelfSize, -HelfSize, 0), Vector2(0.0f, 1.0f)) },
			{ VertexPositionTexture(Vector3(HelfSize, -HelfSize, 0), Vector2(1.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };

		//���b�V���̍쐬
		m_SquareMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//�e�N�X�`���̍쐬
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");

		//��`�̑傫��
		m_Scale = Vector2(128.0f, 128.0f);
		//��]�p
		m_Rot = 0;
		m_PosSpan = Vector2(3.0f, 0);


		m_DrawContext = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateSrvSmpCbv);
		m_DrawContext->CreateConstantBuffer(sizeof(m_DiffuseSpriteConstantBuffer));
		m_DrawContext->CreateDefault2DPipelineCmdList<VertexPositionTexture, VSPTSprite, PSPTSprite>();
		if (m_Trace) {
			m_DrawContext->SetBlendState(BlendState::AlphaBlend);
		}

		m_DiffuseSpriteConstantBuffer.World = Matrix4X4EX::Identity();
		//�����l�X�V
		m_DrawContext->UpdateConstantBuffer(reinterpret_cast<void**>(&m_DiffuseSpriteConstantBuffer), sizeof(m_DiffuseSpriteConstantBuffer));
		//�e�N�X�`���ݒ�
		m_DrawContext->SetTextureResource(m_TextureResource);


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
		m_DiffuseSpriteConstantBuffer.Emissive = Color4(0, 0, 0, 0.0f);
		m_DiffuseSpriteConstantBuffer.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		m_DiffuseSpriteConstantBuffer.World = World;
		//�X�V
		m_DrawContext->UpdateConstantBuffer(&m_DiffuseSpriteConstantBuffer, sizeof(m_DiffuseSpriteConstantBuffer));
		//�`��
		m_DrawContext->DrawIndexed<VertexPositionTexture>(m_SquareMesh);


	}



}
//end basecross
