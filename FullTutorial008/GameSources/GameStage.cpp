/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	// �ʃJ����
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MyCamera::MyCamera() :
		Camera(),
		m_ToTargetLerp(1.0f)
	{
	}
	MyCamera::~MyCamera() {}
	//�A�N�Z�T
	shared_ptr<GameObject> MyCamera::GetTargetObject() const {
		if (!m_TargetObject.expired()) {
			return m_TargetObject.lock();
		}
		return nullptr;
	}

	void MyCamera::SetTargetObject(const shared_ptr<GameObject>& Obj) {
		m_TargetObject = Obj;
	}

	float MyCamera::GetToTargetLerp() const {
		return m_ToTargetLerp;
	}
	void MyCamera::SetToTargetLerp(float f) {
		m_ToTargetLerp = f;
	}

	void MyCamera::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		if (CntlVec[0].bConnected) {
			Vec3 NewAt(0, 0, 0);
			auto TargetPtr = GetTargetObject();
			if (TargetPtr) {
				//�ڎw�������ꏊ
				Vec3 ToAt = TargetPtr->GetComponent<Transform>()->GetPosition();
				NewAt = Lerp::CalculateLerp(GetAt(), ToAt, 0, 1.0f, m_ToTargetLerp, Lerp::Linear);
			}
			//�X�e�b�v1�A�����_�ƈʒu�̕ύX
			Vec3 Span = GetAt() - GetEye();
			Vec3 NewEye = NewAt - Span;
			SetAt(NewAt);
			SetEye(NewEye);
			//�X�e�b�v�Q�A�Y�[���̕ύX
			//�J�����ʒu�ƒ����_�̊Ԃ̃x�N�g�����Z�o
			Span = GetAt() - GetEye();
			//���K��
			Span.normalize();
			//�ω��l�̌���
			Span = Span * ElapsedTime * 10.0f;

			Vec3 NewArm = GetAt() - GetEye();
			//D�p�b�h��
			//�J����������
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				//�J�����ʒu������
				NewEye = NewEye - Span;
				NewArm = NewAt - NewEye;
				if (NewArm.length() > (GetFar() * 0.1f)) {
					NewEye = NewEye + Span;
					NewArm = NewAt - NewEye;
				}
			}
			//D�p�b�h��
			//�J���������
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				//�J�����ʒu�����
				NewEye = NewEye + Span;
				NewArm = NewAt - NewEye;
				if (NewArm.length() < GetNear() * 2.0f) {
					NewEye = NewEye - Span;
					NewArm = NewAt - NewEye;
				}
			}
			SetAt(NewAt);
			SetEye(NewEye);
			//�X�e�b�v3�p�x�̕ύX
			//���݂�At��Eye�̊p�x�𓾂�
			Vec3 ArmInv = GetEye() - GetAt();
			//�E�X�e�B�b�NX����
			FLOAT AngleY = 0;
			//�E�X�e�B�b�NY����
			FLOAT AngleX = 0;
			FLOAT AngleZ = 0;
			if (CntlVec[0].fThumbRX != 0) {
				//�E�X�e�B�b�N�������Ȃ��悤�ɂ���
			}
			if (CntlVec[0].fThumbRY != 0) {
				//�E�X�e�B�b�N�������Ȃ��悤�ɂ���
			}
			if (ArmInv.z > 0) {
				AngleX *= -1.0f;
			}
			if (ArmInv.x < 0) {
				AngleZ *= -1.0f;
			}
			Quat QtSpan;
			QtSpan.rotationRollPitchYawFromVector(Vec3(AngleX, AngleY, AngleZ));
			QtSpan.normalize();
			//��]��v�Z�̍s����쐬
			Mat4x4 Mat, Mat2;
			Mat.strTransformation(
				Vec3(1.0f, 1.0f, 1.0f),
				ArmInv,
				QtSpan);
			Mat2.translation(GetAt());
			Mat *= Mat2;
			NewEye = Mat.transInMatrix();
			if (NewEye.y < 0.5f) {
				NewEye.y = 0.5f;
			}
			//�J���������ȏ�A�ォ�王���ɂȂ�Ȃ悤�ɒ���
			ArmInv = NewEye - GetAt();
			ArmInv.normalize();
			float y2 = ArmInv.y * ArmInv.y;
			float x2 = ArmInv.x * ArmInv.x;
			float z2 = ArmInv.z * ArmInv.z;
			if (y2 <= (x2 + z2)) {
				SetEye(NewEye);
			}

		}
		Camera::OnUpdate();
	}


	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------


	//���\�[�X�̍쐬
	void GameStage::CreateResourses() {
		wstring DataDir;
		//�T���v���̂��߃A�Z�b�g�f�B���N�g�����擾
		App::GetApp()->GetAssetsDirectory(DataDir);
		//�e�Q�[���͈ȉ��̂悤�Ƀf�[�^�f�B���N�g�����擾���ׂ�
		//App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
	}

	//�r���[�ƃ��C�g�̍쐬
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PttCamera = ObjectFactory::Create<MyCamera>();
		//LookAtCamera�ɂ���ꍇ�͈ȉ��R�����g���O��
