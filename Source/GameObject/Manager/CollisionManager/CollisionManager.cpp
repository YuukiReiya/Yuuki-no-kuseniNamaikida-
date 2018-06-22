/*
	@file	CollisionManager.cpp
	@brief	�I�u�W�F�N�g�̏Փ˂��Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#include "CollisionManager.h"


/*
	@brief	�R���X�g���N�^
*/
CollisionManager::CollisionManager()
{
}

/*
	@brief	�f�X�g���N�^
*/
CollisionManager::~CollisionManager()
{
}

/*
	@brief	�X�V
*/
void CollisionManager::Update()
{
	if (m_List.empty()) { return; }
	/*! ���X�g�ɓo�^���ꂽ�Փ˂����ɌĂяo�� */
	for (auto itr : m_List) {
		itr.A->Collision(*itr.B);
	}
	
}

/*
	@brief	������
*/
void CollisionManager::Init()
{
	Clear();
}

/*
	@brief	�Փ˃��X�g�֒ǉ�
*/
void CollisionManager::Add(Character & A, Character & B)
{
	CollisionInfo info{
		&A,&B
	};
	m_List.push_back(info);
}

/*
	@brief	���X�g����ɂ���
*/
void CollisionManager::Clear()
{
	if (!m_List.empty()) {
		m_List.clear();			/*!< �v�f�̉�� */
		m_List.shrink_to_fit();	/*!< �L���p�̉�� */
	}
}
