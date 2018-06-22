/*
	@file	Common.h
	@brief	汎用ヘッダ
	@author	番場 宥輝
*/
#pragma once
#pragma warning(disable:4996)
#include "DirectDevice.h"
#include <Windows.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

/* 
	@brief	マクロ定義
*/
#define MFAIL(code,string) if(FAILED(code)){MessageBox(NULL,_T(string),_T("error"),MB_OK);return E_FAIL;}
#define MFALSE(code,string) if(!(code)){MessageBox(NULL,_T(string),_T("error"),MB_OK);return false;}

/*
	@brief	メモリリーク特定
*/
#ifdef _DEBUG
#include <crtdbg.h>
#define	new	new(_NORMAL_BLOCK, __FILE__, __LINE__)	//メモリリーク特定マクロ
#endif

/*
	@brief	定数宣言
*/
const LPCWSTR APP_NAME = L"ユウキのくせになまいきだ！";						//アプリケーションの名前
const int FPS = 60;										//FPS
const int GRID_SIZE = 31;								//グリッドのサイズ
const int SOIL_WIDTH = 60;								//土ブロックの横幅
const int SOIL_HEIGHT = 26;								//土ブロックの縦幅
const int GROUND_WIDTH = SOIL_WIDTH + 2;				//地上の横幅
const int GROUND_HEIGHT = 9;							//地上の縦幅
const int COMPONENT_LEVEL3 = 17;						//土レベル３
const int COMPONENT_LEVEL2 = 10;						//土レベル２
const int COMPONENT_LEVEL1 = 1;							//土レベル１
const int MAX_COMPONENT = 99;							//養分・魔分の最大値
const int MIN_COMPONENT = 0;							//養分・魔分の最小値
const int HERO_POP_X = 28;								//勇者の出現初期位置x
const int HERO_POP_Y = -1;								//勇者の出現初期位置y
const int GAME_OVER_X = HERO_POP_X;						//勇者の魔王連行座標x
const int GAME_OVER_Y = 0;								//勇者の魔王連行座標y


/*
	@struct	INT2
	@brief	int型2つを持つ汎用的な構造体
*/
struct INT2 {
	int x;	 /*!< 要素1 */
	int y;	 /*!< 要素2 */

	/*! コンストラクタ */
	INT2() = default;
	INT2(int vx, int vy) { x = vx, y = vy; }
	
	/**************/
	/* オペレータ */
	/**************/

	/*! INT2型の足し算 */
	INT2 operator +(INT2 val) {
		return INT2(this->x + val.x, this->y + val.y);
	}

	/*! INT2型の引き算 */
	INT2 operator -(INT2 val) {
		return INT2(this->x - val.x, this->y - val.y);
	}

	//
	/*! INT2型とint型の掛け算 */
	INT2 operator *(int val) {
		return INT2(this->x*val, this->y*val);
	}

	/*! INT2型の判定 */
	bool operator ==(INT2 val) {
		return this->x == val.x&&this->y == val.y;
	}

	/*! INT2型の判定 */
	bool operator !=(INT2 val) {
		return this->x != val.x||this->y != val.y;
	}
};