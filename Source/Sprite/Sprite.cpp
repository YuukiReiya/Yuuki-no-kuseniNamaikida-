/*
	@file	Sprite.h
	@brief	�X�v���C�g
	@author	�ԏ� �G�P
*/
#include "Sprite.h"


/*
	@brief	�R���X�g���N�^
*/
Sprite::Sprite()
{
	m_Width = m_Height = 0;
	m_Pos = { 0,0 };
	m_IndexU = m_IndexV = 0;
	m_SplitU = m_SplitV = 1;
}

/*
	@brief	�f�X�g���N�^
*/
Sprite::~Sprite()
{
}

/*
	@brief	�����_�����O
*/
void Sprite::Render(IDirect3DTexture9* pTexture, bool isReverse) {
	/*! ���_��� */
	Vertex vertex[] = {
		{ m_Pos.x + m_Width / 2,m_Pos.y - m_Height / 2,0.0f,1.0f,(isReverse ? (float)m_IndexU / m_SplitU : (float)(m_IndexU + 1) / m_SplitU),(float)m_IndexV / m_SplitV },			//�E��
		{ m_Pos.x + m_Width / 2,m_Pos.y + m_Height / 2,0.0f,1.0f,(isReverse ? (float)m_IndexU / m_SplitU : (float)(m_IndexU + 1) / m_SplitU),(float)(m_IndexV + 1) / m_SplitV },	//�E��
		{ m_Pos.x - m_Width / 2,m_Pos.y - m_Height / 2,0.0f,1.0f,(isReverse ? (float)(m_IndexU + 1) / m_SplitU : (float)m_IndexU / m_SplitU),(float)m_IndexV / m_SplitV },			//����
		{ m_Pos.x - m_Width / 2,m_Pos.y + m_Height / 2,0.0f,1.0f,(isReverse ? (float)(m_IndexU + 1) / m_SplitU : (float)m_IndexU / m_SplitU),(float)(m_IndexV + 1) / m_SplitV },	//����
	};

	/*! �e�N�X�`���Z�b�g */
	DirectDevice::GetInstance().GetDevice()->SetTexture(0, pTexture);
	/*! ���_���Z�b�g */
	DirectDevice::GetInstance().GetDevice()->SetFVF(SPRITE_FVF);
	/*! �X�v���C�g�`�� */
	DirectDevice::GetInstance().GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(Vertex));
}

/*
	@brief	�����`��̒��S�Ƃ������_�����O
*/
void Sprite::RenderLeftTop(IDirect3DTexture9* pTexture, bool isReverse) {
	/*! ���_��� */
	Vertex vertex[] = {
		{ m_Pos.x + m_Width,m_Pos.y,0.0f,1.0f,(isReverse ? (float)m_IndexU / m_SplitU : (float)(m_IndexU + 1) / m_SplitU),(float)m_IndexV / m_SplitV },							//�E��
		{ m_Pos.x + m_Width,m_Pos.y + m_Height,0.0f,1.0f,(isReverse ? (float)m_IndexU / m_SplitU : (float)(m_IndexU + 1) / m_SplitU),(float)(m_IndexV + 1) / m_SplitV },	//�E��
		{ m_Pos.x,m_Pos.y,0.0f,1.0f,(isReverse ? (float)(m_IndexU + 1) / m_SplitU : (float)m_IndexU / m_SplitU),(float)m_IndexV / m_SplitV },												//����
		{ m_Pos.x,m_Pos.y + m_Height,0.0f,1.0f,(isReverse ? (float)(m_IndexU + 1) / m_SplitU : (float)m_IndexU / m_SplitU),(float)(m_IndexV + 1) / m_SplitV },					//����
	};

	/*! �e�N�X�`���Z�b�g */
	DirectDevice::GetInstance().GetDevice()->SetTexture(0, pTexture);
	/*! ���_���Z�b�g */
	DirectDevice::GetInstance().GetDevice()->SetFVF(SPRITE_FVF);
	/*! �X�v���C�g�`�� */
	DirectDevice::GetInstance().GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(Vertex));
}

