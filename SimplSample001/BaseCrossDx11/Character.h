/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O�p�`�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class TriangleObject : public ObjectInterface, public ShapeInterface {
		//���b�V��
		shared_ptr<MeshResource> m_TriangleMesh;
	public:
		//�\�z�Ɣj��
		TriangleObject();
		virtual ~TriangleObject();
		//������
		virtual void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override;
	};


}
//end basecross
