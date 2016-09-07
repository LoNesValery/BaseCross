/*!
@file DrawComponents.h
@brief �`��R���|�[�l���g
*/
#pragma once
#include "stdafx.h"

namespace basecross {


	//Shadow
	struct ShadowConstantBuffer
	{
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProj;
		XMVECTOR Bones[3 * 72];	//Bone�p
		ShadowConstantBuffer() {
			memset(this, 0, sizeof(ShadowConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBShadow, ShadowConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmap, VertexPositionNormalTexture)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmapBone, VertexPositionNormalTextureSkinning)


	//PNTStatic
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

	DECLARE_DX11_CONSTANT_BUFFER(CBPNTStatic, PNTStaticConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNTStatic, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStatic)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticNoTexture)


	struct PNTStaticShadowConstantBuffer
	{
		Matrix4X4 World;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Color4 Emissive;
		Color4 Diffuse;
		Vector4 LightPos;
		Vector4 EyePos;
		XMUINT4 ActiveFlg;			//�e�N�X�`��=x���A�N�e�B�u���ǂ���
		Matrix4X4 LightView;
		Matrix4X4 LightProjection;
		PNTStaticShadowConstantBuffer() {
			memset(this, 0, sizeof(PNTStaticShadowConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBPNTStaticShadow, PNTStaticShadowConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNTStaticShadow, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow2)




	//--------------------------------------------------------------------------------------
	///	�`��p�R���e�N�X�g�i�e�I�u�W�F�N�g�̕`��N���X�j<br />��{�I�ȃp�����[�^�ŏ��������`��p���[�e�B���e�B�N���X
	//--------------------------------------------------------------------------------------
	class DrawContext : public ObjectInterface {
		void SetVertexShader(ID3D11VertexShader* pVShader, bool SetConstantBuffer);
		bool IsVertexShaderSetConstantBuffer() const;
		void SetPixelShader(ID3D11PixelShader* pPShader, bool SetConstantBuffer);
		bool IsPixelShaderSetConstantBuffer() const;
		void SetInputLayout(ID3D11InputLayout* pInputLayout);
		void DrawIndexedBase(const shared_ptr<GameObject>& GameObjectPtr,
			ID3D11Buffer* pConstantBuffer, const void* CBData,
			const shared_ptr<MeshResource>& Mesh, UINT stride, UINT offset);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		DrawContext();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DrawContext();
		//���z�֐�
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�\�z�O�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreCreate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�\�z�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�u�����h�X�e�[�g�̐ݒ�
		@param[in]	State	�u�����h�X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetBlendState(BlendState State);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�v�X�X�e���V���X�e�[�g�̐ݒ�
		@param[in]	State	�f�v�X�X�e���V���X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetDepthStencilState(DepthStencilState State);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���X�^���C�U�X�e�[�g�̐ݒ�
		@param[in]	State	���X�^���C�U�X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetRasterizerState(RasterizerState State);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�p�C�v���C���ݒ�l�̈ꊇ�ݒ�
		@param[in]	Desc	�p�C�v���C���ݒ�l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetPipeLineDesc(const PipeLineDesc& Desc);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�p�C�v���C���ݒ�l�̎擾
		@return	�p�C�v���C���ݒ�l
		*/
		//--------------------------------------------------------------------------------------
		const PipeLineDesc& GetPipeLineDesc()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���_�V�F�[�_�ƃR���X�^���g�o�b�t�@�̐ݒ�
		@tparam	VS	���_�V�F�[�_�^
		@param[in]	SetConstantBuffe	�R���X�^���g�o�b�t�@��ݒ肷�邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename VS>
		void SetVertexShader(bool SetConstantBuffer) {
			SetVertexShader(VS::GetPtr()->GetShader(), SetConstantBuffer);
			SetInputLayout(VS::GetPtr()->GetInputLayout());
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s�N�Z���V�F�[�_�ƃR���X�^���g�o�b�t�@�̐ݒ�
		@tparam	PS	�s�N�Z���V�F�[�_�^
		@param[in]	SetConstantBuffe	�R���X�^���g�o�b�t�@��ݒ肷�邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename PS>
		void SetPixelShader(bool SetConstantBuffer) {
			SetPixelShader(PS::GetPtr()->GetShader(), SetConstantBuffer);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�T���v���[�ƃV�F�[�_���\�[�X�r���[�Z�b�g�̐ݒ�
		@param[in]	PilelineNum	�p�C�v���C����ID
		@param[in]	state	�T���v���[�X�e�[�g
		@param[in]	Srv	�V�F�[�_���\�[�X�r���[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddSamplerAndSrv(UINT PilelineNum, SamplerState state, ID3D11ShaderResourceView* Srv);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C���f�b�N�X�`��
		@tparam	CBType	�R���X�^���g�o�b�t�@�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		@param[in]	CBData	�R���X�^���g�o�b�t�@�f�[�^
		@param[in]	Mesh	���b�V��
		@param[in]	stride	�X�g���C�h
		@param[in]	offset	�I�t�Z�b�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename CBType>
		void DrawIndexed(const shared_ptr<GameObject>& GameObjectPtr,
			const void* CBData, const shared_ptr<MeshResource>& Mesh, UINT stride, UINT offset) {
			DrawIndexedBase(GameObjectPtr,
				CBType::GetPtr()->GetBuffer(), CBData,
				Mesh, stride, offset);
		}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




	class GameObject;

	//--------------------------------------------------------------------------------------
	///	�`��R���|�[�l���g�̐e�N���X
	//--------------------------------------------------------------------------------------
	class DrawComponent : public Component {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		DrawComponent(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DrawComponent();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�u�����h�X�e�[�g�̎擾
		@return	�u�����h�X�e�[�g
		*/
		//--------------------------------------------------------------------------------------
		BlendState GetBlendState() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�v�X�X�e���V���X�e�[�g�̎擾
		@return	�f�v�X�X�e���V���X�e�[�g
		*/
		//--------------------------------------------------------------------------------------
		DepthStencilState GetDepthStencilState() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���X�^���C�U�X�e�[�g�̎擾
		@return	���X�^���C�U�X�e�[�g
		*/
		//--------------------------------------------------------------------------------------
		RasterizerState GetRasterizerState() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�T���v���[�X�e�[�g�̎擾
		@return	�T���v���[�X�e�[�g
		*/
		//--------------------------------------------------------------------------------------
		SamplerState GetSamplerState() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�u�����h�X�e�[�g�̐ݒ�
		@param[in]	state	�u�����h�X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetBlendState(const BlendState state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�v�X�X�e���V���X�e�[�g�̐ݒ�
		@param[in]	state	�f�v�X�X�e���V���X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetDepthStencilState(const DepthStencilState state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���X�^���C�U�X�e�[�g�̐ݒ�
		@param[in]	state	���X�^���C�U�X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetRasterizerState(const RasterizerState state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�T���v���[�X�e�[�g�̐ݒ�
		@param[in]	state	�T���v���[�X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetSamplerState(const SamplerState state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V���ƃ��[���h�s��̊Ԃ�⊮����s��̎擾<br />
		���b�V���̃T�C�Y������A���S�̈ʒu���Q�[���I�u�W�F�N�g�̃��[���h�s��ƍ��ق�����ꍇ�A<br />
		���̊Ԃ�⊮����s���ݒ�ł���B
		@return	���b�V���ƃ��[���h�s��̊Ԃ�⊮����s��
		*/
		//--------------------------------------------------------------------------------------
		const Matrix4X4& GetMeshToTransformMatrix() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V���ƃ��[���h�s��̊Ԃ�⊮����s��̎擾<br />
		���b�V���̃T�C�Y������A���S�̈ʒu���Q�[���I�u�W�F�N�g�̃��[���h�s��ƍ��ق�����ꍇ�A<br />
		���̊Ԃ�⊮����s���ݒ�ł���B
		@param[in]	Mat	�ݒ肷��s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshToTransformMatrix(const Matrix4X4& Mat);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�I�u�W�F�N�g���ƂɃ{�[�����������Ă����|�C���^<br />
		�V���h�E�}�b�v�Ȃǂ���Q�Ƃł���悤�ɉ��z�֐��ɂ���<br />
		�h���N���X�Ń{�[������������ꍇ�͑��d��`����
		@return	�{�[���s��̔z��̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		virtual const vector< Matrix4X4 >* GetVecLocalBonesPtr() const { return nullptr; }
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	�V���h�E�}�b�v�R���|�[�l���g�i�O�����p�j
	//--------------------------------------------------------------------------------------
	class Shadowmap : public DrawComponent {
	public:
		explicit Shadowmap(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~Shadowmap();
		//�A�N�Z�T
		static float GetLightHeight();
		static float GetLightNear();
		static float GetLightFar();
		static float GetViewWidth();
		static float GetViewHeight();

		static void SetLightHeight(float f);
		static void SetLightNear(float f);
		static void SetLightFar(float f);
		static void SetViewWidth(float f);
		static void SetViewHeight(float f);
		static void SetViewSize(float f);


		shared_ptr<MeshResource> GetMeshResource(bool ExceptionActive = true) const;
		void SetMeshResource(const wstring& ResKey);
		void SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr);


		//����
		virtual void OnUpdate()override {}
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	///	PNTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PNTStaticDraw : public DrawComponent {
		void DrawNotShadow();
		void DrawWithShadow();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTStaticDraw();
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
		@brief	���b�V�����\�[�X�̓o�^
		@param[in]	MeshKey	�o�^����Ă��郁�b�V���L�[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshResource(const wstring& MeshKey);
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
		@brief	�e�N�X�`�����\�[�X�̐ݒ�
		@param[in]	TextureKey	�o�^����Ă���e�N�X�`���L�[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const wstring& TextureKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�N�X�`�����\�[�X�̎擾
		@return	�e�N�X�`�����\�[�X
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<TextureResource> GetTextureResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�G�~�b�V�u�F�̎擾
		@return	�G�~�b�V�u�F
		*/
		//--------------------------------------------------------------------------------------
		Color4 GetEmissive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�G�~�b�V�u�F�̐ݒ�
		@param[in]	col	�G�~�b�V�u�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetEmissive(const Color4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e��`�悷�邩�ǂ�������
		@return	�e��`�悷���true
		*/
		//--------------------------------------------------------------------------------------
		bool GetOwnShadowActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e��`�悷�邩�ǂ�������
		@return	�e��`�悷���true
		*/
		//--------------------------------------------------------------------------------------
		bool IsOwnShadowActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e��`�悷�邩�ǂ����ݒ肷��
		@param[in]	b		�e��`�悷�邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetOwnShadowActive(bool b);
		//����
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


}
//end basecross
