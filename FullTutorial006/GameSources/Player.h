/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{



	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	�p�r: �v���C���[
	//--------------------------------------------------------------------------------------
	class Player : public GameObject {
		shared_ptr< StateMachine<Player> >  m_StateMachine;	//�X�e�[�g�}�V�[��
															//�ړ��̌����𓾂�
		Vector3 GetAngle();
		//�ō����x
		float m_MaxSpeed;
		//������
		float m_Decel;
		//����
		float m_Mass;
	public:
		//�\�z�Ɣj��
		Player(const shared_ptr<Stage>& StagePtr);
		virtual ~Player() {}
		//������
		virtual void OnCreate() override;
		//�A�N�Z�T
		shared_ptr< StateMachine<Player> > GetStateMachine() const {
			return m_StateMachine;
		}
		//���[�V��������������֐��Q
		//�ړ����Č������ړ������ɂ���
		void MoveRotationMotion();
		//A�{�^���ŃW�����v����ǂ����𓾂�
		bool IsJumpMotion();
		//A�{�^���ŃW�����v����u�Ԃ̏���
		void JumpMotion();
		//A�{�^���ŃW�����v���Ă���Ԃ̏���
		//�W�����v�I��������true��Ԃ�
		bool JumpMoveMotion();
		//�X�V
		virtual void OnUpdate() override;
		//�Փˎ�
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
		//�^�[���̍ŏI�X�V��
		virtual void OnLastUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class DefaultState : public ObjState<Player>;
	//	�p�r: �ʏ�ړ�
	//--------------------------------------------------------------------------------------
	class DefaultState : public ObjState<Player>
	{
		DefaultState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<DefaultState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class JumpState : public ObjState<Player>;
	//	�p�r: �W�����v���
	//--------------------------------------------------------------------------------------
	class JumpState : public ObjState<Player>
	{
		JumpState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		static shared_ptr<JumpState> Instance();
		//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};



}
//end basecross
