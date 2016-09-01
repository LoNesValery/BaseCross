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

	///���[�g�V�O�l�`���쐬
	void TriangleObject::CreateRootSignature() {
		m_RootSignature = RootSignature::CreateSimple();
	}

	///�p�C�v���C���X�e�[�g�쐬
	void TriangleObject::CreatePipelineState() {
		//�p�C�v���C���X�e�[�g�̒�`
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		m_PipelineState 
		= PipelineState::CreateDefault2D<VertexPositionColor, VSPCDirect, PSPCDirect>(m_RootSignature, PineLineDesc);
	}

	///�R�}���h���X�g�쐬
	void TriangleObject::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}



	void TriangleObject::OnCreate() {
		//���_���쐬���邽�߂̔z��
		vector<VertexPositionColor> vertices = {
			{ Vector3(0.0f, 0.5f , 0.0f),Color4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vector3(0.5f, -0.5f, 0.5f),Color4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vector3(-0.5f, -0.5f, 0.5f),Color4(0.0f, 0.0f, 1.0f, 1.0f) }
		};
		m_TriangleMesh = MeshResource::CreateMeshResource(vertices,false);
		CreateRootSignature();
		CreatePipelineState();
		CreateCommandList();
	}
	void TriangleObject::OnUpdate() {
		//�������Ȃ�
	}

	///�`�揈��
	void TriangleObject::DrawObject() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R�}���h���X�g�̃��Z�b�g
		CommandList::Reset(m_PipelineState, m_CommandList);
		//���b�V�����X�V����Ă���΃��\�[�X�X�V
		m_TriangleMesh->UpdateResources<VertexPositionColor>(m_CommandList);

		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());

		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

		//�����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = Dev->GetRtvHandle();
		//�f�v�X�X�e���V���r���[�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = Dev->GetDsvHandle();
		//�擾�����n���h�����Z�b�g
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetVertexBuffers(0, 1, &m_TriangleMesh->GetVertexBufferView());
		m_CommandList->DrawInstanced(m_TriangleMesh->GetNumVertices(), 1, 0, 0);

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}

	void TriangleObject::OnDraw() {
		//�`��
		DrawObject();
	}


}
//end basecross
