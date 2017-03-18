/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	RollingBox�O���[�v(�C���X�^���X�`��̂��߂����̃I�u�W�F�N�g)
	//--------------------------------------------------------------------------------------
	class RollingBoxGroup : public GameObject {
	public:
		//�\�z�Ɣj��
		RollingBoxGroup(const shared_ptr<Stage>& StagePtr);
		virtual ~RollingBoxGroup();
		//������
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	//	class RollingBox : public GameObject;
	//	�p�r: �Œ�̉�]�{�b�N�X
	//--------------------------------------------------------------------------------------
	class RollingBox : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
		//��]�W��
		float m_RotParam;
	public:
		//�\�z�Ɣj��
		RollingBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position,
			float RotParam
		);
		virtual ~RollingBox();
		//������
		virtual void OnCreate() override;
		//����
		virtual void OnUpdate() override;
	};



}
//end basecross
