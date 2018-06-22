/*
	@file	AStar.h
	@brief	最短経路を求めるアルゴリズム A*(A-star)
	@detail	参考サイト:2dgames.jp/2012/05/22/a-starar/
	@author	番場 宥輝
*/
#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "../../Common/Common.h"
#include "../../GameObject/Manager/TerrainManager/TerrainManager.h"

/*
	@def	ハッシュ計算マクロ
*/
#define KEY(X,Y) ((X) * 100 + (Y))/*!< ハッシュ値 */
#define KEYDATA(X, Y, N) std::pair<int, NODE>(KEY(X,Y), N)/*!< ハッシュキー */

/*
	@brief	双方向リストのノード
*/
struct NODE
{
	int x, y;
	int prev_x, prev_y;
	int cost;
};
/*
	@brief	ノードへデータをセット
*/
static void SetNode(NODE* node, int n_x, int n_y, int n_px, int n_py, int n_cost);

/*
	@brief	A*クラス
*/
class AStar
{
public:
	/*! コンストラクタ */
	AStar();
	/*! デストラクタ */
	~AStar();

	/*! 初期化 */
	void Init(int start_x, int start_y, int goal_x, int goal_y, int range_x, int range_y);

	void UpdateTerrain();	/*!< 地形情報の更新 */
	bool Explore();			/*!< 探索 */

	/*! ゲッター */
	NODE GetRoot(int hash);/*!< ハッシュ */
	NODE GetRoot();

	/*! 変数 */
	std::vector<int>RootHash;/*!< 最短経路が入っているハッシュ値の動的配列 */
private:
	int Search(int count);							/*!< 最短経路の情報をmapに登録 */
	int ComputeDistance(int start_x, int start_y);	/*!< ある地点から目標地点までの距離を求める */
	int ComputeRootCost(int x, int y);				/*!< ある地点からスタート地点までの距離コストを求める */
	void SetRootHash(int x, int y);					/*!< RootHashに最短経路を登録 */

	int m_Start_x;									/*!< 開始点x */
	int m_Start_y;									/*!< 開始点y */
	int m_Range_x;									/*!< 範囲上限x */
	int m_Range_y;									/*!< 範囲上限y */
	int m_Goal_x;									/*!< 終点x */
	int m_Goal_y;									/*!< 終点y */
	int m_Map[SOIL_HEIGHT][SOIL_WIDTH];				/*!< 地形情報 */

	std::map<int, NODE>m_Open;						/*!< オープンリスト */
	std::map<int, NODE>m_Close;						/*!< クローズリスト */
};