/*
	@file	Common.h
	@brief	�ėp�w�b�_
	@author	�ԏ� �G�P
*/
#pragma once
#pragma warning(disable:4996)
#include "DirectDevice.h"
#include <Windows.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

/* 
	@brief	�}�N����`
*/
#define MFAIL(code,string) if(FAILED(code)){MessageBox(NULL,_T(string),_T("error"),MB_OK);return E_FAIL;}
#define MFALSE(code,string) if(!(code)){MessageBox(NULL,_T(string),_T("error"),MB_OK);return false;}

/*
	@brief	���������[�N����
*/
#ifdef _DEBUG
#include <crtdbg.h>
#define	new	new(_NORMAL_BLOCK, __FILE__, __LINE__)	//���������[�N����}�N��
#endif

/*
	@brief	�萔�錾
*/
const LPCWSTR APP_NAME = L"���E�L�̂����ɂȂ܂������I";						//�A�v���P�[�V�����̖��O
const int FPS = 60;										//FPS
const int GRID_SIZE = 31;								//�O���b�h�̃T�C�Y
const int SOIL_WIDTH = 60;								//�y�u���b�N�̉���
const int SOIL_HEIGHT = 26;								//�y�u���b�N�̏c��
const int GROUND_WIDTH = SOIL_WIDTH + 2;				//�n��̉���
const int GROUND_HEIGHT = 9;							//�n��̏c��
const int COMPONENT_LEVEL3 = 17;						//�y���x���R
const int COMPONENT_LEVEL2 = 10;						//�y���x���Q
const int COMPONENT_LEVEL1 = 1;							//�y���x���P
const int MAX_COMPONENT = 99;							//�{���E�����̍ő�l
const int MIN_COMPONENT = 0;							//�{���E�����̍ŏ��l
const int HERO_POP_X = 28;								//�E�҂̏o�������ʒux
const int HERO_POP_Y = -1;								//�E�҂̏o�������ʒuy
const int GAME_OVER_X = HERO_POP_X;						//�E�҂̖����A�s���Wx
const int GAME_OVER_Y = 0;								//�E�҂̖����A�s���Wy


/*
	@struct	INT2
	@brief	int�^2�����ėp�I�ȍ\����
*/
struct INT2 {
	int x;	 /*!< �v�f1 */
	int y;	 /*!< �v�f2 */

	/*! �R���X�g���N�^ */
	INT2() = default;
	INT2(int vx, int vy) { x = vx, y = vy; }
	
	/**************/
	/* �I�y���[�^ */
	/**************/

	/*! INT2�^�̑����Z */
	INT2 operator +(INT2 val) {
		return INT2(this->x + val.x, this->y + val.y);
	}

	/*! INT2�^�̈����Z */
	INT2 operator -(INT2 val) {
		return INT2(this->x - val.x, this->y - val.y);
	}

	//
	/*! INT2�^��int�^�̊|���Z */
	INT2 operator *(int val) {
		return INT2(this->x*val, this->y*val);
	}

	/*! INT2�^�̔��� */
	bool operator ==(INT2 val) {
		return this->x == val.x&&this->y == val.y;
	}

	/*! INT2�^�̔��� */
	bool operator !=(INT2 val) {
		return this->x != val.x||this->y != val.y;
	}
};