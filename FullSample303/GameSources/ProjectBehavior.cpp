/*!
@file ProjectBehavior.cpp
@brief �v���W�F�N��`�̍s���N���X����
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�v���C���[�̍s���N���X
	//--------------------------------------------------------------------------------------
	Vector3 PlayerBehavior::GetMoveVector() const {
		Vector3 Angle(0, 0, 0);
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//���������̃X�s�[�h
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				auto PtrCamera = GetGameObject()->OnGetDrawCamera();
				//�i�s�����̌������v�Z
				Vector3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
				Front.y = 0;
				Front.Normalize();
				//�i�s������������̊p�x���Z�o
				float FrontAngle = atan2(Front.z, Front.x);
				//�R���g���[���̌����v�Z
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				Vector2 MoveVec(MoveX, MoveZ);
				float MoveSize = MoveVec.Length();
				//�R���g���[���̌�������p�x���v�Z
				float CntlAngle = atan2(-MoveX, MoveZ);
				//�g�[�^���̊p�x���Z�o
				float TotalAngle = FrontAngle + CntlAngle;
				//�p�x����x�N�g�����쐬
				Angle = Vector3(cos(TotalAngle), 0, sin(TotalAngle));
				//���K������
				Angle.Normalize();
				//�ړ��T�C�Y��ݒ�B
				Angle *= MoveSize;
				//Y���͕ω������Ȃ�
				Angle.y = 0;
			}
		}
		return Angle;
	}

	void PlayerBehavior::MovePlayer() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vector3 Angle = GetMoveVector();
		//Transform
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		//Rigidbody�����o��
		auto PtrRedit = GetGameObject()->GetComponent<Rigidbody>();
		//���݂̑��x�����o��
		auto Velo = PtrRedit->GetVelocity();
		//�ړI�n���ō����x���|���ċ��߂�
		auto Target = Angle * GetMaxSpeed();
		//�ړI�n�Ɍ��������߂ɗ͂̂�����������v�Z����
		//Force�̓t�H�[�X�ł���
		auto Force = Target - Velo;
		//y��0�ɂ���
		Force.y = 0;
		//�����x�����߂�
		auto Accel = Force / GetMass();
		//�^�[�����Ԃ��|�������̂𑬓x�ɉ��Z����
		Velo += (Accel * ElapsedTime);
		//��������
		Velo.x *= GetDecel();
		Velo.z *= GetDecel();
		//���x��ݒ肷��
		PtrRedit->SetVelocity(Velo);
		//��]�̌v�Z
		if (Angle.Length() > 0.0f) {
			auto UtilPtr = GetGameObject()->GetBehavior<UtilBehavior>();
			UtilPtr->RotToHead(Angle,1.0f);
		}
	}

	void PlayerBehavior::FireAttackBall() {
		auto PtrCamera = GetGameObject()->OnGetDrawCamera();
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		auto Group = GetStage()->GetSharedObjectGroup(L"AttackBall");
		for (auto& v : Group->GetGroupVector()) {
			auto shptr = v.lock();
			if (shptr) {
				auto AttackPtr = dynamic_pointer_cast<AttackBall>(shptr);
				if (AttackPtr && !AttackPtr->IsUpdateActive()) {
					//��]�̌v�Z
					auto RotY = PtrTrans->GetRotation().y;
					auto Angle = Vector3(sin(RotY), 0, cos(RotY));
					Angle.Normalize();
					auto Span = Angle * 0.5f;
					AttackPtr->Weakup(PtrTrans->GetPosition() + Span, Angle * 10.0f);
					return;
				}
			}
		}

	}




}

//end basecross

