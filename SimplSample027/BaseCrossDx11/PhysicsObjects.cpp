/*!
@file PhysicsObjects.cpp
@brief �����v�Z�I�u�W�F�N�g����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�����v�Z����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedPsBox::FixedPsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	FixedPsBox::~FixedPsBox() {}
	//������
	void FixedPsBox::OnCreate() {

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f,vertices, indices);
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_BoxMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Position
		);
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(L"SKY_TX");
		//�`��f�[�^�̍\�z
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = m_BoxMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = true;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_FogEnabled = true;
		//�t�H�O�͐F���ۂ�
		m_PtrObj->m_FogColor = Col4(0.4f, 0.4f, 0.8f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;

		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = m_BoxMesh;
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();


		//�����v�Z�{�b�N�X
		PsBoxParam param;
		param.m_HalfSize = m_Scale * 0.5f;
		//�Œ�Ȃ̂Ŏ��ʂ͂���Ȃ�
		param.m_Mass = 0.0f;
		//�����e���\�����f�t�H���g�ŗǂ�
		param.m_MotionType = PsMotionType::MotionTypeFixed;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		m_PhysicsBox = GetStage()->GetPhysicsManager()->AddBox(param);

	}

	void FixedPsBox::OnDrawShadowmap() {
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Position
		);
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}


	void FixedPsBox::OnDraw() {
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
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

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_BoxMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Position
		);
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(L"SKY_TX");
		//�`��f�[�^�̍\�z
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = m_BoxMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = true;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_FogEnabled = true;
		//�t�H�O�͐F���ۂ�
		m_PtrObj->m_FogColor = Col4(0.4f, 0.4f, 0.8f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;

		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = m_BoxMesh;
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();

		//�����v�Z�{�b�N�X
		PsBoxParam param;
		//basecross�̃X�P�[�����O�͊e�ӊ�Ȃ̂ŁA�n�[�t�T�C�Y��ɂ���
		param.m_HalfSize = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		//�����e���\���̌v�Z
		param.m_Inertia = BasePhysics::CalcInertiaBox(param.m_HalfSize, param.m_Mass);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		m_PhysicsBox = GetStage()->GetPhysicsManager()->AddBox(param);

	}

	void ActivePsBox::OnUpdate() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsBox->GetIndex(), Status);

	}

	void ActivePsBox::OnDrawShadowmap() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsBox->GetIndex(), Status);
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			Status.m_Orientation,
			Status.m_Position
		);
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}


	void ActivePsBox::OnDraw() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsBox->GetIndex(), Status);
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			Status.m_Orientation,
			Status.m_Position
		);
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_WorldMatrix = World;
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
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
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			Vec3(m_Scale),
			Vec3(0, 0, 0),
			m_Qt,
			m_Position
		);
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(L"SKY_TX");
		//�`��f�[�^�̍\�z
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = m_SphereMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = true;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_FogEnabled = true;
		//�t�H�O�͐F���ۂ�
		m_PtrObj->m_FogColor = Col4(0.4f, 0.4f, 0.8f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;

		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = m_SphereMesh;
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();

		//�����v�Z����
		PsSphereParam param;
		//basecross�̃X�P�[�����O�͒��a��Ȃ̂ŁA���a��ɂ���
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		//�����e���\���̌v�Z
		param.m_Inertia = BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		m_PhysicsSphere = GetStage()->GetPhysicsManager()->AddSphere(param);

	}

	void ActivePsSphere::OnUpdate() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);

	}

	void ActivePsSphere::OnDrawShadowmap() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			Vec3(m_Scale),
			Vec3(0, 0, 0),
			Status.m_Orientation,
			Status.m_Position
		);
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}


	void ActivePsSphere::OnDraw() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			Vec3(m_Scale),
			Vec3(0, 0, 0),
			Status.m_Orientation,
			Status.m_Position
		);
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_WorldMatrix = World;
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}


	//--------------------------------------------------------------------------------------
	///	�����v�Z���锭�˂��鋅��
	//--------------------------------------------------------------------------------------
	FirePsSphere::FirePsSphere(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity) :
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity)
	{}
	FirePsSphere::~FirePsSphere() {}
	//������
	void FirePsSphere::OnCreate() {

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);


		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			Vec3(0.25f),
			Vec3(0, 0, 0),
			Quat(),
			m_Emitter
		);
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(L"SKY_TX");
		//�`��f�[�^�̍\�z
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = m_SphereMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = true;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_FogEnabled = true;
		//�t�H�O�͐F���ۂ�
		m_PtrObj->m_FogColor = Col4(0.4f, 0.4f, 0.8f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;

		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = m_SphereMesh;
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();

		PsSphereParam param;
		//basecross�̃X�P�[�����O�͒��a��Ȃ̂ŁA���a��ɂ���
		param.m_Radius = 0.25f * 0.5f;
		param.m_Mass = 1.0f;
		//�����e���\���̌v�Z
		param.m_Inertia = BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
		param.m_UseSleep = false;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat.identity();
		param.m_Pos = m_Emitter;
		param.m_LinearVelocity = m_Velocity;
		m_PhysicsSphere = GetStage()->GetPhysicsManager()->AddSphere(param);
	}
	//�X�V
	void FirePsSphere::OnUpdate() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);

	}

	void FirePsSphere::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		//���݂̃C���f�b�N�X�𓾂�
		auto Index = m_PhysicsSphere->GetIndex();
		PsSphereParam param;
		//basecross�̃X�P�[�����O�͒��a��Ȃ̂ŁA���a��ɂ���
		param.m_Radius = 0.25f * 0.5f;
		param.m_Mass = 1.0f;
		//�����e���\���̌v�Z
		param.m_Inertia = BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
		param.m_UseSleep = false;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat.identity();
		param.m_Pos = Emitter;
		param.m_LinearVelocity = Velocity;
		//�����C���f�b�N�X�ōč\�z
		m_PhysicsSphere = GetStage()->GetPhysicsManager()->AddSphere(param, Index);
	}


	void FirePsSphere::OnDrawShadowmap() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			Vec3(0.25f),
			Vec3(0, 0, 0),
			Status.m_Orientation,
			Status.m_Position
		);
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}


	void FirePsSphere::OnDraw() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			Vec3(0.25f),
			Vec3(0, 0, 0),
			Status.m_Orientation,
			Status.m_Position
		);
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_WorldMatrix = World;
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

}
//end basecross
