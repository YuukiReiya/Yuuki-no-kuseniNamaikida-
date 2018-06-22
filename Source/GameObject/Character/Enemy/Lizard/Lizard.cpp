/*
	@file	Lizard.h
	@brief	�g�J�Q
	@author	�ԏ� �G�P
*/
#include "Lizard.h"
#include "../../../Manager/LocationManager/LocationManager.h"

/*
	@def	�萔�錾
*/
const unsigned int Lizard::MAX_HP = 160;
const unsigned int Lizard::WAIT_COUNT = 30;
const unsigned int Lizard::MAKE_NEST_COMPONENT = 14;
const unsigned int Lizard::MADE_NEST_HP = 2;
const unsigned int Lizard::LAY_EGG_COMPONENT = 12;
const unsigned int Lizard::LAY_EGG_HP = 40;
const unsigned int Lizard::STACK_SIZE = sizeof(Lizard::m_Direction) / sizeof(D3DXVECTOR2);
const D3DXVECTOR2 Lizard::INITIALIZE_STACK_ELEMENTS = { 0,0 };

/*
	@brief	�R���X�g���N�^
*/
Lizard::Lizard()
{
}

/*
	@brief	�f�X�g���N�^
*/
Lizard::~Lizard()
{
	Destroy();
}

/*
	@brief	������
*/
void Lizard::Init(INT2 pos,INT2 component)
{
	m_pSprite = new Sprite;


	m_Type = Type::LIZARD;
	m_isDelete = false;
	m_Hp = MAX_HP;
	m_Atk = 35;
	m_IndexPos = pos;
	state = STATE::WAIT;
	m_Pos = Game::ConvertIndexToPosition(pos);
	m_Component = component;
	m_MoveWeight = 0.6f;
	m_Direction = Game::RIGHT;
	m_pSprite->SetSpriteSize((float)(GRID_SIZE), (float)(GRID_SIZE));
	m_pSprite->SetSplit(3, 4);
	m_pSprite->SetSizeScaling(1.3f);
	isMove = true;
	isStraight = true;
	isMadeNest = false;
	m_Count = 0;
	m_NestIndexPos = pos;
	m_AtkCount = 0;
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;
	StackInit();

	/*! �i�s�挈�� */
	{

		D3DXVECTOR2 dir[] = {
			Game::UP,Game::DOWN,
			Game::LEFT,Game::RIGHT,
		};
		m_Direction = dir[rand() % sizeof(dir) / sizeof(D3DXVECTOR2)];
		m_Direction = CalculateDirection();
		m_NextIndexPos = m_IndexPos + Game::ConvertIndex(m_Direction);
	}

	/*! ���ꏊ�̒ʒm */
	LocationManager::GetInstance().SetData(m_IndexPos, this);
}

