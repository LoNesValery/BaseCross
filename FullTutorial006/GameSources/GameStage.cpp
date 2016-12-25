/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------


	//リソースの作成
	void GameStage::CreateResourses() {
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);

		auto ModelMesh = MeshResource::CreateBoneModelMesh(DataDir,L"Chara_R.bmf");
		App::GetApp()->RegisterResource(L"Chara_R_MESH", ModelMesh);
		auto StaticModelMesh = MeshResource::CreateStaticModelMesh(DataDir, L"Chara_Rst.bmf");
		App::GetApp()->RegisterResource(L"Chara_Rst_MESH", StaticModelMesh);
	}



	//ビューとライトの作成
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrLookAtCamera = ObjectFactory::Create<LookAtCamera>();
		PtrView->SetCamera(PtrLookAtCamera);



		PtrLookAtCamera->SetEye(Vector3(0.0f, 5.0f, -5.0f));
		PtrLookAtCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
		//シングルライトの作成
		auto PtrSingleLight = CreateLight<SingleLight>();
		//ライトの設定
		PtrSingleLight->GetLight().SetPositionToDirectional(-1.0f, 1.0f, -1.0f);
//		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}


	//プレートの作成
	void GameStage::CreatePlate() {
		//ステージへのゲームオブジェクトの追加
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(50.0f, 50.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);

		auto ColPtr = Ptr->AddComponent<CollisionRect>();
		//描画コンポーネントの追加
		auto DrawComp = Ptr->AddComponent<PNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//自分に影が映りこむようにする
		DrawComp->SetOwnShadowActive(true);

		//描画コンポーネントテクスチャの設定
		DrawComp->SetTextureResource(L"SKY_TX");
	}

	//追いかけるオブジェクトの作成
	void GameStage::CreateSeekObject() {
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//配列の初期化
		vector<Vector3> Vec = {
			{ 0, 2.0f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<SeekObject>(v);
		}
	}



	//固定のボックスの作成
	void GameStage::CreateFixedBox() {
		//配列の初期化
		vector< vector<Vector3> > Vec = {
			{
				Vector3(5.0f, 0.5f, 5.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(10.0f, 0.25f, 10.0f)
			},
			{
				Vector3(5.0f, 0.5f, 5.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(14.0f, 0.25f, 10.0f)
			},

			{
				Vector3(2.0f, 1.0f, 2.0f),
				Vector3(0, 0, 0),
				Vector3(10.0f, 0.5f, 10.0f)
			},
			{
				Vector3(4.0f, 1.0f, 4.0f),
				Vector3(0, 0, 0),
				Vector3(-10.0f, 0.5f, 10.0f)
			},
			{
				Vector3(10.0f, 0.5f, 10.0f),
				Vector3(-0.5f, 0.0f, -0.5f),
				Vector3(-10.0f, 0.25f, 10.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}


	//上下移動しているボックスの作成
	void GameStage::CreateMoveBox() {
		CreateSharedObjectGroup(L"MoveBox");
		AddGameObject<MoveBox>(
			Vector3(5.0f, 0.5f, 5.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, -0.5f, 20.0f)
			);
	}

	//ヒットする球体の作成
	void GameStage::CreateSphere() {
		//配列の初期化
		vector<Vector3> Vec = {
			{ 20.0f, 0, 20.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<SphereObject>(v);
		}
	}

	//ヒットするカプセルの作成
	void GameStage::CreateCapsule() {
		//配列の初期化
		vector<Vector3> Vec = {
			{ 20.0f, 0, 0.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<CapsuleObject>(v);
		}

	}

	//でこぼこ床の作成
	void GameStage::CreateUnevenGround() {
		//でこぼこ床のデータの作成
		AddGameObject<UnevenGroundData>();
		//配列の初期化
		vector< vector<Vector3> > Vec = {
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 5.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 5.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 7.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 7.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 9.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 9.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 11.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 11.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 13.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 13.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 15.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 15.0f)
			},


			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-2.0f, 0.0f, 5.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-1.0f, 0.0f, 5.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-2.0f, 0.0f, 7.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-1.0f, 0.0f, 7.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-2.0f, 0.0f, 9.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-1.0f, 0.0f, 9.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-2.0f, 0.0f, 11.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-1.0f, 0.0f, 11.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-2.0f, 0.0f, 13.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-1.0f, 0.0f, 13.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-2.0f, 0.0f, 15.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-1.0f, 0.0f, 15.0f)
			},


			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(0.0f, 0.0f, 5.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(1.0f, 0.0f, 5.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(0.0f, 0.0f, 7.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(1.0f, 0.0f, 7.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(0.0f, 0.0f, 9.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(1.0f, 0.0f, 9.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(0.0f, 0.0f, 11.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(1.0f, 0.0f, 11.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(0.0f, 0.0f, 13.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(1.0f, 0.0f, 13.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(0.0f, 0.0f, 15.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(1.0f, 0.0f, 15.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(2.0f, 0.0f, 5.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(3.0f, 0.0f, 5.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(2.0f, 0.0f, 7.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(3.0f, 0.0f, 7.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(2.0f, 0.0f, 9.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(3.0f, 0.0f, 9.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(2.0f, 0.0f, 11.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(3.0f, 0.0f, 11.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(2.0f, 0.0f, 13.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(3.0f, 0.0f, 13.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(2.0f, 0.0f, 15.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(3.0f, 0.0f, 15.0f)
			},



		};
		//オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<UnevenGround>(v[0], v[1], v[2]);
		}

	}


	//固定のモデルの作成
	void GameStage::CreateStaticModel() {
		AddGameObject<StaticModel>(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(5.0f, 0.25f, 5.0f)
			);

	}
	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
	}


	void GameStage::OnCreate() {
		try {
			//リソースの作成
			CreateResourses();
			//ビューとライトの作成
			CreateViewLight();
			//プレートの作成
			CreatePlate();
			//固定のボックスの作成
			CreateFixedBox();
			//上下移動しているボックスの作成
			CreateMoveBox();
			//球体作成
			CreateSphere();
			//カプセルの作成
			CreateCapsule();
			//固定のモデルの作成
			CreateStaticModel();
			//追いかけるオブジェクトの作成
			CreateSeekObject();
			//でこぼこ床の作成
			CreateUnevenGround();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
