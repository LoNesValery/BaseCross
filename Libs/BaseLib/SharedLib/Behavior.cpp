/*!
@file Behavior.cpp
@brief �s���N���X����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct Behavior::Impl;
	//--------------------------------------------------------------------------------------
	struct Behavior::Impl {
		weak_ptr<GameObject> m_GameObject;
		explicit Impl(const shared_ptr<GameObject>& GameObjectPtr) :
			m_GameObject(GameObjectPtr)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	///	�s���N���X�̐e�N���X
	//--------------------------------------------------------------------------------------
	Behavior::Behavior(const shared_ptr<GameObject>& GameObjectPtr) :
		pImpl(new Impl(GameObjectPtr))
	{}
	Behavior::~Behavior() {}
	shared_ptr<GameObject> Behavior::GetGameObject() const {
		auto shptr = pImpl->m_GameObject.lock();
		if (!shptr) {
			throw BaseException(
				L"GameObject�͗L���ł͂���܂���",
				L"if (!shptr)",
				L"Behavior::GetGameObject()"
			);
		}
		else {
			return shptr;
		}
	}
	shared_ptr<Stage> Behavior::GetStage() const {
		return GetGameObject()->GetStage();
	}


	//--------------------------------------------------------------------------------------
	///	�s�����[�e�B���e�B�N���X
	//--------------------------------------------------------------------------------------
	//�i�s�����������悤�ɂ���
	void UtilBehavior::RotToHead(float LerpFact) {
		if (LerpFact <= 0.0f) {
			//��ԌW����0�ȉ��Ȃ牽�����Ȃ�
			return;
		}
		//��]�̍X�V
		//Velocity�̒l�ŁA��]��ύX����
		//����Ői�s�����������悤�ɂȂ�
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		Vector3 Velocity = PtrTransform->GetVelocity();
		if (Velocity.Length() > 0.0f) {
			Vector3 Temp = Velocity;
			Temp.Normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quaternion Qt;
			Qt.RotationRollPitchYaw(0, ToAngle, 0);
			Qt.Normalize();
			//���݂̉�]���擾
			Quaternion NowQt = PtrTransform->GetQuaternion();
			//���݂ƖڕW����
			//���݂ƖڕW����
			if (LerpFact >= 1.0f) {
				NowQt = Qt;
			}
			else {
				NowQt.Slerp(NowQt, Qt, LerpFact);
			}
			PtrTransform->SetQuaternion(NowQt);
		}
	}

	void UtilBehavior::RotToHead(const Vector3& Velocity, float LerpFact) {
		if (LerpFact <= 0.0f) {
			//��ԌW����0�ȉ��Ȃ牽�����Ȃ�
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		//��]�̍X�V
		if (Velocity.Length() > 0.0f) {
			Vector3 Temp = Velocity;
			Temp.Normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quaternion Qt;
			Qt.RotationRollPitchYaw(0, ToAngle, 0);
			Qt.Normalize();
			//���݂̉�]���擾
			Quaternion NowQt = PtrTransform->GetQuaternion();
			//���݂ƖڕW����
			if (LerpFact >= 1.0f) {
				NowQt = Qt;
			}
			else {
				NowQt.Slerp(NowQt, Qt, LerpFact);
			}
			PtrTransform->SetQuaternion(NowQt);
		}

	}


	//--------------------------------------------------------------------------------------
	///	Jump�s���N���X
	//--------------------------------------------------------------------------------------
	void JumpBehavior::StartJump(const Vector3& FirstVelocity) {
		//�d��
		auto PtrGravity = GetGameObject()->GetComponent<Gravity>();
		//�W�����v�X�^�[�g
		PtrGravity->StartJump(FirstVelocity);
	}

	bool JumpBehavior::Execute() {
		//�d��
		auto PtrGravity = GetGameObject()->GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0) {
			return true;
		}
		return false;
	}





	//--------------------------------------------------------------------------------------
	///	Seek�s���N���X
	//--------------------------------------------------------------------------------------
	void SeekBehavior::Enter(const Vector3& TargetPos) {
		auto PtrSeek = GetGameObject()->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(TargetPos);
	}
	void SeekBehavior::Enter(const wstring& TargetKey) {
		auto TargetPtr = GetStage()->GetSharedObject(TargetKey);
		auto TargetPos = TargetPtr->GetComponent<Transform>()->GetPosition();
		auto PtrSeek = GetGameObject()->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(TargetPos);
	}



	float  SeekBehavior::Execute(const Vector3& TargetPos, bool RotHead, float LerpFact) {
		auto PtrSeek = GetGameObject()->GetComponent<SeekSteering>();
		PtrSeek->SetTargetPosition(TargetPos);
		if (RotHead) {
			auto UtilPtr = GetGameObject()->GetBehavior<UtilBehavior>();
			UtilPtr->RotToHead(LerpFact);
		}
		auto Pos = GetGameObject()->GetComponent<Transform>()->GetPosition();
		return Vector3EX::Length(Pos - TargetPos);
	}

	float SeekBehavior::Execute(const wstring& TargetKey, bool RotHead, float LerpFact) {
		auto TargetPtr = GetStage()->GetSharedObject(TargetKey);
		auto TargetPos = TargetPtr->GetComponent<Transform>()->GetPosition();
		auto PtrSeek = GetGameObject()->GetComponent<SeekSteering>();
		PtrSeek->SetTargetPosition(TargetPos);
		if (RotHead) {
			auto UtilPtr = GetGameObject()->GetBehavior<UtilBehavior>();
			UtilPtr->RotToHead(LerpFact);
		}
		auto Pos = GetGameObject()->GetComponent<Transform>()->GetPosition();
		return Vector3EX::Length(Pos - TargetPos);
	}


	void SeekBehavior::Exit() {
		auto PtrSeek = GetGameObject()->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}


	//--------------------------------------------------------------------------------------
	///	Arrive�s���N���X
	//--------------------------------------------------------------------------------------
	void ArriveBehavior::Enter(const Vector3& TargetPos) {
		auto PtrArrive = GetGameObject()->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(TargetPos);
	}
	void ArriveBehavior::Enter(const wstring& TargetKey) {
		auto TargetPtr = GetStage()->GetSharedObject(TargetKey);
		auto TargetPos = TargetPtr->GetComponent<Transform>()->GetPosition();
		auto PtrArrive = GetGameObject()->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(TargetPos);
	}



	float  ArriveBehavior::Execute(const Vector3& TargetPos, bool RotHead, float LerpFact) {
		auto PtrArrive = GetGameObject()->GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(TargetPos);
		if (RotHead) {
			auto UtilPtr = GetGameObject()->GetBehavior<UtilBehavior>();
			UtilPtr->RotToHead(LerpFact);
		}
		auto Pos = GetGameObject()->GetComponent<Transform>()->GetPosition();
		return Vector3EX::Length(Pos - TargetPos);
	}

	float ArriveBehavior::Execute(const wstring& TargetKey, bool RotHead, float LerpFact) {
		auto TargetPtr = GetStage()->GetSharedObject(TargetKey);
		auto TargetPos = TargetPtr->GetComponent<Transform>()->GetPosition();
		auto PtrArrive = GetGameObject()->GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(TargetPos);
		if (RotHead) {
			auto UtilPtr = GetGameObject()->GetBehavior<UtilBehavior>();
			UtilPtr->RotToHead(LerpFact);
		}
		auto Pos = GetGameObject()->GetComponent<Transform>()->GetPosition();
		return Vector3EX::Length(Pos - TargetPos);
	}


	void ArriveBehavior::Exit() {
		auto PtrArrive = GetGameObject()->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(false);
	}





}
//end basecross
