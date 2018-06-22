/*
	@file	Larvae.cpp
	@brief	�c��
	@author	�ԏ� �G�P
*/
#include "Larvae.h"
#include "../../../Manager/LocationManager/LocationManager.h"

/*
	@def	�萔�錾
*/
const unsigned int Larvae::MAX_HP = 64;
const unsigned int Larvae::POWER = 19;
const unsigned int Larvae::WAIT_COUNT = 60;
const unsigned int Larvae::GROW_COMPONENT = 20;
const unsigned int Larvae::GROW_HP = 20;
const unsigned int Larvae::STACK_SIZE = sizeof(Larvae::m_StackDirection) / sizeof(D3DXVECTOR2);
const D3DXVECTOR2 Larvae::INITIALIZE_STACK_ELEMENTS = { 0,0 };

/*
	@brief	namespace
*/
using namespace Game;

/*
	@brief	�R���X�g���N�^
*/
Larvae::Larvae()
{
}

/*
	@brief	�f�X�g���N�^
*/
Larvae::~Larvae()
{
	Destroy();
}

/*
	@brief	������
*/
void Larvae::Init(INT2 pos, INT2 component)
{
	m_pSprite = new Sprite;
	m_pHero = NULL;
	m_isCollision = false;
	m_isDead = false;
	m_isDelete = false;
	m_Type = Type::LARVAE;
	m_Hp = 23;
	m_Atk = POWER;
	m_IndexPos = pos;
	m_Pos = ConvertIndexToPosition(pos);
	m_Component = component;
	m_MoveWeight = 0.6f;
	m_AtkCount = 0;
	isMove = true;
	m_pSprite->SetSpriteSize((float)(GRID_SIZE), (float)(GRID_SIZE));
	m_pSprite->SetSizeScaling(1.3f);
	m_pSprite->SetSplit(3, 4);
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;
	m_AtkCount = 0;
	StackInit();
	state = STATE::WAIT;
	m_Debug.Init();

	/*! �i�s�挈�� */
	{
		D3DXVECTOR2 dir[] = {
			Game::UP,Game::DOWN,
			Game::LEFT,Game::RIGHT,
		};
		for (auto i : dir) {
			INT2 index = m_IndexPos + Game::ConvertIndex(i);
			if (index.x < 0 || SOIL_WIDTH <= index.x)  { continue; }
			if (index.y < 0 || SOIL_HEIGHT <= index.y) { continue; }
			if (!TerrainManager::GetInstance().GetBreakFlag(index)) { continue; }
			m_Direction = i;
			break;
		}
		m_NextIndexPos = m_IndexPos + Game::ConvertIndex(m_Direction);
	}

	/*! ���ꏊ�̒ʒm */
	LocationManager::GetInstance().SetData(m_IndexPos, this);
}

/*
	@brief	���
*/
void Larvae::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	�X�V
*/
void Larvae::Update()
{

	switch (state)
	{
		/*! ���̈ړ������蓖�� */
	case Larvae::ASSIGN:

		break;
		/*! �ړ� */
	case Larvae::MOVE:
		if (!Move()) {
			EnemyManager::GetInstance().HitListAdd(this);/*!< �Փ˔��胊�X�g�֒ǉ� */
			IndexUpdate();
			/*! �����\�Ȃ� */
			if (CheckGrow()) {
				state = STATE::ACTION;
			}
		}
		break;
		/*! �U�� */
	case Larvae::ATK:
		EnemyManager::GetInstance().HitListAdd(this);/*!< �Փ˔��胊�X�g�֒ǉ� */
		state = STATE::MOVE;
		/*! Collision��Update�̌�ŌĂяo���Ă��邽�ߏՓ˂��Ă�����state��ATK�ɂȂ� */
		if (Wait()) {
			Attack();
			m_Count = 0;
		}
		break;
		/*! �ҋ@ */
	case Larvae::WAIT:
		if (Wait()) {
			EnemyManager::GetInstance().HitListAdd(this);/*!< �Փ˔��胊�X�g�֒ǉ� */
			m_Count = 0;/*!< �ҋ@�J�E���^�����Z�b�g */
			state = STATE::MOVE;
		}
		break;
		/*! �A�N�V���� */
	case Larvae::ACTION:
		Grow();
		break;
	default:
		break;
	}

	/*! �A�j���[�V�����X�V */
	AnimUpdate();
}

