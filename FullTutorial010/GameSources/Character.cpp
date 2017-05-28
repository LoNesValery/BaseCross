/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	�p�r: ��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Sword::Sword(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const wstring& Tag
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Tag(Tag)
	{
	}

	Sword::~Sword() {

	}
	//������
	void Sword::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		AddTag(m_Tag);
		//�Փ˔��������(CAPSULE�����o�����߂̂�)
		auto PtrCol = AddComponent<CollisionCapsule>();
		PtrCol->SetIsHitAction(IsHitAction::None);
		if (m_Tag == L"PlayerSword") {
			PtrCol->AddExcludeCollisionTag(L"Player");
		}
		else if (m_Tag == L"EnemySword") {
			PtrCol->AddExcludeCollisionTag(L"Enemy");
		}
	//	PtrCol->SetDrawActive(true);
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_CAPSULE");
		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_CAPSULE");
		//�`�悷��e�N�X�`����ݒ�
		if (m_Tag == L"PlayerSword") {
			PtrDraw->SetTextureResource(L"BLUE_TX");
		}
		else if (m_Tag == L"EnemySword") {
			PtrDraw->SetTextureResource(L"RED_TX");
		}
	}

	//�\�z�Ɣj��
	TilingPlate::TilingPlate(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Quaternion& Qt,
		const Vector3& Position,
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
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position,
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
	// �����̃X�p�[�N�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiSpark::MultiSpark(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiSpark::~MultiSpark() {}

	//������
	void MultiSpark::OnCreate() {
		//���Z�`�揈��������
		SetAddType(true);
	}


	void MultiSpark::InsertSpark(const Vector3& Pos) {
		auto ParticlePtr = InsertParticle(16);
		ParticlePtr->SetEmitterPos(Pos);
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->SetMaxTime(0.3f);
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.2f - 0.1f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.2f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.2f - 0.1f;
			rParticleSprite.m_LocalScale = Vector3(0.5f, 0.5f, 0.5f);
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vector3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//�F�̎w��
			rParticleSprite.m_Color = Color4(1.0f, 0.5f, 0.0f, 1.0f);
		}
	}


	//--------------------------------------------------------------------------------------
	//�����̉��N���X
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
		auto ParticlePtr = InsertParticle(16);
		ParticlePtr->SetEmitterPos(Pos);
		ParticlePtr->SetTextureResource(L"FIRE_TX");
		ParticlePtr->SetMaxTime(4.0f);
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.01f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vector3(
				rParticleSprite.m_LocalPos.x * 10.0f,
				rParticleSprite.m_LocalPos.y * 10.0f,
				rParticleSprite.m_LocalPos.z * 10.0f
			);
			//�F�̎w��
			rParticleSprite.m_Color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}


	//--------------------------------------------------------------------------------------
	//	�G
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Enemy::Enemy(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<StageCellMap>& CellMap,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_CelMap(CellMap),
		m_Scale(Scale),
		m_StartRotation(Rotation),
		m_StartPosition(Position),
		m_MaxHP(100.0f),
		m_HP(100.0f)
	{
	}
	Enemy::~Enemy() {}

	//������
	void Enemy::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPosition);
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_StartRotation);
		//Rigidbody������
		auto PtrRigid = AddComponent<Rigidbody>();
		//�p�X����
		auto MapPtr = m_CelMap.lock();
		if (!MapPtr) {
			throw BaseException(
				L"�Z���}�b�v���s��ł�",
				L"if (!MapPtr) ",
				L" Enemy::OnCreate()"
			);
		}
		auto PathPtr = AddComponent<PathSearch>(MapPtr);

		//�Փ˔��������
		auto PtrColl = AddComponent<CollisionCapsule>();
		PtrColl->SetIsHitAction(IsHitAction::Slide);
