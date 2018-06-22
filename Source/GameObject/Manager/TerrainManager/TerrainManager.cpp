/*
	@file	TerrainManager.cpp
	@brief	�n�`���Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#include "TerrainManager.h"
#include "../../../Camera/Camera2D.h"

/*
	@brief	�R���X�g���N�^
*/
TerrainManager::TerrainManager()
{
}

/*
	@brief	�f�X�g���N�^
*/
TerrainManager::~TerrainManager()
{
	Destroy();
}

/*
	@brief	������
*/
void TerrainManager::Init()
{
	/*! �C���X�^���X���� */
	m_pTerrain				=	 new Terrain;
	m_pImmortalBlock		=	 new ImmortalBlock;
	m_pGround				=	 new Ground;
	m_pZeroSoilTexture		=	 new Texture;
	m_pImmortalBlockTexture =	 new Texture;
	m_pBreakedSoilTexture	=	 new Texture;
	m_pLevel1NatTexture		=	 new Texture;
	m_pLevel1MagTexture		=	 new Texture;
	m_pLevel2NatTexture		=	 new Texture;
	m_pLevel2MagTexture		=	 new Texture;
	m_pLevel3NatTexture		=	 new Texture;
	m_pLevel3MagTexture		=	 new Texture;

	if (!TextureLoad()) {
		MSG("�n�`�̃e�N�X�`���̃��[�h�Ɏ��s");
		return;
	}
	m_pTerrain->Init();
	m_pImmortalBlock->Init();
	m_pGround->Init();

	if (!d.Init()) { MSG("�f�o�b�O�N���X�̏������Ɏ��s"); }
}

/*
	@brief	�X�V
*/
void TerrainManager::Update()
{
	for(int x=0;x<SOIL_WIDTH;x++)
		for (int y = 0; y < SOIL_HEIGHT; y++) {
			/*! ���g��k���̊֌W����W�͍��� */
			D3DXVECTOR2 Pos{
				(float)GRID_SIZE 					+ (float)GRID_SIZE*x,
				(float)GRID_SIZE*GROUND_HEIGHT		+ (float)GRID_SIZE*y
			};

			m_pTerrain->m_pSoils[x][y]->m_Pos = (ConvertView(Pos));
		}
}

/*
	@brief	�����_�����O
*/
void TerrainManager::Render()
{
	/*! �n�` */
	for (int x = 0; x<SOIL_WIDTH; x++)
		for (int y = 0; y < SOIL_HEIGHT; y++) {
			if (!m_pTerrain->m_pSoils[x][y]->m_isBreaked) {
				IDirect3DTexture9* texture = TextureGetter(m_pTerrain->m_pSoils[x][y]->m_Component);
				m_pTerrain->m_pSoils[x][y]->Render(texture);
			}
			else {
				m_pTerrain->m_pSoils[x][y]->Render(m_pBreakedSoilTexture->GetTexture());
			}

		}
	/*! �󂹂Ȃ��u���b�N */
	m_pImmortalBlock->Render(m_pImmortalBlockTexture->GetTexture());

	/*! �n�� */
	m_pGround->Render();
}

/*
	@brief	�u���b�N�̔j��
*/
bool TerrainManager::Break(INT2 Pos)
{
	/*! �G���[�`�F�b�N */
	if (Pos.x < 0 || Pos.y < 0)return false;
	if (SOIL_WIDTH <= Pos.x || SOIL_HEIGHT <= Pos.y)return false;

	/*! �t���O�������ĂȂ������� */
	if (!m_pTerrain->m_pSoils[Pos.x][Pos.y]->m_isBreaked) {
		/*! �t���O�𗧂Ă� */
		m_pTerrain->m_pSoils[Pos.x][Pos.y]->m_isBreaked = true;
		return true;
	}
	return false;
}