/*
	@brief	�`��
*/
void Larvae::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	�_���[�W
*/
void Larvae::Damage(int damage)
{
	m_Hp -= damage;
	if (m_Hp <= 0) {
		m_Hp = 0;
		OutComponent();
		m_isDelete = true;
		/*!< �f�[�^�̍폜 */
		LocationManager::GetInstance().DeleteData(m_IndexPos, this);
	}
}

/*
	@brief	�Փ�
*/
void Larvae::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	�R�P�Ƃ̏Փ�
*/
void Larvae::Collision(Moss & moss)
{
	
	/*! �Փː�Ɍ�����ς��� */
	///*
	INT2 dir = moss.GetIndex() - m_IndexPos;
	if (ConvertDirection(dir) != D3DXVECTOR2{ 0, 0 }) {
		m_Direction = ConvertDirection(dir);
	}
	StackInit();
	isMove = !Game::TheSamePositionToGrid(m_Pos);
	//*/
	Predator(moss);
}

/*
	@brief	�g�J�Q�Ƃ̏Փ�
*/
void Larvae::Collision(Lizard & lizard)
{
	isMove = false;
	/*! �Փː�Ɍ�����ς��� */
	INT2 dir = lizard.GetIndex() - m_IndexPos;
	m_Direction = ConvertDirection(dir);
}

/*
	@brief	��m�Ƃ̏Փ�
*/
void Larvae::Collision(Soldier & soldier)
{
	state = STATE::ATK;
}

/*
	@brief	�C���f�b�N�X�X�V
*/
void Larvae::IndexUpdate()
{
	auto before = m_IndexPos;
	m_IndexPos = ConvertPositionToIndex(m_Pos);/*!< �C���f�b�N�X�X�V */

	/*! ���ꏊ���X�V */
	LocationManager::GetInstance().DeleteData(before, this);	/*!< �f�[�^�̍폜 */
	LocationManager::GetInstance().SetData(m_IndexPos, this);	/*!< �f�[�^�̃Z�b�g */

	/*! ����� */
	D3DXVECTOR2 dirList[] = {
		ConvertLocalDirection(m_Direction,FORWARD),
		ConvertLocalDirection(m_Direction,LEFT),
		ConvertLocalDirection(m_Direction,RIGHT),
	};

	/*! �ړ��\�������X�^�b�N�Ɋi�[ */
	for (auto i : dirList) {
		INT2 pos = m_IndexPos + ConvertIndex(i);
		if (pos.x < 0 || SOIL_WIDTH <= pos.x) { continue; }
		if (pos.y < 0 || SOIL_HEIGHT <= pos.y) { continue; }
		if (!TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		StackPushDirection(i);
	}

	/*! �i�[�����̎��o�� */
	if (StackElementsNum() != 0) {
		int index = rand() % StackElementsNum();	/*!< �����ŃC���f�b�N�X������ */
		D3DXVECTOR2 dir = m_StackDirection[index];
		m_Direction = dir;
		m_NextIndexPos = m_IndexPos + ConvertIndex(dir);
	}
	/*! �i�s�����̌�₪�Ȃ� */
	else {
		D3DXVECTOR2 dir = ConvertLocalDirection(m_Direction, BACK);
		m_Direction = dir;
		m_NextIndexPos = m_IndexPos + ConvertIndex(dir);
	}
	/*! �X�^�b�N�̏����� */
	StackInit();

	/*! HP�����炷 */
	Damage(1);

}

/*
	@brief	�ۗL�{���E�����̕��o
*/
void Larvae::OutComponent()
{
	INT2 around[] = {
		{ -1,-1 },{ 0,-1 },{ 1,-1 },
		{ -1, 0 }	  ,	   { 1, 0 },
		{ -1, 1 },{ 0, 1 },{ 1, 1 },
	};

	/*! ���o�ł��邩���� */
	int output = false;

	/*! ���͂ɕ��o�\�ȃu���b�N�����邩���� */
	for (auto i : around) {
		INT2 pos = m_IndexPos + i;
		/*! �O���Q�� */
		if (pos.x < 0 || SOIL_WIDTH <= pos.x) { continue; }
		if (pos.y < 0 || SOIL_HEIGHT <= pos.y) { continue; }
		/*! �u���b�N�L�� */
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		output = true;
		break;
	}

	/*! ���o�o���Ȃ���Ώ������Ȃ� */
	if (!output) { return; }

	for (int i = 0;; i++) {
		/*! ���o����{���E���������� */
		if (m_Component == INT2(0, 0)) { break; }/*!< �����𔲂��� */
		/*! ���o�ʒu */
		INT2 pos = m_IndexPos + around[i % (sizeof(around) / sizeof(INT2))];
		/*! �O���Q�� */
		if (pos.x < 0 || SOIL_WIDTH <= pos.x)  { continue; }
		if (pos.y < 0 || SOIL_HEIGHT <= pos.y) { continue; }
		/*! �u���b�N�L�� */
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		/*! �{���E�����̂ǂ��炪��������r */
		INT2 tmp = TerrainManager::GetInstance().GetComponent(pos);
		int component = tmp.x >= tmp.y ? tmp.x : tmp.y;
		if (component >= MAX_COMPONENT) { continue; }
		
		/*! ���o��̃u���b�N�̗{���E����������̏ꍇ���Z���Ȃ� */
		if (component >= MAX_COMPONENT) {

			/*! �{�������� */
			if (tmp.x >= tmp.y) {
				m_Component.x -= 1;
			}
			/*! ���������� */
			else {
				m_Component.y -= 1;
			}
			continue;
		}
		component += 1;/*!< 1�����o���Ă��� */

		/*! �{�������� */
		if (tmp.x >= tmp.y) {
			m_Component.x -= 1;
			TerrainManager::GetInstance().
				SetComponent(pos, { component,tmp.y });
		}
		/*! ���������� */
		else {
			m_Component.y -= 1;
			TerrainManager::GetInstance().
				SetComponent(pos, { tmp.x,component });
		}
	}
}

/*
	@brief	�ړ�
*/
bool Larvae::Move()
{
	if (isMove) {
		m_Pos += m_Direction*m_MoveWeight;
	}

	/*! ���W���ړ���𒴂��Ă�����␳ */
	{
		bool isOver = false;/*!< ���W�̒��߃t���O */
		D3DXVECTOR2 pos = ConvertIndexToPosition(m_NextIndexPos);
		/*! ���W���ߔ��� */
		if (m_Direction == UP&&pos.y >= m_Pos.y)			{ isOver = true; }	/*!< �� */
		else if (m_Direction == DOWN&&pos.y <= m_Pos.y)   { isOver = true; }	/*!< �� */
		else if (m_Direction == LEFT&&pos.x >= m_Pos.x)   { isOver = true; }	/*!< �� */
		else if (m_Direction == RIGHT&&pos.x <= m_Pos.x)  { isOver = true; }	/*!< �E */

		if (isOver) {
			m_Pos = pos;
		}
	}

	return !Game::TheSamePositionToGrid(m_Pos);
}

/*
	@brief	�ҋ@
	@return	�I��:���I
*/
bool Larvae::Wait()
{
	m_Count++;
	if (m_Count < WAIT_COUNT) { return false; }
	return true;
}

/*
	@brief	�U��
*/
void Larvae::Attack()
{
	m_pHero->Damage(m_Atk);
}

/*
	@brief	�����ł��邩����
*/
bool Larvae::CheckGrow()
{
	return GROW_HP < m_Hp&&GROW_COMPONENT < m_Component.x;
}

/*! ���� */
void Larvae::Grow()
{
	Chrysalis* grow = new Chrysalis;
	grow->Init(m_IndexPos, m_Component, m_Hp);
	EnemyManager::GetInstance().AddEnemy(grow);
	/*!< �f�[�^�̍폜 */
	LocationManager::GetInstance().DeleteData(m_IndexPos, this);
	m_isDelete = true;
}

/*
	@brief	�ߐH
*/
void Larvae::Predator(Moss & moss)
{
	m_Hp += m_Atk < moss.GetHp() ? m_Atk : moss.GetHp();
	m_Component.x += moss.GetComponent().x;
	moss.Damage(m_Atk); 
	if (moss.GetDeleteFlags()) {
		isMove = true;
	}
}

/*
	@brief	�X�^�b�N�̏�����
*/
void Larvae::StackInit()
{
	for (auto &i : m_StackDirection) {
		i = INITIALIZE_STACK_ELEMENTS;
	}
	m_StackIndex = 0;
}

/*
	@brief	�X�^�b�N�ւ̃v�b�V��
*/
void Larvae::StackPushDirection(D3DXVECTOR2 direction)
{
	m_StackDirection[m_StackIndex] = direction;
	m_StackIndex++;
}
/*
@brief	�A�j���[�V�����X�V
*/
void Larvae::AnimUpdate()
{
	m_AnimCount++;

	if (m_AnimCount > 8) {
		m_AnimIndexU++;
		if (m_AnimIndexU > 2)m_AnimIndexU = 0;
		m_AnimCount = 0;
	}

	if (m_Direction == Game::UP) {
		m_AnimIndexV = 3;
	}
	else if (m_Direction == Game::DOWN) {
		m_AnimIndexV = 0;
	}
	else if (m_Direction == Game::LEFT) {
		m_AnimIndexV = 1;
	}
	else if (m_Direction == Game::RIGHT) {
		m_AnimIndexV = 2;
	}
	m_AnimIndexU %= 3;
}

/*
	@brief	�X�^�b�N�Ɋi�[���ꂽ�v�f�̐���Ԃ�
	@return	�����l�łȂ��X�^�b�N�̗v�f��
*/
int Larvae::StackElementsNum()
{
	int ret = 0;
	for (auto i : m_StackDirection) {
		/*! �����l�ƈ�v���Ă��邩���� */
		if (i == INITIALIZE_STACK_ELEMENTS) { return ret; }
		ret++;/*!< �J�E���^�ɉ��Z */
	}
	/*! �X�^�b�N���ׂĂɒl�������Ă��� */
	return STACK_SIZE;
}
