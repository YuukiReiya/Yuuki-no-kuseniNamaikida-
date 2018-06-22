/*
	@file	Wave.h
	@brief	Wave�t�@�C��
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../Common/Common.h"
#include "../../Common/DirectDevice.h"

/*
	@brief	Wave�N���X
*/
class Wave
{
public:
	/*! �R���X�g���N�^ */
	Wave();
	/*! �f�X�g���N�^ */
	~Wave();

	bool Init(TCHAR* FileName);		/*!< ������ */
	void Play(bool isLoop);			/*!< �Đ� */
	void Stop();					/*!< ��~ */
	void SetVol(unsigned int per);	/*!< ���� */
private:
	/*! �ϐ� */
	WAVEFORMATEX			m_WaveFormat;			/*!< Wave�t�H�[�}�b�g */
	byte*					m_WaveData;				/*!< ���̐��f�[�^ */
	int						m_DataSize;				/*!< �f�[�^�T�C�Y */ 
	IDirectSoundBuffer8*	m_pSecondaryBuffer;		/*!< �Z�J���_���o�b�t�@ */

	/*! �֐� */
	bool Create();							/*!< �Z�J���_���o�b�t�@���� */
	bool Load(TCHAR* FileName);				/*!< �t�@�C���ǂݍ��� */
	LONG ConvertDB(unsigned int per);		/*!< �䗦�̃f�V�x���ϊ� */
};
