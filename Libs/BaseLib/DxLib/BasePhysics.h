/*!
@file BasePhysics.h
@brief 物理計算インターファイス
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	モーションタイプ
	//--------------------------------------------------------------------------------------
	enum PsMotionType {
		MotionTypeFixed = 0,
		MotionTypeActive,
		MotionTypeKeyframe,
		MotionTypeOneWay,
		MotionTypeTrigger,
		MotionTypeCount
	};

	struct PsParamBase {
		PsMotionType m_MotionType;
		bsm::Quat m_Quat;
		bsm::Vec3 m_Pos;
		bsm::Vec3 m_LinearVelocity;
		bsm::Vec3 m_AngularVelocity;
		bool m_UseSleep;
		float m_Mass;
		PsParamBase() :
			m_Quat(),
			m_Pos(0),
			m_LinearVelocity(0),
			m_AngularVelocity(0),
			m_UseSleep(true),
			m_Mass(1.0f)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	ボックス作成のパラメータ
	//--------------------------------------------------------------------------------------
	struct PsBoxParam : public PsParamBase {
		bsm::Vec3 m_HalfSize;
		PsBoxParam():
			m_HalfSize(1.0f)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	球体作成のパラメータ
	//--------------------------------------------------------------------------------------
	struct PsSphereParam : public PsParamBase {
		float m_Radius;
		PsSphereParam():
			m_Radius(1.0f)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	カプセル作成のパラメータ
	//--------------------------------------------------------------------------------------
	struct PsCapsuleParam : public PsParamBase {
		float m_HalfLen;
		float m_Radius;
		PsCapsuleParam() :
			m_HalfLen(0.5f),
			m_Radius(0.5f)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	シリンダー作成のパラメータ
	//--------------------------------------------------------------------------------------
	struct PsCylinderParam : public PsParamBase {
		float m_HalfLen;
		float m_Radius;
		PsCylinderParam() :
			m_HalfLen(0.5f),
			m_Radius(0.5f)
		{}
	};


	//--------------------------------------------------------------------------------------
	///	剛体のステータス(取得用)
	//--------------------------------------------------------------------------------------
	struct PsBodyStatus {
		bsm::Vec3	m_Position;
		bsm::Quat	m_Orientation;
		bsm::Vec3	m_LinearVelocity;
		bsm::Vec3	m_AngularVelocity;
	};

	//--------------------------------------------------------------------------------------
	///	剛体のステータス(設定用)
	//--------------------------------------------------------------------------------------
	struct PsBodyUpdateStatus {
		bsm::Vec3	m_Force;
		bsm::Vec3	m_Torque;
		bsm::Vec3	m_LinearVelocity;
		bsm::Vec3	m_AngularVelocity;
		PsBodyUpdateStatus():
			m_Force(0.0f),
			m_Torque(0.0f),
			m_LinearVelocity(0.0f),
			m_AngularVelocity(0.0f)
		{}
		PsBodyUpdateStatus(const PsBodyStatus& src) :
			m_Force(0.0f),
			m_Torque(0.0f),
			m_LinearVelocity(src.m_LinearVelocity),
			m_AngularVelocity(src.m_AngularVelocity)
		{}
	};



	//--------------------------------------------------------------------------------------
	///	物理オブジェクトの親
	//--------------------------------------------------------------------------------------
	class PhysicsObject : public ObjectInterface {
	protected:
		/// 剛体インデックス
		uint16_t m_Index;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		explicit PhysicsObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化時のStatusの設定
		@param[in]	param	インデックス
		*/
		//--------------------------------------------------------------------------------------
		void SetParamStatus(const PsParamBase& param);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体インデックスの取得
		*/
		//--------------------------------------------------------------------------------------
		uint16_t GetIndex() const {
			return m_Index;
		}
	};

	//--------------------------------------------------------------------------------------
	///	ボックス物理オブジェクト
	//--------------------------------------------------------------------------------------
	class PhysicsBox : public PhysicsObject {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	param	作成のパラメータ
		@param[in]	index	インデックス
		*/
		//--------------------------------------------------------------------------------------
		explicit PhysicsBox(const PsBoxParam& param, uint16_t index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsBox();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化時のパラメータを得る
		@return	初期化時のパラメータ
		*/
		//--------------------------------------------------------------------------------------
		const PsBoxParam& GetParam() const;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	球体物理オブジェクト
	//--------------------------------------------------------------------------------------
	class PhysicsSphere : public PhysicsObject {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	param	作成のパラメータ
		@param[in]	index	インデックス
		*/
		//--------------------------------------------------------------------------------------
		explicit PhysicsSphere(const PsSphereParam& param,uint16_t index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsSphere();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化時のパラメータを得る
		@return	初期化時のパラメータ
		*/
		//--------------------------------------------------------------------------------------
		const PsSphereParam& GetParam() const;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	カプセル物理オブジェクト
	//--------------------------------------------------------------------------------------
	class PhysicsCapsule : public PhysicsObject {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	param	作成のパラメータ
		@param[in]	index	インデックス
		*/
		//--------------------------------------------------------------------------------------
		explicit PhysicsCapsule(const PsCapsuleParam& param, uint16_t index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsCapsule();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化時のパラメータを得る
		@return	初期化時のパラメータ
		*/
		//--------------------------------------------------------------------------------------
		const PsCapsuleParam& GetParam() const;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	シリンダー物理オブジェクト
	//--------------------------------------------------------------------------------------
	class PhysicsCylinder : public PhysicsObject {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	param	作成のパラメータ
		@param[in]	index	インデックス
		*/
		//--------------------------------------------------------------------------------------
		explicit PhysicsCylinder(const PsCylinderParam& param, uint16_t index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsCylinder();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化時のパラメータを得る
		@return	初期化時のパラメータ
		*/
		//--------------------------------------------------------------------------------------
		const PsCylinderParam& GetParam() const;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	///	物理計算用のインターフェイス
	//--------------------------------------------------------------------------------------
	class BasePhysics{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		explicit BasePhysics();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BasePhysics();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Update();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化（すべてのオブジェクトが無くなる）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Reset();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単体のボックスの追加
		@param[in]	param	作成のパラメータ
		@param[in]	index	インデックス（オブジェクトを再利用する場合）
		@return	オブジェクトのポインタ（バックアップはしないので呼び出し側で保存すること）
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsBox> AddSingleBox(const PsBoxParam& param, uint16_t index = UINT16_MAX);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単体の球体の追加
		@param[in]	param	作成のパラメータ
		@param[in]	index	インデックス（オブジェクトを再利用する場合）
		@return	オブジェクトのポインタ（バックアップはしないので呼び出し側で保存すること）
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsSphere> AddSingleSphere(const PsSphereParam& param,uint16_t index = UINT16_MAX);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単体のカプセルの追加
		@param[in]	param	作成のパラメータ
		@param[in]	index	インデックス（オブジェクトを再利用する場合）
		@return	オブジェクトのポインタ（バックアップはしないので呼び出し側で保存すること）
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsCapsule> AddSingleCapsule(const PsCapsuleParam& param, uint16_t index = UINT16_MAX);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単体のシリンダーの追加<br />
		シリンダーは、ボックスの縁からの落下時に不具合があるので、利用は慎重に！
		@param[in]	param	作成のパラメータ
		@param[in]	index	インデックス（オブジェクトを再利用する場合）
		@return	オブジェクトのポインタ（バックアップはしないので呼び出し側で保存すること）
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsCylinder> AddSingleCylinder(const PsCylinderParam& param, uint16_t index = UINT16_MAX);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のボディ数を得る
		@return	現在のボディ数
		*/
		//--------------------------------------------------------------------------------------
		uint16_t GetNumBodies() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体のステータスを得る
		@param[in]	body_index	ボディID
		@param[out]	st	ステータスを得る参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetBodyStatus(uint16_t body_index, PsBodyStatus& st) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体のステータスを設定する
		@param[in]	body_index	ボディID
		@param[out]	st	ステータスを得る参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBodyStatus(uint16_t body_index,const PsBodyUpdateStatus& st);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体を起こす（sleepが有効の場合）
		@param[in]	body_index	ボディID
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void WakeUpBody(uint16_t body_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体の位置を設定する（PsMotionTypeがMotionTypeTriggerのときのみ有効）
		@param[in]	body_index	ボディID
		@param[out]	pos	位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBodyPosition(uint16_t body_index, const bsm::Vec3& pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体の速度を設定する
		@param[in]	body_index	ボディID
		@param[out]	v	速度
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBodyLinearVelocity(uint16_t body_index, const bsm::Vec3& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体の回転速度を設定する
		@param[in]	body_index	ボディID
		@param[out]	v	速度
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBodyAngularVelocity(uint16_t body_index, const bsm::Vec3& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体のフォースを追加する
		@param[in]	body_index	ボディID
		@param[out]	v	フォース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ApplyBodyForce(uint16_t body_index, const bsm::Vec3& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体のトルクを追加する
		@param[in]	body_index	ボディID
		@param[out]	v	トルク
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ApplyBodyTorque(uint16_t body_index, const bsm::Vec3& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	形状の数を得る
		@param[in]	body_index	ボディID
		@return	形状数
		*/
		//--------------------------------------------------------------------------------------
		uint16_t GetNumShapes(uint16_t body_index)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	形状のオフセット回転とオフセット位置を得る
		@param[in]	body_index	ボディID
		@param[in]	shape_index	形状ID
		@param[out]	qt	回転を受ける参照
		@param[out]	pos	位置を受ける参照
		@return　なし
		*/
		//--------------------------------------------------------------------------------------
		void GetShapeOffsetQuatPos(uint16_t body_index, uint16_t shape_index, bsm::Quat& qt, bsm::Vec3& pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	形状のタイプを得る
		@param[in]	body_index	ボディID
		@param[in]	shape_index	形状ID
		@return　形状のタイプ
		*/
		//--------------------------------------------------------------------------------------
		sce::PhysicsEffects::ePfxShapeType GetShapeType(uint16_t body_index, uint16_t shape_index) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ジョイントの数を得る
		@return	ジョイント数
		*/
		//--------------------------------------------------------------------------------------
		uint16_t GetNumJoints()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンタクト(衝突)の数を得る
		@return	ジョイント数
		*/
		//--------------------------------------------------------------------------------------
		uint16_t GetNumContacts()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定のボディがコンタクトしている相手の配列を得る
		@param[in]	body_index	ボディID
		@param[out]	contacts	相手のIDの配列
		@return	衝突があればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetContactsVec(uint16_t body_index, vector<uint16_t>& contacts)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定のボディがコンタクトしている相手のset集合を得る
		@param[in]	body_index	ボディID
		@param[out]	contacts	相手のIDの集合
		@return	衝突があればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetContactsSet(uint16_t body_index, set<uint16_t>& contacts)const;
		//以下、直接アクセス(使用する場合は慎重に)
		//--------------------------------------------------------------------------------------
		/*!
		@brief	形状を得る
		@param[in]	body_index	ボディID
		@param[in]	shape_index	形状ID
		@return　形状の参照
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxShape& getPfxShape(uint16_t body_index, uint16_t shape_index) const;
		sce::PhysicsEffects::PfxShape& getPfxShape(uint16_t body_index, uint16_t shape_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体のステートを得る
		@param[in]	body_index	ボディID
		@return　剛体のステートの参照
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxRigidState& getPfxRigidState(uint16_t body_index) const;
		sce::PhysicsEffects::PfxRigidState& getPfxRigidState(uint16_t body_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体を得る
		@param[in]	body_index	ボディID
		@return　剛体の参照
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxRigidBody& getPfxRigidBody(uint16_t body_index) const;
		sce::PhysicsEffects::PfxRigidBody& getPfxRigidBody(uint16_t body_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突する剛体を得る
		@param[in]	body_index	ボディID
		@return　衝突する剛体の参照
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxCollidable& getPfxCollidable(uint16_t body_index) const;
		sce::PhysicsEffects::PfxCollidable& getPfxCollidable(uint16_t body_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体のソルバーを得る
		@param[in]	body_index	ボディID
		@return　剛体のソルバーの参照
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxSolverBody& getPfxSolverBody(uint16_t body_index) const;
		sce::PhysicsEffects::PfxSolverBody& getPfxSolverBody(uint16_t body_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ジョイントのペアを得る
		@param[in]	joint_index	ジョイントID
		@return　ジョイントのペアの参照
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxConstraintPair& getPfxConstraintPair(uint16_t joint_index) const;
		sce::PhysicsEffects::PfxConstraintPair& getPfxConstraintPair(uint16_t joint_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ジョイントを得る
		@param[in]	joint_index	ジョイントID
		@return　ジョイントの参照
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxJoint& getPfxJoint(uint16_t joint_index) const;
		sce::PhysicsEffects::PfxJoint& getPfxJoint(uint16_t joint_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	今のブロードフェイズペアを得る
		@param[in]	contact_index	コンタクトID
		@return　今のブロードフェイズペアの参照
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxBroadphasePair& getNowPfxBroadphasePair(uint16_t contact_index) const;
		sce::PhysicsEffects::PfxBroadphasePair& getNowPfxBroadphasePair(uint16_t contact_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1つ前のブロードフェイズペアを得る
		@param[in]	contact_index	コンタクトID
		@return　1つ前のブロードフェイズペアの参照
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxBroadphasePair& getPrevPfxBroadphasePair(uint16_t contact_index) const;
		sce::PhysicsEffects::PfxBroadphasePair& getPrevPfxBroadphasePair(uint16_t contact_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンタクトのマニホールドを得る
		@param[in]	contact_index	コンタクトID
		@return　コンタクトのマニホールドの参照
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxContactManifold& getPfxContactManifold(uint16_t contact_index) const;
		sce::PhysicsEffects::PfxContactManifold& getPfxContactManifold(uint16_t contact_index);

	};



}
//end basecross
