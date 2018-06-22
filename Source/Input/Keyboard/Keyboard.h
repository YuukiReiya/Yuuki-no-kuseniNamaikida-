/*
	@file	Keyboard.h
	@brief	Windows�L�[�{�[�h����
	@detail	�L�[�R�[�h�m�F�p�����N:http://www.azaelia.net/factory/vk.html
	@author	�ԏ� �G�P
*/
#pragma once
#include<Windows.h>



/*! ���O��� */
namespace Keyboard
{
	/*! �����L�[ */
	const char UP    = 0x26;	/*!< �� */
	const char DOWN  = 0x28;	/*!< �� */
	const char LEFT  = 0x25;	/*!< �� */
	const char RIGHT = 0x27;	/*!< �� */

	/*! ����L�[ */
	const char SPACE = 0x20;	/*! �X�y�[�X�L�[		*/
	const char ENTER = 0x0D;	/*! �G���^�[�L�[		*/
	const char SHIFT = 0x10;	/*! �V�t�g�L�[			*/
	const char BACK  = 0x08;	/*! �o�b�N�X�y�[�X�L�[	*/
	const char TAB   = 0x09;	/*! �^�u�L�[			*/


	/*! �L�[�̍X�V�^�C�~���O(���܂����ꏊ��) */
	void Update();

	/*! ������ */
	bool GetButton(BYTE key);		/*!< �����Ă�� */
	bool GetButtonDown(BYTE key);	/*!< �������u�� */
	bool GetButtonUp(BYTE key);		/*!< �������u�� */
}