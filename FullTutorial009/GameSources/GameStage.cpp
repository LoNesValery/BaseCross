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
		//サンプルのためアセットディレクトリを取得
		App::GetApp()->GetAssetsDirectory(DataDir);
		//各ゲームは以下のようにデータディレクトリを取得すべき
		//App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"trace2.png";
		App::GetApp()->RegisterTexture(L"TRACE2_TX", strTexture);
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
	}



	//ビューとライトの作成
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrLookAtCamera = ObjectFactory::Create<LookAtCamera>();
		PtrView->SetCamera(PtrLookAtCamera);

		PtrLookAtCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		PtrLookAtCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}


	//プレートの作成
	void GameStage::CreatePlate() {
		//ステージへのゲームオブジェクトの追加
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(20.0f, 40.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 20.0f);
		auto ColPtr = Ptr->AddComponent<CollisionRect>();
		//描画コンポーネントの追加
		auto DrawComp = Ptr->AddComponent<BcPNTStaticDraw>();
		DrawComp->SetFogEnabled(true);
		//描画コンポーネントに形状（メッシュ）を設定
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//自分に影が映りこむようにする
		DrawComp->SetOwnShadowActive(true);
		//描画コンポーネントテクスチャの設定
		DrawComp->SetTextureResource(L"SKY_TX");
	}

	//セルマップの作成
	void GameStage::CreateStageCellMap() {
		auto Group = CreateSharedObjectGroup(L"CellMap");
		float  PieceSize = 1.0f;
		auto Ptr = AddGameObject<StageCellMap>(Vec3(-10.0f, 0, 4.0f),PieceSize,20,7);
		//セルマップの区画を表示する場合は以下の設定
		Ptr->SetDrawActive(true);
		//さらにセルのインデックスとコストを表示する場合は以下の設定
		Ptr->SetCellStringActive(true);
		SetSharedGameObject(L"StageCellMap1", Ptr);
		//グループに追加
		Group->IntoGroup(Ptr);

		Ptr = AddGameObject<StageCellMap>(Vec3(-10.0f, 0, 16.0f), PieceSize, 20, 7);
		//セルマップの区画を表示する場合は以下の設定
		Ptr->SetDrawActive(true);
		//さらにセルのインデックスとコストを表示する場合は以下の設定
		//Ptr->SetCellStringActive(true);
		SetSharedGameObject(L"StageCellMap2", Ptr);
		//グループに追加
		Group->IntoGroup(Ptr);

		//以下3つ目のセルマップはグループを別にする
		//動的にセルマップを変更する敵用
		auto Group2 = CreateSharedObjectGroup(L"CellMap2");

		Ptr = AddGameObject<StageCellMap>(Vec3(-10.0f, 0, 28.0f), PieceSize, 20, 7);
		//セルマップの区画を表示する場合は以下の設定
		Ptr->SetDrawActive(true);
		//さらにセルのインデックスとコストを表示する場合は以下の設定
		Ptr->SetCellStringActive(true);
		SetSharedGameObject(L"StageCellMap3", Ptr);
		//グループに追加
		Group2->IntoGroup(Ptr);


	}



	//固定のボックスの作成
	void GameStage::CreateFixedBox() {
		//配列の初期化
		vector< vector<Vec3> > Vec = {
			{
				Vec3(1.0f, 0.5f, 40.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(9.5f, 0.25f, 20.0f)
			},
			{
				Vec3(1.0f, 0.5f, 40.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-9.5f, 0.25f, 20.0f)
			},

			{
				Vec3(20.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.25f, 0.5f)
			},

			{
				Vec3(20.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.25f, 39.5f)
			},

			///////


			{
				Vec3(2.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-8.0f, 0.25f, 6.5f)
			},

			{
				Vec3(1.0f, 0.5f, 2.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-4.5f, 0.25f, 7.0f)
			},

			{
				Vec3(2.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.25f, 7.5f)
			},

			{
				Vec3(1.0f, 0.5f, 2.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(4.5f, 0.25f, 7.0f)
			},


			{
				Vec3(2.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(8.0f, 0.25f, 6.5f)
			},


			///////


			{
				Vec3(2.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-8.0f, 0.25f,18.5f)
			},

			{
				Vec3(1.0f, 0.5f, 2.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-4.5f, 0.25f, 19.0f)
			},

			{
				Vec3(2.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.25f, 19.5f)
			},

			{
				Vec3(1.0f, 0.5f, 2.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(4.5f, 0.25f, 19.0f)
			},


			{
				Vec3(2.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(8.0f, 0.25f, 18.5f)
			},

			///////


			{
				Vec3(2.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-8.0f, 0.25f, 30.5f)
			},

			{
				Vec3(1.0f, 0.5f, 2.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-4.5f, 0.25f, 31.0f)
			},

			{
				Vec3(2.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.25f, 31.5f)
			},

			{
				Vec3(1.0f, 0.5f, 2.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(4.5f, 0.25f, 31.0f)
			},


			{
				Vec3(2.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(8.0f, 0.25f, 30.5f)
			},
		};

		//ボックスのグループを作成
		auto BoxGroup = CreateSharedObjectGroup(L"FixedBoxes");
		//オブジェクトの作成
		for (auto v : Vec) {
			auto BoxPtr = AddGameObject<FixedBox>(v[0], v[1], v[2]);
			//ボックスのグループに追加
			BoxGroup->IntoGroup(BoxPtr);
		}
		//最初の2つのセルマップへのボックスのコスト設定
		SetCellMapCost(L"CellMap");
		//奥のセルマップへのボックスのコスト設定
		SetCellMapCost(L"CellMap2");
	}

	//固定のボックスのコストをセルマップに反映
	void GameStage::SetCellMapCost(const wstring& CellMapGroupName) {
		//セルマップ内にFixedBoxの情報をセット
		auto Group = GetSharedObjectGroup(CellMapGroupName);
		auto BoxGroup = GetSharedObjectGroup(L"FixedBoxes");

		//セルマップグループを取得
		for (auto& gv : Group->GetGroupVector()) {
			auto MapPtr = dynamic_pointer_cast<StageCellMap>(gv.lock());
			if (MapPtr) {
				//セルマップからセルの配列を取得
				auto& CellVec = MapPtr->GetCellVec();
				//ボックスグループからボックスの配列を取得
				auto& BoxVec = BoxGroup->GetGroupVector();
				vector<AABB> ObjectsAABBVec;
				for (auto& v : BoxVec) {
					auto FixedBoxPtr = dynamic_pointer_cast<FixedBox>(v.lock());
					if (FixedBoxPtr) {
						auto ColPtr = FixedBoxPtr->GetComponent<CollisionObb>();
						//ボックスの衝突判定かラッピングするAABBを取得して保存
						ObjectsAABBVec.push_back(ColPtr->GetWrappingAABB());
					}
				}
				//セル配列からセルをスキャン
				for (auto& v : CellVec) {
					for (auto& v2 : v) {
						for (auto& vObj : ObjectsAABBVec) {
							if (HitTest::AABB_AABB_NOT_EQUAL(v2.m_PieceRange, vObj)) {
								//ボックスのABBとNOT_EQUALで衝突判定
								v2.m_Cost = -1;
								break;
							}
						}
					}
				}
			}
		}
	}


	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		CreateSharedObjectGroup(L"AttackBall");
		//アタックボールは10個用意する
		for (int i = 0; i < 10; i++) {
			AddGameObject<AttackBall>();
		}
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>(Vec3(0.0f, 0.125f, 2.0f));
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	//敵の作成
	void GameStage::CreateEnemy() {
		auto MapPtr = GetSharedGameObject<StageCellMap>(L"StageCellMap1");
		vector< vector<Vec3> > vec1 = {
			{
				Vec3(0.25f, 0.25f, 0.25f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-2.5f, 0.125f, 9.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec1) {
			AddGameObject<Enemy>(MapPtr,v[0], v[1], v[2]);
		}

		MapPtr = GetSharedGameObject<StageCellMap>(L"StageCellMap2");
		vector< vector<Vec3> > vec2 = {
			{
				Vec3(0.25f, 0.25f, 0.25f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(4.5f, 0.125f, 21.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec2) {
			AddGameObject<Enemy>(MapPtr, v[0], v[1], v[2]);
		}

		//3つ目の敵

		MapPtr = GetSharedGameObject<StageCellMap>(L"StageCellMap3");
		vector< vector<Vec3> > vec3 = {
			{
				Vec3(0.25f, 0.25f, 0.25f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(6.5f, 0.125f, 33.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : vec3) {
			//セルマップを変更する敵
			AddGameObject<TestCellChangeEnemy>(MapPtr, v[0], v[1], v[2]);
		}
	}


	void GameStage::OnCreate() {
		try {
			//リソースの作成
			CreateResourses();
			//ビューとライトの作成
			CreateViewLight();
			//プレートの作成
			CreatePlate();
			//セルマップの作成
			CreateStageCellMap();
			//固定のボックスの作成
			CreateFixedBox();
			//プレーヤーの作成
			CreatePlayer();
			//敵の作成
			CreateEnemy();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
