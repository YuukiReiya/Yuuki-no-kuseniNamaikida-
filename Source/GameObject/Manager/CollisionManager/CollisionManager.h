/*
	@file	CollisionManager.h
	@brief	�I�u�W�F�N�g�̏Փ˂��Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../../Common/Common.h"
#include "../../Character/Character.h"
#include <vector>

/*
	@struct	CollisionInfo
	@brief	�Փˏ����i�[����\����
	@detail	�Փ˂��鑤�ƏՓː�̏���ێ�����
*/
struct CollisionInfo
{
	Character* A;	/*!< �Փˑ� */
	Character* B;	/*!< �Փː� */
};

/*
	@brief	�ՓˊǗ��N���X
*/
class CollisionManager
{
public:
	/*! �R���X�g���N�^ */
	CollisionManager();
	/*! �f�X�g���N�^ */
	~CollisionManager();

	/*! �V���O���g�� */
	static CollisionManager& GetInstance() {
		static CollisionManager instance;
		return instance;
	}

	void Update();
	void Init();
	void Add(Character& A, Character& B);
	void Clear();
private:
	std::vector<CollisionInfo> m_List;
};

