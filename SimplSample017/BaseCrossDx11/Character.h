/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class StaticModel : public GameObject;
	//	�p�r: �Œ�̃��f��
	//--------------------------------------------------------------------------------------
	class StaticModel : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		StaticModel(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~StaticModel();
		//������
		virtual void OnCreate() override;
		//����
	};


}
//end basecross
