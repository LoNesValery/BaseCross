/*!
@file ProjectBehavior.h
@brief �v���W�F�N��`�̍s���N���X
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�R���g���[���̃{�^���̃n���h��
	//--------------------------------------------------------------------------------------
	template<typename T>
	struct InputHandler {
		void PushHandle(const shared_ptr<T>& Obj) {
			//�R���g���[���̎擾
			auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (CntlVec[0].bConnected) {
				//A�{�^��
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					Obj->OnPushA();
				}
				//X�{�^��
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_X) {
					Obj->OnPushX();
				}
			}
		}
	};

	//--------------------------------------------------------------------------------------
	///	�키�n�̍s���N���X
	//--------------------------------------------------------------------------------------
	class Sword;
	template <typename T>
	class FightBehavior : public ObjBehavior<T> {
		//��]�؂�̉�]�p�x
		float m_RotationShakeRad;
		//����weak_ptr
		weak_ptr<Sword> m_SwordWeak;
		//����̌��ɓ��������Ƃ��̕ϐ�
		//����̏ꏊ
		Vector3 m_HitBase;
		//�Ђ�ގ���
		float m_HitSwordTime;
		//�|�ꂽ�����p
		enum class DieFlag {
			Down,
			Die,
			Up,
		};
		float m_DieRot;
		float m_DieInterval;
		DieFlag m_DieFlag;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		FightBehavior(const shared_ptr<T>& GameObjectPtr):
			ObjBehavior(GameObjectPtr)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~FightBehavior() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	����������
		@param[in]	SwordKey	������肷��L�[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void HaveSword(const wstring& SwordKey) {
			auto SwordPtr = m_SwordWeak.lock();
			if (!SwordPtr) {
				SwordPtr = GetStage()->GetSharedGameObject<Sword>(SwordKey, false);
				if (SwordPtr) {
					m_SwordWeak = SwordPtr;
				}
				else {
					//��O
					throw BaseException(
						L"��������ł��܂���",
						L"if (!SwordPtr)",
						L" FightBehavior<T>::HaveSword()"
					);
				}
			}
			auto SwordTransPtr = SwordPtr->GetComponent<Transform>();
			//���̐e�ɂ���
			SwordTransPtr->SetParent(GetGameObject());
			Vector3 Pos(0, 0, -0.2f);
			SwordTransPtr->SetPosition(Pos);
			Quaternion Qt(Vector3(0, 1, 0), 0);
			SwordTransPtr->SetQuaternion(Qt);
			//���̃R���W�����͖����ɂ���
			auto SowdCollPtr = SwordPtr->GetComponent<Collision>();
			SowdCollPtr->SetUpdateActive(false);
			//�{�̂̃R���W�����͗L���ɂ���
			GetGameObject()->GetComponent<Collision>()->SetUpdateActive(true);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	����U�鏈���̊J�n
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void StartShakeSword() {
			auto SwordPtr = m_SwordWeak.lock();
			if (SwordPtr) {
				Vector3 Pos(sin(0.0f + XM_PIDIV2) * 0.5f, 0.0f, cos(0.0f + XM_PIDIV2) * 0.5f);
				auto SwordTransPtr = SwordPtr->GetComponent<Transform>();
				SwordTransPtr->ResetPosition(Pos);
				Quaternion Qt(Vector3(0, 0, 1), XM_PIDIV2);
				SwordTransPtr->SetQuaternion(Qt);
				m_RotationShakeRad = 0;
				//���̃R���W������L���ɂ���
				auto SowdCollPtr = SwordPtr->GetComponent<Collision>();
				SowdCollPtr->SetUpdateActive(true);

			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	����U��i��]�j
		@return	������]������true
		*/
		//--------------------------------------------------------------------------------------
		bool RotationShakeSword() {
			auto SwordPtr = m_SwordWeak.lock();
			if (SwordPtr) {
				auto SwordTransPtr = SwordPtr->GetComponent<Transform>();
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				float RotSpan = -ElapsedTime * 10.0f;
				m_RotationShakeRad += RotSpan;
				Quaternion QtSpan(Vector3(0, 1, 0), RotSpan);
				auto Qt = SwordTransPtr->GetQuaternion();
				Qt *= QtSpan;
				SwordTransPtr->SetQuaternion(Qt);
				Vector3 Pos(sin(m_RotationShakeRad + XM_PIDIV2) * 0.5f, 0.0f, cos(m_RotationShakeRad + XM_PIDIV2) * 0.5f);
				SwordTransPtr->SetPosition(Pos);
				if (abs(m_RotationShakeRad) >= XM_PI) {
					m_RotationShakeRad = 0;
					//���̃R���W�����͖����ɂ���
					auto SowdCollPtr = SwordPtr->GetComponent<Collision>();
					SowdCollPtr->SetUpdateActive(false);
					return true;
				}
				else {
					return false;
				}
			}
			//�����擾�ł��Ȃ���Ώ�Ɂu��]�C���v
			return true;
		}
		Vector3 m_PokeStart;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	����˂������̊J�n
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void StartPokeSword() {
			auto SwordPtr = m_SwordWeak.lock();
			if (SwordPtr) {
				auto SwordTransPtr = SwordPtr->GetComponent<Transform>();

				m_PokeStart = SwordTransPtr->GetPosition();

				Vector3 Pos(sin(0.0f) * 1.0f, 0.0f, cos(0.0f) * 1.0f);
				SwordTransPtr->ResetPosition(Pos);
				//���̃R���W������L���ɂ���
				auto SowdCollPtr = SwordPtr->GetComponent<Collision>();
				SowdCollPtr->SetUpdateActive(true);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	����˂�
		@return	��苗���˂�����true
		*/
		//--------------------------------------------------------------------------------------
		bool PokeSword() {
			auto SwordPtr = m_SwordWeak.lock();
			if (SwordPtr) {
				auto SwordTransPtr = SwordPtr->GetComponent<Transform>();
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				float PokeSpan = ElapsedTime * 5.0f;
				Vector3 Pos = SwordTransPtr->GetPosition();
				Pos.y -= PokeSpan;
				SwordTransPtr->SetPosition(Pos);
				if (Pos.y <= (m_PokeStart.y - 2.5f)) {
					Pos = m_PokeStart;
					SwordTransPtr->SetPosition(Pos);
					//���̃R���W�����͖����ɂ���
					auto SowdCollPtr = SwordPtr->GetComponent<Collision>();
					SowdCollPtr->SetUpdateActive(false);
					return true;
				}
				else {
					return false;
				}
			}
			//�����擾�ł��Ȃ���Ώ�Ɂu�C���v
			return true;
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief	����̌��ɓ��������Ƃ��̑���̈ʒu�̐ݒ�
		@param[in]	pos	�ʒu
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetHitBase(const Vector3& pos) {
			m_HitBase = pos;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	����̌��ɓ��������s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void HitSwordBehavior() {
			CAPSULE SrcCap = GetGameObject()->GetComponent<CollisionCapsule>()->GetCapsule();
			Vector3 HitVelo = SrcCap.GetCenter() - m_HitBase;
			HitVelo.y = 0;
			HitVelo.Normalize();
			HitVelo *= 6.0f;
			auto PtrRig = GetGameObject()->GetComponent<Rigidbody>();
			PtrRig->SetVelocity(HitVelo);
			m_HitSwordTime = 0;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���ɓ������ĂЂ�ލs��
		@return	�Ђ�ގ��Ԃ��I�������true
		*/
		//--------------------------------------------------------------------------------------
		bool HitAfterdBehavior() {
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			m_HitSwordTime += ElapsedTime;
			auto PtrDraw = GetGameObject()->GetComponent<BcPNTStaticDraw>();
			float val = abs(sin(m_HitSwordTime * 10.0f));
			Color4 Col(val, val, val, 0);
			PtrDraw->SetEmissive(Col);
			if (m_HitSwordTime >= 0.7f) {
				PtrDraw->SetEmissive(Color4(0,0,0,1));
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�|���J�n
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void DieStartBehavior() {
			//�R���W�������~�߂�
			GetGameObject()->GetComponent<Collision>()->SetUpdateActive(false);
			//���̃R���W�������~�߂�
			auto SwordPtr = m_SwordWeak.lock();
			if (SwordPtr) {
				SwordPtr->GetComponent<Collision>()->SetUpdateActive(false);
			}
			//���x���~�߂�
			auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>();
			PtrRigid->SetVelocityZero();

			m_DieRot = 0.0f;
			m_DieInterval = 0.0f;
			m_DieFlag = DieFlag::Down;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�|���s��
		@return	��A�̓��삪�I����true
		*/
		//--------------------------------------------------------------------------------------
		bool DieBehavior() {
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			switch (m_DieFlag) {
			case DieFlag::Down: 
				{
					m_DieRot += ElapsedTime;
					auto Qt = GetGameObject()->GetComponent<Transform>()->GetQuaternion();
					Quaternion SpanQt(Vector3(1, 0, 0), ElapsedTime);
					Qt *= SpanQt;
					GetGameObject()->GetComponent<Transform>()->SetQuaternion(Qt);
					if (m_DieRot >= XM_PIDIV2) {
						auto MoveToPtr = GetGameObject()->GetComponent<MoveTo>();
						MoveToPtr->SetUpdateActive(true);
						MoveToPtr->SetParams(3.0f, GetGameObject()->GetStartPosition());
						MoveToPtr->Run();
						m_DieFlag = DieFlag::Die;
					}
					auto Pos = GetGameObject()->GetComponent<Transform>()->GetPosition();
					if (Pos.y >= 0.25f) {
						Pos.y -= ElapsedTime;
						GetGameObject()->GetComponent<Transform>()->SetPosition(Pos);
					}
					Color4 Col = GetGameObject()->GetComponent<BcPNTStaticDraw>()->GetDiffuse();
					Col.x -= 0.05f;
					if (Col.x <= 0.0f) {
						Col.x = 0.0f;
					}
					Col.y -= 0.05f;
					if (Col.y <= 0.0f) {
						Col.y = 0.0f;
					}
					Col.z -= 0.05f;
					if (Col.z <= 0.0f) {
						Col.z = 0.0f;
					}
					GetGameObject()->GetComponent<BcPNTStaticDraw>()->SetDiffuse(Col);
					return false;
				}
				break;
			case DieFlag::Die:
				m_DieInterval += ElapsedTime;
				if (m_DieInterval >= 5.0f) {
					auto MoveToPtr = GetGameObject()->GetComponent<MoveTo>();
					MoveToPtr->SetUpdateActive(false);
					auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>();
					PtrRigid->SetVelocityZero();
					m_DieFlag = DieFlag::Up;
				}
				return false;
				break;
			case DieFlag::Up:
				{
					m_DieRot -= ElapsedTime * 2.0f;
					auto Qt = GetGameObject()->GetComponent<Transform>()->GetQuaternion();
					Quaternion SpanQt(Vector3(1, 0, 0), -ElapsedTime * 2.0f);
					Qt *= SpanQt;
					GetGameObject()->GetComponent<Transform>()->SetQuaternion(Qt);
					auto Pos = GetGameObject()->GetComponent<Transform>()->GetPosition();
					if (Pos.y <= 0.5f) {
						Pos.y += ElapsedTime * 2.0f;
						GetGameObject()->GetComponent<Transform>()->SetPosition(Pos);
					}
					if (m_DieRot <= 0) {
						GetGameObject()->GetComponent<BcPNTStaticDraw>()->SetDiffuse(Color4(1.0f,1.0f,1.0f,0.0f));
						return true;
					}
					Color4 Col = GetGameObject()->GetComponent<BcPNTStaticDraw>()->GetDiffuse();
					Col.x += 0.05f;
					if (Col.x >= 1.0f) {
						Col.x = 1.0f;
					}
					Col.y += 0.05f;
					if (Col.y >= 1.0f) {
						Col.y = 1.0f;
					}
					Col.z += 0.05f;
					if (Col.z >= 1.0f) {
						Col.z = 1.0f;
					}
					GetGameObject()->GetComponent<BcPNTStaticDraw>()->SetDiffuse(Col);

					return false;
				}
				break;
			}
			return false;
		}

	};

	//--------------------------------------------------------------------------------------
	///	�Z���������g�����s���N���X�̃t���O
	//--------------------------------------------------------------------------------------
	enum class CellSearchFlg {
		Failed,
		Seek,
		Arrived
	};

	//--------------------------------------------------------------------------------------
	///	�Z���������g�����s���N���X
	//--------------------------------------------------------------------------------------
	template <typename T>
	class CellSearchBehavior : public ObjBehavior<T> {
		//�ΏۂƂȂ�Z���}�b�v
		weak_ptr<StageCellMap> m_CelMap;
		//�p�X�o�H
		vector<CellIndex> m_CellPath;
		//���݂̎����̃Z���C���f�b�N�X
		int m_CellIndex;
		//�߂����i���́j�̃Z���C���f�b�N�X
		int m_NextCellIndex;
		//�^�[�Q�b�g�̃Z���C���f�b�N�X
		int m_TargetCellIndex;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		@param[in]	CellMap	�Z���}�b�v
		*/
		//--------------------------------------------------------------------------------------
		CellSearchBehavior(const shared_ptr<T>& GameObjectPtr,
			const weak_ptr<StageCellMap>& CellMap) :
			ObjBehavior(GameObjectPtr),
			m_CelMap(CellMap)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CellSearchBehavior() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ړI�̏ꏊ���T�[�`����
		@param[in]	TargetPos	�ړI�̉ӏ�
		@return	�Z����Ɍ���������true
		*/
		//--------------------------------------------------------------------------------------
		bool Search(const Vector3& TargetPos) {
			auto MapPtr = m_CelMap.lock();
			if (MapPtr) {
				auto PathPtr = GetGameObject()->GetComponent<PathSearch>();
				m_CellPath.clear();
				//�p�X������������
				if (PathPtr->SearchCell(TargetPos, m_CellPath)) {
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ړI�̏ꏊ���T�[�`��Seek�s��������
		@param[in]	TargetPos	�ړI�̉ӏ�
		@return	Seek�ł����true
		*/
		//--------------------------------------------------------------------------------------
		CellSearchFlg SeekBehavior(const Vector3& TargetPos) {
			auto MyPos = GetGameObject()->GetComponent<Transform>()->GetPosition();
			auto MapPtr = m_CelMap.lock();
			if (Search(TargetPos)) {
				auto PtrSeek = GetGameObject()->GetBehavior<SeekSteering>();
				auto PtrArrive = GetGameObject()->GetBehavior<ArriveSteering>();
				if (m_NextCellIndex == 0) {
					auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>();
					PtrArrive->Execute(TargetPos);
					//��������
					return CellSearchFlg::Arrived;
				}
				else {
					AABB ret;
					MapPtr->FindAABB(m_CellPath[m_NextCellIndex], ret);
					auto Pos = ret.GetCenter();
					Pos.y = TargetPos.y;
					PtrSeek->Execute(Pos);
					return CellSearchFlg::Seek;
				}
			}
			else {
				auto PtrSeek = GetGameObject()->GetBehavior<SeekSteering>();
				CellIndex PlayerCell;
				if (MapPtr->FindCell(TargetPos, PlayerCell)) {
					AABB ret;
					MapPtr->FindAABB(PlayerCell, ret);
					auto Pos = ret.GetCenter();
					Pos.y = TargetPos.y;
					PtrSeek->Execute(Pos);
					return CellSearchFlg::Seek;
				}
			}
			return CellSearchFlg::Failed;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ړI�̏ꏊ��arrive�s�����Ƃ�
		@param[in]	TargetPos	�ړI�̉ӏ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ArriveBehavior(const Vector3& TargetPos) {
			auto PtrArrive = GetGameObject()->GetBehavior<ArriveSteering>();
			PtrArrive->Execute(TargetPos);
		}
	};


	//--------------------------------------------------------------------------------------
	///	�v���C���[�̍s���N���X
	//--------------------------------------------------------------------------------------
	class PlayerBehavior : public Behavior {
		//�ō����x
		float m_MaxSpeed;
		//����
		float m_Mass;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���g���[����������x�N�g���𓾂�
		@return	�����x�N�g��
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetMoveVector() const;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		PlayerBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr),
			m_MaxSpeed(10.0f),	//�ō����x
			m_Mass(0.5f)	//����
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PlayerBehavior() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ō����x�𓾂�
		@return	�ō����x
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxSpeed() const {
			return m_MaxSpeed;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���ʂ𓾂�
		@return	����
		*/
		//--------------------------------------------------------------------------------------
		float GetMass() const {
			return m_Mass;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���C���[���ړ�������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void MovePlayer();
	};




}

//end basecross

