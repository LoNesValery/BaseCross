/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	���ʎ���
	//--------------------------------------------------------------------------------------
	SquareObject::SquareObject(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	SquareObject::~SquareObject() {}

	void SquareObject::CreateBuffers(float WrapX, float WrapY) {
		float HelfSize = 0.5f;
		vector<VertexPositionNormalTexture> vertices = {
			{ VertexPositionNormalTexture(Vec3(-HelfSize, HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vec3(HelfSize, HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(WrapX, 0.0f)) },
			{ VertexPositionNormalTexture(Vec3(-HelfSize, -HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(0.0f, WrapY)) },
			{ VertexPositionNormalTexture(Vec3(HelfSize, -HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(WrapX, WrapY)) },

		};

		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2,
		};
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_SquareMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}


	void SquareObject::OnCreate() {
		CreateBuffers(m_Scale.x, m_Scale.y);
	}
	void SquareObject::OnUpdate() {
	}
	void SquareObject::OnDraw() {
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto PtrGameStage = GetStage<GameStage>();
		//�s��̒�`
		auto World = (Mat4x4)XMMatrixAffineTransformation(
			m_Scale, 
			Vec3(0, 0, 0),
			m_Qt, 
			m_Pos
		);
		PtrGameStage->GetPNTDrawObject()->AddDrawMesh(
			m_SquareMesh,
			TexPtr,
			World,
			false,
			true
		);
	}



	//--------------------------------------------------------------------------------------
	///	������
	//--------------------------------------------------------------------------------------
	SphereObject::SphereObject(const shared_ptr<Stage>& StagePtr,
		UINT Division, const wstring& TextureResName, bool Trace, const Vec3& Pos) :
		GameObject(StagePtr),
		m_Division(Division),
		m_TextureResName(TextureResName),
		m_Trace(Trace),
		m_Scale(0.25f, 0.25f, 0.25f),
		m_BaseY(m_Scale.y / 2.0f),
		m_Qt(),
		m_Pos(Pos),
		m_Velocity(0,0,0),
		m_Gravity(0,-9.8f,0),
		m_GravityVelocity(0,0,0),
		m_JumpLock(false),
		m_BeforePos(Pos),
		m_Mass(1.0f)
	{}
	SphereObject::~SphereObject() {}

	Vec3 SphereObject::GetMoveVector() const {
		Vec3 Angle(0, 0, 0);
		auto PtrGameStage = GetStage<GameStage>();
		Vec3 CameraEye, CameraAt;
		PtrGameStage->GetCamera().GetCameraEyeAt(CameraEye, CameraAt);

		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//���������̃X�s�[�h
										//�i�s�����̌������v�Z
				Vec3 Front = m_Pos - CameraEye;
				Front.y = 0;
				Front.normalize();
				//�i�s������������̊p�x���Z�o
				float FrontAngle = atan2(Front.z, Front.x);
				//�R���g���[���̌����v�Z
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				Vec2 MoveVec(MoveX, MoveZ);
				float MoveSize = length(MoveVec);
				//�R���g���[���̌�������p�x���v�Z
				float CntlAngle = atan2(-MoveX, MoveZ);
				//�g�[�^���̊p�x���Z�o
				float TotalAngle = FrontAngle + CntlAngle;
				//�p�x����x�N�g�����쐬
				Angle = Vec3(cos(TotalAngle), 0, sin(TotalAngle));
				//���K������
				Angle.normalize();
				//�ړ��T�C�Y��ݒ�B
				Angle *= MoveSize;
				//Y���͕ω������Ȃ�
				Angle.y = 0;
			}
		}
		return Angle;
	}


	SPHERE SphereObject::GetSPHERE()const {
		SPHERE sp;
		sp.m_Center = (Vector3)m_Pos;
		sp.m_Radius =  m_Scale.y * 0.5f;
		return sp;
	}

	void SphereObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, m_Division,vertices, indices);
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}
	void SphereObject::OnUpdate() {
		//1�O�̈ʒu������Ă���
		m_BeforePos = m_Pos;
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (!m_JumpLock) {
				//A�{�^��
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					m_BeforePos.y += 0.01f;
					m_Pos.y += 0.01f;
					m_GravityVelocity = Vec3(0, 4.0f, 0);
					m_JumpLock = true;
				}
			}
			Vec3 Direction = GetMoveVector();
			if (length(Direction) < 0.1f) {
				m_Velocity *= 0.9f;
			}
			else {
				m_Velocity = Direction * 5.0f;
			}
		}
		m_Pos += (m_Velocity * ElapsedTime);
		m_GravityVelocity += m_Gravity * ElapsedTime;
		m_Pos += m_GravityVelocity * ElapsedTime;
		if (m_Pos.y <= m_BaseY) {
			m_Pos.y = m_BaseY;
			m_GravityVelocity = Vec3(0, 0, 0);
			m_JumpLock = false;
		}
		OnRotation();
	}


	void SphereObject::RotToHead(float LerpFact) {
		if (LerpFact <= 0.0f) {
			//��ԌW����0�ȉ��Ȃ牽�����Ȃ�
			return;
		}
		//��]�̍X�V
		//Velocity�̒l�ŁA��]��ύX����
		Vec3 Temp = m_Velocity;
		Temp.normalize();
		float ToAngle = atan2(Temp.x, Temp.z);
		Quat Qt(XMQuaternionRotationRollPitchYaw(0, ToAngle, 0));
		Qt.normalize();
		//���݂ƖڕW����
		if (LerpFact >= 1.0f) {
			m_Qt = Qt;
		}
		else {
			m_Qt = XMQuaternionSlerp(m_Qt, Qt, LerpFact);
		}
	}

	void SphereObject::OnRotation() {
		//��]
		RotToHead(0.1f);
	}



	void SphereObject::OnDraw() {
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto PtrGameStage = GetStage<GameStage>();
		//���[���h�s��̌���
		auto World = (Mat4x4)XMMatrixAffineTransformation(m_Scale, Vec3(0, 0, 0),
			m_Qt, m_Pos);
		PtrGameStage->GetPNTDrawObject()->AddDrawMesh(
			m_SphereMesh,
			TexPtr,
			World,
			true
		);
	}


	//--------------------------------------------------------------------------------------
	///	PNT���_�I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	PNTDrawObject::PNTDrawObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	PNTDrawObject::~PNTDrawObject() {}

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
		m_DrawObjectVec.push_back(Obj);
	}

	void PNTDrawObject::OnUpdate() {
		m_DrawObjectVec.clear();
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
		//�T���v���[�̏���
		ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
		ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//�s��̒�`
		Mat4x4 View, Proj;
		//���C�e�B���O
		Vec4 LightDir;
		PtrGameStage->GetCamera().GetViewProjMatrix(View, Proj);
		PtrGameStage->GetLightDir(LightDir);
		//�r���[�s��̌���
		//�]�u����
		View = transpose(View);
		//�ˉe�s��̌���
		//�]�u����
		Proj = transpose(Proj);
		//�R���X�^���g�o�b�t�@�̏���
		PNTStaticConstantBuffer sb;
		sb.View = View;
		sb.Projection = Proj;
		sb.LightDir = LightDir;
		//�f�B�t���[�Y
		sb.Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		//�G�~�b�V�u���Z�B
		sb.Emissive = Col4(0.4f, 0.4f, 0.4f, 0);
		//�ʏ���
		for (auto& v : m_DrawObjectVec) {
			//�]�u����
			v.m_WorldMatrix = transpose(v.m_WorldMatrix);
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
		//��n��
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	///	���b�s���O�������ꂽ�X�v���C�g
	//--------------------------------------------------------------------------------------
	SpriteBase::SpriteBase(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, 
		const Vec2& StartScale, 
		float StartRot,
		const Vec2& StartPos,
		UINT XWrap, UINT YWrap) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Scale(StartScale),
		m_Rot(StartRot),
		m_Pos(StartPos),
		m_XWrap(XWrap),
		m_YWrap(YWrap),
		m_Emissive(0,0,0,0),
		m_BlendState(BlendState::Opaque)
	{}
	void SpriteBase::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��(�c���w�萔���\��)
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, (float)m_YWrap)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, (float)m_YWrap)) },
		};
		//���_�̏����C���i���z�֐��Ăт����j
		AdjustVertex();
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//���b�V���̍쐬�i�ύX�ł���j
		m_SquareMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);
	}

	void SpriteBase::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//���_�̕ύX
		//D3D11_MAP_WRITE_DISCARD�͏d�v�B���̏����ɂ��AGPU�Ɏז�����Ȃ�
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//���_�̃}�b�v
		if (FAILED(pD3D11DeviceContext->Map(m_SquareMesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map���s
			throw BaseException(
				L"���_��Map�Ɏ��s���܂����B",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"WrappedSprite::UpdateVertex()"
			);
		}
		//���_�̕ύX
		VertexPositionColorTexture* vertices
			= (VertexPositionColorTexture*)mappedBuffer.pData;
		//���z�֐��Ăяo��
		UpdateVertex(ElapsedTime, vertices);
		//�A���}�b�v
		pD3D11DeviceContext->Unmap(m_SquareMesh->GetVertexBuffer().Get(), 0);
	}



	void SpriteBase::OnDraw() {
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//���[���h�s��̌���
		Mat4x4 World(XMMatrixAffineTransformation2D(
			m_Scale,			//�X�P�[�����O
			Vec2(0, 0),		//��]�̒��S�i�d�S�j
			m_Rot,				//��]�p�x
			m_Pos				//�ʒu
		));
		//�ˉe�s��̌���
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Mat4x4 Proj(XMMatrixOrthographicLH(w, h, -1.0, 1.0f));
		//�s��̍���
		World = Proj * World;
		//�R���X�^���g�o�b�t�@�̏���
		SpriteConstantBuffer sb;
		//�G�~�b�V�u���Z�B
		sb.Emissive = m_Emissive;
		//�s��̐ݒ�
		sb.World = World;
		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionColorTexture);
		UINT offset = 0;
		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_SquareMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(m_SquareMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//���_�V�F�[�_�ɓn��
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�s�N�Z���V�F�[�_�ɓn��
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPCTSprite::GetPtr()->GetInputLayout());
		//�u�����h�X�e�[�g
		switch (m_BlendState) {
		case BlendState::Opaque:
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
			break;
		case BlendState::Trace:
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			break;
		case BlendState::Additive:
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
			break;
		}
		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthNone(), 0);
		//���X�^���C�U�X�e�[�g
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//�e�N�X�`���ƃT���v���[�̐ݒ�
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, TexPtr->GetShaderResourceView().GetAddressOf());
		//���b�s���O�T���v���[
		ID3D11SamplerState* pSampler = RenderState->GetLinearWrap();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		//�`��
		pD3D11DeviceContext->DrawIndexed(m_SquareMesh->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates();
	}
	//--------------------------------------------------------------------------------------
	///	��]����X�v���C�g
	//--------------------------------------------------------------------------------------
	RotateSprite::RotateSprite(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, 
		const Vec2& StartScale,
		float StartRot,
		const Vec2& StartPos,
		UINT XWrap, UINT YWrap):
		SpriteBase(StagePtr, TextureResName, StartScale, StartRot, StartPos, XWrap, YWrap),
		m_TotalTime(0)
	{
		SetBlendState(BlendState::Additive);
	}

	void RotateSprite::AdjustVertex() {
		//���_�F��ύX����
		for (size_t i = 0; i < m_BackupVertices.size();i++) {
			switch (i) {
			case 0:
				m_BackupVertices[i].color = Col4(1.0f, 0.0f, 0.0f, 1.0f);
				break;
			case 1:
				m_BackupVertices[i].color = Col4(0.0f, 1.0f, 0.0f, 1.0f);
				break;
			case 2:
				m_BackupVertices[i].color = Col4(0.0f, 0.0f, 1.0f, 1.0f);
				break;
			case 3:
				m_BackupVertices[i].color = Col4(1.0f, 1.0f, 0, 1.0);
				break;
			}
		}
	}

	void RotateSprite::UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) {
		m_Rot += ElapsedTime;
		if (m_Rot >= XM_2PI) {
			m_Rot = 0;
		}
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= 1.0f) {
			m_TotalTime = 0;
		}
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			Vec2 UV = m_BackupVertices[i].textureCoordinate;
			if (UV.x == 0.0f) {
				UV.x = m_TotalTime;
			}
			else if (UV.x == 4.0f) {
				UV.x += m_TotalTime;
			}
			vertices[i] = VertexPositionColorTexture(
				m_BackupVertices[i].position,
				m_BackupVertices[i].color,
				UV
			);
		}


	}


	//--------------------------------------------------------------------------------------
	///	���b�Z�[�W��\������X�v���C�g
	//--------------------------------------------------------------------------------------
	MessageSprite::MessageSprite(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName,
		const Vec2& StartScale,
		float StartRot,
		const Vec2& StartPos,
		UINT XWrap, UINT YWrap) :
		SpriteBase(StagePtr, TextureResName, StartScale, StartRot, StartPos, XWrap, YWrap),
		m_TotalTime(0)
	{
		SetBlendState(BlendState::Trace);
	}

	void MessageSprite::AdjustVertex() {
		//�����ł͉������Ȃ�
	}

	void  MessageSprite::UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) {
		m_TotalTime += (ElapsedTime * 5.0f);
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0;
		}
		float sin_val = sin(m_TotalTime) * 0.5f + 0.5f;
		Col4 UpdateCol(1.0f, 1.0f, 1.0f, sin_val);
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			vertices[i] = VertexPositionColorTexture(
				m_BackupVertices[i].position,
				UpdateCol,
				m_BackupVertices[i].textureCoordinate
			);

		}
	}



}
//end basecross
