/*
	@file	Sprite.h
	@brief	スプライト
	@author	番場 宥輝
*/
#include "Sprite.h"


/*
	@brief	コンストラクタ
*/
Sprite::Sprite()
{
	m_Width = m_Height = 0;
	m_Pos = { 0,0 };
	m_IndexU = m_IndexV = 0;
	m_SplitU = m_SplitV = 1;
}

/*
	@brief	デストラクタ
*/
Sprite::~Sprite()
{
}

/*
	@brief	レンダリング
*/
void Sprite::Render(IDirect3DTexture9* pTexture, bool isReverse) {
	/*! 頂点情報 */
	Vertex vertex[] = {
		{ m_Pos.x + m_Width / 2,m_Pos.y - m_Height / 2,0.0f,1.0f,(isReverse ? (float)m_IndexU / m_SplitU : (float)(m_IndexU + 1) / m_SplitU),(float)m_IndexV / m_SplitV },			//右上
		{ m_Pos.x + m_Width / 2,m_Pos.y + m_Height / 2,0.0f,1.0f,(isReverse ? (float)m_IndexU / m_SplitU : (float)(m_IndexU + 1) / m_SplitU),(float)(m_IndexV + 1) / m_SplitV },	//右下
		{ m_Pos.x - m_Width / 2,m_Pos.y - m_Height / 2,0.0f,1.0f,(isReverse ? (float)(m_IndexU + 1) / m_SplitU : (float)m_IndexU / m_SplitU),(float)m_IndexV / m_SplitV },			//左上
		{ m_Pos.x - m_Width / 2,m_Pos.y + m_Height / 2,0.0f,1.0f,(isReverse ? (float)(m_IndexU + 1) / m_SplitU : (float)m_IndexU / m_SplitU),(float)(m_IndexV + 1) / m_SplitV },	//左下
	};

	/*! テクスチャセット */
	DirectDevice::GetInstance().GetDevice()->SetTexture(0, pTexture);
	/*! 頂点情報セット */
	DirectDevice::GetInstance().GetDevice()->SetFVF(SPRITE_FVF);
	/*! スプライト描画 */
	DirectDevice::GetInstance().GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(Vertex));
}

/*
	@brief	左上を描画の中心としレンダリング
*/
void Sprite::RenderLeftTop(IDirect3DTexture9* pTexture, bool isReverse) {
	/*! 頂点情報 */
	Vertex vertex[] = {
		{ m_Pos.x + m_Width,m_Pos.y,0.0f,1.0f,(isReverse ? (float)m_IndexU / m_SplitU : (float)(m_IndexU + 1) / m_SplitU),(float)m_IndexV / m_SplitV },							//右上
		{ m_Pos.x + m_Width,m_Pos.y + m_Height,0.0f,1.0f,(isReverse ? (float)m_IndexU / m_SplitU : (float)(m_IndexU + 1) / m_SplitU),(float)(m_IndexV + 1) / m_SplitV },	//右下
		{ m_Pos.x,m_Pos.y,0.0f,1.0f,(isReverse ? (float)(m_IndexU + 1) / m_SplitU : (float)m_IndexU / m_SplitU),(float)m_IndexV / m_SplitV },												//左上
		{ m_Pos.x,m_Pos.y + m_Height,0.0f,1.0f,(isReverse ? (float)(m_IndexU + 1) / m_SplitU : (float)m_IndexU / m_SplitU),(float)(m_IndexV + 1) / m_SplitV },					//左下
	};

	/*! テクスチャセット */
	DirectDevice::GetInstance().GetDevice()->SetTexture(0, pTexture);
	/*! 頂点情報セット */
	DirectDevice::GetInstance().GetDevice()->SetFVF(SPRITE_FVF);
	/*! スプライト描画 */
	DirectDevice::GetInstance().GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(Vertex));
}

