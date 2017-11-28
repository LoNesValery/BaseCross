/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�����v�Z����{�b�N�X
	//--------------------------------------------------------------------------------------
	class PhysicsBox : public GameObject {
		PsMotionType m_MotionType;
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
		size_t m_PsTransIndex;
	public:
		//�\�z�Ɣj��
		PhysicsBox(const shared_ptr<Stage>& StagePtr,
			PsMotionType MotionType,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~PhysicsBox();
		//������
		virtual void OnCreate() override;
		//����
		virtual void OnUpdate() override;
	};


}
//end basecross
