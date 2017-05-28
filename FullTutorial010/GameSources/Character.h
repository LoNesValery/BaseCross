/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	�p�r: ��
	//--------------------------------------------------------------------------------------
	class Sword : public GameObject {
		Vector3 m_Scale;
		wstring m_Tag;
	public:
		//�\�z�Ɣj��
		Sword(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const wstring& Tag
		);
		virtual ~Sword();
		//������
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	//	�p�r: �^�C�����O����v���[�g
	//--------------------------------------------------------------------------------------
	class TilingPlate : public GameObject {
		Vector3 m_Scale;
		Quaternion m_Qt;
		Vector3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//�\�z�Ɣj��
		TilingPlate(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Quaternion& Qt,
			const Vector3& Position,
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
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//�\�z�Ɣj��
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingFixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	// �����̃X�p�[�N�N���X
	//--------------------------------------------------------------------------------------
	class MultiSpark : public MultiParticle {
	public:
		//�\�z�Ɣj��
		MultiSpark(shared_ptr<Stage>& StagePtr);
		virtual ~MultiSpark();
		//������
		virtual void OnCreate() override;
		void InsertSpark(const Vector3& Pos);
	};

	//--------------------------------------------------------------------------------------
	// �����̉��N���X
	//--------------------------------------------------------------------------------------
	class MultiFire : public MultiParticle {
	public:
		//�\�z�Ɣj��
		MultiFire(shared_ptr<Stage>& StagePtr);
		virtual ~MultiFire();
		//������
		virtual void OnCreate() override;
		void InsertFire(const Vector3& Pos);
	};

	//--------------------------------------------------------------------------------------
	//	�G
	//--------------------------------------------------------------------------------------
	class Enemy : public GameObject {
	protected:
		weak_ptr<StageCellMap> m_CelMap;
		Vector3 m_Scale;
		Vector3 m_StartRotation;
		Vector3 m_StartPosition;
		vector<CellIndex> m_CellPath;
		shared_ptr<StateMachine<Enemy>> m_StateMachine;
		//�i�s�����������悤�ɂ���
		void RotToHead();
		//�v���C���[�����������悤�ɂ���
		void RotToPlayer();
		//�Z�������̍s���N���X
		unique_ptr<CellSearchBehavior<Enemy>> m_CellSearchBehavior;
		//�킢�̍s��
		unique_ptr<FightBehavior<Enemy>> m_FightBehavior;
		//�}�b�N�XHP
		float m_MaxHP;
		//���݂�HP
		float m_HP;
	public:
		//�\�z�Ɣj��
		Enemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<StageCellMap>& CellMap,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~Enemy();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Z�������̍s���𓾂�
		@return	�Z�������̍s��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr<CellSearchBehavior<Enemy>>& GetCellSearchBehavior() {
			return m_CellSearchBehavior;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�킢�s���𓾂�
		@return	�킢�s��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr<FightBehavior<Enemy>>& GetFightBehavior() {
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
		@brief	�X�e�[�g�}�V���𓾂�
		@return	�X�e�[�g�}�V���ʒu
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr< StateMachine<Enemy> > GetStateMachine() const {
			return m_StateMachine;
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
		//����
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;
		//�Փˎ�
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
	};

	//--------------------------------------------------------------------------------------
	///	�f�t�H���g�X�e�[�g
	//--------------------------------------------------------------------------------------
	class EnemyDefault : public ObjState<Enemy>
	{
		EnemyDefault() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(EnemyDefault)
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�G������U��X�e�[�g
	//--------------------------------------------------------------------------------------
	class EnemySwordState : public ObjState<Enemy>
	{
		EnemySwordState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(EnemySwordState)
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	�v���C���[�̌��ɓ��������X�e�[�g
	//--------------------------------------------------------------------------------------
	class EnemySwordHit : public ObjState<Enemy>
	{
		EnemySwordHit() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(EnemySwordHit)
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	�|���ꂽ�X�e�[�g
	//--------------------------------------------------------------------------------------
	class EnemyDieState : public ObjState<Enemy>
	{
		EnemyDieState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(EnemyDieState)
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};



	//--------------------------------------------------------------------------------------
	//	HP�̃X�N�G�A�̗̈�
	//--------------------------------------------------------------------------------------
	class HPSquareBase : public GameObject {
	public:
		//�\�z�Ɣj��
		HPSquareBase(const shared_ptr<Stage>& StagePtr);
		virtual ~HPSquareBase();
		//������
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	//	HP�̃X�N�G�A
	//--------------------------------------------------------------------------------------
	class HPSquare : public GameObject {
		Color4 m_Color;
	public:
		//�\�z�Ɣj��
		HPSquare(const shared_ptr<Stage>& StagePtr,const Color4& Col);
		virtual ~HPSquare();
		//������
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	//	HP�̃}�l�[�W��(�e���v���[�g�N���X)
	//--------------------------------------------------------------------------------------
	template<typename T>
	class HPManeger : public GameObject {
		weak_ptr<T> m_TargetObj;
		weak_ptr<HPSquareBase> m_HPSquareBase;
		weak_ptr<HPSquare> m_HPSquare;
		Color4 m_Color;

		void UpdateHPTrans() {
			auto ShTarget = m_TargetObj.lock();
			auto ShHPSquareBase = m_HPSquareBase.lock();
			auto ShHPSquare = m_HPSquare.lock();
			if (ShTarget && ShHPSquareBase && ShHPSquare) {
				auto HPBaseTrans = ShHPSquareBase->GetComponent<Transform>();
				auto HPTrans = ShHPSquare->GetComponent<Transform>();
				//�X�P�[�����O
				float MaxHP = ShTarget->GetMaxHP();
				float HP = ShTarget->GetHP();
				float Width = HP / MaxHP;

				HPBaseTrans->SetScale(0.5f, 0.125f, 0.5);
				HPTrans->SetScale(0.46f * Width, 0.1f, 0.46f);

				auto TargetTrans = ShTarget->GetComponent<Transform>();
				auto Pos = TargetTrans->GetPosition();
				Pos.y += 0.75f;
				HPBaseTrans->SetPosition(Pos);
				Pos = Vector3(0, 0, -0.001f);
				Pos.x -= 0.46f * 0.5f;
				Pos.x += (0.46f * Width * 0.5f);
				HPTrans->SetPosition(Pos);


 				auto PtrCamera = GetStage()->GetView()->GetTargetCamera();
				Quaternion Qt;
				//�������r���{�[�h�ɂ���
				Qt.Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());
				HPBaseTrans->SetQuaternion(Qt);
			}
		}
	public:
		//�\�z�Ɣj��
		HPManeger(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<T>& TargetObj,
			const Color4& Col):
			GameObject(StagePtr),
			m_TargetObj(TargetObj),
			m_Color(Col)
		{}

		virtual ~HPManeger() {}
		//������
		virtual void OnCreate() override {
			auto HPBase = GetStage()->AddGameObject<HPSquareBase>();
			auto HP = GetStage()->AddGameObject<HPSquare>(m_Color);
			auto PtrTrans = HP->GetComponent<Transform>();
			PtrTrans->SetParent(HPBase);

			m_HPSquareBase = HPBase;
			m_HPSquare = HP;
			UpdateHPTrans();
		}

		virtual void OnUpdate2() override {
			UpdateHPTrans();
		}


	};




}
//end basecross
