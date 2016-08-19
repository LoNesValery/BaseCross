/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O�p�`�X�v���C�g
	//--------------------------------------------------------------------------------------
	class TriangleSprite : public ObjectInterface, public ShapeInterface {
		///���b�V��
		shared_ptr<MeshResource> m_TriangleMesh;
		Vector3 m_Pos;					///<���݂̈ʒu
		Vector3 m_MoveSpan;				///<�ʒu�ύX�l
	public:
		//�\�z�Ɣj��
		TriangleSprite();
		virtual ~TriangleSprite();
		//������
		virtual void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override;
	};


}
//end basecross
