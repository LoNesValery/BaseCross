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
		// Diffuse����R���X�^���g�o�b�t�@
		struct DiffuseSpriteConstantBuffer
		{
			Matrix4X4 World;
			Color4 Emissive;
			Color4 Diffuse;
			DiffuseSpriteConstantBuffer() {
				memset(this, 0, sizeof(DiffuseSpriteConstantBuffer));
			};
		};
		DiffuseSpriteConstantBuffer m_DiffuseSpriteConstantBuffer;
		///���b�V��
		shared_ptr<MeshResource> m_SquareMesh;
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		bool m_Trace;					///<�����������邩�ǂ���
		shared_ptr<TextureResource> m_TextureResource;	///<�e�N�X�`�����\�[�X
		Vector2 m_Scale;				///<�X�P�[�����O
		float m_Rot;				///<��]�p�x
		Vector2 m_Pos;				///<�ʒu
		Vector2 m_PosSpan;				///<�ʒu�ύX�Ԋu
		Color4 m_Diffuse;			///<�ω�������f�t�B�[�Y
		float m_TotalTime;			///<�^�C��
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	TextureFileName	�e�N�X�`���t�@�C����
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		SquareSprite(const wstring& TextureFileName, bool Trace, const Vector2& StartPos);
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
