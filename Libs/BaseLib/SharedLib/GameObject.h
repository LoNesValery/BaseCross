/*!
@file GameObject.h
@brief �Q�[���I�u�W�F�N�g�A�X�e�[�W
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�Q�[���z�u�I�u�W�F�N�g�e�N���X
	//--------------------------------------------------------------------------------------
	class GameObject :public ObjectInterface, public ShapeInterface {
		shared_ptr<Component> SearchComponent(type_index TypeIndex)const;
		shared_ptr<Transform> GetTransform()const;
		void SetTransform(const shared_ptr<Transform>& Ptr);
		void AddMakedComponent(type_index TypeIndex, const shared_ptr<Component>& Ptr);
		template<typename T>
		shared_ptr<T> SearchDynamicComponent()const {
			auto mapPtr = GetCompoMap();
			auto it = mapPtr.begin();
			while (it != mapPtr.end()) {
				auto Ptr = dynamic_pointer_cast<T>(it->second);
				if (Ptr) {
					return Ptr;
				}
				it++;
			}
			return nullptr;
		}
		map<type_index, shared_ptr<Component> >& GetCompoMap() const;
		void RemoveTgtComponent(type_index TypeIndex);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		explicit GameObject(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V�������L�����ǂ����𓾂�
		@return	�X�V�������L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsUpdateActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V�������L�����ǂ����𓾂�
		@return	�X�V�������L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetUpdateActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V�������L�����ǂ�����ݒ肷��
		@param[in]	b	�X�V�������L���Ȃ�Ȃ�true
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetUpdateActive(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�揈�����L�����ǂ����𓾂�
		@return	�`�揈�����L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsDrawActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�揈�����L�����ǂ����𓾂�
		@return	�`�揈�����L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetDrawActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�揈�����L�����ǂ�����ݒ肷��
		@param[in]	b	�`�揈�����L���Ȃ�Ȃ�true
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetDrawActive(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���������i�������܂ށj���L�����ǂ����𓾂�
		@return	���������i�������܂ށj���L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsAlphaActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���������i�������܂ށj���L�����ǂ����𓾂�
		@return	���������i�������܂ށj���L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetAlphaActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���������i�������܂ށj���L�����ǂ�����ݒ肷��
		@param[in]	b	���������i�������܂ށj���L���Ȃ�Ȃ�true
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetAlphaActive(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�v���C�g�Ƃ���Draw���邩�ǂ����𓾂�
		@return	�X�v���C�g�Ƃ���Draw����Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsSpriteDraw() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�v���C�g�Ƃ���Draw���邩�ǂ����𓾂�
		@return	�X�v���C�g�Ƃ���Draw����Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetSpriteDraw() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�v���C�g�Ƃ���Draw���邩�ǂ�����ݒ肷��
		@param[in]	b	�X�v���C�g�Ƃ���Draw����Ȃ�true
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetSpriteDraw(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�惌�C���[�𓾂�i�f�t�H���g0�j
		@return	�`�惌�C���[
		*/
		//--------------------------------------------------------------------------------------
		int GetDrawLayer() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�惌�C���[��ݒ肷��
		@param[in]	l	�`�惌�C���[�i�}�C�i�X�ݒ�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void  SetDrawLayer(int l);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��������X�e�[�W�𓾂�
		@param[in]	ExceptionActive	�Ώۂ�null�������ꍇ�ɗ�O�������邩�ǂ���
		@return	��������X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetStage(bool ExceptionActive = true) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��������X�e�[�W��ݒ肷��
		@param[in]	stage	��������X�e�[�W
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetStage(const shared_ptr<Stage>& stage);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���|�[�l���g�̎擾
		@tparam	T	�擾����^
		@param[in]	ExceptionActive	�Ώۂ�null�������ꍇ�ɗ�O�������邩�ǂ���
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		shared_ptr<T> GetComponent(bool ExceptionActive = true)const {
			auto Ptr = SearchComponent(type_index(typeid(T)));
			if (Ptr) {
				//�w��̌^�̃R���|�[�l���g����������
				auto RetPtr = dynamic_pointer_cast<T>(Ptr);
				if (RetPtr) {
					return RetPtr;
				}
				else {
					throw BaseException(
						L"�R���|�[�l���g������܂������A�^�L���X�g�ł��܂���",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetComponent<T>()"
					);
				}
			}
			else {
				if (ExceptionActive) {
					throw BaseException(
						L"�R���|�[�l���g��������܂���",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetComponent<T>()"
					);
				}
			}
			return nullptr;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Transform�R���|�[�l���g�̎擾
		@param[in]	ExceptionActive	�Ώۂ�null�������ꍇ�ɗ�O�������邩�ǂ���
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<Transform> GetComponent<Transform>(bool ExceptionActive)const {
			auto Ptr = GetTransform();
			if (!Ptr) {
				if (ExceptionActive) {
					throw BaseException(
						L"�R���|�[�l���g��������܂���",
						L"Transform",
						L"GameObject::GetComponent<Transform>()"
					);
				}
				else {
					return nullptr;
				}
			}
			return Ptr;
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief	Transform�R���|�[�l���g�̔h���N���X�̎擾
		@tparam	T	�擾����^�iTransform�Ɍ^�ϊ��ł�����́j
		@param[in]	ExceptionActive	�Ώۂ�null�������ꍇ�ɗ�O�������邩�ǂ���
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		shared_ptr<T> GetDynamicTransform(bool ExceptionActive = true)const {
			auto Ptr = dynamic_pointer_cast<T>(GetTransform());
			if (!Ptr) {
				if (ExceptionActive) {
					throw BaseException(
						L"�w��̌^�ւ�Transform����L���X�g�ł��܂���",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetDynamicTransform<T>()"
					);
				}
				else {
					return nullptr;
				}
			}
			return Ptr;
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief	DynamicCast�𗘗p�����R���|�[�l���g�̎擾�B�R���|�[�l���g�̔h���N���X�g�p���Ɏg��
		@tparam	T	�擾����^�iT�Ɍ^�ϊ��ł�����́j
		@param[in]	ExceptionActive	�Ώۂ�null�������ꍇ�ɗ�O�������邩�ǂ���
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		shared_ptr<T> GetDynamicComponent(bool ExceptionActive = true)const {
			auto Ptr = SearchDynamicComponent<T>();
			if (Ptr) {
				return Ptr;
			}
			else {
				if (ExceptionActive) {
					throw BaseException(
						L"�^�L���X�g�ł���R���|�[�l���g��������܂���",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetDynamicComponent<T>()"
					);
				}
			}
			return nullptr;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���|�[�l���g�̒ǉ��B�V�K�ɃR���|�[�l���g�N���X�����ꍇ�A�R���X�g���N�^�̑�1������GameObject�Ƃ��č쐬����B
		@tparam	T	�ǉ�����^
		@tparam	Ts	�ϒ��ϐ��̌^
		@param[in]	params	���̃R���|�[�l���g���\�z����̂Ɏg�p����p�����[�^�B�i��2�p�����[�^�ȍ~�j
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> AddComponent(Ts&&... params) {
			auto Ptr = SearchComponent(type_index(typeid(T)));
			//�w��̌^�̃R���|�[�l���g����������
			if (Ptr) {
				auto RetPtr = dynamic_pointer_cast<T>(Ptr);
				if (RetPtr) {
					return RetPtr;
				}
				else {
					throw BaseException(
						L"���łɃR���|�[�l���g������܂������A�^�L���X�g�ł��܂���",
						Util::GetWSTypeName<T>(),
						L"GameObject::AddComponent<T>()"
					);
				}
			}
			else {
				//������Ȃ��B�V���ɍ쐬����
				shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>(), params...);
				AddMakedComponent(type_index(typeid(T)), newPtr);
				return newPtr;
			}
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Transform�R���|�[�l���g�̒ǉ�
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<Transform> AddComponent<Transform>() {
			auto Ptr = GetTransform();
			if (Ptr) {
				return Ptr;
			}
			else {
				//������ΐV���ɐ��삷��
				auto TransformPtr = ObjectFactory::Create<Transform>(GetThis<GameObject>());
				SetTransform(TransformPtr);
				return TransformPtr;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Transform�R���|�[�l���g�̔h���N���X�̒ǉ�
		@tparam	T	�擾����^�iTransform�Ɍ^�ϊ��ł�����́j
		@tparam	Ts	�ϒ��ϐ��̌^
		@param[in]	params	���̃R���|�[�l���g���\�z����̂Ɏg�p����p�����[�^�B�i��2�p�����[�^�ȍ~�j
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicTransform(Ts&&... params) {
			//���݂̌����͍s�킸�A���̂܂܃Z�b�g����
			shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>(), params...);
			//Transform�ɃL���X�g���Ă݂�
			auto RetPtr = dynamic_pointer_cast<Transform>(newPtr);
			if (!RetPtr) {
				//�L���X�g�ł��Ȃ�
				throw BaseException(
					L"���̃R���|�[�l���g��Transform�ɃL���X�g�ł��܂���B",
					Util::GetWSTypeName<T>(),
					L"GameObject::AddDynamicTransform<T>()"
				);
			}
			SetTransform(newPtr);
			return newPtr;
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���|�[�l���g�̍폜
		@tparam	T	�폜����^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		void RemoveComponent() {
			RemoveTgtComponent(type_index(typeid(T)));
		}

		//RemoveComponent�͓��ꉻ�͗p�ӂ��Ȃ�
		//�Œ�R���|�[�l���g�𖳌��ɂ���ꍇ��
		//UpdaetActive��false�ɂ��ׂ��ł���

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���|�[�l���g�̍X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ComponentUpdate();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V���h�E�}�b�v�̕`�揈��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void DrawShadowmap();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���|�[�l���g�̕`�揈���i���Draw�R���|�[�l���g�����j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ComponentDraw();

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�O������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V�i�f�t�H���g�͉������Ȃ��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`��O�����i�f�t�H���g�͉������Ȃ��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class Stage :public ObjectInterface, public ShapeInterface {
		//�v���C�x�[�g�T�u�֐�
		void PushBackGameObject(const shared_ptr<GameObject>& Ptr);
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Stage();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Stage();
	public:
		//�A�N�Z�T
		vector< shared_ptr<GameObject> >& GetGameObjectVec();

		//GameObject�̐V�K�ǉ�
		template<typename T, typename... Ts>
		shared_ptr<T> AddGameObject(Ts&&... params) {
			try {
				auto Ptr = ObjectFactory::Create<T>(GetThis<Stage>(), params...);
				PushBackGameObject(Ptr);
				return Ptr;
			}
			catch (...) {
				throw;
			}
		}

		template<typename T, typename... Ts>
		shared_ptr<T> AddGameObjectWithParam(Ts&&... params) {
			try {
				auto Ptr = ObjectFactory::CreateGameObjectWithParam<T>(GetThis<Stage>(), params...);
				PushBackGameObject(Ptr);
				return Ptr;
			}
			catch (...) {
				throw;
			}
		}
		//�X�e�[�W���̍X�V�i�V�[�������΂��j
		virtual void UpdateStage();
		//�X�e�[�W���̕`��i�V�[�������΂��j
		virtual void DrawStage();

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V�i�f�t�H���g�͉������Ȃ��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`��i�f�t�H���g�͉������Ȃ��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	///	�V�[���e�N���X
	//--------------------------------------------------------------------------------------
	class SceneBase :public SceneInterface {
		void SetActiveStage(const shared_ptr<Stage>& stage);
	protected:
		SceneBase();
		virtual ~SceneBase();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃X�e�[�W�𓾂�
		@return	���݂̃X�e�[�W�i���s����O�j
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetActiveStage() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w��̌^�̌��݂̃X�e�[�W�𓾂�
		@tparam	T	�擾����^
		@return	�w��̌^�̃X�e�[�W�i���s����O�j
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetActiveTypeStage() const {
			auto TypeStagePtr = dynamic_pointer_cast<T>(GetActiveStage());
			if (!TypeStagePtr) {
				throw BaseException(
					L"�ȉ��Ɍ^�L���X�g�ł��܂���B",
					Util::GetWSTypeName<T>(),
					L"SceneBase::GetActiveTypeStage<T>()"
				);
			}
			return TypeStagePtr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�A�N�e�B�u�ȃX�e�[�W��ݒ肵�ď���������
		@tparam	T	�擾����^�iStage�Ɍ^�ϊ��ł�����́j
		@tparam	Ts	�ϒ��ϐ��̌^
		@param[in]	params	���̃X�e�[�W���\�z����̂Ɏg�p����p�����[�^�B
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> ResetActiveStage(Ts&&... params) {
			auto Ptr = ObjectFactory::Create<T>(params...);
			auto StagePtr = dynamic_pointer_cast<Stage>(Ptr);
			if (!StagePtr) {
				throw BaseException(
					L"�ȉ���Stage�Ɍ^�L���X�g�ł��܂���B",
					Util::GetWSTypeName<T>(),
					L"SceneBase::ResetActiveStage<T>()"
				);
			}
			SetActiveStage(StagePtr);
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V�[����ω�������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V�[����`�悷��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



}
//end basecross
