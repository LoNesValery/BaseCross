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
	//class MultiFire : public MultiParticle;
	//�p�r: �����̉��N���X
	//--------------------------------------------------------------------------------------
	class MultiFire : public MultiParticle {
	public:
		//�\�z�Ɣj��
		MultiFire(shared_ptr<Stage>& StagePtr);
		virtual ~MultiFire();
		//������
		virtual void OnCreate() override;
		void InsertFire(const Vec3& Pos);
	};


	//--------------------------------------------------------------------------------------
	///	�������̃X�v���C�g
	//--------------------------------------------------------------------------------------
	class TraceSprite : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		float m_TotalTime;
		//�o�b�N�A�b�v���_�f�[�^
		vector<VertexPositionColor> m_BackupVertices;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	StartScale	�����X�P�[��
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		TraceSprite(const shared_ptr<Stage>& StagePtr, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TraceSprite();
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
	};


	//--------------------------------------------------------------------------------------
	///	�ǖ͗l�̃X�v���C�g
	//--------------------------------------------------------------------------------------
	class WallSprite : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec2 m_StartPos;
		wstring m_TextureKey;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	TextureKey	�e�N�X�`���L�[
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	StartScale	�����X�P�[��
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		WallSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec2& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~WallSprite();
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
		virtual void OnUpdate()override {}
	};



	//--------------------------------------------------------------------------------------
	///	�X�N���[������X�v���C�g
	//--------------------------------------------------------------------------------------
	class ScrollSprite : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec2 m_StartPos;
		wstring m_TextureKey;
		float m_TotalTime;
		//�o�b�N�A�b�v���_�f�[�^
		vector<VertexPositionTexture> m_BackupVertices;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	TextureKey	�e�N�X�`���L�[
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	StartScale	�����X�P�[��
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		ScrollSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale,const Vec2& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ScrollSprite();
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
	};


	//--------------------------------------------------------------------------------------
	///	����ŉ�]���闧����
	//--------------------------------------------------------------------------------------
	class RollingCube : public GameObject {
		bool m_Trace;
		Vec3 m_StartScale;
		Quat m_StartQt;
		Vec3 m_StartPos;
		float m_TotalTime;
		//�o�b�N�A�b�v���_�f�[�^
		vector<VertexPositionColor> m_BackupVertices;
		//���̃I�u�W�F�N�g��`�悷��J����
		shared_ptr<Camera> m_Camera;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	StartScale	�����X�P�[��
		@param[in]	StartQt	������]
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		RollingCube(const shared_ptr<Stage>& StagePtr,bool Trace,
			const Vec3& StartScale, const Quat& StartQt,const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~RollingCube();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̃I�u�W�F�N�g��`�悷�邽�߂̃J�����𓾂�i�f�t�H���g�̓X�e�[�W�̃r���[����擾�j
		@return	�J������shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		virtual const shared_ptr<Camera>& OnGetDrawCamera() const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
	};

	//--------------------------------------------------------------------------------------
	///	����ŉ�]����Wall������
	//--------------------------------------------------------------------------------------
	class RollingWallCube : public GameObject {
		wstring m_TextureKey;
		bool m_Trace;
		Vec3 m_StartScale;
		Quat m_StartQt;
		Vec3 m_StartPos;
		float m_TotalTime;
		shared_ptr<MeshResource> m_MeshResource;

		//���̃I�u�W�F�N�g��`�悷��J����
		shared_ptr<Camera> m_Camera;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	TextureKey	�e�N�X�`���L�[
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	StartScale	�����X�P�[��
		@param[in]	StartQt	������]
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		RollingWallCube(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec3& StartScale, const Quat& StartQt, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~RollingWallCube();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̃I�u�W�F�N�g��`�悷�邽�߂̃J�����𓾂�i�f�t�H���g�̓X�e�[�W�̃r���[����擾�j
		@return	�J������shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		virtual const shared_ptr<Camera>& OnGetDrawCamera() const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
	};


	//--------------------------------------------------------------------------------------
	///	����������
	//--------------------------------------------------------------------------------------
	class WhiteCube : public GameObject {
		Vec3 m_StartScale;
		Quat m_StartQt;
		Vec3 m_StartPos;
		float m_TotalTime;
		shared_ptr<MeshResource> m_MeshResource;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	StartScale	�����X�P�[��
		@param[in]	StartQt	������]
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		WhiteCube(const shared_ptr<Stage>& StagePtr,
			const Vec3& StartScale, const Quat& StartQt, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~WhiteCube() {}
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
	};




	//--------------------------------------------------------------------------------------
	///	�`�󂪕ς�鋅��
	//--------------------------------------------------------------------------------------
	class TransSphere : public GameObject {
		wstring m_TextureKey;
		bool m_Trace;
		Vec3 m_StartScale;
		Quat m_StartQt;
		Vec3 m_StartPos;
		float m_TotalTime;
		//�o�b�N�A�b�v���_�f�[�^
		vector<VertexPositionColorTexture> m_BackupVertices;
		//�����^�C���p���_�f�[�^�i�ω�������j
		vector<VertexPositionColorTexture> m_RuntimeVertices;
		//���̃I�u�W�F�N�g��`�悷��J����
		shared_ptr<Camera> m_Camera;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	TextureKey	�e�N�X�`���L�[
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	StartScale	�����X�P�[��
		@param[in]	StartQt	������]
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		TransSphere(const shared_ptr<Stage>& StagePtr,const wstring& TextureKey,bool Trace,
			const Vec3& StartScale, const Quat& StartQt, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TransSphere();
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
	};


	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	�p�r: �ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class SeekObject : public GameObject {
		//�X�e�[�g�}�V�[��
		unique_ptr< StateMachine<SeekObject> >  m_StateMachine;
		Vec3 m_StartPos;
		float m_StateChangeSize;
	public:
		//�\�z�Ɣj��
		SeekObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~SeekObject();
		//������
		virtual void OnCreate() override;
		//�A�N�Z�T
		const unique_ptr<StateMachine<SeekObject>>& GetStateMachine() {
			return m_StateMachine;
		}
		float GetStateChangeSize() const {
			return m_StateChangeSize;
		}
		//����
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;
		//�Փˎ�
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
	};

	//--------------------------------------------------------------------------------------
	//	class FarState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	class FarState : public ObjState<SeekObject>
	{
		FarState() {}
	public:
		static shared_ptr<FarState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class NearState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	class NearState : public ObjState<SeekObject>
	{
		NearState() {}
	public:
		static shared_ptr<NearState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	//	�����̃X�N�G�A
	//--------------------------------------------------------------------------------------
	class NumberSquare : public GameObject {
		weak_ptr<SeekObject> m_SeekObject;
		//���̃I�u�W�F�N�g�݂̂Ŏg�p����X�N�G�A���b�V��
		shared_ptr<MeshResource> m_SquareMeshResource;
		//�w�ԍ�
		size_t m_Number;

		Quat Billboard(const Vec3& Line) {
			Vec3 Temp = Line;
			Mat4x4 RotMatrix;
			Vec3 DefUp(0, 1.0f, 0);
			Vec2 TempVec2(Temp.x, Temp.z);
			if (TempVec2.length() < 0.1f) {
				DefUp = Vec3(0, 0, 1.0f);
			}
			Temp.normalize();
			RotMatrix = XMMatrixLookAtLH(Vec3(0, 0, 0), Temp, DefUp);
			RotMatrix.inverse();
			Quat Qt;
			Qt = RotMatrix.quatInMatrix();
			Qt.normalize();
			return Qt;
		}

	public:
		//�\�z�Ɣj��
		NumberSquare(const shared_ptr<Stage>& StagePtr, 
			const shared_ptr<SeekObject>& SeekObjectPtr, size_t Number);
		virtual ~NumberSquare();
		//������
		virtual void OnCreate() override;
		//�ω�
		virtual void OnUpdate() override;
	};





	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	�p�r: �㉺�ړ�����{�b�N�X
	//--------------------------------------------------------------------------------------
	class MoveBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		MoveBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~MoveBox();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	//	class SphereObject : public GameObject;
	//	�p�r: ��Q����
	//--------------------------------------------------------------------------------------
	class SphereObject : public GameObject {
		Vec3 m_StartPos;
	public:
		//�\�z�Ɣj��
		SphereObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~SphereObject();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	//	class UnevenGroundData : public GameObject;
	//	�p�r: �ł��ڂ����̃f�[�^
	//--------------------------------------------------------------------------------------
	class UnevenGroundData : public GameObject {
		vector<TRIANGLE> m_Triangles;
	public:
		//�\�z�Ɣj��
		UnevenGroundData(const shared_ptr<Stage>& StagePtr);
		virtual ~UnevenGroundData();
		//������
		virtual void OnCreate() override;
		//�O�p�`�̔z���Ԃ�
		const vector<TRIANGLE>& GetTriangles() const {
			return m_Triangles;
		}
	};



	//--------------------------------------------------------------------------------------
	//	class UnevenGround : public GameObject;
	//	�p�r: �ł��ڂ���
	//--------------------------------------------------------------------------------------
	class UnevenGround : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		UnevenGround(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position);
		virtual ~UnevenGround();
		//������
		virtual void OnCreate() override;
		//����
	};


}
//end basecross
