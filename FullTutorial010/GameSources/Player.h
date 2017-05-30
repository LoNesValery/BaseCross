/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	//--------------------------------------------------------------------------------------
	///	�v���C���[
	//--------------------------------------------------------------------------------------
	class Player : public GameObject {
		//������̕\��
		void DrawStrings();
		//���̓n���h���[
		InputHandler<Player> m_InputHandler;
		//�X�e�[�g�}�V�[��
		unique_ptr<StateMachine<Player>>  m_StateMachine;
		//�킢�̍s��
		unique_ptr<FightBehavior<Player>> m_FightBehavior;
		//�}�b�N�XHP
		float m_MaxHP;
		//���݂�HP
		float m_HP;
		//�X�^�[�g�ʒu
		Vector3 m_StartPosition;
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�}�V���𓾂�
		@return	�X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<Player> >& GetStateMachine(){
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�킢�̍s���𓾂�
		@return	�킢�̍s��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< FightBehavior<Player> >& GetFightBehavior() {
			return m_FightBehavior;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�^�[�g�ʒu�𓾂�
		@return	�X�^�[�g�ʒu
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetStartPosition()const {
			return m_StartPosition;
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief	HP�𓾂�
		@return	HP
		*/
		//--------------------------------------------------------------------------------------
		float GetHP()const {
			return m_HP;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	HP��ݒ肷��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetHP(float f) {
			m_HP = f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MaxHP�𓾂�
		@return	MaxHP
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxHP()const {
			return m_MaxHP;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MaxHP��ݒ肷��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMaxHP(float f) {
			m_MaxHP = f;
		}
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//��X�V
		virtual void OnUpdate2() override;
		//�Փˎ�
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
		//A�{�^���n���h��
		void OnPushA();
		//X�{�^���n���h��
		void OnPushX();
	};


	//--------------------------------------------------------------------------------------
	///	�ʏ�X�e�[�g
	//--------------------------------------------------------------------------------------
	class PlayerDefaultState : public ObjState<Player>
	{
		PlayerDefaultState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PlayerDefaultState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�W�����v�X�e�[�g
	//--------------------------------------------------------------------------------------
	class PlayerJumpState : public ObjState<Player>
	{
		PlayerJumpState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PlayerJumpState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Execute2(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	����U��X�e�[�g
	//--------------------------------------------------------------------------------------
	class PlayerSwordState : public ObjState<Player>
	{
		PlayerSwordState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PlayerSwordState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	����˂��X�e�[�g
	//--------------------------------------------------------------------------------------
	class PlayerSwordPokeState : public ObjState<Player>
	{
		PlayerSwordPokeState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PlayerSwordPokeState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	�G�̌��ɓ��������X�e�[�g
	//--------------------------------------------------------------------------------------
	class PlayerSwordHit : public ObjState<Player>
	{
		PlayerSwordHit() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PlayerSwordHit)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�|���ꂽ�X�e�[�g
	//--------------------------------------------------------------------------------------
	class PlayerDieState : public ObjState<Player>
	{
		PlayerDieState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(PlayerDieState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};



}
//end basecross

