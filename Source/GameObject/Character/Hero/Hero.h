/*
	@file	Hero.h
	@brief	勇者
	@author	番場 宥輝
*/
#pragma once
#include "../Character.h"
#include "../../../Camera/Camera2D.h"
#include "../../../Sprite/Sprite.h"

/*
	@brief	勇者の基底クラス
*/
class Hero :
	public Character
{
public:
	/*! コンストラクタ */
			 Hero() = default;
	/*! 仮想デストラクタ */
	virtual ~Hero() = default;

	/*! 純粋仮想関数 */
	virtual void Init()		= 0;
	virtual void Destroy()	= 0;
	virtual void Update()	= 0;
	virtual void Render(IDirect3DTexture9* texture) = 0;
	virtual void IndexUpdate() = 0;
	virtual void Catch() = 0;
	virtual void Damage(int damage) = 0;
	virtual void Collision(Character& character) = 0;

	/*! ゲッター */
	int GetHp() { return m_Hp; }
	INT2 GetIndex() { return m_IndexPos; }
	bool GetCaught() { return m_isCaught; }
	D3DXVECTOR2 GetPos() { return m_Pos; }
	float GetMovePow() { return m_Move; }

	/*! セッター */
	void SetCaught(bool flags) { m_isCaught = flags; }
	void SetParam(int hp, int atk, float movePow) {
		m_Hp = hp, m_Atk = atk, m_Move = movePow;
	}
protected:
	std::string m_Name;			/*!< 名前 */
	int			m_Hp;			/*!< 体力 */
	int			m_Atk;			/*!< 攻撃力 */
	float		m_Move;			/*!< 移動量 */
	D3DXVECTOR2 m_Direction;	/*!< 向き */
	D3DXVECTOR2 m_Pos;			/*!< 座標 */
	INT2		m_IndexPos;		/*!< 座標のインデックス */
	INT2		m_NextIndexPos;	/*!< 移動先座標のインデックス */
	bool		m_isCaught;		/*!< 魔王捕縛フラグ */
	Sprite*		m_pSprite;		/*!< スプライト */
	int			m_AnimCount;	/*!< アニメーションカウンタ */
	int			m_AnimIndexU;	/*!< UV画像Uインデックス */
	int			m_AnimIndexV;	/*!< UV画像Vインデックス */
	
};