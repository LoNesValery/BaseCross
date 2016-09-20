/*!
@file GameObject.cpp
@brief �Q�[���I�u�W�F�N�g�A�X�e�[�W����
*/

#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct GameObject::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct GameObject::Impl {
		bool m_UpdateActive;	//update���邩�ǂ���
		bool m_DrawActive;		//Draw���邩�ǂ���
		bool m_AlphaActive;		//�������ǂ���
		int m_DrawLayer;	//�`�惌�C���[
		bool m_SpriteDraw;	//�X�v���C�g�Ƃ��ĕ`�悷�邩�ǂ���

		weak_ptr<Stage> m_Stage;	//�����X�e�[�W
		map<type_index, shared_ptr<Component> > m_CompMap;
		list<type_index> m_CompOrder;	//�R���|�[�l���g���s����

		shared_ptr<Rigidbody> m_Rigidbody;	//Rigidbody�͕ʂɂ���
		shared_ptr<Gravity> m_Gravity;	//Gravity�͕ʂɂ���
		shared_ptr<Collision> m_Collision;	//Collision���ʂɂ���
		shared_ptr<Transform> m_Transform;	//Transform���ʂɂ���


		Impl(const shared_ptr<Stage>& StagePtr) :
			m_UpdateActive(true),
			m_DrawActive(true),
			m_AlphaActive(false),
			m_DrawLayer(0),
			m_SpriteDraw(false),
			m_Stage(StagePtr)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	�Q�[���z�u�I�u�W�F�N�g�e�N���X
	//--------------------------------------------------------------------------------------
	//private�����o
	shared_ptr<Component> GameObject::SearchComponent(type_index TypeIndex)const {
		auto it = pImpl->m_CompMap.find(TypeIndex);
		if (it != pImpl->m_CompMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	shared_ptr<Transform> GameObject::GetTransform()const {
		return pImpl->m_Transform;

	}
	shared_ptr<Rigidbody> GameObject::GetRigidbody()const {
		return pImpl->m_Rigidbody;
	}
	shared_ptr<Gravity> GameObject::GetGravity()const {
		return pImpl->m_Gravity;
	}
	shared_ptr<Collision> GameObject::GetCollision()const {
		return pImpl->m_Collision;
	}
	shared_ptr<CollisionSphere> GameObject::GetCollisionSphere()const {
		return dynamic_pointer_cast<CollisionSphere>(pImpl->m_Collision);
	}

	shared_ptr<CollisionCapsule> GameObject::GetCollisionCapsule()const {
		return dynamic_pointer_cast<CollisionCapsule>(pImpl->m_Collision);
	}
	shared_ptr<CollisionObb> GameObject::GetCollisionObb()const {
		return dynamic_pointer_cast<CollisionObb>(pImpl->m_Collision);
	}

	void GameObject::SetRigidbody(const shared_ptr<Rigidbody>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Rigidbody = Ptr;
	}
	void GameObject::SetGravity(const shared_ptr<Gravity>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Gravity = Ptr;
	}
	void GameObject::SetCollision(const shared_ptr<Collision>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}
	void GameObject::SetCollisionSphere(const shared_ptr<CollisionSphere>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}

	void GameObject::SetCollisionCapsule(const shared_ptr<CollisionCapsule>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}

	void GameObject::SetCollisionObb(const shared_ptr<CollisionObb>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}

	void GameObject::SetTransform(const shared_ptr<Transform>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Transform = Ptr;
	}

	void GameObject::AddMakedComponent(type_index TypeIndex, const shared_ptr<Component>& Ptr) {
		if (!SearchComponent(TypeIndex)) {
			//���̃R���|�[�l���g���܂��Ȃ���ΐV�K�o�^
			pImpl->m_CompOrder.push_back(TypeIndex);
		}
		//map�ɒǉ��������͍X�V
		pImpl->m_CompMap[TypeIndex] = Ptr;
		Ptr->AttachGameObject(GetThis<GameObject>());
	}

	map<type_index, shared_ptr<Component> >& GameObject::GetCompoMap()const {
		return pImpl->m_CompMap;
	}

	void GameObject::RemoveTgtComponent(type_index TypeIndex) {
		//���ԃ��X�g�����؂��č폜
		auto it = pImpl->m_CompOrder.begin();
		while (it != pImpl->m_CompOrder.end()) {
			if (*it == TypeIndex) {
				auto it2 = pImpl->m_CompMap.find(*it);
				if (it2 != pImpl->m_CompMap.end()) {
					//�w��̌^�̃R���|�[�l���g����������
					//map�f�[�^���폜
					pImpl->m_CompMap.erase(it2);
				}
				pImpl->m_CompOrder.erase(it);
				return;
			}
			it++;
		}
	}



	GameObject::GameObject(const shared_ptr<Stage>& StagePtr) :
		ObjectInterface(),
		ShapeInterface(),
		pImpl(new Impl(StagePtr))
	{
	}
	GameObject::~GameObject() {}
	//�A�N�Z�T
	bool GameObject::IsUpdateActive() const { return pImpl->m_UpdateActive; }
	bool GameObject::GetUpdateActive() const { return pImpl->m_UpdateActive; }
	void GameObject::SetUpdateActive(bool b) { pImpl->m_UpdateActive = b; }

	bool GameObject::IsDrawActive() const { return pImpl->m_DrawActive; }
	bool GameObject::GetDrawActive() const { return pImpl->m_DrawActive; }
	void GameObject::SetDrawActive(bool b) { pImpl->m_DrawActive = b; }

	bool GameObject::IsAlphaActive() const { return pImpl->m_AlphaActive; }
	bool GameObject::GetAlphaActive() const { return pImpl->m_AlphaActive; }
	void GameObject::SetAlphaActive(bool b) { pImpl->m_AlphaActive = b; }


	//�X�v���C�g�Ƃ���Draw���邩�ǂ���
	bool  GameObject::IsSpriteDraw() const {return pImpl->m_SpriteDraw;}
	bool  GameObject::GetSpriteDraw() const {return pImpl->m_SpriteDraw;}
	void  GameObject::SetSpriteDraw(bool b) {pImpl->m_SpriteDraw = b;}
	//�`�惌�C���[�̎擾�Ɛݒ�
	int GameObject::GetDrawLayer() const {return pImpl->m_DrawLayer;}
	void  GameObject::SetDrawLayer(int l) {pImpl->m_DrawLayer = l;}

	shared_ptr<Stage> GameObject::GetStage(bool ExceptionActive) const {
		auto shptr = pImpl->m_Stage.lock();
		if (shptr) {
			return shptr;
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"�����X�e�[�W��null�ł��B�������g���X�e�[�W�ł͂���܂��񂩁H",
					L"if (!shptr)",
					L"GameObject::GetStage()"
				);
			}
			else {
				return nullptr;
			}
		}
		// �����X�e�[�W��null������
		// �������g���X�e�[�W�̉\��������
		return nullptr;
	}
	void GameObject::SetStage(const shared_ptr<Stage>& stage) { pImpl->m_Stage = stage; }

	void GameObject::ComponentUpdate() {
		auto TMptr = GetComponent<Transform>();
		auto RigidPtr = GetComponent<Rigidbody>(false);
		auto GravityPtr = GetComponent<Gravity>(false);
		if (RigidPtr) {
			//Rigidbody������΃t�H�[�X��������
			RigidPtr->SetForce(0, 0, 0);
		}
		//�}�b�v�����؂���Update
		list<type_index>::iterator it = pImpl->m_CompOrder.begin();
		while (it != pImpl->m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = pImpl->m_CompMap.find(*it);
			if (it2 != pImpl->m_CompMap.end()) {
				//�w��̌^�̃R���|�[�l���g����������
				if (it2->second->IsUpdateActive()) {
					it2->second->OnUpdate();
				}
			}
			it++;
		}
		if (GravityPtr && GravityPtr->IsUpdateActive()) {
			//GravityPtr�������Update()
			GravityPtr->OnUpdate();
		}
		if (RigidPtr && RigidPtr->IsUpdateActive()) {
			//Rigidbody�������Update()
			RigidPtr->OnUpdate();
		}
		//Transform��Update
		if (TMptr->IsUpdateActive()) {
			TMptr->OnUpdate();
		}
	}

	void GameObject::CollisionReset() {
		auto CollisionPtr = GetComponent<Collision>(false);
		if (CollisionPtr) {
			CollisionPtr->ClearHitObject();
		}
	}

	void GameObject::CollisionChk() {
		auto CollisionPtr = GetComponent<Collision>(false);
		if (CollisionPtr && CollisionPtr->IsUpdateActive()) {
			//Collision�������Update()
			CollisionPtr->OnUpdate();
		}
	}

	void GameObject::CollisionGravityChk() {
		auto CollisionPtr = GetComponent<Collision>(false);
		if (CollisionPtr && !CollisionPtr->IsOnObject()) {
			auto GravityPtr = GetComponent<Gravity>(false);
			if (GravityPtr) {
				GravityPtr->SetGravityDefault();
			}
		}
	}

	void GameObject::ToMessageCollision() {
		auto CollisionPtr = GetComponent<Collision>(false);
		if (CollisionPtr && CollisionPtr->GetHitObjectVec().size() > 0) {
			OnCollision(CollisionPtr->GetHitObjectVec());
		}
	}

	void GameObject::DrawShadowmap() {
		auto shadowptr = GetDynamicComponent<Shadowmap>(false);
		if (shadowptr) {
			shadowptr->OnDraw();
		}
	}

	void GameObject::ComponentDraw() {
		//Transform���Ȃ���Η�O
		auto Tptr = GetComponent<Transform>();
		auto RigidPtr = GetComponent<Rigidbody>(false);
		auto GravityPtr = GetComponent<Gravity>(false);
		//�}�b�v�����؂���Draw
		list<type_index>::iterator it = pImpl->m_CompOrder.begin();
		while (it != pImpl->m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = pImpl->m_CompMap.find(*it);
			//�w��̌^�̃R���|�[�l���g����������
			if (it2 != pImpl->m_CompMap.end() && !dynamic_pointer_cast<Shadowmap>(it2->second)) {
				//�V���h�E�}�b�v�ȊO�Ȃ���s
				//���̃R���|�[�l���g�̎q�R���|�[�l���g�̕`��
				if (it2->second->IsDrawActive()) {
					it2->second->OnDraw();
				}
			}
			it++;
		}
		//�h���N���X�΍�
		if (GravityPtr && GravityPtr->IsDrawActive()) {
			//GravityPtr�������Update()
			GravityPtr->OnDraw();
		}
		if (RigidPtr && RigidPtr->IsDrawActive()) {
			//Rigidbody�������Update()
			RigidPtr->OnDraw();
		}
		//Transform��Draw
		if (Tptr->IsDrawActive()) {
			Tptr->OnDraw();
		}
	}

	void GameObject::OnPreCreate() {
		//Transform�K�{
		AddComponent<Transform>();
	}

	void GameObject::OnDraw() {
		//�R���|�[�l���g�`��
		//�h���N���X�ő��d��`����ꍇ��
		//�R���|�[�l���g�`�悷��ꍇ��
		//GameObject::Draw()���Ăяo��
		ComponentDraw();
	}


	//--------------------------------------------------------------------------------------
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct GameObjectGroup::Impl {
		vector< weak_ptr<GameObject> > m_Group;
		Impl() {}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class GameObjectGroup;
	//--------------------------------------------------------------------------------------
	GameObjectGroup::GameObjectGroup() :
		ObjectInterface(),
		pImpl(new Impl())
	{}
	GameObjectGroup::~GameObjectGroup() {}
	//�A�N�Z�T
	const vector< weak_ptr<GameObject> >& GameObjectGroup::GetGroupVector() const {
		return pImpl->m_Group;
	}
	shared_ptr<GameObject> GameObjectGroup::at(size_t index) {
		if (index >= pImpl->m_Group.size()) {
			wstring msg = Util::UintToWStr(index);
			msg += L" >= ";
			msg += Util::UintToWStr(pImpl->m_Group.size());
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				msg,
				L"GameObjectGroup::at()"
			);
		}
		if (pImpl->m_Group.at(index).expired()) {
			wstring msg = Util::UintToWStr(index);
			throw BaseException(
				L"���̃C���f�b�N�X�̃I�u�W�F�N�g�͖����ł��B",
				msg,
				L"GameObjectGroup::at()"
			);
		}
		return pImpl->m_Group.at(index).lock();
	}
	size_t GameObjectGroup::size() const {
		return pImpl->m_Group.size();
	}
	//����
	void GameObjectGroup::IntoGroup(const shared_ptr<GameObject>& Obj) {
		pImpl->m_Group.push_back(Obj);
	}

	void GameObjectGroup::AllClear() {
		pImpl->m_Group.clear();
	}



	//--------------------------------------------------------------------------------------
	//	struct Stage::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct Stage::Impl {
		bool m_UpdateActive;	//update���邩�ǂ���

		//�I�u�W�F�N�g�̔z��
		vector< shared_ptr<GameObject> > m_GameObjectVec;
		//�r���ɃI�u�W�F�N�g���ǉ����ꂽ�ꍇ�A�^�[���̊J�n�܂ő҂z��
		vector< shared_ptr<GameObject> > m_WaitAddObjectVec;
		//����Draw����Ă���r���[�̃C���f�b�N�X
		size_t m_DrawViewIndex;
		//�r���[�̃|�C���^
		shared_ptr<ViewBase> m_ViewBase;
		//���C�g�̃|�C���^
		shared_ptr<LightBase> m_LightBase;

		//�V�F�A�I�u�W�F�N�g�|�C���^�̃}�b�v
		map<const wstring, weak_ptr<GameObject> > m_SharedMap;
		//�V�F�A�O���[�v�̃|�C���^�̃}�b�v
		map<const wstring, shared_ptr<GameObjectGroup> >  m_SharedGroupMap;


		vector< shared_ptr<Stage> > m_ChildStageVec;	//�q���X�e�[�W�̔z��
		weak_ptr<Stage> m_ParentStage;		//�e�X�e�[�W



		Impl() :
			m_UpdateActive(true),
			m_DrawViewIndex(0)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	Stage::Stage() :
		ObjectInterface(),
		ShapeInterface(),
		pImpl(new Impl())
	{}
	Stage::~Stage() {}

	//�v���C�x�[�g�T�u�֐�
	void Stage::PushBackGameObject(const shared_ptr<GameObject>& Ptr) {
		//���̃X�e�[�W�̓N���G�C�g��ł���
		if (IsCreated()) {
			pImpl->m_WaitAddObjectVec.push_back(Ptr);
		}
		else {
			//�N���G�C�g�O
			pImpl->m_GameObjectVec.push_back(Ptr);
		}
	}

	shared_ptr<GameObject> Stage::GetSharedGameObjectEx(const wstring& Key, bool ExceptionActive)const {
		map<const wstring, weak_ptr<GameObject> >::const_iterator it;
		//�d���L�[�̌���
		it = pImpl->m_SharedMap.find(Key);
		if (it != pImpl->m_SharedMap.end()) {
			if (it->second.expired()) {
				//���łɖ���
				if (ExceptionActive) {
					//��O����
					wstring keyerr = Key;
					throw BaseException(
						L"�I�u�W�F�N�g�������ł�",
						keyerr,
						L"Stage::GetSharedGameObject()"
					);
				}
			}
			return it->second.lock();
		}
		else {
			//�w��̖��O��������Ȃ�����
			if (ExceptionActive) {
				//��O����
				wstring keyerr = Key;
				throw BaseException(
					L"�I�u�W�F�N�g��������܂���",
					keyerr,
					L"Stage::GetSharedGameObject()"
				);
			}
		}
		return nullptr;
	}


	vector< shared_ptr<GameObject> >& Stage::GetGameObjectVec() { return pImpl->m_GameObjectVec; }

	shared_ptr<GameObject> Stage::GetSharedObject(const wstring& Key, bool ExceptionActive)const {
		shared_ptr<GameObject> Ptr = GetSharedGameObjectEx(Key, ExceptionActive);
		return Ptr;
	}


	void Stage::SetSharedGameObject(const wstring& Key, const shared_ptr<GameObject>& Ptr) {
		map<const wstring, weak_ptr<GameObject> >::iterator it;
		//�d���L�[�̌���
		it = pImpl->m_SharedMap.find(Key);
		if (it != pImpl->m_SharedMap.end()) {
			//���ɑ��݂���
			//��O����
			wstring keyerr = Key;
			throw BaseException(
				L"�����̃V�F�A�I�u�W�F�N�g������܂�",
				keyerr,
				L"Stage::SetSharedGameObjectEx()"
			);
		}
		else {
			pImpl->m_SharedMap[Key] = Ptr;
		}
	}

	shared_ptr<GameObjectGroup> Stage::CreateSharedObjectGroup(const wstring& Key) {
		try {
			map<const wstring, shared_ptr<GameObjectGroup> >::iterator it;
			//�d���L�[�̌���
			it = pImpl->m_SharedGroupMap.find(Key);
			if (it != pImpl->m_SharedGroupMap.end()) {
				//���ɑ��݂���
				//��O����
				wstring keyerr = Key;
				throw BaseException(
					L"�����̃V�F�A�I�u�W�F�N�g�z�񂪂���܂�",
					keyerr,
					L"Stage::CreateSharedObjectGroup()"
				);
			}
			else {
				auto Ptr = ObjectFactory::Create<GameObjectGroup>();
				pImpl->m_SharedGroupMap[Key] = Ptr;
				return Ptr;
			}
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<GameObjectGroup> Stage::GetSharedObjectGroup(const wstring& Key, bool ExceptionActive)const {
		//�d���L�[�̌���
		auto it = pImpl->m_SharedGroupMap.find(Key);
		if (it != pImpl->m_SharedGroupMap.end()) {
			//�y�A��second��Ԃ�
			return it->second;
		}
		else {
			//�w��̖��O��������Ȃ�����
			if (ExceptionActive) {
				//��O����
				wstring keyerr = Key;
				throw BaseException(
					L"�w��̃L�[��������܂���",
					keyerr,
					L"Stage::GetSharedObjectGroup() const"
				);
			}
		}
		return nullptr;
	}

	void Stage::SetSharedObjectGroup(const wstring& Key, const shared_ptr<GameObjectGroup>& NewPtr) {
		//�d���L�[�̌���
		auto it = pImpl->m_SharedGroupMap.find(Key);
		if (it != pImpl->m_SharedGroupMap.end()) {
			//��O����
			wstring keyerr = Key;
			throw BaseException(
				L"�����̃V�F�A�I�u�W�F�N�g�z�񂪂���܂�",
				keyerr,
				L"Stage::SetSharedObjectGroup()"
			);
		}
		else {
			//�w��̖��O��������Ȃ�����
			//�o�^�ł���
			pImpl->m_SharedGroupMap[Key] = NewPtr;
		}
	}


	vector< shared_ptr<Stage> >& Stage::GetChileStageVec() {
		return pImpl->m_ChildStageVec;
	}
	void Stage::AddChileStageBase(const shared_ptr<Stage>& ChildStage) {
		pImpl->m_ChildStageVec.push_back(ChildStage);
		ChildStage->SetParentStage(GetThis<Stage>());
	}

	shared_ptr<Stage> Stage::GetParentStage() const {
		if (!pImpl->m_ParentStage.expired()) {
			return  pImpl->m_ParentStage.lock();
		}
		return nullptr;
	}
	void Stage::SetParentStage(const shared_ptr<Stage>& ParentStage) {
		pImpl->m_ParentStage = ParentStage;
	}


	void Stage::SetView(const shared_ptr<ViewBase>& v) {
		pImpl->m_ViewBase = v;
	}

	const shared_ptr<ViewBase>& Stage::GetView()const {
		if (!pImpl->m_ViewBase) {
			throw BaseException(
				L"�X�e�[�W�Ƀr���[���ݒ肳��Ă��܂���B",
				L"if (!pImpl->m_ViewBase)",
				L"Stage::GetView()"
			);
		}
		return pImpl->m_ViewBase;
	}

	void Stage::SetLight(const shared_ptr<LightBase>& L) {
		pImpl->m_LightBase = L;

	}
	const shared_ptr<LightBase>& Stage::GetLight()const {
		if (!pImpl->m_LightBase) {
			throw BaseException(
				L"�X�e�[�W�Ƀ��C�g���ݒ肳��Ă��܂���B",
				L"if (!pImpl->m_LightBase)",
				L"Stage::GetLight()"
			);
		}
		return pImpl->m_LightBase;
	}


	//�A�N�Z�T
	bool Stage::IsUpdateActive() const { return pImpl->m_UpdateActive; }
	bool Stage::GetUpdateActive() const { return pImpl->m_UpdateActive; }
	void Stage::SetUpdateActive(bool b) { pImpl->m_UpdateActive = b; }




	//�X�e�[�W���̍X�V�i�V�[�������΂��j
	void Stage::UpdateStage() {
		//Transform�R���|�[�l���g�̒l���o�b�N�A�b�v�ɃR�s�[
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				auto ptr2 = ptr->GetComponent<Transform>();
				ptr2->SetToBefore();
			}
		}
		//�z�u�I�u�W�F�N�g�̍X�V1
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->OnUpdate();
			}
		}
		//���g�̍X�V1
		if (IsUpdateActive()) {
			OnUpdate();
		}
		//�z�u�I�u�W�F�N�g�̃R���|�[�l���g�X�V1
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->ComponentUpdate();
			}
		}
		//�Փ˔���̍X�V�i�X�e�[�W����Ăԁj
		UpdateCollision();
		//�Փ˂ɂ��d�͂̕ω��̍X�V
		UpdateCollisionGravity();
		//���g�̃r���[���A�b�v�f�[�g
		if (IsUpdateActive() && pImpl->m_ViewBase) {
			pImpl->m_ViewBase->OnUpdate();
		}
		//�Փ˔���̃��b�Z�[�W���s�i�X�e�[�W����Ăԁj
		UpdateMessageCollision();
		//�z�u�I�u�W�F�N�g�̃R���|�[�l���g�X�V3
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->OnLastUpdate();
			}
		}
		//���g�̍X�V3
		if (IsUpdateActive()) {
			OnLastUpdate();
		}

		//�R���W�����̃��Z�b�g
		for (auto ptr : GetGameObjectVec()) {
			ptr->CollisionReset();
		}


		//�q���X�e�[�W�̍X�V
		for (auto PtrChileStage : GetChileStageVec()) {
			PtrChileStage->UpdateStage();
		}
	}

	//�Փ˔���̍X�V�i�X�e�[�W����Ăԁj
	void Stage::UpdateCollision() {
		//�Փ˔���`�F�b�N
		//�z�u�I�u�W�F�N�g�̏Փ˃`�F�b�N
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->CollisionChk();
			}
		}
	}

	void Stage::UpdateCollisionGravity() {
		//�z�u�I�u�W�F�N�g�̏Փˏd�͍X�V�`�F�b�N
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->CollisionGravityChk();
			}
		}

	}

	void Stage::UpdateMessageCollision() {
		//�z�u�I�u�W�F�N�g�̏Փ˃��b�Z�[�W���s
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->ToMessageCollision();
			}
		}

	}



	void Stage::DrawShadowmapStage() {
		for (auto ptr : pImpl->m_GameObjectVec) {
			ptr->DrawShadowmap();
		}
	}


	//�X�e�[�W���̕`��i�V�[�������΂��j
	void Stage::DrawStage() {
		for (auto ptr : pImpl->m_GameObjectVec) {
			ptr->OnDraw();
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct SceneBase::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SceneBase::Impl {
		shared_ptr<Stage> m_ActiveStage;	//�A�N�e�B�u�ȃX�e�[�W
		Impl():
			m_ActiveStage()
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	�V�[���e�N���X
	//--------------------------------------------------------------------------------------
	SceneBase::SceneBase() :
		SceneInterface(),
		pImpl(new Impl())
	{
		try {
			//�f�t�H���g�̃��\�[�X�̍쐬
			App::GetApp()->RegisterResource(L"DEFAULT_SQUARE", MeshResource::CreateSquare(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_CUBE", MeshResource::CreateCube(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_SPHERE", MeshResource::CreateSphere(1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_CAPSULE", MeshResource::CreateCapsule(1.0f, 1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_CYLINDER", MeshResource::CreateCylinder(1.0f, 1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_CONE", MeshResource::CreateCone(1.0f, 1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_TORUS", MeshResource::CreateTorus(1.0f, 0.3f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_TETRAHEDRON", MeshResource::CreateTetrahedron(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_OCTAHEDRON", MeshResource::CreateOctahedron(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_DODECAHEDRON", MeshResource::CreateDodecahedron(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_ICOSAHEDRON", MeshResource::CreateIcosahedron(1.0f));
		}
		catch (...) {
			throw;
		}
	}
	SceneBase::~SceneBase() {}

	shared_ptr<Stage> SceneBase::GetActiveStage() const {
		if (!pImpl->m_ActiveStage) {
			//�A�N�e�B�u�ȃX�e�[�W�������Ȃ�
			throw BaseException(
				L"�A�N�e�B�u�ȃX�e�[�W������܂���",
				L"if(!m_ActiveStage.get())",
				L"SceneBase::GetActiveStage()"
			);
		}
		return pImpl->m_ActiveStage;
	}
	void SceneBase::SetActiveStage(const shared_ptr<Stage>& stage) {
		pImpl->m_ActiveStage = stage;
	}

	void SceneBase::OnUpdate() {
		if (pImpl->m_ActiveStage) {
			//�X�e�[�W�̃A�b�v�f�[�g
			pImpl->m_ActiveStage->UpdateStage();
		}
	}
	void SceneBase::OnDraw() {
		if (pImpl->m_ActiveStage) {
			//�`��f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			Dev->ClearDefultViews(Color4(0, 0, 0, 1.0));
			Dev->ClearShadowmapViews();
			Dev->StartShadowmapDraw();
			pImpl->m_ActiveStage->DrawShadowmapStage();
			Dev->EndShadowmapDraw();
			//�f�t�H���g�`��̊J�n
			Dev->StartDefultDraw();
			pImpl->m_ActiveStage->DrawStage();
			//�f�t�H���g�`��̏I��
			Dev->EndDefultDraw();


		}
	}









}
//end basecross
