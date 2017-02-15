/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	�p�r: �ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class SeekObject : public GameObject {
		//�X�e�[�g�}�V�[��
		unique_ptr< StateMachine<SeekObject> >  m_StateMachine;
		Vector3 m_StartPos;
		float m_BaseY;
		float m_StateChangeSize;
	public:
		//�\�z�Ɣj��
		SeekObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~SeekObject();
		//������
		virtual void OnCreate() override;
		//�A�N�Z�T
		const unique_ptr<StateMachine<SeekObject>>& GetStateMachine(){
			return m_StateMachine;
		}
		float GetStateChangeSize() const {
			return m_StateChangeSize;
		}
		//����
		virtual void OnUpdate() override;
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
	//	class FixedNormalBox : public GameObject;
	//	�p�r: �Œ�̖@���}�b�v�����{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedNormalBox : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
		wstring m_TexKey;
		wstring m_NormalTexKey;
	public:
		//�\�z�Ɣj��
		FixedNormalBox(const shared_ptr<Stage>& StagePtr,
			const wstring& TexKey,
			const wstring& NormalTexKey,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~FixedNormalBox();
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
	//	class UnevenGroundData : public GameObject;
	//	�p�r: �ł��ڂ����̃f�[�^
	//--------------------------------------------------------------------------------------
	class UnevenGroundData : public GameObject {
		vector<TRIANGLE> m_Triangles;
	public:
		//�\�z�Ɣj��
		UnevenGroundData(const shared_ptr<Stage>& StagePtr);
		virtual ~UnevenGroundData();
		//������
		virtual void OnCreate() override;
		//�O�p�`�̔z���Ԃ�
		const vector<TRIANGLE>& GetTriangles() const {
			return m_Triangles;
		}
	};



	//--------------------------------------------------------------------------------------
	//	class UnevenGround : public GameObject;
	//	�p�r: �ł��ڂ���
	//--------------------------------------------------------------------------------------
	class UnevenGround : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		UnevenGround(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position);
		virtual ~UnevenGround();
		//������
		virtual void OnCreate() override;
		//����
	};




}
//end basecross
