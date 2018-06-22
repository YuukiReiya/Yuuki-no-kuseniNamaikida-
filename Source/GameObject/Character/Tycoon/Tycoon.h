/*
	@file	Tycoon.h
	@brief	魔王
	@author	番場 宥輝
*/
#pragma once
#include "../Character.h"
#include "../../../Sprite/Sprite.h"
#include ".././../../Camera/Camera2D.h"
#include "../../../Debug/Debug.h"
#include "../../../Algorithm/AStar/AStar.h"

/*
	@brief	魔王クラス
*/
class Tycoon :
	public Character
{
public:
	/*! コンストラクタ */
	Tycoon();
	/*! デストラクタ */
	~Tycoon();

	
	void Init();								/*!< 初期化 */
	void Update();								/*!< 更新 */
	void Render(IDirect3DTexture9* texture);	/*!< 描画 */
	void Destroy();								/*!< 解放 */
	void Move();								/*!< 移動 */
	bool isCaught;								/*!< 捕縛フラグ */

	/*! 衝突 */
	void Collision(Character& character) {};	
	void Collision(Soldier& solder) {};			

	/*! 変数 */
	D3DXVECTOR2 m_Pos;
	INT2		m_IndexPos;
	INT2		m_NextIndexPos;
	float		m_MovePow;
private:
	void IndexUpdate();							/*!< インデックス更新 */
	void AnimUpdate();							/*!< アニメーション更新 */

	/*! 定数 */
	static const unsigned int ANIMATION_UPDATE_COUNT;

	DWORD			m_AnimCount;
	unsigned int	m_AnimIndexU;
	unsigned int	m_AnimIndexV;
	Sprite*			m_pSprite;
	Debug*			m_pDebug;
};

