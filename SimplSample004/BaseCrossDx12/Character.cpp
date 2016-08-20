/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	�l�p�`�X�v���C�g�̃O���[�v����
	//--------------------------------------------------------------------------------------
	SquareSpriteGroup::SquareSpriteGroup() :
		ObjectInterface(),
		ShapeInterface(),
		m_TotalTime(0)
	{}
	SquareSpriteGroup::~SquareSpriteGroup() {}
	void SquareSpriteGroup::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��
		m_BackupVertices = {
			{ VertexPositionColor(Vector3(-HelfSize, HelfSize, 0), Color4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, HelfSize, 0), Color4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(-HelfSize, -HelfSize, 0), Color4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, -HelfSize, 0), Color4(1.0f, 0.0f, 1.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//���b�V���̍쐬
		//���_�ύX�ł���
		m_SquareSpriteMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);
		//�O���[�v�̔z��̍쐬
		m_SquareSpriteVec.assign(100, SquareSprite());
		for (auto& v : m_SquareSpriteVec) {
			v.m_LocalRot = Util::RandZeroToOne(true);
			v.m_LocalRotVelocity = Util::RandZeroToOne(true) * 20.0f - 10.0f;
			v.m_LocalPos = Vector2(0, 0);
			v.m_LocalVelocity = Vector2(Util::RandZeroToOne(true) * 200.0f - 100.0f, 100 + Util::RandZeroToOne(true) * 100.0f);
			v.m_LocalGravityVelocity = Vector2(0, 0);
			v.m_DrawContext = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateCbv);
			v.m_DrawContext->CreateConstantBuffer(sizeof(v.m_SpriteConstantBuffer));
			v.m_DrawContext->CreateDefault2DPipelineCmdList<VertexPositionColor, VSPCSprite, PSPCSprite>();
		}
	}

	//�e�I�u�W�F�N�g�̈ʒu���̕ύX
	void SquareSpriteGroup::UpdateObjects(float ElapsedTime) {
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		h /= 2.0f;
		for (auto& v : m_SquareSpriteVec) {
			if (v.m_LocalPos.y < -h) {
				v.m_LocalRot = Util::RandZeroToOne(true);
				v.m_LocalRotVelocity = Util::RandZeroToOne(true) * 20.0f - 10.0f;
				v.m_LocalPos = Vector2(0, 0);
				v.m_LocalVelocity = Vector2(Util::RandZeroToOne(true) * 200.0f - 100.0f, 100 + Util::RandZeroToOne(true) * 100.0f);
				v.m_LocalGravityVelocity = Vector2(0, 0);
			}
			else {
				v.m_LocalRot += v.m_LocalRotVelocity * ElapsedTime;
				v.m_LocalPos += v.m_LocalVelocity * ElapsedTime;
				v.m_LocalGravityVelocity += Vector2(0, -98.0f) * ElapsedTime;
				v.m_LocalPos += v.m_LocalGravityVelocity * ElapsedTime;
			}
		}
	}


	void SquareSpriteGroup::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateObjects(ElapsedTime);
	}

	void SquareSpriteGroup::OnDraw() {
		//�O���[�v���܂Ƃ߂ĕ`��
		for (auto& v : m_SquareSpriteVec) {
			//�s��̒�`
			Matrix4X4 World, Proj;
			//���[���h�s��̌���
			World.AffineTransformation2D(
				v.m_LocalScale,			//�X�P�[�����O
				Vector2(0, 0),		//��]�̒��S�i�d�S�j
				v.m_LocalRot,				//��]�p�x
				v.m_LocalPos				//�ʒu
			);
			//�ˉe�s��̌���
			float w = static_cast<float>(App::GetApp()->GetGameWidth());
			float h = static_cast<float>(App::GetApp()->GetGameHeight());
			Proj.OrthographicLH(w, h, -1.0, 1.0f);
			//�s��̍���
			World *= Proj;
			//�R���X�^���g�o�b�t�@�̏���
			v.m_SpriteConstantBuffer.Emissive = Color4(0.0f, 0.0f, 0, 1.0f);
			v.m_SpriteConstantBuffer.World = World;
			//�X�V
			v.m_DrawContext->UpdateConstantBuffer(&v.m_SpriteConstantBuffer, sizeof(v.m_SpriteConstantBuffer));
			//�`��
			v.m_DrawContext->DrawIndexed<VertexPositionColor>(m_SquareSpriteMesh);
		}
	}


}
//end basecross
