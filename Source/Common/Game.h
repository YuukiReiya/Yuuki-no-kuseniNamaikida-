#include "Common.h"
#pragma once

/* 
	@brief	ゲームで使う関数
*/
namespace Game {
	/*
		@brief	座標がインデックスと完全一致しているかチェック
		@return	結果
	*/
	static bool TheSamePositionToGrid(D3DXVECTOR2 Pos) {

		INT2 index{
			(int)Pos.x / GRID_SIZE - 1,
			(int)Pos.y / GRID_SIZE - GROUND_HEIGHT
		};
		D3DXVECTOR2 temp{
			(float)GRID_SIZE + (float)GRID_SIZE / 2 + index.x*GRID_SIZE,
			(float)GRID_SIZE*GROUND_HEIGHT + (float)GRID_SIZE / 2 + index.y*GRID_SIZE
		};
		if (Pos.x != temp.x || Pos.y != temp.y) {
			return false;
		}
		return true;
	}

	/*
		@brief	座標からインデックスを求める
		@detail	座標が完全一致している必要がある
		@return	座標から求めたインデックス
	*/
	static INT2 ConvertPositionToIndex(D3DXVECTOR2 Pos) {
		Pos.x -= (float)GRID_SIZE / 2;
		Pos.y -= (float)GRID_SIZE / 2;
		Pos /= (float)GRID_SIZE;
		INT2 temp;
		temp.x = (int)Pos.x;
		temp.y = (int)Pos.y;
		//座標を調整
		temp.x = temp.x - 1;
		temp.y = temp.y - GROUND_HEIGHT;
		return temp;
	}

	/*
		@brief	インデックスから座標を求める
		@return	算出した座標
	*/
	static D3DXVECTOR2 ConvertIndexToPosition(INT2 IndexPos) {
		D3DXVECTOR2 Pos{
			(float)GRID_SIZE + (float)GRID_SIZE / 2 + IndexPos.x*(float)GRID_SIZE,
			(float)GRID_SIZE*GROUND_HEIGHT + (float)GRID_SIZE / 2 + IndexPos.y*(float)GRID_SIZE,
		};
		return Pos;
	}


	/*
		@def	定数宣言
	*/
	const D3DXVECTOR2 UP{ 0,-1 };	//上方向の単位ベクトル
	const D3DXVECTOR2 DOWN{ 0,1 };	//下方向の単位ベクトル
	const D3DXVECTOR2 LEFT{ -1,0 };	//左方向の単位ベクトル
	const D3DXVECTOR2 RIGHT{ 1,0 };	//右方向の単位ベクトル
	const D3DXVECTOR2 FORWARD{ 0,-1 };//前方の単位ベクトル
	const D3DXVECTOR2 BACK{ 0,1 };//後方の単位ベクトル
					
	/*
		@brief	単位ベクトルをINT2型に変換
	*/
	static INT2 ConvertIndex(const D3DXVECTOR2 direction) {
		if (direction == UP)		return INT2(0, -1);
		else if (direction == DOWN)	return INT2(0, 1);
		else if (direction == LEFT)	return INT2(-1, 0);
		else if (direction == RIGHT)return INT2(1, 0);
		return INT2(0, 0);
	}

	/*
		@brief	INT2型を単位ベクトルに変換
	*/
	static D3DXVECTOR2 ConvertDirection(INT2 normal) 
	{
		if (normal == INT2(0, -1)) { return UP; }
		if (normal == INT2(0,  1)) { return DOWN; }
		if (normal == INT2(-1, 0)) { return LEFT; }
		if (normal == INT2( 1, 0)) { return RIGHT; }
		return D3DXVECTOR2(0, 0);
	}

	/*
		@brief	方向を受け取り、対するローカルな方向を返す
		@detail	どちらも単位ベクトル
	*/
	static D3DXVECTOR2 ConvertLocalDirection(D3DXVECTOR2 direction, D3DXVECTOR2 LocalTargetDirection) {
		/*! 上 */
		if (direction == UP) {
			/*! ローカルの前方 */
			if (LocalTargetDirection == UP)			return UP;
			/*! ローカルの後方 */
			else if (LocalTargetDirection == DOWN)	return DOWN;
			/*! ローカルの左 */
			else if (LocalTargetDirection == LEFT)	return LEFT;
			/*! ローカルの右 */
			else if (LocalTargetDirection == RIGHT)	return RIGHT;
		}
		/*! 下 */
		else if (direction == DOWN) {
			/*! ローカルの前方 */
			if (LocalTargetDirection == UP)			return DOWN;
			/*! ローカルの後方 */
			else if (LocalTargetDirection == DOWN)	return UP;
			/*! ローカルの左 */
			else if (LocalTargetDirection == LEFT)	return RIGHT;
			/*! ローカルの右 */
			else if (LocalTargetDirection == RIGHT)	return LEFT;
		}
		/*! 左 */
		else if (direction == LEFT) {
			/*! ローカルの前方 */
			if (LocalTargetDirection == UP)			return LEFT;
			/*! ローカルの後方 */
			else if (LocalTargetDirection == DOWN)	return RIGHT;
			/*! ローカルの左 */
			else if (LocalTargetDirection == LEFT)	return DOWN;
			/*! ローカルの右 */
			else if (LocalTargetDirection == RIGHT)	return UP;
		}
		/*! 右 */
		else if (direction == RIGHT) {
			/*! ローカルの前方 */
			if (LocalTargetDirection == UP)			return RIGHT;
			/*! ローカルの後方 */
			else if (LocalTargetDirection == DOWN)	return LEFT;
			/*! ローカルの左 */
			else if (LocalTargetDirection == LEFT)	return UP;
			/*! ローカルの右 */
			else if (LocalTargetDirection == RIGHT)	return DOWN;
		}
		return D3DXVECTOR2(0, 0);
	}
}