/*
	@brief	���
*/
void Lizard::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	�X�V
*/
void Lizard::Update()
{
	switch (state)
	{
	/*! ���̈ړ������蓖�� */
	case Lizard::ASSIGN:

		break;
	/*! �ړ� */
	case Lizard::MOVE:
		if (!Move()) {
			EnemyManager::GetInstance().HitListAdd(this);/*!< �Փ˔��胊�X�g�֒ǉ� */
			state = STATE::MOVE;
			IndexUpdate();
		}
		break;
	/*! �U�� */
	case Lizard::ATK:
		EnemyManager::GetInstance().HitListAdd(this);/*!< �Փ˔��胊�X�g�֒ǉ� */
		state = STATE::MOVE;
		/*! Collision��Update�̌�ŌĂяo���Ă��邽�ߏՓ˂��Ă�����state��ATK�ɂȂ� */
		if (Wait()) {
			Attack();
			m_Count = 0;
		}
		break;
	/*! �ҋ@ */
	case Lizard::WAIT:
		if (Wait()) {
			EnemyManager::GetInstance().HitListAdd(this);/*!< �Փ˔��胊�X�g�֒ǉ� */
			m_Count = 0;/*!< �ҋ@�J�E���^�����Z�b�g */
			state = STATE::MOVE;
		}
		break;
	/*! �A�N�V���� */
	case Lizard::ACTION:
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
void Lizard::Render(IDirect3DTexture9* texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	�_���[�W
*/
void Lizard::Damage(int damage)
{
	m_Hp -= damage;
	if (m_Hp <= 0) {
		m_Hp = 0;
		m_isDelete = true;
		/*!< �f�[�^�̍폜 */
		LocationManager::GetInstance().DeleteData(m_IndexPos, this);
	}
}

/*
	@brief	�Փ�
*/
void Lizard::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	�c���ƏՓ�
*/
void Lizard::Collision(Larvae & larvae)
{
	
	/*! �Փː�Ɍ�����ς��� */
	INT2 dir = larvae.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);
	m_Component.x += larvae.GetComponent().x;
	larvae.Damage(m_Atk);
}

/*
	@brief	匂ƏՓ�
*/
void Lizard::Collision(Chrysalis & chrysalis)
{
	/*! �Փː�Ɍ�����ς��� */
	INT2 dir = chrysalis.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);
	m_Component.x += chrysalis.GetComponent().x;
	chrysalis.Damage(m_Atk);
}

/*
	@brief	�����ƏՓ�
*/
void Lizard::Collision(Adult & adult)
{
	/*! �Փː�Ɍ�����ς��� */
	INT2 dir = adult.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);
	m_Component.x += adult.GetComponent().x;
	adult.Damage(m_Atk);
}

/*
	@brief	��m�Ƃ̏Փ�
*/
void Lizard::Collision(Soldier & solder)
{
	state = STATE::ATK;
}

/*
	@brief	�����Y�߂邩����
*/
bool Lizard::CheckLay()
{
	return LAY_EGG_HP < m_Hp &&  LAY_EGG_COMPONENT < m_Component.x&&m_Hp < 130;
}

/*
	@brief	�����Y��
*/
void Lizard::Lay()
{
	m_Hp -= LAY_EGG_HP;
	m_Component.x -= LAY_EGG_COMPONENT;

	Egg*  egg = new Egg;
	egg->Init(m_IndexPos, m_Component);
	EnemyManager::GetInstance().AddEnemy(egg);
}

/*
	@brief	�C���f�b�N�X�X�V
*/
void Lizard::IndexUpdate()
{
	auto before = m_IndexPos;
	m_IndexPos = Game::ConvertPositionToIndex(m_Pos);/*!< �C���f�b�N�X�X�V */

	/*! �C���f�b�N�X�ɕύX���Ȃ������珈���𔲂��� */
	if (m_IndexPos == before) { return; }

	/*! ���ꏊ���X�V */
	LocationManager::GetInstance().DeleteData(before, this);	/*!< �f�[�^�̍폜 */
	LocationManager::GetInstance().SetData(m_IndexPos, this);	/*!< �f�[�^�̃Z�b�g */

	if (!isMadeNest&&CheckMakeNest()) {
		MakeNest();
	}

	if (isStraight)
		StraightMoveIndexUpdate();
	else
		NestMoveIndexUpdate();
}

/*
	@brief	�A�j���[�V�����X�V
*/
void Lizard::AnimUpdate()
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
	@brief	�ړ�
*/
bool Lizard::Move()
{
	if (isMove) {
		m_Pos += m_Direction*m_MoveWeight;
	}
	/*! ���W���ړ���𒴂��Ă�����␳ */
	{
		bool isOver = false;/*!< ���W�̒��߃t���O */
		D3DXVECTOR2 pos = Game::ConvertIndexToPosition(m_NextIndexPos);
		/*! ���W���ߔ��� */
		if (m_Direction == Game::UP&&pos.y >= m_Pos.y) { isOver = true; }			/*!< �� */
		else if (m_Direction == Game::DOWN&&pos.y <= m_Pos.y) { isOver = true; }	/*!< �� */
		else if (m_Direction == Game::LEFT&&pos.x >= m_Pos.x) { isOver = true; }	/*!< �� */
		else if (m_Direction == Game::RIGHT&&pos.x <= m_Pos.x) { isOver = true; }	/*!< �E */

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
bool Lizard::Wait()
{
	m_Count++;
	if (m_Count < WAIT_COUNT) { return false; }
	return true;
}

/*
	@brief	�U��
*/
void Lizard::Attack()
{
	m_pHero->Damage(m_Atk);
}

/*
	@brief	�������邩����
*/bool Lizard::CheckMakeNest()
{
	/*! HP�Ɨ{�������ȏ�Ȃ���Ώ������Ȃ� */
	if (m_Hp <= MADE_NEST_HP || m_Component.x <= MAKE_NEST_COMPONENT) {
		return false;
	}
	/*! ���͂̃}�X�𔻒� */
	INT2 around[8]{
		{ -1, -1},{  0,-1 },{  1,-1 },
		{ -1, 0 },			{  1, 0 },
		{ -1, 1 },{  0, 1 },{  1, 1 },
	};
	int count = 0;/*!< �ړ��\�ȃ}�X�̌� */
	for (int i = 0; i < sizeof(around) / sizeof(INT2); i++) {
		INT2 index = m_IndexPos + around[i];
		/*! �O���Q�� */
		if (index.x < 0 || index.x >= SOIL_WIDTH)	{ continue; }
		if (index.y < 0 || index.y >= SOIL_HEIGHT)	{ continue; }
		/*! �u���b�N�L�� */
		if (TerrainManager::GetInstance().GetBreakFlag(index)) { count++; }
	}
	/*! ����5�}�X���ړ��\ */
	if (5 <= count) { return true; }
	return false;
}

/*
	@brief	�������
*/
void Lizard::MakeNest()
{
	/*
		���̃C���X�^���X��������
	*/
	m_NestIndexPos = m_IndexPos;
	isMadeNest = true;
	isStraight = false;
}

/*
	@brief	�ۗL�{���E�����̎��͂ւ̕��o
*/
void Lizard::OutComponent()
{
	INT2 around[] = {
		{ -1,-1 },{ 0,-1 },{ 1,-1 },
		{ -1, 0 }	  ,{ 1, 0 },
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

	/*! ���o�ł���u���b�N���Ȃ� */
	if (!output) { return; }

	for (int i = 0;; i++) {
		/*! ���o����{���E���������� */
		if (m_Component == INT2(0, 0)) { break; }/*!< �����𔲂��� */
												 /*! ���o�ʒu */
		INT2 pos = m_IndexPos + around[i %( sizeof(around) / sizeof(INT2))];
		/*! �O���Q�� */
		if (pos.x < 0 || SOIL_WIDTH <= pos.x) { continue; }
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
		component += 1;

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
	@brief	�i�s�����̎Z�o
	@detail	���݌����Ă����������ړ��\�ȕ������Z�o
*/
D3DXVECTOR2 Lizard::CalculateDirection()
{
	D3DXVECTOR2 dir;
	INT2 temp;

	/*! ���� */
	D3DXVECTOR2 way[3] = {
		Game::FORWARD,
		Game::RIGHT,
		Game::LEFT,
	};

	for (int i = 0; i < sizeof(way) / sizeof(D3DXVECTOR2); i++) {
		dir = Game::ConvertLocalDirection(m_Direction, way[i]);
		temp = Game::ConvertIndex(dir);
		temp = m_IndexPos + temp;
		if (temp.x < 0 || temp.x >= SOIL_WIDTH)		{ continue; }
		if (temp.y < 0 || temp.y >= SOIL_HEIGHT)	{ continue; }
		if (TerrainManager::GetInstance().GetBreakFlag(temp))return dir;
	}
	//���E���i�߂Ȃ��̂ŗ��������֌�����ς���
	dir = Game::ConvertLocalDirection(m_Direction, Game::BACK);
	return dir;
}

/*
	@brief	���i�ړ����邽�߂̃C���f�b�N�X�X�V
*/
void Lizard::StraightMoveIndexUpdate()
{
	m_Direction = CalculateDirection();
	m_NextIndexPos = m_IndexPos + Game::ConvertIndex(m_Direction);
}

/*
	@brief	���̎��͂�p�j����ړ�
*/void Lizard::NestMoveIndexUpdate()
{
	/*! ���̂���ꏊ�ɋ����� */
	if (m_IndexPos == m_NestIndexPos) {
		/*! 4�� */
		INT2 corner[]{
			{-1,-1},{1,-1},
			{-1, 1},{1, 1},
		};
		/*! �ړ��ڕW������ */
		std::vector<INT2>stack;
		for (auto i : corner) {
			INT2 target = m_IndexPos + i;
			/*! �O���Q�� */
			if (target.x < 0 || SOIL_WIDTH <= target.x)  { continue; }
			if (target.y < 0 || SOIL_HEIGHT <= target.y) { continue; }
			/*! �u���b�N�����ĂȂ� */
			if (!TerrainManager::GetInstance().GetBreakFlag(target)) { continue; }
			/*! 
				������
				������	������̂悤�ȏ�Ԃ͈ړ��ڕW�ɐݒ肵�Ȃ�
				������
			*/
			INT2 v = m_IndexPos + INT2(0, i.y);/*!< �c */
			INT2 h = m_IndexPos + INT2(i.x, 0);/*!< �� */
			if (TerrainManager::GetInstance().GetBreakFlag(v) ||
				TerrainManager::GetInstance().GetBreakFlag(h)) {
				stack.push_back(target);/*!< �ړ��ڕW�����X�^�b�N�ɐς� */
			}
		}
		
		/*! �f�[�^�̎��o�� */
		m_GoalIndexPos = stack[rand() % stack.size()];

		/*! �ڕW�n�_�܂ł̍ŒZ�o�H��T�� */
		m_aStar.Init(
			m_IndexPos.x, m_IndexPos.y,
			m_GoalIndexPos.x, m_GoalIndexPos.y,
			SOIL_WIDTH, SOIL_HEIGHT);
		m_aStar.Explore();

		/*! �ŒZ�o�H�̎��o�� */
		m_aStar.RootHash.pop_back();
		auto next = m_aStar.GetRoot();
		D3DXVECTOR2 dir = {
			(float)(next.x - m_IndexPos.x),
			(float)(next.y - m_IndexPos.y)
		};
		m_Direction = dir;
		m_NextIndexPos = { next.x,next.y };

		/*! ���[�J�����I�z��̉�� */
		stack.clear();
		stack.shrink_to_fit();
		
		/*! �����Y�߂邩���� */
		if (CheckLay()) {
			Lay();
		}
	}
	/*! �ړ��ڕW�ɓ��B */
	else if (m_IndexPos == m_GoalIndexPos) {
		/*! ���̈ʒu���ړ��ڕW�� */
		m_GoalIndexPos = m_NestIndexPos;
		/*! �ŒZ�o�H��T�� */
		m_aStar.Init(
			m_IndexPos.x, m_IndexPos.y,
			m_GoalIndexPos.x, m_GoalIndexPos.y,
			SOIL_WIDTH, SOIL_HEIGHT);
		m_aStar.Explore();	
		/*! �ŒZ�o�H�����o�� */
		m_aStar.RootHash.pop_back();
		auto next = m_aStar.GetRoot();
		D3DXVECTOR2 dir = {
			(float)(next.x - m_IndexPos.x),
			(float)(next.y - m_IndexPos.y)
		};
		m_Direction = dir;
		m_NextIndexPos = { next.x,next.y };
	}
	/*! ���p�n�_ */
	else {
		auto next = m_aStar.GetRoot();
		D3DXVECTOR2 dir = {
			(float)(next.x - m_IndexPos.x),
			(float)(next.y - m_IndexPos.y)
		};
		m_Direction = dir;
		m_NextIndexPos = { next.x,next.y };
	}
}

/*
	@brief	�X�^�b�N�̏�����
*/
void Lizard::StackInit()
{
	for (auto &i : m_StackDirection) {
		i = INITIALIZE_STACK_ELEMENTS;
	}
	m_StackIndex = 0;
}

/*
	@brief	�X�^�b�N�ւ̃v�b�V��
*/
void Lizard::StackPushDirection(D3DXVECTOR2 direction)
{
	m_StackDirection[m_StackIndex] = direction;
	m_StackIndex++;
}

/*
	@brief	�X�^�b�N�Ɋi�[���ꂽ�v�f�̐���Ԃ�
	@return	�����l�łȂ��X�^�b�N�̗v�f��
*/
int Lizard::StackElementsNum()
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
