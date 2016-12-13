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
		m_BaseY(0.125f),
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
		PtrColl->SetIsHitAction(IsHitAction::AutoOnParentSlide);

		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();


		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//��������������
		SetAlphaActive(true);

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

	//���[�V��������������֐��Q
	void  SeekObject::SeekStartMoton() {
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(GetPlayerPosition());

	}
	bool  SeekObject::SeekUpdateMoton() {
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetTargetPosition(GetPlayerPosition());
		if (GetPlayerLength() <= m_StateChangeSize) {
			return true;
		}
		return false;
	}
	void  SeekObject::SeekEndMoton() {
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}

	void  SeekObject::ArriveStartMoton() {
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(GetPlayerPosition());
	}
	bool  SeekObject::ArriveUpdateMoton() {
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(GetPlayerPosition());
		if (GetPlayerLength() > m_StateChangeSize) {
			//�v���C���[�Ƃ̋��������ȏ�Ȃ�true
			return true;
		}
		return false;
	}
	void  SeekObject::ArriveEndMoton() {
		auto PtrArrive = GetComponent<ArriveSteering>();
		//Arrive�R���|�[�l���g�𖳌��ɂ���
		PtrArrive->SetUpdateActive(false);
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
		Obj->SeekStartMoton();
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj) {
		if (Obj->SeekUpdateMoton()) {
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void FarState::Exit(const shared_ptr<SeekObject>& Obj) {
		Obj->SeekEndMoton();
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
		Obj->ArriveStartMoton();
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj) {
		if (Obj->ArriveUpdateMoton()) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj) {
		Obj->ArriveEndMoton();
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
		auto PtrTransform = GetComponent<Transform>();

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

	//--------------------------------------------------------------------------------------
	//	class UnevenGroundData : public GameObject;
	//	�p�r: �ł��ڂ����̃f�[�^
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	UnevenGroundData::UnevenGroundData(const shared_ptr<Stage>& StagePtr):
		GameObject(StagePtr)
	{}
	UnevenGroundData::~UnevenGroundData() {}
	//������
	void UnevenGroundData::OnCreate() {
		vector<VertexPositionNormalTexture> vertices =
		{
			{ VertexPositionNormalTexture(Vector3(-1.0f, 0.0f, 0.5f), Vector3( 0.0f, 1.0f,0.0f), Vector2( 0.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vector3( 0.0f, 0.5f, 0.5f), Vector3( 0.0f, 1.0f,0.0f), Vector2( 0.5f, 0.0f)) },
			{ VertexPositionNormalTexture(Vector3(-1.0f, 0.0f,-0.5f), Vector3( 0.0f, 1.0f,0.0f), Vector2( 0.0f, 1.0f)) },
			{ VertexPositionNormalTexture(Vector3( 0.0f, 0.0f,-0.5f), Vector3( 0.0f, 1.0f,0.0f), Vector2( 0.5f, 1.0f)) },
			{ VertexPositionNormalTexture(Vector3( 1.0f, 0.0f, 0.5f), Vector3( 0.0f, 1.0f,0.0f), Vector2( 1.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vector3( 1.0f, 0.0f,-0.5f), Vector3( 0.0f, 1.0f,0.0f), Vector2( 1.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2 ,
			1, 4, 5 ,
			3, 1, 5
		};
		wstring CheckStr = L"UnevenGroundMesh";
		//���\�[�X���o�^����Ă��邩�m�F���A�o�^����ĂȂ���΍쐬
		//�������Ă����Ƃق��̃X�e�[�W�ł����b�V�����g����
		if (!App::GetApp()->CheckResource<MeshResource>(CheckStr)) {
			//���_���g���ĕ\���p���b�V�����\�[�X�̍쐬
			auto mesh = MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, false);
			//�\���p���b�V�������\�[�X�ɓo�^
			App::GetApp()->RegisterResource(CheckStr, mesh);
			//���C�A�t���[���\���p�̃��b�V���̍쐬
			vector<VertexPositionColor> new_vertices;
			for (size_t i = 0; i < vertices.size(); i++) {
				VertexPositionColor new_v;
				new_v.position = vertices[i].position;
				new_v.color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
				new_vertices.push_back(new_v);
			}
			//���C�A�t���[�����b�V�������\�[�X�ɓo�^
			App::GetApp()->RegisterResource(L"UnevenGroundWireMesh", MeshResource::CreateMeshResource(new_vertices, indices, false));
		}
		//�O�p�`�Փ˔���p�̎O�p�`�̔z����쐬
		//������̓X�e�[�W�P��
		size_t i = 0;
		while (i < indices.size()) {
			TRIANGLE tr;
			tr.m_A = vertices[indices[i]].position;
			tr.m_B = vertices[indices[i + 1]].position;
			tr.m_C = vertices[indices[i + 2]].position;
			m_Triangles.push_back(tr);
			i += 3;
		}

		//�������g���V�F�A�I�u�W�F�N�g�ɓo�^
		GetStage()->SetSharedGameObject(L"UnevenGroundData", GetThis<UnevenGroundData>());

	}



	//--------------------------------------------------------------------------------------
	//	class UnevenGround : public GameObject;
	//	�p�r: �ł��ڂ���
	//--------------------------------------------------------------------------------------
	UnevenGround::UnevenGround(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position):
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	UnevenGround::~UnevenGround() {}
	//������
	void UnevenGround::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		auto DataPtr = GetStage()->GetSharedGameObject<UnevenGroundData>(L"UnevenGroundData");
		//�O�p�`�R���W����
		auto PtrTriangle = AddComponent<CollisionTriangles>();
		PtrTriangle->SetMakedTriangles(DataPtr->GetTriangles());
		PtrTriangle->SetWireFrameMesh(L"UnevenGroundWireMesh");
		PtrTriangle->SetDrawActive(true);

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"UnevenGroundMesh");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"WALL_TX");

	}



}
//end basecross
