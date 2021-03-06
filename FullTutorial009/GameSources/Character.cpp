/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{



	//--------------------------------------------------------------------------------------
	///	固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
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

	//初期化
	void FixedBox::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//衝突判定
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
	}

	//--------------------------------------------------------------------------------------
	//	敵
	//--------------------------------------------------------------------------------------
	//構築と破棄
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
			//パス検索をかける
			if (PathPtr->SearchCell(PlayerPos, m_CellPath)) {
				//検索が成功した
				m_CellIndex = 0;
				m_TargetCellIndex = m_CellPath.size() - 1;
				if (m_CellIndex == m_TargetCellIndex) {
					//すでに同じセルにいる
					m_NextCellIndex = m_CellIndex;
				}
				else {
					//離れている
					m_NextCellIndex = m_CellIndex + 1;

				}
				return true;
			}
			else {
				//失敗した
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


	//初期化
	void Enemy::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPosition);
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_StartRotation);
		//Rigidbodyをつける
		auto PtrRigid = AddComponent<Rigidbody>();
		//パス検索
		auto MapPtr = m_CelMap.lock();
		if (!MapPtr) {
			throw BaseException(
				L"セルマップが不定です",
				L"if (!MapPtr) ",
				L" Enemy::OnCreate()"
			);
		}
		auto PathPtr = AddComponent<PathSearch>(MapPtr);

		//SPの衝突判定をつける
		auto PtrColl = AddComponent<CollisionSphere>();
		PtrColl->SetIsHitAction(IsHitAction::Auto);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"TRACE2_TX");
		//透明処理をする
		SetAlphaActive(true);

		m_StateMachine = make_shared<StateMachine<Enemy>>(GetThis<Enemy>());
		m_StateMachine->ChangeState(EnemyDefault::Instance());
	}

	//更新
	void Enemy::OnUpdate() {
		//ステートによって変わらない行動を実行
		auto PtrGrav = GetBehavior<Gravity>();
		PtrGrav->Execute();
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	void Enemy::OnUpdate2() {
		//進行方向を向くようにする
		RotToHead();
	}


	//進行方向を向くようにする
	void Enemy::RotToHead() {
		auto PtrRigidbody = GetComponent<Rigidbody>();
		//回転の更新
		//Velocityの値で、回転を変更する
		//これで進行方向を向くようになる
		auto PtrTransform = GetComponent<Transform>();
		Vec3 Velocity = PtrRigidbody->GetVelocity();
		if (Velocity.length() > 0.0f) {
			Vec3 Temp = Velocity;
			Temp.normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quat Qt;
			Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
			Qt.normalize();
			//現在の回転を取得
			Quat NowQt = PtrTransform->GetQuaternion();
			//現在と目標を補間（10分の1）
			NowQt = XMQuaternionSlerp(NowQt, Qt, 0.1f);
			PtrTransform->SetQuaternion(NowQt);
		}
	}




	//--------------------------------------------------------------------------------------
	///	デフォルトステート
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
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
	///	プレイヤーを追いかけるステート
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
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
	//	強い敵
	//--------------------------------------------------------------------------------------
	//構築と破棄
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
				//プレイヤーがセルマップ上に入った。
				return false;
			}
			else {
				//プレイヤーはマップ上にいない
				//マップをプレイヤーの周りに再設定
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
