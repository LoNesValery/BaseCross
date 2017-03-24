/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	//--------------------------------------------------------------------------------------
	///	Static�L����
	//--------------------------------------------------------------------------------------
	class StaticChara : public GameObject {
		Vector3 m_StartPos;
	public:
		//�\�z�Ɣj��
		StaticChara(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~StaticChara();
		//������
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	///	Bone�L����
	//--------------------------------------------------------------------------------------
	class BoneChara : public GameObject {
		Vector3 m_StartPos;
	public:
		//�\�z�Ɣj��
		BoneChara(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~BoneChara();
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};




}
//end basecross
