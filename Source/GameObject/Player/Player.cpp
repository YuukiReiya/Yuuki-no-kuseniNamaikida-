/*
	@file	Player.cpp
	@brief	�v���C���[
	@author	�ԏ� �G�P
*/
#include "Player.h"
#include "../Character/Enemy/Larvae/Larvae.h"
#include "../Manager/InputManager/InputManager.h"
#include "../Manager/UiManager/UiManager.h"
#include "../Manager/SoundManager/SoundManager.h"

/*
	@def	�萔
*/
const unsigned int Player::ATK = 30;


/*
	@brief	�R���X�g���N�^
*/
Player::Player()
{
}

/*
	@brief	�f�X�g���N�^
*/
Player::~Player()
{
}

/*
	@brief	������
*/
void Player::Init()
{
	m_IndexPos = { 28,4 };
	m_Pos = Game::ConvertIndexToPosition(m_IndexPos);
	m_Pickel.Init();
	if (!m_Debug.Init()) {
		MSG("�f�o�b�O���s");
	}

}

/*
	@brief	���
*/
void Player::Destroy()
{
	m_Pickel.Destroy();
}

/*
	@brief	�X�V
*/
void Player::Update()
{
	/*! ���W�̍X�V */
	m_Pos = Game::ConvertIndexToPosition(m_IndexPos);
}

/*
	@brief	�����_�����O
*/
void Player::Render()
{
	D3DXVECTOR2 Pos{
		m_Pos.x, 
		m_Pos.y,
	};
	m_Debug.DrawSquare(ConvertView(Pos), (float)GRID_SIZE, (float)GRID_SIZE);
	m_Pickel.Render(ConvertView(Pos));
}

/*
	@brief	�ړ�
*/
void Player::Move()
{
	/*! ���L�[ */
	if (input.GetKeyDown(input.LEFT)) {
		if (m_IndexPos.x > 0)
			m_IndexPos.x -= 1;
	}
	/*! �E�L�[ */
	else if (input.GetKeyDown(input.RIGHT)) {
		if (m_IndexPos.x < (SOIL_WIDTH - 1))
			m_IndexPos.x += 1;
	}
	/*! ��L�[ */
	else if (input.GetKeyDown(input.UP)) {
		if (m_IndexPos.y > 0)
			m_IndexPos.y -= 1;
	}
	/*! ���L�[ */
	else if (input.GetKeyDown(input.DOWN)) {
		if (m_IndexPos.y < (SOIL_HEIGHT - 1))
			m_IndexPos.y += 1;
	}
}

/*
	@brief	�v���C���[���s���A�N�V����
	@detail	�u���b�N�ɑ΂��āu�x�v�A�����X�^�[�ɑ΂��āu�Ԉ����v
*/
void Player::Action()
{
	/*! �L�[�������ꂽ�� */
	if (input.GetKeyDown(input.DIG)) {
		/*! �u���b�N���L�� */
		if (!TerrainManager::GetInstance().GetBreakFlag(m_IndexPos) && 0 < m_DigPower) {
			Break(m_IndexPos);
		}
		/*! �u���b�N������ */
		else {
			Attack(m_IndexPos);
		}
	}
}

/*
	@brief	�y�u���b�N�̔j��
*/
void Player::Break(INT2 pos)
{
	/*! �אڃu���b�N�����Ă���ΐi�� */
	if (!ContactBleakedBlock(pos)) { 
		SoundManager::GetInstance().Play(SE::DIGMISS, false);
		return; 
	}
	m_DigPower--;
	SoundManager::GetInstance().Play(SE::DIG, false);
	TerrainManager::GetInstance().Break(pos);
	auto component = TerrainManager::GetInstance().GetComponent(pos);
	/* �{���E�����������Ă���΃����X�^�[�������� */
	if (component == INT2{ 0,0 }) { return; }
	EnemyManager::GetInstance().Produce(pos, component);
}

/*
	@brief	�����X�^�[���U��
	@detail	�C���f�b�N�X�̍őO�ʂɂ��郂���X�^�[���U��
*/
void Player::Attack(INT2 pos)
{
	Enemy* enemy = LocationManager::GetInstance().GetTopFront(pos);
	if (enemy == NULL) { return; }
	enemy->Damage(ATK);
}

/*
	@brief	�����̑ҋ@�ʒu�ݒ�
*/
bool Player::SetTycoon()
{
	if (input.GetKeyDown(input.DECIDE) && TerrainManager::GetInstance().GetBreakFlag(m_IndexPos)) {
		TycoonManager::GetInstance().SetTycoonPos(m_IndexPos);
		return true;
	}
	return false;
}

/*
	@brief	�x�p���[�ݒ�
*/
void Player::SetDigPower(int power)
{
	m_DigPower = power;
}

/*
	@brief	�x�p���[�\��
*/
void Player::DrawDigPower()
{
	UiManager::GetInstance().DrawDigPower(m_DigPower);
}

/*
	@brief	�אڃ}�X�ɉ�ꂽ�u���b�N�����邩�`�F�b�N
*/
bool Player::ContactBleakedBlock(INT2 pos)
{
	/*! �אڃ}�X */
	INT2 contact[4] = {
		{ 0,-1 },/*!< �� */
		{ 0, 1 },/*!< �� */
		{ -1,0 },/*!< �� */
		{ 1, 0 },/*!< �E */
	};

	for (auto i : contact) {
		INT2 index = pos + i;

		if (index.x < 0 || SOIL_WIDTH <= index.x) { continue; }
		if (index.y < 0 || SOIL_HEIGHT <= index.y) { continue; }
		if (TerrainManager::GetInstance().GetBreakFlag(index)) { return true; }
	}
	/*! �אڂ���u���b�N�͂ǂ������Ă��Ȃ� */
	return false;
}
