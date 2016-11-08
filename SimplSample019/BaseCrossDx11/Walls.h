/*!
@file Walls.h
@brief �ǂȂ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	���_�ύX�ł���v���[�g
	//--------------------------------------------------------------------------------------
	class DynamicPlate : public GameObject {
	protected:
		//�o�b�N�A�b�v���_
		vector<VertexPositionNormalTexture> m_BackupVertices;
		//�o�b�N�A�b�v�̃C���f�b�N�X�i�@���̌v�Z�Ɏg���j
		vector<uint16_t> m_BackupIndices;
		//�X�V�p�̒��_
		vector<VertexPositionNormalTexture> m_UpdateVertices;
		//�e�N�X�`�����\�[�X��
		wstring m_TextureResName;
		UINT m_WidthCount;
		UINT m_HeightCount;
		bool m_IsTile;	//�^�C�����O
		Vector3 m_StartScale;
		Quaternion m_StartQt;
		Vector3 m_StartPosition;
		//���C�g
		Light m_MyLight;
	protected:
		//�\�z�Ɣj��
		DynamicPlate(const shared_ptr<Stage>& StagePtr,
			wstring TextureResName,
			UINT WidthCount,
			UINT HeightCount,
			bool IsTile,
			const Vector3& Scale,
			const Quaternion& Qt,
			const Vector3& Position
		);
		virtual ~DynamicPlate();
	public:
		//������
		virtual void OnCreate() override;
		virtual const Light& OnGetDrawLight()const override;

		//���ʂ�Ԃ�
		PLANE GetPLANE() const;
		//����p��RECT��Ԃ�
		COLRECT GetCOLRECT() const;
		//�_�Ƃ̍ŋߐړ_��Ԃ�
		void ClosestPtPoint(const Vector3& Point, Vector3& Ret);
		//�_�Ƃ̋�����Ԃ��i�߂�l���}�C�i�X�Ȃ痠���j
		float GetDistPointPlane(const Vector3& Point) const;
		//�q�b�g�e�X�g�����čŋߐړ_�ƁASquare�̖@����Ԃ�
		bool HitTestSphere(const SPHERE& Sp, Vector3& RetVec, Vector3& Normal);
	};

	//--------------------------------------------------------------------------------------
	//	�ʂ蔲�����
	//--------------------------------------------------------------------------------------
	class ThroughWall : public DynamicPlate {
	public:
		//�\�z�Ɣj��
		ThroughWall(const shared_ptr<Stage>& StagePtr,
			wstring TextureResName,
			UINT WidthCount,
			UINT HeightCount,
			bool IsTile,
			const Vector3& Scale,
			const Quaternion& Qt,
			const Vector3& Position
		);
		virtual ~ThroughWall();
		//����
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	�^�C�����O���镁�ʂ̕�
	//--------------------------------------------------------------------------------------
	class TileWall : public DynamicPlate {
		UINT m_WidthTileCount;
		UINT m_HeightTileCount;
	public:
		//�\�z�Ɣj��
		TileWall(const shared_ptr<Stage>& StagePtr,
			wstring TextureResName,
			UINT WidthTileCount,
			UINT HeightTileCount,
			const Vector3& Scale,
			const Quaternion& Qt,
			const Vector3& Position
		);
		virtual ~TileWall();
		//������
		virtual void OnCreate() override;
	};


}
//end basecross
