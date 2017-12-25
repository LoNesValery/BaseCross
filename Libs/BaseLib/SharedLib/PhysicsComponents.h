/*!
@file PhysicsManager.h
@brief 物理計算用マネージャなど
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	物理計算コンポーネントの親
	//--------------------------------------------------------------------------------------
	class PsBodyComponent : public Component {
		//自動的にTransformコンポーネントを反映するかどうか
		bool m_AutoTransform;
	protected:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PsBodyComponent(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PsBodyComponent();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ワイアフレームの描画
		@param[in]	res	メッシュリソース
		@param[in]	world	ワールド行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void DrawShapeWireFrame(const shared_ptr<MeshResource>& res, const bsm::Mat4x4& world);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief 自動的にTransformを変更するかどうかを得る
		@return	自動的にTransformを変更するかどうか
		*/
		//--------------------------------------------------------------------------------------
		bool IsAutoTransform() const {
			return m_AutoTransform;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 自動的にTransformを変更するかどうかを設定する
		@param[in]	b	自動的にTransformを変更するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAutoTransform(bool b) {
			m_AutoTransform = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体のステータスを得る
		@param[out]	st	ステータスを得る参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetStatus(PsBodyStatus& st) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体の位置を得る
		@return	剛体の位置
		*/
		//--------------------------------------------------------------------------------------
		bsm::Vec3	GetPosition() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体の姿勢を得る
		@return	剛体の姿勢
		*/
		//--------------------------------------------------------------------------------------
		bsm::Quat	GetOrientation() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体の速度を得る
		@return	剛体の速度
		*/
		//--------------------------------------------------------------------------------------
		bsm::Vec3	GetLinearVelocity()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体の回転速度を得る
		@return	剛体の回転速度
		*/
		//--------------------------------------------------------------------------------------
		bsm::Vec3	GetAngularVelocity()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体のステータスを設定する
		@param[out]	st	ステータスを得る参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetStatus(const PsBodyUpdateStatus& st);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体を起こす（sleepが有効の場合）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void WakeUp();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体の位置を設定する（PsMotionTypeがMotionTypeTriggerのときのみ有効）
		@param[in]	pos	新しい位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPosition(const bsm::Vec3& pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体の速度を設定する
		@param[in]	v	速度
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLinearVelocity(const bsm::Vec3& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体の回転速度を設定する
		@param[in]	v	速度
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAngularVelocity(const bsm::Vec3& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体のフォースを追加する
		@param[in]	v	フォース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ApplyForce(const bsm::Vec3& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体のトルクを追加する
		@param[in]	v	トルク
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ApplyTorque(const bsm::Vec3& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体インデックスを得る（純粋仮想関数）
		@return	剛体インデックス
		*/
		//--------------------------------------------------------------------------------------
		virtual uint16_t GetIndex() const = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ボディがコンタクト（衝突）している相手の配列(インデックス)を得る
		@param[out]	contacts	相手のIDの配列
		@return	衝突があればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetContactsVec(vector<uint16_t>& contacts)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ボディがコンタクト（衝突）している相手のset集合を得る
		@param[out]	contacts	相手のIDのset集合
		@return	衝突があればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetContactsSet(set<uint16_t>& contacts)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ボディがコンタクト（衝突）している相手の配列(ゲームオブジェクト)を得る
		@param[out]	objects	相手のゲームオブジェクトの配列
		@return	衝突があればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetContactGameObjects(vector<shared_ptr<GameObject>>& objects)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンとの衝突判定（簡易）
		@param[in]	src	コリジョン球体
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual bool CollisionTestBase(const SPHERE& src) { return false; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンとの衝突判定（簡易）
		@param[in]	src	コリジョンカプセル
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual bool CollisionTestBase(const CAPSULE& src) { return false; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンとの衝突判定（簡易）
		@param[in]	src	コリジョンOBB
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual bool CollisionTestBase(const OBB& src) { return false; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンとの衝突判定（呼び出しテンプレート）
		@tparam	T	コリジョン型
		@param[in]	src	コリジョンOBB
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		bool CollisionTest(const T& src) {
			return CollisionTestBase(src);
		}
	};

	//--------------------------------------------------------------------------------------
	///	単体の球体コンポーネント
	//--------------------------------------------------------------------------------------
	class PsSingleSphereBody : public PsBodyComponent {
		//物理計算用のオブジェクト
		shared_ptr<PhysicsSphere> m_PhysicsSphere;
		//ワイフレフレームメッシュ
		shared_ptr<MeshResource> m_SphereMeshRes;
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		@param[in]	param	球体の作成パラメータ
		*/
		//--------------------------------------------------------------------------------------
		explicit PsSingleSphereBody(const shared_ptr<GameObject>& GameObjectPtr,const PsSphereParam& param);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PsSingleSphereBody() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体インデックスを得る
		@return	剛体インデックス
		*/
		//--------------------------------------------------------------------------------------
		virtual uint16_t GetIndex() const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	再利用して剛体をリセットする
		@param[in]	param	球体の作成パラメータ
		@param[in]	index	インデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Reset(const PsSphereParam& param, uint16_t index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンとの衝突判定（簡易）
		@param[in]	src	コリジョン球体
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual bool CollisionTestBase(const SPHERE& src)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理。
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	単体のボックスコンポーネント
	//--------------------------------------------------------------------------------------
	class PsSingleBoxBody : public PsBodyComponent {
		//物理計算用のオブジェクト
		shared_ptr<PhysicsBox> m_PhysicsBox;
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		@param[in]	param	の作成パラメータ
		*/
		//--------------------------------------------------------------------------------------
		explicit PsSingleBoxBody(const shared_ptr<GameObject>& GameObjectPtr,const PsBoxParam& param);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PsSingleBoxBody() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体インデックスを得る
		@return	剛体インデックス
		*/
		//--------------------------------------------------------------------------------------
		virtual uint16_t GetIndex() const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	再利用して剛体をリセットする
		@param[in]	param	ボックスの作成パラメータ
		@param[in]	index	インデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Reset(const PsBoxParam& param, uint16_t index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンとの衝突判定（簡易）
		@param[in]	src	コリジョン球体
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual bool CollisionTestBase(const SPHERE& src)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理。
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;

	};

	//--------------------------------------------------------------------------------------
	///	単体のカプセルコンポーネント
	//--------------------------------------------------------------------------------------
	class PsSingleCapsuleBody : public PsBodyComponent {
		//物理計算用のオブジェクト
		shared_ptr<PhysicsCapsule> m_PhysicsCapsule;
		//ワイアフレーム用メッシュ（カプセルは各コンポーネントが保持）
		shared_ptr<MeshResource> m_CapsuleMesh;
		void CreateMesh(const PsCapsuleParam& param);
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		@param[in]	param	の作成パラメータ
		*/
		//--------------------------------------------------------------------------------------
		explicit PsSingleCapsuleBody(const shared_ptr<GameObject>& GameObjectPtr, const PsCapsuleParam& param);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PsSingleCapsuleBody() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体インデックスを得る
		@return	剛体インデックス
		*/
		//--------------------------------------------------------------------------------------
		virtual uint16_t GetIndex() const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	再利用して剛体をリセットする
		@param[in]	param	ボックスの作成パラメータ
		@param[in]	index	インデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Reset(const PsCapsuleParam& param, uint16_t index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンとの衝突判定（簡易）
		@param[in]	src	コリジョン球体
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual bool CollisionTestBase(const SPHERE& src)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理。
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	///	単体のシリンダーコンポーネント。シリンダーは、ボックスの縁からの落下時に不具合があるので、利用は慎重に！
	//--------------------------------------------------------------------------------------
	class PsSingleCylinderBody : public PsBodyComponent {
		//物理計算用のオブジェクト
		shared_ptr<PhysicsCylinder> m_PhysicsCylinder;
		//ワイアフレーム用メッシュ（シリンダーは各コンポーネントが保持）
		shared_ptr<MeshResource> m_CylinderMesh;
		void CreateMesh(const PsCylinderParam& param);
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		@param[in]	param	の作成パラメータ
		*/
		//--------------------------------------------------------------------------------------
		explicit PsSingleCylinderBody(const shared_ptr<GameObject>& GameObjectPtr, const PsCylinderParam& param);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PsSingleCylinderBody() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体インデックスを得る
		@return	剛体インデックス
		*/
		//--------------------------------------------------------------------------------------
		virtual uint16_t GetIndex() const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	再利用して剛体をリセットする
		@param[in]	param	ボックスの作成パラメータ
		@param[in]	index	インデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Reset(const PsCylinderParam& param, uint16_t index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理。
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};




}
// end basecross

