/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

/*

	void PhysicsManager::DrawShapeWireFrame(const shared_ptr<MeshResource>& res, const Mat4x4& world) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		Mat4x4 World, ViewMat, ProjMat;
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
*/



	//--------------------------------------------------------------------------------------
	///	�����v�Z����{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PhysicsBox::PhysicsBox(const shared_ptr<Stage>& StagePtr,
		PsMotionType MotionType,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_MotionType(MotionType),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{
	}
	PhysicsBox::~PhysicsBox() {}

	//������
	void PhysicsBox::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		auto PSPtr = GetStage()->GetSharedGameObject<PhysicsManager>(L"PhysicsManager");
		m_PsTransIndex = PSPtr->AddSingleShape(PsShapeType::ShapeTypeBox, m_MotionType,
			m_Scale, m_Qt, m_Position);

	}

	void PhysicsBox::OnUpdate() {
		auto PSPtr = GetStage()->GetSharedGameObject<PhysicsManager>(L"PhysicsManager");
		Quat Qt;
		Vec3 Pos;

		PSPtr->GetShapeQuatPos(m_PsTransIndex, Qt, Pos);
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(Qt);
		PtrTransform->SetPosition(Pos);
	}

}
//end basecross
