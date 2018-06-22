/*
	@file	Enemy.h
	@brief	モンスター
	@author	番場 宥輝
*/
#pragma once
#include "../Character.h"
#include "../../../Common/Common.h"
#include "../../../Sprite/Sprite.h"
#include "../../../Camera/Camera2D.h"
#include "../../Manager/TerrainManager/TerrainManager.h"
#include "../Hero/Hero.h"


/*! 前方宣言 */
struct ENEMY_NODE;

/*
	@brief	モンスターの基底クラス
*/
class Enemy :
	public Character
{
public:
	/*! コンストラクタ */
			 Enemy() = default;
	/*! 仮想デストラクタ */
	virtual ~Enemy() = default;

	/*
		@enum	Type
		@brief	モンスターの種類
	*/
	enum Type {
		/*! コケ類 */
		MOSS,		/*!< コケ */
		BUDS,		/*!< 蕾 */
		FLOWER,		/*!< 花 */

		/*! 虫類 */
		LARVAE,		/*!< 幼虫 */
		CHRYSALIS,	/*!< 蛹 */
		ADULT,		/*!< 成虫 */

		/*! トカゲ類 */
		EGG,		/*!< 卵 */
		LIZARD		/*!< トカゲ */
	};

	/*! 純粋仮想関数 */
	virtual void Init(INT2 pos, INT2 component)		= 0;
	virtual void Destroy()							= 0;
	virtual void Update()							= 0;
	virtual void Render(IDirect3DTexture9* texture)	= 0;
	virtual void Damage(int damage)					= 0;
	virtual void Collision(Character& character)	= 0;

	/*! 仮想関数 */
	virtual void Init(INT2 pos, INT2 component, int hp) {};

	/*! ゲッター */
	Type		GetType()			{ return m_Type; }
	bool		GetDeleteFlags()	{ return m_isDelete; }
	int			GetHp()				{ return m_Hp; }
	D3DXVECTOR2	GetPos()			{ return m_Pos; }
	D3DXVECTOR2 GetDirection()		{ return m_Direction; }
	INT2		GetIndex()			{ return m_IndexPos; }
	INT2		GetComponent()		{ return m_Component; }
	/*! セッター */
	void SetEnemyNode(ENEMY_NODE* node) { m_pNode = node; }
	void SetHero(Hero* hero)			{ m_pHero = hero; }
protected:
	Sprite*		m_pSprite;		/*!< スプライト */
	ENEMY_NODE* m_pNode;		/*!< ノード */
	Type		m_Type;			/*!< 種類 */
	Hero*		m_pHero;		/*!< 攻撃対象の勇者 */
	int			m_Hp;			/*!< 体力 */
	int			m_Atk;			/*!< 攻撃力 */
	INT2		m_Component;	/*!< 養分・魔分 */
	D3DXVECTOR2 m_Pos;			/*!< 座標 */
	INT2		m_IndexPos;		/*!< 座標のインデックス */
	INT2		m_NextIndexPos;	/*!< 移動先インデックス座標 */
	D3DXVECTOR2 m_Direction;	/*!< 向いている方向 */
	float		m_MoveWeight;	/*!< 移動量 */
	bool		m_isCollision;	/*!< 衝突フラグ */
	bool		m_isDelete;		/*!< 削除フラグ */
	bool		m_isDead;		/*!< 死亡フラグ */
	DWORD		m_AtkCount;		/*!< 攻撃カウンタ */
	int			m_AnimCount;	/*!< アニメーションカウンタ */
	int			m_AnimIndexU;	/*!< UV画像Uインデックス */
	int			m_AnimIndexV;	/*!< UV画像Vインデックス */
};

