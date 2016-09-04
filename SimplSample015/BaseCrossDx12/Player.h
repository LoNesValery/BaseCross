/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	�v���C���[
	//--------------------------------------------------------------------------------------
	class Player : public GameObject{
		//���b�V��
		shared_ptr<MeshResource> m_Mesh;
		UINT m_Division;				///<������
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		shared_ptr<TextureResource> m_TextureResource;	///<�e�N�X�`�����\�[�X
		Vector3 m_Scale;				///<�X�P�[�����O
		Quaternion m_Qt;			///<��]
		Vector3 m_Pos;				///<�ʒu
		bool m_Trace;					///<�����������邩�ǂ���
		void CreateBuffers();
		void CollisionTest();			///<�Փ˔���e�X�g
		bool OnObjectTest(const SPHERE& Src, const OBB& Dest);	///<sp��obb�ɏ���Ă邩�ǂ���
		void CollisionEscape();			///<�G�X�P�[�v����

		OBB m_OnObb;
		bool m_OnObject;

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
		Player(const shared_ptr<Scene> PtrScene,
			UINT Division, const wstring& TextureFileName, bool Trace, const Vector3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Player();
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
		@brief	�ŏI�X�V�i�Փ˔���Ȃǂ��`�F�b�N����j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnLastUpdate()override;

	};



}
//end basecross

