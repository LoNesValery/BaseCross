/*!
@file Enemy.h
@brief �G�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class EnemyInterface {
	public:
		EnemyInterface() {}
		virtual ~EnemyInterface() {}
	};

	//--------------------------------------------------------------------------------------
	//	class HeadObject : public GameObject;
	//	�p�r: �ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class HeadEnemy : public GameObject ,public EnemyInterface {
		Vector3 m_StartPos;
		float m_BaseY;
		float m_StateChangeSize;
		//�s���}�V�[��
		shared_ptr< BehaviorMachine<HeadEnemy> >  m_BehaviorMachine;
	public:
		//���[�e�B���e�B�֐��Q
		float GetStateChangeSize()const {
			return m_StateChangeSize;
		}
		//�v���C���[�̈ʒu��Ԃ�
		Vector3 GetPlayerPosition() const;
		//�v���C���[�܂ł̋�����Ԃ�
		float GetPlayerLength() const;
		//��]��i�s�����Ɍ�������
		void RotToFront();
		//�\�z�Ɣj��
		HeadEnemy(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~HeadEnemy();
		//������
		virtual void OnCreate() override;
		//�A�N�Z�T
		shared_ptr<BehaviorMachine<HeadEnemy>> GetBehaviorMachine() const {
			return m_BehaviorMachine;
		}
		//����
		virtual void OnUpdate() override;
		//�Փˎ�
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
		virtual void OnLastUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	�Î~�s��
	//--------------------------------------------------------------------------------------
	class HeadStop : public Behavior<HeadEnemy> {
		float m_StopTime;
	public:
		HeadStop() :m_StopTime(0){}
		virtual ~HeadStop() {}
		virtual void OnCreateWithParam() {}
		virtual void Enter(const shared_ptr<HeadEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<HeadEnemy>& Obj) override;
		virtual void WakeUp(const shared_ptr<HeadEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<HeadEnemy>& Obj) override;
	};

	//--------------------------------------------------------------------------------------
	//	�W�����v���ĒǐՍs��
	//--------------------------------------------------------------------------------------
	class HeadJumpSeek : public Behavior<HeadEnemy> {
	public:
		HeadJumpSeek() {}
		virtual ~HeadJumpSeek() {}
		virtual void OnCreateWithParam() {}
		virtual void Enter(const shared_ptr<HeadEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<HeadEnemy>& Obj) override;
		virtual void Execute2(const shared_ptr<HeadEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<HeadEnemy>& Obj) override;
	};


	//--------------------------------------------------------------------------------------
	//	�������������G
	//--------------------------------------------------------------------------------------
	class MozoEnemy : public GameObject, public EnemyInterface {
		Vector3 m_StartPos;
		float m_BaseY;
		//�s���}�V�[��
		shared_ptr< BehaviorMachine<MozoEnemy> >  m_BehaviorMachine;
		//���C�g
		Light m_MyLight;
		void CreateMeshRes();
	public:
		//���[�e�B���e�B�֐��Q
		//�v���C���[�̈ʒu��Ԃ�
		Vector3 GetPlayerPosition() const;
		//�v���C���[�܂ł̋�����Ԃ�
		float GetPlayerLength() const;
		//�X�^�[�g���̈ʒu��Ԃ�
		Vector3 GetStartPosition() const {
			return m_StartPos;
		}
		//�\�z�Ɣj��
		MozoEnemy(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~MozoEnemy();
		//������
		virtual void OnCreate() override;
		//�A�N�Z�T
		shared_ptr<BehaviorMachine<MozoEnemy>> GetBehaviorMachine() const {
			return m_BehaviorMachine;
		}
		//����
		virtual void OnUpdate() override;
		//�Փˎ�
		virtual void OnLastUpdate() override;
		//�Ǝ��̃��C�g
		virtual const Light& OnGetDrawLight()const override;
	};

	//--------------------------------------------------------------------------------------
	//	�X�^�[�g�s��
	//--------------------------------------------------------------------------------------
	class MozoStart : public Behavior<MozoEnemy> {
	public:
		MozoStart(){}
		virtual ~MozoStart() {}
		virtual void OnCreateWithParam() {}
		virtual void Enter(const shared_ptr<MozoEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<MozoEnemy>& Obj) override;
		virtual void Execute2(const shared_ptr<MozoEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<MozoEnemy>& Obj) override;
	};

	//--------------------------------------------------------------------------------------
	//	�f�t�H���g�s��
	//--------------------------------------------------------------------------------------
	class MozoDefault : public Behavior<MozoEnemy> {
		bool m_IsInit;
	public:
		MozoDefault():m_IsInit(false){}
		virtual ~MozoDefault() {}
		virtual void OnCreateWithParam() {}
		virtual void Enter(const shared_ptr<MozoEnemy>& Obj) override;
		virtual void Sleep(const shared_ptr<MozoEnemy>& Obj) override;
		virtual void WakeUp(const shared_ptr<MozoEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<MozoEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<MozoEnemy>& Obj) override;
	};

	//--------------------------------------------------------------------------------------
	//	class FixedHead : public GameObject;
	//	�p�r: �Œ�̓�
	//--------------------------------------------------------------------------------------
	class FixedHead : public GameObject {
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedHead(const shared_ptr<Stage>& StagePtr,
			const Vector3& Position
		);
		virtual ~FixedHead();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	//	class FixedCylinder : public GameObject;
	//	�p�r: �Œ�̃V�����_�[
	//--------------------------------------------------------------------------------------
	class FixedCylinder : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
		bool m_BalloonActive;
	public:
		//�\�z�Ɣj��
		FixedCylinder(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~FixedCylinder();
		//������
		virtual void OnCreate() override;
		//����
		void BalloonObOff();
	};




}

//end basecross