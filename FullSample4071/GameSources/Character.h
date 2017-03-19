/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�J�X�^���V���h�E�}�b�v�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class CustomShadowmap : public Shadowmap {
	public:
		explicit CustomShadowmap(const shared_ptr<GameObject>& GameObjectPtr);

		virtual ~CustomShadowmap();
		//����
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	//	�W�I���g���V�F�[�_���g�����Ǝ��̕`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class CustomPNCStaticDraw : public StaticBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit CustomPNCStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CustomPNCStaticDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	//	�Ǝ��̕`��R���|�[�l���g�ŕ`�悷��8�ʑ�
	//--------------------------------------------------------------------------------------
	class CustomDrawOctahedron : public GameObject {
		Vector3 m_StartScale;
		Vector3 m_StartRotation;
		Vector3 m_StartPos;
		//�o�b�N�A�b�v�p�̒��_�z��
		vector<VertexPositionNormalColor> m_BackupVertices;
		//���_�̕ύX
		void UpdateCustomMesh();
		//�o�ߎ���
		float m_TotalTime;
	public:
		//�\�z�Ɣj��
		CustomDrawOctahedron(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos);
		virtual ~CustomDrawOctahedron();
		//�A�N�Z�T

		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};


}
//end basecross
