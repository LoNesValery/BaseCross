/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	Dx12��3D�`��p�̍\����
	//--------------------------------------------------------------------------------------
	struct Dx12DrawContext3D {
		//���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		//�f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		//�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		//GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		//CPU���T���v���[�f�X�N�v���^�̃n���h��
		D3D12_CPU_DESCRIPTOR_HANDLE m_SamplerDescriptorHandle;

		//�p�C�v���C���X�e�[s�ƍ쐬�p��`
		D3D12_GRAPHICS_PIPELINE_STATE_DESC m_PineLineDesc;
		//�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_FrontPipelineState;
		ComPtr<ID3D12PipelineState> m_BackPipelineState;

		//�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

		UINT m_SrvDescriptorHeapCount{ 0 };
		UINT m_CbxDescriptorHeapCount{ 0 };
		UINT m_SamplerDescriptorHeapCount{ 0 };

		//�R���X�^���g�o�b�t�@
		struct StaticConstantBuffer
		{
			Matrix4X4 World;
			Matrix4X4 View;
			Matrix4X4 Projection;
			Vector4 LightDir;
			Color4 Emissive;
			Color4 Diffuse;
			StaticConstantBuffer() {
				memset(this, 0, sizeof(StaticConstantBuffer));
			};
		};
		//�R���X�^���g�o�b�t�@�̃I�u�W�F�N�g���ϐ�
		StaticConstantBuffer m_StaticConstantBuffer;
		//�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		//�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };

		//���b�V��
		shared_ptr<MeshResource> m_SphereMesh;
		//�e�N�X�`�����\�[�X
		shared_ptr<TextureResource> m_TextureResource;
		//�����������邩�ǂ���
		bool m_Trace;
		//������
		void Init();
		void DrawContext();
	};


	//--------------------------------------------------------------------------------------
	///	��
	//--------------------------------------------------------------------------------------
	class SphereObject : public ObjectInterface, public ShapeInterface {
		//�`��p�\����
		Dx12DrawContext3D m_Dx12DrawContext3D;
		UINT m_Division;				///<������
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		Vector3 m_Scale;				///<�X�P�[�����O
		Quaternion m_Qt;			///<��]
		Vector3 m_Pos;				///<�ʒu
		bool m_Trace;					///<�����������邩�ǂ���
		void CreateBuffers();
		void UpdateConstantBuffer();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	Division	������
		@param[in]	TextureFileName	�e�N�X�`���t�@�C����
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	Pos	�ʒu
		*/
		//--------------------------------------------------------------------------------------
		SphereObject(UINT Division, const wstring& TextureFileName, bool Trace, const Vector3& Pos);
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
