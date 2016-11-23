/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	�������ێ����邾���̃I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class StringGameObject : public GameObject {
	public:
		//�\�z�Ɣj��
		StringGameObject(const shared_ptr<Stage>& StagePtr);
		virtual ~StringGameObject();
		//������
		virtual void OnCreate() override;
		void SetText(const wstring& str);
	};



}
//end basecross
