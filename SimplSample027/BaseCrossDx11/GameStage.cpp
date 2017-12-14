/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�Q�[���X�e�[�W
	//--------------------------------------------------------------------------------------
	GameStage::GameStage()
	{
	}

	void GameStage::RegisterNormalBox() {
		if (!App::GetApp()->CheckResource<MeshResource>(L"NORMAL_BOX")) {
			vector<VertexPositionNormalTexture> vertices;
			vector<VertexPositionNormalTangentTexture> new_vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateCube(1.0f, vertices, indices);
			BcRenderer::ConvertToNormalVertex(vertices, new_vertices);
			App::GetApp()->RegisterResource(L"NORMAL_BOX",
				MeshResource::CreateMeshResource(new_vertices, indices, false));
		}
	}

	void GameStage::OnCreate() {

		if (!App::GetApp()->GetScene<Scene>()->IsFirstGame()) {
			SetCamera(App::GetApp()->GetScene<Scene>()->GetBackupCamera());
		}

		//�V���h�E�}�b�v�̕`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->GetShadowMapRenderTarget(2048.0f);

		//�����g�p����@���t���{�b�N�X�̓o�^�i���\�[�X�o�^����j
		RegisterNormalBox();

		//���ʂ̍쐬
		Quat Qt;
		Qt.rotationX(XM_PIDIV2);
		AddGameObject<SquareObject>(
			L"WALL_TX",
			L"WALL_NORMAL_TX",
			Vec3(50.0f, 50.0f, 1.0f),
			Qt,
			Vec3(0.0f, 0.0f, 0.0f)
			);


		Quat Qt1, Qt2, Qt3, Qt4;
		Qt1.rotationZ(2.0f);
		Qt2.rotationX(0.7f);
		Qt3.rotationZ(-2.0f);
		Qt4.rotationX(-0.7f);
		//���̑�
		AddGameObject<FixedPsBox>(Vec3(15.0f, 0.25f, 15.0f), Quat(), Vec3(0.0f, 0.125f, 10.0f));

		//�ォ��~���Ă���{�b�N�X
		AddGameObject<ActivePsBox>(Vec3(2.0f, 1.0f, 1.0f), Quat(), Vec3(2.0f, 1.0f, 9.0f));
		AddGameObject<ActivePsBox>(Vec3(2.0f, 1.0f, 1.0f), Qt2, Vec3(1.0f, 3.0f, 8.0f));
		AddGameObject<ActivePsBox>(Vec3(2.0f, 1.0f, 1.0f), Qt3, Vec3(-2.0f, 4.0f, 8.5f));
		AddGameObject<ActivePsBox>(Vec3(1.0f, 1.0f, 2.0f), Qt3, Vec3(2.0f, 10.0f, 7.0f));
		AddGameObject<ActivePsBox>(Vec3(1.0f, 1.0f, 1.0f), Qt4, Vec3(1.0f, 11.0f, 13.0f));
		AddGameObject<ActivePsBox>(Vec3(1.0f, 1.0f, 2.0f), Qt1, Vec3(2.5f, 12.0f, 14.0f));
		AddGameObject<ActivePsBox>(Vec3(1.0f, 1.0f, 1.0f), Qt2, Vec3(-3.0f, 13.0f, 7.0f));
		AddGameObject<ActivePsBox>(Vec3(1.0f, 1.0f, 1.0f), Qt3, Vec3(-3.0f, 14.0f, 6.0f));
		AddGameObject<ActivePsBox>(Vec3(2.0f, 1.0f, 1.0f), Qt4, Vec3(2.5f, 15.0f, 15.0f));
		//�ォ��~���Ă��鋅��
		AddGameObject<ActivePsSphere>(1.0f, Quat(), Vec3(0.0f, 6.0f, 10.0f));
		AddGameObject<ActivePsSphere>(1.0f, Quat(), Vec3(-0.5f, 7.0f, 11.0f));
		AddGameObject<ActivePsSphere>(1.0f, Quat(), Vec3(0.0f, 8.0f, 10.5f));


		//�v���C���[�̍쐬
		auto PtrPlayer = AddGameObject<Player>(
			L"TRACE_TX", 
			true, 
			Vec3(0.0f, 0.125f, 0.0f)
			);
		SetPlayer(PtrPlayer);

		//�X�p�[�N�G�t�F�N�g
		AddGameObject<MultiSpark>();
		//�t�@�C�A�G�t�F�N�g
		AddGameObject<MultiFire>();

		//�`��I�u�W�F�N�g�̒ǉ�
		CreateDrawObjects();


		//��]����X�v���C�g�̍쐬
		AddGameObject<RotateSprite>(
			L"TRACE_TX",
			Vec2(160, 160),
			0.0f,
			Vec2(-480, 260),
			4, 4
			);


		//���b�Z�[�W��\������X�v���C�g�̍쐬
		AddGameObject<MessageSprite>(
			L"MESSAGE_TX",
			Vec2(256, 64),
			0.0f,
			Vec2(480, 260),
			1, 1
			);

		//������`��I�u�W�F�N�g�̍쐬
		AddGameObject<StringDrawObject>();


	}

	//�`��I�u�W�F�N�g�̒ǉ�
	void GameStage::CreateDrawObjects() {
		//�V���h�E�}�b�v�`��I�u�W�F�N�g�̍쐬
		AddGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");

		//SimplePCStaticRenderer�`��I�u�W�F�N�g�̍쐬
		AddGameObject<SimplePCStaticRenderer>(L"SimplePCStaticRenderer");

		//SimplePCTStaticRenderer�`��I�u�W�F�N�g�̍쐬
		AddGameObject<SimplePCTStaticRenderer>(L"SimplePCTStaticRenderer");

		//SimplePNTStaticRenderer�`��I�u�W�F�N�g�̍쐬
		AddGameObject<SimplePNTStaticRenderer>(L"SimplePNTStaticRenderer");
		//SimplePNTStaticRenderer2�`��I�u�W�F�N�g�̍쐬
		AddGameObject<SimplePNTStaticRenderer2>(L"SimplePNTStaticRenderer2");

		//SimplePNTStaticModelRenderer�`��I�u�W�F�N�g�̍쐬
		AddGameObject<SimplePNTStaticModelRenderer>(L"SimplePNTStaticModelRenderer");
		//SimplePNTStaticModelRenderer2�`��I�u�W�F�N�g�̍쐬
		AddGameObject<SimplePNTStaticModelRenderer2>(L"SimplePNTStaticModelRenderer2");

		//SimplePNTBoneModelRenderer�`��I�u�W�F�N�g�̍쐬
		AddGameObject<SimplePNTBoneModelRenderer>(L"SimplePNTBoneModelRenderer");
		//SimplePNTBoneModelRenderer2�`��I�u�W�F�N�g�̍쐬
		AddGameObject<SimplePNTBoneModelRenderer2>(L"SimplePNTBoneModelRenderer2");


		//BcPNTStaticDrawObject�`��I�u�W�F�N�g�̍쐬
		AddGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
		//BcPNTnTStaticDrawObject�`��I�u�W�F�N�g�̍쐬
		AddGameObject<BcPNTnTStaticRenderer>(L"BcPNTnTStaticRenderer");

		//BcPNTStaticModelDrawObject�`��I�u�W�F�N�g�̍쐬
		AddGameObject<BcPNTStaticModelRenderer>(L"BcPNTStaticModelRenderer");
		//BcPNTnTStaticModelDrawObject�`��I�u�W�F�N�g�̍쐬
		AddGameObject<BcPNTnTStaticModelRenderer>(L"BcPNTnTStaticModelRenderer");

		//BcPNTBoneModelDrawObject�`��I�u�W�F�N�g�̍쐬
		AddGameObject<BcPNTBoneModelRenderer>(L"BcPNTBoneModelRenderer");
		//BcPNTnTBoneModelDrawObject�`��I�u�W�F�N�g�̍쐬
		AddGameObject<BcPNTnTBoneModelRenderer>(L"BcPNTnTBoneModelRenderer");

		//Particle�`��I�u�W�F�N�g�̍쐬(���Z�������Ȃ�)
		AddGameObject<ParticleDrawObject>(false);
		//Particle�`��I�u�W�F�N�g�̍쐬(���Z��������)
		AddGameObject<ParticleDrawObject>(true);

	}


	void GameStage::OnUpdateStage() {
		//�^�[�����̏�����
		GetRigidbodyManager()->InitRigidbody();
		//�����}�l�[�W���̍X�V
		GetPhysicsManager()->OnUpdate();
		for (auto& v : GetGameObjectVec()) {
			//�e�I�u�W�F�N�g�̍X�V
			v->OnUpdate();
		}
		//Rigidbody�}�l�[�W���̍X�V�i�Փ˔���Ȃǁj
		GetRigidbodyManager()->OnUpdate();
		for (auto& v : GetGameObjectVec()) {
			//�e�I�u�W�F�N�g�̍ŏI�X�V
			v->OnUpdate2();
		}
		//�������g�̍X�V(�J����)
		this->OnUpdate();
		//Rigidbody�}�l�[�W���̍ŏI�X�V�i�Փ˔�����̃N���A�j
		GetRigidbodyManager()->OnUpdate2();
	}


	void GameStage::OnUpdate() {
		auto& camera = GetCamera();
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {

			//D�p�b�h��
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				//�J�����ʒu������
				camera.m_CameraArmLen += 0.1f;
				if (GetCamera().m_CameraArmLen >= 50.0f) {
					GetCamera().m_CameraArmLen = 50.0f;
				}
			}
			//D�p�b�h��
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				//�J�����ʒu�����
				camera.m_CameraArmLen -= 0.1f;
				if (GetCamera().m_CameraArmLen <= 2.0f) {
					camera.m_CameraArmLen = 2.0f;
				}
			}

			if (CntlVec[0].fThumbRX != 0) {
				camera.m_CameraXZRad += CntlVec[0].fThumbRX * 0.02f;
				if (abs(camera.m_CameraXZRad) >= XM_2PI) {
					camera.m_CameraXZRad = 0;
				}
			}
			if (CntlVec[0].fThumbRY != 0) {
				camera.m_CameraYRad -= CntlVec[0].fThumbRY * 0.02f;
				if (camera.m_CameraYRad >= XM_PIDIV2 - 0.1f) {
					camera.m_CameraYRad = XM_PIDIV2 - 0.1f;
				}
				else if (camera.m_CameraYRad <= 0.2) {
					camera.m_CameraYRad = 0.2;
				}
			}

			camera.m_CamerAt = FindTagGameObject<GameObject>(L"Player")->GetPosition();
			Vec3 CameraLocalEye =
				Vec3(
					sin(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad),
					cos(camera.m_CameraYRad) * camera.m_CameraArmLen,
					-cos(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad)
				);
			camera.m_CamerEye = camera.m_CamerAt + CameraLocalEye;
			//B�{�^��
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				auto Pos = GetPlayer()->GetPosition();
				App::GetApp()->GetScene<Scene>()->SetBackups(GetCamera(),Pos);
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToEmptyStage");
			}
		}

		//�I�u�W�F�N�g��
		auto ObjCount = GetGameObjectVec().size();
		wstring OBJ_COUNT(L"OBJ_COUNT: ");
		OBJ_COUNT += Util::UintToWStr(ObjCount);
		OBJ_COUNT += L"\n";

		wstring Mess(L"A�{�^���W�����v�B�ړ����Ȃ���X�͋�����\n");

		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\nElapsedTime: ";
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		FPS += Util::FloatToWStr(ElapsedTime);
		FPS += L"\n";

		Mess += FPS;
		Mess += OBJ_COUNT;

		if (!m_StringDrawObject) {
			m_StringDrawObject = FindTagGameObject<StringDrawObject>(L"StringDrawObject");
		}

		m_StringDrawObject->SetText(Mess);
	}

	void GameStage::OnDrawStage() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));

		//�V���h�I�E�}�b�v�`��̊J�n
		Dev->ClearShadowmapViews();
		Dev->StartShadowmapDraw();
		for (auto& v : GetGameObjectVec()) {
			//�e�I�u�W�F�N�g�̕`��
			v->OnDrawShadowmap();
		}
		Dev->EndShadowmapDraw();

		//�f�t�H���g�`��̊J�n
		Dev->StartDefaultDraw();
		for (auto& v : GetGameObjectVec()) {
			//�e�I�u�W�F�N�g�̕`��
			v->OnDraw();
		}
		//�����}�l�[�W���̃��C�A�t���[���`��
		GetPhysicsManager()->OnDraw();
		//�������g�̕`��
		this->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefaultDraw();
	}


	void GameStage::OnDraw() {
		m_RigidbodyManager->OnDraw();
	}

	//--------------------------------------------------------------------------------------
	///	��̃X�e�[�W�i���b�Z�[�W�݂̂���j
	//--------------------------------------------------------------------------------------
	void EmptyStage::OnCreate() {
		//���b�Z�[�W�X�v���C�g
		m_MessageSprite = ObjectFactory::Create<MessageSprite>(
			GetThis<Stage>(),
			L"MESSAGE_TX",
			Vec2(256, 64),
			0.0f,
			Vec2(0, 0),
			1, 1);
	}
	void EmptyStage::OnUpdateStage() {
		//�X�v���C�g�̍X�V
		m_MessageSprite->OnUpdate();
		//�������g�̍X�V
		this->OnUpdate();
	}
	void EmptyStage::OnUpdate() {
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//B�{�^��
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
			}
		}
	}

	void EmptyStage::OnDrawStage() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefaultDraw();
		//�X�v���C�g�`��
		m_MessageSprite->OnDraw();
		//�������g�̕`��
		this->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefaultDraw();
	}


	void EmptyStage::OnDraw() {
		//�������Ȃ�
	}


}
//end basecross
