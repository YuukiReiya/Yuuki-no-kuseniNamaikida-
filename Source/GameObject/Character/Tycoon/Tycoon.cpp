/*
	@file	Tycoon.cpp
	@brief	����
	@author	�ԏ� �G�P
*/
#include "Tycoon.h"

/*
	@def	�萔�錾
*/
const unsigned int Tycoon::ANIMATION_UPDATE_COUNT = 10;

/*
	@brief	�R���X�g���N�^
*/
Tycoon::Tycoon()
{
}

/*
	@brief	�f�X�g���N�^
*/
Tycoon::~Tycoon()
{
	Destroy();
}

/*
	@brief	������
*/
void Tycoon::Init()
{
	m_pSprite = new Sprite;
	m_pDebug = new Debug;

	m_IndexPos = INT2{ 29,4 };
	m_Pos = Game::ConvertIndexToPosition(m_IndexPos);
	m_NextIndexPos = INT2{ -1,-1 };
	m_AnimCount = 0;
	m_AnimIndexU = m_AnimIndexV = 0;
	m_pSprite->SetSplit(3, 5);
	m_pSprite->SetSpriteSize(32, 32);
	m_pSprite->SetSizeScaling(1.8f);
	m_pDebug->Init();
}

/*
	@brief	�X�V
*/
void Tycoon::Update()
{
	AnimUpdate();
	if (Game::TheSamePositionToGrid(m_Pos))
		IndexUpdate();

	/*
	
		�E�����X�^�[�S�Ăɓ����蔻��

	
	*/


	Move();
}

/*
	@brief	�C���f�b�N�X�X�V
*/
void Tycoon::IndexUpdate()
{
	m_IndexPos = Game::ConvertPositionToIndex(m_Pos);
}

/*
	@brief	�A�j���[�V�����X�V
*/
void Tycoon::AnimUpdate()
{
	
	m_AnimCount++;
	/*! �ߔ� */
	if (isCaught) {
		/*! �����ύX */
			INT2 tmp = m_NextIndexPos - m_IndexPos;			/*!< �ړ���Ƌ��ꏊ������������o�A�ꎞ�ϐ��� */
			D3DXVECTOR2 dir = Game::ConvertDirection(tmp);	/*!< �ϊ������ꎞ�ϐ���vector2�ɕϊ� */
			if (dir == Game::UP) { m_AnimIndexV = 3; }
			else if (dir == Game::DOWN) { m_AnimIndexV = 1; }
			else if (dir == Game::LEFT) { m_AnimIndexV = 2; }
			else if (dir == Game::RIGHT) { m_AnimIndexV = 4; }

		if (m_AnimCount < ANIMATION_UPDATE_COUNT) { return; }
		m_AnimIndexU++;
		m_AnimCount = 0;
		if (m_AnimIndexU > 2)m_AnimIndexU = 0;
	}
	/*! ���� */
	else {
		m_AnimIndexV = 0;
		if (m_AnimCount < ANIMATION_UPDATE_COUNT) { return; }
		m_AnimIndexU++;
		m_AnimCount = 0;
		if (m_AnimIndexU > 1)m_AnimIndexU = 0;
	}
	/*
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
	*/
}

/*
	@brief	�����_�����O
*/
void Tycoon::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture,m_AnimIndexU,m_AnimIndexV);
//	m_pDebug->DrawSquare(ConvertView(m_Pos), (float)GRID_SIZE, (float)GRID_SIZE, Debug::GREEN);
}

/*
	@brief	���
*/
void Tycoon::Destroy()
{
	SAFE_DELETE(m_pSprite);
	SAFE_DELETE(m_pDebug);
}

/*
	@brief	�ړ�
*/
void Tycoon::Move()
{
	/*
		�ړ���͉ϒ��z��ɂ���ۑ�����
		�iASTAR�Ŏ����j
	*/
	D3DXVECTOR2 dir = {
	(float)(m_NextIndexPos.x - m_IndexPos.x),
	(float)(m_NextIndexPos.y - m_IndexPos.y),
	};

	m_Pos += dir*m_MovePow;
	//���W����
	{
		bool isOver = false;
		D3DXVECTOR2 temp = Game::ConvertIndexToPosition(m_NextIndexPos);

		if (dir == Game::UP&&temp.y >= m_Pos.y) { isOver = true; }			//��
		else if (dir == Game::DOWN&&temp.y <= m_Pos.y) { isOver = true; }	//��
		else if (dir == Game::LEFT&&temp.x >= m_Pos.x) { isOver = true; }	//��
		else if (dir == Game::RIGHT&&temp.x <= m_Pos.x) { isOver = true; }	//�E
		//���W���ߔ���
		if (isOver)
			m_Pos = temp;
	}
}