//		auto PttCamera = ObjectFactory::Create<LookAtCamera>();

		PtrView->SetCamera(PttCamera);
		PttCamera->SetEye(Vec3(0.0f, 2.0f, -8.0f));
		PttCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}


	//�v���[�g�̍쐬
	void GameStage::CreatePlate() {
		//�X�e�[�W�ւ̃Q�[���I�u�W�F�N�g�̒ǉ�
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(50.0f, 50.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);

		auto ColPtr = Ptr->AddComponent<CollisionRect>();
		//�`��R���|�[�l���g�̒ǉ�
		auto DrawComp = Ptr->AddComponent<BcPNTStaticDraw>();
		DrawComp->SetFogEnabled(true);
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//�����ɉe���f�肱�ނ悤�ɂ���
		DrawComp->SetOwnShadowActive(true);

		//�`��R���|�[�l���g�e�N�X�`���̐ݒ�
		DrawComp->SetTextureResource(L"SKY_TX");
	}


	//�Œ�̃{�b�N�X�̍쐬
	void GameStage::CreateFixedBox() {
		//�z��̏�����
		vector< vector<Vec3> > Vec = {
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(10.0f, 0.25f, 0.0f)
			},
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-10.0f, 0.25f, 0.0f)
			},
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 0.25f, 2.0f)
			},
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-5.0f, 0.25f, 2.0f)
			},

			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(10.0f, 0.25f, 4.0f)
			},
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-10.0f, 0.25f, 4.0f)
			},
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 0.25f, 6.0f)
			},
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-5.0f, 0.25f, 6.0f)
			},

			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(10.0f, 0.25f, 8.0f)
			},
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-10.0f, 0.25f, 8.0f)
			},

		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		wstring DataDir;
		//�T���v���̂��߃A�Z�b�g�f�B���N�g�����擾
		App::GetApp()->GetAssetsDirectory(DataDir);
		//�e�Q�[���͈ȉ��̂悤�Ƀf�[�^�f�B���N�g�����擾���ׂ�
		//App::GetApp()->GetDataDirectory(DataDir);


		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>(DataDir + L"character_template_sample1\\");
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	//Egg�̍쐬
	void GameStage::CreateEgg() {
		wstring DataDir;
		//�T���v���̂��߃A�Z�b�g�f�B���N�g�����擾
		App::GetApp()->GetAssetsDirectory(DataDir);
		//�e�Q�[���͈ȉ��̂悤�Ƀf�[�^�f�B���N�g�����擾���ׂ�
		//App::GetApp()->GetDataDirectory(DataDir);

		AddGameObject<EggAnime>(DataDir + L"character_template_sample1\\");
	}

	void GameStage::OnCreate() {
		try {
			//���\�[�X�̍쐬
			CreateResourses();
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�v���[�g�̍쐬
			CreatePlate();
			//�Œ�̃{�b�N�X�̍쐬
			CreateFixedBox();
			//�v���[���[�̍쐬
			CreatePlayer();
			//Egg�̍쐬
			CreateEgg();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
