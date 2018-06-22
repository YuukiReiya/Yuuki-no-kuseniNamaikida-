/*
	@file	AStar.cpp
	@brief	A*(A-star)
	@detail	参考サイト:2dgames.jp/2012/05/22/a-starar/
	@author	番場 宥輝
*/
#include "AStar.h"

/*
	@def	障害物を表すマクロ
*/
#define OBSTACLE -1

/*
	@brief	コンストラクタ
*/
AStar::AStar()
{
}

/*
	@brief	デストラクタ
*/
AStar::~AStar()
{
}

/*
	@brief	初期化
*/
void AStar::Init(int startx, int starty, int goalx, int goaly, int rangex, int rangey)
{
	/*! 地形データ初期化 */
	for (int y = 0; y < SOIL_HEIGHT; y++) {
		for (int x = 0; x < SOIL_WIDTH; x++) {
			/*! 移動可能 */
			if (TerrainManager::GetInstance().GetBreakFlag(INT2(x, y))) {
				m_Map[y][x] = 0;
			}
			/*! 移動不可能 */
			else {
				m_Map[y][x] = OBSTACLE;
			}
		}
	}
	/*! 動的配列を空にする */
	RootHash.clear();
	/*! open,closeを空にする */
	m_Open.clear();
	m_Close.clear();

	/*! 座標の初期化 */
	m_Start_x = startx;
	m_Start_y = starty;

	m_Goal_x = goalx;
	m_Goal_y = goaly;

	m_Range_x = rangex;
	m_Range_y = rangey;

	NODE n;
	SetNode(
		&n,
		m_Start_x, m_Start_y,
		NULL, NULL,
		0
	);

	m_Open.insert(KEYDATA(n.x, n.y, n));
}

/*
	@brief	地形情報の更新
*/
void AStar::UpdateTerrain()
{
	for (int y = 0; y < SOIL_HEIGHT; y++) {
		for (int x = 0; x < SOIL_WIDTH; x++) {
			/*! 移動可能 */
			if (TerrainManager::GetInstance().GetBreakFlag(INT2(x, y))) {
				m_Map[y][x] = 0;
			}
			/*! 移動不可能 */
			else {
				m_Map[y][x] = OBSTACLE;
			}
		}
	}
}

/*
	@brief	探索
*/
bool AStar::Explore()
{
	if (Search(0) == -1) {
		SetRootHash(m_Goal_x, m_Goal_y);
		return true;
	}

	return false;
}

/*
	@brief	経路を求める
	@detail	最短経路として格納したハッシュ値に応じたノードを返す
	@return	ハッシュ値に対応したNODE
*/
NODE AStar::GetRoot(int hash)
{
	auto it = m_Close.find(hash);
	if ( it!= m_Close.end()) {
		return it->second;
	}
	MSG("最短経路がクローズリストに存在しない");
	return NODE();
}

/*
	@brief	経路を求める
	@detail	最短経路を求めたハッシュリストの後ろから順に経路を求める
	@return	RootHashの一番後ろのノード
*/
NODE AStar::GetRoot()
{
	int hash = RootHash.back();
	RootHash.pop_back();
	return GetRoot(hash);
}

/*
	@brief	最短経路の算出
*/
int AStar::Search(int count)
{
	/*! マップの空チェック */
	if (m_Open.empty())return -5;	/*!< エラー */

	/*! コスト */
	int cost_min = 9999;
	int cost_root = 0;

	/*! 探査経路 */
	NODE nodes[4];
	int way[sizeof(nodes) / sizeof(NODE)][2] = 
	{
		{0,-1},
		{0, 1},
		{-1,0},
		{ 1,0}
	};

	/*! イテレータ */
	auto it = m_Open.begin();
	auto it_min = it;

	while (it!=m_Open.end()){
		if (cost_min > ComputeDistance(it->second.x, it->second.y)) {
			cost_min = ComputeDistance(it->second.x, it->second.y);
			it_min = it;
		}
		it++;
	}

	NODE n;
	SetNode(
		&n,
		it_min->second.x,		it_min->second.y,
		it_min->second.prev_x,	it_min->second.prev_y,
		it_min->second.cost
	);

	cost_root = ComputeRootCost(n.x, n.y);

	m_Close.insert(KEYDATA(n.x, n.y, n));
	m_Open.erase(KEY(n.x, n.y));

	for (int i = 0; i < sizeof(nodes) / sizeof(NODE); i++) {
		int x = n.x + way[i][0];
		int y = n.y + way[i][1];

		if (x < 0)						continue;
		if (y < 0)						continue;
		if (x >= m_Range_x)				continue;
		if (y >= m_Range_y)				continue;
		if (m_Map[y][x] == OBSTACLE)	continue;

		SetNode(
			&nodes[i],
			x, y,
			n.x, n.y,
			cost_root + ComputeDistance(x, y) + 1
		);

		/*! openに既に存在 */
		if (m_Open.find(KEY(x, y)) != m_Open.end()) {
			if (m_Open[KEY(x, y)].cost > nodes[i].cost) {
				m_Open[KEY(x, y)].prev_x = n.x;
				m_Open[KEY(x, y)].prev_y = n.y;
				m_Open[KEY(x, y)].cost = n.cost;
			}
			continue;
		}

		/*! clsoeに既に存在 */
		if (m_Close.find(KEY(x, y)) != m_Close.end()) {
			if (m_Close[KEY(x, y)].cost > nodes[i].cost) {
				NODE temp;
				SetNode(
					&temp,
					x, y,
					n.x, n.y,
					nodes[i].cost
				);
				m_Open.insert(KEYDATA(temp.x, temp.y, temp));
				m_Close.erase(KEY(temp.x, temp.y));
			}
			continue;
		}

		/*! 新規追加 */
		m_Open.insert(KEYDATA(nodes[i].x, nodes[i].y, nodes[i]));
	}
	/*! 見つかれば終了 */
	if (n.x == m_Goal_x&&n.y == m_Goal_y)return -1;

	/*! サーチを続行 */
	return Search(++count);
}

/*
	@brief	2点間の距離算出
	@detail	ある地点からゴールまでの距離を求める
*/
int AStar::ComputeDistance(int start_x, int start_y)
{
	return abs(m_Goal_x - start_x) + abs(m_Goal_y - start_y);
}

/*
	@brief コストの算出
*/
int AStar::ComputeRootCost(int x, int y)
{
	if (x == m_Start_x&&y == m_Start_y)return 0;
	auto it = m_Close.find(KEY(x, y));
	if (it == m_Close.end())return 0;

	return ComputeRootCost(it->second.prev_x, it->second.prev_y) + 1;
}

/*
	@brief	RootHashに最短経路を登録
*/
void AStar::SetRootHash(int x, int y)
{
	if (x == m_Start_x&&y == m_Start_y) {
		RootHash.push_back(KEY(x, y));
		return;
	}
	

	auto temp = m_Close.find(KEY(x, y));
	if (temp != m_Close.end()) {
		RootHash.push_back(KEY(x, y));
		SetRootHash(temp->second.prev_x, temp->second.prev_y);
	}
	return;
}

/*
	@brief	ノードへデータをセット
*/
static void SetNode(NODE * node, int nx, int ny, int npx, int npy, int ncost)
{
	node->x = nx;
	node->y = ny;
	node->prev_x = npx;
	node->prev_y = npy;
	node->cost = ncost;
}
