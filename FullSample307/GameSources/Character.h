/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	�p�r: �^�C�����O����v���[�g
	//--------------------------------------------------------------------------------------
	class TilingPlate : public GameObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//�\�z�Ɣj��
		TilingPlate(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode);
		virtual ~TilingPlate();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//�\�z�Ɣj��
		TilingFixedBox(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode);
		virtual ~TilingFixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	///	�t���[���{�b�N�X
	//--------------------------------------------------------------------------------------
	class FrameBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		FrameBox(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode);
		virtual ~FrameBox();
		//������
		virtual void OnCreate() override;
		//����
	};






}
//end basecross
