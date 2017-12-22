/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�����v�Z����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedPsBox : public GameObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedPsBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~FixedPsBox();
		//������
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃ{�b�N�X
	//--------------------------------------------------------------------------------------
	class ActivePsBox : public GameObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		ActivePsBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsBox();
		//������
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȋ���
	//--------------------------------------------------------------------------------------
	class ActivePsSphere : public GameObject {
		float m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		ActivePsSphere(const shared_ptr<Stage>& StagePtr,
			float Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsSphere();
		//������
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃJ�v�Z��
	//--------------------------------------------------------------------------------------
	class ActivePsCapsule : public GameObject {
		//����
		float m_Len;
		//���a
		float m_Diameter;
		Quat m_Qt;
		Vec3 m_Position;
		//���b�V���i�J�v�Z���͊e�I�u�W�F�N�g���ێ��j
		shared_ptr<MeshResource> m_CapsuleMesh;
	public:
		//�\�z�Ɣj��
		ActivePsCapsule(const shared_ptr<Stage>& StagePtr,
			float Len,
			float Diameter,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsCapsule();
		//������
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃV�����_�[
	//--------------------------------------------------------------------------------------
	class ActivePsCylinder : public GameObject {
		//����
		float m_Len;
		//���a
		float m_Diameter;
		Quat m_Qt;
		Vec3 m_Position;
		//���b�V���i�V�����_�[�͊e�I�u�W�F�N�g���ێ��j
		shared_ptr<MeshResource> m_CylinderMesh;
	public:
		//�\�z�Ɣj��
		ActivePsCylinder(const shared_ptr<Stage>& StagePtr,
			float Len,
			float Diameter,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsCylinder();
		//������
		virtual void OnCreate() override;
	};



	//--------------------------------------------------------------------------------------
	///	�����v�Z���锭�˂��鋅��
	//--------------------------------------------------------------------------------------
	class FirePsSphere : public GameObject {
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		float m_Scale;
		void CreateDefParam(PsSphereParam& param);
	public:
		//�\�z�Ɣj��
		FirePsSphere(const shared_ptr<Stage>& StagePtr,
			const Vec3& Emitter, const Vec3& Velocity
		);
		virtual ~FirePsSphere();
		//������
		virtual void OnCreate() override;
		//���̂����Z�b�g����
		void Reset(const Vec3& Emitter, const Vec3& Velocity);
	};




}
//end basecross
