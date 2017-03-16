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
		@tparam	T	���_�̌^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void ExpandAndContract() {
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			m_TotalTime += ElapsedTime;
			if (m_TotalTime >= XM_2PI) {
				m_TotalTime = 0;
			}
			auto PtrDraw = GetGameObject()->GetDynamicComponent<StaticBaseDraw>();
			const vector<T>& BackupVec = PtrDraw->GetOriginalMeshResource()->GetBackupVerteces<T>();
			vector<T> new_vec;
			for (auto& v : BackupVec) {
				T new_v;
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

