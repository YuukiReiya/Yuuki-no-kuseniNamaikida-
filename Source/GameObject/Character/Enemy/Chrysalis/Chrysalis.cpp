/*
	@file	Chrysalis.h
	@brief	�
	@author	�ԏ� �G�P
*/
#include "Chrysalis.h"
#include "../../../Manager/LocationManager/LocationManager.h"

/*
	@def	�萔�錾
*/
const unsigned int Chrysalis::ACTION_COUNT = 60;
const unsigned int Chrysalis::GROW_COUNT = FPS * 8;

/*
	@brief	�R���X�g���N�^
*/
Chrysalis::Chrysalis()
{
}

/*
	@brief	�f�X�g���N�^
*/
Chrysalis::~Chrysalis()
{
	Destroy();
}

/*
	@brief	������
*/
void Chrysalis::Init(INT2 pos, INT2 component)
{
	m_pSprite = new Sprite;

	m_isDelete = false;
	m_Type = Type::CHRYSALIS;
	m_Atk = 0;
	m_IndexPos = pos;
	m_Pos = Game::ConvertIndexToPosition(pos);
	m_Component = component;
	m_Direction = { 0,0 };
	m_MoveWeight = 0;
	m_pSprite->SetSpriteSize((float)(GRID_SIZE), (float)(GRID_SIZE));
	m_pSprite->SetSizeScaling(1.1f);
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;
	m_Count = 0;

	/*! ���ꏊ�̒ʒm */
	LocationManager::GetInstance().SetData(m_IndexPos, this);
}

/*
	@brief	������
	@detail	��������HP���w��ł���
*/
void Chrysalis::Init(INT2 pos, INT2 component, int hp)
{
	Init(pos, component);
	m_Hp = hp;
}

/*
	@brief	���
*/
void Chrysalis::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	�X�V
*/
void Chrysalis::Update()
{
	m_Count++;

	/*! �����ɕK�v�Ȏ��Ԃ��o������ */
	if (GROW_COUNT < m_Count) {
		Grow();
	}
}

/*
	@brief	�`��
*/
void Chrysalis::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	�_���[�W
*/
void Chrysalis::Damage(int damage)
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
void Chrysalis::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	�g�J�Q�Ƃ̏Փ�
*/
void Chrysalis::Collision(Lizard & lizard)
{
	/*! �Փː�Ɍ�����ς��� */
	INT2 dir = lizard.GetIndex() - m_IndexPos;
	m_Direction = Game::ConvertDirection(dir);
}

/*
	@brief	����
*/
void Chrysalis::Grow()
{
	Adult* child = new Adult;
	child->Init(m_IndexPos, m_Component);
	EnemyManager::GetInstance().AddEnemy(child);
	/*!< �f�[�^�̍폜 */
	LocationManager::GetInstance().DeleteData(m_IndexPos, this);
	m_isDelete = true;
}

/*
	@brief	�ۗL�{���E�������͂ւ̕��o
*/
void Chrysalis::OutComponent()
{
	INT2 around[] = {
		{ -1,-1 },{ 0,-1 },{ 1,-1 },
		{ -1, 0 },{ 1, 0 },
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
