/*!
@file Player.cpp
@brief �v���C���[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�v���C���[
	//--------------------------------------------------------------------------------------
	//�\�z
	Player::Player(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName)
	{}

	Vec3 Player::GetMoveVector()const {
		Vec3 Angle(0, 0, 0);
		auto PtrGameStage = GetStage<GameStage>();
		Vec3 CameraEye, CameraAt;
		PtrGameStage->GetCamera().GetCameraEyeAt(CameraEye, CameraAt);
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if(CntlVec[0].bConnected){
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//���������̃X�s�[�h
				//�i�s�����̌������v�Z
				Vec3 Front =m_Pos - CameraEye;
				Front.y = 0;
				Front.normalize();
				//�i�s������������̊p�x���Z�o
				float FrontAngle = atan2(Front.z, Front.x);
				//�R���g���[���̌����v�Z
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				Vec2 MoveVec(MoveX, MoveZ);
				float MoveSize = length(MoveVec);
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

	//������
	void Player::OnCreate() {
		//�^�O�̒ǉ�
		AddTag(L"Player");
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//�����ʒu�Ȃǂ̐ݒ�
		m_Scale = Vec3(0.25f, 0.25f, 0.25f);	//���a25�Z���`�̋���
		m_Qt.identity();
		if (App::GetApp()->GetScene<Scene>()->IsFirstGame()) {
			m_Pos = Vec3(0, 0.125f, 0);
		}
		else {
			m_Pos = App::GetApp()->GetScene<Scene>()->GetBackupPlayerPos();
		}
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		
		m_PtrObj = make_shared<BcDrawObject>();
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		m_PtrObj->m_MeshRes = m_SphereMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = false;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_BlendState = BlendState::AlphaBlend;
		m_PtrObj->m_RasterizerState = RasterizerState::DoubleDraw;
		
		//�V���h�E�}�b�v�`��f�[�^�̍\�z
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = m_SphereMesh;
		//�`��f�[�^�̍s����R�s�[
		m_PtrShadowmapObj->m_WorldMatrix = World;

		//�������Z
		PsSphereParam param;
		//basecross�̃X�P�[�����O�͒��a��Ȃ̂ŁA���a��ɂ���
		param.m_Radius = 0.25f * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat.identity();
		param.m_Pos = m_Pos;
		param.m_Velocity = Vec3(0);
		m_PhysicsSphere = GetStage()->GetPhysicsManager()->AddSingleSphere(param);

	}

	//�X�V
	void Player::OnUpdate() {
		//�R���g���[���`�F�b�N���ē��͂�����΃R�}���h�Ăяo��
		m_InputHandler.PushHandle(GetThis<Player>());

		auto Vec = GetMoveVector();
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);

		PsBodyUpdateStatus SetStatus(Status);
		Vec3 Velo = GetMoveVector();
		SetStatus.m_LinearVelocity.x = Velo.x * 5.0f;
		SetStatus.m_LinearVelocity.z = Velo.z * 5.0f;
		GetStage()->GetPhysicsManager()->SetBodyStatus(m_PhysicsSphere->GetIndex(), SetStatus);
	}

	void Player::RotToHead(const bsm::Vec3& Velocity, float LerpFact) {
		if (LerpFact <= 0.0f) {
			//��ԌW����0�ȉ��Ȃ牽�����Ȃ�
			return;
		}
		//��]�̍X�V
		if (Velocity.length() > 0.0f) {
			bsm::Vec3 Temp = Velocity;
			Temp.normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			bsm::Quat Qt;
			Qt.rotationRollPitchYawFromVector(bsm::Vec3(0, ToAngle, 0));
			Qt.normalize();
			//���݂̉�]���擾
			bsm::Quat NowQt = m_Qt;
			//���݂ƖڕW����
			if (LerpFact >= 1.0f) {
				NowQt = Qt;
			}
			else {
				NowQt = XMQuaternionSlerp(NowQt, Qt, LerpFact);
			}
			m_Qt = NowQt;
		}

	}

	//��X�V
	void Player::OnUpdate2() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);
		m_Pos = Status.m_Position;
		//��]�̌v�Z
		Vec3 Angle = GetMoveVector();
		if (Angle.length() > 0.0f) {
			RotToHead(Angle, 0.1f);
		}
	}

	void Player::OnDrawShadowmap() {
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
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
	
	void Player::OnDraw() {
		//�s��̒�`
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}


	//A�{�^���n���h��
	void  Player::OnPushA() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);
		if (Status.m_Position.y > 0.125f) {
			//�n�ʂɂ���Ƃ������W�����v�ł���
			return;
		}
		PsBodyUpdateStatus SetStatus(Status);
		SetStatus.m_LinearVelocity += Vec3(0, 4.0f, 0.0);
		GetStage()->GetPhysicsManager()->SetBodyStatus(m_PhysicsSphere->GetIndex(), SetStatus);
	}


	//X�{�^���n���h��
	void Player::OnPushX() {
		Vec3 Pos = m_Pos;
		Pos.y += 0.25f;
		Quat Qt = m_Qt;
		Vec3 Rot = Qt.toRotVec();
		float RotY = Rot.y;
		Vec3 velo(sin(RotY), 0.05f, cos(RotY));
		velo.normalize();
		velo *= 20.0f;

		auto StagePtr = GetStage<GameStage>();

		auto ShPtr = StagePtr->GetFirePsSphere();
		if (ShPtr) {
			ShPtr->Reset(Pos, velo);
		}
		else {
			ShPtr = StagePtr->AddGameObject<FirePsSphere>(Pos, velo);
			StagePtr->SetFirePsSphere(ShPtr);
		}
	}

}
//end basecross
