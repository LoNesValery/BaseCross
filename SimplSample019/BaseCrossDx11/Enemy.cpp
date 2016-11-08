/*!
@file Enemy.cpp
@brief �G�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	class HeadEnemy : public GameObject;
	//	�p�r: �ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	HeadEnemy::HeadEnemy(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_BaseY(m_StartPos.y),
		m_StateChangeSize(1.0f)
	{
	}
	HeadEnemy::~HeadEnemy() {}

	//������
	void HeadEnemy::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.25f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();
		//�ŉ��n�_
		PtrGravity->SetBaseY(0.125f);
		//Rigidbody������
		auto PtrRigid = AddComponent<Rigidbody>();
		//�������̂ݔ���
		//�����W����0.5�i�����j
		PtrRigid->SetReflection(0.5f);
		//Seek����
		auto PtrSeek = AddComponent<SeekSteering>();
		//Seek�͖����ɂ��Ă���
		PtrSeek->SetUpdateActive(false);
		//Arrive����
		auto PtrArrive = AddComponent<ArriveSteering>();
		//Arrive�͖����ɂ��Ă���
		PtrArrive->SetUpdateActive(false);

		//SP�̏Փ˔��������
		auto PtrColl = AddComponent<CollisionSphere>();
		//�������̂ݔ���
		PtrColl->SetIsHitAction(IsHitAction::AutoOnObjectRepel);

		PtrColl->SetDrawActive(true);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"TRACE2_TX");
		//��������������
		SetAlphaActive(true);
		m_BehaviorMachine = ObjectFactory::Create<BehaviorMachine<HeadEnemy>>(GetThis<HeadEnemy>());
		m_BehaviorMachine->AddBehavior<HeadStop>(L"Stop");
		m_BehaviorMachine->AddBehavior<HeadJumpSeek>(L"JumpSeek");
		m_BehaviorMachine->Reset(L"Stop");
	}

	//���[�e�B���e�B�֐��Q
	Vector3 HeadEnemy::GetPlayerPosition() const {
		//�����v���C���[��������������ĂȂ��ꍇ�ɂ́AVector3(0,m_BaseY,0)��Ԃ�
		Vector3 PlayerPos(0, m_BaseY, 0);
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player", false);
		if (PtrPlayer) {
			PlayerPos = PtrPlayer->GetComponent<Transform>()->GetPosition();
		}
		return PlayerPos;
	}
	float HeadEnemy::GetPlayerLength() const {
		auto MyPos = GetComponent<Transform>()->GetPosition();
		auto LenVec = GetPlayerPosition() - MyPos;
		return LenVec.Length();
	}

	//����
	void HeadEnemy::OnUpdate() {
		m_BehaviorMachine->Update();
	}

	void HeadEnemy::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
	}

	//��]��i�s�����Ɍ�������
	void HeadEnemy::RotToFront() {
		auto PtrRigidbody = GetComponent<Rigidbody>();
		//��]�̍X�V
		//Velocity�̒l�ŁA��]��ύX����
		//����Ői�s�����������悤�ɂȂ�
		auto PtrTransform = GetComponent<Transform>();
		Vector3 Velocity = PtrRigidbody->GetVelocity();
		if (Velocity.Length() > 0.0f) {
			Vector3 Temp = Velocity;
			Temp.Normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quaternion Qt;
			Qt.RotationRollPitchYaw(0, ToAngle, 0);
			Qt.Normalize();
			//���݂̉�]���擾
			Quaternion NowQt = PtrTransform->GetQuaternion();
			//���݂ƖڕW���ԁi10����1�j
			NowQt.Slerp(NowQt, Qt, 0.1f);
			PtrTransform->SetQuaternion(NowQt);
		}
	}


	void HeadEnemy::OnLastUpdate() {
		m_BehaviorMachine->Update2();
	}

	//--------------------------------------------------------------------------------------
	//	�Î~�s��
	//--------------------------------------------------------------------------------------
	void HeadStop::Enter(const shared_ptr<HeadEnemy>& Obj) {
		auto PtrRigidbody = Obj->GetComponent<Rigidbody>();
		PtrRigidbody->SetVelocity(0, 0, 0);
	}
	void HeadStop::Execute(const shared_ptr<HeadEnemy>& Obj) {
		if (Obj->GetPlayerLength() < 3.0f) {
			Obj->GetBehaviorMachine()->Push(L"JumpSeek");
		}
	}

	void HeadStop::WakeUp(const shared_ptr<HeadEnemy>& Obj) {
		auto PtrRigidbody = Obj->GetComponent<Rigidbody>();
		PtrRigidbody->SetVelocity(0, 0, 0);
	}

	void HeadStop::Exit(const shared_ptr<HeadEnemy>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	�W�����v���ĒǐՍs��
	//--------------------------------------------------------------------------------------
	void HeadJumpSeek::Enter(const shared_ptr<HeadEnemy>& Obj) {
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetDecl(2.0f);
		PtrArrive->SetTargetPosition(Obj->GetPlayerPosition());
		//�d��
		auto PtrGravity = Obj->GetComponent<Gravity>();
		//�W�����v�X�^�[�g
		Vector3 JumpVec(0.0f, 2.0f, 0);
		PtrGravity->StartJump(JumpVec);
	}
	void HeadJumpSeek::Execute(const shared_ptr<HeadEnemy>& Obj) {
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(Obj->GetPlayerPosition());
		//�d��
		auto PtrGravity = Obj->GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0) {
			if (Obj->GetPlayerLength() >= 3.0f) {
				Obj->GetBehaviorMachine()->Pop();
			}
			else if (Obj->GetPlayerLength() >= 2.0f) {
				//�W�����v�X�^�[�g
				Vector3 JumpVec(0.0f, 2.0f, 0);
				PtrGravity->StartJump(JumpVec);
			}
		}
	}

	void HeadJumpSeek::Execute2(const shared_ptr<HeadEnemy>& Obj) {
		Obj->RotToFront();
	}

	void HeadJumpSeek::Exit(const shared_ptr<HeadEnemy>& Obj) {
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(false);
		auto PtrTrans = Obj->GetComponent<Transform>();
		auto Pos = PtrTrans->GetPosition();
		Pos.y = 0.125f;
		PtrTrans->SetPosition(Pos);
	}

	//--------------------------------------------------------------------------------------
	//	�������������G
	//--------------------------------------------------------------------------------------

	//���[�e�B���e�B�֐��Q
	Vector3 MozoEnemy::GetPlayerPosition() const {
		//�����v���C���[��������������ĂȂ��ꍇ�ɂ́AVector3(0,m_BaseY,0)��Ԃ�
		Vector3 PlayerPos;
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		PlayerPos = PtrPlayer->GetComponent<Transform>()->GetPosition();
		return PlayerPos;
	}
	float MozoEnemy::GetPlayerLength() const {
		auto MyPos = GetComponent<Transform>()->GetPosition();
		auto LenVec = GetPlayerPosition() - MyPos;
		return LenVec.Length();
	}


	//�\�z�Ɣj��
	MozoEnemy::MozoEnemy(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_BaseY(m_StartPos.y)
	{
	}
	MozoEnemy::~MozoEnemy() {}

	void MozoEnemy::CreateMeshRes() {
		if (!App::GetApp()->CheckResource<MeshResource>(L"MOZO_SPHERE")) {
			App::GetApp()->RegisterResource(L"MOZO_SPHERE", MeshResource::CreateSphere(1.0f, 6));
		}
	}


	//������
	void MozoEnemy::OnCreate() {
		CreateMeshRes();
		m_MyLight.SetPositionToDirectional(-0.00001f, 1.0f, -0.00001f);

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.1f, 0.1f, 0.1f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();
		//�ŉ��n�_
		PtrGravity->SetBaseY(0.05f);
		//Rigidbody������
		auto PtrRigid = AddComponent<Rigidbody>();
		//�������̂ݔ���
		//�����W����0.5�i�����j
		PtrRigid->SetReflection(1.0f);
		//SP�̏Փ˔��������
		auto PtrColl = AddComponent<CollisionSphere>();
		//�������̂ݔ���
		PtrColl->SetIsHitAction(IsHitAction::AutoOnObjectRepel);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"MOZO_SPHERE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"MOZO_SPHERE");
		PtrDraw->SetTextureResource(L"MOZO_TX");


		//��������������
		SetAlphaActive(true);
		m_BehaviorMachine = ObjectFactory::Create<BehaviorMachine<MozoEnemy>>(GetThis<MozoEnemy>());
		m_BehaviorMachine->AddBehavior<MozoStart>(L"Start");
		m_BehaviorMachine->AddBehavior<MozoDefault>(L"Default");
		m_BehaviorMachine->Reset(L"Start");

	}

	const Light& MozoEnemy::OnGetDrawLight()const{
		return m_MyLight;
	}



	//����
	void MozoEnemy::OnUpdate() {
		m_BehaviorMachine->Update();
	}

	void MozoEnemy::OnLastUpdate() {
		m_BehaviorMachine->Update2();
	}


	//--------------------------------------------------------------------------------------
	//	�X�^�[�g�s��
	//--------------------------------------------------------------------------------------
	void MozoStart::Enter(const shared_ptr<MozoEnemy>& Obj) {
		auto PtrTransform = Obj->GetComponent<Transform>();
		PtrTransform->SetPosition(Obj->GetStartPosition());
		PtrTransform->SetScale(0.1f, 0.1f, 0.1f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		PtrTransform->SetToBefore();
		auto PtrRigid = Obj->GetComponent<Rigidbody>();
		PtrRigid->SetVelocity(0, 0, 0);

		auto PtrGrav = Obj->GetComponent<Gravity>();
		PtrGrav->SetUpdateActive(false);

	}
	void MozoStart::Execute(const shared_ptr<MozoEnemy>& Obj) {
		auto PlayerPos = Obj->GetPlayerPosition();
		auto Pos = Obj->GetComponent<Transform>()->GetPosition();
		Vector2 PlayerVec2(PlayerPos.x, PlayerPos.z);
		Vector2 Vec2(Pos.x, Pos.z);
		if (Vector2EX::Length(PlayerVec2 - Vec2) <= 0.5f) {
			auto PtrGrav = Obj->GetComponent<Gravity>();
			PtrGrav->SetUpdateActive(true);
		}
	}
	void MozoStart::Execute2(const shared_ptr<MozoEnemy>& Obj) {
		auto PtrGrav = Obj->GetComponent<Gravity>();
		if (PtrGrav->IsUpdateActive()) {
			auto PtrCol = Obj->GetComponent<CollisionSphere>();
			for (auto& v : PtrCol->GetHitObjectVec()) {
				Obj->GetBehaviorMachine()->Push(L"Default");
				return;
			}
		}
	}

	void MozoStart::Exit(const shared_ptr<MozoEnemy>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	�f�t�H���g�s��
	//--------------------------------------------------------------------------------------
	void MozoDefault::Enter(const shared_ptr<MozoEnemy>& Obj) {
		auto PtrRigid = Obj->GetComponent<Rigidbody>();
		float x = (Util::RandZeroToOne() - 0.5f);
		if (x < 0.0f) {
			x -= 0.5f;
		}
		else {
			x += 0.5f;
		}
		x *= 1.0f;
		float z = (Util::RandZeroToOne() - 0.5f);
		if (z < 0.0f) {
			z -= 0.5f;
		}
		else {
			z += 0.5f;
		}
		z *= 1.0f;
		PtrRigid->SetVelocity(x, 0, z);

	}
	void MozoDefault::Execute(const shared_ptr<MozoEnemy>& Obj) {
		auto Pos = Obj->GetComponent<Transform>()->GetPosition();
		auto StartPos = Obj->GetStartPosition();
		Vector2 Vec2Start(StartPos.x, StartPos.z);
		Vector2 Vec2(Pos.x, Pos.z);


		if (Vector2EX::Length(Vec2Start - Vec2) >= 3.0f) {
			Obj->GetBehaviorMachine()->Reset(L"Start");
		}
	}

	void MozoDefault::Sleep(const shared_ptr<MozoEnemy>& Obj) {
	}

	void MozoDefault::Exit(const shared_ptr<MozoEnemy>& Obj) {
		auto PtrRigid = Obj->GetComponent<Rigidbody>();
		PtrRigid->SetVelocity(0, 0, 0);
	}
	void MozoDefault::WakeUp(const shared_ptr<MozoEnemy>& Obj) {
	}



	//--------------------------------------------------------------------------------------
	//	class FixedCylinder : public GameObject;
	//	�p�r: �Œ�̃V�����_�[
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedCylinder::FixedCylinder(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedCylinder::~FixedCylinder() {}

	//������
	void FixedCylinder::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//�Փ˔���
		auto PtrColl = AddComponent<CollisionObb>();
		PtrColl->SetFixed(true);
		PtrColl->SetDrawActive(true);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CYLINDER");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"MOZO_SPHERE");
		PtrDraw->SetTextureResource(L"SKY_TX");

	}



}
// end basecross
