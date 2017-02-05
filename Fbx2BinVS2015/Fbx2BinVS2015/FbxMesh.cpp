/*!
@file FbxMesh.cpp
@brief FbxMesh�֘A����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	struct VertexParams {
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//���_�o�b�t�@
		ComPtr<ID3D11Buffer> m_IndexBuffer;	//�C���f�b�N�X�o�b�t�@
		UINT m_NumVertices;				//���_�̐�
		UINT m_NumIndicis;				//�C���f�b�N�X�̐�
	};

	//--------------------------------------------------------------------------------------
	//	Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct FbxMeshResource2::Impl {
		unique_ptr<FbxMesh, FbxMeshDeleter> m_FbxMesh;
		//FBX�V�[��
		weak_ptr<FbxSceneResource> m_FbxSceneResource;
		//FBX���i�󔒂̏ꍇ����j
		string m_FbxName;
		//�ȉ��A�X�L�����b�V���p
		//�X�L�����b�V���̏ꍇnullptr�ȊO�ɂȂ�
		unique_ptr<FbxSkin, FbxSkinDeleter> m_FbxSkin;
		//�P�t���[���̊�ƂȂ鎞��
		FbxTime m_timePeriod;
		//�X�L���A�j���[�V�����Ɏg�p����{�[��
		//�{�[���̔z��
		vector< Bone > m_vecBones;
		//�{�[���𖼑O�ŏƉ�邷��ۂɎg�p����C���f�b�N�X�e�[�u��
		map< string, UINT > m_mapBoneList;
		//�X�L�����b�V���ł������I�ɃX�^�e�B�b�N�œǂނ��ǂ���
		bool m_NeedStatic;
		Impl(shared_ptr<FbxSceneResource> FbxSceneResourcePtr, FbxMesh* pFbxMesh, bool NeedStatic) :
			m_FbxSceneResource(FbxSceneResourcePtr),
			m_FbxSkin(nullptr),
			m_timePeriod(0),
			m_FbxMesh(pFbxMesh),
			m_NeedStatic(NeedStatic)
		{}
		~Impl() {
		}
	};
	//--------------------------------------------------------------------------------------
	///	FBX���b�V�����\�[�X(MeshResource�h����)
	//--------------------------------------------------------------------------------------
	FbxMeshResource2::FbxMeshResource2(shared_ptr<FbxSceneResource> FbxSceneResourcePtr, FbxMesh* pFbxMesh, bool NeedStatic):
		MeshResource(),
		pImpl(new Impl(FbxSceneResourcePtr, pFbxMesh, NeedStatic))
	{}
	FbxMeshResource2::~FbxMeshResource2() {}

	void FbxMeshResource2::OnCreate() {
		try {
			//�X�L�����b�V�����ǂ����𔻒f
			pImpl->m_FbxSkin.reset(FbxCast< FbxSkin >(pImpl->m_FbxMesh->GetDeformer(0, FbxDeformer::eSkin)));
			//�X�L�����b�V���̏ꍇ
			if (pImpl->m_FbxSkin && !pImpl->m_NeedStatic) {
				CreateInstanceFromSkinFbx();
			}
			//�X�^�e�B�b�N���b�V���������͋����I�ɃX�^�e�B�b�N�œǂޏꍇ
			else {
				pImpl->m_FbxSkin = nullptr;
				CreateInstanceFromStaticFbx();
			}
		}
		catch (...) {
			throw;
		}
	}



	void FbxMeshResource2::GetMaterialVec(vector<MaterialEx>& materials) {

		DWORD MaterialCount = pImpl->m_FbxMesh->GetNode()->GetMaterialCount();

		//�}�e���A���̐ݒ�
		//�e�N�X�`���t�@�C�����쐬�̂��߂̃��[�N�z��
		wchar_t Buff[MAX_PATH];
		setlocale(LC_CTYPE, "jpn");

		if (pImpl->m_FbxSceneResource.expired()) {
			//���s����
			throw BaseException(
				L"FbxSceneResource�������ł�",
				L"if (pImpl->m_FbxSceneResource.expired())",
				L"FbxMeshResource::GetMaterialVec()");

		}
		auto FbxSceneResourcePtr = pImpl->m_FbxSceneResource.lock();

		for (DWORD i = 0; i < MaterialCount; i++) {
			//�}�e���A���擾
			MaterialEx material;
			::ZeroMemory(&material, sizeof(MaterialEx));

			FbxSurfaceMaterial*			pMaterial = pImpl->m_FbxMesh->GetNode()->GetMaterial(i);
			FbxSurfacePhong*			pPhong = (FbxSurfacePhong*)pMaterial;
			FbxPropertyT<FbxDouble3>	color;
			color = pPhong->Ambient;
			//�A���r�G���g�͎g�p���Ȃ�
			material.m_Ambient = Color4(0.5f, 0.5f, 0.5f, 1.0f);
			//			material.m_Ambient = Color4((float)color.Get()[ 0 ], (float)color.Get()[ 1 ], (float)color.Get()[ 2 ], 1.0f);
			color = pPhong->Diffuse;
			//�f�t�B�[�Y�̓e�N�X�`�����g���̂Ńt��
			material.m_Diffuse = Color4((float)color.Get()[0], (float)color.Get()[1], (float)color.Get()[2], 1.0f);
			//			material.m_Specular = Color4(0.0f,0.0f,0.0f,0.4f);
			material.m_Specular = Color4((float)color.Get()[0] * 0.5f, (float)color.Get()[1] * 0.5f, (float)color.Get()[2] * 0.5f, 0.4f);
			color = pPhong->Emissive;
			//�G�~�b�V�u��DirectXTK�f�t�H���g
			//material.m_Emissive = Color4(0.05333332f,0.09882354f,0.1819608f,1.0f);
			material.m_Emissive = Color4((float)color.Get()[0], (float)color.Get()[1], (float)color.Get()[2], 1.0f);

			//�}�e���A���Ɋ֘A�t�����Ă���e�N�X�`����ǂݍ���
			const FbxProperty	fbxProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
			//�e�N�X�`������t�@�C�����𔲂��o��
			FbxFileTexture*	pFbxFileTexture = fbxProperty.GetSrcObject< FbxFileTexture >(i);
			TextureResource* pTexture = 0;
			if (pFbxFileTexture) {
				//�e�N�X�`���t�@�C��������p�X��r�����t�@�C����+�g���q�Ƃ��č�������
				char szTextureFilename[256], szFileExt[8];
				_splitpath_s(pFbxFileTexture->GetFileName(), nullptr, 0, nullptr, 0, szTextureFilename, 256, szFileExt, 8);
				//�t�@�C�����̍���
				string sTextureFilename(szTextureFilename);
				sTextureFilename += szFileExt;
				//UNI�R�[�h�ɕϊ�
				size_t size = 0;
				mbstowcs_s(&size, Buff, sTextureFilename.c_str(), MAX_PATH - 1);
				wstring strWork = FbxSceneResourcePtr->GetDataDir() + Buff;
				//pFbxFileTexture���烉�b�v���[�h���擾���ăe�N�X�`�����쐬
				auto PtrTexture = TextureResource::CreateTextureResource(strWork.c_str());
				material.m_TextureResource = PtrTexture;
			}
			else {
				//�e�N�X�`�����Ȃ��ꍇ��nullptr��ݒ�
				material.m_TextureResource = nullptr;
			}
			//�}�e���A���z��ɒǉ�
			materials.push_back(material);
		}
	}


	void FbxMeshResource2::GetStaticVerticesIndicesMaterials(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices,
		vector<MaterialEx>& materials) {
		vertices.clear();
		indices.clear();
		materials.clear();
		GetMaterialVec(materials);
		auto MaterialCount = materials.size();
		UINT NumVertices = 0;
		//���b�V���P�̂̓ǂݍ���
		DWORD dwNumPolygons = 0;	//�|���S����
									//���_���Ȃ�	
		if ((NumVertices = pImpl->m_FbxMesh->GetControlPointsCount()) <= 0) {
			//���s����
			throw BaseException(L"Fbx�ɒ��_������܂���",
				L"m_FbxMesh->GetControlPointsCount() <= 0",
				L"FbxMeshResource::GetStaticVerticesIndicesMaterials()");
		}
		//�|���S�����̎擾
		dwNumPolygons = pImpl->m_FbxMesh->GetPolygonCount();
		//���_���쐬���邽�߂̔z��
		vertices.resize(NumVertices);
		FbxStringList sUVSetNames;
		pImpl->m_FbxMesh->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		//���_���W�E�@���E�e�N�X�`�����W�̎擾
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//�|���S���̃T�C�Y�𓾂�i�ʏ�R�j
			const DWORD dwPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = pImpl->m_FbxMesh->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				//Fbx���璸�_�𓾂�
				vPos = pImpl->m_FbxMesh->GetControlPointAt(iIndex);
				//�@���𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexNormal(i, j, vNormal);
				//UV�l�𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				vertices[iIndex] =
					VertexPositionNormalTexture(
						//���_�̐ݒ�
						//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
						XMFLOAT3(static_cast< float >(vPos[0]), static_cast< float >(vPos[1]), -static_cast< float >(vPos[2])),
						//�@���̐ݒ�
						//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
//						XMFLOAT3(static_cast< float >(-vNormal[0]), -static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
						XMFLOAT3(static_cast< float >(vNormal[0]), static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
						//UV�l�̐ݒ�
						//V�̒l���A1.0����������l�ɂȂ�
						XMFLOAT2(static_cast< float >(vUV[0]), 1.0f - static_cast< float >(vUV[1]))
					);

				int tangentCount = pImpl->m_FbxMesh->GetElementTangentCount();
				int binormalCount = pImpl->m_FbxMesh->GetElementBinormalCount();

			}
		}
		//�C���f�b�N�X
		//�}�e���A���̃|�C���^���擾����
		const FbxLayerElementMaterial*	fbxMaterial = pImpl->m_FbxMesh->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			//���_�C���f�b�N�X���œK������(�����}�e���A�����g�p����|���S�����܂Ƃ߂ĕ`��ł���悤�ɕ��ׁA
			//�`�掞�Ƀ}�e���A���̐؂�ւ��񐔂����炷)
			for (DWORD j = 0; j < dwNumPolygons; j++) {
				DWORD	dwMaterialId = fbxMaterial->GetIndexArray().GetAt(j);
				if (dwMaterialId == i) {
					int iPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(j);
					for (int k = 0; k < iPolygonSize; k++) {
						indices.push_back(static_cast< uint16_t >(pImpl->m_FbxMesh->GetPolygonVertex(j, 2 - k)));
						materials[i].m_IndexCount++;
					}
				}
			}
		}
		//�}�e���A���z��ɃX�^�[�g�n�_��ݒ�
		UINT StarIndex = 0;
		for (DWORD i = 0; i < materials.size(); i++) {
			materials[i].m_StartIndex = StarIndex;
			StarIndex += materials[i].m_IndexCount;
		}
	}

	void FbxMeshResource2::CreateInstanceFromStaticFbx() {
		//����FBX�ɖ��O������΂����ێ�
		if (pImpl->m_FbxMesh->GetName()) {
			pImpl->m_FbxName = pImpl->m_FbxMesh->GetName();
		}
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		GetStaticVerticesIndicesMaterials(vertices, indices,GetMaterialExVec());
		//�z������Ƃɒ��_�ƃC���f�b�N�X���쐬
		ComPtr<ID3D11Buffer> VertexBuffer;	//���_�o�b�t�@
		ComPtr<ID3D11Buffer> IndexBuffer;	//�C���f�b�N�X�o�b�t�@
		MeshResource::CreateVertexBuffer(VertexBuffer, vertices);
		MeshResource::CreateIndexBuffer(IndexBuffer, indices);
		SetVertexBuffer(VertexBuffer);
		SetIndexBuffer(IndexBuffer);
		SetNumVertices(vertices.size());
		SetNumIndicis(indices.size());
		SetVertexType<VertexPositionNormalTexture>();
	}

	void FbxMeshResource2::GetSkinVerticesIndicesMaterials(vector<VertexPositionNormalTextureSkinning>& vertices,
		vector<uint16_t>& indices, vector<MaterialEx>& materials,
		vector<Bone>& Bones, map< string, UINT >& mapBoneList) {
		vertices.clear();
		indices.clear();
		materials.clear();
		Bones.clear();
		mapBoneList.clear();
		GetMaterialVec(materials);
		auto MaterialCount = materials.size();
		UINT NumVertices = 0;
		//���b�V���P�̂̓ǂݍ���
		DWORD dwNumPolygons = 0;	//�|���S����
		//���_���Ȃ�	
		if ((NumVertices = pImpl->m_FbxMesh->GetControlPointsCount()) <= 0) {
			//���s����
			throw BaseException(L"Fbx�ɒ��_������܂���",
				L"m_pFbxMesh->GetControlPointsCount() <= 0",
				L"FbxMeshResource::GetSkinVerticesIndicesMaterials()");
		}
		//�|���S�����̎擾
		dwNumPolygons = pImpl->m_FbxMesh->GetPolygonCount();

		//���_���쐬���邽�߂̔z��
		vertices.resize(NumVertices);
		FbxStringList sUVSetNames;
		pImpl->m_FbxMesh->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		//���_���W�E�@���E�e�N�X�`�����W�̎擾
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//�|���S���̃T�C�Y�𓾂�i�ʏ�R�j
			const DWORD dwPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = pImpl->m_FbxMesh->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				//Fbx���璸�_�𓾂�
				vPos = pImpl->m_FbxMesh->GetControlPointAt(iIndex);
				//�@���𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexNormal(i, j, vNormal);
				//UV�l�𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				uint32_t temp[4] = { 0, 0, 0, 0 };
				float tempf[4] = { 0, 0, 0, 0 };
				vertices[iIndex] =
					VertexPositionNormalTextureSkinning(
						//���_�̐ݒ�
						//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
						XMFLOAT3(static_cast< float >(vPos[0]), static_cast< float >(vPos[1]), -static_cast< float >(vPos[2])),
						//�@���̐ݒ�
						//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
						XMFLOAT3(static_cast< float >(vNormal[0]), static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
						//UV�l�̐ݒ�
						//V�̒l���A1.0����������l�ɂȂ�
						XMFLOAT2(static_cast< float >(vUV[0]), 1.0f - static_cast< float >(vUV[1])),
						//�u�����h�C���f�b�N�X�͂Ƃ肠����0
						temp,
						//�u�����h�E�G�C�g�͂Ƃ肠����0
						tempf
					);
			}
		}

		//�u�����h�E�F�C�g�ƃu�����h�C���f�b�N�X�̓ǂݍ���
		const int	iNumCluster = pImpl->m_FbxSkin->GetClusterCount();
		// �ϊ����� FbxSkin ���� �N���X�^�[(�{�[��)�̐����擾����

		for (int i = 0; i < iNumCluster; i++) {
			int		iNumBlendIndices = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointIndicesCount();	// i �Ԗڂ̃N���X�^�[�ɉe�����󂯂钸�_�̐����擾����
			int*	piBlendIndices = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointIndices();		// i �Ԗڂ̃N���X�^�[�ɉe�����󂯂钸�_�̔ԍ���z��Ŏ擾����
			double*	pdBlendWeights = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointWeights();		// i �Ԗڂ̃N���X�^�[�ɉe�����󂯂钸�_�ɑΉ������d�݃f�[�^��z��Ŏ擾����

			for (int j = 0; j < iNumBlendIndices; j++) {
				int	idx = piBlendIndices[j];

				//���_�ɓo�^����S�̃u�����h�E�F�C�g�̂����A�ŏ��̒l�����v�f����������iDirectX9�̌Œ�V�F�[�_�ł͂S�̃{�[�����炵���u�����h�ł��Ȃ��j
				int	iMinIndex = 0;
				for (int k = 0; k < 4 - 1; k++) {
					for (int l = k + 1; l < 4; l++) {
						if (vertices[idx].weights[k] < vertices[idx].weights[l]) {
							iMinIndex = k;
						}
						else {
							iMinIndex = l;
							k = l;
							break;
						}
					}
				}
				//���łɓo�^����Ă��钆�ōŏ��̃u�����h�E�F�C�g�����傫���l�����f�[�^��o�^����
				if (static_cast< float >(pdBlendWeights[j]) > vertices[idx].weights[iMinIndex]) {
					vertices[idx].indices[iMinIndex] = static_cast< BYTE >(i);
					vertices[idx].weights[iMinIndex] = static_cast< float >(pdBlendWeights[j]);
				}
			}
		}
		//�E�G�C�g�̃`�F�b�N
		//�e���_���ƂɃE�F�C�g�𑫂���1.0f�ɂȂ�Ȃ��ƃX�L�������܂��ł��Ȃ�
		for (size_t i = 0; i < vertices.size(); i++) {
			float f = vertices[i].weights[0] + vertices[i].weights[1] + vertices[i].weights[2] + vertices[i].weights[3];
			if (f > 0 && f < 1.0f) {
				float k = 1.0f / f;
				vertices[i].weights[0] *= k;
				vertices[i].weights[1] *= k;
				vertices[i].weights[2] *= k;
				vertices[i].weights[3] *= k;
			}
		}

		//��^�C�}�[�̐ݒ�
		FbxGlobalSettings&	globalTimeSettings = pImpl->m_FbxMesh->GetScene()->GetGlobalSettings();
		FbxTime::EMode timeMode = globalTimeSettings.GetTimeMode();
		pImpl->m_timePeriod.SetTime(0, 0, 0, 1, 0, timeMode);

		//�C���f�b�N�X�o�b�t�@�̍쐬
		//�}�e���A���̃|�C���^���擾����
		const FbxLayerElementMaterial*	fbxMaterial = pImpl->m_FbxMesh->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			//���_�C���f�b�N�X���œK������(�����}�e���A�����g�p����|���S�����܂Ƃ߂ĕ`��ł���悤�ɕ��ׁA
			//�`�掞�Ƀ}�e���A���̐؂�ւ��񐔂����炷)
			for (DWORD j = 0; j < dwNumPolygons; j++) {
				DWORD	dwMaterialId = fbxMaterial->GetIndexArray().GetAt(j);
				if (dwMaterialId == i) {
					int iPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(j);
					for (int k = 0; k < iPolygonSize; k++) {
						indices.push_back(static_cast< uint16_t >(pImpl->m_FbxMesh->GetPolygonVertex(j, 2 - k)));
						materials[i].m_IndexCount++;
					}
				}
			}
		}
		//�}�e���A���z��ɃX�^�[�g�n�_��ݒ�
		UINT StarIndex = 0;
		for (DWORD i = 0; i < materials.size(); i++) {
			materials[i].m_StartIndex = StarIndex;
			StarIndex += materials[i].m_IndexCount;
		}

		//�{�[�����𓾂�
		auto NumBones = (UINT)pImpl->m_FbxSkin->GetClusterCount();
		for (UINT i = 0; i < NumBones; i++) {
			Bone	bone;

			FbxAMatrix	mBindPose, mCurrentPose;
			pImpl->m_FbxSkin->GetCluster(i)->GetTransformLinkMatrix(mBindPose);
			mCurrentPose = pImpl->m_FbxSkin->GetCluster(i)->GetLink()->EvaluateGlobalTransform(pImpl->m_timePeriod * 0);

			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					bone.m_BindPose(r, c) = static_cast< float >(mBindPose.Get(r, c));
					bone.m_CurrentPose(r, c) = static_cast< float >(mCurrentPose.Get(r, c));
				}
			}

			Matrix4X4	mMirror, mBindInverse;
			mMirror.Identity();
			mMirror(2, 2) = -1.0f;

			bone.m_BindPose *= mMirror;
			bone.m_CurrentPose *= mMirror;
			Vector4 temp4;
			mBindInverse = Matrix4X4EX::Inverse(&temp4, bone.m_BindPose);
			bone.m_ConbinedPose = mBindInverse * bone.m_CurrentPose;

			Bones.push_back(bone);
			//�}�b�v�̓o�^
			mapBoneList[pImpl->m_FbxSkin->GetCluster(i)->GetName()] = i;
		}
	}

	void FbxMeshResource2::CreateInstanceFromSkinFbx() {
		//����FBX�ɖ��O������΂����ێ�
		if (pImpl->m_FbxMesh->GetName()) {
			pImpl->m_FbxName = pImpl->m_FbxMesh->GetName();
		}
		vector<VertexPositionNormalTextureSkinning> vertices;
		vector<uint16_t> indices;
		GetSkinVerticesIndicesMaterials(vertices, indices, GetMaterialExVec(),
			pImpl->m_vecBones,pImpl->m_mapBoneList);

		//�z������Ƃɒ��_�ƃC���f�b�N�X���쐬
		ComPtr<ID3D11Buffer> VertexBuffer;	//���_�o�b�t�@
		ComPtr<ID3D11Buffer> IndexBuffer;	//�C���f�b�N�X�o�b�t�@
		MeshResource::CreateVertexBuffer(VertexBuffer, vertices);
		MeshResource::CreateIndexBuffer(IndexBuffer, indices);
		SetVertexBuffer(VertexBuffer);
		SetIndexBuffer(IndexBuffer);
		SetNumVertices(vertices.size());
		SetNumIndicis(indices.size());
		SetVertexType<VertexPositionNormalTextureSkinning>();

	}
	shared_ptr<FbxMeshResource2> FbxMeshResource2::CreateFbxMeshResource(shared_ptr<FbxSceneResource> FbxSceneResourcePtr,
		FbxMesh* pFbxMesh, bool NeedStatic) {
		return ObjectFactory::Create<FbxMeshResource2>(FbxSceneResourcePtr, pFbxMesh, NeedStatic);
	}

	bool FbxMeshResource2::IsSkining() const {
		return (pImpl->m_FbxSkin && !pImpl->m_NeedStatic);
	}

	const unique_ptr<FbxMesh, FbxMeshResource2::FbxMeshDeleter>& FbxMeshResource2::GetFbxMesh()const {
		return pImpl->m_FbxMesh;
	}
	const unique_ptr<FbxSkin, FbxMeshResource2::FbxSkinDeleter>& FbxMeshResource2::GetFbxSkin() const {
		return pImpl->m_FbxSkin;
	}

	const vector<Bone>& FbxMeshResource2::GetBonesVec() const {
		return pImpl->m_vecBones;
	}


	//
	bool FbxMeshResource2::GenerateCurrentPose(vector<Bone>& tgtBones, AnimationData& rAnimationData, float CurrentTime) {
		//�o�ߎ��Ԃ���t���[�������߂�
		float fCurrentFrame = CurrentTime * rAnimationData.m_SamplesParSecond;
		//�A�j���[�V�������Ō�܂ł����Ă邩�ǂ������m�F
		bool ret = false;
		if (!rAnimationData.m_IsLoop && ((UINT)fCurrentFrame >= rAnimationData.m_SampleLength)) {
			//���[�v���Ȃ��ŁA�A�j�����Ō�ɓ��B������
			ret = true;
		}
		int iAnimationFrame = static_cast< int >(fCurrentFrame * 10000.0f);

		if (rAnimationData.m_SampleLength > 0) {
			if (rAnimationData.m_IsLoop) {
				//���[�v����
				iAnimationFrame %= rAnimationData.m_SampleLength * 10000;
				iAnimationFrame += rAnimationData.m_StartSample * 10000;
			}
			else {
				//���[�v���Ȃ�
				if (ret) {
					//�Ō�̃t���[���Ƀ|�[�Y��ݒ肷��
					iAnimationFrame = (rAnimationData.m_StartSample + rAnimationData.m_SampleLength) * 10000;
				}
				else {
					iAnimationFrame += rAnimationData.m_StartSample * 10000;
				}
			}
		}
		FbxAMatrix	mCurrentPose;
		Matrix4X4	mMirror, mBindInverse;
		mMirror.Identity();
		mMirror(2, 2) = -1.0f;	//���W�n�̊֌W�Ń{�[�������]���Ă���̂ŁA�����DirectX�ɍ��킹�邽�߂Ɏg�p����s��

								//�J�����g�s��ƍ����s����Čv�Z����
		for (UINT i = 0; i < GetNumBones(); i++) {
			mCurrentPose = pImpl->m_FbxSkin->GetCluster(i)->GetLink()->EvaluateGlobalTransform((pImpl->m_timePeriod * iAnimationFrame) / 10000);
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					tgtBones[i].m_CurrentPose(r, c) = static_cast< float >(mCurrentPose.Get(r, c));
				}
			}
			tgtBones[i].m_CurrentPose *= mMirror;

			Vector4 temp;
			mBindInverse = Matrix4X4EX::Inverse(&temp, tgtBones[i].m_BindPose);

			tgtBones[i].m_ConbinedPose = mBindInverse * tgtBones[i].m_CurrentPose;
		}
		rAnimationData.m_IsAnimeEnd = ret;
		return ret;
	}


	//--------------------------------------------------------------------------------------
	//	Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct FbxSceneResource::Impl {
		//�f�[�^�f�B���N�g����
		wstring m_DataDir;
		//FBX�t�@�C����
		wstring m_FileName;
		//FBX�V�[����
		string m_FbxSceneName;
		//FBX�V�[��
		shared_ptr<FbxScene> m_FbxScene;
		//�����I��static�œǂނ��ǂ���
		bool m_NeedStatic;
		//�P���FBX���b�V���̔z��
		vector< shared_ptr<FbxMeshResource2> > m_FbxMeshResourceVec;
		Impl(const wstring& DataDir,
			const wstring& FileName, const string& SceneName, bool  NeedStatic):
			m_DataDir(DataDir),
			m_FileName(FileName),
			m_FbxSceneName(SceneName),
			m_NeedStatic(NeedStatic)
		{}
		~Impl() {}

	};

	//--------------------------------------------------------------------------------------
	///	FBX�V�[�����\�[�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FbxSceneResource::FbxSceneResource(const wstring& DataDir,
		const wstring& FileName, const string& SceneName, bool NeedStatic):
		BaseResource(),
		pImpl(new Impl(DataDir, FileName, SceneName, NeedStatic))
	{
		try {
			if (FileName == L"") {
				//���s����
				throw BaseException(
					L"Fbx�t�@�C�����������ł�",
					L"if(FileName == \"\")",
					L"FbxSceneResource::FbxSceneResource()");
			}

			if (pImpl->m_FbxSceneName == "") {
				if (FileName == L"") {
					//���s����
					throw BaseException(
						L"�t�@�C�����������ł�",
						L"if(FileName == \"\")",
						L"FbxSceneResource::FbxSceneResource()");
				}
				wstring strWork;
				strWork = pImpl->m_FileName;
				//�t�@�C�������}���`�o�C�g�ɕϊ����V�[�������쐬
				Util::WStoMB(strWork, pImpl->m_FbxSceneName);
				pImpl->m_FbxSceneName += ".scene";
			}
		}
		catch (...) {
			throw;
		}
	}
	FbxSceneResource::~FbxSceneResource() {}

	//static�\�z
	shared_ptr<FbxSceneResource> FbxSceneResource::CreateFbxScene(const wstring& DataDir,
		const wstring& FileName, const string& SceneName, bool NeedStatic) {
		try {
			return ObjectFactory::Create<FbxSceneResource>(DataDir, FileName, SceneName, NeedStatic);
		}
		catch (...) {
			throw;
		}
	}

	//�ċA�I�ɃV�[����ǂݍ���
	void FbxSceneResource::CreateFbxMeshVector(FbxNode* pFbxNode) {
		//�m�[�h���L���łȂ���΃��^�[��
		if (!pFbxNode) {
			return;
		}
		//FBX�V�[���̎擾
		FbxScene* pScene = pFbxNode->GetScene();
		//�m�[�h�̑������擾
		FbxNodeAttribute* pFbxNodeAttribute = pFbxNode->GetNodeAttribute();
		//�m�[�h�̑������L���ł���΁A�����̃^�C�v�𔻒肵�A�^�C�v�����b�V���ł����vector�ɓo�^����
		if (pFbxNodeAttribute) {
			if (pFbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
				//�O�p����
				FbxGeometryConverter converter(pFbxNode->GetFbxManager());
				converter.Triangulate(pScene, true);
				//���b�V�������o��
				FbxMesh* pFbxMesh = pFbxNode->GetMesh();
				//�V�[���̃O���[�o���s����擾
				FbxAnimEvaluator* pFbxAnimEvaluator = pScene->GetAnimationEvaluator();
				FbxMatrix mGlobal = pFbxAnimEvaluator->GetNodeGlobalTransform(pFbxNode);
				//�O���[�o���s������ׂĂ̒��_�ɓK�p
				int	iNumControlPoints = pFbxMesh->GetControlPointsCount();
				FbxVector4*	v = pFbxMesh->GetControlPoints();
				for (int i = 0; i < iNumControlPoints; i++) {
					v[i] = mGlobal.MultNormalize(v[i]);
				}
				//Fbx���b�V���̔z��ɓo�^
				auto FbxMeshPtr = FbxMeshResource2::CreateFbxMeshResource(GetThis<FbxSceneResource>(), pFbxMesh, pImpl->m_NeedStatic);
				pImpl->m_FbxMeshResourceVec.push_back(
					FbxMeshPtr
				);
			}
		}
		//�q�m�[�h�̐����擾����
		int iNumChild = pFbxNode->GetChildCount();
		for (int i = 0; i < iNumChild; i++) {
			//�q�m�[�h���ׂĂ��ċA�I�Ƀ��b�V����o�^����
			CreateFbxMeshVector(pFbxNode->GetChild(i));
		}
	}

	void FbxSceneResource::OnCreate() {
		try {

			//�C���|�[�^�[���쐬����
			FbxImporter* lImporter = FbxImporter::Create(App::GetApp()->GetScene<Scene>()->GetFbxManager().get(), "");
			//�t�@�C�������}���`�o�C�g�ɂ���
			wstring strWork;
			strWork = pImpl->m_DataDir + pImpl->m_FileName;
			//�}���`�o�C�g�ɕϊ�
			string sFileName;
			Util::WStoMB(strWork, sFileName);
			//�C���|�[�^�[�̏�����
			if (!lImporter->Initialize(sFileName.c_str(), -1, App::GetApp()->GetScene<Scene>()->GetFbxManager().get()->GetIOSettings())) {
				//���s����
				throw BaseMBException(
					"�C���|�[�^�̏������Ɏ��s���܂���",
					lImporter->GetStatus().GetErrorString(),
					"FbxSceneResource::FbxSceneResource()");
			}
			//�V�[���̍쐬
			pImpl->m_FbxScene = shared_ptr<FbxScene>(
				FbxScene::Create(
					App::GetApp()->GetScene<Scene>()->GetFbxManager().get(), 
					pImpl->m_FbxSceneName.c_str()),	//���|�C���^
					FbxSceneDeleter()	//�J�X�^���f���[�^
				);
			//�C���|�[�^�ɂ��V�[���ւ̓ǂݍ���
			lImporter->Import(pImpl->m_FbxScene.get());
			//�C���|�[�^�͂����K�v�Ȃ�
			lImporter->Destroy();


			//�ċA�I�ɓǂݍ���
			CreateFbxMeshVector(pImpl->m_FbxScene->GetRootNode());


		}
		catch (...) {
			throw;
		}
	}

	//�A�N�Z�T
	wstring FbxSceneResource::GetDataDir() const {
		return pImpl->m_DataDir;
	}
	wstring FbxSceneResource::GetFileName() const {
		return pImpl->m_FileName;
	}
	string FbxSceneResource::GetFbxSceneName() const {
		return pImpl->m_FbxSceneName;
	}
	shared_ptr<FbxMeshResource2> FbxSceneResource::GetFbxMeshResource(size_t Index) const {
		if (Index >= pImpl->m_FbxMeshResourceVec.size()) {
			auto str = Util::UintToWStr(Index);
			str += L" >= m_FbxMeshResourceVec.size()";
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				str,
				L"FbxSceneResource::GetMeshResource()"
			);
		}
		return pImpl->m_FbxMeshResourceVec[Index];
	}
	size_t FbxSceneResource::GetFbxMeshResourceSize() const {
		return pImpl->m_FbxMeshResourceVec.size();
	}
	vector< shared_ptr<FbxMeshResource2> >& FbxSceneResource::GetFbxMeshResourceVec() const {
		return pImpl->m_FbxMeshResourceVec;
	}

	//--------------------------------------------------------------------------------------
	//	struct BasicFbxPNTBoneDraw::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct BasicFbxPNTBoneDraw::Impl {
		string m_sCurrentAnimationName;	//���ݏ����ΏۂɂȂ��Ă���A�j���[�V�����̖��O
		float m_CurrentTime;			//�A�j���[�V�����̌��݂̌o�ߎ��ԁi�b�j
		bool m_IsAnimeEnd;				//���݂̃A�j���[�V�������I���������ǂ���
		vector< Bone > m_vecLocalBones;	//�e�I�u�W�F�N�g���ƂɃ{�[�����������Ă���
										//�V�F�[�_�ɓn���{�[���s��
		vector<Matrix4X4> m_LocalBonesMatrix;	//�V���h�E�}�b�v�ɓn���|�C���^

		//�A�j���[�V�����𖼑O�ŏƉ�邷��ۂɎg�p����C���f�b�N�X�e�[�u��
		map< string, AnimationData > m_AnimationMap;
		Impl() :
			m_sCurrentAnimationName(""),
			m_CurrentTime(0),
			m_IsAnimeEnd(false)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class BasicFbxPNTBoneDraw : public BasicFbxPNTDraw;
	//	�p�r: BasicFbxPNTBoneDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------

	//�\�z�Ɣj��
	BasicFbxPNTBoneDraw::BasicFbxPNTBoneDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		PNTStaticModelDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}
	BasicFbxPNTBoneDraw::~BasicFbxPNTBoneDraw() {}

	//�{�[���s������\�[�X����ǂݍ���
	void BasicFbxPNTBoneDraw::SetBoneVec() {
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		if (PtrMeshResource->GetNumBones() > 0) {
			//���[�J���{�[���̃I���W�i������̃R�s�[
			pImpl->m_vecLocalBones.resize(PtrMeshResource->GetBonesVec().size());
			pImpl->m_vecLocalBones = PtrMeshResource->GetBonesVec();

			pImpl->m_LocalBonesMatrix.resize(PtrMeshResource->GetNumBones());
		}
		else {
			throw BaseException(
				L"�{�[����������܂���",
				L"if (PtrMeshResource->GetNumBones() <= 0)",
				L"BasicFbxPNTBoneDraw::SetBoneVec()"
			);
		}
	}
	void BasicFbxPNTBoneDraw::GetBoneVec(vector< Bone >& Bones) {
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		if (PtrMeshResource->GetNumBones() > 0) {
			//���[�J���{�[���̃I���W�i������̃R�s�[
			Bones.resize(PtrMeshResource->GetBonesVec().size());
			Bones = PtrMeshResource->GetBonesVec();
		}
		else {
			throw BaseException(
				L"�{�[����������܂���",
				L"if (PtrMeshResource->GetNumBones() <= 0)",
				L"BasicFbxPNTBoneDraw::GetBoneVec()"
			);
		}
	}


	//�A�N�Z�T
	//�e�I�u�W�F�N�g���Ƃ̃{�[��
	const vector< Bone >& BasicFbxPNTBoneDraw::GetVecLocalFbxBones() const {
		return pImpl->m_vecLocalBones;
	}
	const vector< Bone >* BasicFbxPNTBoneDraw::GetVecLocalFbxBonesPtr() const {
		return &pImpl->m_vecLocalBones;
	}

	const vector< Matrix4X4 >* BasicFbxPNTBoneDraw::GetVecLocalBonesPtr() const {
		return &pImpl->m_LocalBonesMatrix;
	}


	void BasicFbxPNTBoneDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		PNTStaticModelDraw::SetMeshResource(MeshRes);
		//�{�[��������Γǂݍ���
		SetBoneVec();
	}
	void BasicFbxPNTBoneDraw::SetMeshResource(const wstring& MeshKey) {
		PNTStaticModelDraw::SetMeshResource(MeshKey);
		//�{�[��������Γǂݍ���
		SetBoneVec();
	}


	void BasicFbxPNTBoneDraw::AddAnimation(const char* Name, UINT StartFrame, UINT FrameLength, bool Loop,
		float FramesParSecond) {
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		if (PtrMeshResource->IsSkining()) {
			map< string, AnimationData >::iterator it
				= pImpl->m_AnimationMap.find(Name);
			if (it != pImpl->m_AnimationMap.end()) {
				//�w��̖��O����������
				//���̃f�[�^�ɍ����ւ�
				it->second.m_StartSample = StartFrame;
				it->second.m_SampleLength = FrameLength;
				it->second.m_SamplesParSecond = FramesParSecond;
				it->second.m_IsLoop = Loop;
			}
			else {
				//������Ȃ�
				//�A�j���[�V������`�̒ǉ�
				pImpl->m_AnimationMap[Name] = AnimationData(StartFrame, FrameLength,
					Loop, FramesParSecond);
			}
		}
	}
	const AnimationData& BasicFbxPNTBoneDraw::GetAnimationData(const string& AnimeName) const {
		auto it = pImpl->m_AnimationMap.find(AnimeName);
		if (it != pImpl->m_AnimationMap.end()) {
			return it->second;
		}
		else {
			throw BaseMBException(
				"�w��̃A�j���[�V������������܂���",
				AnimeName,
				"AnimationData& FbxMeshResource::GetAnimationData()");
		}
	}


	const string& BasicFbxPNTBoneDraw::GetCurrentAnimation() const {
		return pImpl->m_sCurrentAnimationName;
	}
	void BasicFbxPNTBoneDraw::SetCurrentAnimation(const string& AnemationName, float StartTime) {
		//�w��̃A�j���[�V���������邩�ǂ����`�F�b�N
		//������Η�O���o��
		AnimationData animData = GetAnimationData(AnemationName);
		pImpl->m_sCurrentAnimationName = AnemationName;
		pImpl->m_CurrentTime = StartTime;
		pImpl->m_IsAnimeEnd = false;
	}

	float BasicFbxPNTBoneDraw::GetCurrentTime() const {
		return pImpl->m_CurrentTime;
	}


	//���݂̃A�j���[�V�������I�����Ă��邩�ǂ���
	bool BasicFbxPNTBoneDraw::IsTargetAnimeEnd() const {
		return pImpl->m_IsAnimeEnd;
	}

	//�w�肵��ID�̃{�[���̌��݂̍s����擾����
	void BasicFbxPNTBoneDraw::GetBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const {
		auto PtrGameObject = GetGameObject();
		auto PtrT = PtrGameObject->GetComponent<Transform>();
		Matrix4X4 MeshMatrix = GetMeshToTransformMatrix() * PtrT->GetWorldMatrix();
		if (pImpl->m_vecLocalBones.size() <= BoneId) {
			throw BaseException(
				L"�{�[��ID���͈͊O�ł�",
				L"if (pImpl->m_vecLocalBones.size() <= BoneId)",
				L"BasicFbxPNTBoneDraw::GetBoneMatrix()"
			);
		}
		Matrix = pImpl->m_vecLocalBones[BoneId].m_CurrentPose * MeshMatrix;
	}

	//�w�肵��ID�̃{�[���̌��݂̃��[�J���s����擾����i�e�q�֌W���\�z����ȂǗp�j
	void BasicFbxPNTBoneDraw::GetLocalBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const {
		auto PtrGameObject = GetGameObject();
		auto PtrT = PtrGameObject->GetComponent<Transform>();
		Matrix4X4 MeshMatrix = GetMeshToTransformMatrix();
		if (pImpl->m_vecLocalBones.size() <= BoneId) {
			throw BaseException(
				L"�{�[��ID���͈͊O�ł�",
				L"if (pImpl->m_vecLocalBones.size() <= BoneId)",
				L"BasicFbxPNTBoneDraw::GetLocalBoneMatrix()"
			);
		}
		Matrix = pImpl->m_vecLocalBones[BoneId].m_CurrentPose * MeshMatrix;
	}

	bool BasicFbxPNTBoneDraw::UpdateAnimation(float ElapsedTime) {
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		//�{�[�����o�^����Ă�����{�[����T��
		if (PtrMeshResource->IsSkining()) {
			if(!pImpl->m_IsAnimeEnd){
				pImpl->m_CurrentTime += ElapsedTime;
			}
			//�A�j���[�V�����f�[�^�̎擾
			//�`�F�b�N���邾���Ȃ̂Ńe���|�����ł悢
			AnimationData animData = GetAnimationData(pImpl->m_sCurrentAnimationName);
			//�{�[�������݂̎��ԂɍX�V����
			//�߂�l�͏I�����Ă邩�ǂ���
			pImpl->m_IsAnimeEnd = PtrMeshResource->GenerateCurrentPose(pImpl->m_vecLocalBones, animData, pImpl->m_CurrentTime);
			if (animData.m_IsLoop) {
				float TotalTime = (float)animData.m_SampleLength / (float)animData.m_SamplesParSecond;
				if (TotalTime <= pImpl->m_CurrentTime) {
					pImpl->m_CurrentTime = 0.0f;
				}
			}
			//�V���h�E�}�b�v�p�̍s��ɃR�s�[
			size_t BoneSz = pImpl->m_vecLocalBones.size();
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				pImpl->m_LocalBonesMatrix[b] = pImpl->m_vecLocalBones[b].m_ConbinedPose;
			}
			return pImpl->m_IsAnimeEnd;
		}
		return false;
	}


	void  BasicFbxPNTBoneDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();

		auto GameObjectPtr = GetGameObject();
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//�s��̒�`
		Matrix4X4 World, ViewMat, ProjMat;
		//���[���h�s��̌���
		World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		//�]�u����
		World.Transpose();
		//�J�����𓾂�
		auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
		//�r���[�Ǝˉe�s��𓾂�
		ViewMat = CameraPtr->GetViewMatrix();
		//�]�u����
		ViewMat.Transpose();
		//�]�u����
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.Transpose();

		//�R���X�^���g�o�b�t�@�̐ݒ�
		PNTBoneConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//�s��̐ݒ�(�]�u����)
		//�R���X�^���g�o�b�t�@�̐ݒ�
		cb1.World = World;
		cb1.View = ViewMat;
		cb1.Projection = ProjMat;
		auto StageLight = GameObjectPtr->OnGetDrawLight();
		cb1.LightDir = StageLight.m_Directional;
		cb1.LightDir.w = 1.0f;

		//�{�[���̐ݒ�
		size_t BoneSz = pImpl->m_vecLocalBones.size();
		UINT cb_count = 0;
		for (size_t b = 0; b < BoneSz; b++) {
			Matrix4X4 mat = pImpl->m_vecLocalBones[b].m_ConbinedPose;
			mat.Transpose();
			cb1.Bones[cb_count] = ((XMMATRIX)mat).r[0];
			cb1.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
			cb1.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
			cb_count += 3;
		}


/*
		size_t BoneSz = pImpl->m_LocalBonesMatrix.size();
		UINT cb_count = 0;
		for (size_t b = 0; b < BoneSz; b++) {
			Matrix4X4 mat = pImpl->m_LocalBonesMatrix[b];
			mat.Transpose();
			cb1.Bones[cb_count] = ((XMMATRIX)mat).r[0];
			cb1.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
			cb1.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
			cb_count += 3;
		}
*/

		auto MeshPtr = GetMeshResource();
		auto& MatVec = MeshPtr->GetMaterialExVec();

		//�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(VSPNTBone::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(VSPNTBone::GetPtr()->GetInputLayout());

		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionNormalTextureSkinning);
		UINT offset = 0;
		//���_�o�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshPtr->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(MeshPtr->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�f�v�X�X�e���V��
		switch (GetDepthStencilState()) {
		case DepthStencilState::None:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
			break;
		case DepthStencilState::Default:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			break;
		case DepthStencilState::Read:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthRead(), 0);
			break;
		}
		//�T���v���[
		//null�ɏ�����
		ID3D11SamplerState* pSampler = nullptr;
		switch (GetSamplerState()) {
		case SamplerState::SamplerNone:
			break;
		case SamplerState::PointWrap:
			pSampler = RenderStatePtr->GetPointWrap();
			break;
		case SamplerState::PointClamp:
			pSampler = RenderStatePtr->GetPointClamp();
			break;
		case SamplerState::LinearWrap:
			pSampler = RenderStatePtr->GetLinearWrap();
			break;
		case SamplerState::LinearClamp:
			pSampler = RenderStatePtr->GetLinearClamp();
			break;
		case SamplerState::AnisotropicWrap:
			pSampler = RenderStatePtr->GetAnisotropicWrap();
			break;
		case SamplerState::AnisotropicClamp:
			pSampler = RenderStatePtr->GetAnisotropicClamp();
			break;
		case SamplerState::ComparisonLinear:
			pSampler = RenderStatePtr->GetComparisonLinear();
			break;
		}
		//�T���v���[��ݒ�
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		for (auto& m : MatVec) {
			cb1.Emissive = m.m_Emissive;
			cb1.Diffuse = m.m_Diffuse;
			//�R���X�^���g�o�b�t�@�̍X�V
			ID3D11Buffer* pConstantBuffer = CBPNTBone::GetPtr()->GetBuffer();
			pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb1, 0, 0);
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�e�N�X�`����ݒ�
			pID3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
			if (GetRasterizerState() == RasterizerState::Wireframe) {
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetWireframe());
				//�`��
				pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else {
				//�u�����h�X�e�[�g�ƃ��X�^���C�U��ݒ肵�ĕ`��
				//�����A�����`��Ȃ�AlphaBlendEx�ɐݒ肵�A�����łȂ���΁A�w��ɏ]���B
				if (GameObjectPtr->GetAlphaActive()) {
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
					//���X�^���C�U�X�e�[�g
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
					//�`��
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					//���X�^���C�U�X�e�[�g
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
					//�`��
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				else {
					switch (GetBlendState()) {
					case BlendState::Opaque:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
						break;
					case BlendState::AlphaBlend:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
						break;
					case BlendState::Additive:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAdditive(), nullptr, 0xffffffff);
						break;
					case BlendState::NonPremultiplied:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetNonPremultiplied(), nullptr, 0xffffffff);
						break;
					}
					switch (GetRasterizerState()) {
					case RasterizerState::CullBack:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
						break;
					case RasterizerState::CullFront:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
						break;
					case RasterizerState::CullNone:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
						break;
					}
					//�`��
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
			}
		}
		Dev->InitializeStates();
	}




	//--------------------------------------------------------------------------------------
	//	class FbxMeshObject : public GameObject;
	//	�p�r: FBX���b�V���̃I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	FbxMeshObject::FbxMeshObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_DataDir(L""),
		m_FbxFileName(L""),
		m_FbxResName(L""),
		m_MeshIndex(0),
		m_FbxMeshResName(L""),
		m_CharaLocalScale(1.0f),
		m_CharaLocalPosition(0,0,0),
		m_IsReadStaticMesh(false),
		m_IsAnimeRun(false)
	{}
	void FbxMeshObject::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(1.0f, 1.0f, 1.0f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.0f, 0);

		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


	}

	void FbxMeshObject::OnUpdate() {
		auto PtrBoneDraw = GetComponent<BasicFbxPNTBoneDraw>(false);
		if (PtrBoneDraw && m_IsAnimeRun) {
			if (PtrBoneDraw->IsTargetAnimeEnd()) {
				m_IsAnimeRun = false;
			}
			else {
				PtrBoneDraw->UpdateAnimation(App::GetApp()->GetElapsedTime());
			}
		}
	}

	void FbxMeshObject::OnLastUpdate() {
		//������\��
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\n";
		wstring CurrentTime = L"";
		wstring AnimeRun = L"";
		auto PtrBoneDraw = GetComponent<BasicFbxPNTBoneDraw>(false);
		if (PtrBoneDraw) {
			CurrentTime += Util::FloatToWStr(PtrBoneDraw->GetCurrentTime(), 6, Util::FloatModify::Fixed) + L"\n";
			if (m_IsAnimeRun) {
				AnimeRun = L"ANIME: RUN\n";
			}
			else {
				AnimeRun = L"ANIME: STOP\n";
			}
		}
		wstring str = FPS + CurrentTime + AnimeRun;
		//�����������
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}


	void FbxMeshObject::ClearFbxMesh() {
		try {
			if (m_FbxMeshResName != L"") {
				App::GetApp()->UnRegisterResource<FbxMeshResource2>(m_FbxMeshResName);
				m_FbxMeshResName = L"";
			}
			if (m_FbxResName != L"") {
				App::GetApp()->UnRegisterResource<FbxSceneResource>(m_FbxResName);
				m_FbxResName = L"";
			}
		}
		catch (...) {
			throw;
		}
	}

	void FbxMeshObject::ResetFbxMesh(const wstring& DirName, const wstring& FbxName, size_t MeshIndex, float Scale, const Vector3& Position,
		bool IsReadStatic) {
		try {
			if (m_FbxMeshResName != L"") {
				App::GetApp()->UnRegisterResource<FbxMeshResource2>(m_FbxMeshResName);
				m_FbxMeshResName = L"";
			}
			if (m_FbxResName != L"") {
				App::GetApp()->UnRegisterResource<FbxSceneResource>(m_FbxResName);
				m_FbxResName = L"";
			}
			m_DataDir = DirName;
			m_FbxFileName = FbxName;
			m_FbxResName = FbxName;
			m_MeshIndex = MeshIndex;
			m_FbxMeshResName = m_FbxResName + Util::UintToWStr(m_MeshIndex);
			m_CharaLocalScale = Scale;
			m_CharaLocalPosition = Position;
			m_IsReadStaticMesh = IsReadStatic;

			shared_ptr<FbxSceneResource> PtrFbxScene;
			if (App::GetApp()->CheckResource<FbxSceneResource>(m_FbxResName)) {
				PtrFbxScene = App::GetApp()->GetResource<FbxSceneResource>(m_FbxResName);
			}
			else {
				PtrFbxScene = FbxSceneResource::CreateFbxScene(DirName, FbxName,"", m_IsReadStaticMesh);
				App::GetApp()->RegisterResource(m_FbxResName, PtrFbxScene);
			}

			shared_ptr<FbxMeshResource2> PtrFbxMesh;
			if (App::GetApp()->CheckResource<FbxMeshResource2>(m_FbxMeshResName)) {
				PtrFbxMesh = App::GetApp()->GetResource<FbxMeshResource2>(m_FbxMeshResName);
			}
			else {
				PtrFbxMesh = PtrFbxScene->GetFbxMeshResource(m_MeshIndex);
				App::GetApp()->RegisterResource(m_FbxMeshResName, PtrFbxMesh);
			}
			//���f���̍s��
			auto Ptr = GetComponent<Transform>();
			Ptr->SetScale(m_CharaLocalScale, m_CharaLocalScale, m_CharaLocalScale);
			Ptr->SetRotation(0.0f, 0.0f, 0.0f);
			Ptr->SetPosition(m_CharaLocalPosition);

			auto ShadowPtr = AddComponent<Shadowmap>();
			ShadowPtr->SetMeshResource(PtrFbxMesh);


			if (PtrFbxMesh->IsSkining()) {
				RemoveComponent<PNTStaticModelDraw>();
				auto PtrDraw = AddComponent<BasicFbxPNTBoneDraw>();
				PtrDraw->SetMeshResource(PtrFbxMesh);
				PtrDraw->AddAnimation("start", 0, 1, true, 30);
				PtrDraw->SetCurrentAnimation("start");
			}
			else {
				RemoveComponent<BasicFbxPNTBoneDraw>();
				auto PtrDraw = AddComponent<PNTStaticModelDraw>();
				PtrDraw->SetMeshResource(PtrFbxMesh);
				PtrDraw->SetLighting(ShaderLighting::Simple);
			}
		}
		catch (...) {
			throw;
		}
	}

	bool FbxMeshObject::CheckSkinMesh() {
		auto PtrBoneDraw = GetComponent<BasicFbxPNTBoneDraw>(false);
		if (PtrBoneDraw) {
			auto FbxMeshPtr = PtrBoneDraw->GetMeshResource();
			if (FbxMeshPtr->IsSkining()) {
				return true;
			}
		}
		return false;
	}

	bool FbxMeshObject::CheckMesh() {
		auto PtrDraw = GetComponent<PNTStaticModelDraw>(false);
		if (PtrDraw && PtrDraw->GetMeshResource()) {
			return true;
		}
		auto PtrBoneDraw = GetComponent<BasicFbxPNTBoneDraw>(false);
		if (PtrBoneDraw) {
			auto FbxMeshPtr = PtrBoneDraw->GetMeshResource();
			if (FbxMeshPtr->IsSkining()) {
				return true;
			}
		}
		return false;
	}



	void FbxMeshObject::MoveFbxMesh(UINT FrameRate, UINT StartTime, UINT EndTime, bool IsLoop) {

		m_IsAnimeRun = false;
		auto PtrBoneDraw = GetComponent<BasicFbxPNTBoneDraw>(false);
		if (PtrBoneDraw) {
			auto FbxMeshPtr = PtrBoneDraw->GetMeshResource();
			if (!FbxMeshPtr) {
				AfxMessageBox(L"FbxMesh���ǂݍ��܂�Ă܂���B");
				return;
			}
			UINT FrameLength = (EndTime - StartTime) * FrameRate;
			PtrBoneDraw->AddAnimation("run", StartTime, FrameLength, IsLoop,(float)FrameRate);
			PtrBoneDraw->SetCurrentAnimation("run");
			m_IsAnimeRun = true;
		}
		else {
			AfxMessageBox(L"�ǂݍ��񂾃��b�V���̓X�L�����b�V���ł͂���܂���B");
		}

	}

	void FbxMeshObject::AnimePoseStart() {

		auto PtrBoneDraw = GetComponent<BasicFbxPNTBoneDraw>(false);
		if (PtrBoneDraw) {
			if (m_IsAnimeRun) {
				m_IsAnimeRun = false;
			}
			else {
				m_IsAnimeRun = true;
			}
		}
		else {
			AfxMessageBox(L"�ǂݍ��񂾃��b�V���̓X�L�����b�V���ł͂���܂���B");
		}
	}

	void FbxMeshObject::SaveStaticBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale) {
		try {
			string header("BDV1.0");
			if (header.size() < 16) {
				header.resize(16, '\0');
			}
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			vector<MaterialEx> materials;
			vector< shared_ptr<TextureResource> > textures;
			auto PtrDraw = GetComponent<PNTStaticModelDraw>();
			auto Mesh = dynamic_pointer_cast<FbxMeshResource2>(PtrDraw->GetMeshResource());
			Mesh->GetStaticVerticesIndicesMaterials(vertices, indices,materials);
			for (auto& v : vertices) {
				v.position *= Scale;
			}

			wstring filename = Dir + FileName;

			ofstream ofs(filename, ios::out | ios::binary);
			ofs.write(header.c_str(), 16);
			//���_�̕ۑ�
			BlockHeader VerTexHeader;
			VerTexHeader.m_Type = BlockType::Vertex;
			VerTexHeader.m_Size = (UINT)vertices.size() * sizeof(VertexPositionNormalTexture);
			ofs.write((const char*)&VerTexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&vertices.front(), VerTexHeader.m_Size);
			//�C���f�b�N�X�̕ۑ�
			BlockHeader IndexHeader;
			IndexHeader.m_Type = BlockType::Index;
			IndexHeader.m_Size = (UINT)indices.size() * sizeof(uint16_t);
			ofs.write((const char*)&IndexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&indices.front(), IndexHeader.m_Size);
			//�}�e���A���̕ۑ�
			//�}�e���A�����̃w�b�_�̕ۑ�
			BlockHeader MaterialCountHeader;
			MaterialCountHeader.m_Type = BlockType::MaterialCount;
			MaterialCountHeader.m_Size = (UINT)materials.size();
			ofs.write((const char*)&MaterialCountHeader, sizeof(BlockHeader));
			//�}�e���A���{�̂̕ۑ�
			wchar_t Drivebuff[_MAX_DRIVE];
			wchar_t Dirbuff[_MAX_DIR];
			wchar_t FileNamebuff[_MAX_FNAME];
			wchar_t Extbuff[_MAX_EXT];
			BlockHeader MaterialHeader;
			MaterialHeader.m_Type = BlockType::Material;
			for (auto mat : materials) {
				wstring TextureFileName = mat.m_TextureResource->GetTextureFileName();
				::ZeroMemory(Drivebuff, sizeof(Drivebuff));
				::ZeroMemory(Dirbuff, sizeof(Dirbuff));
				::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
				::ZeroMemory(Extbuff, sizeof(Extbuff));
				//���W���[��������A�e�u���b�N�ɕ�����
				_wsplitpath_s(TextureFileName.c_str(),
					Drivebuff, _MAX_DRIVE,
					Dirbuff, _MAX_DIR,
					FileNamebuff, _MAX_FNAME,
					Extbuff, _MAX_EXT);
				TextureFileName = FileNamebuff;
				TextureFileName += Extbuff;

				SaveMaterialEx SaveMat;
				SaveMat.m_StartIndex = mat.m_StartIndex;
				SaveMat.m_IndexCount = mat.m_IndexCount;
				SaveMat.m_Diffuse = mat.m_Diffuse;
				SaveMat.m_Specular = mat.m_Specular;
				SaveMat.m_Ambient = mat.m_Ambient;
				SaveMat.m_Emissive = mat.m_Emissive;
				UINT TextureStrSize = (TextureFileName.size() + 1) * sizeof(wchar_t);
				MaterialHeader.m_Size = sizeof(SaveMaterialEx) + TextureStrSize;
				ofs.write((const char*)&MaterialHeader, sizeof(BlockHeader));
				ofs.write((const char*)&SaveMat, sizeof(SaveMaterialEx));
				ofs.write((const char*)TextureFileName.c_str(), TextureStrSize);

			}
			//End(�w�b�_�̂�)
			BlockHeader EndHeader;
			EndHeader.m_Type = BlockType::End;
			EndHeader.m_Size = 0;
			ofs.write((const char*)&EndHeader, sizeof(BlockHeader));
			ofs.close();





		}
		catch (...) {
			throw;
		}

	}


	void  FbxMeshObject::SaveSkinBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale,
		UINT FrameParSec, UINT Start, UINT End) {
		try {
			string header("BDV1.0");
			if (header.size() < 16) {
				header.resize(16, '\0');
			}
			vector<VertexPositionNormalTextureSkinning> vertices;
			vector<uint16_t> indices;
			vector<MaterialEx> materials;
			vector<Bone> bones;
			map< string, UINT > mapBoneList;
			auto PtrDraw = GetComponent<BasicFbxPNTBoneDraw>();
			auto Mesh = dynamic_pointer_cast<FbxMeshResource2>(PtrDraw->GetMeshResource());

			Mesh->GetSkinVerticesIndicesMaterials(vertices, indices, materials,
				bones, mapBoneList);


			for (auto& v : vertices) {
				v.position *= Scale;
			}

			vector<Matrix4X4> animematrix;

			//�{�[�������݂̎��ԂɍX�V����
			vector<Bone> AnimeBones;
			AnimeBones.resize(bones.size());
			AnimeBones = bones;

			float start = (float)Start * (float)FrameParSec;
			float end = (float)End * (float)FrameParSec;

			float SampleSpan = 1.0f / (float)FrameParSec;

			//�A�j���f�[�^�𓾂�
			PtrDraw->AddAnimation("save_run", (UINT)start, (UINT)(end - start), true,(float) FrameParSec);
			PtrDraw->SetCurrentAnimation("save_run");
			AnimationData animData = PtrDraw->GetAnimationData("save_run");

			for (float f = (float)Start; f < (float)End; f += SampleSpan) {
				Mesh->GenerateCurrentPose(AnimeBones, animData, f);
				for (auto b : AnimeBones) {
					Matrix4X4 Mat = b.m_ConbinedPose;
					Mat._41 *= Scale;
					Mat._42 *= Scale;
					Mat._43 *= Scale;
					animematrix.push_back(Mat);
				}
			}


			wstring filename = Dir + FileName;
			ofstream ofs(filename, ios::out | ios::binary);
			ofs.write(header.c_str(), 16);
			//���_�̕ۑ�
			BlockHeader VerTexHeader;
			VerTexHeader.m_Type = BlockType::SkinedVertex;
			VerTexHeader.m_Size = (UINT)vertices.size() * sizeof(VertexPositionNormalTextureSkinning);
			ofs.write((const char*)&VerTexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&vertices.front(), VerTexHeader.m_Size);
			//�C���f�b�N�X�̕ۑ�
			BlockHeader IndexHeader;
			IndexHeader.m_Type = BlockType::Index;
			IndexHeader.m_Size = (UINT)indices.size() * sizeof(uint16_t);
			ofs.write((const char*)&IndexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&indices.front(), IndexHeader.m_Size);
			//�}�e���A���̕ۑ�
			//�}�e���A�����̃w�b�_�̕ۑ�
			BlockHeader MaterialCountHeader;
			MaterialCountHeader.m_Type = BlockType::MaterialCount;
			MaterialCountHeader.m_Size = (UINT)materials.size();
			ofs.write((const char*)&MaterialCountHeader, sizeof(BlockHeader));
			//�}�e���A���{�̂̕ۑ�
			wchar_t Drivebuff[_MAX_DRIVE];
			wchar_t Dirbuff[_MAX_DIR];
			wchar_t FileNamebuff[_MAX_FNAME];
			wchar_t Extbuff[_MAX_EXT];
			BlockHeader MaterialHeader;
			MaterialHeader.m_Type = BlockType::Material;
			for (auto mat : materials) {
				wstring TextureFileName = mat.m_TextureResource->GetTextureFileName();
				::ZeroMemory(Drivebuff, sizeof(Drivebuff));
				::ZeroMemory(Dirbuff, sizeof(Dirbuff));
				::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
				::ZeroMemory(Extbuff, sizeof(Extbuff));
				//���W���[��������A�e�u���b�N�ɕ�����
				_wsplitpath_s(TextureFileName.c_str(),
					Drivebuff, _MAX_DRIVE,
					Dirbuff, _MAX_DIR,
					FileNamebuff, _MAX_FNAME,
					Extbuff, _MAX_EXT);
				TextureFileName = FileNamebuff;
				TextureFileName += Extbuff;

				SaveMaterialEx SaveMat;
				SaveMat.m_StartIndex = mat.m_StartIndex;
				SaveMat.m_IndexCount = mat.m_IndexCount;
				SaveMat.m_Diffuse = mat.m_Diffuse;
				SaveMat.m_Specular = mat.m_Specular;
				SaveMat.m_Ambient = mat.m_Ambient;
				SaveMat.m_Emissive = mat.m_Emissive;
				UINT TextureStrSize = (TextureFileName.size() + 1) * sizeof(wchar_t);
				MaterialHeader.m_Size = sizeof(SaveMaterialEx) + TextureStrSize;
				ofs.write((const char*)&MaterialHeader, sizeof(BlockHeader));
				ofs.write((const char*)&SaveMat, sizeof(SaveMaterialEx));
				ofs.write((const char*)TextureFileName.c_str(), TextureStrSize);

			}
			//�{�[���J�E���g�i�w�b�_�̂݁j
			BlockHeader BoneCountHeader;
			BoneCountHeader.m_Type = BlockType::BoneCount;
			//�{�[���J�E���g�̏ꍇm_Size�̓{�[�����ɂȂ�
			BoneCountHeader.m_Size = (UINT)bones.size();
			ofs.write((const char*)&BoneCountHeader, sizeof(BlockHeader));

			//�A�j���[�V�����s��
			BlockHeader AnimeMatrixHeader;
			AnimeMatrixHeader.m_Type = BlockType::AnimeMatrix;
			AnimeMatrixHeader.m_Size = (UINT)animematrix.size() * sizeof(Matrix4X4);
			ofs.write((const char*)&AnimeMatrixHeader, sizeof(BlockHeader));
			ofs.write((const char*)&animematrix.front(), AnimeMatrixHeader.m_Size);

			//End(�w�b�_�̂�)
			BlockHeader EndHeader;
			EndHeader.m_Type = BlockType::End;
			EndHeader.m_Size = 0;
			ofs.write((const char*)&EndHeader, sizeof(BlockHeader));


			ofs.close();
		}
		catch (...) {
			throw;
		}

	}


}
//basecross