/*
	@brief	アトラステクスチャのレンダリング
*/
bool Sprite::RenderAtlas(IDirect3DTexture9* pTexture, unsigned int U, unsigned int V, bool isReverse) {

	/*! インデックスが0未満なら処理を抜ける */
	if (U<0 || V<0)return false;
	/*! インデックスが分割数より多かったら処理を抜ける */
	if (m_SplitU<U || m_SplitV<V)return false;

	/*! 頂点情報 */
	Vertex vertex[] = {
		{ m_Pos.x + m_Width / 2,m_Pos.y - m_Height / 2,0.0f,1.0f,(isReverse ? (float)U / m_SplitU : (float)(U + 1) / m_SplitU),(float)V / m_SplitV },			//右上
		{ m_Pos.x + m_Width / 2,m_Pos.y + m_Height / 2,0.0f,1.0f,(isReverse ? (float)U / m_SplitU : (float)(U + 1) / m_SplitU),(float)(V + 1) / m_SplitV }, //右下
		{ m_Pos.x - m_Width / 2,m_Pos.y - m_Height / 2,0.0f,1.0f,(isReverse ? (float)(U + 1) / m_SplitU : (float)U / m_SplitU),(float)V / m_SplitV },			//左上
		{ m_Pos.x - m_Width / 2,m_Pos.y + m_Height / 2,0.0f,1.0f,(isReverse ? (float)(U + 1) / m_SplitU : (float)U / m_SplitU),(float)(V + 1) / m_SplitV }, //左下
	};

	/*! テクスチャセット */
	DirectDevice::GetInstance().GetDevice()->SetTexture(0, pTexture);
	/*! 頂点情報セット */
	DirectDevice::GetInstance().GetDevice()->SetFVF(SPRITE_FVF);
	/*! スプライト描画 */
	DirectDevice::GetInstance().GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(Vertex));
	return true;
}

/*
	@brief	左上を中心としたアトラステクスチャのレンダリング
*/
bool Sprite::RenderAtlasLeftTop(IDirect3DTexture9* pTexture, unsigned int U, unsigned int V, bool isReverse) {

	/*! インデックスが0未満なら処理を抜ける */
	if (U<0 || V<0)return false;
	/*! インデックスが分割数より多かったら処理を抜ける */
	if (m_SplitU<U || m_SplitV<V)return false;

	/*! 頂点情報 */
	Vertex vertex[] = {
		{ m_Pos.x + m_Width,m_Pos.y,0.0f,1.0f,(isReverse ? (float)U / m_SplitU : (float)(U + 1) / m_SplitU),(float)V / m_SplitV },									//右上
		{ m_Pos.x + m_Width,m_Pos.y + m_Height,0.0f,1.0f,(isReverse ? (float)U / m_SplitU : (float)(U + 1) / m_SplitU),(float)(V + 1) / m_SplitV },		//右下
		{ m_Pos.x,m_Pos.y,0.0f,1.0f,(isReverse ? (float)(U + 1) / m_SplitU : (float)U / m_SplitU),(float)V / m_SplitV },													//左上
		{ m_Pos.x,m_Pos.y + m_Height,0.0f,1.0f,(isReverse ? (float)(U + 1) / m_SplitU : (float)U / m_SplitU),(float)(V + 1) / m_SplitV },						//左下
	};

	/*! テクスチャセット */
	DirectDevice::GetInstance().GetDevice()->SetTexture(0, pTexture);
	/*! 頂点情報セット */
	DirectDevice::GetInstance().GetDevice()->SetFVF(SPRITE_FVF);
	/*! スプライト描画 */
	DirectDevice::GetInstance().GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(Vertex));
	return true;
}

/*
	@brief	画像を分割する
*/
bool Sprite::SetSplit(unsigned int U, unsigned int V) {
	/*! 分割数が0以下なら処理を抜ける */
	if (U <= 0 || V <= 0)return false;
	/*! 分割する */
	m_SplitU = U;
	m_SplitV = V;
	return true;
}

/*
	@brief	アトラス画像のインデックスをセット
*/
bool Sprite::SetIndex(unsigned int U, unsigned int V) {
	/*! インデックスが0未満なら処理を抜ける */
	if (U<0 || V<0)return false;
	/*! インデックスが分割数より多かったら処理を抜ける */
	if (m_SplitU<U || m_SplitV<V)return false;
	/*! インデックスの更新 */
	m_IndexU = U;
	m_IndexV = V;
	return true;
}