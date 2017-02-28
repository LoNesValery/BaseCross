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
		m_StateChangeSize(7.0f)
	{
	}
	SeekObject::~SeekObject() {}

	//������
	void SeekObject::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.125f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();

		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"ObjectGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<SeekObject>());
		//�����s��������
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->SetGameObjectGroup(Group);
		//Obb�̏Փ˔��������
		auto PtrColl = AddComponent<CollisionObb>();
		PtrColl->SetIsHitAction(IsHitAction::Auto);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTnTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_PNTnT_CUBE");
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetTextureResource(L"GREEN_TX");
		PtrDraw->SetNormalMapTextureResource(L"NORMAL2_TX");

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<SeekObject>(GetThis<SeekObject>()));
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		m_StateMachine->ChangeState(FarState::Instance());
	}


	//����
	void SeekObject::OnUpdate() {
		//�X�e�[�g�ɂ���ĕς��Ȃ��s�������s
		auto PtrGrav = GetBehavior<Gravity>();
		PtrGrav->Execute();
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->Execute();
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	void SeekObject::OnUpdate2() {
		auto PtrUtil = GetBehavior<UtilBehavior>();
		PtrUtil->RotToHead(0.1f);
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
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekSteering>();
		float f = PtrSeek->Execute(L"Player");
		if (f < Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void FarState::Exit(const shared_ptr<SeekObject>& Obj) {
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
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveSteering>();
		if (PtrArrive->Execute(L"Player") >= Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj) {
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

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);

		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
	}

	//--------------------------------------------------------------------------------------
	//	class FixedNormalBox : public GameObject;
	//	�p�r: �Œ�̖@���}�b�v�����{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedNormalBox::FixedNormalBox(const shared_ptr<Stage>& StagePtr,
		const wstring& TexKey,
		const wstring& NormalTexKey,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_TexKey(TexKey),
		m_NormalTexKey(NormalTexKey),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedNormalBox::~FixedNormalBox() {}

	//������
	void FixedNormalBox::OnCreate() {

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

		auto PtrDraw = AddComponent<BcPNTnTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_PNTnT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetFogEnabled(true);
		//�t�H�O�͂��߂�
		PtrDraw->SetFogColor(Color4(0.3f, 0.3f, 0.3f, 1.0f));
		PtrDraw->SetFogStart(-10.0f);
		PtrDraw->SetFogEnd(-30.0f);
		PtrDraw->SetTextureResource(m_TexKey);
		PtrDraw->SetNormalMapTextureResource(m_NormalTexKey);
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

		AddTag(L"MoveBox");

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

		auto PtrDraw = AddComponent<BcPNTnTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_PNTnT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetFogEnabled(true);
		//�t�H�O�͂��߂�
		PtrDraw->SetFogColor(Color4(0.3f, 0.3f, 0.3f, 1.0f));
		PtrDraw->SetFogStart(-10.0f);
		PtrDraw->SetFogEnd(-30.0f);
		PtrDraw->SetTextureResource(L"RED_TX");
		PtrDraw->SetNormalMapTextureResource(L"FLOOR_NORMAL_TX");



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

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
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
		vector<VertexPositionNormalTangentTexture> vertices =
		{
			{ VertexPositionNormalTangentTexture(Vector3(-1.0f, 0.0f, 0.5f), Vector3( 0.0f, 1.0f,0.0f),Vector4(0.0f,0.0f,0.0f,1.0f), Vector2( 0.0f, 0.0f)) },
			{ VertexPositionNormalTangentTexture(Vector3( 0.0f, 0.5f, 0.5f), Vector3( 0.0f, 1.0f,0.0f), Vector4(0.0f,0.0f,0.0f,1.0f),Vector2( 0.5f, 0.0f)) },
			{ VertexPositionNormalTangentTexture(Vector3(-1.0f, 0.0f,-0.5f), Vector3( 0.0f, 1.0f,0.0f),Vector4(0.0f,0.0f,0.0f,1.0f), Vector2( 0.0f, 1.0f)) },
			{ VertexPositionNormalTangentTexture(Vector3( 0.0f, 0.0f,-0.5f), Vector3( 0.0f, 1.0f,0.0f),Vector4(0.0f,0.0f,0.0f,1.0f), Vector2( 0.5f, 1.0f)) },
			{ VertexPositionNormalTangentTexture(Vector3( 1.0f, 0.0f, 0.5f), Vector3( 0.0f, 1.0f,0.0f),Vector4(0.0f,0.0f,0.0f,1.0f), Vector2( 1.0f, 0.0f)) },
			{ VertexPositionNormalTangentTexture(Vector3( 1.0f, 0.0f,-0.5f), Vector3( 0.0f, 1.0f,0.0f),Vector4(0.0f,0.0f,0.0f,1.0f), Vector2( 1.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2 ,
			1, 4, 5 ,
			3, 1, 5
		};
		MeshUtill::SetNormalTangent(vertices);
		wstring CheckStr = L"UnevenGroundMesh";
		//���\�[�X���o�^����Ă��邩�m�F���A�o�^����ĂȂ���΍쐬
		//�������Ă����Ƃق��̃X�e�[�W�ł����b�V�����g����
		if (!App::GetApp()->CheckResource<MeshResource>(CheckStr)) {
			//���_���g���ĕ\���p���b�V�����\�[�X�̍쐬
			auto mesh = MeshResource::CreateMeshResource<VertexPositionNormalTangentTexture>(vertices, indices, false);
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

		auto PtrDraw = AddComponent<BcPNTnTStaticDraw>();
		PtrDraw->SetMeshResource(L"UnevenGroundMesh");
		PtrDraw->SetTextureResource(L"BROWN2_TX");
		PtrDraw->SetNormalMapTextureResource(L"WALL2_NORMAL_TX");
		//�X�y�L�����[���
		PtrDraw->SetSpecularColor(Color4(0.3f, 0.3f, 0.3f, 0));
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetOwnShadowActive(true);

	}



}
//end basecross
