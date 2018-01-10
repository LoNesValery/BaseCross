/*!
@file PhysicsObjects.h
@brief �����v�Z�I�u�W�F�N�g
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Scene;
	class Stage;
	class GameObject;


	//--------------------------------------------------------------------------------------
	///	�����v�Z����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedPsBox : public GameObject {
		//���b�V��
		shared_ptr<MeshResource> m_BoxMesh;
		//�����v�Z�p�̃I�u�W�F�N�g
		shared_ptr<PsBox> m_PhysicsBox;
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;

		//�`��f�[�^
		shared_ptr<BcDrawObject> m_PtrObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_Renderer;
		//�V���h�E�}�b�v�p�`��f�[�^
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//�V���h�E�}�b�v�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;


	public:
		//�\�z�Ɣj��
		FixedPsBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~FixedPsBox();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override {}
		//�V���h�E�}�b�v�`��
		virtual void OnDrawShadowmap() override;
		//�`��
		virtual void OnDraw()override;

	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃ{�b�N�X
	//--------------------------------------------------------------------------------------
	class ActivePsBox : public GameObject {
		//���b�V��
		shared_ptr<MeshResource> m_BoxMesh;
		//�����v�Z�p�̃I�u�W�F�N�g
		shared_ptr<PsBox> m_PhysicsBox;
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;

		//�`��f�[�^
		shared_ptr<BcDrawObject> m_PtrObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_Renderer;
		//�V���h�E�}�b�v�p�`��f�[�^
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//�V���h�E�}�b�v�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;

	public:
		//�\�z�Ɣj��
		ActivePsBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsBox();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//�V���h�E�}�b�v�`��
		virtual void OnDrawShadowmap() override;
		//�`��
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȋ���
	//--------------------------------------------------------------------------------------
	class ActivePsSphere : public GameObject {
		//���b�V��
		shared_ptr<MeshResource> m_SphereMesh;
		//�����v�Z�p�̃I�u�W�F�N�g
		shared_ptr<PsSphere> m_PhysicsSphere;
		float m_Scale;
		Quat m_Qt;
		Vec3 m_Position;

		//�`��f�[�^
		shared_ptr<BcDrawObject> m_PtrObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_Renderer;
		//�V���h�E�}�b�v�p�`��f�[�^
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//�V���h�E�}�b�v�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;

	public:
		//�\�z�Ɣj��
		ActivePsSphere(const shared_ptr<Stage>& StagePtr,
			float Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsSphere();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//�V���h�E�}�b�v�`��
		virtual void OnDrawShadowmap() override;
		//�`��
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z���锭�˂��鋅��
	//--------------------------------------------------------------------------------------
	class FirePsSphere : public GameObject {
		//���b�V��
		shared_ptr<MeshResource> m_SphereMesh;
		//�����v�Z�p�̃I�u�W�F�N�g
		shared_ptr<PsSphere> m_PhysicsSphere;
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		//�`��f�[�^
		shared_ptr<BcDrawObject> m_PtrObj;
		//�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_Renderer;
		//�V���h�E�}�b�v�p�`��f�[�^
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//�V���h�E�}�b�v�`��I�u�W�F�N�g(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;

	public:
		//�\�z�Ɣj��
		FirePsSphere(const shared_ptr<Stage>& StagePtr,
			const Vec3& Emitter, const Vec3& Velocity
		);
		virtual ~FirePsSphere();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//���̂����Z�b�g����
		void Reset(const Vec3& Emitter, const Vec3& Velocity);

		//�V���h�E�}�b�v�`��
		virtual void OnDrawShadowmap() override;
		//�`��
		virtual void OnDraw()override;

	};





}
// end basecross