//		PtrColl->SetDrawActive(true);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CAPSULE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CAPSULE");
		PtrDraw->SetTextureResource(L"TRACE2_TX");
		//��������������
		SetAlphaActive(true);

		//�Z�������̍s���̍\�z
		m_CellSearchBehavior.reset(new CellSearchBehavior<Enemy>(GetThis<Enemy>(), m_CelMap));
		//�킢�̍s���̍\�z
		m_FightBehavior.reset(new FightBehavior<Enemy>(GetThis<Enemy>()));

		m_StateMachine = make_shared<StateMachine<Enemy>>(GetThis<Enemy>());
		m_StateMachine->ChangeState(EnemyDefault::Instance());
	}

	//�X�V
	void Enemy::OnUpdate() {
		if (GetStateMachine()->GetCurrentState() != EnemyDieState::Instance()){
			//�X�e�[�g�ɂ���ĕς��Ȃ��s�������s
			auto PtrGrav = GetBehavior<Gravity>();
			PtrGrav->Execute();
		}
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	void Enemy::OnUpdate2() {
		if (GetStateMachine()->GetCurrentState() == EnemySwordState::Instance()
			|| GetStateMachine()->GetCurrentState() == EnemySwordHit::Instance()) {
			RotToPlayer();
		}
		else if (GetStateMachine()->GetCurrentState() != EnemyDieState::Instance()) {
			//�i�s�����������悤�ɂ���
			RotToHead();
		}
	}

	void Enemy::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
		auto SrcColl = GetComponent<CollisionCapsule>();
		//�G�������ɓ�������
		for (auto&v : OtherVec) {
			if (v->FindTag(L"PlayerSword")) {
				//���ɓ�������
				auto DestColl = v->GetComponent<CollisionCapsule>();
				//���̃R���W�������ꎞ�I�ɖ����ɂ���
				DestColl->SetUpdateActive(false);
				//�X�p�[�N�̕��o
				auto PtrSpark = GetStage()->GetSharedGameObject<MultiSpark>(L"MultiSpark", false);
				if (PtrSpark) {
					Vector3 Pos1, Pos2;
					HitTest::CAPSULE_CAPSULE(SrcColl->GetCapsule(), DestColl->GetCapsule(), Pos1, Pos2);
					PtrSpark->InsertSpark(Pos1);
				}
				//HP��������
				m_HP -= 5.0f;
				if (m_HP <= 0.0f) {
					m_HP = 0.0f;
					//�t�@�C�A�̕��o
					auto PtriFire = GetStage()->GetSharedGameObject<MultiFire>(L"MultiFire", false);
					if (PtriFire) {
						PtriFire->InsertFire(GetComponent<Transform>()->GetPosition());
					}
					//�|���
					GetStateMachine()->ChangeState(EnemyDieState::Instance());
				}
				else {
					auto HaveSordPtr = v->GetComponent<Transform>()->GetParent();
					m_FightBehavior->SetHitBase(HaveSordPtr->GetComponent<Transform>()->GetPosition());
					GetStateMachine()->ChangeState(EnemySwordHit::Instance());
				}
				return;
			}
		}
	}



	//�i�s�����������悤�ɂ���
	void Enemy::RotToHead() {
		auto PtrRigidbody = GetComponent<Rigidbody>();
		//��]�̍X�V
		//Velocity�̒l�ŁA��]��ύX����
		//����Ői�s�����������悤�ɂȂ�
		auto PtrTransform = GetComponent<Transform>();
		Vector3 Velocity = PtrRigidbody->GetVelocity();
		if (Velocity.Length() > 0.0f) {
			Vector3 Temp = Velocity;
			Temp.Normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quaternion Qt;
			Qt.RotationRollPitchYaw(0, ToAngle, 0);
			Qt.Normalize();
			//���݂̉�]���擾
			Quaternion NowQt = PtrTransform->GetQuaternion();
			//���݂ƖڕW���ԁi10����1�j
			NowQt.Slerp(NowQt, Qt, 0.1f);
			PtrTransform->SetQuaternion(NowQt);
		}
	}

	//�v���C���[�����������悤�ɂ���
	void Enemy::RotToPlayer() {
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PtrTransform = GetComponent<Transform>();
		auto Pos = PtrTransform->GetPosition();
		auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
		Vector3 Velocity = PlayerPos - Pos;
		if (Velocity.Length() > 0.0f) {
			Vector3 Temp = Velocity;
			Temp.Normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quaternion Qt;
			Qt.RotationRollPitchYaw(0, ToAngle, 0);
			Qt.Normalize();
			//���݂̉�]���擾
			Quaternion NowQt = PtrTransform->GetQuaternion();
			//���݂ƖڕW���ԁi5����1�j
			NowQt.Slerp(NowQt, Qt, 0.2f);
			PtrTransform->SetQuaternion(NowQt);
		}

	}





	//--------------------------------------------------------------------------------------
	///	�f�t�H���g�X�e�[�g
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	IMPLEMENT_SINGLETON_INSTANCE(EnemyDefault)

	void EnemyDefault::Enter(const shared_ptr<Enemy>& Obj) {
		Obj->GetFightBehavior()->HaveSword(L"EnemySword");
	}

	void EnemyDefault::Execute(const shared_ptr<Enemy>& Obj) {
		auto PlayerPtr = Obj->GetStage()->GetSharedGameObject<Player>(L"Player");
		//�v���C���[���|��Ă����玩���̈ʒu�ɖ߂�
		if (PlayerPtr->GetStateMachine()->GetCurrentState() == PlayerDieState::Instance()) {
			if (Obj->GetCellSearchBehavior()->SeekBehavior(Obj->GetStartPosition()) == CellSearchFlg::Arrived) {
				Obj->GetCellSearchBehavior()->ArriveBehavior(Obj->GetStartPosition());
			}
		}
		else {
			auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
			PlayerPos.y = Obj->GetStartPosition().y;
			if (Obj->GetCellSearchBehavior()->SeekBehavior(PlayerPos) == CellSearchFlg::Failed) {
				if (Obj->GetCellSearchBehavior()->SeekBehavior(Obj->GetStartPosition()) == CellSearchFlg::Arrived) {
					Obj->GetCellSearchBehavior()->ArriveBehavior(Obj->GetStartPosition());
				}
			}
			else {
				auto EnemyPos = Obj->GetComponent<Transform>()->GetPosition();
				if (Vector3EX::Length(EnemyPos - PlayerPos) <= 1.8f) {
					Obj->GetStateMachine()->ChangeState(EnemySwordState::Instance());
				}
			}
		}


	}
	void EnemyDefault::Exit(const shared_ptr<Enemy>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	�G������U��X�e�[�g
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(EnemySwordState)

	void EnemySwordState::Enter(const shared_ptr<Enemy>& Obj) {
		Obj->GetFightBehavior()->StartShakeSword();
	}

	void EnemySwordState::Execute(const shared_ptr<Enemy>& Obj) {
		auto PlayerPtr = Obj->GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
		Obj->GetCellSearchBehavior()->SeekBehavior(PlayerPos);
		if (Obj->GetFightBehavior()->RotationShakeSword()) {
			//������]��������f�t�H���g�X�e�[�g�Ɉڍs
			Obj->GetStateMachine()->ChangeState(EnemyDefault::Instance());
		}
	}

	void EnemySwordState::Exit(const shared_ptr<Enemy>& Obj) {
		//�������Ȃ�
	}



	//--------------------------------------------------------------------------------------
	///	�v���C���[�̌��ɓ��������X�e�[�g
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	IMPLEMENT_SINGLETON_INSTANCE(EnemySwordHit)

	void EnemySwordHit::Enter(const shared_ptr<Enemy>& Obj) {
		Obj->GetFightBehavior()->HitSwordBehavior();
	}

	void EnemySwordHit::Execute(const shared_ptr<Enemy>& Obj) {
		if (Obj->GetFightBehavior()->HitAfterdBehavior()) {
			Obj->GetStateMachine()->ChangeState(EnemyDefault::Instance());
		}
	}

	void EnemySwordHit::Exit(const shared_ptr<Enemy>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	�|���ꂽ�X�e�[�g
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	IMPLEMENT_SINGLETON_INSTANCE(EnemyDieState)

	void EnemyDieState::Enter(const shared_ptr<Enemy>& Obj) {
		Obj->GetFightBehavior()->DieStartBehavior();
	}

	void EnemyDieState::Execute(const shared_ptr<Enemy>& Obj) {
		if (Obj->GetFightBehavior()->DieBehavior()) {
			Obj->GetStateMachine()->ChangeState(EnemyDefault::Instance());
		}
	}

	void EnemyDieState::Exit(const shared_ptr<Enemy>& Obj) {
		//HP�����Ƃɖ߂�
		Obj->SetHP(Obj->GetMaxHP());
	}



	//--------------------------------------------------------------------------------------
	//	HP�̃X�N�G�A�̗̈�
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	HPSquareBase::HPSquareBase(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{
	}
	HPSquareBase::~HPSquareBase() {}

	void HPSquareBase::OnCreate() {
		auto PtrDraw = AddComponent<PCStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_PC_SQUARE");
		PtrDraw->SetDiffuse(Color4(0, 0, 0, 1));

	}

	//--------------------------------------------------------------------------------------
	//	HP�̃X�N�G�A
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	HPSquare::HPSquare(const shared_ptr<Stage>& StagePtr, const Color4& Col) :
		GameObject(StagePtr),
		m_Color(Col)
	{
	}
	HPSquare::~HPSquare() {}

	void HPSquare::OnCreate() {
		auto PtrDraw = AddComponent<PCStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_PC_SQUARE");
		PtrDraw->SetDiffuse(m_Color);
		SetAlphaActive(true);

	}




}
//end basecross
