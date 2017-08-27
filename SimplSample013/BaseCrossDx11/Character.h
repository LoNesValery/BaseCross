/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Scene;

	//--------------------------------------------------------------------------------------
	///	����
	//--------------------------------------------------------------------------------------
	class SquareObject : public ObjectInterface, public ShapeInterface {
		weak_ptr<Scene> m_Scene;			///<�V�[��
		//���b�V��
		shared_ptr<MeshResource> m_SquareMesh;
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		shared_ptr<TextureResource> m_TextureResource;	///<�e�N�X�`�����\�[�X
		Vec3 m_Scale;				///<�X�P�[�����O
		Quat m_Qt;			///<��]
		Vec3 m_Pos;				///<�ʒu
		void CreateBuffers();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	PtrScene	�V�[���̃|�C���^
		@param[in]	TextureFileName	�e�N�X�`���t�@�C����
		@param[in]	Scale	�X�P�[�����O
		@param[in]	Qt	������]
		@param[in]	Pos	�ʒu
		*/
		//--------------------------------------------------------------------------------------
		SquareObject(const shared_ptr<Scene> PtrScene,
			const wstring& TextureFileName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareObject();
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



	//--------------------------------------------------------------------------------------
	///	��
	//--------------------------------------------------------------------------------------
	class SphereObject : public ObjectInterface, public ShapeInterface {
		weak_ptr<Scene> m_Scene;			///<�V�[��
		//���b�V��
		shared_ptr<MeshResource> m_SphereMesh;
		UINT m_Division;				///<������
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		shared_ptr<TextureResource> m_TextureResource;	///<�e�N�X�`�����\�[�X
		Vec3 m_Scale;				///<�X�P�[�����O
		Quat m_Qt;			///<��]
		Vec3 m_Pos;				///<�ʒu
		bool m_Trace;					///<�����������邩�ǂ���
		void CreateBuffers();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	PtrScene	�V�[���̃|�C���^
		@param[in]	Division	������
		@param[in]	TextureFileName	�e�N�X�`���t�@�C����
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	Pos	�ʒu
		*/
		//--------------------------------------------------------------------------------------
		SphereObject(const shared_ptr<Scene> PtrScene,
			UINT Division, const wstring& TextureFileName, bool Trace, const Vec3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SphereObject();
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
