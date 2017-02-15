/*!
@file Behavior.h
@brief �s���N���X
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class GameObject;
	//--------------------------------------------------------------------------------------
	///	�s���N���X�̐e�N���X
	//--------------------------------------------------------------------------------------
	class Behavior : public ObjectInterface {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�R���X�g���N�^
		@param[in]	GameObjectPtr	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit Behavior(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Behavior();
	public:
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Q�[���I�u�W�F�N�g�̎擾
		@return	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetGameObject() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��������X�e�[�W�̎擾
		@return	�Q�[���I�u�W�F�N�g����������X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetStage() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������(��֐��A�K�v�Ȃ瑽�d��`����)
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() {}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	�s�����[�e�B���e�B�N���X
	//--------------------------------------------------------------------------------------
	class UtilBehavior : public Behavior {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		UtilBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~UtilBehavior() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �i�s�����������悤�ɂ���
		@param[in]	LerpFact	��ԌW���i0.0����1.0�̊ԁj
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void RotToHead(float LerpFact);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �i�s�����������悤�ɂ���(���x�w�����)
		@param[in]	LerpFact	��ԌW���i0.0����1.0�̊ԁj
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void RotToHead(const Vector3& Velocity,float LerpFact);

	};

	//--------------------------------------------------------------------------------------
	///	Jump�s���N���X
	//--------------------------------------------------------------------------------------
	class JumpBehavior : public Behavior {
	public:
		JumpBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr)
		{}
		virtual ~JumpBehavior() {}
		void StartJump(const Vector3& FirstVelocity);
		bool Execute();
	};


	//--------------------------------------------------------------------------------------
	///	Seek�s���N���X
	//--------------------------------------------------------------------------------------
	class SeekBehavior : public Behavior {
	public:
		SeekBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr)
		{}
		virtual ~SeekBehavior() {}
		void Enter(const Vector3& TargetPos);
		void Enter(const wstring& TargetKey);
		float Execute(const Vector3& TargetPos,bool RotHead, float LerpFact);
		float Execute(const wstring& TargetKey, bool RotHead, float LerpFact);
		void Exit();
	};

	//--------------------------------------------------------------------------------------
	///	Arrive�s���N���X
	//--------------------------------------------------------------------------------------
	class ArriveBehavior : public Behavior {
	public:
		ArriveBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr)
		{}
		virtual ~ArriveBehavior() {}
		void Enter(const Vector3& TargetPos);
		void Enter(const wstring& TargetKey);
		float Execute(const Vector3& TargetPos, bool RotHead, float LerpFact);
		float Execute(const wstring& TargetKey, bool RotHead, float LerpFact);
		void Exit();
	};


}
//end basecross
