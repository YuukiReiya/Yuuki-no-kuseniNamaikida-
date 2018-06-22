/*
	@file	Egg.h
	@brief	��
	@author	�ԏ� �G�P
*/
#include "Egg.h"
#include "../../../Manager/LocationManager/LocationManager.h"

/*
	@def	�萔�錾
*/
const unsigned int Egg::HATCH_COUNT = FPS * 6;

/*
	@brief	�R���X�g���N�^
*/
Egg::Egg()
{
}

/*
	@brief	�f�X�g���N�^
*/
Egg::~Egg()
{
	Destroy();
}

/*
	@brief	������
*/
void Egg::Init(INT2 pos, INT2 component)
{
	m_pSprite = new Sprite;
	m_isDelete = false;
	m_Type = Type::EGG;
	m_Atk = 0;
	m_IndexPos = pos;
	m_Pos = Game::ConvertIndexToPosition(pos);
	m_Component = component;
	m_Direction = { 0,0 };
	m_MoveWeight = 0;
	m_pSprite->SetSpriteSize((float)(GRID_SIZE), (float)(GRID_SIZE));
	m_pSprite->SetSizeScaling(1.0f);
	m_pSprite->SetSplit(4, 2);
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;
	m_Count = 0;

	/*! ���ꏊ�̒ʒm */
	LocationManager::GetInstance().SetData(m_IndexPos, this);
}

/*
	@brief	���
*/
void Egg::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

/*
	@brief	�X�V
*/
void Egg::Update()
{
	m_Count++;

	/*! �z���ɕK�v�Ȏ��Ԃ��o������ */
	if (HATCH_COUNT < m_Count) {
		Hatch();
	}
}

/*
	@brief	�`��
*/
void Egg::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);
}

/*
	@brief	�_���[�W
*/
void Egg::Damage(int damage)
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
void Egg::Collision(Character & character)
{
	character.Collision(*this);
}

/*
	@brief	�z��
*/
void Egg::Hatch()
{
	Lizard* hatch = new Lizard;
	hatch->Init(m_IndexPos, m_Component);
	EnemyManager::GetInstance().AddEnemy(hatch);
	m_isDelete = true;
	/*!< �f�[�^�̍폜 */
	LocationManager::GetInstance().DeleteData(m_IndexPos, this);
}

