/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{



	//--------------------------------------------------------------------------------------
	///	�Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
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
	//	�G
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Enemy::Enemy(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<StageCellMap>& CellMap,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_CelMap(CellMap),
		m_Scale(Scale),
		m_StartRotation(Rotation),
		m_StartPosition(Position),
		m_CellIndex(-1),
		m_NextCellIndex(-1),
		m_TargetCellIndex(-1)
	{
	}
	Enemy::~Enemy() {}

	bool Enemy::SearchPlayer() {
		auto MapPtr = m_CelMap.lock();
		if (MapPtr) {
			auto PathPtr = GetComponent<PathSearch>();
			auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
			auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
			m_CellPath.clear();
			//�p�X������������
			if (PathPtr->SearchCell(PlayerPos, m_CellPath)) {
				//��������������
				m_CellIndex = 0;
				m_TargetCellIndex = m_CellPath.size() - 1;
				if (m_CellIndex == m_TargetCellIndex) {
					//���łɓ����Z���ɂ���
					m_NextCellIndex = m_CellIndex;
				}
				else {
					//����Ă���
					m_NextCellIndex = m_CellIndex + 1;

				}
				return true;
			}
			else {
				//���s����
				m_CellIndex = -1;
				m_NextCellIndex = -1;
				m_TargetCellIndex = -1;
			}
		}
		return false;
	}

	bool Enemy::DefaultBehavior() {
		auto PtrRigid = GetComponent<Rigidbody>();
		auto Velo = PtrRigid->GetVelocity();
		Velo *= 0.95f;
		PtrRigid->SetVelocity(Velo);
		auto MapPtr = m_CelMap.lock();
		if (MapPtr) {
			auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
			auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
			CellIndex PlayerCell;
			if (MapPtr->FindCell(PlayerPos, PlayerCell)) {
				return false;
			}
		}
		return true;
	}


	bool Enemy::SeekBehavior() {
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
		auto MyPos = GetComponent<Transform>()->GetPosition();

		auto MapPtr = m_CelMap.lock();
		if (MapPtr) {
			if (SearchPlayer()) {
				auto PtrSeek = GetBehavior<SeekSteering>();
				if (m_NextCellIndex == 0) {
					auto PtrRigid = GetComponent<Rigidbody>();
					auto Velo = PtrRigid->GetVelocity();
					Velo *= 0.95f;
					PtrRigid->SetVelocity(Velo);
					PlayerPos.y = m_StartPosition.y;
					PtrSeek->Execute(PlayerPos);
				}
				else {
					if (length(MyPos - PlayerPos) <= 3.0f){
						auto PtrRigid = GetComponent<Rigidbody>();
						auto Velo = PtrRigid->GetVelocity();
						Velo *= 0.95f;
						PtrRigid->SetVelocity(Velo);
					}
					AABB ret;
					MapPtr->FindAABB(m_CellPath[m_NextCellIndex], ret);
					auto Pos = ret.GetCenter();
					Pos.y = m_StartPosition.y;
					PtrSeek->Execute(Pos);
				}
				return true;
			}
			else {
				auto PtrSeek = GetBehavior<SeekSteering>();
				CellIndex PlayerCell;
				if (MapPtr->FindCell(PlayerPos, PlayerCell)) {
					AABB ret;
					MapPtr->FindAABB(PlayerCell, ret);
					auto Pos = ret.GetCenter();
					Pos.y = m_StartPosition.y;
					PtrSeek->Execute(Pos);
					return true;
				}
			}
		}
		return false;
	}


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

		//SP�̏Փ˔��������
		auto PtrColl = AddComponent<CollisionSphere>();
		PtrColl->SetIsHitAction(IsHitAction::Auto);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"TRACE2_TX");
		//��������������
		SetAlphaActive(true);

		m_StateMachine = make_shared<StateMachine<Enemy>>(GetThis<Enemy>());
		m_StateMachine->ChangeState(EnemyDefault::Instance());
	}

	//�X�V
	void Enemy::OnUpdate() {
		//�X�e�[�g�ɂ���ĕς��Ȃ��s�������s
		auto PtrGrav = GetBehavior<Gravity>();
		PtrGrav->Execute();
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	void Enemy::OnUpdate2() {
		//�i�s�����������悤�ɂ���
		RotToHead();
	}


	//�i�s�����������悤�ɂ���
	void Enemy::RotToHead() {
		auto PtrRigidbody = GetComponent<Rigidbody>();
		//��]�̍X�V
		//Velocity�̒l�ŁA��]��ύX����
		//����Ői�s�����������悤�ɂȂ�
		auto PtrTransform = GetComponent<Transform>();
		Vec3 Velocity = PtrRigidbody->GetVelocity();
		if (Velocity.length() > 0.0f) {
			Vec3 Temp = Velocity;
			Temp.normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quat Qt;
			Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
			Qt.normalize();
			//���݂̉�]���擾
			Quat NowQt = PtrTransform->GetQuaternion();
			//���݂ƖڕW���ԁi10����1�j
			NowQt = XMQuaternionSlerp(NowQt, Qt, 0.1f);
			PtrTransform->SetQuaternion(NowQt);
		}
	}




	//--------------------------------------------------------------------------------------
	///	�f�t�H���g�X�e�[�g
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	IMPLEMENT_SINGLETON_INSTANCE(EnemyDefault)

	void EnemyDefault::Enter(const shared_ptr<Enemy>& Obj) {
	}

	void EnemyDefault::Execute(const shared_ptr<Enemy>& Obj) {
		if (!Obj->DefaultBehavior()) {
			Obj->GetStateMachine()->ChangeState(EnemySeek::Instance());
		}
	}
	void EnemyDefault::Exit(const shared_ptr<Enemy>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	�v���C���[��ǂ�������X�e�[�g
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	IMPLEMENT_SINGLETON_INSTANCE(EnemySeek)

	void EnemySeek::Enter(const shared_ptr<Enemy>& Obj) {
	}

	void EnemySeek::Execute(const shared_ptr<Enemy>& Obj) {
		if (!Obj->SeekBehavior()) {
			Obj->GetStateMachine()->ChangeState(EnemyDefault::Instance());
		}
	}

	void EnemySeek::Exit(const shared_ptr<Enemy>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	�����G
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	TestCellChangeEnemy::TestCellChangeEnemy(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<StageCellMap>& CellMap,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	):
		Enemy(StagePtr, CellMap, Scale, Rotation, Position)
	{}
	TestCellChangeEnemy::~TestCellChangeEnemy() {}

	bool TestCellChangeEnemy::DefaultBehavior() {
		auto PtrRigid = GetComponent<Rigidbody>();
		auto Velo = PtrRigid->GetVelocity();
		Velo *= 0.95f;
		PtrRigid->SetVelocity(Velo);
		auto MapPtr = m_CelMap.lock();
		if (MapPtr) {
			auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
			auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
			CellIndex PlayerCell;
			if (MapPtr->FindCell(PlayerPos, PlayerCell)) {
				//�v���C���[���Z���}�b�v��ɓ������B
				return false;
			}
			else {
				//�v���C���[�̓}�b�v��ɂ��Ȃ�
				//�}�b�v���v���C���[�̎���ɍĐݒ�
				Vec3  CellmapStart;
				CellmapStart.x = float((int)(PlayerPos.x - 2.0f));
				CellmapStart.z = float((int)(PlayerPos.z - 2.0f));
				CellmapStart.y = 0.0f;
				MapPtr->RefleshCellMap(CellmapStart, 1.0f, 4, 4);
				auto GameStagePtr = dynamic_pointer_cast<GameStage>(GetStage());
				GameStagePtr->SetCellMapCost(L"CellMap2");
			}
		}
		return true;
	}

}
//end basecross
