/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	������
	//--------------------------------------------------------------------------------------
	struct CubeObject {
		Vector3 m_Scale;			///<�X�P�[�����O
		Quaternion m_Quaternion;	///<��]
		Vector3 m_QuaternionRot;	///<��]��
		float m_QuaternionVelocity;	///<��]���x
		Vector3 m_Posision;				///<�ʒu
		Vector3 m_Velocity;			///<���x
		CubeObject() {}
		void Refresh() {
			m_Scale = Vector3(0.1f, 0.1f, 0.1f);
			m_QuaternionRot = Vector3(Util::RandZeroToOne() - 0.5f, 1.0f, Util::RandZeroToOne() - 0.5f);
			m_Quaternion.RotationAxis(m_QuaternionRot, Util::RandZeroToOne());
			m_QuaternionVelocity = (Util::RandZeroToOne() - 0.5f) * 10.0f;
			m_Posision = Vector3(Util::RandZeroToOne() - 0.5f, Util::RandZeroToOne(), Util::RandZeroToOne());
			m_Velocity = Vector3(Util::RandZeroToOne() - 0.5f, Util::RandZeroToOne() - 0.5f, Util::RandZeroToOne() - 0.5f);
			m_Velocity *= 2.0f;
		}
	};


	//--------------------------------------------------------------------------------------
	///	�����̃O���[�v
	//--------------------------------------------------------------------------------------
	class CubeObjectGroup : public ObjectInterface, public ShapeInterface {
		//���b�V��
		shared_ptr<MeshResource> m_CubeMesh;
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		shared_ptr<TextureResource> m_TextureResource;	///<�e�N�X�`�����\�[�X
		void CreateBuffers();	///<�o�b�t�@�̍쐬
		const size_t m_MaxInstance;				///<�C���X�^���X�ő�l
		vector<CubeObject> m_CubeObjectVec;		///<�����̂̃C���X�^���X�`��z��
		ComPtr<ID3D11Buffer> m_MatrixBuffer;	///<�s��p�̒��_�o�b�t�@
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	TextureFileName	�e�N�X�`���t�@�C����
		*/
		//--------------------------------------------------------------------------------------
		CubeObjectGroup(const wstring& TextureFileName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CubeObjectGroup();
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
