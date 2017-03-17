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
		Vector3 m_StartPos;
	public:
		//�\�z�Ɣj��
		PntSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~PntSphere();
		//������
		virtual void OnCreate() override;
		virtual void OnDraw() override;
	};



}
//end basecross
