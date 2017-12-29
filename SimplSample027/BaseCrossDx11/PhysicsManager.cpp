/*!
@file PhysicsManager.cpp
@brief �����v�Z�p�}�l�[�W���Ȃǎ���
*/
#include "stdafx.h"
#include "Project.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�����v�Z�p�}�l�[�W��
	//--------------------------------------------------------------------------------------
	PhysicsManager::PhysicsManager(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		BasePhysics()
	{
	}
	PhysicsManager::~PhysicsManager() {}

	void PhysicsManager::CreateDrawObject() {
		auto PtrObj = make_shared<SimpleDrawObject>();
		//�A�t�B���ϊ�
		PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		PtrObj->m_Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		PtrObj->m_RasterizerState = RasterizerState::Wireframe;
		m_PtrObjVec.push_back(PtrObj);
	}


	shared_ptr<PhysicsBox> PhysicsManager::AddBox(const PsBoxParam& param, uint16_t index) {
		if (!m_BoxMeshRes) {
			//���C�A�t���[���p���b�V���̍쐬�i�ύX�ł��Ȃ��j
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateCube(2.0f, vertices, indices);
			vector<VertexPositionColor> col_vertices;
			for (auto& v : vertices) {
				VertexPositionColor vertex;
				vertex.position = v.position;
				vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				col_vertices.push_back(vertex);
			}
			m_BoxMeshRes = MeshResource::CreateMeshResource(col_vertices, indices, false);
		}
		if (index >= GetNumBodies()) {
			CreateDrawObject();
		}
		return BasePhysics::AddBox(param, index);

	}

	shared_ptr<PhysicsSphere> PhysicsManager::AddSphere(const PsSphereParam& param, uint16_t index) {
		if (!m_SphereMeshRes) {
			//���C�A�t���[���p���b�V���̍쐬�i�ύX�ł��Ȃ��j
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateSphere(2.0f, 6, vertices, indices);
			vector<VertexPositionColor> col_vertices;
			for (auto& v : vertices) {
				VertexPositionColor vertex;
				vertex.position = v.position;
				vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				col_vertices.push_back(vertex);
			}
			m_SphereMeshRes = MeshResource::CreateMeshResource(col_vertices, indices, false);
		}
		if (index >= GetNumBodies()) {
			CreateDrawObject();
		}
		return BasePhysics::AddSphere(param, index);
	}


	void PhysicsManager::OnCreate() {
	}


	void PhysicsManager::OnUpdate() {
		BasePhysics::Update();
	}


	void PhysicsManager::DrawShapeWireFrame(uint16_t index, const shared_ptr<MeshResource>& res, const bsm::Mat4x4& world) {
		m_PtrObjVec[index]->m_MeshRes = res;
		m_PtrObjVec[index]->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObjVec[index]->m_WorldMatrix = world;
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<SimplePCStaticRenderer>(L"SimplePCStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObjVec[index]);
	}


	void PhysicsManager::OnDraw() {
		for (auto i = 0; i < GetNumBodies(); i++) {
			//�s��̒�`
			bsm::Mat4x4 World, Local;
			//bsm::Vec3 Pos;
			//bsm::Quat Qt;
			PsBodyStatus Status;
			GetBodyStatus(i, Status);
			//���[���h�s��̌���
			World.affineTransformation(
				bsm::Vec3(1.0, 1.0, 1.0),			//�X�P�[�����O
				bsm::Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
				Status.m_Orientation,				//��]�p�x
				Status.m_Position			//�ʒu
			);
			for (auto j = 0; j <  GetNumShapes(i); j++) {
				bsm::Vec3 LocalPos;
				bsm::Quat LocalQt;
				GetShapeOffsetQuatPos(i, j, LocalQt, LocalPos);
				switch (GetShapeType(i,j)) {
				case sce::PhysicsEffects::ePfxShapeType::kPfxShapeBox:
				{
					auto& shape = getPfxShape(i, j);
					Local.affineTransformation(
						(bsm::Vec3)shape.getBox().m_half,			//�X�P�[�����O
						bsm::Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
						LocalQt,				//��]�p�x
						LocalPos				//�ʒu
					);
					bsm::Mat4x4 DrawWorld = Local * World;
					DrawShapeWireFrame(i,m_BoxMeshRes, DrawWorld);
				}
				break;
				case sce::PhysicsEffects::ePfxShapeType::kPfxShapeSphere:
				{
					auto& shape = getPfxShape(i, j);
					Local.affineTransformation(
						bsm::Vec3(shape.getSphere().m_radius),			//�X�P�[�����O
						bsm::Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
						LocalQt,				//��]�p�x
						LocalPos				//�ʒu
					);
					bsm::Mat4x4 DrawWorld = Local * World;
					DrawShapeWireFrame(i,m_SphereMeshRes, DrawWorld);

				}
				break;
				}
			}
		}

	}



}
