/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	MySeekSteering�s���N���X
	//--------------------------------------------------------------------------------------
	class MySeekSteering : public SteeringBehavior {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		MySeekSteering(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MySeekSteering();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s�������s����
		@param[in]	TargetPos	�ǂ�������ʒu
		@return	�ǂ�������ʒu�Ƃ̋���
		*/
		//--------------------------------------------------------------------------------------
		float Execute(const Vector3& TargetPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s�������s����
		@param[in]	TargetKey	�ǂ�������I�u�W�F�N�g�̃L�[�iSharedObjec�j
		@return	�ǂ�������I�u�W�F�N�g�Ƃ̋���
		*/
		//--------------------------------------------------------------------------------------
		float Execute(const wstring& TargetKey);
	};



	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	�p�r: �ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class SeekObject : public GameObject {
		shared_ptr< StateMachine<SeekObject> >  m_StateMachine;	//�X�e�[�g�}�V�[��
		Vector3 m_StartPos;
		float m_StateChangeSize;
		//�i�s�����������悤�ɂ���
		void RotToHead();
	public:
		//�\�z�Ɣj��
		SeekObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~SeekObject();
		//������
		virtual void OnCreate() override;
		//�A�N�Z�T
		shared_ptr< StateMachine<SeekObject> > GetStateMachine() const {
			return m_StateMachine;
		}

		float GetStateChangeSize() const {
			return m_StateChangeSize;
		}
		//����
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;
	};

	//--------------------------------------------------------------------------------------
	//	class FarState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	class FarState : public ObjState<SeekObject>
	{
		FarState() {}
	public:
		static shared_ptr<FarState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class NearState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	class NearState : public ObjState<SeekObject>
	{
		NearState() {}
	public:
		static shared_ptr<NearState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};


}
//end basecross
