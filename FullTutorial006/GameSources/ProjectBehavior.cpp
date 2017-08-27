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
	Vec3 PlayerBehavior::GetMoveVector() const {
		Vec3 Angle(0, 0, 0);
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//���������̃X�s�[�h
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				auto PtrCamera = GetGameObject()->OnGetDrawCamera();
				//�i�s�����̌������v�Z
				Vec3 Front = PtrTransform->GetWorldMatrix().transInMatrix() - PtrCamera->GetEye();
				Front.y = 0;
				Front.normalize();
				//�i�s������������̊p�x���Z�o
				float FrontAngle = atan2(Front.z, Front.x);
				//�R���g���[���̌����v�Z
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				Vec2 MoveVec(MoveX, MoveZ);
				float MoveSize = MoveVec.length();
				//�R���g���[���̌�������p�x���v�Z
				float CntlAngle = atan2(-MoveX, MoveZ);
				//�g�[�^���̊p�x���Z�o
				float TotalAngle = FrontAngle + CntlAngle;
				//�p�x����x�N�g�����쐬
				Angle = Vec3(cos(TotalAngle), 0, sin(TotalAngle));
				//���K������
				Angle.normalize();
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
		Vec3 Angle = GetMoveVector();
		//Rigidbody�����o��
		auto PtrRedit = GetGameObject()->GetComponent<Rigidbody>();
		auto Velo = PtrRedit->GetVelocity();
		if (Angle.length() <= 0.0f && Velo.y == 0.0f) {
			//�R���g���[���𗣂����Ƃ��΍�
			Velo *= GetDecel();
			PtrRedit->SetVelocity(Velo);
			return;
		}
		//Transform
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		//���݂̑��x�����o��
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
		//���x��ݒ肷��
		PtrRedit->SetVelocity(Velo);
		//��]�̌v�Z
		if (Angle.length() > 0.0f) {
			auto UtilPtr = GetGameObject()->GetBehavior<UtilBehavior>();
			UtilPtr->RotToHead(Angle, 1.0f);
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
					auto Angle = Vec3(sin(RotY), 0, cos(RotY));
					Angle.normalize();
					auto Span = Angle * 0.5f;
					AttackPtr->Weakup(PtrTrans->GetWorldMatrix().transInMatrix() + Span, Angle * 10.0f);
					return;
				}
			}
		}

	}

	bool PlayerBehavior::OnHitObjMoveBox(vector<shared_ptr<GameObject>>& OtherVec, shared_ptr<GameObject>& retv) {
		for (auto& v : OtherVec) {
			if (v->FindTag(L"MoveBox")) {
				auto PtrTrans = GetGameObject()->GetComponent<Transform>();
				auto sp = GetGameObject()->GetComponent<CollisionSphere>()->GetSphere();
				auto obb = v->GetComponent<CollisionObb>()->GetObb();
				Vec3 RetPoint;
				HitTest::SPHERE_OBB(sp, obb, RetPoint);
				auto Normal = sp.m_Center - RetPoint;
				if (Normal.length() > sp.m_Radius * 1.5f) {
					return false;
				}
				Normal.normalize();
				Vec3 angle(XMVector3AngleBetweenNormals(Vec3(0, 1.0f, 0), Normal));
				if (angle.x >= 0.1f) {
					return false;
				}
				retv = v;
				return true;
			}
		}
		return false;
	}


	bool PlayerBehavior::OnMoveBox() {
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		auto sp = GetGameObject()->GetComponent<CollisionSphere>()->GetSphere();
		auto obb = PtrTrans->GetParent()->GetComponent<CollisionObb>()->GetObb();
		Vec3 RetPoint;
		HitTest::SPHERE_OBB(sp, obb, RetPoint);
		auto Normal = sp.m_Center - RetPoint;
		if (Normal.length() > sp.m_Radius * 1.5f) {
			return false;
		}
		Normal.normalize();
		Vec3 angle(XMVector3AngleBetweenNormals(Vec3(0, 1.0f, 0), Normal));
		if (angle.x >= 0.1f) {
			return false;
		}
		return true;
	}

}

//end basecross

