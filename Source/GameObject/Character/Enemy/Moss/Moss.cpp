/*
	@file	Moss.cpp
	@brief	�R�P
	@author	�ԏ� �G�P
*/
#include "Moss.h"
#include "../../../Manager/LocationManager/LocationManager.h"

/*
	@def	�萔�錾
*/	
const unsigned int Moss::GROW_COMPONENT = 2;
const unsigned int Moss::GROW_HP = 2;
const unsigned int Moss::RECOVERY_POWER = 2;
const unsigned int Moss::MAX_HP = 21;
const unsigned int Moss::WAIT_COUNT = 30;

/*
	�R���X�g���N�^
*/
Moss::Moss()
{
}

/*
	�f�X�g���N�^
*/
Moss::~Moss()
{
	Destroy();
}

/*
	@brief	������
*/
void Moss::Init(INT2 pos, INT2 component)
{
	m_pSprite = new Sprite;
	m_pHero = NULL;
	m_isCollision = false;
	m_isDead = false;
	m_isDelete = false;
	m_Type = Type::MOSS;
	m_Hp = 16;
	m_Atk = 13;
	m_IndexPos = pos;
	m_Pos = Game::ConvertIndexToPosition(pos);
	m_Component = component;
	m_MoveWeight = 0.3f;
	isMove = true;
	state = STATE::WAIT;
	m_pSprite->SetSpriteSize((float)GRID_SIZE, (float)GRID_SIZE);
	m_pSprite->SetSplit(3, 4);

	m_AbsorptionPow = 3;
	isAgain = false;
	isAbsorp = true;
	m_Count = 0;
	
	m_Debug.Init();
	m_Debug.SetLineWidth(1.0f);
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;

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
void Moss::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	�X�V
*/
void Moss::Update()
{

	switch (state)
	{
	/*! ���̈ړ������蓖�� */
	case Moss::ASSIGN:

		break;
	/*! �ړ� */
	case Moss::MOVE:
		if (!Move()) {
			EnemyManager::GetInstance().HitListAdd(this);/*!< �Փ˔��胊�X�g�֒ǉ� */
			state = STATE::MOVE;
			IndexUpdate();
			/*! �{���z�� */
			if (isAbsorp) {
				/*! �{���z���o���邩���� */
				if (CheckAbsorption()) {
					state = STATE::ACTION;
				}
			}
			/*! �{�����o */
			else {
				/*! �{�����o�\������ */
				if (CheckDivergence()) {
					state = STATE::ACTION;
				}
			}
			/*! ���� */
			if (CheckGrow()) {
				Grow();
			}
		}
		break;
	/*! �U�� */
	case Moss::ATK:
		EnemyManager::GetInstance().HitListAdd(this);/*!< �Փ˔��胊�X�g�֒ǉ� */
		state = STATE::MOVE;
		/*! Collision��Update�̌�ŌĂяo���Ă��邽�ߏՓ˂��Ă�����state��ATK�ɂȂ� */
		if (Wait()) {
			Attack();
			m_Count = 0;
		}
		break;
	/*! �ҋ@ */
	case Moss::WAIT:
		if (Wait()) {
			/*! �ҋ@�I�� */
			EnemyManager::GetInstance().HitListAdd(this);/*!< �Փ˔��胊�X�g�֒ǉ� */
			m_Count = 0;/*!< �ҋ@�J�E���^�����Z�b�g */
			state = STATE::MOVE;
		}
		break;
	/*! �A�N�V���� */
	case Moss::ACTION:
		if (Wait()) {
			/*! �ҋ@�I�� */
			m_Count = 0;/*!< �ҋ@�J�E���^�����Z�b�g */
			if (isAbsorp) {
				Absorption();/*!< �{���z�� */
			}
			else {
				Divergence();/*!< �{�����o */
			}

			state = STATE::MOVE;
		}
		break;
	default:
		break;
	}

	/*! �A�j���[�V�����X�V */
	AnimUpdate();

	/*! �ړ��t���O��߂� */
	if (!isMove) { isMove = true; }
}

/*
	@brief	�`��
*/
void Moss::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	�_���[�W
*/
void Moss::Damage(int damage)
{
	m_Hp -= damage;
	if (m_Hp <= 0) {
		m_Hp = 0;
		/*!< �f�[�^�̍폜 */
		LocationManager::GetInstance().DeleteData(m_IndexPos, this);
		m_isDelete = true;
	}
}

/*
	@brief	�Փ�
*/
void Moss::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	�c���Ƃ̓����蔻��
*/
void Moss::Collision(Larvae& larvae)
{
	/*! �ړ����~�߂� */
	isMove = false;
	/*! �Փː�Ɍ�����ς��� */
	INT2 dir = larvae.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);

}

/*
	@brief	�����Ƃ̓����蔻��
*/
void Moss::Collision(Adult & adult)
{
	/*! �ړ����~�߂� */
	isMove = false;
	/*! �Փː�Ɍ�����ς��� */
	INT2 dir = adult.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);
}

