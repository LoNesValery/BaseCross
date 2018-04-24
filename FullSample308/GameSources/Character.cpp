/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//class MultiSpark : public MultiParticle;
	//�p�r: �����̃X�p�[�N�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiSpark::MultiSpark(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiSpark::~MultiSpark() {}

	//������
	void MultiSpark::OnCreate() {
	}


	void MultiSpark::InsertSpark(const Vec3& Pos) {
		auto ParticlePtr = InsertParticle(4);
		ParticlePtr->SetEmitterPos(Pos);
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->SetMaxTime(0.1f);
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//�F�̎w��
			rParticleSprite.m_Color = Col4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}


	//--------------------------------------------------------------------------------------
	///	���C��
	//--------------------------------------------------------------------------------------
	ActionLine::ActionLine(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& EndtPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_EndPos(EndtPos)
	{}

	//������
	void ActionLine::OnCreate() {
		GetStage()->SetSharedGameObject(L"ActionLine", GetThis<ActionLine>());

		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vec3(1.0f));
		PtrTrans->SetQuaternion(Quat());
		PtrTrans->SetPosition(Vec3(0.0f));

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<PCStaticDraw>();
		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(m_StartPos,  Col4(1.0f, 1.0f,0.0f,1.0f)) },
			{ VertexPositionColor(m_EndPos,  Col4(1.0f, 1.0f,0.0f,1.0f)) }

		};
		vector<uint16_t> indices = {
			0, 1
		};
		auto MeshRes = MeshResource::CreateMeshResource(vertices, indices, true);
		MeshRes->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		PtrDraw->SetOriginalMeshResource(MeshRes);
		PtrDraw->SetOriginalMeshUse(true);

	}

	void ActionLine::OnUpdate() {


		auto PtrDraw = GetComponent<PCStaticDraw>();
		const vector<VertexPositionColor>& BackupVec = PtrDraw->GetOriginalMeshResource()->GetBackupVerteces<VertexPositionColor>();
		vector<VertexPositionColor> new_vec;
		VertexPositionColor new_v;
		new_v = BackupVec[0];
		new_v.position = m_StartPos;
		new_vec.push_back(new_v);

		new_v = BackupVec[1];
		new_v.position = m_EndPos;
		new_vec.push_back(new_v);

		PtrDraw->UpdateVertices(new_vec);

	}


	//--------------------------------------------------------------------------------------
	//	�^�C�����O����v���[�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	TilingPlate::TilingPlate(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position,
		float UPic,
		float VPic
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position),
		m_UPic(UPic),
		m_VPic(VPic)
	{
		try {
			if (m_UPic <= 0.0f || m_VPic <= 0.0f) {
				throw BaseException(
					L"UV��0�ȉ��͓n���܂���",
					L"if (m_UPic <= 0.0f || m_VPic <= 0.0f)",
					L"TilingPlate::TilingPlate()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	TilingPlate::~TilingPlate() {

	}
	//������
	void TilingPlate::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetQuaternion(m_Qt);
		PtrTrans->SetPosition(m_Position);
		AddComponent<CollisionRect>();

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSquare(1.0f, vertices, indices);
		float UCount = m_Scale.x / m_UPic;
		float VCount = m_Scale.y / m_VPic;
		for (size_t i = 0; i < vertices.size(); i++) {
			if (vertices[i].textureCoordinate.x >= 1.0f) {
				vertices[i].textureCoordinate.x = UCount;
			}
			if (vertices[i].textureCoordinate.y >= 1.0f) {
				vertices[i].textureCoordinate.y = VCount;
			}
		}
		//�`��R���|�[�l���g�̒ǉ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//�����ɉe���f�肱�ނ悤�ɂ���
		PtrDraw->SetOwnShadowActive(true);
		//�`��R���|�[�l���g�e�N�X�`���̐ݒ�
		PtrDraw->SetTextureResource(L"SKY_TX");
		//�^�C�����O�ݒ�
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
	}


	TilingFixedBox::TilingFixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position,
		float UPic,
		float VPic
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position),
		m_UPic(UPic),
		m_VPic(VPic)
	{}
	TilingFixedBox::~TilingFixedBox() {}
	//������
	void TilingFixedBox::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(m_Rotation);
		PtrTrans->SetPosition(m_Position);
		auto Coll = AddComponent<CollisionObb>();
		Coll->SetFixed(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		float UCount = m_Scale.x / m_UPic;
		float VCount = m_Scale.y / m_VPic;
		for (size_t i = 0; i < vertices.size(); i++) {
			if (vertices[i].textureCoordinate.x >= 1.0f) {
				vertices[i].textureCoordinate.x = UCount;
			}
			if (vertices[i].textureCoordinate.y >= 1.0f) {
				vertices[i].textureCoordinate.y = VCount;
			}
		}
		//�`��R���|�[�l���g�̒ǉ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//�����ɉe���f�肱�ނ悤�ɂ���
		PtrDraw->SetOwnShadowActive(true);
		//�`��R���|�[�l���g�e�N�X�`���̐ݒ�
		PtrDraw->SetTextureResource(L"WALL_TX");
		//�^�C�����O�ݒ�
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);

	}

	//--------------------------------------------------------------------------------------
	//	�G
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Enemy::Enemy(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	):
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{}
	Enemy::~Enemy(){}
	//������
	void Enemy::OnCreate() {
		//���_�\���p�̃��b�V��
		m_BallMesh = MeshResource::CreateSphere(0.2f, 9);

		GetStage()->SetSharedGameObject(L"Enemy", GetThis<Enemy>());

		AddTag(L"Enemy");


		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(m_Scale);	
		Ptr->SetRotation(m_Rotation);
		Ptr->SetPosition(m_Position);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//�`�悷��e�N�X�`����ݒ�
		PtrDraw->SetTextureResource(L"CHECKER_TX");
	}

	bool Enemy::IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, bsm::Vec3& HitPoint) {
		auto PtrDraw = GetComponent<BcPNTStaticDraw>();
		auto PtrTrans = GetComponent<Transform>();
		TRIANGLE tri;
		return PtrDraw->GetMeshResource()->IsHitSegmentTriangles(PtrTrans->GetWorldMatrix(), StartPos, EndPos, HitPoint, tri);
	}

	vector<Vec3>&  Enemy::WorldTriangles() {
		auto PtrDraw = GetComponent<BcPNTStaticDraw>();
		auto PtrTrans = GetComponent<Transform>();
		PtrDraw->GetMeshResource()->GetDrawingPositions(m_WorldTriangles, PtrTrans->GetWorldMatrix());
		return m_WorldTriangles;
	}


	//--------------------------------------------------------------------------------------
	///	Bone�L����
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	BoneChara::BoneChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	BoneChara::~BoneChara() {}

	//������
	void BoneChara::OnCreate() {

		AddTag(L"Enemy");
		GetStage()->SetSharedGameObject(L"BoneChara", GetThis<BoneChara>());

		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.5f, 0.5f, 0.5f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		Mat4x4 SpanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		SpanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();

		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"Chara_R_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTBoneModelDraw>();
		PtrDraw->SetFogEnabled(true);
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"Chara_R_MESH");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);

		PtrDraw->AddAnimation(L"Default", 0, 100, true, 20.0f);
		PtrDraw->ChangeCurrentAnimation(L"Default");

		//��������
		SetAlphaActive(true);
	}
	//�X�V
	void BoneChara::OnUpdate() {
		//�A�j���[�V�������X�V����
		auto PtrDraw = GetComponent<BcPNTBoneModelDraw>();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		PtrDraw->UpdateAnimation(ElapsedTime);

	}

	//--------------------------------------------------------------------------------------
	///	Bone�O�p�`�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	BoneTriangles::BoneTriangles(const shared_ptr<Stage>& StagePtr, const shared_ptr<BoneChara>& boneChara):
		GameObject(StagePtr),
		m_BoneChara(boneChara)
	{}

	void BoneTriangles::OnCreate() {
		GetStage()->SetSharedGameObject(L"BoneTriangles", GetThis<BoneTriangles>());
		auto ShBone = m_BoneChara.lock();
		if (ShBone) {
			auto PtrBoneTrans = ShBone->GetComponent<Transform>();
			auto PtrTrans = GetComponent<Transform>();
			PtrTrans->SetScale(PtrBoneTrans->GetScale());
			PtrTrans->SetQuaternion(PtrBoneTrans->GetQuaternion());
			PtrTrans->SetPosition(PtrBoneTrans->GetPosition());

			auto PtrBoneDraw = ShBone->GetComponent<BcPNTBoneModelDraw>();
			PtrBoneDraw->GetSkinedPositions(m_TempPosvec);
			if (m_TempPosvec.size() % 3) {
				return;
			}
			//�`��R���|�[�l���g
			auto PtrDraw = AddComponent<PCStaticDraw>();
			vector<uint16_t> indices;
			for (auto& v : m_TempPosvec) {
				VertexPositionColor poscol;
				poscol.position = v;
				poscol.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				m_BackupVertices.push_back(poscol);
			}
			for (size_t i = 0; i < m_BackupVertices.size(); i++) {
				indices.push_back((uint16_t)i);
			}
			m_TriangleMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);
			PtrDraw->SetMeshResource(m_TriangleMesh);
			PtrDraw->SetRasterizerState(RasterizerState::Wireframe);
		}
	}
	void BoneTriangles::OnUpdate() {
		auto ShBone = m_BoneChara.lock();
		if (ShBone) {
			auto PtrBoneDraw = ShBone->GetComponent<BcPNTBoneModelDraw>();
			PtrBoneDraw->GetSkinedPositions(m_TempPosvec);
			m_BackupVertices.clear();
			for (auto& v : m_TempPosvec) {
				VertexPositionColor poscol;
				poscol.position = v;
				poscol.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				m_BackupVertices.push_back(poscol);
			}
			m_TriangleMesh->UpdateVirtexBuffer(m_BackupVertices);
		}
	}

	bool BoneTriangles::IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, bsm::Vec3& HitPoint) {
		auto PtrTrans = GetComponent<Transform>();
		Mat4x4 Worldmat = PtrTrans->GetWorldMatrix();
		vector<Vec3> ChkPosvec(m_TempPosvec.size());


		for (size_t i = 0; i < m_TempPosvec.size();i++) {
			ChkPosvec[i] = m_TempPosvec[i] * Worldmat;
		}

		for (size_t i = 0; i < m_TempPosvec.size(); i += 3) {
			TRIANGLE tri;
			tri.m_A = ChkPosvec[i];
			tri.m_B = ChkPosvec[i + 1];
			tri.m_C = ChkPosvec[i + 2];
			bsm::Vec3 ret;
			float t;
			if (HitTest::SEGMENT_TRIANGLE(StartPos, EndPos, tri, ret, t)) {
				auto Len = length(EndPos - StartPos);
				Len *= t;
				auto Nomal = EndPos - StartPos;
				Nomal.normalize();
				Nomal *= Len;
				HitPoint = StartPos + Nomal;
				auto HitTrianglesPtr = GetStage()->GetSharedGameObject<HitTriangles>(L"HitTriangles");
				HitTrianglesPtr->AddHitTriangle(tri);
				return true;
			}
		}
		return false;
	}

	//--------------------------------------------------------------------------------------
	///	�q�b�g���̎O�p�`�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	HitTriangles::HitTriangles(const shared_ptr<Stage>& StagePtr):
		GameObject(StagePtr)
	{}

	void HitTriangles::OnCreate() {
		SetDrawLayer(2);
		GetStage()->SetSharedGameObject(L"HitTriangles", GetThis<HitTriangles>());
		vector<VertexPositionColor> vertices = {
			{ Vec3(0.0f, 0.5f , 0.0f),Col4(1.0f) },
			{ Vec3(0.5f, -0.5f, 0.5f),Col4(1.0f) },
			{ Vec3(-0.5f, -0.5f, 0.5f),Col4(1.0f) }
		};
		m_TriangleMesh = MeshResource::CreateMeshResource(vertices, true);
	}

	void HitTriangles::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto& v : m_DrawTriangleVec) {
			if (v.m_LastTime > 0.0f) {
				v.m_LastTime -= ElapsedTime;
			}
		}
	}

	void HitTriangles::AddHitTriangle(const TRIANGLE& tri) {
		for (auto& v : m_DrawTriangleVec) {
			if (v.m_LastTime <= 0.0f) {
				v.m_Triangle = tri;
				v.m_LastTime = 1.0f;
				return;
			}
		}
		DrawTriangle d_tri(tri, 1.0f);
		m_DrawTriangleVec.push_back(d_tri);
	}

	void HitTriangles::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();
