/*!
@file DrawObjects.cpp
@brief �`��I�u�W�F�N�g����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�V���h�E�}�b�v�̕`��N���X
	//--------------------------------------------------------------------------------------
	float ShadowmapDrawObject::m_LightHeight(20.0f);
	float ShadowmapDrawObject::m_LightNear(1.0f);
	float ShadowmapDrawObject::m_LightFar(200.0f);
	float ShadowmapDrawObject::m_ViewWidth(32.0f);
	float ShadowmapDrawObject::m_ViewHeight(32.0f);

	ShadowmapDrawObject::ShadowmapDrawObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	ShadowmapDrawObject::~ShadowmapDrawObject() {}

	void ShadowmapDrawObject::OnCreate() {
		//�^�O�̒ǉ�
		AddTag(L"ShadowmapDrawObject");
	}

	void ShadowmapDrawObject::AddDrawMesh(const shared_ptr<MeshResource>& MeshRes, const Mat4x4& WorldMat) {
		ShadowmapObject Obj;
		Obj.m_MeshRes = MeshRes;
		Obj.m_WorldMatrix = WorldMat;
		m_ShadowmapObjectVec.push_back(Obj);
	}


	void ShadowmapDrawObject::OnDrawShadowmap() {

		auto PtrGameStage = GetStage<GameStage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//�e�I�u�W�F�N�g���ʏ���
		//�V�F�[�_�̐ݒ�
		//���_�V�F�[�_�[�̃Z�b�g
		pID3D11DeviceContext->VSSetShader(VSShadowmap::GetPtr()->GetShader(), nullptr, 0);
		//�s�N�Z���V�F�[�_�̓Z�b�g���Ȃ��I
		pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		//�W�I���g���V�F�[�_�̐ݒ�i�g�p���Ȃ��j
		pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//�C���v�b�g���C�A�E�g�̃Z�b�g
		pID3D11DeviceContext->IASetInputLayout(VSShadowmap::GetPtr()->GetInputLayout());
		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;

		//�ʏ���
		for (auto& v : m_ShadowmapObjectVec) {
			//�ʒu�̎擾
			auto Pos = v.m_WorldMatrix.transInMatrix();
			Vec4 LightDir4;
			PtrGameStage->GetLightDir(LightDir4);
			Vec3 LightDir(LightDir4.x, LightDir4.y, LightDir4.z);
			Vec3 PosSpan = LightDir;
			PosSpan *= 0.1f;
			Pos += PosSpan;
			//�s��̒�`
			Mat4x4 World, LightView, LightProj;
			//���[���h�s��̌���
			World.affineTransformation(
				v.m_WorldMatrix.scaleInMatrix(),			//�X�P�[�����O
				Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
				v.m_WorldMatrix.quatInMatrix(),				//��]�p�x
				Pos				//�ʒu
			);
			LightDir = LightDir  * -1.0;
			Vec3 LightAt = PtrGameStage->GetCamera().m_CamerAt;
			Vec3 LightEye = LightAt + (LightDir * m_LightHeight);
			//���C�g�̃r���[�Ǝˉe���v�Z
			LightView = XMMatrixLookAtLH(LightEye, LightAt, Vec3(0, 1.0f, 0));
			LightProj = XMMatrixOrthographicLH(m_ViewWidth, m_ViewHeight, m_LightNear, m_LightFar);
			ShadowConstants Cb;
			Cb.mWorld = bsm::transpose(World);
			Cb.mView = bsm::transpose(LightView);
			Cb.mProj = bsm::transpose(LightProj);
			//�R���X�^���g�o�b�t�@�̍X�V
			pID3D11DeviceContext->UpdateSubresource(CBShadow::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
			//���_�o�b�t�@���Z�b�g
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, v.m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pID3D11DeviceContext->IASetIndexBuffer(v.m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBShadow::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�R���X�^���g�o�b�t�@���s�N�Z���V�F�[�_�ɃZ�b�g
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
			//�`��
			pID3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
		}
		//��n��
		Dev->InitializeStates();
		m_ShadowmapObjectVec.clear();
	}


	//--------------------------------------------------------------------------------------
	///	PNT���_�I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	PNTDrawObject::PNTDrawObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	PNTDrawObject::~PNTDrawObject() {}

	void PNTDrawObject::OnCreate() {
		//�^�O�̒ǉ�
		AddTag(L"PNTDrawObject");
	}


	void PNTDrawObject::AddDrawMesh(const shared_ptr<MeshResource>& MeshRes,
		const shared_ptr<TextureResource>& TextureRes,
		const Mat4x4& WorldMat,
		bool Trace, bool Wrap) {
		DrawObject Obj;
		Obj.m_MeshRes = MeshRes;
		Obj.m_TextureRes = TextureRes;
		Obj.m_WorldMatrix = WorldMat;
		Obj.m_Trace = Trace;
		Obj.m_Wrap = Wrap;
		if (Trace) {
			m_TraceDrawObjectVec.push_back(Obj);
		}
		else {
			m_DrawObjectVec.push_back(Obj);
		}
	}

	void PNTDrawObject::OnDrawSub(vector<DrawObject>& ObjectVec, PNTStaticConstantBuffer& sb) {
		auto PtrGameStage = GetStage<GameStage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//�T���v���[�̏���
		ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
		ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//�ʏ���
		for (auto& v : ObjectVec) {
			//�]�u����
			v.m_WorldMatrix.transpose();
			//���[���h�s��̌���
			sb.World = v.m_WorldMatrix;
			//�R���X�^���g�o�b�t�@�̍X�V
			pD3D11DeviceContext->UpdateSubresource(CBPNTStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBPNTStatic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//���_�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, v.m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetIndexBuffer(v.m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//�e�N�X�`���̐ݒ�
			ID3D11ShaderResourceView* pNull[1] = { 0 };
			pD3D11DeviceContext->PSSetShaderResources(0, 1, v.m_TextureRes->GetShaderResourceView().GetAddressOf());
			//�T���v���[
			if (v.m_Wrap) {
				pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerWrap);
			}
			else {
				pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerClamp);
			}
			//�u�����h�X�e�[�g
			if (v.m_Trace) {
				//��������
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				//���������̏ꍇ�́A���X�^���C�U�X�e�[�g��ύX����2��`��
				//���X�^���C�U�X�e�[�g�i���ʕ`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//�`��
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
				//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//�`��
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//�����������Ȃ�
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
				//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//�`��
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}
		}
	}

	void PNTDrawObject::OnDraw() {
		auto PtrGameStage = GetStage<GameStage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//�e�I�u�W�F�N�g���ʏ���
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//�s��̒�`
		Mat4x4 View, Proj;
		//���C�e�B���O
		Vec4 LightDir;
		PtrGameStage->GetCamera().GetViewProjMatrix(View, Proj);
		PtrGameStage->GetLightDir(LightDir);
		//�r���[�s��̌���
		//�]�u����
		View.transpose();
		//�ˉe�s��̌���
		//�]�u����
		Proj.transpose();
		//�R���X�^���g�o�b�t�@�̏���
		PNTStaticConstantBuffer sb;
		sb.View = View;
		sb.Projection = Proj;
		sb.LightDir = LightDir;
		//�f�B�t���[�Y
		sb.Diffuse = Col4(0.8f, 0.8f, 0.8f, 1.0f);
		//�G�~�b�V�u���Z�B
		sb.Emissive = Col4(0.4f, 0.4f, 0.4f, 0);
		//�X�y�L�����[�B
		sb.Specular = Col4(0.2f, 0.2f, 0.2f, 0);
		//�J�����̈ʒu
		Vec3 CameraEye = PtrGameStage->GetCamera().m_CamerEye;
		sb.EyePos = CameraEye;
		//�T�u�֐��Ăяo��(�s����)
		OnDrawSub(m_DrawObjectVec, sb);
		//--------------------------------------------------------
		//������3D�I�u�W�F�N�g���J��������̋����Ń\�[�g
		//�ȉ��́A�I�u�W�F�N�g�������Ɏ��bool��Ԃ������_��
		//--------------------------------------------------------
		auto func = [&](DrawObject& Left, DrawObject& Right)->bool {
			auto LeftPos = Left.m_WorldMatrix.transInMatrix();
			auto RightPos = Right.m_WorldMatrix.transInMatrix();
			auto LeftLen = bsm::length(LeftPos - CameraEye);
			auto RightLen = bsm::length(RightPos - CameraEye);
			return (LeftLen > RightLen);
		};
		//�����_�����g���ă\�[�g
		std::sort(m_TraceDrawObjectVec.begin(), m_TraceDrawObjectVec.end(), func);
		//�T�u�֐��Ăяo��(����)
		OnDrawSub(m_TraceDrawObjectVec, sb);
		//��n��
		Dev->InitializeStates();
		//�`��p�̔z����N���A
		m_DrawObjectVec.clear();
		m_TraceDrawObjectVec.clear();
	}


	//--------------------------------------------------------------------------------------
	///	�e�t��PNT���_�I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	PNTShadowDrawObject::PNTShadowDrawObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	PNTShadowDrawObject::~PNTShadowDrawObject() {}

	void PNTShadowDrawObject::OnCreate() {
		//�^�O�̒ǉ�
		AddTag(L"PNTShadowDrawObject");
	}

	void PNTShadowDrawObject::AddDrawMesh(const shared_ptr<MeshResource>& MeshRes,
		const shared_ptr<TextureResource>& TextureRes,
		const Mat4x4& WorldMat,
		bool Trace, bool Wrap, bool OwnShadowmapActive) {
		DrawObject Obj;
		Obj.m_MeshRes = MeshRes;
		Obj.m_TextureRes = TextureRes;
		Obj.m_WorldMatrix = WorldMat;
		Obj.m_Trace = Trace;
		Obj.m_Wrap = Wrap;
		Obj.m_OwnShadowmapActive = OwnShadowmapActive;
		m_DrawObjectVec.push_back(Obj);
	}


	void PNTShadowDrawObject::SetConstants(DrawObject& DrawObj, PNTStaticShadowConstantBuffer& Cb) {
		auto PtrGameStage = GetStage<GameStage>();
		//�s��̒�`
		bsm::Mat4x4 World, ViewMat, ProjMat;
		//���[���h�s��̌���
		World = DrawObj.m_WorldMatrix;
		//�]�u����
		World.transpose();
		//�J�����𓾂�
		PtrGameStage->GetCamera().GetViewProjMatrix(ViewMat, ProjMat);
		Vec4 LightDir;
		PtrGameStage->GetLightDir(LightDir);
		//�]�u����
		ViewMat.transpose();
		//�]�u����
		ProjMat.transpose();
		Cb.World = World;
		Cb.View = ViewMat;
		Cb.Projection = ProjMat;
		//�f�B�t���[�Y
		Cb.Diffuse = Col4(0.8f, 0.8f, 0.8f, 1.0f);
		//�G�~�b�V�u���Z�B
		Cb.Emissive = Col4(0.4f, 0.4f, 0.4f, 0);
		//�X�y�L�����[�B
		Cb.Specular = Col4(0.2f, 0.2f, 0.2f, 0);
		//���C�e�B���O
		Cb.LightDir = LightDir;
		Cb.LightDir.w = 1.0f;
		Cb.EyePos = PtrGameStage->GetCamera().m_CamerEye;
		Cb.EyePos.w = 1.0f;
		Vec3 CalcLightDir(LightDir.x, LightDir.y, LightDir.z);
		CalcLightDir = -1.0 * CalcLightDir;
		Vec3 LightAt = PtrGameStage->GetCamera().m_CamerAt;
		Vec3 LightEye = CalcLightDir;
		auto ShadowObj = PtrGameStage->FindTagGameObject<ShadowmapDrawObject>(L"ShadowmapDrawObject");
		LightEye *= ShadowmapDrawObject::m_LightHeight;
		LightEye = LightAt + LightEye;
		Cb.LightPos = LightEye;
		Cb.LightPos.w = 1.0f;
		bsm::Mat4x4 LightView, LightProj;
		//���C�g�̃r���[�Ǝˉe���v�Z
		LightView = XMMatrixLookAtLH(LightEye, LightAt, Vec3(0, 1.0f, 0));
		LightProj = XMMatrixOrthographicLH(ShadowmapDrawObject::m_ViewWidth, ShadowmapDrawObject::m_ViewHeight,
			ShadowmapDrawObject::m_LightNear, ShadowmapDrawObject::m_LightFar);
		Cb.LightView = bsm::transpose(LightView);
		Cb.LightProjection = bsm::transpose(LightProj);

	}


	void PNTShadowDrawObject::OnDraw() {
		auto PtrGameStage = GetStage<GameStage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//�T���v���[�̏���
		ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
		ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();

		//�ʏ���
		for (auto& v : m_DrawObjectVec) {
			//�R���X�^���g�o�b�t�@�̏���
			PNTStaticShadowConstantBuffer Cb;
			SetConstants(v, Cb);
			//�e�N�X�`��
			pD3D11DeviceContext->PSSetShaderResources(0, 1, v.m_TextureRes->GetShaderResourceView().GetAddressOf());
			//�R���X�^���g�o�b�t�@�̍X�V
			pD3D11DeviceContext->UpdateSubresource(CBPNTStaticShadow::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
			//�X�g���C�h�ƃI�t�Z�b�g
			UINT stride = v.m_MeshRes->GetNumStride();
			UINT offset = 0;
			//���_�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, v.m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetIndexBuffer(v.m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//�`����@�i3�p�`�j
			pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBPNTStaticShadow::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�V�F�[�_�̐ݒ�
			pD3D11DeviceContext->VSSetShader(VSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
			if (v.m_OwnShadowmapActive) {
				//���ȉe������ꍇ
				pD3D11DeviceContext->PSSetShader(PSPNTStaticShadow2::GetPtr()->GetShader(), nullptr, 0);
			}
			else {
				//���ȉe���Ȃ��ꍇ
				pD3D11DeviceContext->PSSetShader(PSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
			}
			//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
			auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
			ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
			pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
			//�V���h�E�}�b�v�T���v���[
			ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
			pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pD3D11DeviceContext->IASetInputLayout(VSPNTStaticShadow::GetPtr()->GetInputLayout());
			//�I�u�W�F�N�g�̃T���v���[
			if (v.m_Wrap) {
				pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerWrap);
			}
			else {
				pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerClamp);
			}
			//�f�v�X�X�e���V���X�e�[�g
			pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
			//�u�����h�X�e�[�g
			if (v.m_Trace) {
				//��������
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				//���������̏ꍇ�́A���X�^���C�U�X�e�[�g��ύX����2��`��
				//���X�^���C�U�X�e�[�g�i���ʕ`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//�`��
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
				//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//�`��
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//�����������Ȃ�
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
				//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//�`��
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}

		}
		//��n��
		Dev->InitializeStates();
		m_DrawObjectVec.clear();
	}



}
//end basecross
