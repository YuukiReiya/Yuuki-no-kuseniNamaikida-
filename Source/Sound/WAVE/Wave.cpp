/*
	@file	Wave.cpp
	@brief	Wave�t�@�C��
	@author	�ԏ� �G�P
*/
#include "Wave.h"


/*  
	@brief	�R���X�g���N�^
*/
Wave::Wave()
{
	memset(&m_WaveFormat, 0, sizeof(WAVEFORMATEX));
	m_WaveData = NULL;
	m_DataSize = 0;
	m_pSecondaryBuffer = NULL;
}

/*
	@brief	�f�X�g���N�^
*/
Wave::~Wave()
{
	if (m_WaveData != NULL) {
		delete[] m_WaveData;
	}
}

/*
	@brief	������
	@param(FileName)	�ǂݍ���Wave�t�@�C���ւ̃p�X
*/
bool Wave::Init(TCHAR * FileName)
{
	if (!Load(FileName)) {
		MSG("�T�E���h�̓ǂݍ��݂Ɏ��s");
		return false;
	}
	if (!Create()) {
		MSG("�T�E���h�̃Z�J���_���o�b�t�@�쐬�Ɏ��s");
		return false;
	}
	return true;
}

/*
	@brief	�Đ�
*/
void Wave::Play(bool isLoop)
{
	/*! �T�E���h�Đ� */
	if (m_pSecondaryBuffer != NULL) {
		m_pSecondaryBuffer->SetCurrentPosition(0);			/*!< �Đ��ʒu���ŏ����� */
		DWORD LoopFlag = (isLoop ? DSBPLAY_LOOPING : 0);	/*!< ���[�v�̏��� */
		m_pSecondaryBuffer->Play(0, 0, LoopFlag);
	}
}

/*
	@brief	��~
*/
void Wave::Stop()
{
	if (m_pSecondaryBuffer != NULL)
		m_pSecondaryBuffer->Stop();
}

/*
	@brief	���ʂ̐ݒ�
	@param	0�`100%�܂ł̊���
*/
void Wave::SetVol(unsigned int per)
{
	auto vol = ConvertDB(per);
	m_pSecondaryBuffer->SetVolume(vol);
}

/*
	@brief	�Z�J���_���o�b�t�@�̐���
*/
bool Wave::Create()
{
	DSBUFFERDESC	desc = {};			/*!< �Z�J���_���o�b�t�@�쐬�p�ݒ� */ 
	/*!< �`�����l�����ł̕���A���m������1�`�����l���A�X�e���I��2�`�����l�� */ 
	if (m_WaveFormat.nChannels == 1) {
		desc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_STATIC;
		desc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
	}
	else {
		desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLPAN | DSBCAPS_STATIC;
			/*!< | DSBCAPS_CTRLFX;	�G�t�F�N�g��ǉ������ Duplicate �ł��Ȃ� */ 
		desc.guid3DAlgorithm = GUID_NULL;
	}
	desc.dwSize				= sizeof(DSBUFFERDESC);
	desc.dwBufferBytes		= m_DataSize;				/*!< ���f�[�^�T�C�Y�w�� */
	desc.lpwfxFormat		= &m_WaveFormat;			/*!< �t�H�[�}�b�g�w�� */ 

	IDirectSoundBuffer*	pPrimaryBuffer = NULL;			/*!< �v���C�}���o�b�t�@ */ 
	/*! �v���C�}���o�b�t�@�쐬 */
	HRESULT h;
	h=
	DirectDevice::GetInstance().GetSoundDevice()->CreateSoundBuffer(&desc, &pPrimaryBuffer, NULL);
	h;
	/*! �v���C�}���o�b�t�@����Z�J���_���o�b�t�@�쐬 */
	pPrimaryBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&m_pSecondaryBuffer);
	/*! �Z�J���_���o�b�t�@���쐬�ł���΁A�v���C�}���o�b�t�@�͔j�����Ă��܂�Ȃ� */
	pPrimaryBuffer->Release();

	unsigned char* block1 = NULL;
	unsigned char* block2 = NULL;
	unsigned long blockSize1 = 0;
	unsigned long blockSize2 = 0;
	/*! �Z�J���_���o�b�t�@�����b�N���ăf�[�^�������� */
	m_pSecondaryBuffer->Lock(0, m_DataSize,
		(void**)&block1, &blockSize1, (void**)&block2, &blockSize2, DSBLOCK_ENTIREBUFFER);
	/*! �Z�J���_���o�b�t�@�ɉ��f�[�^�R�s�[ */
	memcpy(block1, m_WaveData, m_DataSize);
	/*! �Z�J���_���o�b�t�@���b�N���� */
	m_pSecondaryBuffer->Unlock(block1, blockSize1, block2, 0);

	return true;
}

/*
	@brief	Wave�t�@�C���̓ǂݍ���
	@param(FileName)	Wave�t�@�C���̃p�X
*/
bool Wave::Load(TCHAR * FileName)
{
	/*! �o�C�i���ǂݍ��݃��[�h�ŊJ�� */
	FILE* fp;
	if (!(fp = ::_tfopen(FileName, _T("rb"))))return false;

	char chunkId[5] = {};
	char tmp[5] = {};
	unsigned int chunkSize = 0;

	/*! RIFF�`�����N�ǂݍ��� */
	fread(chunkId, sizeof(char) * 4, 1, fp);
	fread(&chunkSize, sizeof(unsigned int), 1, fp);
	fread(tmp, sizeof(char) * 4, 1, fp);
	if (strcmp(chunkId, "RIFF") || strcmp(tmp, "WAVE")) {
		return false;	// Wave�t�@�C������Ȃ�
	}

	/*! �q�`�����N�ǂݍ��� */
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
			/*! �f�[�^�T�C�Y�m�� */
			m_DataSize = chunkSize;
			m_WaveData = new byte[chunkSize];
			/*! �f�[�^�ǂݍ��� */
			if (fread(m_WaveData, sizeof(byte), chunkSize, fp) != chunkSize) {
				fclose(fp);
				return false;	/*!< �t�@�C�������Ă��� */
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
	@brief	�䗦���f�V�x���ɕϊ�����
	@detail	���^�⊮�Ŏ�������
*/
LONG Wave::ConvertDB(unsigned int per)
{
	/*! �͈͊O */
	if (per <= 0) {
		return 	DSBVOLUME_MIN;
	}
	else if (100 <= per) {
		return 	DSBVOLUME_MAX;
	}

	/*! ���`�⊮�𗘗p */
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
