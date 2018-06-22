/*
	@file	Wave.cpp
	@brief	Waveファイル
	@author	番場 宥輝
*/
#include "Wave.h"


/*  
	@brief	コンストラクタ
*/
Wave::Wave()
{
	memset(&m_WaveFormat, 0, sizeof(WAVEFORMATEX));
	m_WaveData = NULL;
	m_DataSize = 0;
	m_pSecondaryBuffer = NULL;
}

/*
	@brief	デストラクタ
*/
Wave::~Wave()
{
	if (m_WaveData != NULL) {
		delete[] m_WaveData;
	}
}

/*
	@brief	初期化
	@param(FileName)	読み込むWaveファイルへのパス
*/
bool Wave::Init(TCHAR * FileName)
{
	if (!Load(FileName)) {
		MSG("サウンドの読み込みに失敗");
		return false;
	}
	if (!Create()) {
		MSG("サウンドのセカンダリバッファ作成に失敗");
		return false;
	}
	return true;
}

/*
	@brief	再生
*/
void Wave::Play(bool isLoop)
{
	/*! サウンド再生 */
	if (m_pSecondaryBuffer != NULL) {
		m_pSecondaryBuffer->SetCurrentPosition(0);			/*!< 再生位置を最初から */
		DWORD LoopFlag = (isLoop ? DSBPLAY_LOOPING : 0);	/*!< ループの処理 */
		m_pSecondaryBuffer->Play(0, 0, LoopFlag);
	}
}

/*
	@brief	停止
*/
void Wave::Stop()
{
	if (m_pSecondaryBuffer != NULL)
		m_pSecondaryBuffer->Stop();
}

/*
	@brief	音量の設定
	@param	0～100%までの割合
*/
void Wave::SetVol(unsigned int per)
{
	auto vol = ConvertDB(per);
	m_pSecondaryBuffer->SetVolume(vol);
}

/*
	@brief	セカンダリバッファの生成
*/
bool Wave::Create()
{
	DSBUFFERDESC	desc = {};			/*!< セカンダリバッファ作成用設定 */ 
	/*!< チャンネル数での分岐、モノラルは1チャンネル、ステレオは2チャンネル */ 
	if (m_WaveFormat.nChannels == 1) {
		desc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_STATIC;
		desc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
	}
	else {
		desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLPAN | DSBCAPS_STATIC;
			/*!< | DSBCAPS_CTRLFX;	エフェクトを追加すると Duplicate できない */ 
		desc.guid3DAlgorithm = GUID_NULL;
	}
	desc.dwSize				= sizeof(DSBUFFERDESC);
	desc.dwBufferBytes		= m_DataSize;				/*!< 音データサイズ指定 */
	desc.lpwfxFormat		= &m_WaveFormat;			/*!< フォーマット指定 */ 

	IDirectSoundBuffer*	pPrimaryBuffer = NULL;			/*!< プライマリバッファ */ 
	/*! プライマリバッファ作成 */
	HRESULT h;
	h=
	DirectDevice::GetInstance().GetSoundDevice()->CreateSoundBuffer(&desc, &pPrimaryBuffer, NULL);
	h;
	/*! プライマリバッファからセカンダリバッファ作成 */
	pPrimaryBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&m_pSecondaryBuffer);
	/*! セカンダリバッファが作成できれば、プライマリバッファは破棄してかまわない */
	pPrimaryBuffer->Release();

	unsigned char* block1 = NULL;
	unsigned char* block2 = NULL;
	unsigned long blockSize1 = 0;
	unsigned long blockSize2 = 0;
	/*! セカンダリバッファをロックしてデータ書き込み */
	m_pSecondaryBuffer->Lock(0, m_DataSize,
		(void**)&block1, &blockSize1, (void**)&block2, &blockSize2, DSBLOCK_ENTIREBUFFER);
	/*! セカンダリバッファに音データコピー */
	memcpy(block1, m_WaveData, m_DataSize);
	/*! セカンダリバッファロック解除 */
	m_pSecondaryBuffer->Unlock(block1, blockSize1, block2, 0);

	return true;
}

/*
	@brief	Waveファイルの読み込み
	@param(FileName)	Waveファイルのパス
*/
bool Wave::Load(TCHAR * FileName)
{
	/*! バイナリ読み込みモードで開く */
	FILE* fp;
	if (!(fp = ::_tfopen(FileName, _T("rb"))))return false;

	char chunkId[5] = {};
	char tmp[5] = {};
	unsigned int chunkSize = 0;

	/*! RIFFチャンク読み込み */
	fread(chunkId, sizeof(char) * 4, 1, fp);
	fread(&chunkSize, sizeof(unsigned int), 1, fp);
	fread(tmp, sizeof(char) * 4, 1, fp);
	if (strcmp(chunkId, "RIFF") || strcmp(tmp, "WAVE")) {
		return false;	// Waveファイルじゃない
	}

	/*! 子チャンク読み込み */
	bool fmtchunk = false;
	bool datachunk = false;
	while (true)
	{
		fread(chunkId, sizeof(char) * 4, 1, fp);
		fread(&chunkSize, sizeof(unsigned int), 1, fp);
		if (!strcmp(chunkId, "fmt ")) {
			if (chunkSize >= sizeof(WAVEFORMATEX)) {
				fread(&m_WaveFormat, sizeof(WAVEFORMATEX), 1, fp);
				int diff = chunkSize - sizeof(WAVEFORMATEX);
				fseek(fp, diff, SEEK_CUR);
			}
			else {
				memset(&m_WaveFormat, 0, sizeof(WAVEFORMATEX));
				fread(&m_WaveFormat, chunkSize, 1, fp);
			}
			fmtchunk = true;
		}
		else if (!strcmp(chunkId, "data")) {
			/*! データサイズ確保 */
			m_DataSize = chunkSize;
			m_WaveData = new byte[chunkSize];
			/*! データ読み込み */
			if (fread(m_WaveData, sizeof(byte), chunkSize, fp) != chunkSize) {
				fclose(fp);
				return false;	/*!< ファイルが壊れている */
			}
			datachunk = true;
		}
		else {
			fseek(fp, chunkSize, SEEK_CUR);
		}

		if (fmtchunk && datachunk)break;
	}

	fclose(fp);
	return true;
}

/*
	@brief	比率をデシベルに変換する
	@detail	線型補完で実現する
*/
LONG Wave::ConvertDB(unsigned int per)
{
	/*! 範囲外 */
	if (per <= 0) {
		return 	DSBVOLUME_MIN;
	}
	else if (100 <= per) {
		return 	DSBVOLUME_MAX;
	}

	/*! 線形補完を利用 */
	if (per >= 50) {
		return -600 + 600 * (per - 50) / 50;
	}
	else if (per >= 25) {
		return -1200 + 600 * (per - 25) / 25;
	}
	else if (per >= 12) {
		return -1800 + 600 * (per - 12) / 12;
	}
	else if (per >= 6) {
		return -2400 + 600 * (per - 6) / 6;
	}
	else if (per >= 3) {
		return -3000 + 600 * (per - 3) / 3;
	}
	else if (per >= 1) {
		return -3600 + 600 * (per - 1);
	}
	return DSBVOLUME_MIN;
}
