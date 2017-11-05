/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�J�X�^���V���h�E�}�b�v�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	CustomShadowmap::CustomShadowmap(const shared_ptr<GameObject>& GameObjectPtr) :
		Shadowmap(GameObjectPtr) {}

	CustomShadowmap::~CustomShadowmap() {}

	//����
	void CustomShadowmap::OnDraw() {
		auto PtrGameObject = GetGameObject();
		auto PtrMeshResource = GetMeshResource();

		auto PtrT = PtrGameObject->GetComponent<Transform>();
		//�X�e�[�W����0�Ԗڂ̃��C�g�����o��
		auto DrawLight = PtrGameObject->OnGetDrawLight();
		//�X�e�[�W����J���������o��
		auto PtrCamera = PtrGameObject->OnGetDrawCamera();


		Mat4x4 RealWorldMatrix = GetMeshToTransformMatrix() * PtrT->GetWorldMatrix();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();


		//���C�g�̎擾
		Mat4x4 LightView, LightProj;

		Vec3 LightDir = -1.0 * DrawLight.m_Directional;
		Vec3 LightAt = PtrCamera->GetAt();
		Vec3 LightEye = LightAt + (LightDir * GetLightHeight());

		//���C�g�̃r���[�Ǝˉe���v�Z
		LightView = XMMatrixLookAtLH(LightEye, LightAt, Vec3(0, 1.0f, 0));
		LightProj = XMMatrixOrthographicLH(GetViewWidth(), GetViewHeight(), GetLightNear(), GetLightFar());
		CustomShadowmapConstantBuffer Cb;

		Cb.mWorld[0] = transpose(RealWorldMatrix);
		Mat4x4 Left;
		Left.translation(Vec3(-5.0f, 0, 0));
		Left = RealWorldMatrix * Left;
		Cb.mWorld[1] = transpose(Left);
		Mat4x4 Right;
		Right.translation(Vec3(5.0f, 0, 0));
		Right = RealWorldMatrix * Right;
		Cb.mWorld[2] = transpose(Right);
		Cb.mView = transpose(LightView);
		Cb.mProj = transpose(LightProj);
		//������`�揈��
		//�R���X�^���g�o�b�t�@�̍X�V
		pID3D11DeviceContext->UpdateSubresource(CBCustomShadowmap::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
		//�C���v�b�g���C�A�E�g�̃Z�b�g
		pID3D11DeviceContext->IASetInputLayout(VSCustomShadowmap::GetPtr()->GetInputLayout());
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = PtrMeshResource->GetNumStride();
		UINT offset = 0;

		auto GameObj = dynamic_pointer_cast<CustomDrawOctahedron>(GetGameObject());
		//���_�o�b�t�@���Z�b�g

		pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//���_�V�F�[�_�[�̃Z�b�g
		pID3D11DeviceContext->VSSetShader(VSCustomShadowmap::GetPtr()->GetShader(), nullptr, 0);
		//�W�I���g���V�F�[�_�̃Z�b�g
		pID3D11DeviceContext->GSSetShader(GSCustomShadowmap::GetPtr()->GetShader(), nullptr, 0);

		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�s�N�Z���V�F�[�_�̓Z�b�g���Ȃ��I
		pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);

		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBCustomShadowmap::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->GSSetConstantBuffers(0, 1, &pConstantBuffer);
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//�R���X�^���g�o�b�t�@���s�N�Z���V�F�[�_�ɃZ�b�g
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
		//�`��
		pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates();


	}

	//--------------------------------------------------------------------------------------
	//	�W�I���g���V�F�[�_���g�����Ǝ��̕`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	CustomPNCStaticDraw::CustomPNCStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr) {}
	//����
	void CustomPNCStaticDraw::OnDraw() {
		auto PtrGameObject = GetGameObject();
		//���b�V�����Ȃ���Ε`�悵�Ȃ�
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!MeshRes)",
				L"CustomPNCStaticDraw::OnDraw()"
			);
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		auto PtrT = PtrGameObject->GetComponent<Transform>();
		//�J�����𓾂�
		auto PtrCamera = PtrGameObject->OnGetDrawCamera();
		//�J�����̎擾
		Mat4x4 View, Proj, WorldViewProj;
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();

		//�R���X�^���g�o�b�t�@�̐ݒ�
		CustomDrawConstantBuffer cb1;
		//�s��̐ݒ�(�]�u����)
		cb1.World[0] = transpose(PtrT->GetWorldMatrix());
		Mat4x4 Left;
		Left.translation(Vec3(-5.0f, 0, 0));
		Left = PtrT->GetWorldMatrix() * Left;
		cb1.World[1] = transpose(Left);
		Mat4x4 Right;
		Right.translation(Vec3(5.0f, 0, 0));
		Right = PtrT->GetWorldMatrix() * Right;
		cb1.World[2] = transpose(Right);
		cb1.View = transpose(View);
		cb1.Projection = transpose(Proj);
		//���C�g�̐ݒ�
		auto PtrLight = PtrGameObject->OnGetDrawLight();
		cb1.LightDir = PtrLight.m_Directional;
		cb1.LightDir.w = 1.0f;

		//�R���X�^���g�o�b�t�@�̍X�V
		pID3D11DeviceContext->UpdateSubresource(CBCustomDraw::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//���_�o�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�X�e�[�^�X�̃|�C���^
		//�e�N�X�`�����擾
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pNullSR[1] = { 0 };
		//����������
		pID3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		//�f�v�X�X�e���V���͎g�p����
		pID3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(VSCustomDraw::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->GSSetShader(GSCustomDraw::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSCustomDraw::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(VSCustomDraw::GetPtr()->GetInputLayout());
		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBCustomDraw::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->GSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderState->GetCullFront());
		//�`��
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//�`��
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	//	�Ǝ��̕`��R���|�[�l���g�ŕ`�悷��8�ʑ�
	//--------------------------------------------------------------------------------------
	//���_�̕ύX
	void CustomDrawOctahedron::UpdateCustomMesh() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0;
		}
		vector<VertexPositionNormalColor> newvec;
		for (size_t i = 0; i < m_BackupVertices.size(); i++) {
			Vec3 Pos = m_BackupVertices[i].position;
			if (Pos.y > 0) {
				//�T�C�����g���Ă������藈���肷��悤�ɂ���
				Pos.y += sin(m_TotalTime);
			}
			newvec.push_back(
				VertexPositionNormalColor(
					Pos,
					m_BackupVertices[i].normal,
					m_BackupVertices[i].color
				)
			);
		}
		auto PtrDraw = GetComponent<CustomPNCStaticDraw>();
		PtrDraw->UpdateVertices(newvec);
	}

	//�\�z�Ɣj��
	CustomDrawOctahedron::CustomDrawOctahedron(shared_ptr<Stage>& StagePtr, 
		const Vec3& StartScale, const Vec3& StartRotation, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartScale(StartScale),
		m_StartRotation(StartRotation),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}
	CustomDrawOctahedron::~CustomDrawOctahedron() {
	}
	//������
	void CustomDrawOctahedron::OnCreate() {
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(m_StartScale);
		Ptr->SetPosition(m_StartPos);
		Ptr->SetRotation(m_StartRotation);
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		//��8�ʑ�
		MeshUtill::CreateOctahedron(1.0f, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionNormalColor new_v;
			new_v.position = vertices[i].position;
			new_v.normal = vertices[i].normal;
			new_v.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			m_BackupVertices.push_back(new_v);
		}
		auto PtrDraw = AddComponent<CustomPNCStaticDraw>();
		PtrDraw->CreateOriginalMesh(m_BackupVertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<CustomShadowmap>();
		ShadowPtr->SetMeshResource(PtrDraw->GetMeshResource());
		//���������i�`�揇����̂��߁j
		SetAlphaActive(true);
	}
	void CustomDrawOctahedron::OnUpdate() {
		UpdateCustomMesh();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto PtrTransform = GetComponent<Transform>();
		auto Qt = PtrTransform->GetQuaternion();
		Quat Span;
		Span.rotationRollPitchYawFromVector(Vec3(0, ElapsedTime, 0));
		Qt *= Span;
		PtrTransform->SetQuaternion(Qt);
	}

}
//end basecross