//		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAdditive(), nullptr, 0xffffffff);
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
		//�e�N�X�`���ƃT���v���[
		//�V�F�[�_�[���\�[�X���N���A
		//NULL�̃V�F�[�_���\�[�X�̏���
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
		//�T���v���[���N���A
		RenderStatePtr->SetSamplerAllClear(pID3D11DeviceContext);
		//���_�V�F�[�_
		pID3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
		//�s�N�Z���V�F�[�_
		pID3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//�ʏ���
		SimpleConstants SmCb;
		//�R���X�^���g�o�b�t�@�̍쐬
		//�J�����𓾂�
		auto CameraPtr = OnGetDrawCamera();
		//�r���[�Ǝˉe�s��𓾂�
		auto ViewMat = CameraPtr->GetViewMatrix();
		//�]�u����
		ViewMat.transpose();
		//�]�u����
		auto ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.transpose();
		//SmCb.World = World;
		SmCb.View = ViewMat;
		SmCb.Projection = ProjMat;
		//�G�~�b�V�u
		SmCb.Emissive = Col4(1, 1, 1, 1.0);
		//�f�t�B�[�Y
		SmCb.Diffuse = Col4(1, 1, 1, 1.0);
		//�X�y�L�����[
		SmCb.Specular = Col4(0.4, 0.4, 0.4, 1.0);
		//���C�e�B���O
		auto StageLight = OnGetDrawLight();
		SmCb.LightDir = StageLight.m_Directional;
		SmCb.LightDir.w = 1.0f;
		SmCb.EyePos = CameraPtr->GetEye();
		SmCb.EyePos.w = 1.0f;
		//�e�N�X�`��
		SmCb.ActiveFlg.x = 0;

		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = m_TriangleMesh->GetNumStride();
		UINT offset = 0;
		//�`����@�̃Z�b�g
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//���_�o�b�t�@�̃Z�b�g
//		pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_TriangleMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
//		pID3D11DeviceContext->IASetIndexBuffer(m_BallMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		for (auto& v : m_DrawTriangleVec) {
			if (v.m_LastTime > 0.0f) {
				vector<VertexPositionColor> vertices = {
					{ v.m_Triangle.m_A,Col4(0.0f,0.0f,1.0f,1.0f) },
					{ v.m_Triangle.m_B,Col4(0.0f,0.0f,1.0f,1.0f) },
					{ v.m_Triangle.m_C,Col4(0.0f,0.0f,1.0f,1.0f) }
				};
				m_TriangleMesh->UpdateVirtexBuffer(vertices);

				//���_�o�b�t�@�̃Z�b�g
				pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_TriangleMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				Mat4x4 World;
				World.identity();
				World.transpose();
				SmCb.World = World;
				//�R���X�^���g�o�b�t�@�̍X�V
				pID3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &SmCb, 0, 0);
				//�R���X�^���g�o�b�t�@�̐ݒ�
				ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
				//���_�V�F�[�_�ɓn��
				pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				//�s�N�Z���V�F�[�_�ɓn��
				pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
				//�`��
				pID3D11DeviceContext->Draw(3, 0);
			}
		}
		//��n��
		Dev->InitializeStates();
		m_DrawTriangleVec.clear();
	}




}
//end basecross
