/*!
@file ProjectBehavior.h
@brief �v���W�F�N��`�̍s���N���X
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///���_��ύX����s���N���X
	//--------------------------------------------------------------------------------------
	class VertexBehavior : public Behavior {
		float m_TotalTime;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		VertexBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr),
			m_TotalTime(0)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~VertexBehavior() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �L�яk�݂���
		@tparam	T1	���_�̌^
		@tparam	T2	�`��R���|�[�l���g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T1, typename T2>
		void ExpandAndContract() {
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			m_TotalTime += ElapsedTime;
			if (m_TotalTime >= XM_2PI) {
				m_TotalTime = 0;
			}
			auto PtrDraw = GetGameObject()->GetComponent<T2>();
			const vector<T1>& BackupVec = PtrDraw->GetOriginalMeshResource()->GetBackupVerteces<T1>();
			vector<T1> new_vec;
			for (auto& v : BackupVec) {
				T1 new_v;
				new_v = v;
				auto Len = (sin(m_TotalTime) * 0.5f) + 1.0f;
				new_v.position.x *= Len;
				new_v.position.z *= Len;
				new_vec.push_back(new_v);
			}
			PtrDraw->UpdateVertices(new_vec);
		}

	};


}

//end basecross

