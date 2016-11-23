/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	class Player;
	//--------------------------------------------------------------------------------------
	///	�W�����v�R�}���h
	//--------------------------------------------------------------------------------------
	class JumpCommand : public ObjCommand<Player> {
		virtual void Excute(const shared_ptr<Player>& Obj)override;
	};
	//--------------------------------------------------------------------------------------
	///	�s���؂�ւ��R�}���h
	//--------------------------------------------------------------------------------------
	class BehaviorChangeCommand : public ObjCommand<Player> {
		virtual void Excute(const shared_ptr<Player>& Obj)override;
	};


	class PlayerBehavior;
	//--------------------------------------------------------------------------------------
	///	�v���C���[
	//--------------------------------------------------------------------------------------
	class Player : public GameObject {
		//�s���֐��N���X
		shared_ptr<PlayerBehavior> m_PlayerBehavior;
		//�K�w���X�e�[�g�}�V�[��
		shared_ptr< LayeredStateMachine<Player> >  m_StateMachine;	//�X�e�[�g�}�V�[��
		//�R�}���h�֘A
		void InputCheck();
		//�W�����v�R�}���h
		JumpCommand m_Jump;
		//�s���؂�ւ�
		BehaviorChangeCommand m_BehaviorChange;
		InputHandler<Player> m_Handler;
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
		virtual ~Player();
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s���֐��N���X��؂�ւ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ChangePlayerBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	�v���C���[�s���e
	//--------------------------------------------------------------------------------------
	class PlayerBehavior {
	protected:
		PlayerBehavior() {}
	public:
		virtual ~PlayerBehavior() {}
		virtual Vector3 GetAngle(const shared_ptr<Player>& Obj);
		virtual shared_ptr<PlayerBehavior> CreateNextBehavior(const shared_ptr<Player>& Obj) = 0;
		virtual void Move(const shared_ptr<Player>& Obj) = 0;
		virtual void StartJump(const shared_ptr<Player>& Obj) = 0;
		virtual void JumpExcute(const shared_ptr<Player>& Obj) = 0;
	};

	//--------------------------------------------------------------------------------------
	///	�V���v���v���C���[�s��
	//--------------------------------------------------------------------------------------
	class SimplePlayerBehavior : public PlayerBehavior {
	protected:
		SimplePlayerBehavior() {}
	public:
		//�s���̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(SimplePlayerBehavior)
		virtual shared_ptr<PlayerBehavior> CreateNextBehavior(const shared_ptr<Player>& Obj)override;
		virtual void Move(const shared_ptr<Player>& Obj)override;
		virtual void StartJump(const shared_ptr<Player>& Obj)override;
		virtual void JumpExcute(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�n�C�W�����v�v���C���[�s��
	//--------------------------------------------------------------------------------------
	class HighJumpPlayerBehavior : public SimplePlayerBehavior {
	protected:
		HighJumpPlayerBehavior() {}
	public:
		//�s���̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(HighJumpPlayerBehavior)
		virtual shared_ptr<PlayerBehavior> CreateNextBehavior(const shared_ptr<Player>& Obj)override;
		virtual void StartJump(const shared_ptr<Player>& Obj)override;
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
	///	�W�����v�X�e�[�g
	//--------------------------------------------------------------------------------------
	class PlayerJump : public ObjState<Player>
	{
		PlayerJump() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PlayerJump)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override {}
	};





}
//end basecross

