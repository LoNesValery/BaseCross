/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class EggAnime :  public SS5ssae;
	//	�p�r: ���̃A�j���[�V����
	//--------------------------------------------------------------------------------------
	class EggAnime : public SS5ssae {
		float m_Span;
	public:
		//�\�z�Ə���
		EggAnime(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir);
		virtual ~EggAnime();
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate()override;
	};


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};



}
//end basecross
