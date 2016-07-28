/*!
@file DeviceResources.h
@brief �e�N�X�`���A���b�V���ADx11�f�o�C�X���\�[�X
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/*!
	��������ɕێ�����Dx12�e�N�X�`�����\�[�X
	*/
	//--------------------------------------------------------------------------------------
	class TextureResource : public BaseResource {
		friend class ObjectFactory;
		TextureResource();
	public:
		virtual ~TextureResource();
		static  shared_ptr<TextureResource> CreateTextureResource(const wstring& FileName, const wstring& TexType = L"WIC");

		//���z�֐�
		virtual void OnPreCreate()override {}
		virtual void OnCreate()override {}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	/*!
	@breaf Dx11���b�V���N���X.<br />
	���b�V���́A�X�^�e�B�b�N�֐����g���Đ�������
	*/
	//--------------------------------------------------------------------------------------
	class MeshResource : public BaseResource {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �R���X�g���N�^<br />
		�\�z�̓X�^�e�B�b�N�֐��𗘗p����
		*/
		//--------------------------------------------------------------------------------------
		MeshResource()
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MeshResource() {}

		//���z�֐�
		virtual void OnPreCreate()override {}
		virtual void OnCreate()override {}
	};


	//--------------------------------------------------------------------------------------
	/*!
	DirectX11�f�o�C�X�N���X
	*/
	//--------------------------------------------------------------------------------------
	class DeviceResources {
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �R���X�g���N�^
		@param[in] hWnd	�E�C���h�E�̃n���h��
		@param[in] isFullScreen �t���X�N���[�����ǂ���
		@param[in] Width		��
		@param[in] Height		����
		*/
		//--------------------------------------------------------------------------------------
		DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DeviceResources();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���e���c�̍쐬��̏���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterInitContents() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �t�����g�o�b�t�@�ɓ]������
		@param[in]	SyncInterval	�C���^�[�o��
		@param[in]	Flags		�t���O
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Present(unsigned int SyncInterval, unsigned int  Flags);
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




}
//end basecross
