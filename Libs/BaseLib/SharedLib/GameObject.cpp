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
		//TransformMatrix��Update
		if (TMptr->IsUpdateActive()) {
			TMptr->OnUpdate();
		}
	}

	void GameObject::DrawShadowmap() {
	}

	void GameObject::ComponentDraw() {
		//Transform���Ȃ���Η�O
		auto Tptr = GetComponent<Transform>();
		//�}�b�v�����؂���Draw
		list<type_index>::iterator it = pImpl->m_CompOrder.begin();
		while (it != pImpl->m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = pImpl->m_CompMap.find(*it);
			if (it2 != pImpl->m_CompMap.end()) {
				//���̃R���|�[�l���g�̕`��
				if (it2->second->IsDrawActive()) {
					it2->second->OnDraw();
				}
			}
			it++;
		}
		//Transform��Draw
		//Transform�̔h���N���X�΍�
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
	//	struct Stage::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct Stage::Impl {
		//�I�u�W�F�N�g�̔z��
		vector< shared_ptr<GameObject> > m_GameObjectVec;
		//�r���ɃI�u�W�F�N�g���ǉ����ꂽ�ꍇ�A�^�[���̊J�n�܂ő҂z��
		vector< shared_ptr<GameObject> > m_WaitAddObjectVec;
		//����Draw����Ă���r���[�̃C���f�b�N�X
		size_t m_DrawViewIndex;
		Impl() :m_DrawViewIndex(0) {}
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

	vector< shared_ptr<GameObject> >& Stage::GetGameObjectVec() { return pImpl->m_GameObjectVec; }


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
		//�z�u�I�u�W�F�N�g�̃R���|�[�l���g�X�V1
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->ComponentUpdate();
			}
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
			//�f�t�H���g�`��̊J�n
			Dev->StartDefultDraw();
			pImpl->m_ActiveStage->DrawStage();
			//�f�t�H���g�`��̏I��
			Dev->EndDefultDraw();
		}
	}









}
//end basecross
