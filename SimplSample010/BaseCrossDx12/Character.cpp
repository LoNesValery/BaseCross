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
	CubeObject::CubeObject(const Vector3& Pos, bool Flat) :
		ObjectInterface(),
		ShapeInterface(),
		m_Pos(Pos),
		m_Flat(Flat)
	{}
	CubeObject::~CubeObject() {}

	void CubeObject::CreateBuffers() {
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

		vector<VertexPositionNormal> vertices;
		vector<uint16_t> indices;
		UINT BasePosCount = 0;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				VertexPositionNormal Data;
				Data.position = PosVec[PosIndeces[BasePosCount + j]];
				if (m_Flat) {
					//�t���b�g�\���̏ꍇ�͖@���͒��_�����ɂ���
					Data.normal = Data.position;
					Data.normal.Normalize();
				}
				else {
					//�t���b�g�\�����Ȃ��ꍇ�́A�@���͖ʂ̌���
					Data.normal = FaceNormalVec[i];
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
		//���C�e�B���O
		Vector4 LightDir(0.5f, -1.0f, 0.5f, 0.0f);
		LightDir.Normalize();
		m_StaticConstantBuffer.World = World;
		m_StaticConstantBuffer.View = View;
		m_StaticConstantBuffer.Projection = Proj;
		m_StaticConstantBuffer.LightDir = LightDir;
		m_StaticConstantBuffer.Diffuse = Color4(0.0f, 0.0f, 1.0f, 1.0f);
		m_StaticConstantBuffer.Emissive = Color4(0, 0, 0, 0);
		//�X�V
		m_DrawContext->UpdateConstantBuffer(reinterpret_cast<void**>(&m_StaticConstantBuffer), sizeof(m_StaticConstantBuffer));
	}



	void CubeObject::OnCreate() {
		CreateBuffers();
		m_Scale = Vector3(1.0f, 1.0f, 1.0f);
		m_Qt.Identity();
		//�`��R���e�L�X�g
		m_DrawContext = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateSrvSmpCbv);
		m_DrawContext->CreateConstantBuffer(sizeof(m_StaticConstantBuffer));
		m_DrawContext->CreateDefault3DPipelineCmdList<VertexPositionNormal, VSPNStatic, PSPNStatic>();
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
		m_DrawContext->DrawIndexed<VertexPositionNormal>(m_CubeMesh);
	}


}
//end basecross
