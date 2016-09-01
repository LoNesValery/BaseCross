/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�l�p�`�X�v���C�g�̍\����
	//--------------------------------------------------------------------------------------
	struct SquareSprite {
		Vector2 m_LocalScale;		///<���[�J���X�P�[�����O
		float m_LocalRot;				///<���[�J����]�p�x
		float m_LocalRotVelocity;		///<���[�J����]���x
		Vector2 m_LocalPos;				///<���[�J���ʒu
		Vector2 m_LocalVelocity;		///<���[�J�����x
		Vector2 m_LocalGravityVelocity;		///<���[�J���������x
		///CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		///GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		///�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };
		///�R���X�^���g�o�b�t�@
		struct SpriteConstantBuffer
		{
			Matrix4X4 World;
			Color4 Emissive;
			SpriteConstantBuffer() {
				memset(this, 0, sizeof(SpriteConstantBuffer));
			};
		};
		///�R���X�^���g�o�b�t�@�f�[�^
		SpriteConstantBuffer m_SpriteConstantBuffer;
		SquareSprite() :
			m_LocalScale(64.0f, 64.0f),
			m_LocalRot(0),
			m_LocalRotVelocity(0),
			m_LocalPos(0, 0),
			m_LocalVelocity(0, 0),
			m_LocalGravityVelocity(0, 0)
		{
		}
		///�f�X�N�v���^�q�[�v�쐬
		void CreateDescriptorHeap();
		///�R���X�^���g�o�b�t�@�쐬
		void CreateConstantBuffer();
		//�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer();
	};

	//--------------------------------------------------------------------------------------
	///	�l�p�`�X�v���C�g�̃O���[�v
	//--------------------------------------------------------------------------------------
	class SquareSpriteGroup : public ObjectInterface, public ShapeInterface {
		//���b�V��
		shared_ptr<MeshResource> m_SquareSpriteMesh;
		vector<SquareSprite> m_SquareSpriteVec;
		float m_TotalTime;
		//�e�I�u�W�F�N�g�̈ʒu���̕ύX
		void UpdateObjects(float ElapsedTime);
		//�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer();
		///���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		//�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_PipelineState;
		///�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		///���[�g�V�O�l�`���쐬
		void CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		void CreateDescriptorHeap();
		///�R���X�^���g�o�b�t�@�쐬
		void CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		void CreatePipelineState();
		///�R�}���h���X�g�쐬
		void CreateCommandList();
		///�`�揈��
		void DrawObject();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		SquareSpriteGroup();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareSpriteGroup();
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
