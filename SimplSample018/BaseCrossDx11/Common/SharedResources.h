/*!
@file SharedResources.h
@brief 共有のリソース（ビュー、カメラ、ライトなど）
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	class GameObject;
	class GameObjectGroup;
	class Stage;

	//--------------------------------------------------------------------------------------
	///	ビュー関連
	//--------------------------------------------------------------------------------------
	struct Viewport {
		float TopLeftX;
		float TopLeftY;
		float Width;
		float Height;
		float MinDepth;
		float MaxDepth;
	};

	//--------------------------------------------------------------------------------------
	///	カメラ
	//--------------------------------------------------------------------------------------
	class Camera :public ObjectInterface, public ShapeInterface {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		explicit Camera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Camera();
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの位置を得る
		@return	カメラの位置
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetEye() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの位置を設定する
		@param[in]	Eye	カメラ位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetEye(const Vector3& Eye);
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの位置を設定する
		@param[in]	x	x位置
		@param[in]	y	y位置
		@param[in]	z	z位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetEye(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの視点を得る
		@return	カメラの視点
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetAt() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの視点を設定する
		@param[in]	At	視点位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetAt(const Vector3& At);
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの視点を設定する
		@param[in]	x	x位置
		@param[in]	y	y位置
		@param[in]	z	z位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetAt(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの傾きを得る
		@return	カメラの傾き
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetUp() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの傾きを設定する
		@param[in]	Up	傾き
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetUp(const Vector3& Up);
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの傾きを設定する
		@param[in]	x	x方向
		@param[in]	y	y方向
		@param[in]	z	z方向
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetUp(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 遠近法を使うかどうかを得る
		@return	遠近法を使えばtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsPers()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 遠近法を使うかどうかを得る
		@return	遠近法を使えばtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetPers()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 遠近法を使うかどうかを設定する
		@param[in]	b	使えばtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPers(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 画角を得る（遠近法が設定されているとき用）
		@return	画角
		*/
		//--------------------------------------------------------------------------------------
		float GetFovY() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 画角を設定する（遠近法が設定されているとき用）
		@param[in]	f	画角
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFovY(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief アスペクト比を得る（遠近法が設定されているとき用）
		@return	アスペクト比
		*/
		//--------------------------------------------------------------------------------------
		float GetAspect() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief アスペクト比を設定する（遠近法が設定されているとき用）
		@param[in]	f	アスペクト比
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAspect(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 画面の幅を得る（遠近法が設定されないとき用）
		@return	画面の幅
		*/
		//--------------------------------------------------------------------------------------
		float GetWidth() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 画面の幅を設定する（遠近法が設定されないとき用）
		@param[in]	f	画面の幅
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetWidth(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 画面の高さを得る（遠近法が設定されないとき用）
		@return	画面の高さ
		*/
		//--------------------------------------------------------------------------------------
		float GetHeight() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 画面の高さを設定する（遠近法が設定されないとき用）
		@param[in]	f	画面の高さ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetHeight(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief Near距離を得る
		@return	Near距離
		*/
		//--------------------------------------------------------------------------------------
		float GetNear() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief Near距離を設定する
		@param[in]	f	Far距離
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetNear(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief Far距離を得る
		@return	Far距離
		*/
		//--------------------------------------------------------------------------------------
		float GetFar() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief Far距離を設定する
		@param[in]	f	Far距離
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFar(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 現在のカメラオブジェクトを得る
		@return	カメラオブジェクト（設定なければnullptr）
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetCameraObject() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラオブジェクトを設定する
		@param[in]	Obj	カメラオブジェクト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetCameraObject(const shared_ptr<GameObject>& Obj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラオブジェクトをクリアする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ClearCameraObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ビューポートを設定する
		@param[in]	v	ビューポート
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetViewPort(const Viewport& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 行列を計算する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CalculateMatrix();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ビュー行列を得る
		@return	ビュー行列
		*/
		//--------------------------------------------------------------------------------------
		const Matrix4X4& GetViewMatrix() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 射影行列を得る
		@return	射影行列
		*/
		//--------------------------------------------------------------------------------------
		const Matrix4X4& GetProjMatrix() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化を行う
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
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

	//--------------------------------------------------------------------------------------
	///	LookAtカメラ（デバッグやサンプル用カメラ。コントローラによって、カメラの向いている方向に目標を移動させる時に使用）
	//--------------------------------------------------------------------------------------
	class LookAtCamera : public Camera {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		explicit LookAtCamera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~LookAtCamera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カメラの目標オブジェクトを得る
		@return	カメラの目標
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetTargetObject() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カメラの目標オブジェクトを設定する
		@param[in]	Obj	カメラの目標オブジェクト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTargetObject(const shared_ptr<GameObject>& Obj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オブジェクトを追いかける場合の補間値を得る
		@return	オブジェクトを追いかける場合の補間値
		*/
		//--------------------------------------------------------------------------------------
		float GetToTargetLerp() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オブジェクトを追いかける場合の補間値を設定する
		@param[in]	f	オブジェクトを追いかける場合の補間値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetToTargetLerp(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	EyeとAtの距離を得る
		@return	EyeとAtの距離
		*/
		//--------------------------------------------------------------------------------------
		float GetArmLengh() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	EyeとAtの距離を更新する（現在のEyeとAtから求める）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateArmLengh();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	EyeとAtの距離の最大値を得る
		@return	EyeとAtの距離の最大値
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxArm() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	EyeとAtの距離の最大値を設定する
		@param[in]	f	EyeとAtの距離の最大値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMaxArm(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	EyeとAtの距離の最小値を得る
		@return	EyeとAtの距離の最小値
		*/
		//--------------------------------------------------------------------------------------
		float GetMinArm() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	EyeとAtの距離の最小値設定する
		@param[in]	f	EyeとAtの距離の最小値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMinArm(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	回転スピードを得る
		@return	回転スピード（0.0f以上）
		*/
		//--------------------------------------------------------------------------------------
		float GetRotSpeed() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	回転スピードを設定する
		@param[in]	f	回転スピード（マイナスを入力してもプラスになる）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetRotSpeed(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ターゲットからAtへの調整ベクトルを得る
		@return	ターゲットからAtへの調整ベクトル
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetTargetToAt() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ターゲットからAtへの調整ベクトルを設定する
		@param[in]	v	ターゲットからAtへの調整ベクトルを
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTargetToAt(const Vector3& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの視点を設定する
		@param[in]	At	視点位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetAt(const Vector3& At)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの視点を設定する
		@param[in]	x	x位置
		@param[in]	y	y位置
		@param[in]	z	z位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetAt(float x, float y, float z)override;


		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	///	ビュー（親）
	//--------------------------------------------------------------------------------------
	class ViewBase :public ObjectInterface, public ShapeInterface {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		@param[in]	StagePtr	このコンポーネントを所持するステージ
		*/
		//--------------------------------------------------------------------------------------
		explicit ViewBase(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ViewBase();
	public:
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理。デフォルトは何も行わない
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画に使用するカメラを得る
		@return	描画に使用するカメラ
		*/
		//--------------------------------------------------------------------------------------
		virtual const shared_ptr<Camera>& GetTargetCamera() const = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画するビューポートを得る
		@return	描画するビューポート
		*/
		//--------------------------------------------------------------------------------------
		virtual const Viewport& GetTargetViewport() const = 0;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	///	シングルビュー
	//--------------------------------------------------------------------------------------
	class SingleView : public ViewBase {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	このコンポーネントを所持するステージ
		*/
		//--------------------------------------------------------------------------------------
		explicit SingleView(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SingleView();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ビューポートを設定する
		@param[in]	v	ビューポート
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetViewport(const Viewport& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief ビューポートを得る
		@return	ビューポート
		*/
		//--------------------------------------------------------------------------------------
		const Viewport& GetViewport() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラを設定する
		@param[in]	c	カメラのポインタ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetCamera(const shared_ptr<Camera>& c);
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラを得る
		@return	カメラのポインタ
		*/
		//--------------------------------------------------------------------------------------
		const shared_ptr<Camera>& GetCamera()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画に使用するカメラを得る
		@return	描画に使用するカメラ
		*/
		//--------------------------------------------------------------------------------------
		virtual const shared_ptr<Camera>& GetTargetCamera() const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画するビューポートを得る
		@return	描画するビューポート
		*/
		//--------------------------------------------------------------------------------------
		virtual const Viewport& GetTargetViewport() const override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化を行う
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
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

	//--------------------------------------------------------------------------------------
	//	ライト
	//--------------------------------------------------------------------------------------
	struct Light {
		Vector3 m_Directional;	//ライトの向き
		Color4 m_DiffuseColor;	//ディフィーズ色
		Color4 m_SpecularColor;	//スペキュラー色
		Light() :
			m_Directional(0, -1.0f, 0),
			m_DiffuseColor(1.0000000f, 0.9607844f, 0.8078432f, 1.0f),
			m_SpecularColor(1.0000000f, 0.9607844f, 0.8078432f, 1.0f)
		{
		}
		~Light() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ライト位置からの向きを設定する
		@param[in]	Position	ライトの位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPositionToDirectional(const Vector3& Position) {
			m_Directional = Position;
			m_Directional *= -1.0f;
			m_Directional.Normalize();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ライト位置からの向きを設定する
		@param[in]	x	ライト位置x
		@param[in]	y	ライト位置y
		@param[in]	z	ライト位置z
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPositionToDirectional(float x, float y, float z) {
			m_Directional = Vector3(x, y, z);
			m_Directional *= -1.0f;
			m_Directional.Normalize();
		}
	};

	//--------------------------------------------------------------------------------------
	///	ステージに設定するライト（親）
	//--------------------------------------------------------------------------------------
	class LightBase :public ObjectInterface {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		@param[in]	StagePtr	このコンポーネントを所持するステージ
		*/
		//--------------------------------------------------------------------------------------
		explicit LightBase(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~LightBase();
	public:
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画に使用するライトを得る
		@return	描画に使用するライト
		*/
		//--------------------------------------------------------------------------------------
		virtual const Light& GetTargetLight() const = 0;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	シングルライト
	//--------------------------------------------------------------------------------------
	class SingleLight : public LightBase {
		Light m_Light;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	このコンポーネントを所持するステージ
		*/
		//--------------------------------------------------------------------------------------
		explicit SingleLight(const shared_ptr<Stage>& StagePtr) :LightBase(StagePtr) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SingleLight() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ライトを設定する
		@param[in]	l	ライトの参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLight(const Light& l) {
			m_Light = l;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ライトを得る
		@return	ライトの参照
		*/
		//--------------------------------------------------------------------------------------
		const Light& GetLight()const {
			return m_Light;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ライトを得る
		@return	ライトの参照(更新用)
		*/
		//--------------------------------------------------------------------------------------
		Light& GetLight() {
			return m_Light;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画に使用するライトを得る
		@return	描画に使用するライト
		*/
		//--------------------------------------------------------------------------------------
		virtual const Light& GetTargetLight() const {
			return m_Light;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化を行う
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
	};



	//--------------------------------------------------------------------------------------
	//	class Path;
	//	用途: 経路クラス
	//--------------------------------------------------------------------------------------
	class Path {
		//経路のポイントのリスト
		list<Vector3> m_WayPoints;
		//現在イテレーター
		list<Vector3>::iterator m_CurWaypoint;
		//ループするかどうか
		bool m_Looped;
	public:
		//--------------------------------------------------------------------------------------
		//	Path();
		//	用途: コンストラクタ
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		Path() :
			m_Looped(false) {
		}
		//--------------------------------------------------------------------------------------
		//	virtual ~Path();
		//	用途: デストラクタ
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		virtual ~Path() {
			m_WayPoints.clear();
		}
		//--------------------------------------------------------------------------------------
		//アクセッサ
		//--------------------------------------------------------------------------------------
		bool GetLooped()const { return m_Looped; }
		list<Vector3>* GetPathPtr() { return &m_WayPoints; }
		Vector3 GetCurWaypoint()const {
			if (m_CurWaypoint == m_WayPoints.end()) {
				return m_WayPoints.back();
			}
			else {
				return *m_CurWaypoint;
			}
		}
		void SetLooped(bool b) { m_Looped = b; }
		//--------------------------------------------------------------------------------------
		//	bool IsFinished();
		//	用途: 最後まで行ってるかどうか
		//	戻り値: 最後まで行っていればtrue
		//--------------------------------------------------------------------------------------
		bool IsFinished() {
			return !(m_CurWaypoint != m_WayPoints.end());
		}
		//--------------------------------------------------------------------------------------
		//	void Clear();
		//	用途: リストをクリアする
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void Clear() { m_WayPoints.clear(); }
		//--------------------------------------------------------------------------------------
		//	void SetList(
		//	const list<Vector3> new_path	//新しいリスト
		//	);
		//	用途: リストを差し替える
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void SetList(const list<Vector3>& new_path) {
			Clear();
			auto it = new_path.begin();
			while (it != new_path.end()) {
				m_WayPoints.push_back(*it);
				it++;
			}
			m_CurWaypoint = m_WayPoints.begin();
		}
		//--------------------------------------------------------------------------------------
		//	void SetNextWaypoint();
		//	用途: 次のポイントに進める
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void SetNextWaypoint() {
			if (m_WayPoints.size() <= 0) {
				throw BaseException(
					L"リストが空です。",
					L"if(m_WayPoints.size() <= 0)",
					L"Path::SetNextWaypoint()"
				);
			}
			if (m_CurWaypoint == m_WayPoints.end()) {
				return;
			}
			if (++m_CurWaypoint == m_WayPoints.end()) {
				if (m_Looped) {
					m_CurWaypoint = m_WayPoints.begin();
				}
			}
		}
	};





	//--------------------------------------------------------------------------------------
	//	struct Steering;
	//	用途: 操舵関連ユーティリティ
	//	＊static呼び出しをする
	//--------------------------------------------------------------------------------------
	struct Steering {
		Steering() {}
		~Steering() {}
		//--------------------------------------------------------------------------------------
		//	static  bool AccumulateForce(
		//	Vector3& Force,
		//	const Vector3& ForceToAdd,
		//	float MaxForce
		//	);
		//	用途: 操舵力を加算する
		//	戻り値: 加算されればtrue
		//	＊Forceに計算結果を加算する
		//--------------------------------------------------------------------------------------
		static bool AccumulateForce(Vector3& Force, const Vector3& ForceToAdd, float MaxForce);
		//--------------------------------------------------------------------------------------
		//	static Vector3 Seek(
		//	const Vector3& Velocity,	//現在の速度
		//	const Vector3& Target,		//目標
		//	const Vector3& Pos,			//現在の位置
		//	float MaxSpeed				//最高速度
		//	);
		//	用途: 探索行動
		//	戻り値: フォース
		//--------------------------------------------------------------------------------------
		static Vector3 Seek(const Vector3& Velocity, const Vector3& Target, const Vector3& Pos, float MaxSpeed);
		//--------------------------------------------------------------------------------------
		//	static Vector3 Flee(
		//	const Vector3& Velocity,	//現在の速度
		//	const Vector3& Target,		//目標
		//	const Vector3& Pos,			//現在の位置
		//	float MaxSpeed,				//最高速度
		//	float PanicDistance			//逃げ出す平方距離範囲の1辺
		//	);
		//	用途: 逃走行動
		//	戻り値: フォース
		//	＊ターゲットが PanicDistance * PanicDistanceの距離範囲に入ったら逃げ出す
		//--------------------------------------------------------------------------------------
		static Vector3 Flee(const Vector3& Velocity, const Vector3& Target,
			const Vector3& Pos, float MaxSpeed, float PanicDistance);
		//--------------------------------------------------------------------------------------
		//	static Vector3 Arrive(
		//	const Vector3& Velocity,	//現在の速度
		//	const Vector3& Target,		//目標
		//	const Vector3& Pos,			//現在の位置
		//	float MaxSpeed				//最高速度
		//	float Decl					//減速値（1.0から3.0の間程度）
		//	);
		//	用途: 到着行動
		//	戻り値: フォース
		//--------------------------------------------------------------------------------------
		static Vector3 Arrive(const Vector3& Velocity, const Vector3& Target,
			const Vector3& Pos, float MaxSpeed, float Decl);
		//--------------------------------------------------------------------------------------
		//	static Vector3 Pursuit(
		//	const Vector3& Velocity,	//現在の速度
		//	const Vector3& Pos,			//現在の位置
		//	const Vector3& Rot,			//現在の向き
		//	float MaxSpeed,				//最高速度
		//	const Vector3& TargetVelocity,	//目標の現在の速度
		//	const Vector3& Target,		//目標
		//	const Vector3& TargetRot	//目標の向き
		//	);
		//	用途: 追跡行動
		//	戻り値: フォース
		//--------------------------------------------------------------------------------------
		static Vector3 Pursuit(const Vector3& Velocity, const Vector3& Pos, const Vector3& Rot, float MaxSpeed,
			const Vector3& TargetVelocity, const Vector3& Target, const Vector3& TargetRot);
		//--------------------------------------------------------------------------------------
		//	static Vector3 Wander(
		//	const Matrix4X4 Matrix,		//現在の行列
		//	float WanderRadius,	//徘徊する半径
		//	float WanderDistance,	//徘徊する半径までの距離
		//	float WanderJitter,	//ランダム変異の最大値
		//	Vector3& WanderTarget	//徘徊のターゲット変数（回転値）
		//	);
		//	用途: 徘徊行動
		//	戻り値: フォース
		//--------------------------------------------------------------------------------------
		static Vector3 Wander(const Matrix4X4 Matrix,
			float WanderRadius, float WanderDistance, float WanderJitter, Vector3& WanderTarget);
		//--------------------------------------------------------------------------------------
		//	static Vector3 ObstacleAvoidance(
		//	const Matrix4X4 Matrix,		//現在の行列
		//	const Vector3& Velocity,	//現在の速度
		//	float MaxSpeed,				//最高速度
		//	float Width,				//回避する道幅
		//	float Height,				//回避する高さ
		//	const vector<SPHERE>& SphereVec	//回避すべき球体の配列
		//	);
		//	用途: 障害物回避行動
		//	戻り値: フォース
		//--------------------------------------------------------------------------------------
		static Vector3 ObstacleAvoidance(const Matrix4X4 Matrix,
			const Vector3& Velocity, float MaxSpeed, float Width, float Height,
			const vector<SPHERE>& SphereVec);
		//--------------------------------------------------------------------------------------
		//	static Vector3 WallAvoidance(
		//	const Matrix4X4 Matrix,		//現在の行列
		//	const Vector3& Velocity,	//現在の速度
		//	float MaxSpeed,				//最高速度
		//	const vector<PLANE>& PlaneVec	//回避すべき球体の配列
		//	);
		//	用途: 壁回避行動
		//	戻り値: フォース
		//--------------------------------------------------------------------------------------
		static Vector3 WallAvoidance(const Matrix4X4 Matrix,
			const Vector3& Velocity, float MaxSpeed, const vector<PLANE>& PlaneVec);
		//--------------------------------------------------------------------------------------
		//	static Vector3 FollowPath(
		//	Path& rPath,				//パス
		//	float WaypointSeekDist,		//Seek開始までの距離
		//	const Vector3& Pos,			//現在の位置
		//	const Vector3& Velocity,	//現在の速度
		//	float MaxSpeed,				//最高速度
		//	float Decl					//減速値（1.0から3.0の間程度）
		//	);
		//	用途: 経路行動
		//	戻り値: フォース
		//--------------------------------------------------------------------------------------
		static Vector3 FollowPath(Path& rPath,
			float WaypointSeekDist,
			const Vector3& Pos, const Vector3& Velocity,
			float MaxSpeed, float Decl);
		//--------------------------------------------------------------------------------------
		//	static Vector3 Separation(
		//	const shared_ptr<GameObjectGroup>& Group,	//設定するグループの配列
		//	const shared_ptr<GameObject>& MyObj				//自分自身
		//	);
		//	用途: 分離行動
		//	戻り値: フォース
		//--------------------------------------------------------------------------------------
		static Vector3 Separation(const shared_ptr<GameObjectGroup>& Group, const shared_ptr<GameObject>& MyObj);
		//--------------------------------------------------------------------------------------
		//	static Vector3 Alignment(
		//	const shared_ptr<GameObjectGroup>& Group,	//設定するグループの配列
		//	const shared_ptr<GameObject>& MyObj				//自分自身
		//	);
		//	用途: 整列行動
		//	戻り値: フォース
		//--------------------------------------------------------------------------------------
		static Vector3 Alignment(const shared_ptr<GameObjectGroup>& Group, const shared_ptr<GameObject>& MyObj);
		//--------------------------------------------------------------------------------------
		//	static Vector3 Cohesion(
		//	const shared_ptr<GameObjectGroup>& Group,	//設定するグループの配列
		//	const shared_ptr<GameObject>& MyObj			//自分自身
		//	const Vector3& Velocity,	//現在の速度
		//	float MaxSpeed				//最高速度
		//	);
		//	用途: 結合行動
		//	戻り値: フォース
		//--------------------------------------------------------------------------------------
		static Vector3 Cohesion(const shared_ptr<GameObjectGroup>& Group, const shared_ptr<GameObject>& MyObj,
			const Vector3& Velocity, float MaxSpeed);




	};



}
//end basecross
