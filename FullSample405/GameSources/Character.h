/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	Pnt��
	//--------------------------------------------------------------------------------------
	class PntSphere : public GameObject {
		Vec3 m_StartPos;
	public:
		//�\�z�Ɣj��
		PntSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~PntSphere();
		//������
		virtual void OnCreate() override;
		virtual void OnDraw() override;
	};



}
//end basecross
