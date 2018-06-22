/*
	@file	LocationManager.cpp
	@brief	�L�����̋��ꏊ���Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#include "LocationManager.h"
#include "../TerrainManager/TerrainManager.h"

/*
	@brief	�R���X�g���N�^
*/
LocationManager::LocationManager()
{
}

/*
	@brief	�f�X�g���N�^
*/
LocationManager::~LocationManager()
{
	Destroy();
}

/*
	@brief	������
*/
void LocationManager::Init()
{
	m_pLocation = new Location;

	m_pLocation->Init();
}

/*
	@brief	���
*/
void LocationManager::Destroy()
{
	SAFE_DELETE(m_pLocation);
}

/*
	@brief	���I�z�񂩂�f�[�^���폜
*/
bool LocationManager::DeleteData(INT2 pos, Enemy* enemy)
{
	/*! �C�e���[�^ */
	auto itr = m_pLocation->m_EnemyLocation[pos.x][pos.y].enemy.begin();
	/*!  */
	while (itr != m_pLocation->m_EnemyLocation[pos.x][pos.y].enemy.end()) {
		if (enemy == *itr) { 
			m_pLocation->m_EnemyLocation[pos.x][pos.y].enemy.erase(itr);
			return true;
		}
		itr++;
	}
	return false;
}

/*
	@brief	�őO�ʂɂ���C���f�b�N�X�̃L�����N�^�[��Ԃ�
	@param(pos)	����C���f�b�N�X
	@return	�L�����N�^�[�̃|�C���^ �����Ȃ�������NULL��Ԃ�
*/
Enemy * LocationManager::GetTopFront(INT2 pos)
{
	if (m_pLocation->m_EnemyLocation[pos.x][pos.y].enemy.empty()) { return NULL; }
	return *m_pLocation->m_EnemyLocation[pos.x][pos.y].enemy.begin();
}

/*
	@brief	�Ŕw�ʂɂ���C���f�b�N�X�̃L�����N�^�[d��Ԃ�
	@param(pos)	����C���f�b�N�X
	@return	�L�����N�^�[�̃|�C���^ �����Ȃ�������NULL��Ԃ�
*/
Enemy * LocationManager::GetTopBack(INT2 pos)
{
	if (m_pLocation->m_EnemyLocation[pos.x][pos.y].enemy.empty()) { return NULL; }
	return *m_pLocation->m_EnemyLocation[pos.x][pos.y].enemy.end();
}

/*
	@brief	�L�����N�^�[�̃f�[�^��Ԃ�
	@param(pos)	����C���f�b�N�X
	@param(*character)	�T���Ώۂ̃L�����N�^�[
	@return	�L�����N�^�[�̃|�C���^ �����Ȃ�������NULL��Ԃ�
	@detail	�����̃L�����N�^�[�̃C���X�^���X�����I�z��ɑ��݂��Ă���΂���
*/

/*
	@brief	�L�����N�^�[�̏����Z�b�g����
*/
void LocationManager::SetData(INT2 location, Enemy* enemy)
{
	m_pLocation->m_EnemyLocation[location.x][location.y].enemy.push_back(enemy);
}
