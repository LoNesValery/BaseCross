/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	�p�r: �^�C�����O����v���[�g
	//--------------------------------------------------------------------------------------
	class TilingPlate : public GameObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//�\�z�Ɣj��
		TilingPlate(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingPlate();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//�\�z�Ɣj��
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingFixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};


	//--------------------------------------------------------------------------------------
	//	�}�X�L���O�X�v���C�g
	//--------------------------------------------------------------------------------------
	class MaskSprite : public GameObject {
		wstring m_TextureKey;
		shared_ptr<MeshResource> m_SpriteMesh;
	public:
		//�\�z�Ɣj��
		MaskSprite(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureKey
		);
		virtual ~MaskSprite();
		//������
		virtual void OnCreate() override;
		//����
		//�`��
		virtual void OnDraw() override;
	};




}
//end basecross
