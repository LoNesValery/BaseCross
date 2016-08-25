/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	������
	//--------------------------------------------------------------------------------------
	class CubeObject : public ObjectInterface, public ShapeInterface {
		//�`��R���e�L�X�g
		shared_ptr<VSPSDrawContext> m_DrawContext;
		// �R���X�^���g�o�b�t�@
		struct StaticConstantBuffer
		{
			Matrix4X4 World;
			Matrix4X4 View;
			Matrix4X4 Projection;
			Color4 Emissive;
			StaticConstantBuffer() {
				memset(this, 0, sizeof(StaticConstantBuffer));
			};
		};
		StaticConstantBuffer m_StaticConstantBuffer;
		//���b�V��
		shared_ptr<MeshResource> m_CubeMesh;
		Vector3 m_Scale;				///<�X�P�[�����O
		Quaternion m_Qt;			///<��]
		Vector3 m_Pos;				///<�ʒu
		void CreateBuffers();
		void UpdateConstantBuffer();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		CubeObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CubeObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};




}
//end basecross
