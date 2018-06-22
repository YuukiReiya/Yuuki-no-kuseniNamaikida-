/*
	@file	AStar.h
	@brief	�ŒZ�o�H�����߂�A���S���Y�� A*(A-star)
	@detail	�Q�l�T�C�g:2dgames.jp/2012/05/22/a-starar/
	@author	�ԏ� �G�P
*/
#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "../../Common/Common.h"
#include "../../GameObject/Manager/TerrainManager/TerrainManager.h"

/*
	@def	�n�b�V���v�Z�}�N��
*/
#define KEY(X,Y) ((X) * 100 + (Y))/*!< �n�b�V���l */
#define KEYDATA(X, Y, N) std::pair<int, NODE>(KEY(X,Y), N)/*!< �n�b�V���L�[ */

/*
	@brief	�o�������X�g�̃m�[�h
*/
struct NODE
{
	int x, y;
	int prev_x, prev_y;
	int cost;
};
/*
	@brief	�m�[�h�փf�[�^���Z�b�g
*/
static void SetNode(NODE* node, int n_x, int n_y, int n_px, int n_py, int n_cost);

/*
	@brief	A*�N���X
*/
class AStar
{
public:
	/*! �R���X�g���N�^ */
	AStar();
	/*! �f�X�g���N�^ */
	~AStar();

	/*! ������ */
	void Init(int start_x, int start_y, int goal_x, int goal_y, int range_x, int range_y);

	void UpdateTerrain();	/*!< �n�`���̍X�V */
	bool Explore();			/*!< �T�� */

	/*! �Q�b�^�[ */
	NODE GetRoot(int hash);/*!< �n�b�V�� */
	NODE GetRoot();

	/*! �ϐ� */
	std::vector<int>RootHash;/*!< �ŒZ�o�H�������Ă���n�b�V���l�̓��I�z�� */
private:
	int Search(int count);							/*!< �ŒZ�o�H�̏���map�ɓo�^ */
	int ComputeDistance(int start_x, int start_y);	/*!< ����n�_����ڕW�n�_�܂ł̋��������߂� */
	int ComputeRootCost(int x, int y);				/*!< ����n�_����X�^�[�g�n�_�܂ł̋����R�X�g�����߂� */
	void SetRootHash(int x, int y);					/*!< RootHash�ɍŒZ�o�H��o�^ */

	int m_Start_x;									/*!< �J�n�_x */
	int m_Start_y;									/*!< �J�n�_y */
	int m_Range_x;									/*!< �͈͏��x */
	int m_Range_y;									/*!< �͈͏��y */
	int m_Goal_x;									/*!< �I�_x */
	int m_Goal_y;									/*!< �I�_y */
	int m_Map[SOIL_HEIGHT][SOIL_WIDTH];				/*!< �n�`��� */

	std::map<int, NODE>m_Open;						/*!< �I�[�v�����X�g */
	std::map<int, NODE>m_Close;						/*!< �N���[�Y���X�g */
};