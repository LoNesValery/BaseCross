/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	class FineSeekBehavior;
	class TiredSeekBehavior;
	class SeekBehavior;
	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	�p�r: �ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class SeekObject : public GameObject {
		//�s���N���X��friend�錾��t����
		friend class FineSeekBehavior;
		friend class TiredSeekBehavior;
		//���݂̍s���I�u�W�F�N�g
		shared_ptr<SeekBehavior> m_SeekBehavior;
		//�s����؂肩���邽�߂̃^�C�}�[
		float m_FineTiredTime;
		shared_ptr< StateMachine<SeekObject> >  m_StateMachine;	//�X�e�[�g�}�V�[��

		Vector3 m_StartPos;
		float m_BaseY;
		float m_StateChangeSize;
		//���[�e�B���e�B�֐��Q
		//�v���C���[�̈ʒu��Ԃ�
		Vector3 GetPlayerPosition() const;
		//�v���C���[�܂ł̋�����Ԃ�
		float GetPlayerLength() const;
	public:
		//�\�z�Ɣj��
		SeekObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~SeekObject();
		//������
		virtual void OnCreate() override;
		//�A�N�Z�T
		//�s��
		shared_ptr<SeekBehavior> GetSeekBehavior() const {
			return m_SeekBehavior;
		}
		//�X�e�[�g�}�V��
		shared_ptr< StateMachine<SeekObject> > GetStateMachine() const {
			return m_StateMachine;
		}
		//����
		virtual void OnUpdate() override;
		virtual void OnLastUpdate() override;
	};


	//--------------------------------------------------------------------------------------
	//	Seek�I�u�W�F�N�g�̍s���̐e�N���X
	//--------------------------------------------------------------------------------------
	class SeekBehavior {
	protected:
		SeekBehavior() {}
		virtual ~SeekBehavior() {}
	public:
		virtual void FarEnter(const shared_ptr<SeekObject>& Obj) = 0;
		virtual void FarExecute(const shared_ptr<SeekObject>& Obj) = 0;
		virtual void FarExit(const shared_ptr<SeekObject>& Obj) = 0;
		virtual void NearEnter(const shared_ptr<SeekObject>& Obj) = 0;
		virtual void NearExecute(const shared_ptr<SeekObject>& Obj) = 0;
		virtual void NearExit(const shared_ptr<SeekObject>& Obj) = 0;
	};

	//--------------------------------------------------------------------------------------
	//	Seek�I�u�W�F�N�g�̌��C�ȍs��
	//--------------------------------------------------------------------------------------
	class FineSeekBehavior : public SeekBehavior {
		FineSeekBehavior() {}
	public:
		static shared_ptr<FineSeekBehavior> Instance();
		virtual void FarEnter(const shared_ptr<SeekObject>& Obj) override;
		virtual void FarExecute(const shared_ptr<SeekObject>& Obj) override;
		virtual void FarExit(const shared_ptr<SeekObject>& Obj) override;
		virtual void NearEnter(const shared_ptr<SeekObject>& Obj) override;
		virtual void NearExecute(const shared_ptr<SeekObject>& Obj) override;
		virtual void NearExit(const shared_ptr<SeekObject>& Obj) override;
	};

	//--------------------------------------------------------------------------------------
	//	Seek�I�u�W�F�N�g�̔�ꂽ�s��
	//--------------------------------------------------------------------------------------
	class TiredSeekBehavior : public SeekBehavior {
		TiredSeekBehavior() {}
	public:
		static shared_ptr<TiredSeekBehavior> Instance();
		virtual void FarEnter(const shared_ptr<SeekObject>& Obj) override;
		virtual void FarExecute(const shared_ptr<SeekObject>& Obj) override;
		virtual void FarExit(const shared_ptr<SeekObject>& Obj) override;
		virtual void NearEnter(const shared_ptr<SeekObject>& Obj) override;
		virtual void NearExecute(const shared_ptr<SeekObject>& Obj) override;
		virtual void NearExit(const shared_ptr<SeekObject>& Obj) override;
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

	//--------------------------------------------------------------------------------------
	//	class StaticModel : public GameObject;
	//	�p�r: �Œ�̃��f��
	//--------------------------------------------------------------------------------------
	class StaticModel : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		StaticModel(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~StaticModel();
		//������
		virtual void OnCreate() override;
		//����
	};



	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
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
	//	class MoveBox : public GameObject;
	//	�p�r: �㉺�ړ�����{�b�N�X
	//--------------------------------------------------------------------------------------
	class MoveBox : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		MoveBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~MoveBox();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	//	class SphereObject : public GameObject;
	//	�p�r: ��Q����
	//--------------------------------------------------------------------------------------
	class SphereObject : public GameObject {
		Vector3 m_StartPos;
	public:
		//�\�z�Ɣj��
		SphereObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~SphereObject();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	//	class CapsuleObject : public GameObject;
	//	�p�r: ��Q���J�v�Z��
	//--------------------------------------------------------------------------------------
	class CapsuleObject : public GameObject {
		Vector3 m_StartPos;
	public:
		//�\�z�Ɣj��
		CapsuleObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~CapsuleObject();
		//������
		virtual void OnCreate() override;
		//����
	};




}
//end basecross
