/*!
@file DrawObjects.cpp
@brief �`��I�u�W�F�N�g����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

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
	//	�p�[�e�B�N��
	//	*1�̃G�~�b�^�[�������A������ParticleSprite��ێ�����
	//--------------------------------------------------------------------------------------
	Particle::Particle(size_t Count, ParticleDrawOption Option) :
		ObjectInterface(),
		m_DrawOption(Option),
		m_ParticleSpriteVec(Count),
		m_EmitterPos(0, 0, 0),
		m_TotalTime(0),
		m_MaxTime(0)
	{}
	Particle::~Particle() {}

	void Particle::Draw(const shared_ptr<ParticleDrawObject>& Manager) {
		for (auto Psp : m_ParticleSpriteVec) {
			if (Psp.m_Active && !m_TextureResource.expired()) {
				Manager->AddParticle(Psp, m_DrawOption,
					m_EmitterPos, m_TextureResource.lock());
			}
		}
	}


	//--------------------------------------------------------------------------------------
	///	�}���`�p�[�e�B�N��(�p�[�e�B�N���G�t�F�N�g)
	//--------------------------------------------------------------------------------------
	MultiParticle::MultiParticle(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_AddType(false)
	{}
	MultiParticle::~MultiParticle() {}

	shared_ptr<Particle> MultiParticle::InsertParticle(size_t Count, ParticleDrawOption Option) {
		for (size_t i = 0; i < m_ParticleVec.size(); i++) {
			//������A�N�e�B�u�̃p�[�e�B�N��������Ώ��������ă��^�[��
			if (!m_ParticleVec[i]->IsActive()) {
				m_ParticleVec[i]->Reflesh(Count, Option);
				return m_ParticleVec[i];
			}
		}
		//�V�����p�[�e�B�N����ǉ�
		shared_ptr<Particle> ParticlePtr = ObjectFactory::Create<Particle>(Count, Option);
		m_ParticleVec.push_back(ParticlePtr);
		return ParticlePtr;
	}


	void MultiParticle::OnUpdate() {
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto ParticlePtr : GetParticleVec()) {
			ParticlePtr->AddTotalTime(ElapsedTime);
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
					//�ړ����x�ɏ]���Ĉړ�������
					rParticleSprite.m_LocalPos += rParticleSprite.m_Velocity * ElapsedTime;
					if (ParticlePtr->m_TotalTime >= ParticlePtr->m_MaxTime) {
						//�������ԂɂȂ�����
						rParticleSprite.m_Active = false;
					}
				}
			}
		}
	}


	void MultiParticle::OnDraw() {
		auto PtrGameStage = GetStage<GameStage>();
		auto DrawPtrAdd = PtrGameStage->FindTagGameObject<ParticleDrawObject>(L"ParticleDrawObjectAdd");
		auto DrawPtrAlpha = PtrGameStage->FindTagGameObject<ParticleDrawObject>(L"ParticleDrawObjectAlpha");

		if (m_ParticleVec.size() > 0) {
			for (auto Ptr : m_ParticleVec) {
				if (Ptr->IsActive()) {
					if (IsAddType()) {
						Ptr->Draw(DrawPtrAdd);
					}
					else {
						Ptr->Draw(DrawPtrAlpha);
					}
				}
			}
		}
	}



	//--------------------------------------------------------------------------------------
	///	�p�[�e�B�N���`��I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ə���
	ParticleDrawObject::ParticleDrawObject(const shared_ptr<Stage>& StagePtr, bool AddType) :
		GameObject(StagePtr),
		m_MaxInstance(2000),
		m_ZBufferUse(true),
		m_SamplerWrap(false),
		m_AddType(AddType)
	{}
	ParticleDrawObject::~ParticleDrawObject() {}

	void ParticleDrawObject::CreateParticleBuffers() {
		try {
			float HelfSize = 0.5f;
			bsm::Vec4 col(1.0f, 1.0f, 1.0f, 1.0f);
			//���_�z��
			vector<VertexPositionColorTexture> vertices = {
				{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),  col,Vec2(0.0f, 0.0f)) },
				{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), col, Vec2(1.0f, 0.0f)) },
				{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0),  col,Vec2(0.0f, 1.0f)) },
				{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0),  col, Vec2(1.0f, 1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//���b�V���̍쐬
			m_MeshResource = MeshResource::CreateMeshResource(vertices, indices, false);
			//�C���X�^���X�s��o�b�t�@�̍쐬
			//Max�l�ō쐬����
			vector<Mat4x4> matrices(m_MaxInstance, Mat4x4());
			MeshResource::CreateDynamicVertexBuffer(m_MatrixBuffer, matrices);
		}
		catch (...) {
			throw;
		}
	}


	//������
	void ParticleDrawObject::OnCreate() {
		CreateParticleBuffers();
		//�^�O�̒ǉ�
		if (m_AddType) {
			AddTag(L"ParticleDrawObjectAdd");
		}
		else {
			AddTag(L"ParticleDrawObjectAlpha");
		}
	}

	void ParticleDrawObject::AddParticle(const ParticleSprite& rParticleSprite, ParticleDrawOption Option,
		const bsm::Vec3& EmitterPos, const shared_ptr<TextureResource>& TextureRes) {
		if (m_DrawParticleSpriteVec.size() >= m_MaxInstance) {
			//�p�[�e�B�N���X�v���C�g������𒴂���
			return;
		}
		auto PtrGameStage = GetStage<GameStage>();
		//�J�����̈ʒu
		Vec3 CameraEye = PtrGameStage->GetCamera().m_CamerEye;
		Vec3 CameraAt = PtrGameStage->GetCamera().m_CamerAt;
		Vec3 WorldPos = rParticleSprite.m_LocalPos + EmitterPos;
		float ToCaneraLength = bsm::length(CameraEye - WorldPos);
		Vec3 LocalScale;
		LocalScale.x = rParticleSprite.m_LocalScale.x;
		LocalScale.y = rParticleSprite.m_LocalScale.y;
		LocalScale.z = 1.0f;

		Vec3 Temp;
		Quat Qt;
		Mat4x4 RotMatrix;
		Vec3 DefUp(0, 1.0f, 0);
		switch (Option) {
		case ParticleDrawOption::Billboard:
		{
			Temp = CameraAt - CameraEye;
			Vec2 TempVec2(Temp.x, Temp.z);
			if (bsm::length(TempVec2) < 0.1f) {
				DefUp = bsm::Vec3(0, 0, 1.0f);
			}
			Temp.normalize();
			RotMatrix = XMMatrixLookAtLH(bsm::Vec3(0, 0, 0), Temp, DefUp);
			RotMatrix = bsm::inverse(RotMatrix);
			Qt = RotMatrix.quatInMatrix();
			Qt.normalize();
		}
		break;
		case ParticleDrawOption::Faceing:
		{
			Temp = WorldPos - CameraEye;
			Vec2 TempVec2(Temp.x, Temp.z);
			if (bsm::length(TempVec2) < 0.1f) {
				DefUp = bsm::Vec3(0, 0, 1.0f);
			}
			RotMatrix = XMMatrixLookAtLH(bsm::Vec3(0, 0, 0), Temp, DefUp);
			RotMatrix = bsm::inverse(RotMatrix);
			Qt = RotMatrix.quatInMatrix();
			Qt.normalize();
		}
		break;
		case ParticleDrawOption::FaceingY:
			Temp = WorldPos - CameraEye;
			Temp.normalize();
			Qt = XMQuaternionRotationRollPitchYaw(0, atan2(Temp.x, Temp.z), 0);
			Qt.normalize();
			break;
		case ParticleDrawOption::Normal:
			Qt = rParticleSprite.m_LocalQt;
			Qt.normalize();
			break;
		}

		bsm::Mat4x4 matrix;
		matrix.affineTransformation(
			LocalScale,
			Vec3(0, 0, 0),
			Qt,
			WorldPos
		);
		DrawParticleSprite Item;
		Item.m_ToCaneraLength = ToCaneraLength;
		Item.m_WorldMatrix = matrix;
		Item.m_TextureRes = TextureRes;
		Item.m_Diffuse = rParticleSprite.m_Color;
		m_DrawParticleSpriteVec.push_back(Item);
	}

	void ParticleDrawObject::OnDraw() {
		auto PtrGameStage = GetStage<GameStage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		if (m_DrawParticleSpriteVec.size() <= 0) {
			return;
		}

		//�J�����ʒu�Ń\�[�g
		auto func = [](DrawParticleSprite& Left, DrawParticleSprite& Right)->bool {
			return (Left.m_ToCaneraLength > Right.m_ToCaneraLength);
		};
		std::sort(m_DrawParticleSpriteVec.begin(), m_DrawParticleSpriteVec.end(), func);
		//�擪�̃e�N�X�`���𓾂�
		auto  NowTexPtr = m_DrawParticleSpriteVec[0].m_TextureRes;
		size_t NowStartIndex = 0;
		size_t NowDrawCount = 0;

		shared_ptr<TextureResource> NowTexRes = m_DrawParticleSpriteVec[0].m_TextureRes;
		bsm::Col4 NowDiffuse = bsm::Col4(1, 1, 1, 1);
		for (size_t i = 0; i < m_DrawParticleSpriteVec.size(); i++) {
			if (m_DrawParticleSpriteVec[i].m_TextureRes != NowTexRes ||
				m_DrawParticleSpriteVec[i].m_Diffuse != NowDiffuse) {
				m_InstancVec.push_back(InstanceDrawStr(NowStartIndex, NowDrawCount, NowTexRes, NowDiffuse));
				NowStartIndex = i;
				NowDrawCount = 0;
				NowTexRes = m_DrawParticleSpriteVec[i].m_TextureRes;
				NowDiffuse = m_DrawParticleSpriteVec[i].m_Diffuse;
			}
			else {
				NowDrawCount++;
			}
		}
		m_InstancVec.push_back(InstanceDrawStr(NowStartIndex, NowDrawCount, NowTexRes, NowDiffuse));


		//�C���X�^���X�o�b�t�@�Ƀ}�b�v
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//�s��̃}�b�v
		if (FAILED(pD3D11DeviceContext->Map(m_MatrixBuffer.Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map���s
			throw BaseException(
				L"�s���Map�Ɏ��s���܂����B",
				L"if(FAILED(pD3D11DeviceContext->Map()))",
				L"ParticleDrawObject::OnDraw()"
			);
		}
		//�s��̕ύX
		auto* matrices = (bsm::Mat4x4*)mappedBuffer.pData;
		bsm::Mat4x4 World;
		for (size_t i = 0; i < m_DrawParticleSpriteVec.size(); i++) {
			World = m_DrawParticleSpriteVec[i].m_WorldMatrix;
			//�]�u����
			World.transpose();
			matrices[i] = World;
		}
		//�A���}�b�v
		pD3D11DeviceContext->Unmap(m_MatrixBuffer.Get(), 0);
		//�X�g���C�h�ƃI�t�Z�b�g
		//�`��̒��_�o�b�t�@�ƍs��o�b�t�@��ݒ�
		UINT stride[2] = { sizeof(VertexPositionColorTexture), sizeof(bsm::Mat4x4) };
		UINT offset[2] = { 0, 0 };

		ID3D11Buffer* pBuf[2] = { m_MeshResource->GetVertexBuffer().Get(), m_MatrixBuffer.Get() };
		pD3D11DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(m_MeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSPCTStaticInstance::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCTStatic::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPCTStaticInstance::GetPtr()->GetInputLayout());

		//�u�����h�X�e�[�g
		if (m_AddType) {
			//���Z����
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
		}
		else {
			//��������
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		}
		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthRead(), 0);
		//�e�N�X�`���ƃT���v���[�̐ݒ�
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//�r���[�s��̌���
		Mat4x4 View, Proj;
		PtrGameStage->GetCamera().GetViewProjMatrix(View, Proj);
		//�]�u����
		View.transpose();
		//�]�u����
		Proj.transpose();


		//�R���X�^���g�o�b�t�@�̏���
		PCTStaticConstants sb;
		sb.World = bsm::Mat4x4();	//���[���h�s��̓_�~�[
		sb.View = View;
		sb.Projection = Proj;
		//�G�~�b�V�u���Z�͍s��Ȃ��B
		sb.Emissive = bsm::Col4(0, 0, 0, 0);

		for (auto& v : m_InstancVec) {
			sb.Diffuse = v.m_Diffuse;
			//�R���X�^���g�o�b�t�@�̍X�V
			pD3D11DeviceContext->UpdateSubresource(CBPCTStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBPCTStatic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

			pD3D11DeviceContext->PSSetShaderResources(0, 1, v.Tex->GetShaderResourceView().GetAddressOf());
			//�`��
			pD3D11DeviceContext->DrawIndexedInstanced(m_MeshResource->GetNumIndicis(), v.Count, 0, 0, v.Start);
		}
		//��n��
		Dev->InitializeStates();
		m_DrawParticleSpriteVec.clear();
		m_InstancVec.clear();
	}



}
//end basecross
