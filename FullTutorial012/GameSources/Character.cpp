/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�����v�Z����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedPsBox::FixedPsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	):
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	FixedPsBox::~FixedPsBox(){}
	//������
	void FixedPsBox::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//�����v�Z�{�b�N�X
		PsBoxParam param;
		param.m_HalfSize = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeFixed;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		m_PhysicsBox = GetStage()->GetPhysicsManager()->AddSingleBox(param);

	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃ{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ActivePsBox::ActivePsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsBox::~ActivePsBox() {}
	//������
	void ActivePsBox::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//�����v�Z�{�b�N�X
		PsBoxParam param;
		//basecross�̃X�P�[�����O�͊e�ӊ�Ȃ̂ŁA�n�[�t�T�C�Y��ɂ���
		param.m_HalfSize = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		m_PhysicsBox = GetStage()->GetPhysicsManager()->AddSingleBox(param);

	}

	void ActivePsBox::OnUpdate() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsBox->GetIndex(), Status);

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(Status.m_Orientation);
		PtrTransform->SetPosition(Status.m_Position);
	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȋ���
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ActivePsSphere::ActivePsSphere(const shared_ptr<Stage>& StagePtr,
		float Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsSphere::~ActivePsSphere() {}
	//������
	void ActivePsSphere::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");


		//�����v�Z����
		PsSphereParam param;
		//basecross�̃X�P�[�����O�͒��a��Ȃ̂ŁA���a��ɂ���
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		m_PhysicsSphere = GetStage()->GetPhysicsManager()->AddSingleSphere(param);

	}

	void ActivePsSphere::OnUpdate() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(Status.m_Orientation);
		PtrTransform->SetPosition(Status.m_Position);
	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z���锭�˂��鋅��
	//--------------------------------------------------------------------------------------
	FirePsSphere::FirePsSphere(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity):
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity)
	{}
	FirePsSphere::~FirePsSphere() {}
	//������
	void FirePsSphere::OnCreate() {
		m_IndexKey = L"FirePsSphere";
		//���L�I�u�W�F�N�g�ɃZ�b�g
		GetStage()->SetSharedGameObject(L"FirePsSphere",GetThis<FirePsSphere>());

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(0.25f));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Emitter);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"SKY_TX");

		PsSphereParam param;
		//basecross�̃X�P�[�����O�͒��a��Ȃ̂ŁA���a��ɂ���
		param.m_Radius = 0.25f * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat.identity();
		param.m_Pos = m_Emitter;
		param.m_Velocity = m_Velocity;
		m_PhysicsSphere = GetStage()->GetPhysicsManager()->AddSingleSphere(param, m_IndexKey);


	}
	//�X�V
	void FirePsSphere::OnUpdate() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(Vec3(0.25f));
		PtrTransform->SetQuaternion(Status.m_Orientation);
		PtrTransform->SetPosition(Status.m_Position);

	}

	void FirePsSphere::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		//���݂̃C���f�b�N�X�𓾂�
		auto Index = m_PhysicsSphere->GetIndex();
		PsSphereParam param;
		//basecross�̃X�P�[�����O�͒��a��Ȃ̂ŁA���a��ɂ���
		param.m_Radius = 0.25f * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat.identity();
		param.m_Pos = Emitter;
		param.m_Velocity = Velocity;
		//�����C���f�b�N�X�ōč\�z
		m_PhysicsSphere = ObjectFactory::Create<PhysicsSphere>(param, Index);
	}






}
//end basecross
