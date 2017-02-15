/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------

	//�r���[�ƃ��C�g�̍쐬
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrLookAtCamera = ObjectFactory::Create<LookAtCamera>();
		PtrView->SetCamera(PtrLookAtCamera);
		PtrLookAtCamera->SetEye(Vector3(0.0f, 5.0f, -5.0f));
		PtrLookAtCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	//�v���[�g�̃��b�V���쐬
	void GameStage::CreatePlateMesh() {
		//�v���[�g�p�̃��b�V�����쐬
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionNormalTangentTexture> new_pntnt_vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSquare(1.0f, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionNormalTangentTexture new_pntnt_v;
			new_pntnt_v.position = vertices[i].position;
			new_pntnt_v.normal = vertices[i].normal;
			if (vertices[i].textureCoordinate.x == 1.0f) {
				new_pntnt_v.textureCoordinate.x = 20.0f;
			}
			if (vertices[i].textureCoordinate.y == 1.0f) {
				new_pntnt_v.textureCoordinate.y = 20.0f;
			}
			Vector3 n = Vector3EX::Cross(new_pntnt_v.normal, Vector3(0, 1, 0));
			new_pntnt_v.tangent = n;
			new_pntnt_v.tangent.w = 0.0f;
			new_pntnt_vertices.push_back(new_pntnt_v);
		}
		MeshUtill::SetNormalTangent(new_pntnt_vertices);
		App::GetApp()->RegisterResource(L"MY_PNTnT_SQUARE", MeshResource::CreateMeshResource(new_pntnt_vertices, indices, false));
	}

	//�v���[�g�̍쐬
	void GameStage::CreatePlate() {
		//���b�V���쐬
		CreatePlateMesh();
		//�X�e�[�W�ւ̃Q�[���I�u�W�F�N�g�̒ǉ�
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quaternion Qt(Vector3(1.0f,0,0), XM_PIDIV2);
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(50.0f, 50.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);
		auto ColPtr = Ptr->AddComponent<CollisionRect>();
		//�`��R���|�[�l���g
		auto DrawComp = Ptr->AddComponent<BcPNTnTStaticDraw>();
		DrawComp->SetMeshResource(L"MY_PNTnT_SQUARE");
		DrawComp->SetTextureResource(L"GRAY_TX");
		DrawComp->SetNormalMapTextureResource(L"NORMAL3_TX");
		//���b�v���[�h
		DrawComp->SetSamplerState(SamplerState::LinearWrap);
		//�X�y�L�����[�Ȃ�
		DrawComp->DisableSpecular();
		//�t�H�O�͂��߂�
		DrawComp->SetFogEnabled(true);
		DrawComp->SetFogColor(Color4(0.3f, 0.3f,0.3f, 1.0f));
		DrawComp->SetFogStart(-10.0f);
		DrawComp->SetFogEnd(-30.0f);
		//�����ɉe���f�肱�ނ悤�ɂ���
		DrawComp->SetOwnShadowActive(true);
	}

	//�ǂ�������I�u�W�F�N�g�̍쐬
	void GameStage::CreateSeekObject() {
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//�z��̏�����
		vector<Vector3> Vec = {
			{ 0, 0.25f, 10.0f },
			{ 10.0f, 0.25f, 0.0f },
			{ -10.0f, 0.25f, 0.0f },
			{ 0, 0.25f, -10.0f },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<SeekObject>(v);
		}
	}


	struct FixedNormalBoxParam {
		wstring TexKey;
		wstring NormalTexKey;
		Vector3 Scale;
		Vector3 Rot;
		Vector3 Pos;
		FixedNormalBoxParam(const wstring& tk, const wstring& ntk, const Vector3& s, const Vector3& r, const Vector3& p) :
			TexKey(tk), NormalTexKey(ntk), Scale(s), Rot(r), Pos(p) {}
	};

	//�Œ�̖@���}�b�v�����{�b�N�X�̍쐬
	void GameStage::CreateFixedNormalBox() {
		//�z��̏�����
		vector<FixedNormalBoxParam> Vec = {
			FixedNormalBoxParam(
				L"BLUE_TX",
				L"NORMAL2_TX",
				Vector3(5.0f, 0.5f, 5.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(10.0f, 0.25f, 10.0f)
			),
			FixedNormalBoxParam(
				L"WALL2_TX",
				L"WALL2_NORMAL_TX",
				Vector3(2.0f, 1.0f, 2.0f),
				Vector3(0, 0, 0),
				Vector3(10.0f, 0.5f, 10.0f)
			),
			FixedNormalBoxParam(
				L"WALL_TX",
				L"WALL_NORMAL_TX",
				Vector3(4.0f, 1.0f, 4.0f),
				Vector3(0, 0, 0),
				Vector3(-10.0f, 0.5f, 10.0f)
			),
			FixedNormalBoxParam(
				L"BROWN_TX",
				L"FLOOR_NORMAL_TX",
				Vector3(10.0f, 0.5f, 10.0f),
				Vector3(-0.5f, 0.0f, -0.5f),
				Vector3(-10.0f, 0.25f, 10.0f)
			)
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto& v : Vec) {
			AddGameObject<FixedNormalBox>(v.TexKey,v.NormalTexKey,v.Scale,v.Rot,v.Pos);
		}
	}


	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {

		CreateSharedObjectGroup(L"AttackBall");
		//�A�^�b�N�{�[����10�p�ӂ���
		for (int i = 0; i < 10; i++) {
			AddGameObject<AttackBall>();
		}

		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	//�㉺�ړ����Ă���{�b�N�X�̍쐬
	void GameStage::CreateMoveBox() {
		CreateSharedObjectGroup(L"MoveBox");
		AddGameObject<MoveBox>(
			Vector3(5.0f, 0.5f, 5.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, -0.5f, 20.0f)
			);
	}

	//�q�b�g���鋅�̂̍쐬
	void GameStage::CreateSphere() {
		//�z��̏�����
		vector<Vector3> Vec = {
			{ 20.0f, 0, 15.0f },
			{ 20.0f, 0, 0.0f },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<SphereObject>(v);
		}
	}

	//�ł��ڂ����̍쐬
	void GameStage::CreateUnevenGround() {
		//�ł��ڂ����̃f�[�^�̍쐬
		AddGameObject<UnevenGroundData>();
		//�z��̏�����
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
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<UnevenGround>(v[0], v[1], v[2]);
		}

	}

	void GameStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�v���[�g�̍쐬
			CreatePlate();
			//�Œ�̖@���}�b�v�����{�b�N�X�̍쐬
			CreateFixedNormalBox();
			//�㉺�ړ����Ă���{�b�N�X�̍쐬
			CreateMoveBox();
			//���̍쐬
			CreateSphere();
			//�ł��ڂ����̍쐬
			CreateUnevenGround();
			//�v���[���[�̍쐬
			CreatePlayer();
			//�ǂ�������I�u�W�F�N�g�̍쐬
			CreateSeekObject();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
