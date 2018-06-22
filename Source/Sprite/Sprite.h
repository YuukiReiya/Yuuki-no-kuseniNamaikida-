/*
	@file	Sprite.h
	@brief	スプライト
	@author	番場 宥輝
*/
#pragma once
#include "../Common/Common.h"

/*
	@brief	スプライトクラス
*/
class Sprite {

public:
	/*! コンストラクタ */
	Sprite();
	/*! デストラクタ */
	~Sprite();

	/*! セッター */
	void SetPos(D3DXVECTOR2 pos) { m_Pos = pos; }											/*!< 位置 */
	void SetSpriteSize(float Width, float Height) { m_Width = Width, m_Height = Height; }	/*!< スプライトのサイズ */
	void SetSizeScaling(float ScaleMag) { m_Width *= ScaleMag, m_Height *= ScaleMag; }		/*!< サイズのスケーリング(比率を維持したまま拡縮) */
	bool SetSplit(unsigned int U, unsigned int V);											/*!< テクスチャの分割 */
	bool SetIndex(unsigned int U, unsigned int V);											/*!< 分割したインデックス */

	/*! 描画 */
	void Render(IDirect3DTexture9* pTexture, bool isReverse = false);												/*!< 画像の中心から */
	void RenderLeftTop(IDirect3DTexture9* pTexture, bool isReverse = false);										/*!< 画像の左上から */
	bool RenderAtlas(IDirect3DTexture9* pTexture, unsigned int U, unsigned int V, bool isReverse = false);			/*!< アトラス画像の描画（中心） */
	bool RenderAtlasLeftTop(IDirect3DTexture9* pTexture, unsigned int U, unsigned int V, bool isReverse = false);	/*!< アトラス画像の描画（左上） */
private:
	/*
		@struct	Vertex
		@brief	板ポリゴン頂点情報
	*/
	struct Vertex {
		float x, y, z;	/*!< 3次元座標 */
		float rhw;		/*!< 2D変換フラグ */
		float u, v;		/*!< UV座標 */
	};

	/*! FVF(柔軟な頂点構造体宣言)フラグ */
	static const DWORD SPRITE_FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;

	/*! スプライト位置 */
	D3DXVECTOR2 m_Pos;

	/*! スプライトサイズ */
	float m_Width;	/*!< 横幅 */
	float m_Height;	/*!< 縦幅 */

	/*! UV分割数 */
	unsigned int m_SplitU;	/*!< 横:x */
	unsigned int m_SplitV;	/*!< 縦:y */

	/*! 分割したUVのインデックス */
	unsigned int m_IndexU;	/*!< 横:x */
	unsigned int m_IndexV;	/*!< 縦:y */
};