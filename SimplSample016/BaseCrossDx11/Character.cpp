/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�ǖ͗l�̃X�v���C�g
	//--------------------------------------------------------------------------------------
	WallSprite::WallSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vector2& StartScale, const Vector2& StartPos):
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos)
	{}

	WallSprite::~WallSprite() {}
	void WallSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��(�c��5���\��)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vector3(-HelfSize, HelfSize, 0),Color4(1.0f,1.0f,1.0f,1.0f), Vector2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vector3(HelfSize, HelfSize, 0), Color4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(5.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vector3(-HelfSize, -HelfSize, 0), Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 5.0f)) },
			{ VertexPositionColorTexture(Vector3(HelfSize, -HelfSize, 0), Color4(0.0f, 0.0f, 0, 1.0f), Vector2(5.0f, 5.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y, 0.0f);

		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"B�{�^���ŃQ�[���X�e�[�W��");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

		//���_�ƃC���f�b�N�X���w�肵�ăX�v���C�g�쐬
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetWrapSampler(true);
		PtrDraw->SetTextureResource(m_TextureKey);
	}

	void WallSprite::OnUpdate() {

	}




	//--------------------------------------------------------------------------------------
	///	�X�N���[������X�v���C�g
	//--------------------------------------------------------------------------------------
	ScrollSprite::ScrollSprite(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureKey, bool Trace, 
		const Vector2& StartScale, const Vector2& StartPos):
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}

	ScrollSprite::~ScrollSprite() {}
	void ScrollSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��
		m_BackupVertices = {
			{ VertexPositionTexture(Vector3(-HelfSize, HelfSize, 0), Vector2(0.0f, 0.0f)) },
			{ VertexPositionTexture(Vector3(HelfSize, HelfSize, 0), Vector2(4.0f, 0.0f)) },
			{ VertexPositionTexture(Vector3(-HelfSize, -HelfSize, 0), Vector2(0.0f, 1.0f)) },
			{ VertexPositionTexture(Vector3(HelfSize, -HelfSize, 0), Vector2(4.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y,1.0f);
		PtrTransform->SetRotation(0,0,0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y,0.0f);
		//���_�ƃC���f�b�N�X���w�肵�ăX�v���C�g�쐬
		auto PtrDraw = AddComponent<PTSpriteDraw>(m_BackupVertices, indices);
		PtrDraw->SetWrapSampler(true);
		PtrDraw->SetTextureResource(m_TextureKey);
	}
	void ScrollSprite::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime > 1.0f) {
			m_TotalTime = 0;
		}
		vector<VertexPositionTexture> NewVertices;
		for (size_t i = 0; i < m_BackupVertices.size(); i++) {
			Vector2 UV = m_BackupVertices[i].textureCoordinate;
			if (UV.x == 0.0f) {
				UV.x = m_TotalTime;
			}
			else if (UV.x == 4.0f) {
				UV.x += m_TotalTime;
			}
			auto v = VertexPositionTexture(
				m_BackupVertices[i].position,
				UV
			);
			NewVertices.push_back(v);
		}
		auto PtrDraw = GetComponent<PTSpriteDraw>();
		PtrDraw->UpdateVertices(NewVertices);
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
	//	�G
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Enemy::Enemy(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<StageCellMap>& CellMap,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_CelMap(CellMap),
		m_Scale(Scale),
		m_StartRotation(Rotation),
		m_StartPosition(Position),
		m_CellIndex(-1),
		m_NextCellIndex(-1),
		m_TargetCellIndex(-1)
	{
	}
	Enemy::~Enemy() {}

	bool Enemy::SearchPlayer() {
		auto MapPtr = m_CelMap.lock();
		if (MapPtr) {
			auto PathPtr = GetComponent<PathSearch>();
			auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
			auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
			m_CellPath.clear();
			//�p�X������������
			if (PathPtr->SearchCell(PlayerPos, m_CellPath)) {
				//��������������
				m_CellIndex = 0;
				m_TargetCellIndex = m_CellPath.size() - 1;
				if (m_CellIndex == m_TargetCellIndex) {
					//���łɓ����Z���ɂ���
					m_NextCellIndex = m_CellIndex;
				}
				else {
					//����Ă���
					m_NextCellIndex = m_CellIndex + 1;

				}
				return true;
			}
			else {
				//���s����
				m_CellIndex = -1;
				m_NextCellIndex = -1;
				m_TargetCellIndex = -1;
			}
		}
		return false;
	}

	bool Enemy::DefaultBehavior() {
		auto PtrRigid = GetComponent<Rigidbody>();
		auto Velo = PtrRigid->GetVelocity();
		Velo *= 0.95f;
		PtrRigid->SetVelocity(Velo);
		auto MapPtr = m_CelMap.lock();
		if (MapPtr) {
			auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
			auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
			CellIndex PlayerCell;
			if (MapPtr->FindCell(PlayerPos, PlayerCell)) {
				return false;
			}
		}
		return true;
	}


	bool Enemy::SeekBehavior() {
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
		auto MyPos = GetComponent<Transform>()->GetPosition();

		auto MapPtr = m_CelMap.lock();
		if (MapPtr) {
			if (SearchPlayer()) {
				auto PtrSeek = GetComponent<SeekSteering>();
				if (m_NextCellIndex == 0) {
					auto PtrRigid = GetComponent<Rigidbody>();
					auto Velo = PtrRigid->GetVelocity();
					Velo *= 0.95f;
					PtrRigid->SetVelocity(Velo);
					PlayerPos.y = m_StartPosition.y;
					PtrSeek->SetTargetPosition(PlayerPos);
				}
				else {
					if (Vector3EX::Length(MyPos - PlayerPos) <= 3.0f){
						auto PtrRigid = GetComponent<Rigidbody>();
						auto Velo = PtrRigid->GetVelocity();
						Velo *= 0.95f;
						PtrRigid->SetVelocity(Velo);
					}
					AABB ret;
					MapPtr->FindAABB(m_CellPath[m_NextCellIndex], ret);
					auto Pos = ret.GetCenter();
					Pos.y = m_StartPosition.y;
					PtrSeek->SetTargetPosition(Pos);
				}
				return true;
			}
			else {
				CellIndex PlayerCell;
				if (MapPtr->FindCell(PlayerPos, PlayerCell)) {
					auto PtrSeek = GetComponent<SeekSteering>();
					AABB ret;
					MapPtr->FindAABB(PlayerCell, ret);
					auto Pos = ret.GetCenter();
					Pos.y = m_StartPosition.y;
					PtrSeek->SetTargetPosition(Pos);
					return true;
				}
			}
		}
		return false;
	}


	//������
	void Enemy::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPosition);
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_StartRotation);
		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();
		//Rigidbody������
		auto PtrRigid = AddComponent<Rigidbody>();
		//�����W����0.5�i�����j
		PtrRigid->SetReflection(0.5f);
		auto PtrSeek = AddComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
		//�p�X����
		auto MapPtr = m_CelMap.lock();
		if (!MapPtr) {
			throw BaseException(
				L"�Z���}�b�v���s��ł�",
				L"if (!MapPtr) ",
				L" Enemy::OnCreate()"
			);
		}
		auto PathPtr = AddComponent<PathSearch>(MapPtr);

		//SP�̏Փ˔��������
		auto PtrColl = AddComponent<CollisionSphere>();
		PtrColl->SetIsHitAction(IsHitAction::AutoOnParent);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"TRACE2_TX");
		//��������������
		SetAlphaActive(true);

		m_StateMachine = make_shared<StateMachine<Enemy>>(GetThis<Enemy>());
		m_StateMachine->ChangeState(EnemyDefault::Instance());
	}

	//�X�V
	void Enemy::OnUpdate() {
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	//��]��i�s�����Ɍ�������
	void Enemy::OnLastUpdate() {
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
	///	�f�t�H���g�X�e�[�g
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	IMPLEMENT_SINGLETON_INSTANCE(EnemyDefault)

	void EnemyDefault::Enter(const shared_ptr<Enemy>& Obj) {
	}

	void EnemyDefault::Execute(const shared_ptr<Enemy>& Obj) {
		if (!Obj->DefaultBehavior()) {
			Obj->GetStateMachine()->ChangeState(EnemySeek::Instance());
		}
	}
	void EnemyDefault::Exit(const shared_ptr<Enemy>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	�v���C���[��ǂ�������X�e�[�g
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	IMPLEMENT_SINGLETON_INSTANCE(EnemySeek)

	void EnemySeek::Enter(const shared_ptr<Enemy>& Obj) {
		auto PtrSeek = Obj->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
	}

	void EnemySeek::Execute(const shared_ptr<Enemy>& Obj) {
		if (!Obj->SeekBehavior()) {
			Obj->GetStateMachine()->ChangeState(EnemyDefault::Instance());
		}
	}

	void EnemySeek::Exit(const shared_ptr<Enemy>& Obj) {
		auto PtrSeek = Obj->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}

	

}
//end basecross
