/*!
@file Walls.cpp
@brief �ǂȂǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	���_�ύX�ł���v���[�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	DynamicPlate::DynamicPlate(const shared_ptr<Stage>& StagePtr,
		wstring TextureResName,
		UINT WidthCount,
		UINT HeightCount,
		bool IsTile,
		const Vector3& Scale,
		const Quaternion& Qt,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_WidthCount(WidthCount),
		m_HeightCount(HeightCount),
		m_IsTile(IsTile),
		m_StartScale(Scale),
		m_StartQt(Qt),
		m_StartPosition(Position),
		m_MyLight()
	{
		Matrix4X4 mt;
		mt.DefTransformation(Scale, Qt, Position);
		Vector3 Vec0(-0.5f, 0.5f, 0);
		Vector3 Vec1(0.5f, 0.5f, -2.0);
		Vector3 Vec2(-0.5f, -0.5f, -2.0);
		Vec0 = Vector3EX::Transform(Vec0, mt);
		Vec1 = Vector3EX::Transform(Vec1, mt);
		Vec2 = Vector3EX::Transform(Vec2, mt);
		auto Cross = Vector3EX::Cross(Vec1 - Vec0, Vec2 - Vec0);
		Cross.Normalize();
		m_MyLight.SetPositionToDirectional(Cross);
	}
	DynamicPlate::~DynamicPlate() {}


	//������
	void DynamicPlate::OnCreate() {
		m_BackupVertices.clear();
		m_BackupIndices.clear();

		//���b�V���ƒ��_�ύX�ł���`��R���|�[�l���g�̍쐬
		float HelfWidthCount = (float)m_WidthCount / 2.0f;
		float WidthPieceSize = 1.0f / (float)m_WidthCount;
		float HelfHeightCount = (float)m_HeightCount / 2.0f;
		float HeightPieceSize = 1.0f / (float)m_HeightCount;
		uint16_t SqCount = 0;
		float VCount = 0.0f;
		for (float y = HelfHeightCount; y > -HelfHeightCount; y-=1.0f) {
			float UCount = 0.0f;
			for (float x = -HelfWidthCount; x < HelfWidthCount; x+=1.0f) {
				float PosX = WidthPieceSize * x;
				float PosY = HeightPieceSize * y;
				if (m_IsTile) {
					m_BackupVertices.push_back(VertexPositionNormalTexture(Vector3(PosX, PosY, 0), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 0.0f)));
					m_BackupVertices.push_back(VertexPositionNormalTexture(Vector3(PosX + WidthPieceSize, PosY, 0), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 0.0f)));
					m_BackupVertices.push_back(VertexPositionNormalTexture(Vector3(PosX, PosY - HeightPieceSize, 0), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 1.0f)));
					m_BackupVertices.push_back(VertexPositionNormalTexture(Vector3(PosX + WidthPieceSize, PosY - HeightPieceSize, 0), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 1.0f)));
				}
				else {
					m_BackupVertices.push_back(VertexPositionNormalTexture(Vector3(PosX, PosY, 0), Vector3(0.0f, 0.0f, -1.0f), Vector2(UCount, VCount)));
					m_BackupVertices.push_back(VertexPositionNormalTexture(Vector3(PosX + WidthPieceSize, PosY, 0), Vector3(0.0f, 0.0f, -1.0f), Vector2(UCount + WidthPieceSize, VCount)));
					m_BackupVertices.push_back(VertexPositionNormalTexture(Vector3(PosX, PosY - HeightPieceSize, 0), Vector3(0.0f, 0.0f, -1.0f), Vector2(UCount, VCount + HeightPieceSize)));
					m_BackupVertices.push_back(VertexPositionNormalTexture(Vector3(PosX + WidthPieceSize, PosY - HeightPieceSize, 0), Vector3(0.0f, 0.0f, -1.0f), Vector2(UCount + WidthPieceSize, VCount + HeightPieceSize)));
				}
				m_BackupIndices.push_back(SqCount + 0);
				m_BackupIndices.push_back(SqCount + 1);
				m_BackupIndices.push_back(SqCount + 2);
				m_BackupIndices.push_back(SqCount + 1);
				m_BackupIndices.push_back(SqCount + 3);
				m_BackupIndices.push_back(SqCount + 2);
				SqCount += 4;
				UCount += WidthPieceSize;
			}
			VCount += HeightPieceSize;
		}
		auto PtrDraw = AddComponent<PNTDynamicDraw>();
		PtrDraw->CreateMesh(m_BackupVertices, m_BackupIndices);
		//�X�V�p�̒��_���o�b�N�A�b�v�Ɠ������e�ɂ��Ă���
		m_UpdateVertices = m_BackupVertices;
		PtrDraw->SetTextureResource(m_TextureResName);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale);
		PtrTransform->SetQuaternion(m_StartQt);
		PtrTransform->SetPosition(m_StartPosition);
	}
	const Light& DynamicPlate::OnGetDrawLight()const {
		return m_MyLight;
	}

	PLANE DynamicPlate::GetPLANE() const {
		auto TrasnsPtr = GetComponent<Transform>();
		//�\�ʏ��3�̓_���g����PLANE���쐬
		//1�ڂ̓_�͒��S
		Vector3 Point0 = TrasnsPtr->GetPosition();
		//2�ڂ�-0.5,-0.5,0�̓_�����[���h�ϊ���������
		Vector3 Point1(-0.5f, -0.5f, 0);
		Point1.Transform(TrasnsPtr->GetWorldMatrix());
		//3�ڂ�0.5,-0.5,0�̓_�����[���h�ϊ���������
		Vector3 Point2(0.5f, -0.5f, 0);
		Point2.Transform(TrasnsPtr->GetWorldMatrix());
		//3�_���g���Ėʂ��쐬
		PLANE ret(Point0, Point1, Point2);
		return ret;
	}

	COLRECT DynamicPlate::GetCOLRECT() const {
		auto TrasnsPtr = GetComponent<Transform>();
		COLRECT rect(1.0f, 1.0f, TrasnsPtr->GetWorldMatrix());
		return rect;
	}

	//�_�Ƃ̍ŋߐړ_��Ԃ�
	void DynamicPlate::ClosestPtPoint(const Vector3& Point, Vector3& Ret) {
		COLRECT rect = GetCOLRECT();
		Vector3 d = Point - rect.m_Center;
		Ret = rect.m_Center;
		for (int i = 0; i < 2; i++) {
			float dist = Vector3EX::Dot(d, rect.m_Rot[i]);
			if (dist > rect.m_UVec[i]) {
				dist = rect.m_UVec[i];
			}
			if (dist < -rect.m_UVec[i]) {
				dist = -rect.m_UVec[i];
			}
			Ret += rect.m_Rot[i] * dist;
		}
	}


	//�_�Ƃ̋�����Ԃ��i�߂�l���}�C�i�X�Ȃ痠���j
	float DynamicPlate::GetDistPointPlane(const Vector3& Point) const {
		PLANE pl = GetPLANE();
		return (Vector3EX::Dot(Point, pl.m_Normal) - pl.m_DotValue) / Vector3EX::Dot(pl.m_Normal, pl.m_Normal);
	}

	//�q�b�g�e�X�g�����čŋߐړ_�ƁASquare�̖@����Ԃ�
	bool DynamicPlate::HitTestSphere(const SPHERE& Sp, Vector3& RetVec, Vector3& Normal) {
		PLANE pl = GetPLANE();
		Normal = pl.m_Normal;
		//�l�p�`�Ƃ̍ŋߐړ_�𓾂�
		ClosestPtPoint(Sp.m_Center, RetVec);
		//�ŋߐړ_�����a�ȉ��Ȃ�Փ˂��Ă���
		if (Vector3EX::Length(Sp.m_Center - RetVec) <= Sp.m_Radius) {
			return true;
		}
		return false;
	}


	//--------------------------------------------------------------------------------------
	//	�ʂ蔲�����
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ThroughWall::ThroughWall(const shared_ptr<Stage>& StagePtr,
		wstring TextureResName,
		UINT WidthCount,
		UINT HeightCount,
		bool IsTile,
		const Vector3& Scale,
		const Quaternion& Qt,
		const Vector3& Position
	) :
		DynamicPlate(StagePtr, TextureResName,
			WidthCount, HeightCount, IsTile, Scale, Qt, Position)
	{
	}
	ThroughWall::~ThroughWall() {}

	void ThroughWall::OnUpdate() {
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
		auto WorldMat = GetComponent<Transform>()->GetWorldMatrix();
		for (size_t i = 0; i < m_BackupVertices.size(); i++) {
			if (i < m_UpdateVertices.size()) {
				auto VWorldPos = Vector3EX::Transform(m_BackupVertices[i].position, WorldMat);
				auto SpamVec = PlayerPos - VWorldPos;
				auto Len = Vector3EX::Length(SpamVec);
				if (Len <= 4.0f) {
					m_UpdateVertices[i].position.z = sin(Len * 8.0f) * 0.08f;
				}
				else {
					m_UpdateVertices[i].position.z = m_BackupVertices[i].position.z;
				}
				m_UpdateVertices[i].normal = m_BackupVertices[i].normal;
			}
		}
		for (size_t i = 0; i < m_BackupIndices.size(); i++) {
			if ((i % 3) == 0) {
				Vector3 Cros0 =
					m_UpdateVertices[m_BackupIndices[i + 1]].position
					- m_UpdateVertices[m_BackupIndices[i]].position;
				Vector3 Cros1 =
					m_UpdateVertices[m_BackupIndices[i + 2]].position
					- m_UpdateVertices[m_BackupIndices[i]].position;
				auto Cross = Vector3EX::Cross(Cros0, Cros1);
				Cross.Normalize();
				m_UpdateVertices[m_BackupIndices[i]].normal = Cross;
				m_UpdateVertices[m_BackupIndices[i + 1]].normal = Cross;
				m_UpdateVertices[m_BackupIndices[i + 2]].normal = Cross;
			}
		}
		auto PtrDraw = GetComponent<PNTDynamicDraw>();
		PtrDraw->UpdateVertices(m_UpdateVertices);
	}

	//--------------------------------------------------------------------------------------
	//	�^�C�����O���镁�ʂ̕�
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	TileWall::TileWall(const shared_ptr<Stage>& StagePtr,
		wstring TextureResName,
		UINT WidthTileCount,
		UINT HeightTileCount,
		const Vector3& Scale,
		const Quaternion& Qt,
		const Vector3& Position
	) :
		DynamicPlate(StagePtr,TextureResName,1,1,
			true,Scale,Qt,Position),
		m_WidthTileCount(WidthTileCount),
		m_HeightTileCount(HeightTileCount)
	{

	}
	TileWall::~TileWall() {}

	void TileWall::OnCreate() {
		DynamicPlate::OnCreate();
		for (size_t i = 0; i < m_BackupVertices.size(); i++) {
			if (m_BackupVertices[i].textureCoordinate.x != 0.0f) {
				m_BackupVertices[i].textureCoordinate.x = (float)m_WidthTileCount;
				m_UpdateVertices[i].textureCoordinate.x = (float)m_WidthTileCount;
			}
			if (m_BackupVertices[i].textureCoordinate.y != 0.0f) {
				m_BackupVertices[i].textureCoordinate.y = (float)m_HeightTileCount;
				m_UpdateVertices[i].textureCoordinate.y = (float)m_HeightTileCount;
			}
		}
		auto PtrDraw = GetComponent<PNTDynamicDraw>();
		PtrDraw->UpdateVertices(m_UpdateVertices);
		PtrDraw->SetWrapSampler(true);

	}





}
//end basecross
