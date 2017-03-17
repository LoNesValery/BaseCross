/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	Pnt��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PntSphere::PntSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PntSphere::~PntSphere() {}

	//������
	void PntSphere::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		Quaternion Qt;
		Qt.Identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(m_StartPos);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

	}

	void PntSphere::OnDraw() {
		//���b�V���̎擾
		auto MeshRes = App::GetApp()->GetResource<MeshResource>(L"DEFAULT_SPHERE");
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		//�f�o�C�X�R���e�L�X�g�̎擾
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�����_�[�X�e�[�g�̎擾
		auto RenderState = Dev->GetRenderState();
		//�R���X�^���g�o�b�t�@�̏���
		TestConstants Cb;
		//�s��̒�`
		auto PtrTrans = GetComponent<Transform>();
		//�s��̒�`
		Matrix4X4 World, ViewMat, ProjMat;
		//���[���h�s��̌���
		World = PtrTrans->GetWorldMatrix();
		//�]�u����
		World.Transpose();
		//�J�����𓾂�
		auto CameraPtr = OnGetDrawCamera();
		//�r���[�Ǝˉe�s��𓾂�
		ViewMat = CameraPtr->GetViewMatrix();
		//�]�u����
		ViewMat.Transpose();
		ProjMat = CameraPtr->GetProjMatrix();
		//�]�u����
		ProjMat.Transpose();
		Cb.World = World;
		Cb.View = ViewMat;
		Cb.Projection = ProjMat;
		//�G�~�b�V�u
		Cb.Emissive = Color4(0.5f, 0.5f, 0.5f,0);
		//�f�t�B�[�Y
		Cb.Diffuse = Color4(1, 1, 1, 1);
		//���C�e�B���O
		auto StageLight = OnGetDrawLight();
		Cb.LightDir = StageLight.m_Directional;
		Cb.LightDir.w = 1.0f;
		//�e�N�X�`���i�V�F�[�_���\�[�X�j�̎擾
		auto Res = App::GetApp()->GetResource<TextureResource>(L"WALL_TX");
		pD3D11DeviceContext->PSSetShaderResources(0, 1, Res->GetShaderResourceView().GetAddressOf());
		auto pSampler = RenderState->GetLinearClamp();
		//�T���v���[��ݒ�
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBTest::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBTest::GetPtr()->GetBuffer();
		//���_�V�F�[�_�ɓn��
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�s�N�Z���V�F�[�_�ɓn��
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSTest::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSTest::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSTest::GetPtr()->GetInputLayout());
		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//�u�����h�X�e�[�g
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		//���X�^���C�U�X�e�[�g
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//�`��
		pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates();
	}


}
//end basecross