/*
	@brief	�A�g���X�e�N�X�`���̃����_�����O
*/
bool Sprite::RenderAtlas(IDirect3DTexture9* pTexture, unsigned int U, unsigned int V, bool isReverse) {

	/*! �C���f�b�N�X��0�����Ȃ珈���𔲂��� */
	if (U<0 || V<0)return false;
	/*! �C���f�b�N�X����������葽�������珈���𔲂��� */
	if (m_SplitU<U || m_SplitV<V)return false;

	/*! ���_��� */
	Vertex vertex[] = {
		{ m_Pos.x + m_Width / 2,m_Pos.y - m_Height / 2,0.0f,1.0f,(isReverse ? (float)U / m_SplitU : (float)(U + 1) / m_SplitU),(float)V / m_SplitV },			//�E��
		{ m_Pos.x + m_Width / 2,m_Pos.y + m_Height / 2,0.0f,1.0f,(isReverse ? (float)U / m_SplitU : (float)(U + 1) / m_SplitU),(float)(V + 1) / m_SplitV }, //�E��
		{ m_Pos.x - m_Width / 2,m_Pos.y - m_Height / 2,0.0f,1.0f,(isReverse ? (float)(U + 1) / m_SplitU : (float)U / m_SplitU),(float)V / m_SplitV },			//����
		{ m_Pos.x - m_Width / 2,m_Pos.y + m_Height / 2,0.0f,1.0f,(isReverse ? (float)(U + 1) / m_SplitU : (float)U / m_SplitU),(float)(V + 1) / m_SplitV }, //����
	};

	/*! �e�N�X�`���Z�b�g */
	DirectDevice::GetInstance().GetDevice()->SetTexture(0, pTexture);
	/*! ���_���Z�b�g */
	DirectDevice::GetInstance().GetDevice()->SetFVF(SPRITE_FVF);
	/*! �X�v���C�g�`�� */
	DirectDevice::GetInstance().GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(Vertex));
	return true;
}

/*
	@brief	����𒆐S�Ƃ����A�g���X�e�N�X�`���̃����_�����O
*/
bool Sprite::RenderAtlasLeftTop(IDirect3DTexture9* pTexture, unsigned int U, unsigned int V, bool isReverse) {

	/*! �C���f�b�N�X��0�����Ȃ珈���𔲂��� */
	if (U<0 || V<0)return false;
	/*! �C���f�b�N�X����������葽�������珈���𔲂��� */
	if (m_SplitU<U || m_SplitV<V)return false;

	/*! ���_��� */
	Vertex vertex[] = {
		{ m_Pos.x + m_Width,m_Pos.y,0.0f,1.0f,(isReverse ? (float)U / m_SplitU : (float)(U + 1) / m_SplitU),(float)V / m_SplitV },									//�E��
		{ m_Pos.x + m_Width,m_Pos.y + m_Height,0.0f,1.0f,(isReverse ? (float)U / m_SplitU : (float)(U + 1) / m_SplitU),(float)(V + 1) / m_SplitV },		//�E��
		{ m_Pos.x,m_Pos.y,0.0f,1.0f,(isReverse ? (float)(U + 1) / m_SplitU : (float)U / m_SplitU),(float)V / m_SplitV },													//����
		{ m_Pos.x,m_Pos.y + m_Height,0.0f,1.0f,(isReverse ? (float)(U + 1) / m_SplitU : (float)U / m_SplitU),(float)(V + 1) / m_SplitV },						//����
	};

	/*! �e�N�X�`���Z�b�g */
	DirectDevice::GetInstance().GetDevice()->SetTexture(0, pTexture);
	/*! ���_���Z�b�g */
	DirectDevice::GetInstance().GetDevice()->SetFVF(SPRITE_FVF);
	/*! �X�v���C�g�`�� */
	DirectDevice::GetInstance().GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(Vertex));
	return true;
}

/*
	@brief	�摜�𕪊�����
*/
bool Sprite::SetSplit(unsigned int U, unsigned int V) {
	/*! ��������0�ȉ��Ȃ珈���𔲂��� */
	if (U <= 0 || V <= 0)return false;
	/*! �������� */
	m_SplitU = U;
	m_SplitV = V;
	return true;
}

/*
	@brief	�A�g���X�摜�̃C���f�b�N�X���Z�b�g
*/
bool Sprite::SetIndex(unsigned int U, unsigned int V) {
	/*! �C���f�b�N�X��0�����Ȃ珈���𔲂��� */
	if (U<0 || V<0)return false;
	/*! �C���f�b�N�X����������葽�������珈���𔲂��� */
	if (m_SplitU<U || m_SplitV<V)return false;
	/*! �C���f�b�N�X�̍X�V */
	m_IndexU = U;
	m_IndexV = V;
	return true;
}