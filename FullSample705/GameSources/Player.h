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
	class Player : public GameObject {
		Vec3 m_StartPos;
		//������̕\��
		void DrawStrings();
		//�i�s�����𓾂�
		Vec3 GetMoveVector() const;
		//�X�P�[�����O�i���a�̑傫���j
		float m_Scale;
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		Player(const shared_ptr<Stage>& StagePtr, const Vec3& StatPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Player() {}
		//�A�N�Z�T
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//��X�V
		virtual void OnUpdate2() override;
		//A�{�^���n���h��
		void OnPushA();
		//X�{�^���n���h��
		void OnPushX();
	};

}
//end basecross

