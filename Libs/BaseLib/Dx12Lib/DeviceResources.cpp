/*!
@file DeviceResources.h
@brief �e�N�X�`���A���b�V���ADx12�f�o�C�X���\�[�X����
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct TextureResource::Impl;
	//--------------------------------------------------------------------------------------
	struct TextureResource::Impl {
		Impl(){}
	};

	//--------------------------------------------------------------------------------------
	// class TextureResource : public BaseResource
	//--------------------------------------------------------------------------------------
	TextureResource::TextureResource() :
		BaseResource(),
		pImpl(new Impl())
	{}
	TextureResource::~TextureResource() {}

	shared_ptr<TextureResource> TextureResource::CreateTextureResource(const wstring& FileName, const wstring& TexType) {
		return nullptr;
	}


	//--------------------------------------------------------------------------------------
	//	struct DeviceResources::Impl;
	//--------------------------------------------------------------------------------------
	struct DeviceResources::Impl {
		Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class DeviceResources;
	//--------------------------------------------------------------------------------------
	DeviceResources::DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height) :
		pImpl(new Impl())
	{}

	DeviceResources::~DeviceResources() {
	}

	void DeviceResources::Present(unsigned int SyncInterval, unsigned int  Flags) {

	}








}
//end basecross
