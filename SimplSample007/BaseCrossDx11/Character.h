/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	PT�X�v���C�g�`��
	//--------------------------------------------------------------------------------------
	class PTSpriteDraw : public ObjectInterface {
		bool m_Trace;					///<�����������邩�ǂ���
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		shared_ptr<TextureResource> m_TextureResource;	///<�e�N�X�`�����\�[�X
	public:
		PTSpriteDraw(const wstring& TextureFileName, bool Trace);
		virtual ~PTSpriteDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		///�`�揈��
		void DrawObject(const shared_ptr<MeshResource>& Mesh, DiffuseSpriteConstantBuffer& CBuff);
	};



	//--------------------------------------------------------------------------------------
	///	�ǃX�v���C�g
	//--------------------------------------------------------------------------------------
	class WallSprite : public ObjectInterface, public ShapeInterface {
		//PT�X�v���C�g�`��
		shared_ptr<PTSpriteDraw> m_PTSpriteDraw;
		//���b�V��
		shared_ptr<MeshResource> m_SquareMesh;
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		bool m_Trace;					///<�����������邩�ǂ���
		Vec2 m_Scale;				///<�X�P�[�����O
		Vec2 m_Pos;				///<�ʒu
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	TextureFileName	�e�N�X�`���t�@�C����
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		WallSprite(const wstring& TextureFileName, bool Trace, const Vec2& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~WallSprite();
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
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};



	//--------------------------------------------------------------------------------------
	///	�l�p�`�X�v���C�g
	//--------------------------------------------------------------------------------------
	class SquareSprite : public ObjectInterface, public ShapeInterface {
		//PT�X�v���C�g�`��
		shared_ptr<PTSpriteDraw> m_PTSpriteDraw;
		//���b�V��
		shared_ptr<MeshResource> m_SquareMesh;
		//�o�b�N�A�b�v���Ă������_�f�[�^
		vector<VertexPositionTexture> m_BackupVertices;
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		bool m_Trace;					///<�����������邩�ǂ���
		Vec2 m_Scale;				///<�X�P�[�����O
		Vec2 m_Pos;				///<�ʒu
		float m_TotalTime;
		//���_�̕ύX
		void UpdateVertex(float ElapsedTime);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	TextureFileName	�e�N�X�`���t�@�C����
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		SquareSprite(const wstring& TextureFileName, bool Trace, const Vec2& StartPos);
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
