/*!
@file Components.h
@brief コンポーネント
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	class GameObject;
	class Stage;

	//--------------------------------------------------------------------------------------
	///	コンポーネント親クラス
	//--------------------------------------------------------------------------------------
	class Component :public ObjectInterface, public ShapeInterface {
	protected:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit Component(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Component();
	public:
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトの取得
		@return	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetGameObject() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトを設定する
		@param[in]	GameObjectPtr	新しいゲームオブジェクト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AttachGameObject(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステージの取得<br />
		ゲームオブジェクトがステージだった場合、そのステージが返る
		@return	このコンポーネントを所持するゲームオブジェクトが所属するステージ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetStage() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Updateがアクティブかどうかを得る
		@return	アクティブならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsUpdateActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Updateがアクティブかどうかを得る
		@return	アクティブならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetUpdateActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Updateがアクティブかどうかを設定する
		@param[in]	b	アクティブかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetUpdateActive(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Drawがアクティブかどうかを得る
		@return	アクティブならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsDrawActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Drawがアクティブかどうかを得る
		@return	アクティブならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetDrawActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Drawがアクティブかどうかを設定する
		@param[in]	b	アクティブかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDrawActive(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化を行う（仮想関数<br />
		＊thisポインタが必要なオブジェクトはこの関数を多重定義して、取得できる<br />
		＊ここでは空関数にしておくので、必要な場合は派生クラスで多重定義する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() {}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	変換コンポーネント
	//--------------------------------------------------------------------------------------
	class Transform : public Component {
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit Transform(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Transform();
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1ターン前のスケーリングを得る
		@return	1ターン前のスケーリング
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetBeforeScale() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1つ前の重心を得る
		@return	1つ前の重心
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetBeforePivot() const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	1ターン前のワールド回転を得る
		@return	1ターン前のワールド回転
		*/
		//--------------------------------------------------------------------------------------
		Quaternion GetBeforeQuaternion() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1ターン前のワールド回転を得る
		@return	1ターン前のワールド回転（ベクトル）
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetBeforeRotation() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1ターン前のワールド位置を得る
		@return	1ターン前のワールド位置
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetBeforePosition() const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	1つ前のワールド行列を得る
		@return	1つ前のワールド行列
		*/
		//--------------------------------------------------------------------------------------
		Matrix4X4 GetBeforeWorldMatrix() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スケーリングを得る
		@return	スケーリング
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetScale() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スケーリングを設定する.
		@param	const Vector3& Scale	スケーリング
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetScale(const Vector3& Scale);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スケーリングを設定する.
		@param[in]	x	Xスケーリング
		@param[in]	y	Yスケーリング
		@param[in]	z	Zスケーリング
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetScale(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	重心を得る
		@return	重心
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetPivot() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	重心を設定する.
		@param	Pivot	重心
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPivot(const Vector3& Pivot);

		//--------------------------------------------------------------------------------------
		/*!
		@brief	重心を設定する.
		@param[in]	x	X重心
		@param[in]	y	Y重心
		@param[in]	z	Z重心
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPivot(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワールド回転を得る
		@return	ワールド回転（クオータニオン）
		*/
		//--------------------------------------------------------------------------------------
		Quaternion GetQuaternion() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワールド回転を設定する
		@param[in]	quaternion	クオータニオン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetQuaternion(const Quaternion& quaternion);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワールド回転を得る
		@return	ワールド回転（ベクトル）
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetRotation() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワールド回転を設定する
		@param[in]	Rot	回転ベクトル
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetRotation(const Vector3& Rot);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワールド回転を設定する
		@param[in]	x	X回転
		@param[in]	y	Y回転
		@param[in]	z	Z回転
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetRotation(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワールド位置を得る
		@return	ワールド位置
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetPosition() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワールド位置を設定する
		@param[in]	Position	設定する位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPosition(const Vector3& Position);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワールド位置を設定する
		@param[in]	x	X位置
		@param[in]	y	Y位置
		@param[in]	z	Z位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPosition(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワールド行列を得る
		@return	ワールド行列
		*/
		//--------------------------------------------------------------------------------------
		Matrix4X4 GetWorldMatrix() const; 
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Transfoem上で計算した速度を得る
		@return	Transfoem上で計算した速度
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetVelocity() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 情報を一つ前に移す<br />
		各情報はターンごとに1つ前のターン時の情報を持つ<br />
		この関数はその処理を行う
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetToBefore();
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理。デフォルトは何も行わない
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




}
//end basecross
