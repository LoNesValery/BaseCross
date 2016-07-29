/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	class NormalTextureBox  : public ObjectInterface;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class NormalTextureBox : public ObjectInterface {
		//���b�V��
		shared_ptr<MeshResource> m_CubeMesh;
		//�e�N�X�`��
		shared_ptr<TextureResource>m_WallTex;
		Vector3 m_Scale;
		Vector3 m_Pos;
		Quaternion m_Qt;
	public:
		//�\�z�Ɣj��
		NormalTextureBox(const Vector3& Pos);
		virtual ~NormalTextureBox();
		//������
		virtual void OnCreate()override;
		void OnUpdate();
		void OnDraw();
	};


}
//end basecross
