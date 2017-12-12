/*!
@file PhysicsManager.cpp
@brief �����v�Z�p�}�l�[�W���Ȃǎ���
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"


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

	shared_ptr<PhysicsBox> PhysicsManager::AddSingleBox(const PsBoxParam& param, const wstring& indexKey) {
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
		return BasePhysics::AddSingleBox(param, indexKey);

	}

	shared_ptr<PhysicsSphere> PhysicsManager::AddSingleSphere(const PsSphereParam& param, const wstring& indexKey) {
		if (!m_SphereMeshRes) {
			//���C�A�t���[���p���b�V���̍쐬�i�ύX�ł��Ȃ��j
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateSphere(2.0f,9, vertices, indices);
			vector<VertexPositionColor> col_vertices;
			for (auto& v : vertices) {
				VertexPositionColor vertex;
				vertex.position = v.position;
				vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				col_vertices.push_back(vertex);
			}
			m_SphereMeshRes = MeshResource::CreateMeshResource(col_vertices, indices, false);
		}
		return BasePhysics::AddSingleSphere(param, indexKey);
	}



	void PhysicsManager::OnUpdate() {
		if (IsUpdateActive()) {
			BasePhysics::Update();
		}
	}


	void PhysicsManager::DrawShapeWireFrame(const shared_ptr<MeshResource>& res, const bsm::Mat4x4& world) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		bsm::Mat4x4 World, ViewMat, ProjMat;
		World = world;
		//�]�u����
		World.transpose();
		//�J�����𓾂�
		auto CameraPtr = OnGetDrawCamera();
		//�r���[�Ǝˉe�s��𓾂�
		ViewMat = CameraPtr->GetViewMatrix();
		//�]�u����
		ViewMat.transpose();
		//�]�u����
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.transpose();
		//�R���X�^���g�o�b�t�@�̏���
		SimpleConstants sb;
		sb.World = World;
		sb.View = ViewMat;
		sb.Projection = ProjMat;
		//�G�~�b�V�u
		sb.Emissive = Col4(0, 0, 0, 0);
		//�f�t�B�[�Y�͂��ׂĒʂ�
		sb.Diffuse = Col4(1, 1, 1, 1);
		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, res->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(res->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//���_�V�F�[�_�ɓn��
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�s�N�Z���V�F�[�_�ɓn��
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPCStatic::GetPtr()->GetInputLayout());
		//�u�����h�X�e�[�g
		//�����������Ȃ�
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//���X�^���C�U�X�e�[�g(���C�A�t���[��)
		pD3D11DeviceContext->RSSetState(RenderState->GetWireframe());
		pD3D11DeviceContext->DrawIndexed(res->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates();

	}


	void PhysicsManager::OnDraw() {
		if (!IsDrawActive()) {
			return;
		}
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
					bsm::Vec3 scale = GetShapeBoxScale(i, j);
					Local.affineTransformation(
						scale,			//�X�P�[�����O
						bsm::Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
						LocalQt,				//��]�p�x
						LocalPos				//�ʒu
					);
					bsm::Mat4x4 DrawWorld = Local * World;
					DrawShapeWireFrame(m_BoxMeshRes, DrawWorld);
				}
				break;
				case sce::PhysicsEffects::ePfxShapeType::kPfxShapeSphere:
				{
					auto rad = GetShapeSphereRadius(i, j);
					Local.affineTransformation(
						bsm::Vec3(rad),			//�X�P�[�����O
						bsm::Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
						LocalQt,				//��]�p�x
						LocalPos				//�ʒu
					);
					bsm::Mat4x4 DrawWorld = Local * World;
					DrawShapeWireFrame(m_SphereMeshRes, DrawWorld);

				}
				break;
				}
			}
		}

	}



}
