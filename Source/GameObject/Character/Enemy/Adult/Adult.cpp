/*
	@file	Adult.cpp
	@brief	����
	@author	�ԏ� �G�P
*/
#include "Adult.h"
#include "../../../Manager/LocationManager/LocationManager.h"

/*
	@def	�萔�錾
*/
const unsigned int Adult::MAX_HP = 76;
const unsigned int Adult::BREEDING_HP = 20;
const unsigned int Adult::WAIT_COUNT = 45;
const unsigned int Adult::STACK_SIZE = sizeof(Adult::m_StackDirection) / sizeof(D3DXVECTOR2);
const D3DXVECTOR2 Adult::INITIALIZE_STACK_ELEMENTS = { 0,0 };

/*
	@brief	namespace
*/
using namespace Game;

/*
	@brief	�R���X�g���N�^
*/
Adult::Adult()
{
}

/*
	@brief	�f�X�g���N�^
*/
Adult::~Adult()
{
	Destroy();
}

/*
	@brief	������
*/
void Adult::Init(INT2 pos, INT2 component)
{
	m_pSprite = new Sprite;

	m_isDelete = false;
	m_Type = Type::ADULT;
	m_Hp = 23;
	m_Atk = 19;
	m_IndexPos = pos;
	m_Pos = ConvertIndexToPosition(pos);
	m_Component = component;
	m_MoveWeight = 0.6f;
	m_Count = 0;
	isMove = true;
	state = STATE::WAIT;
	m_pSprite->SetSpriteSize((float)(GRID_SIZE), (float)(GRID_SIZE));
	m_pSprite->SetSizeScaling(1.3f);
	m_pSprite->SetSplit(3, 4);
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;
	StackInit();

	/*! �i�s�挈�� */
	{
		D3DXVECTOR2 dir[] = {
			Game::UP,Game::DOWN,
			Game::LEFT,Game::RIGHT,
		};
		for (auto i : dir) {
			INT2 index = m_IndexPos + Game::ConvertIndex(i);
			if (index.x < 0 || SOIL_WIDTH <= index.x) { continue; }
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
void Adult::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	�X�V
*/
void Adult::Update()
{
	switch (state)
	{
		/*! ���̈ړ������蓖�� */
	case Adult::ASSIGN:

		break;
		/*! �ړ� */
	case Adult::MOVE:
		if (!Move()) {
			EnemyManager::GetInstance().HitListAdd(this);/*!< �Փ˔��胊�X�g�֒ǉ� */
			IndexUpdate();
		}
		break;
		/*! �U�� */
	case Adult::ATK:
		EnemyManager::GetInstance().HitListAdd(this);/*!< �Փ˔��胊�X�g�֒ǉ� */
		state = STATE::MOVE;
		/*! Collision��Update�̌�ŌĂяo���Ă��邽�ߏՓ˂��Ă�����state��ATK�ɂȂ� */
		if (Wait()) {
			Attack();
			m_Count = 0;
		}
		break;
		/*! �ҋ@ */
	case Adult::WAIT:
		if (Wait()) {
			EnemyManager::GetInstance().HitListAdd(this);/*!< �Փ˔��胊�X�g�֒ǉ� */
			m_Count = 0;/*!< �ҋ@�J�E���^�����Z�b�g */
			state = STATE::MOVE;
		}
		break;
		/*! �A�N�V���� */
	case Adult::ACTION:
		Breeding();
		state = STATE::WAIT;
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
void Adult::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	�_���[�W
*/
void Adult::Damage(int damage)
{
	m_Hp -= damage;
	if (m_Hp <= 0) {
		m_Hp = 0;
		OutComponent();
		/*!< �f�[�^�̍폜 */
		LocationManager::GetInstance().DeleteData(m_IndexPos, this);
		m_isDelete = true;
	}
}

/*
	@brief	�Փ�
*/
void Adult::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	�R�P�Ƃ̏Փ�
*/
void Adult::Collision(Moss & moss)
{
	StackInit();
	Predator(moss);
	if (CheckBreeding()) {
		state = STATE::ACTION;
	}
}

/*
	@brief	�Q�Ƃ̏Փ�
*/
void Adult::Collision(Buds & buds)
{
	StackInit();
	Predator(buds);
	if (CheckBreeding()) {
		state = STATE::ACTION;
	}
}

/*
	@brief	�ԂƂ̏Փ�
*/
void Adult::Collision(Flower & flower)
{
	StackInit();
	Predator(flower);
	if (CheckBreeding()) {
		state = STATE::ACTION;
	}
}

/*
	@brief	�g�J�Q�Ƃ̏Փ�
*/
void Adult::Collision(Lizard & lizard)
{
	/*! �Փː�Ɍ�����ς��� */
	INT2 dir = lizard.GetIndex() - m_IndexPos;
	m_Direction = ConvertDirection(dir);
	state = STATE::WAIT;
}

/*
		@brief	��m�Ƃ̏Փ�
*/
void Adult::Collision(Soldier & soldier)
{
	state = STATE::ATK;
}

/*
	@brief	�R�P�ߐH
*/
void Adult::Predator(Moss & moss)
{
	m_Hp += m_Atk < moss.GetHp() ? m_Atk : moss.GetHp();
	if (MAX_HP <= m_Hp) { m_Hp = MAX_HP; }
	m_Component.x += moss.GetComponent().x;
	moss.Damage(m_Atk);
}

/*
	@brief	�Q�ߐH
*/
void Adult::Predator(Buds & buds)
{
	m_Hp += m_Atk < buds.GetHp() ? m_Atk : buds.GetHp();
	if (MAX_HP <= m_Hp) { m_Hp = MAX_HP; }
	m_Component.x += buds.GetComponent().x;
	buds.Damage(m_Atk);
}

/*
	@brief	�ԕߐH
*/
void Adult::Predator(Flower & flower)
{
	m_Hp += m_Atk < flower.GetHp() ? m_Atk : flower.GetHp();
	if (MAX_HP <= m_Hp) { m_Hp = MAX_HP; }
	m_Component.x += flower.GetComponent().x;
	flower.Damage(m_Atk);
}

/*
	@brief	�c���ߐH
*/
void Adult::Predator(Larvae & larvae)
{
}

/*
	@brief	�C���f�b�N�X�X�V
*/
void Adult::IndexUpdate()
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
void Adult::OutComponent()
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
		if (pos.x < 0 || SOIL_WIDTH <= pos.x) { continue; }
		if (pos.y < 0 || SOIL_HEIGHT <= pos.y) { continue; }
		/*! �u���b�N�L�� */
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		/*! �{���E�����̂ǂ��炪��������r */
		INT2 tmp = TerrainManager::GetInstance().GetComponent(pos);
		int component = tmp.x >= tmp.y ? tmp.x : tmp.y;
		
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
bool Adult::Move()
{
	if (isMove) {
		m_Pos += m_Direction*m_MoveWeight;
	}

	/*! ���W���ړ���𒴂��Ă�����␳ */
	{
		bool isOver = false;/*!< ���W�̒��߃t���O */
		D3DXVECTOR2 pos = ConvertIndexToPosition(m_NextIndexPos);
		/*! ���W���ߔ��� */
		if (m_Direction == UP&&pos.y >= m_Pos.y) { isOver = true; }	/*!< �� */
		else if (m_Direction == DOWN&&pos.y <= m_Pos.y) { isOver = true; }	/*!< �� */
		else if (m_Direction == LEFT&&pos.x >= m_Pos.x) { isOver = true; }	/*!< �� */
		else if (m_Direction == RIGHT&&pos.x <= m_Pos.x) { isOver = true; }	/*!< �E */

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
bool Adult::Wait()
{
	m_Count++;
	if (m_Count < WAIT_COUNT) { return false; }
	return true;
}

/*
	@brief	�U��
*/
void Adult::Attack()
{
	m_pHero->Damage(m_Atk);
}

/*
	@brief	�ɐB�\������
*/
bool Adult::CheckBreeding()
{
	return BREEDING_HP < m_Hp&&m_Hp < MAX_HP - 3;
}

/*
	@brief	�ɐB
*/
void Adult::Breeding()
{
	/*! HP���� */
	m_Hp -= BREEDING_HP;

	/*! �ɐB */
	Larvae* child = new Larvae;
	INT2 halfComponent = {
		m_Component.x / 2,
		m_Component.y / 2,
	};
	child->Init(m_IndexPos, halfComponent);
	EnemyManager::GetInstance().AddEnemy(child);

}

/*
	@brief	�X�^�b�N�̏�����
*/
void Adult::StackInit()
{
	for (auto &i : m_StackDirection) {
		i = INITIALIZE_STACK_ELEMENTS;
	}
	m_StackIndex = 0;
}

/*
	@brief	�X�^�b�N�ւ̃v�b�V��
*/
void Adult::StackPushDirection(D3DXVECTOR2 direction)
{
	m_StackDirection[m_StackIndex] = direction;
	m_StackIndex++;
}

/*
	@brief	�X�^�b�N�Ɋi�[���ꂽ�v�f�̐���Ԃ�
	@return	�����l�łȂ��X�^�b�N�̗v�f��
*/
int Adult::StackElementsNum()
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

/*
	@brief	�A�j���[�V�����X�V
*/
void Adult::AnimUpdate()
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
