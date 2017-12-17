/*!
@file PhysicsManager.h
@brief �����v�Z�p�}�l�[�W���Ȃ�
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	struct SimpleDrawObject;
	class SimplePCStaticRenderer;

	//--------------------------------------------------------------------------------------
	///	�����v�Z�}�l�[�W���i�Q�[���I�u�W�F�N�g�j
	//--------------------------------------------------------------------------------------
	class PhysicsManager : public GameObject, public BasePhysics {
		shared_ptr<MeshResource> m_BoxMeshRes;
		shared_ptr<MeshResource> m_SphereMeshRes;
		//�`��f�[�^�̔z��
		vector<shared_ptr<SimpleDrawObject>> m_PtrObjVec;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<SimplePCStaticRenderer> m_Renderer;
		void DrawShapeWireFrame(uint16_t index,const shared_ptr<MeshResource>& res, const bsm::Mat4x4& world);
		void CreateDrawObject();
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
		@brief	������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�P�̂̃{�b�N�X�̒ǉ�
		@param[in]	param	�쐬�̃p�����[�^
		@param[in]	index	�C���f�b�N�X�i�I�u�W�F�N�g���ė��p����ꍇ�j
		@return	�I�u�W�F�N�g�̃|�C���^�i�o�b�N�A�b�v�͂��Ȃ��̂ŌĂяo�����ŕۑ����邱�Ɓj
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsBox> AddSingleBox(const PsBoxParam& param, uint16_t index = UINT16_MAX)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�P�̂̋��̂̒ǉ�
		@param[in]	param	�쐬�̃p�����[�^
		@param[in]	index	�C���f�b�N�X�i�I�u�W�F�N�g���ė��p����ꍇ�j
		@return	�I�u�W�F�N�g�̃|�C���^�i�o�b�N�A�b�v�͂��Ȃ��̂ŌĂяo�����ŕۑ����邱�Ɓj
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsSphere> AddSingleSphere(const PsSphereParam& param, uint16_t index = UINT16_MAX)override;
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

