/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�^�C�g���X�v���C�g
	//--------------------------------------------------------------------------------------
	TitleSprite::TitleSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vec2& StartScale, const Vec2& StartPos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos)
	{}

	TitleSprite::~TitleSprite() {}
	void TitleSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��(�c��5���\��)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)) },
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
	///	�A�j���X�v���C�g
	//--------------------------------------------------------------------------------------
	AnimeSprite::AnimeSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vec2& StartScale, const Vec2& StartPos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0.0f)
	{}

	AnimeSprite::~AnimeSprite() {}

	void AnimeSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��(�c��5���\��)
		vector<VertexPositionColorTexture> vertex = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y, 0.0f);
		//���_�ƃC���f�b�N�X���w�肵�ăX�v���C�g�쐬
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertex, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_TextureKey);
	}

	void AnimeSprite::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime * 5.0f;
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0;
		}
		auto PtrDraw = GetComponent<PCTSpriteDraw>();
		Col4 col(1.0, 1.0, 1.0, 1.0);
		col.w = sin(m_TotalTime) * 0.5f + 0.5f;
		PtrDraw->SetDiffuse(col);
	}


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
		ParticlePtr->SetMaxTime(0.5f);
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
	///	�{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Box::Box(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{
	}
	Box::~Box() {}

	//������
	void Box::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vec3(0.5f, 0.5f, 0.5f));
		Quat Qt;
		Qt.identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(m_StartPos);

		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionObb>();
		//�Փ˔����None�ɂ���
		PtrCol->SetIsHitAction(IsHitAction::None);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//�`�悷��e�N�X�`����ݒ�
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//��������
		SetAlphaActive(true);

		//�����v�Z�{�b�N�X
		PsBoxParam param;
		//DEFAULT_CUBE�̃X�P�[�����O�͊e�ӊ�Ȃ̂ŁA�n�[�t�T�C�Y�ɂ���
		param.m_HalfSize = Vec3(0.5f, 0.5f, 0.5f) * 0.5f;
		param.m_Mass = 1.0f;
		//�����e���\���̌v�Z
		param.m_Inertia = BasePhysics::CalcInertiaBox(param.m_HalfSize, param.m_Mass);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = Qt;
		param.m_Pos = m_StartPos;
		auto PsPtr = AddComponent<PsBoxBody>(param);
		PsPtr->SetDrawActive(true);

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<Box>(GetThis<Box>()));
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		m_StateMachine->ChangeState(BoxDefaultState::Instance());

	}

	//�X�V
	void Box::OnUpdate() {
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	//�v���C���[�Ƃ̋����𓾂�
	float Box::GetToPlayerLen() const {
		auto Ptr = GetComponent<Transform>();
		Vec3 Pos = Ptr->GetPosition();
		auto PlayerPos
			= GetStage()->GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
		return length(Pos - PlayerPos);
	}

	//���x��0�ɂ���
	void Box::Stop() {
		auto PsPtr = GetComponent<PsBoxBody>();
		PsPtr->SetLinearVelocity(Vec3(0, 0, 0));
		m_TotalTime = 0;
		m_StartPos = GetComponent<Transform>()->GetPosition();
	}


	//�s�����藈����̍s��
	void Box::GoFromBehavior() {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0;
		}
		auto PsPtr = GetComponent<PsBoxBody>();
		auto Pos = PsPtr->GetPosition();
		Pos.x = sin(m_TotalTime) + m_StartPos.x;
		PsPtr->WakeUp();
		PsPtr->SetPosition(Pos);
	}

	//�v���C���[��ǂ�������s��
	void Box::SeekPlayerBehavior() {
		auto PsPtr = GetComponent<PsBoxBody>();
		PsPtr->WakeUp();
		auto TargetPtr = GetStage()->GetSharedObject(L"Player");
		auto TargetPos = TargetPtr->GetComponent<Transform>()->GetPosition();
		auto Pos = GetComponent<Transform>()->GetPosition();
		TargetPos.y = Pos.y = m_StartPos.y;
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Seek(Vec3(0), TargetPos,
			Pos, 20.0f) * 1.0f;
		PsPtr->ApplyForce(WorkForce);
	}


	//�C�e���ˍs��
	void Box::FirShellBehavior(int div,float up) {
		if (Util::DivProbability(div)) {
			//200����1�̊m���Ŕ���
			auto Ptr = GetComponent<Transform>();
			Vec3 Pos = Ptr->GetPosition();
			Pos.y += 0.5f;
			auto PlayerPos
				= GetStage()->GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
			auto velo = PlayerPos;
			velo -= Pos;
			velo.y = up;
			velo.normalize();
			velo *= 20.0f;
			auto Group = GetStage()->GetSharedObjectGroup(L"ShellGroup");
			for (size_t i = 0; i < Group->size(); i++) {
				auto shptr = dynamic_pointer_cast<ShellSphere>(Group->at(i));
				if (shptr && !shptr->IsUpdateActive()) {
					//�󂫂���������
					shptr->Reset(Pos, velo);
					return;
				}
			}
			//�����܂ŗ��Ă�΋󂫂��Ȃ�
			GetStage()->AddGameObject<ShellSphere>(Pos, velo);
		}
	}

	//--------------------------------------------------------------------------------------
	//	�ʏ�ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<BoxDefaultState> BoxDefaultState::Instance() {
		static shared_ptr<BoxDefaultState> instance(new BoxDefaultState);
		return instance;
	}
	void BoxDefaultState::Enter(const shared_ptr<Box>& Obj) {
		Obj->Stop();
	}
	void BoxDefaultState::Execute(const shared_ptr<Box>& Obj) {
		Obj->GoFromBehavior();
		Obj->FirShellBehavior(120,0.2f);
		if (Obj->GetToPlayerLen() < 10.0f) {
			Obj->GetStateMachine()->ChangeState(BoxSimpleAttackState::Instance());
		}
	}
	void BoxDefaultState::Exit(const shared_ptr<Box>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	�ǂ�������ʏ�U���X�e�[�g
	//--------------------------------------------------------------------------------------
	shared_ptr<BoxSimpleAttackState> BoxSimpleAttackState::Instance() {
		static shared_ptr<BoxSimpleAttackState> instance(new BoxSimpleAttackState);
		return instance;
	}
	void BoxSimpleAttackState::Enter(const shared_ptr<Box>& Obj) {
	}
	void BoxSimpleAttackState::Execute(const shared_ptr<Box>& Obj) {
		Obj->SeekPlayerBehavior();
		Obj->FirShellBehavior(60,0.05f);
		float len = Obj->GetToPlayerLen();
		if (len >= 10.0f) {
			Obj->GetStateMachine()->ChangeState(BoxDefaultState::Instance());
		}
		else if (len < 5.0f) {
			Obj->GetStateMachine()->ChangeState(BoxHradAttackState::Instance());
		}
	}
	void BoxSimpleAttackState::Exit(const shared_ptr<Box>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	�n�[�h�ȍU���X�e�[�g
	//--------------------------------------------------------------------------------------
	shared_ptr<BoxHradAttackState> BoxHradAttackState::Instance() {
		static shared_ptr<BoxHradAttackState> instance(new BoxHradAttackState);
		return instance;
	}
	void BoxHradAttackState::Enter(const shared_ptr<Box>& Obj) {
		Obj->Stop();
	}
	void BoxHradAttackState::Execute(const shared_ptr<Box>& Obj) {
		Obj->FirShellBehavior(40,0.0f);
		float len = Obj->GetToPlayerLen();
		if (len >= 10.0f) {
			Obj->GetStateMachine()->ChangeState(BoxDefaultState::Instance());
		}
	}
	void BoxHradAttackState::Exit(const shared_ptr<Box>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	���˂��鋅��
	//--------------------------------------------------------------------------------------
	ShellSphere::ShellSphere(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity) :
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity),
		m_Scale(0.25f)
	{}
	ShellSphere::~ShellSphere() {}


	//������
	void ShellSphere::OnCreate() {
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellGroup");
		//�O���[�v�ɒǉ�
		Group->IntoGroup(GetThis<GameObject>());

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Emitter);

		//Rigidbody������
		auto PtrRedid = AddComponent<Rigidbody>();
		PtrRedid->SetVelocity(m_Velocity);
		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionSphere>();
		PtrCol->SetIsHitAction(IsHitAction::None);


		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"SKY_TX");

	}

	void ShellSphere::OnUpdate() {
		auto PtrGrav = GetBehavior<Gravity>();
		PtrGrav->Execute();
	}

	void ShellSphere::OnUpdate2() {
		auto PtrTransform = GetComponent<Transform>();
		if (PtrTransform->GetPosition().y < -0.5f) {
			Erase();
			return;
		}
		auto PtrSpark = GetStage()->GetSharedGameObject<MultiSpark>(L"MultiSpark", false);
		if (GetComponent<Collision>()->GetHitObjectVec().size() > 0) {
			for (auto& v : GetComponent<Collision>()->GetHitObjectVec()) {
				auto& ptr = dynamic_pointer_cast<Box>(v);
				auto& ptr2 = dynamic_pointer_cast<Player>(v);
				auto& ptr3 = dynamic_pointer_cast<ActivePsObject>(v);
				if (ptr || ptr2) {
					if (ptr) {
						GetStage()->RemoveGameObject<Box>(ptr);
					}
					else {
						//�����Ƀv���C���[�̃_���[�W���L�q

					}
					//�X�p�[�N�̕��o
					if (PtrSpark) {
						PtrSpark->InsertSpark(GetComponent<Transform>()->GetPosition());
					}
				}
				else if (ptr3) {
					//�X�p�[�N�̕��o
					if (PtrSpark) {
						PtrSpark->InsertSpark(GetComponent<Transform>()->GetPosition());
					}
					Erase();
				}
			}
		}
	}


	void ShellSphere::Erase() {
		SetUpdateActive(false);
		SetDrawActive(false);
	}

	void ShellSphere::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		SetUpdateActive(true);
		SetDrawActive(true);

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->ResetPosition(Emitter);
		auto PtrRedid = GetComponent<Rigidbody>();
		//�d�͂�0�ɂ���
		PtrRedid->SetGravityVelocityZero();
		PtrRedid->SetVelocity(Velocity);
	}


	//--------------------------------------------------------------------------------------
	///	�����v�Z����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedPsBox::FixedPsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	FixedPsBox::~FixedPsBox() {}
	//������
	void FixedPsBox::OnCreate() {

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

		//�����v�Z�{�b�N�X
		PsBoxParam param;
		//DEFAULT_CUBE�̃X�P�[�����O�͊e�ӊ�Ȃ̂ŁA�n�[�t�T�C�Y�ɂ���
		param.m_HalfSize = m_Scale * 0.5f;
		//�Œ�Ȃ̂Ŏ��ʂ͂���Ȃ�
		param.m_Mass = 0.0f;
		//�����e���\�����f�t�H���g�ŗǂ�
		param.m_MotionType = PsMotionType::MotionTypeFixed;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<PsBoxBody>(param);
		PsPtr->SetDrawActive(true);

	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃ{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ActivePsBox::ActivePsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		ActivePsObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsBox::~ActivePsBox() {}
	//������
	void ActivePsBox::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionObb>();
		//�Փ˔����None�ɂ���
		PtrCol->SetIsHitAction(IsHitAction::None);


		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//�����v�Z�{�b�N�X
		PsBoxParam param;
		//DEFAULT_CUBE�̃X�P�[�����O�͊e�ӊ�Ȃ̂ŁA�n�[�t�T�C�Y�ɂ���
		param.m_HalfSize = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		//�����e���\���̌v�Z
		param.m_Inertia = BasePhysics::CalcInertiaBox(param.m_HalfSize, param.m_Mass);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<PsBoxBody>(param);
		PsPtr->SetDrawActive(true);
	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȋ���
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ActivePsSphere::ActivePsSphere(const shared_ptr<Stage>& StagePtr,
		float Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		ActivePsObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsSphere::~ActivePsSphere() {}
	//������
	void ActivePsSphere::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionSphere>();
		//�Փ˔����None�ɂ���
		PtrCol->SetIsHitAction(IsHitAction::None);


		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//�����v�Z����
		PsSphereParam param;
		//DEFAULT_SPHERE�̃X�P�[�����O�͒��a��Ȃ̂ŁA���a�ɂ���
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		//�����e���\���̌v�Z
		param.m_Inertia = BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<PsSphereBody>(param);
		PsPtr->SetDrawActive(true);
	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃJ�v�Z��
	//--------------------------------------------------------------------------------------
	ActivePsCapsule::ActivePsCapsule(const shared_ptr<Stage>& StagePtr,
		float Len,
		float Diameter,
		const Quat& Qt,
		const Vec3& Position
	) :
		ActivePsObject(StagePtr),
		m_Len(Len),
		m_Diameter(Diameter),
		m_Qt(Qt),
		m_Position(Position)
	{}
	ActivePsCapsule::~ActivePsCapsule() {}

	//������
	void ActivePsCapsule::OnCreate() {

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		bsm::Vec3 PointA(0, 0, 0);
		bsm::Vec3 PointB(0, 0, 0);
		PointA -= bsm::Vec3(0, m_Len * 0.5f, 0);
		PointB += bsm::Vec3(0, m_Len * 0.5f, 0);
		MeshUtill::CreateCapsule(m_Diameter,
			PointA, PointB, 18, vertices, indices, true);
		m_CapsuleMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		Quat OffsetQt(Vec3(0, 0, 1), XM_PIDIV2);
		Mat4x4 MeshToTrans;
		MeshToTrans.affineTransformation(
			Vec3(1.0),
			Vec3(0),
			OffsetQt,
			Vec3(0)
		);

		auto PtrTransform = GetComponent<Transform>();
		//�X�P�[�����O��1.0f
		PtrTransform->SetScale(Vec3(1.0f));
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionCapsule>();
		//�Փ˔����None�ɂ���
		PtrCol->SetIsHitAction(IsHitAction::None);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(m_CapsuleMesh);
		ShadowPtr->SetMeshToTransformMatrix(MeshToTrans);

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(m_CapsuleMesh);
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetMeshToTransformMatrix(MeshToTrans);

		//�����v�Z�J�v�Z��
		PsCapsuleParam param;
		//���a�ɂ���
		param.m_HalfLen = m_Len * 0.5f;
		param.m_Radius = m_Diameter * 0.5f;
		param.m_Mass = 1.0f;
		//�����e���\���̌v�Z
		param.m_Inertia = BasePhysics::CalcInertiaCylinderX(
			param.m_HalfLen + param.m_Radius,
			param.m_Radius,
			param.m_Mass
		);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_OffsetOrientation = OffsetQt;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<PsCapsuleBody>(param);
		PsPtr->SetDrawActive(true);
	}






}
//end basecross
