/*
	@file	HeroManager.cpp
	@brief	�E�҂��Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#include "HeroManager.h"

/*! ���O��� */
using namespace Game;

/*
	@brief	�R���X�g���N�^
*/
HeroManager::HeroManager()
{
}

/*
	@brief	�f�X�g���N�^
*/
HeroManager::~HeroManager()
{
	Destroy();
}

/*
	@brief	������
*/
void HeroManager::Init()
{
	m_pHeroTexture = new Texture;
	m_pHeroList = new HeroList;

	m_pHeroList->Init();
	TextureLoad();
	
	isClear = false;
	m_Debug.Init();
}

/*
	@brief	�X�V
*/
void HeroManager::Update()
{
	
	HERO_NODE* p;
	p = m_pHeroList->m_pHead->pNext;
	
	/*! �E�҂̎��� */
	INT2 contact[4] = {
		{ 0,-1 },
		{ 0,1 },
		{ 1,0 },
		{ -1,0 }
	};

	/*! ��l���X�V */
	while (p != NULL&&p != m_pHeroList->m_pTail) {
		
		/*! �C���f�b�N�X�̍X�V */
		if (TheSamePositionToGrid(p->data->GetPos())) {
			p->data->IndexUpdate();
			if (p->data->GetCaught())
				TycoonManager::GetInstance().SetHeroPos(p->data->GetIndex());
		}

		/*! HP��0�Ȃ烊�X�g����O�� */
		if (p->data->GetHp() <= 0) {
			/*
				���S��
			*/
			/*! �ߔ���ԉ��� */
			isClear = true;
			if (p->data->GetCaught()) {
				p->data->SetCaught(false);
				TycoonManager::GetInstance().SetCaughtFlags(false);
			}

			auto del = p;
			p = p->pNext;
			m_pHeroList->DeleteNode(del);
			continue;
		}

		/*! �����ߔ����� */
		if (!TycoonManager::GetInstance().GetCaughtFlags()) {
			/*! �E�҂̎��͂̃}�X�𔻒� */
			for (auto i : contact) {
				INT2 tycoonPos = TycoonManager::GetInstance().GetTycoonPos() + i;
				/*! �������אڃ}�X�ɂ��邩���� */
				if (tycoonPos == p->data->GetIndex()) {
					p->data->SetCaught(true);
					TycoonManager::GetInstance().SetCaughtFlags(true);
					TycoonManager::GetInstance().SetHeroData(p->data->GetMovePow());
					TycoonManager::GetInstance().SetHeroPos(p->data->GetIndex());
					p->data->Catch();
					break;
				}
			}
		}

		/*! �X�V */
		p->data->Update();
		p = p->pNext;
	}


}

/*
	@brief	�����_�����O
*/
void HeroManager::Render()
{
	HERO_NODE* p;
	p = m_pHeroList->m_pHead->pNext;
	while (p != NULL&&p != m_pHeroList->m_pTail) {
		p->data->Render(m_pHeroTexture->GetTexture());
		p = p->pNext;
	}
	/*
	char str[255];
	sprintf(str, "Tycoon:ix=%d,iy=%d\npx=%f,py=%f",
		TycoonManager::GetInstance().GetTycoonPos().x,
		TycoonManager::GetInstance().GetTycoonPos().y,
		TycoonManager::GetInstance().GetTycoonPosV().x,
		TycoonManager::GetInstance().GetTycoonPosV().y);
	*/
	/*
	*/
	/*
	sprintf(str, "Tycoon:ix=%d,iy=%d\nNx=%d,Ny=%d,���W=%f/%f",
		TycoonManager::GetInstance().GetTycoonPos().x,
		TycoonManager::GetInstance().GetTycoonPos().y,
		TycoonManager::GetInstance().GetTycoonNextPos().x,
		TycoonManager::GetInstance().GetTycoonNextPos().y,
		TycoonManager::GetInstance().GetTycoonPosV().x,
		TycoonManager::GetInstance().GetTycoonPosV().y
	);
	//sprintf(str,"")
	SetWindowTextA(m_Debug.m_hWnd, str);
	*/
}

/*
	@brief	���
*/
void HeroManager::Destroy()
{
	SAFE_DELETE(m_pHeroTexture);
	SAFE_DELETE(m_pHeroList);
}

/*
	@brief	�E�҂̒ǉ�
*/
void HeroManager::AddHero(Hero * hero)
{
	m_pHeroList->AddNode(hero);
	hero->Init();
}

/*
	@brief	�E�҂̃Q�b�^�[
	@param(pos)	���W
	@detail	�w����W�̗E�҂̏���Ԃ��B�E�҂����Ȃ����NULL��Ԃ�
*/
Hero * HeroManager::GetHero(INT2 pos)
{
	auto p = m_pHeroList->m_pHead->pNext;
	while (p != NULL&&p != m_pHeroList->m_pTail) {
		//Hp��1�ȏォ�w��C���f�b�N�X���W�̗E��
		if (p->data->GetHp() > 0 && p->data->GetIndex() == pos) {
			return p->data;
		}
		p = p->pNext;
	}
	return NULL;
}

/*
	@brief	�E�҂̎擾
*/
Hero * HeroManager::GetHeroInfo()
{
	auto p = m_pHeroList->m_pHead->pNext;
	if (p != m_pHeroList->m_pTail&&p != NULL) { return p->data; }
	return NULL;
}

/*
	@brief	�E�҂̃e�N�X�`���̓ǂݍ���
*/
void HeroManager::TextureLoad()
{
	if (!m_pHeroTexture->Load(L"../Resource/Texture/71.png"))MSG("�ǂݍ��ݎ��s");

}
