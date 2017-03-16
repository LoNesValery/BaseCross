/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	PC��
	//--------------------------------------------------------------------------------------
	class PcSphere : public GameObject {
		Vector3 m_StartPos;
	public:
		//�\�z�Ɣj��
		PcSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~PcSphere();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;

	};

	//--------------------------------------------------------------------------------------
	///	PT��
	//--------------------------------------------------------------------------------------
	class PtSphere : public GameObject {
		Vector3 m_StartPos;
	public:
		//�\�z�Ɣj��
		PtSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~PtSphere();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	PCT��
	//--------------------------------------------------------------------------------------
	class PctSphere : public GameObject {
		Vector3 m_StartPos;
	public:
		//�\�z�Ɣj��
		PctSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~PctSphere();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
	};




	//--------------------------------------------------------------------------------------
	///	Pnt��
	//--------------------------------------------------------------------------------------
	class PntSphere : public GameObject {
		Vector3 m_StartPos;
		bool m_TextureUse;
	public:
		//�\�z�Ɣj��
		PntSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos,bool TextureUse);
		virtual ~PntSphere();
		//������
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	///	Pnt�X�y�L�����[��
	//--------------------------------------------------------------------------------------
	class PntSpSphere : public GameObject {
		Vector3 m_StartPos;
		bool m_TextureUse;
	public:
		//�\�z�Ɣj��
		PntSpSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos, bool TextureUse);
		virtual ~PntSpSphere();
		//������
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	Static�L����
	//--------------------------------------------------------------------------------------
	class StaticChara : public GameObject {
		Vector3 m_StartPos;
		bool m_TamgentUse;
	public:
		//�\�z�Ɣj��
		StaticChara(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos, bool TamgentUse);
		virtual ~StaticChara();
		//������
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	Bone�L����
	//--------------------------------------------------------------------------------------
	class BoneChara : public GameObject {
		Vector3 m_StartPos;
		bool m_TamgentUse;
	public:
		//�\�z�Ɣj��
		BoneChara(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos, bool TamgentUse);
		virtual ~BoneChara();
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};



}
//end basecross
