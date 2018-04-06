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
		//�X�p�[�N���o
		void InsertSpark(const Vec3& Pos);
	};


	//--------------------------------------------------------------------------------------
	///	�^�C�g���X�v���C�g
	//--------------------------------------------------------------------------------------
	class TitleSprite : public GameObject {
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
		TitleSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec2& StartPos);
		//�j��
		virtual ~TitleSprite();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate()override {}
	};



	//--------------------------------------------------------------------------------------
	///	�A�j���X�v���C�g
	//--------------------------------------------------------------------------------------
	class AnimeSprite : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec2 m_StartPos;
		wstring m_TextureKey;
		//�g�[�^������
		float m_TotalTime;
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
		AnimeSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec2& StartPos);
		//�j��
		virtual ~AnimeSprite();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate()override;
	};


	//--------------------------------------------------------------------------------------
	///	�{�b�N�X
	//--------------------------------------------------------------------------------------
	class Box : public GameObject {
		//�X�e�[�g�}�V�[��
		unique_ptr< StateMachine<Box> >  m_StateMachine;
		Vec3 m_StartPos;
		float m_TotalTime;
	public:
		//�\�z�Ɣj��
		Box(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~Box();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//�X�e�[�g�}�V���𓾂�
		const unique_ptr<StateMachine<Box>>& GetStateMachine() {
			return m_StateMachine;
		}
		//�v���C���[�Ƃ̋����𓾂�
		float GetToPlayerLen() const;
		//���x��0�ɂ���
		void Stop();
		//�s�����藈����̍s��
		void GoFromBehavior();
		//�v���C���[��ǂ�������s��
		void SeekPlayerBehavior();
		//�C�e���ˍs��
		void FirShellBehavior(int div, float up);
	};

	//--------------------------------------------------------------------------------------
	//	�ʏ�ړ�
	//--------------------------------------------------------------------------------------
	class BoxDefaultState : public ObjState<Box>
	{
		BoxDefaultState() {}
	public:
		static shared_ptr<BoxDefaultState> Instance();
		virtual void Enter(const shared_ptr<Box>& Obj)override;
		virtual void Execute(const shared_ptr<Box>& Obj)override;
		virtual void Exit(const shared_ptr<Box>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	�ǂ�������ʏ�U���X�e�[�g
	//--------------------------------------------------------------------------------------
	class BoxSimpleAttackState : public ObjState<Box>
	{
		BoxSimpleAttackState() {}
	public:
		static shared_ptr<BoxSimpleAttackState> Instance();
		virtual void Enter(const shared_ptr<Box>& Obj)override;
		virtual void Execute(const shared_ptr<Box>& Obj)override;
		virtual void Exit(const shared_ptr<Box>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	�n�[�h�ȍU���X�e�[�g
	//--------------------------------------------------------------------------------------
	class BoxHradAttackState : public ObjState<Box>
	{
		BoxHradAttackState() {}
	public:
		static shared_ptr<BoxHradAttackState> Instance();
		virtual void Enter(const shared_ptr<Box>& Obj)override;
		virtual void Execute(const shared_ptr<Box>& Obj)override;
		virtual void Exit(const shared_ptr<Box>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	���˂��鋅��
	//--------------------------------------------------------------------------------------
	class ShellSphere : public GameObject {
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		float m_Scale;
	public:
		//�\�z�Ɣj��
		ShellSphere(const shared_ptr<Stage>& StagePtr,
			const Vec3& Emitter, const Vec3& Velocity
		);
		virtual ~ShellSphere();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//��X�V
		virtual void OnUpdate2() override;
		//���̂�����
		void Erase();
		//���̂����Z�b�g����
		void Reset(const Vec3& Emitter, const Vec3& Velocity);
	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedPsBox : public GameObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
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
	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃI�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	class ActivePsObject : public GameObject {
		bool m_Selected;
	protected:
		ActivePsObject(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr), m_Selected(false){}
		virtual ~ActivePsObject() {}
	public:
		void SetSelected(bool b) {
			m_Selected = b;
			auto PtrDraw = AddComponent<BcPNTStaticDraw>();
			if (b) {
				PtrDraw->SetEmissive(Col4(1.0f, 1.0f, 0, 0));

			}
			else {
				PtrDraw->SetEmissive(Col4(0.0f, 0.0f, 0, 0));
			}

		}
		bool IsSelected() {
			return m_Selected;
		}
		//�X�V
		virtual void OnUpdate() override;
	};


	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃ{�b�N�X
	//--------------------------------------------------------------------------------------
	class ActivePsBox : public ActivePsObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
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
	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȋ���
	//--------------------------------------------------------------------------------------
	class ActivePsSphere : public ActivePsObject {
		float m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
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
	};

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃJ�v�Z��
	//--------------------------------------------------------------------------------------
	class ActivePsCapsule : public ActivePsObject {
		//����
		float m_Len;
		//���a
		float m_Diameter;
		Quat m_Qt;
		Vec3 m_Position;
		//���b�V���i�J�v�Z���͊e�I�u�W�F�N�g���ێ��j
		shared_ptr<MeshResource> m_CapsuleMesh;
	public:
		//�\�z�Ɣj��
		ActivePsCapsule(const shared_ptr<Stage>& StagePtr,
			float Len,
			float Diameter,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsCapsule();
		//������
		virtual void OnCreate() override;
	};




}
//end basecross
