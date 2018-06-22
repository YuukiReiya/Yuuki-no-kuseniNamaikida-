/*
	@file	Wave.h
	@brief	Waveファイル
	@author	番場 宥輝
*/
#pragma once
#include "../../Common/Common.h"
#include "../../Common/DirectDevice.h"

/*
	@brief	Waveクラス
*/
class Wave
{
public:
	/*! コンストラクタ */
	Wave();
	/*! デストラクタ */
	~Wave();

	bool Init(TCHAR* FileName);		/*!< 初期化 */
	void Play(bool isLoop);			/*!< 再生 */
	void Stop();					/*!< 停止 */
	void SetVol(unsigned int per);	/*!< 音量 */
private:
	/*! 変数 */
	WAVEFORMATEX			m_WaveFormat;			/*!< Waveフォーマット */
	byte*					m_WaveData;				/*!< 音の生データ */
	int						m_DataSize;				/*!< データサイズ */ 
	IDirectSoundBuffer8*	m_pSecondaryBuffer;		/*!< セカンダリバッファ */

	/*! 関数 */
	bool Create();							/*!< セカンダリバッファ生成 */
	bool Load(TCHAR* FileName);				/*!< ファイル読み込み */
	LONG ConvertDB(unsigned int per);		/*!< 比率のデシベル変換 */
};
