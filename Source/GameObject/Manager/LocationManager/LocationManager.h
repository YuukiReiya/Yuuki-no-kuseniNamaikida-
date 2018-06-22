/*
	@file	LocationManager.h
	@brief	�L�����̋��ꏊ���Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../List/Location/Location.h"

/*
	@brief	�L�����N�^�[�̋��ꏊ�̊Ǘ��N���X
*/
class LocationManager
{
public:
	/*! �f�X�g���N�^ */
	~LocationManager();
	/*! �V���O���g�� */
	static LocationManager&GetInstance() {
		static LocationManager instance;
		return instance;
	}

	void Init();
	void Destroy();
	bool DeleteData(INT2 pos, Enemy* enemy);

	/*! �Q�b�^�[ */
	Enemy* GetTopFront(INT2 pos);
	Enemy* GetTopBack(INT2 pos);

	/*! �Z�b�^�[ */
	void SetData(INT2 location, Enemy* enemy);
private:

	/*! �R���X�g���N�^ */
	LocationManager();


	/*! �ϐ� */
	Location* m_pLocation;


};

