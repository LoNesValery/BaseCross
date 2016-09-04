/*!
@file GameObject.h
@brief �R���|�[�l���g�A�Q�[���I�u�W�F�N�g
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	class GameObject;
	class Scene;

	//--------------------------------------------------------------------------------------
	///	�R���|�[�l���g�e�N���X
	//--------------------------------------------------------------------------------------
	class Component : public ObjectInterface, public ShapeInterface{
		weak_ptr<GameObject> m_GameObject;
	protected:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�R���X�g���N�^
		@param[in]	GameObjectPtr	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit Component(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Component();
	public:
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Q�[���I�u�W�F�N�g�̎擾
		@return	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetGameObject() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Q�[���I�u�W�F�N�g��ݒ肷��
		@param[in]	GameObjectPtr	�V�����Q�[���I�u�W�F�N�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetGameObject(const shared_ptr<GameObject>& GameObjectPtr);
	};

	//--------------------------------------------------------------------------------------
	///	�ϊ��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class Transform : public Component {
		//1�O�̕ϐ�
		Vector3 m_BeforeScale;
		Vector3 m_BeforePivot;
		Quaternion m_BeforeQuaternion;
		Vector3 m_BeforePosition;
		//���݂̕ϐ�
		Vector3 m_Scale;
		Vector3 m_Pivot;
		Quaternion m_Quaternion;
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit Transform(const shared_ptr<GameObject>& GameObjectPtr) :
			Component(GameObjectPtr),
			m_BeforeScale(1.0f, 1.0f, 1.0f),
			m_BeforePivot(0, 0, 0),
			m_BeforePosition(0, 0, 0),
			m_Scale(1.0f, 1.0f, 1.0f),
			m_Pivot(0, 0, 0),
			m_Position(0, 0, 0)
		{
			m_BeforeQuaternion.Identity();
			m_Quaternion.Identity();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Transform(){}
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1�O�̃X�P�[�����O�𓾂�
		@return	1�O�̃X�P�[�����O
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetBeforeScale() const{
			return m_BeforeScale;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1�O�̏d�S�𓾂�
		@return	1�O�̏d�S
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetBeforePivot() const{
			return m_BeforePivot;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1�O�̃��[���h��]�𓾂�
		@return	1�O�̃��[���h��]�i�N�I�[�^�j�I���j
		*/
		//--------------------------------------------------------------------------------------
		Quaternion GetBeforeQuaternion() const{
			return m_BeforeQuaternion;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1�O�̃��[���h�ʒu�𓾂�
		@return	1�O�̃��[���h�ʒu
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetBeforePosition() const{
			return m_BeforePosition;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1�O�̃��[���h�s��𓾂�
		@return	1�O�̃��[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		Matrix4X4 GetBeforeWorldMatrix() const{
			Matrix4X4 mat;
			mat.AffineTransformation(m_BeforeScale, m_BeforePivot, m_BeforeQuaternion, m_BeforePosition);
			return mat;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�P�[�����O�𓾂�
		@return	�X�P�[�����O
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetScale() const{
			return m_Scale;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�P�[�����O��ݒ肷��.
		@param	Scale	�X�P�[�����O
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetScale(const Vector3& Scale){
			m_Scale = Scale;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�d�S�𓾂�
		@return	�d�S
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetPivot() const{
			return m_Pivot;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�d�S��ݒ肷��.
		@param	Pivot	�d�S
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetPivot(const Vector3& Pivot){
			m_Pivot = Pivot;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���[���h��]�𓾂�
		@return	���[���h��]�i�N�I�[�^�j�I���j
		*/
		//--------------------------------------------------------------------------------------
		Quaternion GetQuaternion() const{
			return m_Quaternion;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���[���h��]��ݒ肷��
		@param[in]	quaternion	�N�I�[�^�j�I��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetQuaternion(const Quaternion& quaternion){
			m_Quaternion = quaternion;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���[���h�ʒu�𓾂�
		@return	���[���h�ʒu
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetPosition() const{
			return m_Position;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���[���h�ʒu��ݒ肷��
		@param[in]	Position	�ݒ肷��ʒu
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetPosition(const Vector3& Position){
			m_Position = Position;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	���[���h�s��𓾂�
		@return	���[���h�s��
		*/
		//--------------------------------------------------------------------------------------
		Matrix4X4 GetWorldMatrix() const{
			Matrix4X4 mat;
			mat.AffineTransformation(m_Scale, m_Pivot, m_Quaternion, m_Position);
			return mat;
		}


		//����
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�[�^��1�O�ɐݒ肷��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetToBefore(){
			m_BeforeScale = m_Scale;
			m_BeforePivot = m_Pivot;
			m_BeforeQuaternion = m_Quaternion;
			m_BeforePosition = m_Position;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���������s���i���z�֐��j<br />
		��this�|�C���^���K�v�ȃI�u�W�F�N�g�͂��̊֐��𑽏d��`���āA�擾�ł���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override{};

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�揈���B�f�t�H���g�͉����s��Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}
	};

	//--------------------------------------------------------------------------------------
	///	�d�̓R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class Gravity : public Component{
		Vector3 m_DefaultGravity;		//���R���������x
		Vector3 m_Gravity;				//���݂̏d�͉����x
		Vector3 m_GravityVelocity;		//�d�͉����x�ɂ�錻�݂̑��x
		float m_BaseY;	//�ŉ������n�_
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit Gravity(const shared_ptr<GameObject>& GameObjectPtr) :
			Component(GameObjectPtr),
			m_DefaultGravity(0.0f, -9.8f, 0.0f),
			m_Gravity(m_DefaultGravity),
			m_GravityVelocity(0, 0, 0),
			m_BaseY(0)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Gravity(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���R�����d�͂𓾂�
		@return	���R�����d��
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetDefaultGravity() const{
			return m_DefaultGravity;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���R�����d�͂�ݒ肷��
		@param[in]	gravity	���R�����d�͒l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetDefaultGravity(const Vector3& gravity){
			m_DefaultGravity = gravity;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̏d�͂𓾂�
		@return	���݂̏d��
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetGravity() const{
			return m_Gravity;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̏d�͂�ݒ肷��
		@param[in]	gravity	�d�͒l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetGravity(const Vector3& gravity){
			m_Gravity = gravity;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̏d�͂�0�ɐݒ肷��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetGravityZero(){
			m_Gravity = Vector3(0, 0, 0);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̏d�͂����R�����ɐݒ肷��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetGravityDefault(){
			m_Gravity = m_DefaultGravity;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�d�͂ɂ�錻�݂̑��x�𓾂�
		@return	�d�͂ɂ�錻�݂̑��x
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetGravityVelocity() const{
			return m_GravityVelocity;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�d�͂ɂ�錻�݂̑��x��ݒ肷��i�����ɏd�͂ɂ�鑬�x��ύX����ꍇ�Ɏg�p�j�B
		@param[in]	GravityVelocity	�d�͂ɂ�錻�݂̑��x
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetGravityVelocity(const Vector3& GravityVelocity){
			m_GravityVelocity = GravityVelocity;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�d�͂ɂ�鑬�x��0���ǂ����𓾂�
		@return	�d�͂ɂ�鑬�x��0���ǂ���
		*/
		//--------------------------------------------------------------------------------------
		bool IsGravityVelocityZero(){
			return (m_GravityVelocity == Vector3(0, 0, 0));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�d�͂ɂ�鑬�x��0�ɂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetGravityVelocityZero(){
			m_GravityVelocity = Vector3(0, 0, 0);

		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�I�u�W�F�N�g�̍ŉ��n�_�𓾂�
		@return	�I�u�W�F�N�g�̍ŉ��n�_
		*/
		//--------------------------------------------------------------------------------------
		float GetBaseY() const{
			return m_BaseY;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�I�u�W�F�N�g�̍ŉ��n�_��ݒ肷��
		@param[in]	y	�I�u�W�F�N�g�̍ŉ��n�_
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetBaseY(float y){
			m_BaseY = y;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�W�����v�̃X�^�[�g
		@param[in]	StartVec	�����x
		@param[in]	EscapeSpan	�W�����v�J�n�n�_�ւ̏㕔�����l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void StartJump(const Vector3& StartVec, float EscapeSpan = 0.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w�肵�����Ԃ����d�͂�������
		@param[in]	CalcTime	����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateFromTime(float CalcTime);
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���������s���i���z�֐��j<br />
		��this�|�C���^���K�v�ȃI�u�W�F�N�g�͂��̊֐��𑽏d��`���āA�擾�ł���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override{};
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`�揈���B�f�t�H���g�͉����s��Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override{}
	};

	//--------------------------------------------------------------------------------------
	///	PNTStaticDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PNTStaticDraw : public Component{

		//���b�V�����\�[�X
		weak_ptr<MeshResource> m_MeshResource;
		//�e�N�X�`��
		weak_ptr<TextureResource> m_TextureResource;
		bool m_OwnShadowActive;
		bool m_Trace;

		///���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		///GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		///�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		//�R���X�^���g�o�b�t�@�\����
		struct PNTStaticConstantBuffer
		{
			Matrix4X4 World;
			Matrix4X4 View;
			Matrix4X4 Projection;
			Vector4 LightDir;
			Color4 Emissive;
			Color4 Diffuse;
			PNTStaticConstantBuffer() {
				memset(this, 0, sizeof(PNTStaticConstantBuffer));
			};
		};
		///�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };
		///�p�C�v���C���X�e�[�g
		D3D12_GRAPHICS_PIPELINE_STATE_DESC m_PineLineDesc;

		ComPtr<ID3D12PipelineState> m_CullBackPipelineState;
		ComPtr<ID3D12PipelineState> m_CullFrontPipelineState;
		///�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

		///�e�������֐�
		///���[�g�V�O�l�`���쐬
		void CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		void CreateDescriptorHeap();
		///�T���v���[�쐬
		void CreateSampler();
		///�V�F�[�_�[���\�[�X�r���[�쐬
		void CreateShaderResourceView();
		///�R���X�^���g�o�b�t�@�쐬
		void CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		void CreatePipelineState();
		///�R�}���h���X�g�쐬
		void CreateCommandList();
		//�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer(PNTStaticConstantBuffer& Buff);
		///�`�揈��
		void DrawObject();

		///�v���p�e�B�ύX
		void RefreshTrace();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
			Component(GameObjectPtr),
			m_Trace(false)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTStaticDraw(){}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�������ǂ����̐ݒ�
		@param[in]	b	�������ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTrace(bool b);

		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V�����\�[�X�̎擾
		@return	���b�V�����\�[�X
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MeshResource> GetMeshResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V�����\�[�X�̓o�^
		@param[in]	MeshRes	���b�V�����\�[�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshResource(const shared_ptr<MeshResource>& MeshRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�N�X�`�����\�[�X�̐ݒ�
		@param[in]	TextureRes	�e�N�X�`�����\�[�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const shared_ptr<TextureResource>& TextureRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�N�X�`�����\�[�X�̎擾
		@return	�e�N�X�`�����\�[�X
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<TextureResource> GetTextureResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���������s���i���z�֐��j<br />
		��this�|�C���^���K�v�ȃI�u�W�F�N�g�͂��̊֐��𑽏d��`���āA�擾�ł���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`�揈���B
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};









	//--------------------------------------------------------------------------------------
	///	�Q�[���I�u�W�F�N�g�N���X
	//--------------------------------------------------------------------------------------
	class GameObject : public ObjectInterface, public ShapeInterface{
		//�V�[���̃|�C���^
		weak_ptr<Scene> m_Scene;
		//�R���|�[�l���g�̃}�b�v
		map<type_index, shared_ptr<Component> > m_CompMap;
		shared_ptr<Gravity> m_Gravity;	//Gravity�͕ʂɂ���
		shared_ptr<Transform> m_Transform;	//Transform���ʂɂ���
		list<type_index> m_CompOrder;	//�R���|�[�l���g���s����
		//�R���|�[�l���g��T��
		shared_ptr<Component> SearchComponent(type_index TypeIndex)const;
		//�쐬�����R���|�[�l���g���Z�b�g����
		void AddMakedComponent(type_index TypeIndex, const shared_ptr<Component>& Ptr);


	protected:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�R���X�g���N�^
		@param[in]	Scene	�V�[��
		*/
		//--------------------------------------------------------------------------------------
		explicit GameObject(const shared_ptr<Scene>& ScenePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameObject();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V�[���̎擾
		@return	�V�[���̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Scene> GetScene()const{
			if (m_Scene.expired()){
				throw BaseException(
					L"�V�[���������ł�",
					L"if (m_Scene.expired())",
					L"GameObject::GetScene()"
					);
			}
			else{
				return m_Scene.lock();
			}
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���|�[�l���g�̎擾
		@tparam	T	�擾����^
		@param[in]	ExceptionActive	�Ώۂ�null�������ꍇ�ɗ�O�������邩�ǂ���
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		shared_ptr<T> GetComponent(bool ExceptionActive = true)const{
			auto Ptr = SearchComponent(type_index(typeid(T)));
			if (Ptr) {
				//�w��̌^�̃R���|�[�l���g����������
				auto RetPtr = dynamic_pointer_cast<T>(Ptr);
				if (RetPtr){
					return RetPtr;
				}
				else{
					throw BaseException(
						L"�R���|�[�l���g������܂������A�^�L���X�g�ł��܂���",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetComponent<T>()"
						);
				}
			}
			else{
				if (ExceptionActive){
					throw BaseException(
						L"�R���|�[�l���g��������܂���",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetComponent<T>()"
						);
				}
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Gravity�R���|�[�l���g�̎擾
		@param[in]	ExceptionActive	�Ώۂ�null�������ꍇ�ɗ�O�������邩�ǂ���
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<Gravity> GetComponent<Gravity>(bool ExceptionActive)const{
			if (!m_Gravity){
				if (ExceptionActive){
					throw BaseException(
						L"�R���|�[�l���g��������܂���",
						L"Gravity",
						L"GameObject::GetComponent<Gravity>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return m_Gravity;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Transform�R���|�[�l���g�̎擾�i�G���[���͏�ɗ�O�j
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<Transform> GetComponent<Transform>(bool ExceptionActive)const {
			if (!m_Transform) {
				//��ɗ�O
				throw BaseException(
					L"�R���|�[�l���g��������܂���",
					L"Transform",
					L"GameObject::GetComponent<Transform>()"
					);
			}
			return m_Transform;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���|�[�l���g�̒ǉ��B�V�K�ɃR���|�[�l���g�N���X�����ꍇ�A�R���X�g���N�^�̑�1������GameObject�Ƃ��č쐬����B
		@tparam	T	�ǉ�����^
		@tparam	Ts	�ϒ��ϐ��̌^
		@param[in]	params	���̃R���|�[�l���g���\�z����̂Ɏg�p����p�����[�^�B�i��2�p�����[�^�ȍ~�j
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> AddComponent(Ts&&... params){
			auto Ptr = SearchComponent(type_index(typeid(T)));
			//�w��̌^�̃R���|�[�l���g����������
			if (Ptr){
				auto RetPtr = dynamic_pointer_cast<T>(Ptr);
				if (RetPtr){
					return RetPtr;
				}
				else{
					throw BaseException(
						L"���łɃR���|�[�l���g������܂������A�^�L���X�g�ł��܂���",
						Util::GetWSTypeName<T>(),
						L"GameObject::AddComponent<T>()"
						);
				}
			}
			else{
				//������Ȃ��B�V���ɍ쐬����
				shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>(), params...);
				AddMakedComponent(type_index(typeid(T)), newPtr);
				return newPtr;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Gravity�R���|�[�l���g�̒ǉ�
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<Gravity> AddComponent<Gravity>(){
			if (m_Gravity){
				return m_Gravity;
			}
			else{
				//������ΐV���ɐ��삷��
				m_Gravity = ObjectFactory::Create<Gravity>(GetThis<GameObject>());
				return m_Gravity;
			}
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Transform�R���|�[�l���g�̒ǉ�
		@return	�R���|�[�l���g
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<Transform> AddComponent<Transform>() {
			if (m_Transform) {
				return m_Transform;
			}
			else {
				//������ΐV���ɐ��삷��
				m_Transform = ObjectFactory::Create<Transform>(GetThis<GameObject>());
				return m_Transform;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���|�[�l���g�̏��������i�^�[���̐擪�ɌĂ΂��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ComponentPreparation();

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���|�[�l���g�̍X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ComponentUpdate();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���|�[�l���g�̕`�揈��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ComponentDraw();

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�O������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreCreate()override{
			//Transform�K�{
			AddComponent<Transform>();
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ŏI�X�V�i�Փ˔���Ȃǂ��`�F�b�N����j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnLastUpdate(){}


		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override{
			ComponentDraw();
		}

	};




}
// end basecross
