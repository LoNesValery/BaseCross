/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O�p�`�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	TriangleObject::TriangleObject() :
		ObjectInterface(),
		ShapeInterface()
	{
	}
	TriangleObject::~TriangleObject() {}

	void TriangleObject::OnCreate() {
		//���_���쐬���邽�߂̔z��
		vector<VertexPositionColor> vertices = {
			{ Vector3(0.0f, 0.5f , 0.0f),Color4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vector3(0.5f, -0.5f, 0.5f),Color4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vector3(-0.5f, -0.5f, 0.5f),Color4(0.0f, 0.0f, 1.0f, 1.0f) }
		};
		m_TriangleMesh = MeshResource::CreateMeshResource(vertices,false);

		m_DrawContext = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateSimple);
		m_DrawContext->CreateDefault2DPipelineCmdList<VertexPositionColor, VSPCDirect, PSPCDirect>();
	}
	void TriangleObject::OnUpdate() {
		//�������Ȃ�
	}
	void TriangleObject::OnDraw() {
		//�`��
		m_DrawContext->DrawVertex<VertexPositionColor>(m_TriangleMesh);
	}


}
//end basecross
