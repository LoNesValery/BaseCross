/*!
@file PhysicsManager.h
@brief �����v�Z�p�}�l�[�W���Ȃ�
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�����v�Z�}�l�[�W���i�Q�[���I�u�W�F�N�g�j
	//--------------------------------------------------------------------------------------
	class PhysicsManager : public GameObject, public BasePhysics {
		shared_ptr<MeshResource> m_BoxMeshRes;
		shared_ptr<MeshResource> m_SphereMeshRes;
		void DrawShapeWireFrame(const shared_ptr<MeshResource>& res, const bsm::Mat4x4& world);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		explicit PhysicsManager(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsManager();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�������i�f�t�H���g�͉������Ȃ��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�P�̂̃{�b�N�X�̒ǉ�
		@param[in]	param	�쐬�̃p�����[�^
		@return	�I�u�W�F�N�g�̃|�C���^�i�o�b�N�A�b�v�͂��Ȃ��̂ŌĂяo�����ŕۑ����邱�Ɓj
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsBox> AddSingleBox(const PsBoxParam& param, const wstring& indexKey = L"")override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�P�̂̋��̂̒ǉ�
		@param[in]	param	�쐬�̃p�����[�^
		@return	�I�u�W�F�N�g�̃|�C���^�i�o�b�N�A�b�v�͂��Ȃ��̂ŌĂяo�����ŕۑ����邱�Ɓj
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsSphere> AddSingleSphere(const PsSphereParam& param,const wstring& indexKey = L"")override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`��i���C�A�t���[���j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};



}
// end basecross

