/*
	@file	TerrainManager.h
	@brief	�n�`���Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../../Common/Common.h"
#include "../../../Texture/Texture.h"
#include "../../List/Terrain/Terrain.h"
#include "../../ImmortalObject/ImmortalBlock/ImmortalBlock.h"
#include "../../Ground/Ground.h"
#include "../../../Debug/Debug.h"


/*
	@brief	�n�`�̊Ǘ��N���X
*/
class TerrainManager
{
public:
	/*! �R���X�g���N�^ */
	TerrainManager();
	/*! �R���X�g���N�^ */
	~TerrainManager();

	/*! �V���O���g�� */
	static TerrainManager& GetInstance() {
		static TerrainManager instance;
		return instance;
	}
	
	void Init();				/*!< ������ */
	void Update();				/*!< �X�V */
	void Render();				/*!< �`�� */
	bool Break(INT2 Pos);		/*!< �j�� */
	void Destroy();				/*!< ��� */

	/*! �Q�b�^�[ */
	bool GetBreakFlag(INT2 Index) { return m_pTerrain->m_pSoils[Index.x][Index.y]->m_isBreaked; }
	INT2 GetComponent(INT2 index) { return m_pTerrain->m_pSoils[index.x][index.y]->m_Component; }

	/*! �Z�b�^�[ */
	void SetComponent(INT2 index, INT2 component) { m_pTerrain->m_pSoils[index.x][index.y]->m_Component = component; }
private:
	IDirect3DTexture9*	TextureGetter(INT2 Component);
	bool TextureLoad();

	Terrain*			m_pTerrain;
	ImmortalBlock*		m_pImmortalBlock;
	Ground*				m_pGround;

	/*! �e�N�X�`�� */
	Texture*			m_pZeroSoilTexture;			/*!< �{���E�������[�� */
	Texture*			m_pLevel1NatTexture;		/*!< �{�����x���P */
	Texture*			m_pLevel1MagTexture;		/*!< �������x���P */
	Texture*			m_pLevel2NatTexture;		/*!< �{�����x���Q */
	Texture*			m_pLevel2MagTexture;		/*!< �������x���Q */
	Texture*			m_pLevel3NatTexture;		/*!< �{�����x���R */
	Texture*			m_pLevel3MagTexture;		/*!< �������x���R */
	Texture*			m_pImmortalBlockTexture;	/*!< �j��s�\�u���b�N */
	Texture*			m_pBreakedSoilTexture;		/*!< �����̓y */

	Debug d;
};

