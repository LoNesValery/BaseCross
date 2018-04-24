/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//class MultiSpark : public MultiParticle;
	//�p�r: �����̃X�p�[�N�N���X
	//--------------------------------------------------------------------------------------
	class MultiSpark : public MultiParticle {
	public:
		//�\�z�Ɣj��
		MultiSpark(shared_ptr<Stage>& StagePtr);
		virtual ~MultiSpark();
		//������
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
	};


	//--------------------------------------------------------------------------------------
	///	���C��
	//--------------------------------------------------------------------------------------
	class ActionLine : public GameObject {
		Vec3 m_StartPos;
		Vec3 m_EndPos;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	StartPos	�J�n�_
		@param[in]	EndtPos�@�@�I���_
		*/
		//--------------------------------------------------------------------------------------
		ActionLine(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& EndtPos);
		virtual ~ActionLine() {}
		//������
		virtual void OnCreate() override;
		//����
		virtual void OnUpdate() override;
		const Vec3& GetStartPos()const {
			return m_StartPos;
		}
		const Vec3& GetEndPos()const {
			return m_EndPos;
		}
		void SetStartPos(const Vec3& StartPos) {
			m_StartPos = StartPos;
		}
		void SetEndPos(const Vec3& EndtPos) {
			m_EndPos = EndtPos;
		}

	};


	//--------------------------------------------------------------------------------------
	//	�^�C�����O����v���[�g
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
	//	�Œ�̃{�b�N�X
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
	//	�G
	//--------------------------------------------------------------------------------------
	class Enemy : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		shared_ptr<MeshResource> m_BallMesh;
		vector<Vec3> m_WorldTriangles;
	public:
		//�\�z�Ɣj��
		Enemy(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~Enemy();
		//������
		virtual void OnCreate() override;
		//����
		vector<Vec3>& WorldTriangles();
		bool IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, bsm::Vec3& HitPoint);
	};

	//--------------------------------------------------------------------------------------
	///	Bone�L����
	//--------------------------------------------------------------------------------------
	class BoneChara : public GameObject {
		Vec3 m_StartPos;
		vector<Vec3> m_TempPosvec;
		shared_ptr<MeshResource> m_TriangleMesh;
	public:
		//�\�z�Ɣj��
		BoneChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~BoneChara();
		//������
		virtual void OnCreate() override;
		//����
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	Bone�O�p�`�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class BoneTriangles : public GameObject {
		weak_ptr<BoneChara> m_BoneChara;
		vector<VertexPositionColor> m_BackupVertices;
		shared_ptr<MeshResource> m_TriangleMesh;
		vector<Vec3> m_TempPosvec;
	public:
		//�\�z�Ɣj��
		BoneTriangles(const shared_ptr<Stage>& StagePtr,const shared_ptr<BoneChara>& boneChara);
		virtual ~BoneTriangles() {}
		//������
		virtual void OnCreate() override;
		//����
		virtual void OnUpdate() override;
		bool IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, bsm::Vec3& HitPoint);
	};


	//--------------------------------------------------------------------------------------
	///	�q�b�g���̎O�p�`�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class HitTriangles : public GameObject {
		shared_ptr<MeshResource> m_TriangleMesh;
		struct DrawTriangle {
			TRIANGLE m_Triangle;
			float m_LastTime;
			DrawTriangle() {}
			DrawTriangle(const TRIANGLE& tri, float lasttime) :
				m_Triangle(tri), m_LastTime(lasttime) {}
		};
		vector<DrawTriangle> m_DrawTriangleVec;
	public:
		//�\�z�Ɣj��
		HitTriangles(const shared_ptr<Stage>& StagePtr);
		virtual ~HitTriangles() {}
		//������
		virtual void OnCreate() override;
		//����
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		void AddHitTriangle(const TRIANGLE& tri);
	};




}
//end basecross
