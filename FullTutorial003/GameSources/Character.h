/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	�Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~FixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	//	�G
	//--------------------------------------------------------------------------------------
	class Enemy : public GameObject {
		weak_ptr<StageCellMap> m_CelMap;
		Vector3 m_Scale;
		Vector3 m_StartRotation;
		Vector3 m_StartPosition;
		vector<CellIndex> m_CellPath;
		//���݂̎����̃Z���C���f�b�N�X
		int m_CellIndex;
		//�߂����i���́j�̃Z���C���f�b�N�X
		int m_NextCellIndex;
		//�^�[�Q�b�g�̃Z���C���f�b�N�X
		int m_TargetCellIndex;
		shared_ptr<StateMachine<Enemy>> m_StateMachine;
	public:
		//�\�z�Ɣj��
		Enemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<StageCellMap>& CellMap,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~Enemy();
		//�v���C���[�̌���
		bool SearchPlayer();

		//�f�t�H���g�s��
		bool DefaultBehavior();
		//Seek�s��
		bool SeekBehavior();
		//�A�N�Z�T
		shared_ptr< StateMachine<Enemy> > GetStateMachine() const {
			return m_StateMachine;
		}
		//������
		virtual void OnCreate() override;
		//����
		virtual void OnUpdate() override;
		virtual void OnLastUpdate() override;
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
	///	�v���C���[��ǂ�������X�e�[�g
	//--------------------------------------------------------------------------------------
	class EnemySeek : public ObjState<Enemy>
	{
		EnemySeek() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(EnemySeek)
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};

}
//end basecross
