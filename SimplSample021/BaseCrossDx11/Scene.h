/*!
@file Scene.h
@brief �V�[���Ȃ�
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	class Scene;

	//--------------------------------------------------------------------------------------
	///	�J����
	//--------------------------------------------------------------------------------------
	struct Camera {
		Vec3 m_CamerEye;			///<�J�����ʒu
		Vec3 m_CamerAt;			///<�J�������_
		Vec3 m_CamerUp;			///<�J�����X��
		float m_FovY;				///<��p
		float m_CameraXZRad;			//�J������XZ���W�A��
		float m_CameraYRad;				//�J������Y���W�A��
		float m_CameraArmLen;		//�J�����̘r�̒���
		Camera():
			m_CamerEye(0, 5.0, -5.0f),
			m_CamerAt(0, 0, 0),
			m_CamerUp(0, 1.0f, 0),
			m_FovY(XM_PIDIV4),
			m_CameraXZRad(0.0f),
			m_CameraYRad(XM_PIDIV4 + 0.5f),
			m_CameraArmLen(5.0f)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �r���[�s��Ǝˉe�s��̎擾�i�e�Q�ƃp�����[�^�ɑ���j
		@param[out]	View	�r���[�s����󂯎��Q��
		@param[out]	Proj	�ˉe�s����󂯎��Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetViewProjMatrix(Mat4x4& View, Mat4x4& Proj)const {
			View = XMMatrixLookAtLH(m_CamerEye, m_CamerAt, m_CamerUp);
			float w = static_cast<float>(App::GetApp()->GetGameWidth());
			float h = static_cast<float>(App::GetApp()->GetGameHeight());
			Proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �J������Eye��At���擾
		@param[out]	Eye	Eye���󂯎��Q��
		@param[out]	At	At���󂯎��Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetCameraEyeAt(Vec3& Eye, Vec3& At)const {
			Eye = m_CamerEye;
			At = m_CamerAt;
		}
	};


	//--------------------------------------------------------------------------------------
	///	�X�e�[�W�i�V�[���ŊǗ�����C���^�[�t�F�C�X�j
	//--------------------------------------------------------------------------------------
	class Stage : public ObjectInterface, public ShapeInterface {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �v���e�N�g�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Stage(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Stage() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�[�W�X�V�i�������z�֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdateStage() = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�[�W�`��i�������z�֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawStage() = 0;
	};

	//--------------------------------------------------------------------------------------
	///	�Q�[���I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	class GameObject : public ObjectInterface, public ShapeInterface {
		weak_ptr<Stage> m_Stage;	//�����X�e�[�W
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �v���e�N�g�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		explicit GameObject(const shared_ptr<Stage>& StagePtr) :
			m_Stage(StagePtr) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameObject() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��������X�e�[�W�𓾂�
		@return	��������X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetStage() const {
			auto shptr = m_Stage.lock();
			if (!shptr) {
				throw BaseException(
					L"�����X�e�[�W��null�ł��B",
					L"if (!shptr)",
					L"GameObject::GetStage()"
				);
			}
			return shptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��������X�e�[�W�𓾂�(�^�`�F�b�N����)
		@return	��������X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetStage() const {
			auto StagePtr = GetStage();
			auto TargetPtr = dynamic_pointer_cast<T>(StagePtr);
			if (!TargetPtr) {
				throw BaseException(
					L"�X�e�[�W������܂������A�^�L���X�g�ł��܂���",
					Util::GetWSTypeName<T>(),
					L"GameObject::GetStage<T>()"
				);
			}
			return TargetPtr;
		}
	};


	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	class Scene : public SceneInterface {
		//�A�N�e�B�u�ȃX�e�[�W
		shared_ptr<Stage> m_ActiveStage;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�A�N�e�B�u�ȃX�e�[�W��ݒ肵�ď���������
		@tparam	T	�擾����^�iStage�Ɍ^�ϊ��ł�����́j
		@tparam	Ts	�ϒ��ϐ��̌^
		@param[in]	params	���̃X�e�[�W���\�z����̂Ɏg�p����p�����[�^�B
		@return	�X�e�[�W
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
					L"Scene::ResetActiveStage<T>()"
				);
			}
			m_ActiveStage = StagePtr;
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���\�[�X�̍쐬
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void CreateResources();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �A�N�e�B�u�ȃX�e�[�W�̎擾
		@return	�A�N�e�B�u�ȃX�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetActiveStage() const {
			if (!m_ActiveStage) {
				throw BaseException(
					L"�A�N�e�B�u�ȃX�e�[�W�͂���܂���",
					L"f (!m_ActiveStage)",
					L"Scene::GetActiveStage()"
				);
			}
			return m_ActiveStage;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C�x���g�擾
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;
	};


}
// end basecross