/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class AttackBall : public GameObject;
	//	用途: 飛んでいくボール
	//--------------------------------------------------------------------------------------
	class AttackBall : public GameObject {
	public:
		//構築と破棄
		AttackBall(const shared_ptr<Stage>& StagePtr);
		virtual ~AttackBall();
		void Weakup(const Vector3& Position, const Vector3& Velocity);
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;
	};


	class Player;
	//--------------------------------------------------------------------------------------
	/// アクションコマンド（Aボタン）
	//--------------------------------------------------------------------------------------
	class ActionCommand : public ObjCommand<Player> {
		virtual void Excute(const shared_ptr<Player>& Obj)override;
	};
	//--------------------------------------------------------------------------------------
	///	行動切り替えコマンド(Bボタン)
	//--------------------------------------------------------------------------------------
	class BehaviorChangeCommand : public ObjCommand<Player> {
		virtual void Excute(const shared_ptr<Player>& Obj)override;
	};



	class PlayerBehavior;
	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	用途: プレイヤー
	//--------------------------------------------------------------------------------------
	class Player : public GameObject {
		//行動関数クラス
		shared_ptr<PlayerBehavior> m_PlayerBehavior;
		//階層化ステートマシーン
		shared_ptr< LayeredStateMachine<Player> >  m_StateMachine;
		//コマンド関連
		void InputCheck() {
			//コントローラボタンチェック
			auto pCom = m_Handler.HandleInput();
			if (pCom) {
				pCom->Excute(GetThis<Player>());
			}
		}
		//アクションコマンド（Aボタン）
		ActionCommand m_Action;
		//行動切り替えコマンド（Bボタン）
		BehaviorChangeCommand m_BehaviorChange;

		//入力ハンドラー
		InputHandler<Player> m_Handler;
		//最高速度
		float m_MaxSpeed;
		//減速率
		float m_Decel;
		//質量
		float m_Mass;
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		Player(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Player() {}
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	階層化ステートマシンを得る
		@return	階層化ステートマシン
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr< LayeredStateMachine<Player> > GetStateMachine() const {
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行動関数クラスを得る
		@return	行動関数クラス
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<PlayerBehavior> GetPlayerBehavior() const {
			return m_PlayerBehavior;
		}
		float GetMaxSpeed() const {
			return m_MaxSpeed;
		}
		float GetDecel() const {
			return m_Decel;
		}
		float GetMass() const {
			return m_Mass;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行動関数クラスを切り替える
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ChangePlayerBehavior();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//衝突時
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
		//ターンの最終更新時
		virtual void OnLastUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	プレイヤー行動親
	//--------------------------------------------------------------------------------------
	class PlayerBehavior {
	protected:
		PlayerBehavior() {}
	public:
		virtual ~PlayerBehavior() {}
		virtual shared_ptr<PlayerBehavior> ChangeNextBehavior(const shared_ptr<Player>& Obj) = 0;
		virtual Vector3 GetAngle(const shared_ptr<Player>& Obj);
		virtual void Move(const shared_ptr<Player>& Obj);
		virtual void StartAction(const shared_ptr<Player>& Obj) = 0;
		virtual void ExcuteAction(const shared_ptr<Player>& Obj) = 0;
	};

	//--------------------------------------------------------------------------------------
	///	デフォルトプレイヤー行動
	//--------------------------------------------------------------------------------------
	class DefaultPlayerBehavior : public PlayerBehavior {
	protected:
		DefaultPlayerBehavior() {}
	public:
		//行動のインスタンス取得
		DECLARE_SINGLETON_INSTANCE(DefaultPlayerBehavior)
		virtual shared_ptr<PlayerBehavior> ChangeNextBehavior(const shared_ptr<Player>& Obj) override;
		virtual void StartAction(const shared_ptr<Player>& Obj) override;
		virtual void ExcuteAction(const shared_ptr<Player>& Obj) override;
	};

	//--------------------------------------------------------------------------------------
	///	攻撃プレイヤー行動
	//--------------------------------------------------------------------------------------
	class AttackPlayerBehavior : public PlayerBehavior {
	protected:
		AttackPlayerBehavior() {}
	public:
		//行動のインスタンス取得
		DECLARE_SINGLETON_INSTANCE(AttackPlayerBehavior)
		virtual shared_ptr<PlayerBehavior> ChangeNextBehavior(const shared_ptr<Player>& Obj) override;
		virtual void StartAction(const shared_ptr<Player>& Obj) override;
		virtual void ExcuteAction(const shared_ptr<Player>& Obj) override;
	};


	//--------------------------------------------------------------------------------------
	///	通常ステート
	//--------------------------------------------------------------------------------------
	class PlayerDefault : public ObjState<Player>
	{
		PlayerDefault() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerDefault)
		virtual void Enter(const shared_ptr<Player>& Obj)override {}
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override {}
	};

	//--------------------------------------------------------------------------------------
	///	アクションステート
	//--------------------------------------------------------------------------------------
	class PlayerAction : public ObjState<Player>
	{
		PlayerAction() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerAction)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override {}
	};




}
//end basecross