/*
	@brief	���
*/
void TerrainManager::Destroy()
{
	SAFE_DELETE(m_pTerrain);
	SAFE_DELETE(m_pZeroSoilTexture);
	SAFE_DELETE(m_pGround);
	SAFE_DELETE(m_pImmortalBlockTexture);
	SAFE_DELETE(m_pBreakedSoilTexture);
	SAFE_DELETE(m_pImmortalBlock);
	SAFE_DELETE(m_pLevel1NatTexture);
	SAFE_DELETE(m_pLevel1MagTexture);
	SAFE_DELETE(m_pLevel2NatTexture);
	SAFE_DELETE(m_pLevel2MagTexture);
	SAFE_DELETE(m_pLevel3NatTexture);
	SAFE_DELETE(m_pLevel3MagTexture);
}

/*
	@brief	�{���E�����ɉ������u���b�N�̃e�N�X�`����Ԃ�
*/
IDirect3DTexture9 * TerrainManager::TextureGetter(INT2 Component)
{
	/* �{���Ɩ����̔�r */
	int temp = Component.x > Component.y ? Component.x : Component.y;
	if (temp >= COMPONENT_LEVEL3)return Component.x > Component.y ? m_pLevel3NatTexture->GetTexture() : m_pLevel3MagTexture->GetTexture();
	else if (temp >= COMPONENT_LEVEL2)return Component.x > Component.y ? m_pLevel2NatTexture->GetTexture() : m_pLevel2MagTexture->GetTexture();
	else if (temp >= COMPONENT_LEVEL1)return Component.x > Component.y ? m_pLevel1NatTexture->GetTexture() : m_pLevel1MagTexture->GetTexture();
	else if (temp == 0)return m_pZeroSoilTexture->GetTexture();
	MSG("�n�`�Ǘ��N���X�̃e�N�X�`���Q�b�^�[�ŗ\�����Ȃ��l������ nullptr��Ԃ��܂�");
	return nullptr;
}

/*
	@brief	�y�u���b�N�Ɏg���e�N�X�`���̓ǂݍ���
*/
bool TerrainManager::TextureLoad()
{
	MFALSE(m_pZeroSoilTexture->Load(L"../Resource/Texture/�y�u���b�N.png"),"�e�N�X�`���̓ǂݍ��ݎ��s")
	MFALSE(m_pLevel1NatTexture->Load(L"../Resource/Texture/�{��1-1-3.png"), "�e�N�X�`���̓ǂݍ��ݎ��s")
	MFALSE(m_pLevel1MagTexture->Load(L"../Resource/Texture/�y�u���b�N.png"), "�e�N�X�`���̓ǂݍ��ݎ��s")
	MFALSE(m_pLevel2NatTexture->Load(L"../Resource/Texture/�{��2-1-9.png"), "�e�N�X�`���̓ǂݍ��ݎ��s")
	MFALSE(m_pLevel2MagTexture->Load(L"../Resource/Texture/�y�u���b�N.png"), "�e�N�X�`���̓ǂݍ��ݎ��s")
	MFALSE(m_pLevel3NatTexture->Load(L"../Resource/Texture/�{��3-2.png"), "�e�N�X�`���̓ǂݍ��ݎ��s")
	MFALSE(m_pLevel3MagTexture->Load(L"../Resource/Texture/�y�u���b�N.png"), "�e�N�X�`���̓ǂݍ��ݎ��s")
	//MFALSE(m_pZeroSoilTexture->Load(L"../Resource/Texture/�{��1-1-2.png"), "�e�N�X�`���̓ǂݍ��ݎ��s")
	MFALSE(m_pImmortalBlockTexture->Load(L"../Resource/Texture/ImmortalObj.png"), "�e�N�X�`���̓ǂݍ��ݎ��s")
	MFALSE(m_pBreakedSoilTexture->Load(L"../Resource/Texture/�y.png"), "�e�N�X�`���̓ǂݍ��ݎ��s")
	
	return true;
}
