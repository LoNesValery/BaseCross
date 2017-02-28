/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	MySeekSteering�s���N���X
	//--------------------------------------------------------------------------------------
	MySeekSteering::MySeekSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr)
	{}
	MySeekSteering::~MySeekSteering() {}


	float  MySeekSteering::Execute(const Vector3& TargetPos) {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		Vector3 Force = RigidPtr->GetForce();
		Vector3 DesiredVelocity = Vector3EX::Normalize(TargetPos - TransPtr->GetPosition()) * RigidPtr->GetMaxSpeed();
		Vector3 WorkForce = (DesiredVelocity - RigidPtr->GetVelocity()) * GetWeight();
		Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
		RigidPtr->SetForce(Force);

		auto Pos = TransPtr->GetPosition();
		return Vector3EX::Length(Pos - TargetPos);
	}

	float MySeekSteering::Execute(const wstring& TargetKey) {
		auto TargetPtr = GetStage()->GetSharedObject(TargetKey);
		auto TargetPos = TargetPtr->GetComponent<Transform>()->GetPosition();
		return Execute(TargetPos);
	}


	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	�p�r: �ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_StateChangeSize(5.0f)
	{
	}
	SeekObject::~SeekObject() {}

	//������
	void SeekObject::OnCreate() {
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.125f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();

		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"ObjectGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<SeekObject>());
		//�����s��������
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->SetGameObjectGroup(Group);
		//Sphere�̏Փ˔��������
		auto PtrColl = AddComponent<CollisionObb>();
		PtrColl->SetIsHitAction(IsHitAction::Auto);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//��������������
		SetAlphaActive(true);

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< StateMachine<SeekObject> >(GetThis<SeekObject>());
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		m_StateMachine->ChangeState(FarState::Instance());
	}


	//����
	void SeekObject::OnUpdate() {
		//�X�e�[�g�ɂ���ĕς��Ȃ��s�������s
		auto PtrGrav = GetBehavior<Gravity>();
		PtrGrav->Execute();
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->Execute();
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	void SeekObject::OnUpdate2() {
		//�i�s�����������悤�ɂ���
		RotToHead();
	}

	//�i�s�����������悤�ɂ���
	void SeekObject::RotToHead() {
		auto PtrTrans = GetComponent<Transform>();
		auto PtrRigidbody = GetComponent<Rigidbody>();
		//��]�̍X�V
		//Velocity�̒l�ŁA��]��ύX����
		//����Ői�s�����������悤�ɂȂ�
		Vector3 Velocity = PtrRigidbody->GetVelocity();
		if (Velocity.Length() > 0.0f) {
			Vector3 Temp = Velocity;
			Temp.Normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quaternion Qt;
			Qt.RotationRollPitchYaw(0, ToAngle, 0);
			Qt.Normalize();
			//���݂̉�]���擾
			Quaternion NowQt = PtrTrans->GetQuaternion();
			//���݂ƖڕW���ԁi10����1�j
			NowQt.Slerp(NowQt, Qt, 0.1f);
			PtrTrans->SetQuaternion(NowQt);
		}

	}

	//--------------------------------------------------------------------------------------
	//	class FarState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<FarState> FarState::Instance() {
		static shared_ptr<FarState> instance(new FarState);
		return instance;
	}
	void FarState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj) {
		auto PtrSeek = Obj->GetBehavior<MySeekSteering>();
		float f = PtrSeek->Execute(L"Player");
		if (f < Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void FarState::Exit(const shared_ptr<SeekObject>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	class NearState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<NearState> NearState::Instance() {
		static shared_ptr<NearState> instance(new NearState);
		return instance;
	}
	void NearState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveSteering>();
		if (PtrArrive->Execute(L"Player") >= Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj) {
	}



}
//end basecross
