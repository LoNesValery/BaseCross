/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class NormalTextureBox  : public GameObject;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class NormalTextureBox : public GameObject {
	public:
		//�\�z�Ɣj��
		NormalTextureBox(const shared_ptr<Stage>& StagePtr);
		virtual ~NormalTextureBox();
		//������
		virtual void OnCreate()override {}
		void OnCreateWithParam(const Vector3& Pos);
		virtual void OnUpdate();
	};



}
//end basecross
