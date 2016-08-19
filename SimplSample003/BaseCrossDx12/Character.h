/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�l�p�`�X�v���C�g
	//--------------------------------------------------------------------------------------
	class SquareSprite : public ObjectInterface, public ShapeInterface {
		//�`��R���e�L�X�g
		shared_ptr<VSPSDrawContext> m_DrawContext;
		struct SpriteConstantBuffer
		{
			Matrix4X4 World;
			Color4 Emissive;
			SpriteConstantBuffer() {
				memset(this, 0, sizeof(SpriteConstantBuffer));
			};
		};
		SpriteConstantBuffer m_SpriteConstantBuffer;
		///���b�V��
		shared_ptr<MeshResource> m_SquareMesh;
		Vector2 m_Scale;				///<�X�P�[�����O
		float m_Rot;				///<��]�p�x
		Vector2 m_Pos;				///<�ʒu
		Vector2 m_PosSpan;				///<�ʒu�ύX�Ԋu
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		SquareSprite();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareSprite();
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
