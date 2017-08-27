/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	// Diffuse����R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	struct DiffuseSpriteConstantBuffer
	{
		Mat4x4 World;
		Col4 Emissive;
		Col4 Diffuse;
		DiffuseSpriteConstantBuffer() {
			memset(this, 0, sizeof(DiffuseSpriteConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	///	PT�X�v���C�g�`��
	//--------------------------------------------------------------------------------------
	class PTSpriteDraw : public ObjectInterface {
		bool m_Trace;					///<�����������邩�ǂ���
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		shared_ptr<TextureResource> m_TextureResource;	///<�e�N�X�`�����\�[�X
		///���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		///GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
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
		///�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer(DiffuseSpriteConstantBuffer& CBuff);
		///�`�揈��
		void DrawObject(const shared_ptr<MeshResource>& Mesh);
	};

	//--------------------------------------------------------------------------------------
	///	�ǃX�v���C�g
	//--------------------------------------------------------------------------------------
	class WallSprite : public ObjectInterface, public ShapeInterface {
		//PT�X�v���C�g�`��
		shared_ptr<PTSpriteDraw> m_PTSpriteDraw;
		//Diffuse����R���X�^���g�o�b�t�@
		DiffuseSpriteConstantBuffer m_DiffuseSpriteConstantBuffer;
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
		//Diffuse����R���X�^���g�o�b�t�@
		DiffuseSpriteConstantBuffer m_DiffuseSpriteConstantBuffer;
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
