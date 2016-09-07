/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	�p�r: �{�b�N�X
	//--------------------------------------------------------------------------------------
	class Box : public GameObject {
		Vector3 m_StartPos;
		float m_TotalTime;
	public:
		//�\�z�Ɣj��
		Box(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~Box();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
	};


}
//end basecross
