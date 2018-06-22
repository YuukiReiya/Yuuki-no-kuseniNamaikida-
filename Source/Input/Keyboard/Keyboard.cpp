/*
	@file	Keyboard.cpp
	@brief	Windows�L�[�{�[�h����
	@detail	�L�[�R�[�h�m�F�p�����N:http://www.azaelia.net/factory/vk.html
	@author	�ԏ� �G�P
*/
#include "Keyboard.h"

/*! ���O��� */
namespace Keyboard {

	/*! �L�[�̐� */
	const int keyNum = 256;

	/*
		@struct	Keyboard
		@brief	�L�[�̏�Ԃ�ۑ�����\����
	*/
	struct Keyboard
	{
		BYTE keys[keyNum];
	};

	Keyboard now,	/*!< ���͏� */
			 old;	/*!< 1�t���[���O�̓��͏� */

	/*
		@brief	������
	*/
	void Init()
	{
		ZeroMemory(&now, sizeof(Keyboard));
		ZeroMemory(&old, sizeof(Keyboard));
	}

	/*
		@brief	�X�V����
	*/
	void Update()
	{
		old = now;						/*!< old�̍X�V */

		GetKeyboardState(now.keys);		/*!< �L�[�̎擾 */
	}


	/*!< �����Ă���� */
	bool GetButton(BYTE key)
	{
		return now.keys[key] & 0x80;
	}

	/*!< �������u�� */
	bool GetButtonDown(BYTE key)
	{
		return (now.keys[key] ^ old.keys[key]) & now.keys[key];
	}

	/*!< �����ꂽ�u�� */
	bool GetButtonUp(BYTE key)
	{
		return (now.keys[key] ^ old.keys[key]) & old.keys[key];
	}
}