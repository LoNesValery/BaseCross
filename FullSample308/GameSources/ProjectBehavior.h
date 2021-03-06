/*!
@file ProjectBehavior.h
@brief プロジェク定義の行動クラス
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	template<typename T>
	struct InputHandler {
		void PushHandle(const shared_ptr<T>& Obj) {
			//コントローラの取得
			auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (CntlVec[0].bConnected) {
				//Aボタン
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					Obj->OnPushA();
				}
				//Xボタン
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_X) {
					Obj->OnPushX();
				}
			}
		}
	};


	//--------------------------------------------------------------------------------------
	///	プレイヤーの行動クラス
	//--------------------------------------------------------------------------------------
	class PlayerBehavior : public Behavior {
		//最高速度
		float m_MaxSpeed;
		//減速率
		float m_Decel;
		//質量
		float m_Mass;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コントローラから方向ベクトルを得る
		@return	方向ベクトル
		*/
		//--------------------------------------------------------------------------------------
		Vec3 GetMoveVector() const;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		PlayerBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr),
			m_MaxSpeed(10.0f),	//最高速度
			m_Decel(0.95f),	//減速値
			m_Mass(0.5f)	//質量
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PlayerBehavior() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	最高速度を得る
		@return	最高速度
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxSpeed() const {
			return m_MaxSpeed;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	減速値を得る
		@return	減速値
		*/
		//--------------------------------------------------------------------------------------
		float GetDecel() const {
			return m_Decel;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	質量を得る
		@return	質量
		*/
		//--------------------------------------------------------------------------------------
		float GetMass() const {
			return m_Mass;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プレイヤーを移動させる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void MovePlayer();
	};

	//--------------------------------------------------------------------------------------
	///	戦う系の行動クラス
	//--------------------------------------------------------------------------------------
	class Sword;
	template <typename T>
	class FightBehavior : public ObjBehavior<T> {
		//回転切りの回転角度
		float m_RotationShakeRad;
		//剣のweak_ptr
		weak_ptr<Sword> m_SwordWeak;
		//相手の剣に当たったときの変数
		//相手の場所
		Vec3 m_HitBase;
		//ひるむ時間
		float m_HitSwordTime;
		//倒れた処理用
		enum class DieFlag {
			Down,
			Die,
			Up,
		};
		float m_DieRot;
		float m_DieInterval;
		DieFlag m_DieFlag;
		Vec3 m_PokeStart;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		FightBehavior(const shared_ptr<T>& GameObjectPtr) :
			ObjBehavior(GameObjectPtr)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~FightBehavior() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剣を持つ処理
		@param[in]	SwordKey	剣を特定するキー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void HaveSword(const wstring& SwordKey) {
			auto SwordPtr = m_SwordWeak.lock();
			if (!SwordPtr) {
				SwordPtr = GetStage()->GetSharedGameObject<Sword>(SwordKey, false);
				if (SwordPtr) {
					m_SwordWeak = SwordPtr;
				}
				else {
					//例外
					throw BaseException(
						L"剣が特定できません",
						L"if (!SwordPtr)",
						L" FightBehavior<T>::HaveSword()"
					);
				}
			}
			auto SwordTransPtr = SwordPtr->GetComponent<Transform>();
			//剣の親にする
			SwordTransPtr->SetParent(GetGameObject());
			Vec3 Pos(0, 0, -0.2f);
			SwordTransPtr->SetPosition(Pos);
			Quat Qt(Vec3(0, 1, 0), 0);
			SwordTransPtr->SetQuaternion(Qt);
			//剣のコリジョンは無効にする
			auto SowdCollPtr = SwordPtr->GetComponent<Collision>();
			SowdCollPtr->SetUpdateActive(false);
			//本体のコリジョンは有効にする
			GetGameObject()->GetComponent<Collision>()->SetUpdateActive(true);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	剣を振る処理の開始
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void StartShakeSword() {
			auto SwordPtr = m_SwordWeak.lock();
			if (SwordPtr) {
				Vec3 Pos(sin(0.0f + XM_PIDIV2) * 0.5f, 0.0f, cos(0.0f + XM_PIDIV2) * 0.5f);
				auto SwordTransPtr = SwordPtr->GetComponent<Transform>();
				SwordTransPtr->ResetPosition(Pos);
				Quat Qt(Vec3(0, 0, 1), XM_PIDIV2);
				SwordTransPtr->SetQuaternion(Qt);
				m_RotationShakeRad = 0;
				//剣のコリジョンを有効にする
				auto SowdCollPtr = SwordPtr->GetComponent<Collision>();
				SowdCollPtr->SetUpdateActive(true);

			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剣を振る（回転）
		@return	半周回転したらtrue
		*/
		//--------------------------------------------------------------------------------------
		bool RotationShakeSword() {
			auto SwordPtr = m_SwordWeak.lock();
			if (SwordPtr) {
				auto SwordTransPtr = SwordPtr->GetComponent<Transform>();
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				float RotSpan = -ElapsedTime * 10.0f;
				m_RotationShakeRad += RotSpan;
				Quat QtSpan(Vec3(0, 1, 0), RotSpan);
				auto Qt = SwordTransPtr->GetQuaternion();
				Qt *= QtSpan;
				SwordTransPtr->SetQuaternion(Qt);
				Vec3 Pos(sin(m_RotationShakeRad + XM_PIDIV2) * 0.5f, 0.0f, cos(m_RotationShakeRad + XM_PIDIV2) * 0.5f);
				SwordTransPtr->SetPosition(Pos);
				if (abs(m_RotationShakeRad) >= XM_PI) {
					m_RotationShakeRad = 0;
					//剣のコリジョンは無効にする
					auto SowdCollPtr = SwordPtr->GetComponent<Collision>();
					SowdCollPtr->SetUpdateActive(false);
					return true;
				}
				else {
					return false;
				}
			}
			//剣が取得できなければ常に「回転修了」
			return true;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	剣の開始点と終了点を取得する
		@param[out]	StartPos	開始点を受ける参照
		@param[out]	EndPos	終了点を受ける参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetSowdStartEndLine(Vec3& StartPos, Vec3& EndPos) {
			auto SwordPtr = m_SwordWeak.lock();
			if (SwordPtr) {
				auto SwordTransPtr = SwordPtr->GetComponent<Transform>();

				auto WorldMat = SwordTransPtr->GetWorldMatrix();
				StartPos = Vec3(0.0, -1.2f, 0.0f);
				StartPos *= WorldMat;
				EndPos = Vec3(0.0, 1.15f, 0.0f);
				EndPos *= WorldMat;
			}

		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief	剣を突く処理の開始
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void StartPokeSword() {
			auto SwordPtr = m_SwordWeak.lock();
			if (SwordPtr) {
				auto SwordTransPtr = SwordPtr->GetComponent<Transform>();

				m_PokeStart = SwordTransPtr->GetPosition();

				Vec3 Pos(sin(0.0f) * 1.0f, 0.0f, cos(0.0f) * 1.0f);
				SwordTransPtr->ResetPosition(Pos);
				//剣のコリジョンを有効にする
				auto SowdCollPtr = SwordPtr->GetComponent<Collision>();
				SowdCollPtr->SetUpdateActive(true);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剣を突く
		@return	一定距離突いたらtrue
		*/
		//--------------------------------------------------------------------------------------
		bool PokeSword() {
			auto SwordPtr = m_SwordWeak.lock();
			if (SwordPtr) {
				auto SwordTransPtr = SwordPtr->GetComponent<Transform>();
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				float PokeSpan = ElapsedTime * 5.0f;
				Vec3 Pos = SwordTransPtr->GetPosition();
				Pos.y -= PokeSpan;
				SwordTransPtr->SetPosition(Pos);
				if (Pos.y <= (m_PokeStart.y - 2.5f)) {
					Pos = m_PokeStart;
					SwordTransPtr->SetPosition(Pos);
					//剣のコリジョンは無効にする
					auto SowdCollPtr = SwordPtr->GetComponent<Collision>();
					SowdCollPtr->SetUpdateActive(false);
					return true;
				}
				else {
					return false;
				}
			}
			//剣が取得できなければ常に「修了」
			return true;
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief	相手の剣に当たったときの相手の位置の設定
		@param[in]	pos	位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetHitBase(const Vec3& pos) {
			m_HitBase = pos;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	相手の剣に当たった行動
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void HitSwordBehavior() {
			CAPSULE SrcCap = GetGameObject()->GetComponent<CollisionCapsule>()->GetCapsule();
			Vec3 HitVelo = SrcCap.GetCenter() - m_HitBase;
			HitVelo.y = 0;
			HitVelo.normalize();
			HitVelo *= 6.0f;
			auto PtrRig = GetGameObject()->GetComponent<Rigidbody>();
			PtrRig->SetVelocity(HitVelo);
			m_HitSwordTime = 0;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剣に当たってひるむ行動
		@return	ひるむ時間が終わったらtrue
		*/
		//--------------------------------------------------------------------------------------
		bool HitAfterdBehavior() {
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			m_HitSwordTime += ElapsedTime;
			auto PtrDraw = GetGameObject()->GetComponent<BcPNTStaticDraw>();
			float val = abs(sin(m_HitSwordTime * 10.0f));
			Col4 Col(val, val, val, 0);
			PtrDraw->SetEmissive(Col);
			if (m_HitSwordTime >= 0.7f) {
				PtrDraw->SetEmissive(Col4(0, 0, 0, 1));
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	倒れる開始
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void DieStartBehavior() {
			//コリジョンを止める
			GetGameObject()->GetComponent<Collision>()->SetUpdateActive(false);
			//剣のコリジョンも止める
			auto SwordPtr = m_SwordWeak.lock();
			if (SwordPtr) {
				SwordPtr->GetComponent<Collision>()->SetUpdateActive(false);
			}
			//速度も止める
			auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>();
			PtrRigid->SetVelocityZero();

			m_DieRot = 0.0f;
			m_DieInterval = 0.0f;
			m_DieFlag = DieFlag::Down;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	倒れる行動
		@return	一連の動作が終わればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool DieBehavior() {
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			switch (m_DieFlag) {
			case DieFlag::Down:
			{
				m_DieRot += ElapsedTime;
				auto Qt = GetGameObject()->GetComponent<Transform>()->GetQuaternion();
				Quat SpanQt(Vec3(1, 0, 0), ElapsedTime);
				Qt *= SpanQt;
				GetGameObject()->GetComponent<Transform>()->SetQuaternion(Qt);
				if (m_DieRot >= XM_PIDIV2) {
					auto MoveToPtr = GetGameObject()->GetComponent<MoveTo>();
					MoveToPtr->SetUpdateActive(true);
					MoveToPtr->SetParams(3.0f, GetGameObject()->GetStartPosition());
					MoveToPtr->Run();
					m_DieFlag = DieFlag::Die;
				}
				auto Pos = GetGameObject()->GetComponent<Transform>()->GetPosition();
				if (Pos.y >= 0.25f) {
					Pos.y -= ElapsedTime;
					GetGameObject()->GetComponent<Transform>()->SetPosition(Pos);
				}
				Col4 Col = GetGameObject()->GetComponent<BcPNTStaticDraw>()->GetDiffuse();
				Col.x -= 0.05f;
				if (Col.x <= 0.0f) {
					Col.x = 0.0f;
				}
				Col.y -= 0.05f;
				if (Col.y <= 0.0f) {
					Col.y = 0.0f;
				}
				Col.z -= 0.05f;
				if (Col.z <= 0.0f) {
					Col.z = 0.0f;
				}
				GetGameObject()->GetComponent<BcPNTStaticDraw>()->SetDiffuse(Col);
				return false;
			}
			break;
			case DieFlag::Die:
				m_DieInterval += ElapsedTime;
				if (m_DieInterval >= 5.0f) {
					auto MoveToPtr = GetGameObject()->GetComponent<MoveTo>();
					MoveToPtr->SetUpdateActive(false);
					auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>();
					PtrRigid->SetVelocityZero();
					m_DieFlag = DieFlag::Up;
				}
				return false;
				break;
			case DieFlag::Up:
			{
				m_DieRot -= ElapsedTime * 2.0f;
				auto Qt = GetGameObject()->GetComponent<Transform>()->GetQuaternion();
				Quat SpanQt(Vec3(1, 0, 0), -ElapsedTime * 2.0f);
				Qt *= SpanQt;
				GetGameObject()->GetComponent<Transform>()->SetQuaternion(Qt);
				auto Pos = GetGameObject()->GetComponent<Transform>()->GetPosition();
				if (Pos.y <= 0.5f) {
					Pos.y += ElapsedTime * 2.0f;
					GetGameObject()->GetComponent<Transform>()->SetPosition(Pos);
				}
				if (m_DieRot <= 0) {
					GetGameObject()->GetComponent<BcPNTStaticDraw>()->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 0.0f));
					return true;
				}
				Col4 Col = GetGameObject()->GetComponent<BcPNTStaticDraw>()->GetDiffuse();
				Col.x += 0.05f;
				if (Col.x >= 1.0f) {
					Col.x = 1.0f;
				}
				Col.y += 0.05f;
				if (Col.y >= 1.0f) {
					Col.y = 1.0f;
				}
				Col.z += 0.05f;
				if (Col.z >= 1.0f) {
					Col.z = 1.0f;
				}
				GetGameObject()->GetComponent<BcPNTStaticDraw>()->SetDiffuse(Col);

				return false;
			}
			break;
			}
			return false;
		}

	};



}

//end basecross

