/*!
@file Player.h
@brief �v���C���[
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�v���C���[
	//--------------------------------------------------------------------------------------
	class Player : public GameObject {
		///���b�V��
		shared_ptr<MeshResource> m_SphereMesh;
		///�e�N�X�`�����\�[�X��
		wstring m_TextureResName;
		///�`��f�[�^
		shared_ptr<BcDrawObject> m_PtrObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_Renderer;
		///�V���h�E�}�b�v�p�`��f�[�^
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//�V���h�E�}�b�v�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		//���̓n���h���[
		InputHandler<Player> m_InputHandler;

		//Transform���
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Pos;
		//�����v�Z�p�̃I�u�W�F�N�g
		shared_ptr<PsSphere> m_PhysicsSphere;
		//�i�s�����𓾂�
		Vec3 GetMoveVector() const;
		//��]��ݒ肷��
		void RotToHead(const bsm::Vec3& Velocity, float LerpFact);
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		Player(const shared_ptr<Stage>& StagePtr, const wstring& TextureResName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Player() {}
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief �ʒu�𓾂�
		@return	�ʒu
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 GetPosition() override {
			return m_Pos;
		}
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//��X�V
		virtual void OnUpdate2() override;
		//�V���h�E�}�b�v�`��
		virtual void OnDrawShadowmap() override;
		//�`��
		virtual void OnDraw()override;
		//A�{�^���n���h��
		void OnPushA();
		//X�{�^���n���h��
		void OnPushX();
	};

}
//end basecross
