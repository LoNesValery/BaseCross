/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class AttackBall : public GameObject;
	//	�p�r: ���ł����{�[��
	//--------------------------------------------------------------------------------------
	class AttackBall : public GameObject {
	public:
		//�\�z�Ɣj��
		AttackBall(const shared_ptr<Stage>& StagePtr);
		virtual ~AttackBall();
		void Weakup(const Vector3& Position, const Vector3& Velocity);
		//������
		virtual void OnCreate() override;
		//����
		virtual void OnUpdate() override;
	};


	class Player;
	//--------------------------------------------------------------------------------------
	/// �A�N�V�����R�}���h�iA�{�^���j
	//--------------------------------------------------------------------------------------
	class ActionCommand : public ObjCommand<Player> {
		virtual void Excute(const shared_ptr<Player>& Obj)override;
	};
	//--------------------------------------------------------------------------------------
	///	�s���؂�ւ��R�}���h(B�{�^��)
	//--------------------------------------------------------------------------------------
	class BehaviorChangeCommand : public ObjCommand<Player> {
		virtual void Excute(const shared_ptr<Player>& Obj)override;
	};



	class PlayerBehavior;
	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	�p�r: �v���C���[
	//--------------------------------------------------------------------------------------
	class Player : public GameObject {
		//�s���֐��N���X
		shared_ptr<PlayerBehavior> m_PlayerBehavior;
		//�K�w���X�e�[�g�}�V�[��
		shared_ptr< LayeredStateMachine<Player> >  m_StateMachine;
		//�R�}���h�֘A
		void InputCheck() {
			//�R���g���[���{�^���`�F�b�N
			auto pCom = m_Handler.HandleInput();
			if (pCom) {
				pCom->Excute(GetThis<Player>());
			}
		}
		//�A�N�V�����R�}���h�iA�{�^���j
		ActionCommand m_Action;
		//�s���؂�ւ��R�}���h�iB�{�^���j
		BehaviorChangeCommand m_BehaviorChange;

		//���̓n���h���[
		InputHandler<Player> m_Handler;
		//�ō����x
		float m_MaxSpeed;
		//������
		float m_Decel;
		//����
		float m_Mass;
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		Player(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Player() {}
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�K�w���X�e�[�g�}�V���𓾂�
		@return	�K�w���X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr< LayeredStateMachine<Player> > GetStateMachine() const {
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s���֐��N���X�𓾂�
		@return	�s���֐��N���X
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<PlayerBehavior> GetPlayerBehavior() const {
			return m_PlayerBehavior;
		}
		float GetMaxSpeed() const {
			return m_MaxSpeed;
		}
		float GetDecel() const {
			return m_Decel;
		}
		float GetMass() const {
			return m_Mass;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s���֐��N���X��؂�ւ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ChangePlayerBehavior();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//�Փˎ�
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
		virtual void OnCollisionExcute(vector<shared_ptr<GameObject>>& OtherVec) override;
		virtual void OnCollisionExit(vector<shared_ptr<GameObject>>& OtherVec) override;
		//�^�[���̍ŏI�X�V��
		virtual void OnLastUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	�v���C���[�s���e
	//--------------------------------------------------------------------------------------
	class PlayerBehavior {
	protected:
		PlayerBehavior() {}
	public:
		virtual ~PlayerBehavior() {}
		virtual shared_ptr<PlayerBehavior> ChangeNextBehavior(const shared_ptr<Player>& Obj) = 0;
		virtual Vector3 GetAngle(const shared_ptr<Player>& Obj);
		virtual void Move(const shared_ptr<Player>& Obj);
		virtual void StartAction(const shared_ptr<Player>& Obj) = 0;
		virtual void ExcuteAction(const shared_ptr<Player>& Obj) = 0;
	};

	//--------------------------------------------------------------------------------------
	///	�f�t�H���g�v���C���[�s��
	//--------------------------------------------------------------------------------------
	class DefaultPlayerBehavior : public PlayerBehavior {
	protected:
		DefaultPlayerBehavior() {}
	public:
		//�s���̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(DefaultPlayerBehavior)
		virtual shared_ptr<PlayerBehavior> ChangeNextBehavior(const shared_ptr<Player>& Obj) override;
		virtual void StartAction(const shared_ptr<Player>& Obj) override;
		virtual void ExcuteAction(const shared_ptr<Player>& Obj) override;
	};

	//--------------------------------------------------------------------------------------
	///	�U���v���C���[�s��
	//--------------------------------------------------------------------------------------
	class AttackPlayerBehavior : public PlayerBehavior {
	protected:
		AttackPlayerBehavior() {}
	public:
		//�s���̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(AttackPlayerBehavior)
		virtual shared_ptr<PlayerBehavior> ChangeNextBehavior(const shared_ptr<Player>& Obj) override;
		virtual void StartAction(const shared_ptr<Player>& Obj) override;
		virtual void ExcuteAction(const shared_ptr<Player>& Obj) override;
	};


	//--------------------------------------------------------------------------------------
	///	�ʏ�X�e�[�g
	//--------------------------------------------------------------------------------------
	class PlayerDefault : public ObjState<Player>
	{
		PlayerDefault() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PlayerDefault)
		virtual void Enter(const shared_ptr<Player>& Obj)override {}
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override {}
	};

	//--------------------------------------------------------------------------------------
	///	�A�N�V�����X�e�[�g
	//--------------------------------------------------------------------------------------
	class PlayerAction : public ObjState<Player>
	{
		PlayerAction() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PlayerAction)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override {}
	};




}
//end basecross

