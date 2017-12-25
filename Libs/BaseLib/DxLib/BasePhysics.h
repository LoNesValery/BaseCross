/*!
@file BasePhysics.h
@brief �����v�Z�C���^�[�t�@�C�X
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	���[�V�����^�C�v
	//--------------------------------------------------------------------------------------
	enum PsMotionType {
		MotionTypeFixed = 0,
		MotionTypeActive,
		MotionTypeKeyframe,
		MotionTypeOneWay,
		MotionTypeTrigger,
		MotionTypeCount
	};

	struct PsParamBase {
		PsMotionType m_MotionType;
		bsm::Quat m_Quat;
		bsm::Vec3 m_Pos;
		bsm::Vec3 m_LinearVelocity;
		bsm::Vec3 m_AngularVelocity;
		bool m_UseSleep;
		float m_Mass;
		PsParamBase() :
			m_Quat(),
			m_Pos(0),
			m_LinearVelocity(0),
			m_AngularVelocity(0),
			m_UseSleep(true),
			m_Mass(1.0f)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	�{�b�N�X�쐬�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct PsBoxParam : public PsParamBase {
		bsm::Vec3 m_HalfSize;
		PsBoxParam():
			m_HalfSize(1.0f)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	���̍쐬�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct PsSphereParam : public PsParamBase {
		float m_Radius;
		PsSphereParam():
			m_Radius(1.0f)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	�J�v�Z���쐬�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct PsCapsuleParam : public PsParamBase {
		float m_HalfLen;
		float m_Radius;
		PsCapsuleParam() :
			m_HalfLen(0.5f),
			m_Radius(0.5f)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	�V�����_�[�쐬�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct PsCylinderParam : public PsParamBase {
		float m_HalfLen;
		float m_Radius;
		PsCylinderParam() :
			m_HalfLen(0.5f),
			m_Radius(0.5f)
		{}
	};


	//--------------------------------------------------------------------------------------
	///	���̂̃X�e�[�^�X(�擾�p)
	//--------------------------------------------------------------------------------------
	struct PsBodyStatus {
		bsm::Vec3	m_Position;
		bsm::Quat	m_Orientation;
		bsm::Vec3	m_LinearVelocity;
		bsm::Vec3	m_AngularVelocity;
	};

	//--------------------------------------------------------------------------------------
	///	���̂̃X�e�[�^�X(�ݒ�p)
	//--------------------------------------------------------------------------------------
	struct PsBodyUpdateStatus {
		bsm::Vec3	m_Force;
		bsm::Vec3	m_Torque;
		bsm::Vec3	m_LinearVelocity;
		bsm::Vec3	m_AngularVelocity;
		PsBodyUpdateStatus():
			m_Force(0.0f),
			m_Torque(0.0f),
			m_LinearVelocity(0.0f),
			m_AngularVelocity(0.0f)
		{}
		PsBodyUpdateStatus(const PsBodyStatus& src) :
			m_Force(0.0f),
			m_Torque(0.0f),
			m_LinearVelocity(src.m_LinearVelocity),
			m_AngularVelocity(src.m_AngularVelocity)
		{}
	};



	//--------------------------------------------------------------------------------------
	///	�����I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	class PhysicsObject : public ObjectInterface {
	protected:
		/// ���̃C���f�b�N�X
		uint16_t m_Index;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		explicit PhysicsObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ����������Status�̐ݒ�
		@param[in]	param	�C���f�b�N�X
		*/
		//--------------------------------------------------------------------------------------
		void SetParamStatus(const PsParamBase& param);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̃C���f�b�N�X�̎擾
		*/
		//--------------------------------------------------------------------------------------
		uint16_t GetIndex() const {
			return m_Index;
		}
	};

	//--------------------------------------------------------------------------------------
	///	�{�b�N�X�����I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class PhysicsBox : public PhysicsObject {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	param	�쐬�̃p�����[�^
		@param[in]	index	�C���f�b�N�X
		*/
		//--------------------------------------------------------------------------------------
		explicit PhysicsBox(const PsBoxParam& param, uint16_t index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsBox();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	������
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���������̃p�����[�^�𓾂�
		@return	���������̃p�����[�^
		*/
		//--------------------------------------------------------------------------------------
		const PsBoxParam& GetParam() const;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	���̕����I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class PhysicsSphere : public PhysicsObject {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	param	�쐬�̃p�����[�^
		@param[in]	index	�C���f�b�N�X
		*/
		//--------------------------------------------------------------------------------------
		explicit PhysicsSphere(const PsSphereParam& param,uint16_t index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsSphere();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	������
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���������̃p�����[�^�𓾂�
		@return	���������̃p�����[�^
		*/
		//--------------------------------------------------------------------------------------
		const PsSphereParam& GetParam() const;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	�J�v�Z�������I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class PhysicsCapsule : public PhysicsObject {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	param	�쐬�̃p�����[�^
		@param[in]	index	�C���f�b�N�X
		*/
		//--------------------------------------------------------------------------------------
		explicit PhysicsCapsule(const PsCapsuleParam& param, uint16_t index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsCapsule();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	������
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���������̃p�����[�^�𓾂�
		@return	���������̃p�����[�^
		*/
		//--------------------------------------------------------------------------------------
		const PsCapsuleParam& GetParam() const;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	�V�����_�[�����I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class PhysicsCylinder : public PhysicsObject {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	param	�쐬�̃p�����[�^
		@param[in]	index	�C���f�b�N�X
		*/
		//--------------------------------------------------------------------------------------
		explicit PhysicsCylinder(const PsCylinderParam& param, uint16_t index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsCylinder();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	������
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���������̃p�����[�^�𓾂�
		@return	���������̃p�����[�^
		*/
		//--------------------------------------------------------------------------------------
		const PsCylinderParam& GetParam() const;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	///	�����v�Z�p�̃C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	class BasePhysics{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		explicit BasePhysics();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BasePhysics();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Update();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�������i���ׂẴI�u�W�F�N�g�������Ȃ�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Reset();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�P�̂̃{�b�N�X�̒ǉ�
		@param[in]	param	�쐬�̃p�����[�^
		@param[in]	index	�C���f�b�N�X�i�I�u�W�F�N�g���ė��p����ꍇ�j
		@return	�I�u�W�F�N�g�̃|�C���^�i�o�b�N�A�b�v�͂��Ȃ��̂ŌĂяo�����ŕۑ����邱�Ɓj
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsBox> AddSingleBox(const PsBoxParam& param, uint16_t index = UINT16_MAX);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�P�̂̋��̂̒ǉ�
		@param[in]	param	�쐬�̃p�����[�^
		@param[in]	index	�C���f�b�N�X�i�I�u�W�F�N�g���ė��p����ꍇ�j
		@return	�I�u�W�F�N�g�̃|�C���^�i�o�b�N�A�b�v�͂��Ȃ��̂ŌĂяo�����ŕۑ����邱�Ɓj
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsSphere> AddSingleSphere(const PsSphereParam& param,uint16_t index = UINT16_MAX);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�P�̂̃J�v�Z���̒ǉ�
		@param[in]	param	�쐬�̃p�����[�^
		@param[in]	index	�C���f�b�N�X�i�I�u�W�F�N�g���ė��p����ꍇ�j
		@return	�I�u�W�F�N�g�̃|�C���^�i�o�b�N�A�b�v�͂��Ȃ��̂ŌĂяo�����ŕۑ����邱�Ɓj
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsCapsule> AddSingleCapsule(const PsCapsuleParam& param, uint16_t index = UINT16_MAX);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�P�̂̃V�����_�[�̒ǉ�<br />
		�V�����_�[�́A�{�b�N�X�̉�����̗������ɕs�������̂ŁA���p�͐T�d�ɁI
		@param[in]	param	�쐬�̃p�����[�^
		@param[in]	index	�C���f�b�N�X�i�I�u�W�F�N�g���ė��p����ꍇ�j
		@return	�I�u�W�F�N�g�̃|�C���^�i�o�b�N�A�b�v�͂��Ȃ��̂ŌĂяo�����ŕۑ����邱�Ɓj
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsCylinder> AddSingleCylinder(const PsCylinderParam& param, uint16_t index = UINT16_MAX);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃{�f�B���𓾂�
		@return	���݂̃{�f�B��
		*/
		//--------------------------------------------------------------------------------------
		uint16_t GetNumBodies() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̂̃X�e�[�^�X�𓾂�
		@param[in]	body_index	�{�f�BID
		@param[out]	st	�X�e�[�^�X�𓾂�Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetBodyStatus(uint16_t body_index, PsBodyStatus& st) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̂̃X�e�[�^�X��ݒ肷��
		@param[in]	body_index	�{�f�BID
		@param[out]	st	�X�e�[�^�X�𓾂�Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetBodyStatus(uint16_t body_index,const PsBodyUpdateStatus& st);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̂��N�����isleep���L���̏ꍇ�j
		@param[in]	body_index	�{�f�BID
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void WakeUpBody(uint16_t body_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̂̈ʒu��ݒ肷��iPsMotionType��MotionTypeTrigger�̂Ƃ��̂ݗL���j
		@param[in]	body_index	�{�f�BID
		@param[out]	pos	�ʒu
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetBodyPosition(uint16_t body_index, const bsm::Vec3& pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̂̑��x��ݒ肷��
		@param[in]	body_index	�{�f�BID
		@param[out]	v	���x
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetBodyLinearVelocity(uint16_t body_index, const bsm::Vec3& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̂̉�]���x��ݒ肷��
		@param[in]	body_index	�{�f�BID
		@param[out]	v	���x
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetBodyAngularVelocity(uint16_t body_index, const bsm::Vec3& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̂̃t�H�[�X��ǉ�����
		@param[in]	body_index	�{�f�BID
		@param[out]	v	�t�H�[�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ApplyBodyForce(uint16_t body_index, const bsm::Vec3& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̂̃g���N��ǉ�����
		@param[in]	body_index	�{�f�BID
		@param[out]	v	�g���N
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ApplyBodyTorque(uint16_t body_index, const bsm::Vec3& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`��̐��𓾂�
		@param[in]	body_index	�{�f�BID
		@return	�`��
		*/
		//--------------------------------------------------------------------------------------
		uint16_t GetNumShapes(uint16_t body_index)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`��̃I�t�Z�b�g��]�ƃI�t�Z�b�g�ʒu�𓾂�
		@param[in]	body_index	�{�f�BID
		@param[in]	shape_index	�`��ID
		@param[out]	qt	��]���󂯂�Q��
		@param[out]	pos	�ʒu���󂯂�Q��
		@return�@�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetShapeOffsetQuatPos(uint16_t body_index, uint16_t shape_index, bsm::Quat& qt, bsm::Vec3& pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`��̃^�C�v�𓾂�
		@param[in]	body_index	�{�f�BID
		@param[in]	shape_index	�`��ID
		@return�@�`��̃^�C�v
		*/
		//--------------------------------------------------------------------------------------
		sce::PhysicsEffects::ePfxShapeType GetShapeType(uint16_t body_index, uint16_t shape_index) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�W���C���g�̐��𓾂�
		@return	�W���C���g��
		*/
		//--------------------------------------------------------------------------------------
		uint16_t GetNumJoints()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���^�N�g(�Փ�)�̐��𓾂�
		@return	�W���C���g��
		*/
		//--------------------------------------------------------------------------------------
		uint16_t GetNumContacts()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w��̃{�f�B���R���^�N�g���Ă��鑊��̔z��𓾂�
		@param[in]	body_index	�{�f�BID
		@param[out]	contacts	�����ID�̔z��
		@return	�Փ˂������true
		*/
		//--------------------------------------------------------------------------------------
		bool GetContactsVec(uint16_t body_index, vector<uint16_t>& contacts)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w��̃{�f�B���R���^�N�g���Ă��鑊���set�W���𓾂�
		@param[in]	body_index	�{�f�BID
		@param[out]	contacts	�����ID�̏W��
		@return	�Փ˂������true
		*/
		//--------------------------------------------------------------------------------------
		bool GetContactsSet(uint16_t body_index, set<uint16_t>& contacts)const;
		//�ȉ��A���ڃA�N�Z�X(�g�p����ꍇ�͐T�d��)
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`��𓾂�
		@param[in]	body_index	�{�f�BID
		@param[in]	shape_index	�`��ID
		@return�@�`��̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxShape& getPfxShape(uint16_t body_index, uint16_t shape_index) const;
		sce::PhysicsEffects::PfxShape& getPfxShape(uint16_t body_index, uint16_t shape_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̂̃X�e�[�g�𓾂�
		@param[in]	body_index	�{�f�BID
		@return�@���̂̃X�e�[�g�̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxRigidState& getPfxRigidState(uint16_t body_index) const;
		sce::PhysicsEffects::PfxRigidState& getPfxRigidState(uint16_t body_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̂𓾂�
		@param[in]	body_index	�{�f�BID
		@return�@���̂̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxRigidBody& getPfxRigidBody(uint16_t body_index) const;
		sce::PhysicsEffects::PfxRigidBody& getPfxRigidBody(uint16_t body_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Փ˂��鍄�̂𓾂�
		@param[in]	body_index	�{�f�BID
		@return�@�Փ˂��鍄�̂̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxCollidable& getPfxCollidable(uint16_t body_index) const;
		sce::PhysicsEffects::PfxCollidable& getPfxCollidable(uint16_t body_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̂̃\���o�[�𓾂�
		@param[in]	body_index	�{�f�BID
		@return�@���̂̃\���o�[�̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxSolverBody& getPfxSolverBody(uint16_t body_index) const;
		sce::PhysicsEffects::PfxSolverBody& getPfxSolverBody(uint16_t body_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�W���C���g�̃y�A�𓾂�
		@param[in]	joint_index	�W���C���gID
		@return�@�W���C���g�̃y�A�̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxConstraintPair& getPfxConstraintPair(uint16_t joint_index) const;
		sce::PhysicsEffects::PfxConstraintPair& getPfxConstraintPair(uint16_t joint_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�W���C���g�𓾂�
		@param[in]	joint_index	�W���C���gID
		@return�@�W���C���g�̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxJoint& getPfxJoint(uint16_t joint_index) const;
		sce::PhysicsEffects::PfxJoint& getPfxJoint(uint16_t joint_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̃u���[�h�t�F�C�Y�y�A�𓾂�
		@param[in]	contact_index	�R���^�N�gID
		@return�@���̃u���[�h�t�F�C�Y�y�A�̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxBroadphasePair& getNowPfxBroadphasePair(uint16_t contact_index) const;
		sce::PhysicsEffects::PfxBroadphasePair& getNowPfxBroadphasePair(uint16_t contact_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1�O�̃u���[�h�t�F�C�Y�y�A�𓾂�
		@param[in]	contact_index	�R���^�N�gID
		@return�@1�O�̃u���[�h�t�F�C�Y�y�A�̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxBroadphasePair& getPrevPfxBroadphasePair(uint16_t contact_index) const;
		sce::PhysicsEffects::PfxBroadphasePair& getPrevPfxBroadphasePair(uint16_t contact_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���^�N�g�̃}�j�z�[���h�𓾂�
		@param[in]	contact_index	�R���^�N�gID
		@return�@�R���^�N�g�̃}�j�z�[���h�̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const sce::PhysicsEffects::PfxContactManifold& getPfxContactManifold(uint16_t contact_index) const;
		sce::PhysicsEffects::PfxContactManifold& getPfxContactManifold(uint16_t contact_index);

	};



}
//end basecross
