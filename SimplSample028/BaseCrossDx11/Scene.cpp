/*!
@file Scene.cpp
@brief �V�[���Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�X�e�[�W�i�V�[���ŊǗ�����C���^�[�t�F�C�X�j
	//--------------------------------------------------------------------------------------
	void Stage::OnPreCreate() {
		//Rigidbody�}�l�[�W���̏�����
		m_RigidbodyManager
			= ObjectFactory::Create<RigidbodyManager>(GetThis<Stage>());
	}


	//�ǉ��I�u�W�F�N�g�̎w��
	void Stage::PushBackGameObject(const shared_ptr<GameObject>& Ptr) {
		//���̃X�e�[�W�̓N���G�C�g��ł���
		if (IsCreated()) {
			m_WaitAddObjectVec.push_back(Ptr);
		}
		else {
			//�N���G�C�g�O
			m_GameObjectVec.push_back(Ptr);
		}
	}
	//�폜�I�u�W�F�N�g�̎w��
	void Stage::RemoveBackGameObject(const shared_ptr<GameObject>& Ptr) {
		m_WaitRemoveObjectVec.push_back(Ptr);
	}
	//�I�u�W�F�N�g�̍폜
	void Stage::RemoveTargetGameObject(const shared_ptr<GameObject>& targetobj) {
		auto it = m_GameObjectVec.begin();
		while (it != m_GameObjectVec.end()) {
			if (*it == targetobj) {
				m_GameObjectVec.erase(it);
				return;
			}
			it++;
		}
		m_RigidbodyManager->RemoveOwnRigidbody(targetobj);
	}
	//�ǉ���폜�҂��ɂȂ��Ă�I�u�W�F�N�g��ǉ��폜����
	void Stage::SetWaitToObjectVec() {
		if (!m_WaitRemoveObjectVec.empty()) {
			for (auto Ptr : m_WaitRemoveObjectVec) {
				RemoveTargetGameObject(Ptr);
			}
		}
		m_WaitRemoveObjectVec.clear();
		if (!m_WaitAddObjectVec.empty()) {
			for (auto Ptr : m_WaitAddObjectVec) {
				m_GameObjectVec.push_back(Ptr);
			}
		}
		m_WaitAddObjectVec.clear();
	}

	void Stage::FindTagGameObjectVec(const wstring& TagName, vector<shared_ptr<GameObject>>& Ret) const {
		Ret.clear();
		for (auto& v : GetGameObjectVec()) {
			if (v->FindTag(TagName)) {
				Ret.push_back(v);
			}
		}
	}

	shared_ptr<RigidbodyManager> Stage::GetRigidbodyManager() const {
		return m_RigidbodyManager;
	}

	shared_ptr<Rigidbody> Stage::AddRigidbody(const Rigidbody& body) {
		return m_RigidbodyManager->AddRigidbody(body);
	}

	void Stage::RemoveOwnRigidbody(const shared_ptr<GameObject>& OwnerPtr) {
		m_RigidbodyManager->RemoveOwnRigidbody(OwnerPtr);
	}

	const vector<shared_ptr<Rigidbody>>& Stage::GetRigidbodyVec()const {
		return m_RigidbodyManager->GetRigidbodyVec();
	}
	const vector<CollisionState>& Stage::GetCollisionStateVec()const {
		return m_RigidbodyManager->GetCollisionStateVec();
	}
	shared_ptr<Rigidbody> Stage::GetOwnRigidbody(const shared_ptr<GameObject>& OwnerPtr) {
		return m_RigidbodyManager->GetOwnRigidbody(OwnerPtr);
	}




	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	Scene::Scene() :
		SceneInterface()
	{
	}

	void Scene::CreateResources() {
		wstring DataDir;
		//�T���v���̂��߃A�Z�b�g�f�B���N�g�����擾
		App::GetApp()->GetAssetsDirectory(DataDir);
		//�e�Q�[���͈ȉ��̂悤�Ƀf�[�^�f�B���N�g�����擾���ׂ�
		//App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"StageMessage.png";
		App::GetApp()->RegisterTexture(L"MESSAGE_TX", strTexture);

		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		strTexture = DataDir + L"wall_normal.png";
		App::GetApp()->RegisterTexture(L"WALL_NORMAL_TX", strTexture);

		//�{�[�����f���̃^���W�F���g�t�����\�[�X
		auto ModelMesh = MeshResource::CreateBoneModelMeshWithTangent(DataDir, L"TangentTest.bmf");
		App::GetApp()->RegisterResource(L"TANGENT_TEST_MESH", ModelMesh);
		//�@���}�b�v
		strTexture = DataDir + L"NormalMap.png";
		App::GetApp()->RegisterTexture(L"TANGENT_TEST_TX", strTexture);

		//�G�t�F�N�g
		strTexture = DataDir + L"spark.png";
		App::GetApp()->RegisterTexture(L"SPARK_TX", strTexture);
		strTexture = DataDir + L"fire.png";
		App::GetApp()->RegisterTexture(L"FIRE_TX", strTexture);


		//�T�E���h
		wstring CursorWav = DataDir + L"cursor.wav";
		App::GetApp()->RegisterWav(L"cursor", CursorWav);

		//BGM
		wstring strMusic = DataDir + L"nanika .wav";
		App::GetApp()->RegisterWav(L"Nanika", strMusic);



	}


	void Scene::OnCreate() {
		CreateResources();

		//BGM�̍Đ�
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		XAPtr->Start(L"Nanika", XAUDIO2_LOOP_INFINITE, 0.1f);

		//�������g�ɃC�x���g�𑗂�
		//����ɂ��e�X�e�[�W��I�u�W�F�N�g��Create���ɃV�[���ɃA�N�Z�X�ł���
		PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
	}

	void Scene::OnUpdate() {
		GetActiveStage()->OnPreUpdateStage();
		GetActiveStage()->OnUpdateStage();
	}
	void Scene::OnDraw() {
		GetActiveStage()->OnDrawStage();
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//�A�N�e�B�u�X�e�[�W��GameStage�ɐݒ�
			ResetActiveStage<GameStage>();
		}
		else if (event->m_MsgStr == L"ToEmptyStage") {
			//�A�N�e�B�u�X�e�[�W��EmptyStage�ɐݒ�
			ResetActiveStage<EmptyStage>();
		}
	}

	void Scene::OnDestroy() {

		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		XAPtr->Stop(m_BGM);

	}

}
//end basecross
