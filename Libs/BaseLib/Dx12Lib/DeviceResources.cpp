/*!
@file DeviceResources.h
@brief �e�N�X�`���A���b�V���ADx12�f�o�C�X���\�[�X����
*/
#include "stdafx.h"

namespace basecross {

	struct D3D11CUSTAM_SUBRESOURCE_DATA
	{
		const void *pSysMem;
		UINT SysMemPitch;
		UINT SysMemSlicePitch;
	};

	//--------------------------------------------------------------------------------------
	//	struct TextureResource::Impl;
	//--------------------------------------------------------------------------------------
	struct TextureResource::Impl {
		//�e�N�X�`���p�̃f�X�N�v���^
		D3D12_RESOURCE_DESC m_TextureResDesc;
		UINT m_TexturePixelSize;
		//���\�[�X
		ComPtr<ID3D12Resource> m_Texture;
		//GPU�A�b�v���[�h�̂��߂̃��\�[�X
		ComPtr<ID3D12Resource> m_TextureUploadHeap;
		//�e�N�X�`���t�@�C���ւ̃p�X
		wstring m_FileName{ L"" };
		//�e�N�X�`���f�[�^
		vector<uint8_t> m_Data;
		bool m_DataRefresh;
		Impl() :m_TexturePixelSize(4), m_DataRefresh(true) {}
		//�e�N�X�`���쐬
		void CreateTexture(uint32_t Width, uint32_t Height) {
			m_TexturePixelSize = 4;
			::ZeroMemory(&m_TextureResDesc, sizeof(D3D12_RESOURCE_DESC));
			m_TextureResDesc.MipLevels = 1;
			m_TextureResDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			m_TextureResDesc.Width = Width;
			m_TextureResDesc.Height = Height;
			m_TextureResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			m_TextureResDesc.DepthOrArraySize = 1;
			m_TextureResDesc.SampleDesc.Count = 1;
			m_TextureResDesc.SampleDesc.Quality = 0;
			m_TextureResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			auto Dev = App::GetApp()->GetDeviceResources();
			ThrowIfFailed(
				Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE, &m_TextureResDesc,
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&m_Texture)),
				L"�e�N�X�`�����\�[�X�̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"TextureResource::Impl::CreateTexture()"
			);
		}

		void CreateTexture(const Image* srcImages, size_t nimages, const DirectX::TexMetadata& metadata) {
			if (metadata.IsVolumemap()) {
				ThrowBaseException(
					L"���̌`���ɂ͑Ή����Ă܂���",
					L"if (metadata.IsVolumemap())",
					L"TextureResource::Impl::CreateTexture()"
				);
			}

			std::unique_ptr<D3D11CUSTAM_SUBRESOURCE_DATA[]> initData(new (std::nothrow) D3D11CUSTAM_SUBRESOURCE_DATA[metadata.mipLevels * metadata.arraySize]);
			if (!initData) {
				ThrowBaseException(
					L"�e�N�X�`���f�[�^�̗̈�擾�Ɏ��s���܂���",
					L"if (!initData)",
					L"TextureResource::Impl::CreateTexture()"
				);

			}

			//--- 1D or 2D texture case ---------------------------------------------------
			size_t idx = 0;
			for (size_t item = 0; item < metadata.arraySize; ++item)
			{
				for (size_t level = 0; level < metadata.mipLevels; ++level)
				{
					size_t index = metadata.ComputeIndex(level, item, 0);
					if (index >= nimages) {
						ThrowBaseException(
							L"�f�[�^�̃C���f�b�N�X���͈͊O�ł�",
							L"if (index >= nimages)",
							L"TextureResource::Impl::CreateTexture()"
						);

					}
					const Image& img = srcImages[index];

					if (img.format != metadata.format) {
						ThrowBaseException(
							L"�f�[�^�̃t�H�[�}�b�g���Ⴂ�܂�",
							L"if (img.format != metadata.format)",
							L"TextureResource::Impl::CreateTexture()"
						);
					}
					if (!img.pixels) {
						ThrowBaseException(
							L"�f�[�^�̃|�C���^���s���ł�",
							L"if (!img.pixels)",
							L"TextureResource::Impl::CreateTexture()"
						);
					}
					if (idx >= (metadata.mipLevels * metadata.arraySize)) {
						ThrowBaseException(
							L"�f�[�^�̕��т��s��ł�",
							L"if (idx >= (metadata.mipLevels * metadata.arraySize))",
							L"TextureResource::Impl::CreateTexture()"
						);
					}
					initData[idx].pSysMem = img.pixels;
					initData[idx].SysMemPitch = static_cast<DWORD>(img.rowPitch);
					initData[idx].SysMemSlicePitch = static_cast<DWORD>(img.slicePitch);
					++idx;
				}
			}

			DXGI_FORMAT tformat = metadata.format;

			switch (metadata.dimension)
			{
			case TEX_DIMENSION_TEXTURE1D:
			{
				::ZeroMemory(&m_TextureResDesc, sizeof(D3D12_RESOURCE_DESC));
				m_TextureResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
				m_TextureResDesc.Format = tformat;
				m_TextureResDesc.Width = static_cast<UINT>(metadata.width);
				m_TextureResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
				m_TextureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.depth);
				m_TextureResDesc.SampleDesc.Count = 1;
				m_TextureResDesc.SampleDesc.Quality = 0;
				m_TextureResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
			}
			break;

			case TEX_DIMENSION_TEXTURE2D:
			{

				::ZeroMemory(&m_TextureResDesc, sizeof(D3D12_RESOURCE_DESC));
				m_TextureResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
				m_TextureResDesc.Format = tformat;
				m_TextureResDesc.Width = static_cast<UINT>(metadata.width);
				m_TextureResDesc.Height = static_cast<UINT>(metadata.height);
				m_TextureResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
				if (metadata.IsCubemap()) {
					m_TextureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
				}
				else {
					m_TextureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.depth);
				}
				m_TextureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
				m_TextureResDesc.SampleDesc.Count = 1;
				m_TextureResDesc.SampleDesc.Quality = 0;
				m_TextureResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			}
			break;

			case TEX_DIMENSION_TEXTURE3D:
			{
				::ZeroMemory(&m_TextureResDesc, sizeof(D3D12_RESOURCE_DESC));
				m_TextureResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
				m_TextureResDesc.Format = tformat;
				m_TextureResDesc.Width = static_cast<UINT>(metadata.width);
				m_TextureResDesc.Height = static_cast<UINT>(metadata.height);
				m_TextureResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
				m_TextureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
				m_TextureResDesc.SampleDesc.Count = 1;
				m_TextureResDesc.SampleDesc.Quality = 0;
				m_TextureResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
			}
			break;
			}

			m_TexturePixelSize = initData[0].SysMemPitch / (UINT)m_TextureResDesc.Width;
			auto Dev = App::GetApp()->GetDeviceResources();
			ThrowIfFailed(
				Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE, &m_TextureResDesc,
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&m_Texture)),
				L"�e�N�X�`�����\�[�X�̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"TextureResource::Impl::CreateTexture()"
			);
			//
			m_Data.clear();
			uint8_t* ptr = (uint8_t*)initData[0].pSysMem;
			unsigned int rowPitch = (unsigned int)m_TextureResDesc.Width * m_TexturePixelSize;
			m_Data.resize(rowPitch * (unsigned int)m_TextureResDesc.Height);
			for (uint32_t i = 0; i < m_Data.size(); i++) {
				m_Data[i] = (uint8_t)ptr[i];
			}
			m_DataRefresh = true;


		}

		//�f�[�^�̃R�s�[
		void CopyData(const vector<uint8_t>& data) {
			m_Data.clear();
			unsigned int rowPitch = (unsigned int)m_TextureResDesc.Width * m_TexturePixelSize;
			m_Data.resize(rowPitch * (unsigned int)m_TextureResDesc.Height);
			if (m_Data.size() > data.size()) {
				ThrowBaseException(
					L"�n���ꂽ�f�[�^�̃T�C�Y���A�c���̐��ɑ���܂���",
					L"if (pImpl->m_Data.size() > data.size())",
					L"TextureResource::Impl::CopyData()"
				);
			}
			for (uint32_t i = 0; i < m_Data.size(); i++) {
				m_Data[i] = data[i];
			}
			m_DataRefresh = true;
		}

		//GPU�A�b�v���[�h�o�b�t�@�̍쐬.
		void CreateTextureUploadHeap() {
			auto Dev = App::GetApp()->GetDeviceResources();
			const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_Texture.Get(), 0, 1);

			ThrowIfFailed(
				Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&m_TextureUploadHeap)),
				L"GPU�A�b�v���[�h�o�b�t�@�̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"TextureResource::Impl::CreateTextureUploadHeap()"
			);
		}

	};

	//--------------------------------------------------------------------------------------
	// class TextureResource : public BaseResource
	//--------------------------------------------------------------------------------------
	TextureResource::TextureResource() :
		BaseResource(),
		pImpl(new Impl())
	{}
	TextureResource::~TextureResource() {}

	void TextureResource::OnCreateWithParam(uint32_t Width, uint32_t Height, const vector<uint8_t>& data) {
		pImpl->CreateTexture(Width, Height);
		pImpl->CopyData(data);
		pImpl->CreateTextureUploadHeap();
	}

	shared_ptr<TextureResource> TextureResource::CreateTextureResource(const wstring& FileName, const wstring& TexType) {
		return ObjectFactory::CreateWithParam<TextureResource>(FileName, TexType);
	}

	void TextureResource::OnCreateWithParam(const wstring& FileName, const wstring& TexType) {

		//�t�@�C���ł̍\�z
		try {

			pImpl->m_FileName = FileName;
			if (FileName == L"") {
				ThrowBaseException(
					L"�t�@�C�����w�肳��Ă��܂���",
					L"if(FileName == L\"\")",
					L"TextureResource::OnCreateWithParam()"
				);
			}
			DWORD RetCode;
			RetCode = GetFileAttributes(FileName.c_str());
			if (RetCode == -1) {
				wstring patherr = FileName;
				ThrowBaseException(
					L"�t�@�C�������݂��܂���",
					patherr.c_str(),
					L"TextureResource::OnCreateWithParam()"
				);
			}
			//�e�N�X�`���쐬
			DirectX::TexMetadata metadata;
			DirectX::ScratchImage image;

			if (TexType == L"WIC") {
				//�t�@�C���g���q�̒���
				wchar_t Drivebuff[_MAX_DRIVE];
				wchar_t Dirbuff[_MAX_DIR];
				wchar_t FileNamebuff[_MAX_FNAME];
				wchar_t Extbuff[_MAX_EXT];

				::ZeroMemory(Drivebuff, sizeof(Drivebuff));
				::ZeroMemory(Dirbuff, sizeof(Dirbuff));
				::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
				::ZeroMemory(Extbuff, sizeof(Extbuff));

				_wsplitpath_s(FileName.c_str(),
					Drivebuff, _MAX_DRIVE,
					Dirbuff, _MAX_DIR,
					FileNamebuff, _MAX_FNAME,
					Extbuff, _MAX_EXT);

				wstring ExtStr = Extbuff;

				if (ExtStr == L".dds" || ExtStr == L".DDS") {
					ThrowIfFailed(
						DirectX::LoadFromDDSFile(FileName.c_str(), 0, &metadata, image),
						L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
						FileName,
						L"TextureResource::OnCreateWithParam()"
					);
				}
				else if (ExtStr == L".tga" || ExtStr == L".TGA") {
					ThrowIfFailed(
						DirectX::LoadFromTGAFile(FileName.c_str(), &metadata, image),
						L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
						FileName,
						L"TextureResource::OnCreateWithParam()"
					);
				}
				else {
					ThrowIfFailed(
						DirectX::LoadFromWICFile(FileName.c_str(), 0, &metadata, image),
						L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
						FileName,
						L"TextureResource::OnCreateWithParam()"
					);
				}
			}
			else if (TexType == L"DDS") {
				ThrowIfFailed(
					DirectX::LoadFromDDSFile(FileName.c_str(), 0, &metadata, image),
					L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
					FileName,
					L"TextureResource::OnCreateWithParam()"
				);
			}
			else if (TexType == L"TGA") {
				ThrowIfFailed(
					DirectX::LoadFromTGAFile(FileName.c_str(), &metadata, image),
					L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
					FileName,
					L"TextureResource::OnCreateWithParam()"
				);
			}
			else {
				ThrowBaseException(
					L"���̃^�C�v�͑Ή����Ă��܂���",
					TexType.c_str(),
					L"TextureResource::OnCreateWithParam()"
				);
			}

			pImpl->CreateTexture(image.GetImages(), image.GetImageCount(), metadata);
			pImpl->CreateTextureUploadHeap();

		}
		catch (...) {
			throw;
		}
	}


	shared_ptr<TextureResource> TextureResource::CreateTextureResource(uint32_t Width, uint32_t Height, const vector<uint8_t>& data) {
		return ObjectFactory::CreateWithParam<TextureResource>(Width, Height, data);
	}

	//���\�[�X
	ComPtr<ID3D12Resource> TextureResource::GetTexture() const {
		return pImpl->m_Texture;

	}
	uint32_t TextureResource::GetTextureWidth() {
		return (uint32_t)pImpl->m_TextureResDesc.Width;

	}
	uint32_t TextureResource::GetTextureHeight() {
		return (uint32_t)pImpl->m_TextureResDesc.Height;
	}
	uint32_t TextureResource::GetPixelSize() {
		return pImpl->m_TexturePixelSize;
	}
	const D3D12_RESOURCE_DESC& TextureResource::GetTextureResDesc() const {
		return pImpl->m_TextureResDesc;
	}


	//�e�N�X�`���f�[�^
	vector<uint8_t>& TextureResource::GetData() const {
		return pImpl->m_Data;
	}
	void TextureResource::SetData(const vector<uint8_t>& data) {
		pImpl->CopyData(data);
	}

	//���\�[�X�̍X�V
	void TextureResource::UpdateResources(const ComPtr<ID3D12GraphicsCommandList>& commandList) {
		if (!pImpl->m_DataRefresh) {
			return;
		}
		D3D12_SUBRESOURCE_DATA textureData = {};
		textureData.pData = &pImpl->m_Data[0];
		textureData.RowPitch = GetTextureWidth() * GetPixelSize();
		textureData.SlicePitch = textureData.RowPitch * GetTextureHeight();
		UpdateSubresources(commandList.Get(),
			GetTexture().Get(),
			pImpl->m_TextureUploadHeap.Get(),
			0, 0, 1, &textureData);
		commandList->ResourceBarrier(
			1,
			&CD3DX12_RESOURCE_BARRIER::Transition(
				GetTexture().Get(),
				D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
			)
		);
		pImpl->m_DataRefresh = false;
	}



	//--------------------------------------------------------------------------------------
	//	struct DeviceResources::Impl;
	//	�p�r: Direct12�f�o�C�X�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct DeviceResources::Impl {

		static const UINT FrameCount = 2;
		//�p�C�v���C��objects.
		D3D12_VIEWPORT m_Viewport;
		D3D12_RECT m_ScissorRect;
		ComPtr<IDXGISwapChain3> m_SwapChain;
		ComPtr<ID3D12Device> m_Device;
		ComPtr<ID3D12Resource> m_RenderTargets[FrameCount];
		ComPtr<ID3D12Resource> m_DepthStencil;

		ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
		ComPtr<ID3D12CommandQueue> m_CommandQueue;
		//RenderTargerView Heap
		ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
		//DepsStensilViewHeap
		ComPtr<ID3D12DescriptorHeap> m_DsvHeap;

		UINT m_RtvDescriptorSize;

		//�N���A�����p�̃I�u�W�F�N�g
		ComPtr<ID3D12RootSignature> m_RootSignature;
		ComPtr<ID3D12PipelineState> m_PipelineState;
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

		//�v���[���g�o���A�p�̃R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_PresentCommandList;
		//�R�}���h���X�g���s�p�̔z��
		vector<ID3D12CommandList*> m_DrawCommandLists;



		//�����I�u�W�F�N�g
		UINT m_FrameIndex;
		HANDLE m_FenceEvent;
		ComPtr<ID3D12Fence> m_Fence;
		UINT64 m_FenceValue;
		float m_dpi;
		float m_aspectRatio;
		//�\�z�Ɣj��
		Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		~Impl();
		//���\�[�X�̍\�z
		void CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		//�A�_�v�^�[�擾
		void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);
		//��������
		void WaitForPreviousFrame(bool ExceptionActive = true);
	};
	//�\�z
	DeviceResources::Impl::Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height) :
		m_dpi(96.0f),
		m_Viewport(),
		m_ScissorRect()
	{
		try {
			//���\�[�X�̍\�z
			CreateDeviceResources(hWnd, isFullScreen, Width, Height);
		}
		catch (...) {
			throw;
		}
	}

	DeviceResources::Impl::~Impl() {
		WaitForPreviousFrame(false);
		CloseHandle(m_FenceEvent);
	}


	//�A�_�v�^�[�擾�w���p�[�֐�
	_Use_decl_annotations_
		void DeviceResources::Impl::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
	{
		ComPtr<IDXGIAdapter1> adapter;
		*ppAdapter = nullptr;

		for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				// If you want a software adapter, pass in "/warp" on the command line.
				continue;
			}

			// Check to see if the adapter supports Direct3D 12, but don't create the
			// actual device yet.
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				break;
			}
		}

		*ppAdapter = adapter.Detach();
	}


	//���\�[�X�̍\�z
	void DeviceResources::Impl::CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height) {

		m_Viewport.Width = static_cast<float>(Width);
		m_Viewport.Height = static_cast<float>(Height);
		m_Viewport.MaxDepth = 1.0f;

		m_ScissorRect.right = static_cast<LONG>(Width);
		m_ScissorRect.bottom = static_cast<LONG>(Height);


		m_aspectRatio = static_cast<float>(Width) / static_cast<float>(Height);

#if defined(_DEBUG)
		//D3D12 debug �L��.
		{
			ComPtr<ID3D12Debug> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();
			}
		}
