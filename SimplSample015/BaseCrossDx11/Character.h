/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Scene;
	class GameObject;

	//--------------------------------------------------------------------------------------
	///	����
	//--------------------------------------------------------------------------------------
	class SquareObject : public GameObject {
		//���b�V��
		shared_ptr<MeshResource> m_SquareMesh;
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		shared_ptr<TextureResource> m_TextureResource;	///<�e�N�X�`�����\�[�X
		Vector3 m_Scale;				///<�X�P�[�����O
		Quaternion m_Qt;			///<��]
		Vector3 m_Pos;				///<�ʒu
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	PtrScene	�V�[���̃|�C���^
		@param[in]	TextureFileName	�e�N�X�`���t�@�C����
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		*/
		//--------------------------------------------------------------------------------------
		SquareObject(const shared_ptr<Scene> PtrScene,
			const wstring& TextureFileName, const Vector3& Scale, const Quaternion& Qt, const Vector3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareObject();
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
	};

	//--------------------------------------------------------------------------------------
	///	������
	//--------------------------------------------------------------------------------------
	class CubeObject : public GameObject {
		//���b�V��
		shared_ptr<MeshResource> m_CubeMesh;
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		shared_ptr<TextureResource> m_TextureResource;	///<�e�N�X�`�����\�[�X
		Vector3 m_Scale;				///<�X�P�[�����O
		Quaternion m_Qt;			///<��]
		Vector3 m_Pos;				///<�ʒu
		bool m_Trace;					///<�����������邩�ǂ���
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	PtrScene	�V�[���̃|�C���^
		@param[in]	TextureFileName	�e�N�X�`���t�@�C����
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		*/
		//--------------------------------------------------------------------------------------
		CubeObject(const shared_ptr<Scene> PtrScene,
			const wstring& TextureFileName, const Vector3& Scale, const Quaternion& Qt, const Vector3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CubeObject();
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
		@brief OBB�𓾂�
		@return	�����̂�OBB
		*/
		//--------------------------------------------------------------------------------------
		OBB GetObb()const;

	};


	enum class SphereMotion {
		RightMotion,
		LeftMotion
	};


	//--------------------------------------------------------------------------------------
	///	����
	//--------------------------------------------------------------------------------------
	class SphereObject : public GameObject {
		shared_ptr< StateMachine<SphereObject> >  m_StateMachine;	//�X�e�[�g�}�V�[��
		//���b�V��
		shared_ptr<MeshResource> m_SphereMesh;
		UINT m_Division;				///<������
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		shared_ptr<TextureResource> m_TextureResource;	///<�e�N�X�`�����\�[�X
		Vector3 m_Scale;				///<�X�P�[�����O
		Quaternion m_Qt;			///<��]
		Vector3 m_Pos;				///<�ʒu
		bool m_Trace;					///<�����������邩�ǂ���
		float m_MoveXDir;	//X�ړ�����
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	PtrScene	�V�[���̃|�C���^
		@param[in]	Division	������
		@param[in]	TextureFileName	�e�N�X�`���t�@�C����
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	Pos	�ʒu
		*/
		//--------------------------------------------------------------------------------------
		SphereObject(const shared_ptr<Scene> PtrScene,
			UINT Division, const wstring& TextureFileName, bool Trace, const Vector3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SphereObject();

		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�e�[�g�}�V���̎擾
		@return	�X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr< StateMachine<SphereObject> > GetStateMachine() const {
			return m_StateMachine;
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
		@brief �A�N�V���������s����
		@param[in]	motion	�A�N�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetActionMotion(SphereMotion motion);

		//--------------------------------------------------------------------------------------
		/*!
		@brief ���݂̃A�N�V�������I���������ǂ���
		@return	�I�����Ă����true
		*/
		//--------------------------------------------------------------------------------------
		bool IstActionArrived();


	};


	//--------------------------------------------------------------------------------------
	//	class RightState : public ObjState<SphereObject>;
	//	�p�r: �E�����ړ�
	//--------------------------------------------------------------------------------------
	class RightState : public ObjState<SphereObject>
	{
		RightState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<RightState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<SphereObject>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<SphereObject>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<SphereObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class LeftState : public ObjState<SphereObject>;
	//	�p�r: �������ړ�
	//--------------------------------------------------------------------------------------
	class LeftState : public ObjState<SphereObject>
	{
		LeftState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<LeftState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<SphereObject>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<SphereObject>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<SphereObject>& Obj)override;
	};


}
//end basecross
