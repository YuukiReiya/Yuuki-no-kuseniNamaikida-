/*
	@file	Flower.cpp
	@brief	��
	@author	�ԏ� �G�P
*/
#include "Flower.h"
#include "../../../Manager/LocationManager/LocationManager.h"

/*
	@def	�萔�錾
*/
const unsigned int Flower::ACTION_COUNT = 60;
const unsigned int Flower::ABSORPTION_POWER = 3;
const unsigned int Flower::ABSORPTION_OVER_LIMIT = 11;
const unsigned int Flower::ABSORPTION_UNDER_LIMIT = 0;
const unsigned int Flower::BREEDING_COMPONENT = 11;
const unsigned int Flower::BREEDING_COUNT = 8;
const unsigned int Flower::MAX_HP = 21;

/*
	@brief�@�R���X�g���N�^
*/
Flower::Flower()
{
}

/*
	@brief	�f�X�g���N�^
*/
Flower::~Flower()
{
	Destroy();
}

/*
	@brief	������
*/
void Flower::Init(INT2 pos, INT2 component)
{
	m_pSprite = new Sprite;
	m_Type = Type::FLOWER;
	m_isDelete = false;
	m_Hp = 18;
	m_Atk = 9;
	m_IndexPos = pos;
	m_Pos = Game::ConvertIndexToPosition(pos);
	m_Component = component;
	m_Direction = { 0,0 };
	m_MoveWeight = 0;
	m_Count = 0;
	m_pSprite->SetSpriteSize((float)GRID_SIZE, (float)GRID_SIZE);
	m_pSprite->SetSizeScaling(0.8f);
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;

	/*! ���ꏊ�̒ʒm */
	LocationManager::GetInstance().SetData(m_IndexPos, this);
}

/*
	@brief	���
*/
void Flower::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	�X�V
*/
void Flower::Update()
{
	m_Count++;/*!< �J�E���^�̉��Z */
				
	/*! �J�E���^���K��l�����Ȃ�s�����Ȃ� */
	if (m_Count < ACTION_COUNT) { return; }
	m_Count = 0;/*!< �J�E���^�̃��Z�b�g */

	/*! �z���Ɏ��s */
	if (!Absorption()) {
		m_Hp -= 2;
	}

	/*! ���� */
	if (BREEDING_COUNT <= m_AbsorptionCount) {
		m_Hp -= 2;
	}

	/*! �ɐB�ɕK�v�ȗ{���Ƌz���񐔂𖞂����Ă��邩���� */
	if (BREEDING_COMPONENT <= m_Component.x&&
		BREEDING_COUNT <= m_AbsorptionCount) {
		Breeding();
	}
}

/*
	@brief	�`��
*/
void Flower::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	�_���[�W
*/
void Flower::Damage(int damage)
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
void Flower::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	�����Ƃ̓����蔻��
*/
void Flower::Collision(Adult & adult)
{
	/*! �Փː�Ɍ�����ς��� */
	INT2 dir = adult.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);
}

/*
	@brief	�{���z��
*/
bool Flower::Absorption()
{
	/*! ���肷����� */
	INT2 around[] = {
		{ -3,-3 },{ -2,-3 },{ -1,-3 },{ 0,-3 },{ 1,-3 },{ 2,-3 },{ 3,-3 },
		{ -3,-2 },{ -2,-2 },{ -1,-2 },{ 0,-2 },{ 1,-2 },{ 2,-2 },{ 3,-2 },
		{ -3,-1 },{ -2,-1 },{ -1,-1 },{ 0,-1 },{ 1,-1 },{ 2,-1 },{ 3,-1 },
		{ -3, 0 },{ -2, 0 },{ -1, 0 }	 ,	   { 1, 0 },{ 2, 0 },{ 3, 0 },
		{ -3, 1 },{ -2, 1 },{ -1, 1 },{ 0, 1 },{ 1, 1 },{ 2, 1 },{ 3, 1 },
		{ -3, 2 },{ -2, 2 },{ -1, 2 },{ 0, 2 },{ 1, 2 },{ 2, 2 },{ 3, 2 },
		{ -3, 3 },{ -2, 3 },{ -1, 3 },{ 0, 3 },{ 1, 3 },{ 2, 3 },{ 3, 3 },
	};
	for (auto i : around) {
		INT2 pos = m_IndexPos + i;

		if (pos.x < 0 || pos.x >= SOIL_WIDTH)	{ continue; }/*!< x���W�͈̔͊O */
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT)	{ continue; }/*!< y���W�͈̔͊O */
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }/*!< �u���b�N���j�� */

		auto component = TerrainManager::GetInstance().GetComponent(pos);
		/*! �{���̋z���\���/�������E */
		if (component.x <= ABSORPTION_UNDER_LIMIT ||
			component.x >= ABSORPTION_OVER_LIMIT) {
			continue;
		}

		/*! �z�� */
		component.x -= ABSORPTION_POWER;
		m_Component.x += ABSORPTION_POWER;
		m_AbsorptionCount++;
		m_Hp += 2;
		if (MAX_HP < m_Hp) { m_Hp = MAX_HP; }
		m_AbsorptionCount++;
		if (component.x < 0)				{ component.x = 0; }				/*!< �{��0���� */
		if (MAX_COMPONENT <= m_Component.x) { m_Component.x = MAX_COMPONENT; }	/*!< �{��99�ȏ� */
		TerrainManager::GetInstance().SetComponent(pos, component);
		return true;
	}
	return false;
}

/*
	@brief	�ɐB
*/
void Flower::Breeding()
{
	/*! HP���� */
	m_Hp -= 2;

	/*! HP0�ȏ�Ȃ�ɐB���Ȃ� */
	if (0 < m_Hp) { return; }


	/*�{���ɉ��������ɐB����*/

	int Num = 0;/*!< �ɐB�� */

	if (m_Component.x < 5)		{ Num = 2; }
	else if (m_Component.x < 11){ Num = 3; }
	else if (m_Component.x < 16){ Num = 4; }
	else						{ Num = 5; }

	for (int i = 0; i < Num; i++) {
		Moss* child = new Moss;
		INT2 halfComponent = {
			m_Component.x / 2,
			m_Component.y / 2,
		};
		child->Init(m_IndexPos, halfComponent);
		EnemyManager::GetInstance().AddEnemy(child);
	}
	m_isDelete = true;
	/*!< �f�[�^�̍폜 */
	LocationManager::GetInstance().DeleteData(m_IndexPos, this);
}

/*
	@brief	�ۗL�{���E�����̎��͂ւ̕��o
*/
void Flower::OutComponent()
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
		if (pos.x < 0 || SOIL_WIDTH <= pos.x)	{ continue; }
		if (pos.y < 0 || SOIL_HEIGHT <= pos.y)	{ continue; }
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
		INT2 pos = m_IndexPos + around[i % (sizeof(around) / sizeof(INT2))];
		/*! �O���Q�� */
		if (pos.x < 0 || SOIL_WIDTH <= pos.x)	{ continue; }
		if (pos.y < 0 || SOIL_HEIGHT <= pos.y)	{ continue; }
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