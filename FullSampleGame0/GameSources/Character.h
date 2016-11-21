/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~FixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};


	//--------------------------------------------------------------------------------------
	//	class RoomDoor : public GameObject;
	//	�p�r: �����̃h�A
	//--------------------------------------------------------------------------------------
	class RoomDoor : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
		wstring m_TexResName;
	public:
		//�\�z�Ɣj��
		RoomDoor(const shared_ptr<Stage>& StagePtr,
			const wstring& TexResName,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~RoomDoor();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	//	�����̑�
	//--------------------------------------------------------------------------------------
	class RoomWindow : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		RoomWindow(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~RoomWindow();
		//������
		virtual void OnCreate() override;
		//����
		void WindowClear();
	};

	
	//--------------------------------------------------------------------------------------
	//	class PopupBalloon : public GameObject;
	//	�p�r:�@�����o��
	//--------------------------------------------------------------------------------------
	class PopupBalloon : public GameObject {
		Vector3 m_Scale;
		Vector3 m_EmitterPosition;
		wstring m_TexResName;
	public:
		//�\�z�Ɣj��
		PopupBalloon(const shared_ptr<Stage>& StagePtr,
			const wstring& TexResName,
			const Vector3& Scale
		);
		virtual ~PopupBalloon();
		//������
		virtual void OnCreate() override;
		//����
		virtual void OnUpdate() override;
		//�\��
		void PopupOpen(const Vector3& EmitterPosition);
		//�N���[�Y
		void PopupClose();
	};

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
