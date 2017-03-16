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


	void MultiSpark::InsertSpark(const Vector3& Pos) {
		auto ParticlePtr = InsertParticle(4);
		ParticlePtr->SetEmitterPos(Pos);
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->SetMaxTime(0.5f);
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vector3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//�F�̎w��
			rParticleSprite.m_Color = Color4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	//--------------------------------------------------------------------------------------
	//class MultiFire : public MultiParticle;
	//�p�r: �����̉��N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiFire::MultiFire(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiFire::~MultiFire() {}

	//������
	void MultiFire::OnCreate() {
		//���Z�`�揈��������
		SetAddType(true);
	}

	void MultiFire::InsertFire(const Vector3& Pos) {
		auto ParticlePtr = InsertParticle(4);
		ParticlePtr->SetEmitterPos(Pos);
		ParticlePtr->SetTextureResource(L"FIRE_TX");
		ParticlePtr->SetMaxTime(0.5f);
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vector3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//�F�̎w��
			rParticleSprite.m_Color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}


	//--------------------------------------------------------------------------------------
	///	�������̃X�v���C�g
	//--------------------------------------------------------------------------------------
	TraceSprite::TraceSprite(const shared_ptr<Stage>& StagePtr, bool Trace,
		const Vector2& StartScale, const Vector3& StartPos):
		GameObject(StagePtr),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}
	TraceSprite::~TraceSprite() {}
	void TraceSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��
		m_BackupVertices = {
			{ VertexPositionColor(Vector3(-HelfSize, HelfSize, 0),Color4(1.0f,0.0f,0.0f,0.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, HelfSize, 0), Color4(0.0f, 1.0f, 0.0f, 0.0f)) },
			{ VertexPositionColor(Vector3(-HelfSize, -HelfSize, 0), Color4(0.0f, 0.0f, 1.0f, 0.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, -HelfSize, 0), Color4(0.0f, 0.0f, 0, 0.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y, 0.0f);
		//���_�ƃC���f�b�N�X���w�肵�ăX�v���C�g�쐬
		auto PtrDraw = AddComponent<PCSpriteDraw>(m_BackupVertices, indices);
	}
	void TraceSprite::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= XM_PI) {
			m_TotalTime = 0;
		}
		vector<VertexPositionColor> NewVertices;
		for (size_t i = 0; i < m_BackupVertices.size(); i++) {
			Color4 col = m_BackupVertices[i].color;
			col.w = sin(m_TotalTime);
			auto v = VertexPositionColor(
				m_BackupVertices[i].position,
				col
			);
			NewVertices.push_back(v);
		}
		auto PtrDraw = GetComponent<PCSpriteDraw>();
		PtrDraw->UpdateVertices(NewVertices);

	}


	//--------------------------------------------------------------------------------------
	///	�ǖ͗l�̃X�v���C�g
	//--------------------------------------------------------------------------------------
	WallSprite::WallSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vector2& StartScale, const Vector2& StartPos):
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos)
	{}

	WallSprite::~WallSprite() {}
	void WallSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��(�c��5���\��)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vector3(-HelfSize, HelfSize, 0),Color4(1.0f,1.0f,1.0f,1.0f), Vector2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vector3(HelfSize, HelfSize, 0), Color4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(5.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vector3(-HelfSize, -HelfSize, 0), Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 5.0f)) },
			{ VertexPositionColorTexture(Vector3(HelfSize, -HelfSize, 0), Color4(0.0f, 0.0f, 0, 1.0f), Vector2(5.0f, 5.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y, 0.0f);
		//���_�ƃC���f�b�N�X���w�肵�ăX�v���C�g�쐬
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_TextureKey);
	}



	//--------------------------------------------------------------------------------------
	///	�X�N���[������X�v���C�g
	//--------------------------------------------------------------------------------------
	ScrollSprite::ScrollSprite(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureKey, bool Trace, 
		const Vector2& StartScale, const Vector2& StartPos):
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}

	ScrollSprite::~ScrollSprite() {}
	void ScrollSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��
		m_BackupVertices = {
			{ VertexPositionTexture(Vector3(-HelfSize, HelfSize, 0), Vector2(0.0f, 0.0f)) },
			{ VertexPositionTexture(Vector3(HelfSize, HelfSize, 0), Vector2(4.0f, 0.0f)) },
			{ VertexPositionTexture(Vector3(-HelfSize, -HelfSize, 0), Vector2(0.0f, 1.0f)) },
			{ VertexPositionTexture(Vector3(HelfSize, -HelfSize, 0), Vector2(4.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y,1.0f);
		PtrTransform->SetRotation(0,0,0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y,0.0f);
		//���_�ƃC���f�b�N�X���w�肵�ăX�v���C�g�쐬
		auto PtrDraw = AddComponent<PTSpriteDraw>(m_BackupVertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_TextureKey);
	}
	void ScrollSprite::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime > 1.0f) {
			m_TotalTime = 0;
		}
		vector<VertexPositionTexture> NewVertices;
		for (size_t i = 0; i < m_BackupVertices.size(); i++) {
			Vector2 UV = m_BackupVertices[i].textureCoordinate;
			if (UV.x == 0.0f) {
				UV.x = m_TotalTime;
			}
			else if (UV.x == 4.0f) {
				UV.x += m_TotalTime;
			}
			auto v = VertexPositionTexture(
				m_BackupVertices[i].position,
				UV
			);
			NewVertices.push_back(v);
		}
		auto PtrDraw = GetComponent<PTSpriteDraw>();
		PtrDraw->UpdateVertices(NewVertices);
	}


	//--------------------------------------------------------------------------------------
	///	����ŉ�]���闧����
	//--------------------------------------------------------------------------------------
	RollingCube::RollingCube(const shared_ptr<Stage>& StagePtr, bool Trace,
		const Vector3& StartScale, const Quaternion& StartQt, const Vector3& StartPos):
		GameObject(StagePtr),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartQt(StartQt),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}

	RollingCube::~RollingCube() {}

	void RollingCube::OnCreate() {
		SetAlphaActive(m_Trace);
		Viewport viewport;
		viewport.Width = static_cast<float>(App::GetApp()->GetGameWidth());
		viewport.Height = static_cast<float>(App::GetApp()->GetGameHeight());
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1.0f;
		m_Camera = ObjectFactory::Create<Camera>();
		m_Camera->SetViewPort(viewport);
		//���ߖ@����
		m_Camera->SetPers(false);
		m_Camera->CalculateMatrix();

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale);
		PtrTransform->SetQuaternion(m_StartQt);
		PtrTransform->SetPosition(m_StartPos);

		auto PtrDraw = AddComponent<PCStaticDraw>();

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f,vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionColor new_v;
			new_v.position = vertices[i].position;
			switch (i % 3) {
			case 0:
				new_v.color = Color4(1.0f, 0, 0, 1.0f);
				break;
			case 1:
				new_v.color = Color4(0.0f, 1.0, 0, 1.0f);
				break;
			case 2:
				new_v.color = Color4(0.0f, 0, 1.0, 1.0f);
				break;
			default:
				new_v.color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			}
			m_BackupVertices.push_back(new_v);

		}
		PtrDraw->CreateOriginalMesh(m_BackupVertices, indices);
		PtrDraw->SeOriginalMeshUse(true);

	}
	const shared_ptr<Camera>& RollingCube::OnGetDrawCamera() const {
		return m_Camera;
	}

	void RollingCube::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Quaternion QtSpan(Vector3(0, 1.0f, 0), ElapsedTime * 5.0f);

		auto PtrTransform = GetComponent<Transform>();
		auto Qt = PtrTransform->GetQuaternion();
		Qt *= QtSpan;
		PtrTransform->SetQuaternion(Qt);

		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= XM_PI) {
			m_TotalTime = 0;
		}
		for (size_t i = 0; i < m_BackupVertices.size(); i++) {
			m_BackupVertices[i].color.w = sin(m_TotalTime);
		}
		auto PtrDraw = GetComponent<PCStaticDraw>();
		PtrDraw->UpdateVertices(m_BackupVertices);
	}

	//--------------------------------------------------------------------------------------
	///	����ŉ�]����Wall������
	//--------------------------------------------------------------------------------------
	RollingWallCube::RollingWallCube(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vector3& StartScale, const Quaternion& StartQt, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartQt(StartQt),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}

	RollingWallCube::~RollingWallCube() {}

	void RollingWallCube::OnCreate() {
		SetAlphaActive(m_Trace);
		Viewport viewport;
		viewport.Width = static_cast<float>(App::GetApp()->GetGameWidth());
		viewport.Height = static_cast<float>(App::GetApp()->GetGameHeight());
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1.0f;
		m_Camera = ObjectFactory::Create<Camera>();
		m_Camera->SetViewPort(viewport);
		//���ߖ@����
		m_Camera->SetPers(false);
		m_Camera->CalculateMatrix();

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale);
		PtrTransform->SetQuaternion(m_StartQt);
		PtrTransform->SetPosition(m_StartPos);

		auto PtrDraw = AddComponent<PTStaticDraw>();


		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionTexture> new_vertices;

		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionTexture new_v;
			new_v.position = vertices[i].position;
			new_v.textureCoordinate = vertices[i].textureCoordinate;
			new_vertices.push_back(new_v);
		}
		m_MeshResource = MeshResource::CreateMeshResource(new_vertices, indices,false);

		PtrDraw->SetMeshResource(m_MeshResource);
		PtrDraw->SetTextureResource(m_TextureKey);

	}
	const shared_ptr<Camera>& RollingWallCube::OnGetDrawCamera() const {
		return m_Camera;
	}

	void RollingWallCube::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Quaternion QtSpan(Vector3(0, 1.0f, 0), ElapsedTime * 5.0f);

		auto PtrTransform = GetComponent<Transform>();
		auto Qt = PtrTransform->GetQuaternion();
		Qt *= QtSpan;
		PtrTransform->SetQuaternion(Qt);

		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= XM_PI) {
			m_TotalTime = 0;
		}
	}

	//--------------------------------------------------------------------------------------
	///	����������
	//--------------------------------------------------------------------------------------
	WhiteCube::WhiteCube(const shared_ptr<Stage>& StagePtr,
		const Vector3& StartScale, const Quaternion& StartQt, const Vector3& StartPos):
		GameObject(StagePtr),
		m_StartScale(StartScale),
		m_StartQt(StartQt),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}

	void WhiteCube::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionNormal> new_vertices;

		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionNormal new_v;
			new_v.position = vertices[i].position;
			new_v.normal = vertices[i].normal;
			new_vertices.push_back(new_v);
		}
		m_MeshResource = MeshResource::CreateMeshResource(new_vertices, indices, false);

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale);
		PtrTransform->SetQuaternion(m_StartQt);
		PtrTransform->SetPosition(m_StartPos);

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetMeshResource(m_MeshResource);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

	}
	void WhiteCube::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Quaternion QtSpan(Vector3(0, 1.0f, 0), ElapsedTime * 5.0f);

		auto PtrTransform = GetComponent<Transform>();
		auto Qt = PtrTransform->GetQuaternion();
		Qt *= QtSpan;
		PtrTransform->SetQuaternion(Qt);

		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= XM_PI) {
			m_TotalTime = 0;
		}

	}






	//--------------------------------------------------------------------------------------
	///	�`�󂪕ς�鋅��
	//--------------------------------------------------------------------------------------
	TransSphere::TransSphere(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vector3& StartScale, const Quaternion& StartQt, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartQt(StartQt),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}

	TransSphere::~TransSphere() {}

	void TransSphere::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale);
		PtrTransform->SetQuaternion(m_StartQt);
		PtrTransform->SetPosition(m_StartPos);



		auto PtrDraw = AddComponent<PCTStaticDraw>();

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f,18, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionColorTexture new_v;
			new_v.position = vertices[i].position;
			new_v.textureCoordinate = vertices[i].textureCoordinate;
			new_v.color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
			m_BackupVertices.push_back(new_v);
			m_RuntimeVertices.push_back(new_v);

		}
		PtrDraw->CreateOriginalMesh(m_BackupVertices, indices);
		PtrDraw->SeOriginalMeshUse(true);
		PtrDraw->SetTextureResource(m_TextureKey);
		SetAlphaActive(m_Trace);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(PtrDraw->GetMeshResource());


	}

	void TransSphere::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Quaternion QtSpan(Vector3(0, 1.0f, 0), ElapsedTime * 5.0f);

		auto PtrTransform = GetComponent<Transform>();
		auto Qt = PtrTransform->GetQuaternion();
		Qt *= QtSpan;
		PtrTransform->SetQuaternion(Qt);

		m_TotalTime += ElapsedTime * 5.0f;
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0;
		}
		for (size_t i = 0; i < m_BackupVertices.size(); i++) {
			auto  Pos = m_BackupVertices[i].position;
			auto Len = (sin(m_TotalTime) * 0.5f) + 1.0f;
			Pos.x *= Len;
			Pos.z *= Len;
			m_RuntimeVertices[i].position = Pos;
		}
		auto PtrDraw = GetComponent<PCTStaticDraw>();
		PtrDraw->UpdateVertices(m_RuntimeVertices);
	}


	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	�p�r: �ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_StateChangeSize(5.0f)
	{
	}
	SeekObject::~SeekObject() {}

	//������
	void SeekObject::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.125f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();

		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"ObjectGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<SeekObject>());
		//�����s��������
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->SetGameObjectGroup(Group);
		//Obb�̏Փ˔��������
		auto PtrColl = AddComponent<CollisionObb>();

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//��������������
		SetAlphaActive(true);

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<SeekObject>(GetThis<SeekObject>()));
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		m_StateMachine->ChangeState(FarState::Instance());
	}


	//����
	void SeekObject::OnUpdate() {
		//�X�e�[�g�ɂ���ĕς��Ȃ��s�������s
		auto PtrGrav = GetBehavior<Gravity>();
		PtrGrav->Execute();
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->Execute();
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	void SeekObject::OnUpdate2() {
		auto PtrUtil = GetBehavior<UtilBehavior>();
		PtrUtil->RotToHead(0.1f);
	}

	void SeekObject::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
		//�t�@�C�A�̕��o
		auto PtriFire = GetStage()->GetSharedGameObject<MultiFire>(L"MultiFire", false);
		if (PtriFire) {
			PtriFire->InsertFire(GetComponent<Transform>()->GetPosition());
		}
	}



	//--------------------------------------------------------------------------------------
	//	class FarState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<FarState> FarState::Instance() {
		static shared_ptr<FarState> instance(new FarState);
		return instance;
	}
	void FarState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekSteering>();
		float f = PtrSeek->Execute(L"Player");
		if (f < Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void FarState::Exit(const shared_ptr<SeekObject>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	class NearState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<NearState> NearState::Instance() {
		static shared_ptr<NearState> instance(new NearState);
		return instance;
	}
	void NearState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveSteering>();
		if (PtrArrive->Execute(L"Player") >= Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj) {
	}




	//--------------------------------------------------------------------------------------
	//	�����̃X�N�G�A
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	NumberSquare::NumberSquare(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<SeekObject>& SeekObjectPtr, size_t Number) :
		GameObject(StagePtr),
		m_SeekObject(SeekObjectPtr),
		m_Number(Number)
	{}
	NumberSquare::~NumberSquare() {}

	//������
	void NumberSquare::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();
		if (!m_SeekObject.expired()) {
			auto SeekPtr = m_SeekObject.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += 0.75f;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
			PtrTransform->SetQuaternion(SeekTransPtr->GetQuaternion());
			//�ύX�ł���X�N�G�A���\�[�X���쐬

			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Square�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateSquare(1.0f, vertices, indices);
			//UV�l�̕ύX
			float from = ((float)m_Number) / 10.0f;
			float to = from + (1.0f / 10.0f);
			//���㒸�_
			vertices[0].textureCoordinate = Vector2(from, 0);
			//�E�㒸�_
			vertices[1].textureCoordinate = Vector2(to, 0);
			//�������_
			vertices[2].textureCoordinate = Vector2(from, 1.0f);
			//�E�����_
			vertices[3].textureCoordinate = Vector2(to, 1.0f);
			//���_�̌^��ς����V�������_���쐬
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}
			//�V�������_���g���ă��b�V�����\�[�X�̍쐬
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, indices, true);

			auto DrawComp = AddComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(L"NUMBER_TX");
			SetAlphaActive(true);
			SetDrawLayer(1);
		}

	}


	//�ω�
	void NumberSquare::OnUpdate() {

		if (!m_SeekObject.expired()) {
			auto SeekPtr = m_SeekObject.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();

			auto PtrTransform = GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += 0.75f;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(1.0f, 1.0f, 1.0f);

			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();

			Quaternion Qt;
			//�������r���{�[�h�ɂ���
			Qt.Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			//�������t�F�C�V���O�ɂ���ꍇ�͈ȉ��̂悤�ɂ���
			// Qt.Facing(Pos - PtrCamera->GetEye());
			//�������t�F�C�V���OY�ɂ���ꍇ�͈ȉ��̂悤�ɂ���
			// Qt.FacingY(Pos - PtrCamera->GetEye());
			//�������V�[�N�I�u�W�F�N�g�Ɠ����ɂ���ꍇ�͈ȉ��̂悤�ɂ���
			// Qt = SeekTransPtr->GetQuaternion();

			PtrTransform->SetQuaternion(Qt);

		}

	}



	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox() {}

	//������
	void FixedBox::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//�Փ˔���
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
	}

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	�p�r: �㉺�ړ�����{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MoveBox::MoveBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}

	MoveBox::~MoveBox() {}

	//������
	void MoveBox::OnCreate() {
		auto PtrTransform = AddComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);
		AddTag(L"MoveBox");

		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//�A�N�V�����̓o�^
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddMoveBy(5.0f, Vector3(5.0f, 5.0f, 0));
		PtrAction->AddMoveBy(5.0f, Vector3(-5.0f, -5.0f, 0));
		//���[�v����
		PtrAction->SetLooped(true);
		//�A�N�V�����J�n
		PtrAction->Run();



		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		auto Group = GetStage()->GetSharedObjectGroup(L"MoveBox");
		Group->IntoGroup(GetThis<GameObject>());


	}

	//--------------------------------------------------------------------------------------
	//	class SphereObject : public GameObject;
	//	�p�r: ��Q����
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SphereObject::SphereObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{}
	SphereObject::~SphereObject() {}
	//������
	void SphereObject::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//Sphere�̏Փ˔��������
		auto CollPtr = AddComponent<CollisionSphere>();
		CollPtr->SetFixed(true);
		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"WALL_TX");
	}

	//--------------------------------------------------------------------------------------
	//	class UnevenGroundData : public GameObject;
	//	�p�r: �ł��ڂ����̃f�[�^
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	UnevenGroundData::UnevenGroundData(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	UnevenGroundData::~UnevenGroundData() {}
	//������
	void UnevenGroundData::OnCreate() {
		vector<VertexPositionNormalTexture> vertices =
		{
			{ VertexPositionNormalTexture(Vector3(-1.0f, 0.0f, 0.5f), Vector3(0.0f, 1.0f,0.0f), Vector2(0.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vector3(0.0f, 0.5f, 0.5f), Vector3(0.0f, 1.0f,0.0f), Vector2(0.5f, 0.0f)) },
			{ VertexPositionNormalTexture(Vector3(-1.0f, 0.0f,-0.5f), Vector3(0.0f, 1.0f,0.0f), Vector2(0.0f, 1.0f)) },
			{ VertexPositionNormalTexture(Vector3(0.0f, 0.0f,-0.5f), Vector3(0.0f, 1.0f,0.0f), Vector2(0.5f, 1.0f)) },
			{ VertexPositionNormalTexture(Vector3(1.0f, 0.0f, 0.5f), Vector3(0.0f, 1.0f,0.0f), Vector2(1.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vector3(1.0f, 0.0f,-0.5f), Vector3(0.0f, 1.0f,0.0f), Vector2(1.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2 ,
			1, 4, 5 ,
			3, 1, 5
		};
		wstring CheckStr = L"UnevenGroundMesh";
		//���\�[�X���o�^����Ă��邩�m�F���A�o�^����ĂȂ���΍쐬
		//�������Ă����Ƃق��̃X�e�[�W�ł����b�V�����g����
		if (!App::GetApp()->CheckResource<MeshResource>(CheckStr)) {
			//���_���g���ĕ\���p���b�V�����\�[�X�̍쐬
			auto mesh = MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, false);
			//�\���p���b�V�������\�[�X�ɓo�^
			App::GetApp()->RegisterResource(CheckStr, mesh);
			//���C�A�t���[���\���p�̃��b�V���̍쐬
			vector<VertexPositionColor> new_vertices;
			for (size_t i = 0; i < vertices.size(); i++) {
				VertexPositionColor new_v;
				new_v.position = vertices[i].position;
				new_v.color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
				new_vertices.push_back(new_v);
			}
			//���C�A�t���[�����b�V�������\�[�X�ɓo�^
			App::GetApp()->RegisterResource(L"UnevenGroundWireMesh", MeshResource::CreateMeshResource(new_vertices, indices, false));
		}
		//�O�p�`�Փ˔���p�̎O�p�`�̔z����쐬
		//������̓X�e�[�W�P��
		size_t i = 0;
		while (i < indices.size()) {
			TRIANGLE tr;
			tr.m_A = vertices[indices[i]].position;
			tr.m_B = vertices[indices[i + 1]].position;
			tr.m_C = vertices[indices[i + 2]].position;
			m_Triangles.push_back(tr);
			i += 3;
		}

		//�������g���V�F�A�I�u�W�F�N�g�ɓo�^
		GetStage()->SetSharedGameObject(L"UnevenGroundData", GetThis<UnevenGroundData>());

	}



	//--------------------------------------------------------------------------------------
	//	class UnevenGround : public GameObject;
	//	�p�r: �ł��ڂ���
	//--------------------------------------------------------------------------------------
	UnevenGround::UnevenGround(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	UnevenGround::~UnevenGround() {}
	//������
	void UnevenGround::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		auto DataPtr = GetStage()->GetSharedGameObject<UnevenGroundData>(L"UnevenGroundData");
		//�O�p�`�R���W����
		auto PtrTriangle = AddComponent<CollisionTriangles>();
		PtrTriangle->SetMakedTriangles(DataPtr->GetTriangles());
		PtrTriangle->SetWireFrameMesh(L"UnevenGroundWireMesh");
		PtrTriangle->SetDrawActive(true);

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"UnevenGroundMesh");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"WALL_TX");

	}



}
//end basecross
