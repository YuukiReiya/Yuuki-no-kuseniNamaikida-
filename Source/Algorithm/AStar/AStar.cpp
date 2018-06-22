/*
	@file	AStar.cpp
	@brief	A*(A-star)
	@detail	�Q�l�T�C�g:2dgames.jp/2012/05/22/a-starar/
	@author	�ԏ� �G�P
*/
#include "AStar.h"

/*
	@def	��Q����\���}�N��
*/
#define OBSTACLE -1

/*
	@brief	�R���X�g���N�^
*/
AStar::AStar()
{
}

/*
	@brief	�f�X�g���N�^
*/
AStar::~AStar()
{
}

/*
	@brief	������
*/
void AStar::Init(int startx, int starty, int goalx, int goaly, int rangex, int rangey)
{
	/*! �n�`�f�[�^������ */
	for (int y = 0; y < SOIL_HEIGHT; y++) {
		for (int x = 0; x < SOIL_WIDTH; x++) {
			/*! �ړ��\ */
			if (TerrainManager::GetInstance().GetBreakFlag(INT2(x, y))) {
				m_Map[y][x] = 0;
			}
			/*! �ړ��s�\ */
			else {
				m_Map[y][x] = OBSTACLE;
			}
		}
	}
	/*! ���I�z�����ɂ��� */
	RootHash.clear();
	/*! open,close����ɂ��� */
	m_Open.clear();
	m_Close.clear();

	/*! ���W�̏����� */
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
	@brief	�n�`���̍X�V
*/
void AStar::UpdateTerrain()
{
	for (int y = 0; y < SOIL_HEIGHT; y++) {
		for (int x = 0; x < SOIL_WIDTH; x++) {
			/*! �ړ��\ */
			if (TerrainManager::GetInstance().GetBreakFlag(INT2(x, y))) {
				m_Map[y][x] = 0;
			}
			/*! �ړ��s�\ */
			else {
				m_Map[y][x] = OBSTACLE;
			}
		}
	}
}

/*
	@brief	�T��
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
	@brief	�o�H�����߂�
	@detail	�ŒZ�o�H�Ƃ��Ċi�[�����n�b�V���l�ɉ������m�[�h��Ԃ�
	@return	�n�b�V���l�ɑΉ�����NODE
*/
NODE AStar::GetRoot(int hash)
{
	auto it = m_Close.find(hash);
	if ( it!= m_Close.end()) {
		return it->second;
	}
	MSG("�ŒZ�o�H���N���[�Y���X�g�ɑ��݂��Ȃ�");
	return NODE();
}

/*
	@brief	�o�H�����߂�
	@detail	�ŒZ�o�H�����߂��n�b�V�����X�g�̌�납�珇�Ɍo�H�����߂�
	@return	RootHash�̈�Ԍ��̃m�[�h
*/
NODE AStar::GetRoot()
{
	int hash = RootHash.back();
	RootHash.pop_back();
	return GetRoot(hash);
}

/*
	@brief	�ŒZ�o�H�̎Z�o
*/
int AStar::Search(int count)
{
	/*! �}�b�v�̋�`�F�b�N */
	if (m_Open.empty())return -5;	/*!< �G���[ */

	/*! �R�X�g */
	int cost_min = 9999;
	int cost_root = 0;

	/*! �T���o�H */
	NODE nodes[4];
	int way[sizeof(nodes) / sizeof(NODE)][2] = 
	{
		{0,-1},
		{0, 1},
		{-1,0},
		{ 1,0}
	};

	/*! �C�e���[�^ */
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

		/*! open�Ɋ��ɑ��� */
		if (m_Open.find(KEY(x, y)) != m_Open.end()) {
			if (m_Open[KEY(x, y)].cost > nodes[i].cost) {
				m_Open[KEY(x, y)].prev_x = n.x;
				m_Open[KEY(x, y)].prev_y = n.y;
				m_Open[KEY(x, y)].cost = n.cost;
			}
			continue;
		}

		/*! clsoe�Ɋ��ɑ��� */
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

		/*! �V�K�ǉ� */
		m_Open.insert(KEYDATA(nodes[i].x, nodes[i].y, nodes[i]));
	}
	/*! ������ΏI�� */
	if (n.x == m_Goal_x&&n.y == m_Goal_y)return -1;

	/*! �T�[�`�𑱍s */
	return Search(++count);
}

/*
	@brief	2�_�Ԃ̋����Z�o
	@detail	����n�_����S�[���܂ł̋��������߂�
*/
int AStar::ComputeDistance(int start_x, int start_y)
{
	return abs(m_Goal_x - start_x) + abs(m_Goal_y - start_y);
}

/*
	@brief �R�X�g�̎Z�o
*/
int AStar::ComputeRootCost(int x, int y)
{
	if (x == m_Start_x&&y == m_Start_y)return 0;
	auto it = m_Close.find(KEY(x, y));
	if (it == m_Close.end())return 0;

	return ComputeRootCost(it->second.prev_x, it->second.prev_y) + 1;
}

/*
	@brief	RootHash�ɍŒZ�o�H��o�^
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
	@brief	�m�[�h�փf�[�^���Z�b�g
*/
static void SetNode(NODE * node, int nx, int ny, int npx, int npy, int ncost)
{
	node->x = nx;
	node->y = ny;
	node->prev_x = npx;
	node->prev_y = npy;
	node->cost = ncost;
}
