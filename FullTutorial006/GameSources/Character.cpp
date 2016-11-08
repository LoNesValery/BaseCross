/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	�p�r: �ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_BaseY(m_StartPos.y),
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
		//�����W����0.5�i�����j
		PtrRegid->SetReflection(0.5f);
		//Seek����
		auto PtrSeek = AddComponent<SeekSteering>();
		//Arrive����
		auto PtrArrive = AddComponent<ArriveSteering>();
		//Arrive�͖����ɂ��Ă���
		PtrArrive->SetUpdateActive(false);

		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"ObjectGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<SeekObject>());
		//�����s��������
		AddComponent<SeparationSteering>(Group);
		//Obb�̏Փ˔��������
		auto PtrColl = AddComponent<CollisionObb>();
		//�������̂ݔ���
		PtrColl->SetIsHitAction(IsHitAction::AutoOnObjectRepel);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//��������������
		SetAlphaActive(true);

		//�ŏ��͌��C�ȍs��
		m_SeekBehavior = FineSeekBehavior::Instance();
		m_FineTiredTime = 0;
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< StateMachine<SeekObject> >(GetThis<SeekObject>());
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		m_StateMachine->ChangeState(FarState::Instance());
	}

	//���[�e�B���e�B�֐��Q
	Vector3 SeekObject::GetPlayerPosition() const {
		//�����v���C���[��������������ĂȂ��ꍇ�ɂ́AVector3(0,m_BaseY,0)��Ԃ�
		Vector3 PlayerPos(0, m_BaseY, 0);
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player", false);
		if (PtrPlayer) {
			PlayerPos = PtrPlayer->GetComponent<Transform>()->GetPosition();
			PlayerPos.y = m_BaseY;
		}
		return PlayerPos;
	}
	float SeekObject::GetPlayerLength() const {
		auto MyPos = GetComponent<Transform>()->GetPosition();
		auto LenVec = GetPlayerPosition() - MyPos;
		return LenVec.Length();
	}
	//����
	void SeekObject::OnUpdate() {
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}
	void SeekObject::OnLastUpdate() {
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
		//���y��m_BaseY
		auto Pos = PtrTransform->GetPosition();
		Pos.y = m_BaseY;
		PtrTransform->SetPosition(Pos);
	}

	//--------------------------------------------------------------------------------------
	//	Seek�I�u�W�F�N�g�̌��C�ȍs��
	//--------------------------------------------------------------------------------------
	shared_ptr<FineSeekBehavior> FineSeekBehavior::Instance() {
		static shared_ptr<FineSeekBehavior> instance(new FineSeekBehavior);
		return instance;
	}

	void FineSeekBehavior::FarEnter(const shared_ptr<SeekObject>& Obj) {
		auto PtrSeek = Obj->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(Obj->GetPlayerPosition());
	}
	void FineSeekBehavior::FarExecute(const shared_ptr<SeekObject>& Obj){
		auto PtrSeek = Obj->GetComponent<SeekSteering>();

		//�T�b�o�߂������ꂽ�s���ɂȂ�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Obj->m_FineTiredTime += ElapsedTime;
		if (Obj->m_FineTiredTime >= 3.0f) {
			Obj->m_FineTiredTime = 0;
			Obj->m_SeekBehavior = TiredSeekBehavior::Instance();
			PtrSeek->SetWeight(0.2f);
			auto PtrDraw = Obj->GetComponent<PNTStaticDraw>();
			PtrDraw->SetEmissive(Color4(0.5f, 0.0, 0.0, 1.0f));
		}

		PtrSeek->SetTargetPosition(Obj->GetPlayerPosition());
		if (Obj->GetPlayerLength() <= Obj->m_StateChangeSize) {
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void FineSeekBehavior::FarExit(const shared_ptr<SeekObject>& Obj) {
		auto PtrSeek = Obj->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}
	void FineSeekBehavior::NearEnter(const shared_ptr<SeekObject>& Obj) {
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(Obj->GetPlayerPosition());
	}
	void FineSeekBehavior::NearExecute(const shared_ptr<SeekObject>& Obj){
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(Obj->GetPlayerPosition());
		if (Obj->GetPlayerLength() > Obj->m_StateChangeSize) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}

	void FineSeekBehavior::NearExit(const shared_ptr<SeekObject>& Obj){
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		//Arrive�R���|�[�l���g�𖳌��ɂ���
		PtrArrive->SetUpdateActive(false);
	}

	//--------------------------------------------------------------------------------------
	//	Seek�I�u�W�F�N�g�̔�ꂽ�s��
	//--------------------------------------------------------------------------------------
	shared_ptr<TiredSeekBehavior> TiredSeekBehavior::Instance() {
		static shared_ptr<TiredSeekBehavior> instance(new TiredSeekBehavior);
		return instance;
	}

	void TiredSeekBehavior::FarEnter(const shared_ptr<SeekObject>& Obj) {
		auto PtrSeek = Obj->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(Obj->GetPlayerPosition());
	}
	void TiredSeekBehavior::FarExecute(const shared_ptr<SeekObject>& Obj) {
		auto PtrSeek = Obj->GetComponent<SeekSteering>();

		//�T�b�o�߂����猳�C�ȍs���ɂȂ�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Obj->m_FineTiredTime += ElapsedTime;
		if (Obj->m_FineTiredTime >= 3.0f) {
			Obj->m_FineTiredTime = 0;
			Obj->m_SeekBehavior = FineSeekBehavior::Instance();
			PtrSeek->SetWeight(1.0f);
			auto PtrDraw = Obj->GetComponent<PNTStaticDraw>();
			PtrDraw->SetEmissive(Color4(0.0f, 0.0, 0.0, 0.0f));
		}


		PtrSeek->SetTargetPosition(Obj->GetPlayerPosition());
		if (Obj->GetPlayerLength() <= Obj->m_StateChangeSize) {
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void TiredSeekBehavior::FarExit(const shared_ptr<SeekObject>& Obj) {
		auto PtrSeek = Obj->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}
	void TiredSeekBehavior::NearEnter(const shared_ptr<SeekObject>& Obj) {
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(Obj->GetPlayerPosition());
	}
	void TiredSeekBehavior::NearExecute(const shared_ptr<SeekObject>& Obj) {
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(Obj->GetPlayerPosition());
		if (Obj->GetPlayerLength() > Obj->m_StateChangeSize) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}

	void TiredSeekBehavior::NearExit(const shared_ptr<SeekObject>& Obj) {
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		//Arrive�R���|�[�l���g�𖳌��ɂ���
		PtrArrive->SetUpdateActive(false);
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
		Obj->GetSeekBehavior()->FarEnter(Obj);
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj) {
		Obj->GetSeekBehavior()->FarExecute(Obj);
	}
	void FarState::Exit(const shared_ptr<SeekObject>& Obj) {
		Obj->GetSeekBehavior()->FarExit(Obj);
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
		Obj->GetSeekBehavior()->NearEnter(Obj);
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj) {
		Obj->GetSeekBehavior()->NearExecute(Obj);
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj) {
		Obj->GetSeekBehavior()->NearExit(Obj);
	}

	//--------------------------------------------------------------------------------------
	//	class StaticModel : public GameObject;
	//	�p�r: �Œ�̃��f��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	StaticModel::StaticModel(const shared_ptr<Stage>& StagePtr,
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
	StaticModel::~StaticModel() {}

	//������
	void StaticModel::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		Matrix4X4 SpanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		SpanMat.DefTransformation(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f,0, 0.0f),
			Vector3(0.0f, 0.0f, 0.0f)
		);


		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"Chara_Rst_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		auto PtrDraw = AddComponent<PNTStaticModelDraw>();
		PtrDraw->SetMeshResource(L"Chara_Rst_MESH");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);
		PtrDraw->SetLighting(ShaderLighting::Midium);
		PtrDraw->SetEmissive(Color4(0.4, 0.4, 0.4, 0));
		PtrDraw->SetDiffuse(Color4(0.6, 0.6, 0.6, 1));

	}


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
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
	FixedBox::~FixedBox() {}

	//������
	void FixedBox::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//�Փ˔���
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
	}

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	�p�r: �㉺�ړ�����{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MoveBox::MoveBox(const shared_ptr<Stage>& StagePtr,
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

	MoveBox::~MoveBox() {}

	//������
	void MoveBox::OnCreate() {
		auto PtrTransform = AddComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//�A�N�V�����̓o�^
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddMoveBy(5.0f, Vector3(5.0f, 5.0f, 0));
		PtrAction->AddMoveBy(5.0f, Vector3(-5.0f, -5.0f, 0));
		//���[�v����
		PtrAction->SetLooped(true);
		//�A�N�V�����J�n
		PtrAction->Run();



		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		auto Group = GetStage()->GetSharedObjectGroup(L"MoveBox");
		Group->IntoGroup(GetThis<GameObject>());


	}

	//--------------------------------------------------------------------------------------
	//	class SphereObject : public GameObject;
	//	�p�r: ��Q����
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SphereObject::SphereObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{}
	SphereObject::~SphereObject() {}
	//������
	void SphereObject::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//Sphere�̏Փ˔��������
		auto CollPtr = AddComponent<CollisionSphere>();
		CollPtr->SetFixed(true);
		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"WALL_TX");

	}




}
//end basecross
