/*!
@file DrawObjects.h
@brief �`��I�u�W�F�N�g
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Scene;
	class Stage;
	class GameObject;

	//--------------------------------------------------------------------------------------
	///	PNT���_�`��Ɏg�p����\����
	//--------------------------------------------------------------------------------------
	struct DrawObject {
		shared_ptr<MeshResource> m_MeshRes;
		shared_ptr<TextureResource> m_TextureRes;
		Mat4x4 m_WorldMatrix;
		bool m_Trace;
		bool m_Wrap;
		DrawObject() :
			m_MeshRes(nullptr),
			m_TextureRes(nullptr),
			m_WorldMatrix(),
			m_Trace(false),
			m_Wrap(false)
		{}
	};


	//--------------------------------------------------------------------------------------
	///	PNT���_�I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	class PNTDrawObject : public GameObject {
		vector<DrawObject> m_DrawObjectVec;
		vector<DrawObject> m_TraceDrawObjectVec;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��T�u����
		@param[in]	ObjectVec	�`�悷��z��
		@param[in]	sb	�R���X�^���g�o�b�t�@
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void OnDrawSub(vector<DrawObject>& ObjectVec, SimpleConstants& sb);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		PNTDrawObject(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTDrawObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`�悷��I�u�W�F�N�g��ǉ�����
		@param[in]	MeshRes	���b�V��
		@param[in]	TextureRes �e�N�X�`��
		@param[in]	WorldMat ���[���h�s��
		@param[in]	Trace �����������邩�ǂ���
		@param[in]	Wrap ���b�s���O�������邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddDrawMesh(const shared_ptr<MeshResource>& MeshRes,
			const shared_ptr<TextureResource>& TextureRes,
			const Mat4x4& WorldMat,
			bool Trace, bool Wrap = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	//Particle�`��̃I�v�V����
	//--------------------------------------------------------------------------------------
	enum class ParticleDrawOption {
		Billboard,	//�r���{�[�h
		Faceing,	//�t�F�C�V���O
		FaceingY,	//�t�F�C�V���O(Y���̂�)
		Normal		//�ʏ�(�蓮�B��]�͐��K���̂ݍs��)
	};
	//--------------------------------------------------------------------------------------
	//ParticleDraw�����̂��߂̍\����
	//--------------------------------------------------------------------------------------
	struct DrawParticleSprite {
		//�p�[�e�B�N���̃J�����܂ł̋���
		float m_ToCaneraLength;
		//���[���h�s��
		Mat4x4 m_WorldMatrix;
		//�e�N�X�`��
		shared_ptr<TextureResource> m_TextureRes;
		//Diffuse�J���[
		Col4 m_Diffuse;
		DrawParticleSprite() :
			m_ToCaneraLength(0),
			m_Diffuse(1, 1, 1, 1)

		{}
	};

	class ParticleDrawObject;

	//--------------------------------------------------------------------------------------
	///	�p�[�e�B�N�����́A1�̃X�v���C�g������킷�\����
	//--------------------------------------------------------------------------------------
	struct ParticleSprite {
		bool m_Active;				//�A�N�e�B�u���ǂ���
		Col4 m_Color;				//�F�i����������ꍇ��a��0�ɂ���j
		Vec2 m_LocalScale;		//���[�J���X�P�[�����O�iZ��1.0�Œ�j		
		Quat m_LocalQt;		//���[�J����]�B�蓮�̂Ƃ��̂ݎg�p
		Vec3 m_LocalPos;			//���[�J���ʒu
		Vec3 m_Velocity;			//���x(�K�v�ɉ����Ďg�p)
		ParticleSprite() :
			m_Active(true),
			m_Color(1.0f, 1.0f, 1.0f, 1.0f),
			m_LocalScale(1.0f, 1.0f),
			m_LocalQt(),
			m_LocalPos(0, 0, 0),
			m_Velocity(0, 0, 0)
		{}
		void Reflesh() {
			m_Active = true;
			m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			m_LocalScale = Vec2(1.0f, 1.0f);
			m_LocalQt.identity();
			m_LocalPos = Vec3(0, 0, 0);
			m_Velocity = Vec3(0, 0, 0);
		}
	};

	//--------------------------------------------------------------------------------------
	///	�p�[�e�B�N��
	///	*1�̃G�~�b�^�[�������A������ParticleSprite��ێ�����
	//--------------------------------------------------------------------------------------
	struct Particle : public ObjectInterface {
		ParticleDrawOption m_DrawOption;		//�\���I�v�V����
		vector<ParticleSprite> m_ParticleSpriteVec;	//�ۑ����Ă����X�v���C�g�̔z��
		Vec3 m_EmitterPos;			//�G�~�b�^�[�ʒu
		float m_TotalTime;				//�^�C�}�[���䂷��ꍇ�Ɏg�p����ϐ�
		float m_MaxTime;				//���̃p�[�e�B�N���W���̂̕\������
		weak_ptr<TextureResource> m_TextureResource;	//�e�N�X�`��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	Count	�p�[�e�B�N���̐�
		@param[in]	Option	�`��I�v�V����
		*/
		//--------------------------------------------------------------------------------------
		explicit Particle(size_t Count, ParticleDrawOption Option = ParticleDrawOption::Billboard);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Particle();
		//����
		//--------------------------------------------------------------------------------------
		/*!
		@brief �g�[�^���^�C���̒ǉ�
		@param[in]	f	�ǉ��l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddTotalTime(float f) {
			m_TotalTime += f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �A�N�e�B�u���ǂ���
		@return	1�ł��A�N�e�B�u�������true
		*/
		//--------------------------------------------------------------------------------------
		bool IsActive() const {
			for (auto Psp : m_ParticleSpriteVec) {
				if (Psp.m_Active) {
					//1�ł��A�N�e�B�u�������true
					return true;
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���ׂăA�N�e�B�u���ǂ���
		@return	���ׂăA�N�e�B�u�Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsAllActive() const {
			for (auto Psp : m_ParticleSpriteVec) {
				if (!Psp.m_Active) {
					//1�ł���A�N�e�B�u�������false
					return false;
				}
			}
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���ׂăA�N�e�B�u�ɂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetAllActive() {
			for (auto Psp : m_ParticleSpriteVec) {
				Psp.m_Active = true;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���ׂĔ�A�N�e�B�u�ɂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetAllNoActive() {
			for (auto Psp : m_ParticleSpriteVec) {
				Psp.m_Active = false;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �l������������i�������ƕ`��I�v�V�����j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Reflesh() {
			m_EmitterPos = Vec3(0, 0, 0);
			m_TotalTime = 0;
			m_MaxTime = 0;
			m_ParticleSpriteVec.clear();
			for (auto Psp : m_ParticleSpriteVec) {
				Psp.Reflesh();
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �l������������
		@param[in]	Count	�V�����p�[�e�B�N���X�v���C�g��
		@param[in]	Option	�V�����`��I�v�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Reflesh(size_t Count, ParticleDrawOption Option) {
			m_DrawOption = Option;
			m_EmitterPos = Vec3(0, 0, 0);
			m_TotalTime = 0;
			m_MaxTime = 0;
			m_ParticleSpriteVec.clear();
			m_ParticleSpriteVec.resize(Count);
			for (auto Psp : m_ParticleSpriteVec) {
				Psp.Reflesh();
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �p�[�e�B�N���X�v���C�g�̔z��𓾂�
		@return	�p�[�e�B�N���X�v���C�g�̔z��
		*/
		//--------------------------------------------------------------------------------------
		vector<ParticleSprite>& GetParticleSpriteVec() {
			return m_ParticleSpriteVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`�����\�[�X�𓾂�
		@return	�e�N�X�`�����\�[�X��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<TextureResource> GetTextureResource(bool ExceptionActive) const {
			if (!m_TextureResource.expired()) {
				return m_TextureResource.lock();
			}
			else {
				if (ExceptionActive) {
					throw BaseException(
						L"�e�N�X�`�����\�[�X��������܂���",
						L"if (pImpl->m_Texture.expired())",
						L"Particle::GetTextureResource()"
					);
				}
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`�����\�[�X��ݒ肷��
		@param[in]	ResKey	�e�N�X�`�����\�[�X��
		@return�@�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const wstring& ResKey) {
			try {
				if (ResKey == L"") {
					throw BaseException(
						L"�e�N�X�`���L�[���󔒂ł�",
						L"if (ResKey == L\"\"",
						L"Particle::SetTextureResource()"
					);
				}
				m_TextureResource = App::GetApp()->GetResource<TextureResource>(ResKey);
			}
			catch (...) {
				throw;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`�����\�[�X��ݒ肷��
		@param[in]	TextureResourcePtr	�e�N�X�`�����\�[�X
		@return�@�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr) {
			m_TextureResource = TextureResourcePtr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Draw(const shared_ptr<ParticleDrawObject>& Manager);
	};


	//--------------------------------------------------------------------------------------
	///	�}���`�p�[�e�B�N��(�p�[�e�B�N���G�t�F�N�g)
	//--------------------------------------------------------------------------------------
	class MultiParticle : public GameObject {
		vector< shared_ptr<Particle> > m_ParticleVec;
		//���Z�������邩�ǂ���
		bool m_AddType;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		explicit MultiParticle(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MultiParticle();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�p�[�e�B�N���̔z��𓾂�
		@return	�p�[�e�B�N���̔z��
		*/
		//--------------------------------------------------------------------------------------
		vector< shared_ptr<Particle> >& GetParticleVec() {
			return m_ParticleVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Z�`�悷�邩�ǂ����𓾂�
		@return	���Z�`�悷��Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetAddType() const {
			return m_AddType;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Z�`�悷�邩�ǂ����𓾂�
		@return	���Z�`�悷��Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsAddType() const {
			return m_AddType;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Z�`�悷�邩�ǂ�����ݒ肷��
		@param[in]	b	���Z�`�悷�邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetAddType(bool b) {
			m_AddType = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �p�[�e�B�N���̒ǉ�
		@param[in]	Count	�p�[�e�B�N���̐�
		@param[in]	Option	�`��I�v�V����
		@return	�ǉ����ꂽ�p�[�e�B�N��
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Particle> InsertParticle(size_t Count, ParticleDrawOption Option = ParticleDrawOption::Billboard);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`�揈��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	//Instance�`��̂��߂̍\����
	//--------------------------------------------------------------------------------------
	struct InstanceDrawStr {
		size_t Start;
		size_t Count;
		shared_ptr<TextureResource> Tex;
		Col4 m_Diffuse;
		InstanceDrawStr(size_t s, size_t c, shared_ptr<TextureResource> t,
			const Col4& Diffuse) :
			Start(s), Count(c), Tex(t), m_Diffuse(Diffuse) {}
	};

	//--------------------------------------------------------------------------------------
	///	�p�[�e�B�N���`��I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class ParticleDrawObject : public GameObject {
		//���b�V�����\�[�X
		shared_ptr<MeshResource> m_MeshResource;
		//ParticleDraw�����̂��߂̍\���̂̔z��
		vector<DrawParticleSprite> m_DrawParticleSpriteVec;
		//�C���X�^���X�`��̍\���̂̔z��
		vector<InstanceDrawStr> m_InstancVec;
		//�C���X�^���X�ő�l
		const size_t m_MaxInstance;
		//�s��p�̃o�b�t�@
		ComPtr<ID3D11Buffer> m_MatrixBuffer;
		//Z�o�b�t�@���g�p���邩�ǂ���
		bool m_ZBufferUse;
		//�T���v���[�̃��b�s���O���邩�ǂ���
		bool m_SamplerWrap;
		//���Z�������邩�ǂ���
		bool m_AddType;
		//���_�o�b�t�@�̍쐬
		void CreateParticleBuffers();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	AddType	���Z�`�悷�邩�ǂ���
		*/
		//--------------------------------------------------------------------------------------
		explicit ParticleDrawObject(const shared_ptr<Stage>& StagePtr, bool AddType);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ParticleDrawObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ����������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Z�o�b�t�@���g�p���邩�ǂ����𓾂�
		@return	Z�o�b�t�@���g�p����Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsZBufferUse() const {
			return m_ZBufferUse;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Z�o�b�t�@���g�p���邩�ǂ�����ݒ肷��
		@param[in]	b	Z�o�b�t�@���g�p���邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetZBufferUse(bool b) {
			m_ZBufferUse = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�s���O�T���v���[���ǂ����𓾂�
		@return���b�s���O�T���v���[�Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsSamplerWrap() const {
			return m_SamplerWrap;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�s���O�T���v���[��ݒ肷��
		@param[in]	b	���b�s���O�T���v���[�ɂ��邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetSamplerWrap(bool b) {
			m_SamplerWrap = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`��̂��߂̃p�[�e�B�N���X�v���C�g��ǉ�����
		@param[in]	rParticleSprite	�p�[�e�B�N���X�v���C�g
		@param[in]	Option	�`��I�v�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddParticle(const ParticleSprite& rParticleSprite, ParticleDrawOption Option,
			const bsm::Vec3& EmitterPos, const shared_ptr<TextureResource>& TextureRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`�揈��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};



}
//end basecross
