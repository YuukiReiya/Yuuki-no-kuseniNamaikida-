/*
	@file	InputManager.h
	@brief	���͌n�����Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#pragma once

#define input InputManager::GetInstance()

/*
	@brief	���͊Ǘ��N���X
*/
class InputManager
{
public:
	/*! �f�X�g���N�^ */
	~InputManager();

	/*! �V���O���g�� */
	static InputManager& GetInstance(){
		static InputManager instance;
		return instance;
	}


	/*
		@enum	Input
	*/
	enum Key{
		DECIDE,	/*!< ���� */
		CANCEL,	/*!< �L�����Z�� */
		DIG,	/*!< �@�� */
		SEARCH,	/*!< ���ׂ� */
		LEFT,	/*!< �� */
		RIGHT,	/*!< �E */
		UP,		/*!< �� */
		DOWN,	/*!< �� */
	};

	/*! �֐� */
	void Update();

	/*! ���� */
	bool GetKey(Key key);
	bool GetKeyDown(Key key);
	bool GetKeyUp(Key key);
private:
	/*! �R���X�g���N�^ */
	InputManager();
};

