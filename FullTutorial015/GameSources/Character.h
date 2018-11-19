/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�A�j���X�v���C�g
	//--------------------------------------------------------------------------------------
	class AnimeSprite : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec2 m_StartPos;
		wstring m_TextureKey;
		//�g�[�^������
		float m_TotalTime;
		//�I������Ă��邩�ǂ���
		bool m_Selected;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	TextureKey	�e�N�X�`���L�[
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	StartScale	�����X�P�[��
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		AnimeSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec2& StartPos);
		//�j��
		virtual ~AnimeSprite();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate()override;
		//�A�N�Z�T
		bool IsSelect() const{
			return m_Selected;
		}
		void SetSelect(bool b) {
			m_Selected = b;
			m_TotalTime = 0.0f;
		}
	};


	//--------------------------------------------------------------------------------------
	//	�p�r: �^�C�����O����v���[�g
	//--------------------------------------------------------------------------------------
	class TilingPlate : public GameObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//�\�z�Ɣj��
		TilingPlate(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingPlate();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//�\�z�Ɣj��
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingFixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};



	//--------------------------------------------------------------------------------------
	///	�G�P
	//--------------------------------------------------------------------------------------
	class Enemy1 : public GameObject {
		Vec3 m_StartPos;
		//�X�e�[�g�}�V�[��
		unique_ptr<StateMachine<Enemy1>>  m_StateMachine;
		//Near��Far��؂�ւ���l
		const float m_NearFarChange;
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		Enemy1(const shared_ptr<Stage>& StagePtr,const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Enemy1() {}
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�}�V���𓾂�
		@return	�X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<Enemy1>>& GetStateMachine() {
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Near��Far�̋��l�𓾂�
		@return	Near��Far�̋��l
		*/
		//--------------------------------------------------------------------------------------
		float GetNearFarChange()const {
			return m_NearFarChange;
		}
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//��X�V
		virtual void OnUpdate2() override;
	};


	//--------------------------------------------------------------------------------------
	///	Enemy1��Far�X�e�[�g
	//--------------------------------------------------------------------------------------
	class Enemy1FarState : public ObjState<Enemy1>
	{
		Enemy1FarState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(Enemy1FarState)
		virtual void Enter(const shared_ptr<Enemy1>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy1>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy1>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	Enemy1��Near�X�e�[�g
	//--------------------------------------------------------------------------------------
	class Enemy1NearState : public ObjState<Enemy1>
	{
		Enemy1NearState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(Enemy1NearState)
		virtual void Enter(const shared_ptr<Enemy1>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy1>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy1>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	�G2
	//--------------------------------------------------------------------------------------
	class Enemy2 : public GameObject {
		Vec3 m_StartPos;
		//�X�e�[�g�}�V�[��
		unique_ptr<StateMachine<Enemy2>>  m_StateMachine;
		//long��medium��؂�ւ���l
		const float m_LongMediumChange;
		//medium��short��؂�ւ���l
		const float m_MediumShortChange;

	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		Enemy2(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Enemy2() {}
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�}�V���𓾂�
		@return	�X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<Enemy2>>& GetStateMachine() {
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Long��Medium�̋��l�𓾂�
		@return	Long��Medium�̋��l
		*/
		//--------------------------------------------------------------------------------------
		float GetLongMediumChange() const {
			return m_LongMediumChange;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Medium��Short�̋��l�𓾂�
		@return	Medium��Short�̋��l
		*/
		//--------------------------------------------------------------------------------------
		float GetMediumShortChange() const {
			return m_MediumShortChange;

		}

		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//��X�V
		virtual void OnUpdate2() override;
		//�Փˎ�
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
	};


	//--------------------------------------------------------------------------------------
	///	Enemy2��long�X�e�[�g
	//--------------------------------------------------------------------------------------
	class Enemy2longState : public ObjState<Enemy2>
	{
		Enemy2longState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(Enemy2longState)
		virtual void Enter(const shared_ptr<Enemy2>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy2>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy2>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	Enemy2��Medium�X�e�[�g
	//--------------------------------------------------------------------------------------
	class Enemy2MediumState : public ObjState<Enemy2>
	{
		Enemy2MediumState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(Enemy2MediumState)
		virtual void Enter(const shared_ptr<Enemy2>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy2>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy2>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	Enemy2��Short�X�e�[�g
	//--------------------------------------------------------------------------------------
	class Enemy2ShortState : public ObjState<Enemy2>
	{
		Enemy2ShortState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(Enemy2ShortState)
		virtual void Enter(const shared_ptr<Enemy2>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy2>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy2>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�G�R
	//--------------------------------------------------------------------------------------
	class Enemy3 : public GameObject {
		Vec3 m_StartPos;
		//�X�e�[�g�}�V�[��
		unique_ptr<StateMachine<Enemy3>>  m_StateMachine;
		//default��near��؂�ւ���l
		const float m_DefaultNearChange;

	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		Enemy3(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Enemy3() {}
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�}�V���𓾂�
		@return	�X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<Enemy3>>& GetStateMachine() {
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Default��Near�̋��l�𓾂�
		@return	Default��Near�̋��l
		*/
		//--------------------------------------------------------------------------------------
		float GetDefaultNearChange() const {
			return m_DefaultNearChange;

		}

		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//��X�V
		virtual void OnUpdate2() override;
	};

	//--------------------------------------------------------------------------------------
	///	Enemy3��Default�X�e�[�g
	//--------------------------------------------------------------------------------------
	class Enemy3DefaultState : public ObjState<Enemy3>
	{
		Enemy3DefaultState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(Enemy3DefaultState)
		virtual void Enter(const shared_ptr<Enemy3>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy3>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy3>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	Enemy3��Near�X�e�[�g
	//--------------------------------------------------------------------------------------
	class Enemy3NearState : public ObjState<Enemy3>
	{
		Enemy3NearState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(Enemy3NearState)
		virtual void Enter(const shared_ptr<Enemy3>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy3>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy3>& Obj)override;
	};




}
//end basecross