#endif
		//DXGIFactory�̍쐬
		ComPtr<IDXGIFactory4> factory;
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)),
			L"DXGIFactory�̍쐬�Ɏ��s���܂���",
			L"CreateDXGIFactory1(IID_PPV_ARGS(&factory)",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);

		//�A�_�v�^�̎擾
		{
			//�n�[�h�E�F�A�A�_�v�^�̎擾
			ComPtr<IDXGIAdapter1> hardwareAdapter;
			GetHardwareAdapter(factory.Get(), &hardwareAdapter);
			if (FAILED(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)))) {
				//���s�����烉�b�v�A�_�v�^�̎擾
				ComPtr<IDXGIAdapter> warpAdapter;
				ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)),
					L"���b�v�A�_�v�^�̍쐬�Ɏ��s���܂���",
					L"factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter))",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);

				ThrowIfFailed(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)),
					L"�f�o�C�X�̍쐬�Ɏ��s���܂���",
					L"D3D12CreateDevice(warpAdapter.Get(),D3D_FEATURE_LEVEL_11_0,IID_PPV_ARGS(&m_Device))",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);

			}
		}
		//�R�}���h�L���[�̍쐬.
		{
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			ThrowIfFailed(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue)),
				L"�R�}���h�L���[�̍쐬�Ɏ��s���܂���",
				L"m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue))",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
		}
		//�X���b�v�`�F�[���̍쐬.
		{
			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			swapChainDesc.BufferCount = FrameCount;
			swapChainDesc.BufferDesc.Width = Width;
			swapChainDesc.BufferDesc.Height = Height;
			swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.OutputWindow = hWnd;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.Windowed = TRUE;

			ComPtr<IDXGISwapChain> swapChain;
			ThrowIfFailed(factory->CreateSwapChain(m_CommandQueue.Get(), &swapChainDesc, &swapChain),
				L"�X���b�v�`�F�[���̍쐬�Ɏ��s���܂���",
				L"factory->CreateSwapChain(m_CommandQueue.Get(), &swapChainDesc, &swapChain)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			ThrowIfFailed(swapChain.As(&m_SwapChain),
				L"�X���b�v�`�F�[���̃o�[�W�����ύX�Ɏ��s���܂���",
				L"swapChain.As(&m_SwapChain)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			//�t���[���C���f�b�N�X��ݒ肵�Ă���
			m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();
		}
		//�f�X�N�v���^�q�[�v�̍쐬.
		{
			//�����_�[�^�[�Q�b�g�r���[�̃f�X�N�v���^�q�[�v�쐬
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = FrameCount;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			ThrowIfFailed(m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RtvHeap)),
				L"�����_�[�^�[�Q�b�g�r���[�̃f�X�N�v���^�q�[�v�쐬�Ɏ��s���܂���",
				L"m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RtvHeap)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			// Describe and create a depth stencil view (DSV) descriptor heap.
			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
			dsvHeapDesc.NumDescriptors = 1;
			dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			ThrowIfFailed(m_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DsvHeap)),
				L"�f�v�X�X�e���V���r���[�̃f�X�N�v���^�q�[�v�쐬�Ɏ��s���܂���",
				L"m_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DsvHeap)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			m_RtvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		}
		//RTV�̃t���[�����\�[�X�̍쐬
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RtvHeap->GetCPUDescriptorHandleForHeapStart());
			//FrameCount������RTV�̃t���[�����\�[�X�̍쐬
			for (UINT n = 0; n < FrameCount; n++)
			{
				ThrowIfFailed(m_SwapChain->GetBuffer(n, IID_PPV_ARGS(&m_RenderTargets[n])),
					L"RTV�̃t���[�����\�[�X�̍쐬�Ɏ��s���܂���",
					L"m_SwapChain->GetBuffer(n, IID_PPV_ARGS(&m_RenderTargets[n])",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);
				m_Device->CreateRenderTargetView(m_RenderTargets[n].Get(), nullptr, rtvHandle);
				rtvHandle.Offset(1, m_RtvDescriptorSize);
			}
		}

		//DSV(�f�v�X�X�e���V���r���[)
		{
			D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
			depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
			depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

			D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
			depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
			depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
			depthOptimizedClearValue.DepthStencil.Stencil = 0;

			ThrowIfFailed(m_Device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, Width, Height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
				D3D12_RESOURCE_STATE_DEPTH_WRITE,
				&depthOptimizedClearValue,
				IID_PPV_ARGS(&m_DepthStencil)
			),
				L"�f�v�X�X�e���V�����\�[�X�쐬�Ɏ��s���܂���",
				L"m_Device->CreateCommittedResource)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);

			m_Device->CreateDepthStencilView(m_DepthStencil.Get(), &depthStencilDesc, m_DsvHeap->GetCPUDescriptorHandleForHeapStart());
		}



		ThrowIfFailed(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)),
			L"�R�}���h�A���P�[�^�̍쐬�Ɏ��s���܂���",
			L"m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);

		{
			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			ComPtr<ID3DBlob> signature;
			ComPtr<ID3DBlob> error;

			ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error),
				L"��̃��[�g�V�O�l�`���̃V���A���C�Y�Ɏ��s���܂���",
				L"D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			ThrowIfFailed(m_Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)),
				L"���[�g�V�O�l�`���̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateRootSignature)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
		}



		//��ʃN���A�p�̃R�}���h���X�g
		ThrowIfFailed(
			m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_CommandList)),
			L"�R�}���h���X�g�̍쐬�Ɏ��s���܂���",
			L"m_Device->CreateCommandList()",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);
		ThrowIfFailed(m_CommandList->Close(),
			L"�R�}���h���X�g�̃N���[�Y�Ɏ��s���܂���",
			L"m_CommandList->Close()",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);

		//�v���[���g�p�̃R�}���h���X�g
		ThrowIfFailed(
			m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_PresentCommandList)),
			L"�R�}���h���X�g�̍쐬�Ɏ��s���܂���",
			L"m_Device->CreateCommandList()",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);
		ThrowIfFailed(m_PresentCommandList->Close(),
			L"�R�}���h���X�g�̃N���[�Y�Ɏ��s���܂���",
			L"m_CommandList->Close()",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);
	}

	//��������
	void DeviceResources::Impl::WaitForPreviousFrame(bool ExceptionActive) {
		// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
		// This is code implemented as such for simplicity. More advanced samples 
		// illustrate how to use fences for efficient resource usage.

		// Signal and increment the fence value.
		const UINT64 fence = m_FenceValue;
		if (ExceptionActive) {
			ThrowIfFailed(m_CommandQueue->Signal(m_Fence.Get(), fence),
				L"�R�}���h�L���[�̃V�O�i���Ɏ��s���܂���",
				L"m_CommandQueue->Signal(m_Fence.Get(), fence)",
				L"Dx12DeviceResources::Impl::WaitForPreviousFrame()"
			);
			m_FenceValue++;
			// Wait until the previous frame is finished.
			if (m_Fence->GetCompletedValue() < fence)
			{

				ThrowIfFailed(m_Fence->SetEventOnCompletion(fence, m_FenceEvent),
					L"�C�x���g�̐ݒ�Ɏ��s���܂���",
					L"m_Fence->SetEventOnCompletion(fence, m_FenceEvent)",
					L"Dx12DeviceResources::Impl::WaitForPreviousFrame()"
				);
				WaitForSingleObject(m_FenceEvent, INFINITE);
			}
			m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();
		}
		else {
			//��O�𓊂��Ȃ��B�f�X�g���N�^�p
			m_CommandQueue->Signal(m_Fence.Get(), fence);
			m_FenceValue++;
			if (m_Fence->GetCompletedValue() < fence)
			{
				m_Fence->SetEventOnCompletion(fence, m_FenceEvent);
				WaitForSingleObject(m_FenceEvent, INFINITE);
			}
			m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();
		}
	}

	//--------------------------------------------------------------------------------------
	//	class Dx12DeviceResources : public DeviceResources;
	//--------------------------------------------------------------------------------------

	//�ʏ�`��̃N���A
	void DeviceResources::ClearDefultViews(const Color4& col) {
		ThrowIfFailed(pImpl->m_CommandAllocator->Reset(),
			L"�R�}���h�A���P�[�^�̃��Z�b�g�Ɏ��s���܂���",
			L"m_CommandAllocator->Reset()",
			L"Dx12DeviceResources::ClearDefultViews()"
		);
		ThrowIfFailed(pImpl->m_CommandList->Reset(pImpl->m_CommandAllocator.Get(), pImpl->m_PipelineState.Get()),
			L"�R�}���h���X�g�̃��Z�b�g�Ɏ��s���܂���",
			L"pImpl->m_CommandList->Reset()",
			L"Dx12DeviceResources::ClearDefultViews()"
		);
		pImpl->m_CommandList->SetGraphicsRootSignature(pImpl->m_RootSignature.Get());
		pImpl->m_CommandList->RSSetViewports(1, &GetViewport());
		pImpl->m_CommandList->RSSetScissorRects(1, &GetScissorRect());
		// Indicate that the back buffer will be used as a render target.
		pImpl->m_CommandList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(pImpl->m_RenderTargets[pImpl->m_FrameIndex].Get(),
				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(pImpl->m_RtvHeap->GetCPUDescriptorHandleForHeapStart(), pImpl->m_FrameIndex, pImpl->m_RtvDescriptorSize);
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(pImpl->m_DsvHeap->GetCPUDescriptorHandleForHeapStart());
		pImpl->m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		// Record commands.
		const float clearColor[] = { col.x, col.y, col.z, col.w };
		pImpl->m_CommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		pImpl->m_CommandList->ClearDepthStencilView(pImpl->m_DsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		ThrowIfFailed(pImpl->m_CommandList->Close(),
			L"�R�}���h���X�g�̃N���[�Y�Ɏ��s���܂���",
			L"m_CommandList->Close()",
			L"Dx12DeviceResources::ClearDefultViews()"
		);
		InsertDrawCommandLists(pImpl->m_CommandList.Get());
	}

	void DeviceResources::InsertDrawCommandLists(ID3D12CommandList* Tgt) {
		pImpl->m_DrawCommandLists.push_back(Tgt);
	}

	void DeviceResources::Present(unsigned int SyncInterval, unsigned int  Flags) {

		ThrowIfFailed(pImpl->m_PresentCommandList->Reset(pImpl->m_CommandAllocator.Get(), pImpl->m_PipelineState.Get()),
			L"�R�}���h���X�g�̃��Z�b�g�Ɏ��s���܂���",
			L"pImpl->m_CommandList->Reset()",
			L"Dx12DeviceResources::ClearDefultViews()"
		);
		//�v���[���g�p�̃o���A�𒣂�
		pImpl->m_PresentCommandList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(pImpl->m_RenderTargets[pImpl->m_FrameIndex].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
		ThrowIfFailed(pImpl->m_PresentCommandList->Close(),
			L"�R�}���h���X�g�̃N���[�Y�Ɏ��s���܂���",
			L"m_CommandList->Close()",
			L"Dx12DeviceResources::ClearDefultViews()"
		);
		InsertDrawCommandLists(pImpl->m_PresentCommandList.Get());
		// Execute the command list.
		pImpl->m_CommandQueue->ExecuteCommandLists(pImpl->m_DrawCommandLists.size(), &pImpl->m_DrawCommandLists[0]);
		// Present the frame.
		ThrowIfFailed(pImpl->m_SwapChain->Present(SyncInterval, Flags),
			L"�X���b�v�`�F�[���̃v���[���g�Ɏ��s���܂���",
			L"pImpl->m_SwapChain->Present(1, 0)",
			L"Dx12DeviceResources::OnDraw()"
		);
		pImpl->WaitForPreviousFrame();
		pImpl->m_DrawCommandLists.clear();
	}

	void DeviceResources::ExecuteCommandLists() {
		// Execute the command list.
		if (pImpl->m_DrawCommandLists.size() > 0) {
			pImpl->m_CommandQueue->ExecuteCommandLists(pImpl->m_DrawCommandLists.size(), &pImpl->m_DrawCommandLists[0]);
			pImpl->m_DrawCommandLists.clear();
		}
	}

	void DeviceResources::WaitForPreviousFrame() {
		pImpl->WaitForPreviousFrame();
	}


	DeviceResources::DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height) :
		pImpl(new Impl(hWnd, isFullScreen, Width, Height))
	{}

	DeviceResources::~DeviceResources() {
	}

	//�R���e���c�̍쐬��̏���
	void DeviceResources::AfterInitContents() {
		//�����I�u�W�F�N�g�̍쐬�Ɠ�������
		{

			ThrowIfFailed(pImpl->m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pImpl->m_Fence)),
				L"�t�F���X�̍쐬�Ɏ��s���܂���",
				L"m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence))",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			pImpl->m_FenceValue = 1;

			//�t���[�������̂��߂̃C�x���g�n���h���̍쐬
			pImpl->m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (pImpl->m_FenceEvent == nullptr)
			{
				ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()),
					L"���X�g�G���[�̎擾�Ɏ��s���܂���",
					L"HRESULT_FROM_WIN32(GetLastError())",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);
			}
			pImpl->WaitForPreviousFrame();
		}
	}


	UINT DeviceResources::GetFrameCount() const {
		return pImpl->FrameCount;
	}


	ComPtr<IDXGISwapChain3> DeviceResources::GetSwapChain() const {
		return pImpl->m_SwapChain;

	}
	ComPtr<ID3D12Device> DeviceResources::GetDevice() const {
		return pImpl->m_Device;
	}
	ComPtr<ID3D12Resource>* DeviceResources::GetRenderTargets() const {
		return pImpl->m_RenderTargets;

	}
	ComPtr<ID3D12Resource> DeviceResources::GetRenderTarget(UINT Index) const {
		if (Index >= pImpl->FrameCount) {
			ThrowBaseException(
				L"�C���f�b�N�X������𒴂��Ă��܂�",
				L"if (Index >= pImpl->FrameCount)",
				L"Dx12DeviceResources::GetRenderTarget()"
			);
		}
		return pImpl->m_RenderTargets[Index];

	}
	ComPtr<ID3D12CommandAllocator> DeviceResources::GetCommandAllocator() const {
		return pImpl->m_CommandAllocator;
	}
	ComPtr<ID3D12CommandQueue> DeviceResources::GetCommandQueue() const {
		return pImpl->m_CommandQueue;

	}
	ComPtr<ID3D12DescriptorHeap> DeviceResources::GetRtvHeap() const {
		return pImpl->m_RtvHeap;
	}
	ComPtr<ID3D12DescriptorHeap> DeviceResources::GetDsvHeap() const {
		return pImpl->m_DsvHeap;
	}


	UINT DeviceResources::GetRtvDescriptorSize() const {
		return pImpl->m_RtvDescriptorSize;
	}
	const D3D12_VIEWPORT& DeviceResources::GetViewport() const {
		return pImpl->m_Viewport;
	}
	const D3D12_RECT& DeviceResources::GetScissorRect() const {
		return pImpl->m_ScissorRect;
	}

	UINT DeviceResources::GetFrameIndex() const {
		return pImpl->m_FrameIndex;

	}


	//--------------------------------------------------------------------------------------
	//	struct ShaderResource::Impl;
	//	�p�r: ShaderResource�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct ShaderResource::Impl {
		//�e�N�X�`��
		weak_ptr<TextureResource> m_TextureResource;
		shared_ptr<TextureResource> TextureResourceExpiredLock() {
			auto shptr = m_TextureResource.lock();
			if (!shptr) {
				ThrowBaseException(
					L"�e�N�X�`���������ł�",
					L"if (!shptr)",
					L"ShaderResource::Impl::TextureResourceExpiredLock()"
				);
			}
			return shptr;
		}

	};

	//--------------------------------------------------------------------------------------
	//	class ShaderResource;
	//--------------------------------------------------------------------------------------
	ShaderResource::ShaderResource() :
		pImpl(new Impl())
	{}
	ShaderResource::~ShaderResource() {}
	void ShaderResource::SetTextureResource(const shared_ptr<TextureResource>& textureResorce, const D3D12_CPU_DESCRIPTOR_HANDLE& hadle) {
		pImpl->m_TextureResource = textureResorce;
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		//�e�N�X�`���̃V�F�[�_���\�[�X�r���[���쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//�t�H�[�}�b�g�Œ�
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		Dev->GetDevice()->CreateShaderResourceView(
			textureResorce->GetTexture().Get(),
			&srvDesc,
			hadle);
	}
	void ShaderResource::UpdateResources(const ComPtr<ID3D12GraphicsCommandList>& commandList) {
		pImpl->TextureResourceExpiredLock()->UpdateResources(commandList);
	}

	//--------------------------------------------------------------------------------------
	//	struct VSPSDrawContext::Impl;
	//	�p�r: VSPSDrawContext�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct VSPSDrawContext::Impl {
		//�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		void CbvSrvUavDescriptorHeapChk() {
			if (!m_CbvSrvUavDescriptorHeap.Get()) {
				ThrowBaseException(
					L"CbvSrvUav�̃f�X�N�v���^�q�[�v�������ł�",
					L"if (!m_CbvSrvUavDescriptorHeap.Get())",
					L"VSPSDrawContext::Impl::CbvSrvUavDescriptorHeapChk()"
				);
			}
		}
		UINT m_SrvDescriptorHeapCount{ 0 };
		UINT m_CbxDescriptorHeapCount{ 0 };

		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		void SamplerDescriptorHeapChk() {
			if (!m_SamplerDescriptorHeap.Get()) {
				ThrowBaseException(
					L"�T���v���[�̃f�X�N�v���^�q�[�v�������ł�",
					L"if (!m_SamplerDescriptorHeap.Get())",
					L"VSPSDrawContext::Impl::SamplerDescriptorHeapChk()"
				);
			}
		}
		UINT m_SamplerDescriptorHeapCount{ 0 };
		//���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		//�f�X�N�v���^�q�[�v�̈�̒�`�̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		//�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_PipelineState;
		//�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		//�e�N�X�`��
		weak_ptr<TextureResource> m_TextureResource;
		shared_ptr<TextureResource> TextureResourceExpiredLock() {
			auto shptr = m_TextureResource.lock();
			if (!shptr) {
				ThrowBaseException(
					L"�e�N�X�`���������ł�",
					L"if (!shptr)",
					L"VSPSDrawContext::Impl::TextureResourceExpiredLock()"
				);
			}
			return shptr;
		}
		//�R���X�^���g�o�b�t�@
		void* m_pConstantBuffer{ nullptr };
		//�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
	};

	//--------------------------------------------------------------------------------------
	//	class VSPSDrawContext : public ObjectInterface;
	//--------------------------------------------------------------------------------------
	VSPSDrawContext::VSPSDrawContext(CreateParam param) :
		pImpl(new Impl()),
		m_CreateParam(param)
	{}
	VSPSDrawContext::~VSPSDrawContext() {}

	void VSPSDrawContext::OnCreate() {
		switch (m_CreateParam) {
		case CreateParam::CreateSimple:
			//�V���v���ȃI�u�W�F�N�g�̍\�z
			CreateSimple();
			break;
		case CreateParam::CreateSrvSmp:
			//�V�F�[�_���\�[�X�ƃT���v���[�����I�u�W�F�N�g�̍\�z
			CreateSrvSmp();
			break;
		case CreateParam::CreateCbv:
			//�R���X�^���g�o�b�t�@�̂ݎ��I�u�W�F�N�g�̍\�z
			CreateCbv();
			break;
		case CreateParam::CreateSrvSmpCbv:
			//�V�F�[�_���\�[�X�ƃT���v���[�ƃR���X�^���g�o�b�t�@�����I�u�W�F�N�g�̍\�z
			CreateSrvSmpCbv();
			break;
		}
	}


	void VSPSDrawContext::CreateSrvSmpCbv() {
		auto Dev = App::GetApp()->GetDeviceResources();
		pImpl->m_CbvSrvDescriptorHandleIncrementSize
			= Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CbvSrv�f�X�N�v���^�q�[�v
		pImpl->m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
		//�T���v���[�f�X�N�v���^�q�[�v
		pImpl->m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
		//���[�g�V�O�l�`��
		pImpl->m_RootSignature = RootSignature::CreateSrvSmpCbv();
		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
		pImpl->m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			pImpl->m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			1,
			0
		);
		pImpl->m_GPUDescriptorHandleVec.push_back(SrvHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			pImpl->m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			1,
			0
		);
		pImpl->m_GPUDescriptorHandleVec.push_back(SamplerHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			pImpl->m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			1,
			pImpl->m_CbvSrvDescriptorHandleIncrementSize
		);
		pImpl->m_GPUDescriptorHandleVec.push_back(CbvHandle);
		//�V�F�[�_���\�[�X�̐���ݒ�
		pImpl->m_SrvDescriptorHeapCount = 1;
		pImpl->m_CbxDescriptorHeapCount = 1;
		pImpl->m_SamplerDescriptorHeapCount = 1;
		//�T���v���[�̃f�t�H���g�ݒ�
		SetSamplerState(SamplerState::LinearClamp);
	}

	//�R���X�^���g�o�b�t�@�̂ݎ��I�u�W�F�N�g�̍\�z
	void VSPSDrawContext::CreateCbv() {
		auto Dev = App::GetApp()->GetDeviceResources();
		pImpl->m_CbvSrvDescriptorHandleIncrementSize
			= Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CbvSrv�f�X�N�v���^�q�[�v(�R���X�^���g�o�b�t�@�̂�)
		pImpl->m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1);
		//���[�g�V�O�l�`��
		pImpl->m_RootSignature = RootSignature::CreateCbv();
		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
		pImpl->m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			pImpl->m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			1,
			0
		);
		pImpl->m_GPUDescriptorHandleVec.push_back(CbvHandle);
		//�V�F�[�_���\�[�X�̐���ݒ�
		pImpl->m_SrvDescriptorHeapCount = 0;
		pImpl->m_CbxDescriptorHeapCount = 1;
		pImpl->m_SamplerDescriptorHeapCount = 0;
	}

	//�V���v���ȃI�u�W�F�N�g�̍\�z
	void VSPSDrawContext::CreateSimple() {
		//�V�F�[�_���\�[�X�̐���ݒ�
		pImpl->m_SrvDescriptorHeapCount = 0;
		pImpl->m_CbxDescriptorHeapCount = 0;
		pImpl->m_SamplerDescriptorHeapCount = 0;
		pImpl->m_RootSignature = RootSignature::CreateSimple();
	}

	//�V�F�[�_���\�[�X�ƃT���v���[�����I�u�W�F�N�g�̍\�z
	void VSPSDrawContext::CreateSrvSmp() {
		auto Dev = App::GetApp()->GetDeviceResources();
		pImpl->m_CbvSrvDescriptorHandleIncrementSize
			= Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CbvSrv�f�X�N�v���^�q�[�v
		pImpl->m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1);
		//�T���v���[�f�X�N�v���^�q�[�v
		pImpl->m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
		//���[�g�V�O�l�`��
		pImpl->m_RootSignature = RootSignature::CreateSrvSmp();
		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
		pImpl->m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			pImpl->m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			1,
			0
		);
		pImpl->m_GPUDescriptorHandleVec.push_back(SrvHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			pImpl->m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			1,
			0
		);
		pImpl->m_GPUDescriptorHandleVec.push_back(SamplerHandle);
		//�V�F�[�_���\�[�X�̐���ݒ�
		pImpl->m_SrvDescriptorHeapCount = 1;
		pImpl->m_CbxDescriptorHeapCount = 0;
		pImpl->m_SamplerDescriptorHeapCount = 1;
		//�T���v���[�̃f�t�H���g�ݒ�
		SetSamplerState(SamplerState::LinearClamp);
	}





	void VSPSDrawContext::SetSamplerState(const SamplerState State) {
		pImpl->SamplerDescriptorHeapChk();
		DynamicSampler::CreateSampler(State, pImpl->m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}

	void VSPSDrawContext::SetBlendState(const D3D12_BLEND_DESC& State) {
		if (!m_PipelineState.Get()) {
			ThrowBaseException(
				L"�p�C�v���C���X�e�[�g���܂��쐬����Ă��܂���",
				L"if (!m_PipelineState.Get())",
				L"VSPSDrawContext::SetBlendState()"
			);
		}
		m_PineLineDesc.BlendState = State;
		m_PipelineState = PipelineState::CreateDirect(m_PineLineDesc);
	}
	void VSPSDrawContext::SetBlendState(const BlendState Mode) {
		if (!m_PipelineState.Get()) {
			ThrowBaseException(
				L"�p�C�v���C���X�e�[�g���܂��쐬����Ă��܂���",
				L"if (!m_PipelineState.Get())",
				L"VSPSDrawContext::SetBlendState()"
			);
		}
		D3D12_BLEND_DESC blend_desc;
		D3D12_RENDER_TARGET_BLEND_DESC Target;
		switch (Mode) {
		case BlendState::Opaque:
			blend_desc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			break;
		case BlendState::AlphaBlend:
			ZeroMemory(&blend_desc, sizeof(blend_desc));
			blend_desc.AlphaToCoverageEnable = false;
			blend_desc.IndependentBlendEnable = false;
			ZeroMemory(&Target, sizeof(Target));
			Target.BlendEnable = true;
			Target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			Target.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			Target.BlendOp = D3D12_BLEND_OP_ADD;
			Target.SrcBlendAlpha = D3D12_BLEND_ONE;
			Target.DestBlendAlpha = D3D12_BLEND_ZERO;
			Target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
			Target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
				blend_desc.RenderTarget[i] = Target;
			}
			break;
		case BlendState::Additive:
			ZeroMemory(&blend_desc, sizeof(blend_desc));
			blend_desc.AlphaToCoverageEnable = false;
			blend_desc.IndependentBlendEnable = false;
			ZeroMemory(&Target, sizeof(Target));
			Target.BlendEnable = true;
			Target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			Target.DestBlend = D3D12_BLEND_ONE;
			Target.BlendOp = D3D12_BLEND_OP_ADD;
			Target.SrcBlendAlpha = D3D12_BLEND_ONE;
			Target.DestBlendAlpha = D3D12_BLEND_ZERO;
			Target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
			Target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
				blend_desc.RenderTarget[i] = Target;
			}
			break;
		case BlendState::NonPremultiplied:
			ZeroMemory(&blend_desc, sizeof(blend_desc));
			blend_desc.AlphaToCoverageEnable = false;
			blend_desc.IndependentBlendEnable = false;
			ZeroMemory(&Target, sizeof(Target));
			Target.BlendEnable = true;
			Target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			Target.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			Target.BlendOp = D3D12_BLEND_OP_ADD;
			Target.SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
			Target.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
			Target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
			Target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
				blend_desc.RenderTarget[i] = Target;
			}
			break;
		default:
			blend_desc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			break;
		}
		m_PineLineDesc.BlendState = blend_desc;
		m_PipelineState = PipelineState::CreateDirect(m_PineLineDesc);
	}

	void VSPSDrawContext::SetRasterizerState(const RasterizerState Mode) {
		if (!m_PipelineState.Get()) {
			ThrowBaseException(
				L"�p�C�v���C���X�e�[�g���܂��쐬����Ă��܂���",
				L"if (!m_PipelineState.Get())",
				L"VSPSDrawContext::SetRasterizerState()"
			);
		}
		D3D12_FILL_MODE FillMode;
		D3D12_CULL_MODE CullMode;
		switch (Mode) {
		case RasterizerState::CullBack:
			FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
			break;
		case RasterizerState::CullFront:
			FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
			break;
		case RasterizerState::CullNone:
			FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
			break;
		case RasterizerState::Wireframe:
			FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME;
			CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
			break;
		}
		m_PineLineDesc.RasterizerState.FillMode = FillMode;
		m_PineLineDesc.RasterizerState.CullMode = CullMode;
		m_PipelineState = PipelineState::CreateDirect(m_PineLineDesc);
	}

	void VSPSDrawContext::SetRasterizerState(const D3D12_RASTERIZER_DESC& State) {
		if (!m_PipelineState.Get()) {
			ThrowBaseException(
				L"�p�C�v���C���X�e�[�g���܂��쐬����Ă��܂���",
				L"if (!m_PipelineState.Get())",
				L"VSPSDrawContext::SetRasterizerState()"
			);
		}
		m_PineLineDesc.RasterizerState = State;
		m_PipelineState = PipelineState::CreateDirect(m_PineLineDesc);
	}

	void VSPSDrawContext::SetDepthStencilState(const DepthStencilState Mode) {
		if (!m_PipelineState.Get()) {
			ThrowBaseException(
				L"�p�C�v���C���X�e�[�g���܂��쐬����Ă��܂���",
				L"if (!m_PipelineState.Get())",
				L"VSPSDrawContext::SetDepthStencilState()"
			);
		}
		D3D12_DEPTH_STENCIL_DESC State;
		ZeroMemory(&State, sizeof(State));
		switch (Mode) {
		case DepthStencilState::Default:
			State = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			break;
		case DepthStencilState::None:
			State.DepthEnable = false;
			State.StencilEnable = false;
			break;
		case DepthStencilState::Read:
			//Read�͖�����
			State = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			break;
		}
		m_PineLineDesc.DepthStencilState = State;
		m_PipelineState = PipelineState::CreateDirect(m_PineLineDesc);
	}
	void VSPSDrawContext::SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& State) {
		if (!m_PipelineState.Get()) {
			ThrowBaseException(
				L"�p�C�v���C���X�e�[�g���܂��쐬����Ă��܂���",
				L"if (!m_PipelineState.Get())",
				L"VSPSDrawContext::SetDepthStencilState()"
			);
		}
		m_PineLineDesc.DepthStencilState = State;
		m_PipelineState = PipelineState::CreateDirect(m_PineLineDesc);
	}

	void VSPSDrawContext::SetTextureResource(const shared_ptr<TextureResource>& textureResorce) {
		pImpl->CbvSrvUavDescriptorHeapChk();
		pImpl->m_TextureResource = textureResorce;
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		//�n���h�����쐬
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			pImpl->m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			1,
			0
		);
		//�e�N�X�`���̃V�F�[�_���\�[�X�r���[���쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//�t�H�[�}�b�g
		srvDesc.Format = textureResorce->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = textureResorce->GetTextureResDesc().MipLevels;
		Dev->GetDevice()->CreateShaderResourceView(
			textureResorce->GetTexture().Get(),
			&srvDesc,
			Handle);
	}
	void VSPSDrawContext::CreateConstantBuffer(UINT BufferSize) {
		pImpl->CbvSrvUavDescriptorHeapChk();
		auto Dev = App::GetApp()->GetDeviceResources();
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(BufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&pImpl->m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"VSPSDrawContext::CreateConstantBuffer()"
		);
		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = pImpl->m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		cbvDesc.SizeInBytes = (BufferSize + 255) & ~255;
		//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
		//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			pImpl->m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			pImpl->m_SrvDescriptorHeapCount,
			pImpl->m_CbvSrvDescriptorHandleIncrementSize * pImpl->m_SrvDescriptorHeapCount
		);
		Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
		//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(pImpl->m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&pImpl->m_pConstantBuffer)),
			L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"VSPSDrawContext::CreateConstantBuffer()"
		);
	}
	void VSPSDrawContext::UpdateConstantBuffer(void* SrcBuffer, UINT BufferSize) {
		pImpl->CbvSrvUavDescriptorHeapChk();
		if (!pImpl->m_pConstantBuffer || !pImpl->m_ConstantBufferUploadHeap.Get()) {
			ThrowBaseException(
				L"�R���X�^���g�o�b�t�@���R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v������������Ă��܂���",
				L"if (!pImpl->m_pConstantBuffer || !pImpl->m_ConstantBufferUploadHeap.Get())",
				L"SrvSmpCbvDeviceContext::UpdateConstantBuffer()"
			);
		}
		memcpy(pImpl->m_pConstantBuffer, SrcBuffer, BufferSize);
	}

	//�`��T�u�֐�
	void VSPSDrawContext::DrawIndexedBase(const shared_ptr<MeshResource>& Mesh) {
		m_CommandList->SetGraphicsRootSignature(pImpl->m_RootSignature.Get());
		if (pImpl->m_SamplerDescriptorHeap.Get()) {
			ID3D12DescriptorHeap* ppHeaps[] = { pImpl->m_CbvSrvUavDescriptorHeap.Get(), pImpl->m_SamplerDescriptorHeap.Get() };
			m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		}
		else {
			ID3D12DescriptorHeap* ppHeaps[] = { pImpl->m_CbvSrvUavDescriptorHeap.Get() };
			m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		}
		for (size_t i = 0; i < pImpl->m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, pImpl->m_GPUDescriptorHandleVec[i]);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			Dev->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
			Dev->GetFrameIndex(),
			Dev->GetRtvDescriptorSize());
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
			Dev->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
		);
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetIndexBuffer(&Mesh->GetIndexBufferView());
		m_CommandList->IASetVertexBuffers(0, 1, &Mesh->GetVertexBufferView());
		m_CommandList->DrawIndexedInstanced(Mesh->GetNumIndicis(), 1, 0, 0, 0);

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());

	}

	void VSPSDrawContext::DrawVertexBase(const shared_ptr<MeshResource>& Mesh) {
		m_CommandList->SetGraphicsRootSignature(pImpl->m_RootSignature.Get());
		if (pImpl->m_SamplerDescriptorHeap.Get()) {
			ID3D12DescriptorHeap* ppHeaps[] = { pImpl->m_CbvSrvUavDescriptorHeap.Get(), pImpl->m_SamplerDescriptorHeap.Get() };
			m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		}
		else {
			if (pImpl->m_CbvSrvUavDescriptorHeap.Get()) {
				ID3D12DescriptorHeap* ppHeaps[] = { pImpl->m_CbvSrvUavDescriptorHeap.Get() };
				m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
			}
		}
		for (size_t i = 0; i < pImpl->m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, pImpl->m_GPUDescriptorHandleVec[i]);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			Dev->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
			Dev->GetFrameIndex(),
			Dev->GetRtvDescriptorSize());
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
			Dev->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
		);
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetVertexBuffers(0, 1, &Mesh->GetVertexBufferView());
		m_CommandList->DrawInstanced(Mesh->GetNumVertices(), 1, 0, 0);

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());

	}

	void VSPSDrawContext::ExecuteCommandLists() {
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ExecuteCommandLists();
	}



	//�p�C�v���C���̃��Z�b�g
	void VSPSDrawContext::ResetPipeLine() {
		//�R�}���h���X�g�̃��Z�b�g
		CommandList::Reset(m_PipelineState, m_CommandList);

	}
	//�V�F�[�_���\�[�X�̍X�V
	void VSPSDrawContext::UpdateShaderResource() {
		if (!pImpl->m_TextureResource.expired()) {
			auto Res = pImpl->TextureResourceExpiredLock();
			Res->UpdateResources(m_CommandList);
		}
	}

	ComPtr<ID3D12RootSignature> VSPSDrawContext::GetRootSignature() const {
		return pImpl->m_RootSignature;
	}


	namespace Dx12ShaderHelper {
		//--------------------------------------------------------------------------------------
		//�@void CreateShaderFlomCso(
		//		const wstring& fileName,	//cso�t�@�C��
		//		ID3DBlob** pResult	//Blob
		//	);
		//	�p�r: CSO�f�[�^����V�F�[�_�p��Blob���쐬����
		//--------------------------------------------------------------------------------------
		void CreateShaderFlomCso(const wstring& fileName, ID3DBlob** pResult) {
			try {
				if (fileName == L"") {
					ThrowBaseException(
						L"�t�@�C�����w�肳��Ă��܂���",
						L"if(fileName == L\"\")",
						L"Dx12ShaderResource::Impl::CreateShaderFlomCso()"
					);
				}
				DWORD RetCode;
				RetCode = GetFileAttributes(fileName.c_str());
				if (RetCode == -1) {
					ThrowBaseException(
						L"�t�@�C�������݂��܂���",
						fileName,
						L"Dx12ShaderResource::Impl::CreateShaderFlomCso()"
					);
				}
				ThrowIfFailed(
					D3DReadFileToBlob(fileName.c_str(), pResult),
					L"Blpb�̍쐬�Ɏ��s���܂����B",
					fileName,
					L"Dx12ShaderResource::Impl::CreateShaderFlomCso()"
				);

			}
			catch (...) {
				throw;
			}
		}
	}

	//end namespace Dx12ShaderHelper;



	//--------------------------------------------------------------------------------------
	//	class Dx12ShaderResource;
	//	�p�r: �V�F�[�_�֘A���\�[�X�̃C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	Dx12ShaderResource::Dx12ShaderResource() {}
	Dx12ShaderResource::~Dx12ShaderResource() {}


	//�V�F�[�_�A�N�Z�b�T
	ID3DBlob* Dx12ShaderResource::GetShaderBlob(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr) {
		//�~���[�e�b�N�X
		std::mutex Mutex;
		//�����_�����p
		return Util::DemandCreate(ShaderComPtr, Mutex, [&](ID3DBlob** pResult)
		{
			Dx12ShaderHelper::CreateShaderFlomCso(Filename, pResult);
		});
	}

	ComPtr<ID3DBlob>& Dx12ShaderResource::GetShaderBlobComPtr(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr) {
		//�~���[�e�b�N�X
		std::mutex Mutex;
		//�����_�����p
		Util::DemandCreate(ShaderComPtr, Mutex, [&](ID3DBlob** pResult)
		{
			Dx12ShaderHelper::CreateShaderFlomCso(Filename, pResult);
		});
		return ShaderComPtr;
	}


}
//end basecross
