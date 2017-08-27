/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�A�N�V��������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class ActionObject : public GameObject {
		Vec3 m_StartPos;
	public:
		//�\�z�Ɣj��
		ActionObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~ActionObject();
		//������
		virtual void OnCreate() override;
	};


}
//end basecross