/*
	@brief	��m�Ƃ̏Փ�
*/
void Moss::Collision(Soldier & solder)
{
	state = STATE::ATK;
}

/*
	@brief	�ҋ@
	@return	�I��:���I
*/
bool Moss::Wait()
{
	m_Count++;
	if (m_Count < WAIT_COUNT) { return false; }
	return true;
}

/*
	@brief	�U��
*/
void Moss::Attack()
{
	m_pHero->Damage(m_Atk);
}

/*
	@brief	�C���f�b�N�X�̍X�V
*/
void Moss::IndexUpdate()
{
	auto before = m_IndexPos;
	m_IndexPos = Game::ConvertPositionToIndex(m_Pos);/*!< �C���f�b�N�X�X�V */
	
	/*! ���ꏊ���X�V */
	LocationManager::GetInstance().DeleteData(before, this);	/*!< �f�[�^�̍폜 */
	LocationManager::GetInstance().SetData(m_IndexPos, this);	/*!< �f�[�^�̃Z�b�g */

	/*! HP�̌������� */
	Damage(1);

	/*! �i�s�����̎Z�o */
	m_Direction = CalculateDirection();
	INT2 dir = Game::ConvertIndex(m_Direction);
	m_NextIndexPos = m_IndexPos + dir;
}

/*
	@brief	�ړ�
	@return	�I��:���I
*/
bool Moss::Move()
{
	if (isMove) {
		m_Pos += m_Direction*m_MoveWeight;
	}

	/*! ���W���ړ���𒴂��Ă�����␳ */
	{
		bool isOver = false;/*!< ���W�̒��߃t���O */
		D3DXVECTOR2 temp = Game::ConvertIndexToPosition(m_NextIndexPos);
		/*! ���W���ߔ��� */
		if (m_Direction == Game::UP&&temp.y >= m_Pos.y)			{ isOver = true; }	/*!< �� */
		else if (m_Direction == Game::DOWN&&temp.y <= m_Pos.y)	{ isOver = true; }	/*!< �� */
		else if (m_Direction == Game::LEFT&&temp.x >= m_Pos.x)	{ isOver = true; }	/*!< �� */
		else if (m_Direction == Game::RIGHT&&temp.x <= m_Pos.x) { isOver = true; }	/*!< �E */

		if (isOver) {
			m_Pos = temp;
		}
	}
	return !Game::TheSamePositionToGrid(m_Pos);
}

/*
	@brief	�{���z���o���邩����
*/
bool Moss::CheckAbsorption()
{
	INT2 around[] = {
		{  0,-1 },{ 0, 1 },
		{ -1, 0 },{ 1, 0 },
	};
	for (auto i : around) {
		INT2 pos = m_IndexPos + i;
		if (pos.x < 0 || pos.x >= SOIL_WIDTH) { continue; }
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT) { continue; }
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		/*! �אڃ}�X�̗{�� */
		auto component = TerrainManager::GetInstance().GetComponent(pos);
		if (component.x == 0) { continue; }/*!< �{��0�̃u���b�N */
		return true;/*!< �l���̃u���b�N�̂ǂꂩ�͗{���z�����o���� */
	}
	return false;
}

