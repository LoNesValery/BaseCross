/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	class NormalTextureBox  : public ObjectInterface,public ShapeInterface;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class NormalTextureBox : public ObjectInterface,public ShapeInterface {
		//�`��R���e�L�X�g
		shared_ptr<VSPSDrawContext> m_DrawContext;
		//�R���X�^���g�o�b�t�@�\����
		struct ConstantBuffer
		{
			Matrix4X4 World;
			Matrix4X4 View;
			Matrix4X4 Projection;
			Vector4 LightDir;
			Color4 Emissive;
			Color4 Diffuse;
			ConstantBuffer() {
				memset(this, 0, sizeof(ConstantBuffer));
			};
		};
		//�R���X�^���g�o�b�t�@�f�[�^
		ConstantBuffer m_ConstantBufferData;
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
		void OnUpdate()override;
		void OnDraw()override;
	};



}
//end basecross
