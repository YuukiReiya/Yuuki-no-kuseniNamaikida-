/*
	@file	Buds.cpp
	@brief	�Q
	@author	�ԏ� �G�P
*/
#include "Buds.h"
#include "../../../Manager/LocationManager/LocationManager.h"

/*
	@def	�萔�錾
*/
const unsigned int Buds::ACTION_COUNT = 60;
const unsigned int Buds::ABSORPTION_OVER_LIMIT = 11;
const unsigned int Buds::ABSORPTION_UNDER_LIMIT = 0;
const unsigned int Buds::GROW_COMPONENT = 8;

/*
	@brief	�R���X�g���N�^
*/
Buds::Buds()
{
}

/*
	@brief	�f�X�g���N�^
*/
Buds::~Buds()
{
	Destroy();
}

/*
	@brief	������
*/
void Buds::Init(INT2 pos, INT2 component)
{
	m_pSprite = new Sprite;

	m_isDelete = false;
	m_Type = Type::BUDS;
	m_Atk = 0;
	m_IndexPos = pos;
	m_Pos = Game::ConvertIndexToPosition(pos);
	m_Component = component;
	m_Direction = D3DXVECTOR2(0, 0);
	m_MoveWeight = 0;
	
	m_pSprite->SetSpriteSize((float)GRID_SIZE, (float)GRID_SIZE);
	m_pSprite->SetSizeScaling(0.8f);
	m_Count = 0;
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;
	m_Debug.Init();

	/*! ���ꏊ�̒ʒm */
	LocationManager::GetInstance().SetData(m_IndexPos, this);
}

/*
	@brief	������
	@detail	��������HP���w��ł���
*/
void Buds::Init(INT2 pos, INT2 component, int hp)
{
	Init(pos, component);
	m_Hp = hp;
}

/*
	@brief	���
*/
void Buds::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	�X�V
*/
void Buds::Update()
{
	m_Count++;/*!< �J�E���^�̉��Z */

	/*! �J�E���^���K��l�����Ȃ�s�����Ȃ� */
	if (m_Count < ACTION_COUNT) { return; }
	m_Count = 0;/*!< �J�E���^�̃��Z�b�g */

	/*! �{���̋z�� */
	if (!Absorption()) {
		Damage(1);/*!< �_���[�W */
	}
	
	/*! �����ɕK�v�ȗ{����ێ����Ă����� */
	if (m_Component.x >= GROW_COMPONENT) {
		Grow();
	}
}

/*
	@brief	�`��
*/
void Buds::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	�_���[�W
*/
void Buds::Damage(int damage)
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
void Buds::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	�����Ƃ̏Փ�
*/
void Buds::Collision(Adult & adult)
{
	/*! �Փː�Ɍ�����ς��� */
	INT2 dir = adult.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);
}

/*
	@brief	�{���z��
*/
bool Buds::Absorption()
{
	/*! ���肷����� */
	INT2 around[] = {
		{ -2,-2 },{ -1,-2 },{ 0,-2 },{ 1,-2 },{ 2,-2 },
		{ -2,-1 },{ -1,-1 },{ 0,-1 },{ 1,-1 },{ 2,-1 },
		{ -2, 0 },{ -1, 0 }		,	 { 1, 0 },{ 2, 0 },
		{ -2, 1 },{ -1, 1 },{ 0, 1 },{ 1, 1 },{ 2, 1 },
		{ -2, 2 },{ -1, 2 },{ 0, 2 },{ 1, 2 },{ 2, 2 },
	};
	for (auto i : around) {
		INT2 pos = m_IndexPos + i;

		if (pos.x < 0 || pos.x >= SOIL_WIDTH)	 { continue; }/*!< x���W�͈̔͊O */
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT)	 { continue; }/*!< y���W�͈̔͊O */
		if (TerrainManager::GetInstance().GetBreakFlag(pos)) { continue; }/*!< �u���b�N���j�� */

		auto component = TerrainManager::GetInstance().GetComponent(pos);
		/*! �{���̋z���\���/�������E */
		if (component.x <= ABSORPTION_UNDER_LIMIT ||
			component.x >= ABSORPTION_OVER_LIMIT) {
			continue;
		}

		/*! �z�� */
		component.x -= 1;
		m_Component.x += 1;
		m_Hp += 1;
		if (component.x < 0) { component.x = 0; }
		TerrainManager::GetInstance().SetComponent(pos, component);

		return true;
	}
	return false;
}

/*
	@brief	����
*/
void Buds::Grow()
{
	/*! �Ԃւ̏��� */
	Flower* grow = new Flower;
	grow->Init(m_IndexPos, m_Component);
	EnemyManager::GetInstance().AddEnemy(grow);
	/*!< �f�[�^�̍폜 */
	LocationManager::GetInstance().DeleteData(m_IndexPos, this);
	m_isDelete = true;
}

/*
	@brief	�ۗL�{���E�������͂ւ̕��o
*/
void Buds::OutComponent()
{
	INT2 around[] = {
		{ -1,-1 },{  0,-1 },{ 1,-1 },
		{ -1, 0 },			{ 1, 0 },
		{ -1, 1 },{  0, 1 },{ 1, 1 },
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
