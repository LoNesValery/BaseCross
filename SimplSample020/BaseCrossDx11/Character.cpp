/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	//--------------------------------------------------------------------------------------
	//	�������ێ����邾���̃I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	StringGameObject::StringGameObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	StringGameObject::~StringGameObject() {}

	//������
	void StringGameObject::OnCreate() {
		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetFont(L"�l�r�S�V�b�N", 32);
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(440.0f, 640.0f, 1200.0f, 800.0f));

	}

	void StringGameObject::SetText(const wstring& str) {
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}



}
//end basecross
