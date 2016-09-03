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
		//���b�V��
		shared_ptr<MeshResource> m_CubeMesh;
		//�e�N�X�`��
		shared_ptr<TextureResource> m_WallTex;
		Vector3 m_Scale;
		Vector3 m_Pos;
		Quaternion m_Qt;

		///���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		///GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;

		//�R���X�^���g�o�b�t�@�\����
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
		//�R���X�^���g�o�b�t�@�f�[�^
		StaticConstantBuffer m_ConstantBufferData;
		///�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };
		///�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_PipelineState;
		///�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

		///�e�������֐�
		///���[�g�V�O�l�`���쐬
		void CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		void CreateDescriptorHeap();
		///�T���v���[�쐬
		void CreateSampler();
		///�V�F�[�_�[���\�[�X�r���[�쐬
		void CreateShaderResourceView();
		///�R���X�^���g�o�b�t�@�쐬
		void CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		void CreatePipelineState();
		///�R�}���h���X�g�쐬
		void CreateCommandList();
		//�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer();
		///�`�揈��
		void DrawObject();


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
