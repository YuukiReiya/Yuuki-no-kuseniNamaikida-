/*
	@file	Soldier.cpp
	@brief	���m
	@author	�ԏ� �G�P
*/
#include "Soldier.h"

#include "../../../../Debug/Debug.h"
#include "../../../Manager/TycoonManager/TycoonManager.h"

/*
	@def	�萔�錾
*/
const unsigned int Soldier::WAIT_COUNT = 20;

/*
	@brief	�R���X�g���N�^
*/
Soldier::Soldier()
{
}

/*
	@brief	�f�X�g���N�^
*/
Soldier::~Soldier()
{
	Destroy();
}

/*
	@brief	������
*/
void Soldier::Init()
{
	m_pSprite = new Sprite;
	m_pEnemy = NULL;

	/*! �X�v���C�g�֘A */
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;
	m_pSprite->SetSpriteSize((float)GRID_SIZE, (float)GRID_SIZE);
	m_pSprite->SetSplit(3, 4);

	/*! �p�����[�^ */
	m_Pos = Game::ConvertIndexToPosition(INT2{ HERO_POP_X,0 });
	m_IndexPos = Game::ConvertPositionToIndex(m_Pos);
	m_isCaught = false;
	m_Count = 0;
	state = STATE::MOVE;
	m_TargetIndexPos = INT2();
	aStar.Init(
		m_IndexPos.x, m_IndexPos.y,
		TycoonManager::GetInstance().GetTycoonPos().x, TycoonManager::GetInstance().GetTycoonPos().y,
		SOIL_WIDTH, SOIL_WIDTH
	);
	if (!aStar.Explore())MSG("�����܂ł̍ŒZ���������߂邱�Ƃ��o���܂���ł���");

	/*! �ړ��ڕW���o�� */
	{
		aStar.RootHash.pop_back();
		auto next = aStar.GetRoot();
		m_NextIndexPos = { next.x,next.y };
	}

	m_Debug.Init();
}

/*
	@brief	���
*/
void Soldier::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief�@�X�V
*/
void Soldier::Update()
{
	/*! �n�`�f�[�^�̍X�V */
	if (!m_isCaught)aStar.UpdateTerrain();

	switch (state)
	{
		/*! ���̈ړ������蓖�� */
	case Soldier::ASSIGN:

		break;
		/*! �ړ� */
	case Soldier::MOVE:
		if (!Move()) {
			/*! �ړ��I�� */
			IndexUpdate();

		}
			
		break;
		/*! �U�� */
	case Soldier::ATK:
		/*! Collision��Update�̌�ŌĂяo���Ă��邽�ߏՓ˂��Ă�����state��ATK�ɂȂ� */
		state = STATE::MOVE;
		if (Wait()) {
			/*! �ҋ@�I�� */
			m_pEnemy->Damage(m_Atk);
		}
		break;
		/*! �ҋ@ */
	case Soldier::WAIT:
		
		break;
		/*! �ߔ� */
	case Soldier::CATCH:
		state = STATE::MOVE;
		break;
	default:
		break;
	}



	/*! �ړ� */
//	Move();

	/*! �A�j���[�V�����X�V */
	AnimUpdate();
}

/*
	@brief	�`��
*/
void Soldier::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	�C���f�b�N�X�̍X�V
*/
void Soldier::IndexUpdate()
{
	auto before = m_IndexPos;
	m_IndexPos = Game::ConvertPositionToIndex(m_Pos);/*!< �C���f�b�N�X�X�V */

	if (before == m_IndexPos) { return; }

	/*! �ړ���X�V */
	if (!aStar.RootHash.empty()) {
		auto next = aStar.GetRoot();
		m_NextIndexPos = { next.x,next.y };
	}

}

/*
	@brief	�����ߔ�
*/
void Soldier::Catch()
{
	/*! �X�^�[�g�n�_�܂ł̍ŒZ�o�H�����߂� */
	m_TargetIndexPos = { GAME_OVER_X,GAME_OVER_Y };
	aStar.Init(
		m_IndexPos.x, m_IndexPos.y,
		m_TargetIndexPos.x, m_TargetIndexPos.y,
		SOIL_WIDTH, SOIL_HEIGHT
	);
	if (!aStar.Explore())MSG("�T�����s");
	aStar.RootHash.pop_back();
	auto next = aStar.GetRoot();
	m_NextIndexPos = { next.x,next.y };
}

/*
	@brief	�_���[�W
*/
void Soldier::Damage(int damage)
{
	m_Hp -= damage;
	if (m_Hp <= 0) {
		m_Hp = 0;
		/*
			���S����
			�i�E�ҌX�j
		*/
	}
}

/*
	@brief �Փ�
*/
void Soldier::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	����̏����擾���U����Ԃ֑J��
*/
void Soldier::Battle(Enemy & enemy)
{
	m_pEnemy = &enemy;
	state = STATE::ATK;
}


/*
	@brief	�A�j���[�V�����̍X�V
*/
void Soldier::AnimUpdate()
{
	m_AnimCount++;
	/*! �~�܂��Ă����瑫���݂��Ȃ� */
	if (m_Move == 0)m_AnimIndexU = 1;
	else {
		if (m_AnimCount > 24) {
			m_AnimCount = 0;
		}
		m_AnimIndexU = m_AnimCount > 12 ? 2 : 0;
	}

	/*! �����ɉ������摜�̃C���f�b�N�X���w�� */
	if (m_Direction == Game::UP)
		m_AnimIndexV = 3;
	else if (m_Direction == Game::DOWN)
		m_AnimIndexV = 0;
	else if (m_Direction == Game::LEFT)
		m_AnimIndexV = 1;
	else if (m_Direction == Game::RIGHT)
		m_AnimIndexV = 2;

	m_AnimIndexU %= 3;
	
}

/*
	@brief	�ړ�
*/
bool Soldier::Move()
{
	/*! �����̍X�V */
	m_Direction = {
		(float)(m_NextIndexPos - m_IndexPos).x,
		(float)(m_NextIndexPos - m_IndexPos).y,
	};

	m_Pos += m_Direction*m_Move;

	/*! ���W���ړ���𒴂��Ă�����␳���Ă����� */
	{
		bool isOver = false;/*!< ���W�̒��߃t���O */
		D3DXVECTOR2 temp = Game::ConvertIndexToPosition(m_NextIndexPos);
		/*! ���W���ߔ��� */
		if (m_Direction == Game::UP&&temp.y >= m_Pos.y) { isOver = true; }			/*!< �� */
		else if (m_Direction == Game::DOWN&&temp.y <= m_Pos.y) { isOver = true; }	/*!< �� */
		else if (m_Direction == Game::LEFT&&temp.x >= m_Pos.x) { isOver = true; }	/*!< �� */
		else if (m_Direction == Game::RIGHT&&temp.x <= m_Pos.x) { isOver = true; }	/*!< �E */

		/*! ���ߍ��W�̕␳ */
		if (isOver)m_Pos = temp;
	}
	return !Game::TheSamePositionToGrid(m_Pos);

}

/*
	@brief	�ҋ@
*/
bool Soldier::Wait()
{
	m_Count++;
	if (m_Count < WAIT_COUNT) { return false; }
	m_Count = 0;
	return true;
}