/*
	@brief	�{���z��
*/
void Moss::Absorption()
{
	INT2 way[] = {
		{  0,-1 },{ 0, 1 },
		{ -1, 0 },{ 1, 0 },
	};

	for (auto i : way) {
		INT2 pos = m_IndexPos + i;
		if (pos.x < 0 || pos.x >= SOIL_WIDTH)	{ continue; }
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT)	{ continue; }
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		/*! �אڃ}�X�̗{�� */
		auto component = TerrainManager::GetInstance().GetComponent(pos);
		if (component.x == 0) { continue; }/*!< �{��0�̃u���b�N */
		/*! �{���̋z�� */
		component.x -= m_AbsorptionPow;
		m_Component.x += m_AbsorptionPow;
		m_Hp += RECOVERY_POWER;/*! ��{�񕜗� */

		/*! �ۗL�{�����ő�l������ */
		if (MAX_COMPONENT < m_Component.x) {
			m_Component.x = MAX_COMPONENT;
		}

		/*! �u���b�N�̗{�����ŏ��l������� */
		if (component.x < 0) {
			m_Component.x += (m_AbsorptionPow - abs(component.x));/*!< �z���\�����z����� */
			component.x = 0;/* �u���b�N�̗{����0 */
			m_Hp -= 1;		/*!< �z���ʂ��Ⴂ�̂�HP�񕜗ʂ����炷 */
		}

		/*! �u���b�N�̗{���㏑�� */
		TerrainManager::GetInstance().SetComponent(pos, component);
		isAbsorp = false;
		break;
	}
}

/*
	@brief	�{�����o�\������
*/
bool Moss::CheckDivergence()
{
	/*! ���� */
	INT2 around[4] = {
		{ 0,-1 },{ 0, 1 },
		{ -1,0 },{ 1, 0 },
	};
	for (auto i : around) {
		INT2 pos = m_IndexPos + i;
		/*! �͈͊O */
		if (pos.x < 0 || pos.x >= SOIL_WIDTH) { continue; }
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT) { continue; }
		/*! �u���b�N�����Ă� */
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		/*! �אڃ}�X�{�� */
		auto component = TerrainManager::GetInstance().GetComponent(pos);
		if (component.x <= 1) { continue; }/*!< 1�ȉ��̃}�X�ɂ͕��o���Ȃ� */
		return true;
	}
	return false;
}

/*
	@brief	�{�����o
*/
void Moss::Divergence()
{
	/*! ���� */
	INT2 way[4] = {
		{ 0,-1 },
		{ 0,1 },
		{ -1,0 },
		{ 1, 0 },
	};
	for (int i = 0; i < sizeof(way) / sizeof(INT2); i++) {
		INT2 pos = m_IndexPos + way[i];
		/*! �͈͊O */
		if (pos.x < 0 || pos.x >= SOIL_WIDTH)  { continue; }
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT) { continue; }
		/*! �u���b�N�����Ă� */
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }
		/*! �אڃ}�X�{�� */
		auto component = TerrainManager::GetInstance().GetComponent(pos);
		if (component.x <= 1)continue;/*!< 1�ȉ��̃}�X�ɂ͕��o���Ȃ� */
		/*! �{���̕��o */
		component.x += (m_Component.x - 1);
		m_Component.x = 1;
		TerrainManager::GetInstance().SetComponent(pos, component);
		isAbsorp = true;
	}
}

/*
	@brief	�ۗL�{���E�����̎��͂ւ̕��o
*/
void Moss::OutComponent()
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
	@brief	�����o���邩����

*/
bool Moss::CheckGrow()
{
	return m_Hp <= GROW_HP&&GROW_COMPONENT <= m_Component.x;
}

/*
	@brief	����
*/
void Moss::Grow()
{
	Buds* grow = new Buds;
	grow->Init(m_IndexPos, m_Component, m_Hp);
	EnemyManager::GetInstance().AddEnemy(grow);
	m_isDelete = true;
	/*!< �f�[�^�̍폜 */
	LocationManager::GetInstance().DeleteData(m_IndexPos, this);
}


/*
	@brief	�ړ��\�����̎Z�o
	@detail	���݌����Ă����������ړ��\�ȕ������Z�o
*/
D3DXVECTOR2 Moss::CalculateDirection()
{
	/*! ���� */
	D3DXVECTOR2 way[] = {
		Game::FORWARD,
		Game::RIGHT,
		Game::LEFT,
		Game::BACK,
	};

	for (auto i : way) {
		D3DXVECTOR2 dir = Game::ConvertLocalDirection(m_Direction, i);/*!< ���[�J���Ȍ��� */
		INT2 pos = m_IndexPos + Game::ConvertIndex(dir);/*!< �����C���f�b�N�X */
		/*! �O���Q�� */
		if (pos.x < 0 || pos.x >= SOIL_WIDTH)  { continue; }
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT) { continue; }
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { return dir; }
	}
	return D3DXVECTOR2(0, 0);
}

/*
	@brief	�A�j���[�V�����̍X�V
*/
void Moss::AnimUpdate()
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

