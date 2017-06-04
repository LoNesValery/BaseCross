/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	PC��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PcSphere::PcSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PcSphere::~PcSphere() {}

	//������
	void PcSphere::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		Quaternion Qt;
		Qt.Identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(m_StartPos);

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<BcPCStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionColor> new_vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f,18, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionColor new_v;
			new_v.position = vertices[i].position;
			new_v.color = Color4(
				new_v.position.x * 2.0f, 
				new_v.position.y * 2.0f, 
				new_v.position.z * 2.0f, 
				1.0f);
			new_vertices.push_back(new_v);

		}
		PtrDraw->CreateOriginalMesh(new_vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(PtrDraw->GetOriginalMeshResource());
	}

	void PcSphere::OnUpdate() {
		auto Beh = GetBehavior<VertexBehavior>();
		Beh->ExpandAndContract<VertexPositionColor, BcPCStaticDraw>();
	}

	//--------------------------------------------------------------------------------------
	///	PT��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PtSphere::PtSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PtSphere::~PtSphere() {}

	//������
	void PtSphere::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		Quaternion Qt;
		Qt.Identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(m_StartPos);

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<BcPTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionTexture> new_vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionTexture new_v;
			new_v.position = vertices[i].position;
			new_v.textureCoordinate = vertices[i].textureCoordinate;
			new_vertices.push_back(new_v);
		}
		PtrDraw->CreateOriginalMesh(new_vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(PtrDraw->GetOriginalMeshResource());
	}
	void PtSphere::OnUpdate() {
		auto Beh = GetBehavior<VertexBehavior>();
		Beh->ExpandAndContract<VertexPositionTexture, BcPTStaticDraw>();
	}


	//--------------------------------------------------------------------------------------
	///	PCT��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PctSphere::PctSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PctSphere::~PctSphere() {}

	//������
	void PctSphere::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		Quaternion Qt;
		Qt.Identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(m_StartPos);

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<BcPCTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionColorTexture> new_vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionColorTexture new_v;
			new_v.position = vertices[i].position;
			new_v.textureCoordinate = vertices[i].textureCoordinate;
			new_v.color = Color4(
				new_v.position.x * 2.0f,
				new_v.position.y * 2.0f,
				new_v.position.z * 2.0f,
				1.0f);
			new_vertices.push_back(new_v);
		}
		PtrDraw->CreateOriginalMesh(new_vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(PtrDraw->GetOriginalMeshResource());
	}

	void PctSphere::OnUpdate() {
		auto Beh = GetBehavior<VertexBehavior>();
		Beh->ExpandAndContract<VertexPositionColorTexture, BcPCTStaticDraw>();
	}



	//--------------------------------------------------------------------------------------
	///	Pnt��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PntSphere::PntSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos, bool TextureUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TextureUse(TextureUse)
	{
	}
	PntSphere::~PntSphere() {}

	//������
	void PntSphere::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		Quaternion Qt;
		Qt.Identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(m_StartPos);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		if (m_TextureUse) {
			PtrDraw->SetTextureResource(L"SKY_TX");
		}
	}

	//--------------------------------------------------------------------------------------
	///	Pnt�X�y�L�����[��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PntSpSphere::PntSpSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos, bool TextureUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TextureUse(TextureUse)
	{
	}
	PntSpSphere::~PntSpSphere() {}
	//������
	void PntSpSphere::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		Quaternion Qt;
		Qt.Identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(m_StartPos);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetSpecularColorAndPower(Color4(1.0f, 1.0f, 1.0f, 1.0f));
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		if (m_TextureUse) {
			PtrDraw->SetTextureResource(L"SKY_TX");
		}
	}


	//--------------------------------------------------------------------------------------
	///	Static�L����
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	StaticChara::StaticChara(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos, bool TamgentUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TamgentUse(TamgentUse)
	{
	}
	StaticChara::~StaticChara() {}
	//������
	void StaticChara::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.5f, 0.5f, 0.5f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		Matrix4X4 SpanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		SpanMat.DefTransformation(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, 0.0f, 0.0f)
		);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"MODEL_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		if (m_TamgentUse) {
			auto PtrDraw = AddComponent<BcPNTnTStaticModelDraw>();
			PtrDraw->SetFogEnabled(true);
			PtrDraw->SetMeshResource(L"MODEL_MESH_WITH_TAN");
			PtrDraw->SetNormalMapTextureResource(L"MODEL_NORMAL_TX");
			PtrDraw->SetMeshToTransformMatrix(SpanMat);
		}
		else {
			auto PtrDraw = AddComponent<BcPNTStaticModelDraw>();
			PtrDraw->SetFogEnabled(true);
			PtrDraw->SetMeshResource(L"MODEL_MESH");
			PtrDraw->SetMeshToTransformMatrix(SpanMat);
		}



	}



	//--------------------------------------------------------------------------------------
	///	Bone�L����
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	BoneChara::BoneChara(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos, bool TamgentUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TamgentUse(TamgentUse)
	{
	}
	BoneChara::~BoneChara() {}

	//������
	void BoneChara::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.5f, 0.5f, 0.5f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		Matrix4X4 SpanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		SpanMat.DefTransformation(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, 0.0f, 0.0f)
		);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();

		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"Chara_R_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		if (m_TamgentUse) {
			//�`��R���|�[�l���g�̐ݒ�
			auto PtrDraw = AddComponent<BcPNTnTBoneModelDraw>();
			PtrDraw->SetFogEnabled(true);
			//�`�悷�郁�b�V����ݒ�
			PtrDraw->SetMeshResource(L"Chara_R_MESH_WITH_TAN");
			PtrDraw->SetNormalMapTextureResource(L"Chara_R_NORMAL_TX");
			PtrDraw->SetMeshToTransformMatrix(SpanMat);
			PtrDraw->AddAnimation(L"Default", 0, 100, true, 20.0f);
			PtrDraw->ChangeCurrentAnimation(L"Default");
		}
		else {
			//�`��R���|�[�l���g�̐ݒ�
			auto PtrDraw = AddComponent<BcPNTBoneModelDraw>();
			PtrDraw->SetFogEnabled(true);
			//�`�悷�郁�b�V����ݒ�
			PtrDraw->SetMeshResource(L"Chara_R_MESH");
			PtrDraw->SetMeshToTransformMatrix(SpanMat);

			PtrDraw->AddAnimation(L"Default", 0, 100, true, 20.0f);
			PtrDraw->ChangeCurrentAnimation(L"Default");

		}
		//��������
		SetAlphaActive(true);
	}

	//�X�V
	void BoneChara::OnUpdate() {
		//�A�j���[�V�������X�V����
		if (m_TamgentUse) {
			auto PtrDraw = GetComponent<BcPNTnTBoneModelDraw>();
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			PtrDraw->UpdateAnimation(ElapsedTime);

		}
		else {
			auto PtrDraw = GetComponent<BcPNTBoneModelDraw>();
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			PtrDraw->UpdateAnimation(ElapsedTime);
		}
	}


}
//end basecross