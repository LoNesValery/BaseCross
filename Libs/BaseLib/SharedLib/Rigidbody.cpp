/*!
@file Rigidbody.h
@brief �����v�Z�R���|�[�l���g����
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct Rigidbody::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct Rigidbody::Impl {
		Vector3 m_Velocity;				//���x
		float m_MaxSpeed;				//�ō����x
		Vector3 m_MinVelocity;			//�Œᑬ�x�iXYZ���w��ł���j
		Vector3 m_AngularVelocity;		//��]���x

		IsHitAction m_IsHitAction;	//�Փ˂�������Rigidbody�̓����`

		float m_Mass;					//���ʁi�L���O�����j
		float m_Reflection;				//�����W��
		Vector3 m_Force;				//���݂̃t�H�[�X�i�X�e�A�����O�n�R���|�[�l���g�ŕύX�����j
		float m_Friction;				//���C�W���iForce�̋t�����ɓ����K��0����-1.0�̊ԁj
		float m_MaxForce;				//�ō��t�H�[�X
		Impl() :
			m_Velocity(0, 0, 0),
			m_MaxSpeed(10.0f),
			m_MinVelocity(0.01f, 0.01f, 0.01f),
			m_AngularVelocity(0, 0, 0),
			m_IsHitAction(IsHitAction::Repel),
			m_Mass(1.0f),
			m_Reflection(1.0f),
			m_Force(0, 0, 0),
			m_Friction(0),
			m_MaxForce(30.0f)
		{}
		~Impl() {}

	};

	//--------------------------------------------------------------------------------------
	//	class Rigidbody : public Component ;
	//	�p�r: �����ʒu�R���g���[��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Rigidbody::Rigidbody(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Rigidbody::~Rigidbody() {}

	const Vector3& Rigidbody::GetVelocity() const { return pImpl->m_Velocity; }
	void Rigidbody::SetVelocity(const Vector3& Velocity) {
		pImpl->m_Velocity = Velocity;
		pImpl->m_Velocity.ClampLength(0, pImpl->m_MaxSpeed);
		if (pImpl->m_Velocity.Length() < pImpl->m_MinVelocity.Length()) {
			pImpl->m_Velocity.Zero();
		}
	}
	void Rigidbody::SetVelocity(float x, float y, float z) {
		SetVelocity(Vector3(x, y, z));
	}

	float Rigidbody::GetMaxSpeed() const { return pImpl->m_MaxSpeed; }
	void Rigidbody::SetMaxSpeed(float f) { pImpl->m_MaxSpeed = f; }

	const Vector3& Rigidbody::GetMinVelocity() const {
		return pImpl->m_MinVelocity;
	}
	void Rigidbody::SetMinVelocity(const Vector3& Velocity) {
		pImpl->m_MinVelocity = Velocity;
	}
	void Rigidbody::SetMinVelocity(float x, float y, float z) {
		SetMinVelocity(Vector3(x, y, z));
	}


	const Vector3& Rigidbody::GetAngularVelocity() const { return pImpl->m_AngularVelocity; }
	void Rigidbody::SetAngularVelocity(const Vector3& AngularVelocity) { pImpl->m_AngularVelocity = AngularVelocity; }
	void Rigidbody::SetAngularVelocity(float x, float y, float z) { pImpl->m_AngularVelocity = Vector3(x, y, z); }

	IsHitAction Rigidbody::GetIsHitAction() const {
		return pImpl->m_IsHitAction;
	}
	void Rigidbody::SetIsHitAction(IsHitAction HitAction) {
		pImpl->m_IsHitAction = HitAction;
	}


	float Rigidbody::GetMass() const { return pImpl->m_Mass; }
	void Rigidbody::SetMass(float f) { pImpl->m_Mass = f; }

	float Rigidbody::GetReflection() const {
		return pImpl->m_Reflection;
	}
	void Rigidbody::SetReflection(float f) {
		pImpl->m_Reflection = f;
	}


	const Vector3& Rigidbody::GetForce() const { return pImpl->m_Force; }
	void Rigidbody::SetForce(const Vector3& Force) { pImpl->m_Force = Force; }
	void Rigidbody::SetForce(float x, float y, float z) { pImpl->m_Force = Vector3(x, y, z); }

	float Rigidbody::GetFriction() const {
		return pImpl->m_Friction;
	}
	void Rigidbody::SetFriction(float f) {
		if (f >= 0) {
			pImpl->m_Friction = 0.0f;
		}
		else if (f <= -1.0f) {
			pImpl->m_Friction = -1.0f;
		}
		else {
			pImpl->m_Friction = f;
		}
	}


	float Rigidbody::GetMaxForce() const { return pImpl->m_MaxForce; }
	void Rigidbody::SetMaxForce(float f) { pImpl->m_MaxForce = f; }


	void Rigidbody::UpdateFromTime(float CalcTime) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		Vector3 Pos = PtrTransform->GetPosition();
		Quaternion Qt = PtrTransform->GetQuaternion();
		//�t�H�[�X���ύX����Ă�����
		if (pImpl->m_Force.Length() > 0) {
			//�t�H�[�X��0�ȏ�Ȃ�
			//���ʂ��v�Z���ĉ��������߂�
			Vector3 Accel = pImpl->m_Force / pImpl->m_Mass;
			pImpl->m_Velocity += Accel * CalcTime;
		}
		if (pImpl->m_Velocity.Length() >= pImpl->m_MaxSpeed) {
			pImpl->m_Velocity.ClampLength(0, pImpl->m_MaxSpeed);
		}
		//�����l�����߂�
		auto  Friction = pImpl->m_Mass * pImpl->m_Friction;
		if (Friction <= -1.0f) {
			Friction = -1.0f;
		}
		else if (Friction >= 0.0f) {
			Friction = 0.0f;
		}
		Vector3 VFriction = pImpl->m_Velocity * Friction;
		pImpl->m_Velocity += VFriction;
		if (abs(pImpl->m_Velocity.x) < pImpl->m_MinVelocity.x) {
			pImpl->m_Velocity.x = 0;
		}
		if (abs(pImpl->m_Velocity.y) < pImpl->m_MinVelocity.y) {
			pImpl->m_Velocity.y = 0;
		}
		if (abs(pImpl->m_Velocity.z) < pImpl->m_MinVelocity.z) {
			pImpl->m_Velocity.z = 0;
		}
		Quaternion QtSpan;
		QtSpan.RotationRollPitchYawFromVector(pImpl->m_AngularVelocity * CalcTime);
		Qt *= QtSpan;
		Qt.Normalize();
		Pos += pImpl->m_Velocity * CalcTime;
		PtrTransform->SetQuaternion(Qt);
		PtrTransform->SetPosition(Pos);
	}





	void Rigidbody::OnUpdate() {
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateFromTime(ElapsedTime);
	}

}
//end basecross
