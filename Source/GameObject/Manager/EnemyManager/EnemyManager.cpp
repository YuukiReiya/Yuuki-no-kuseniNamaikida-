/*
	@file	EnemyManager.cpp
	@brief	�o�������X�^�[���Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#include "EnemyManager.h"
#include "../../Character/Enemy/Moss/Moss.h"
#include "../../Character/Enemy/Larvae/Larvae.h"
#include "../../Character/Enemy/Lizard/Lizard.h"

/*
	@brief	�R���X�g���N�^
*/
EnemyManager::EnemyManager()
{
}

/*
	@brief	�f�X�g���N�^
*/
EnemyManager::~EnemyManager()
{
	Destroy();
}

/*
	@brief	������
*/
void EnemyManager::Init()
{
	m_pEnemyList = new EnemyList;
	
	m_MossTexture = new Texture;
	m_BudsTexture = new Texture;
	m_FlowerTexture = new Texture;

	m_LarvaeTexture = new Texture;
	m_ChrysalisTexture = new Texture;
	m_AdultTexture = new Texture;

	m_EggTexture = new Texture;
	m_LizardTexture = new Texture;
	
	m_pEnemyList->Init();
	LoadTexture();
}

/*
	@brief	�X�V
*/
void EnemyManager::Update()
{
	ENEMY_NODE* p;
	p = m_pEnemyList->m_pHead->pNext;
	while (p != NULL&&p != m_pEnemyList->m_pTail) {
		p->data->Update();
		/*! �폜�t���O�������Ă�΃m�[�h���Ȃ��Ȃ��� */
		if (p->data->GetDeleteFlags()) {
			ENEMY_NODE* del;
			del = p;
			p->pPrev->pNext = p->pNext;
			p->pNext->pPrev = p->pPrev;
			p = p->pNext;
			SAFE_DELETE(del);
		}
		/*! �����ĂȂ����p�ɂ͎��|���^������ */
		else {
			p = p->pNext;
		}
	}

	/*! �Փ˔��� */
	CollisionEtoE();
	CollisionEtoH();
}

/*
	@brief	�����X�^�[���m�̓����蔻��
*/
bool EnemyManager::Hit(Enemy & A, Enemy & B)
{
	///*
	auto w = abs(A.GetPos().x - B.GetPos().x);
	auto h = abs(A.GetPos().y - B.GetPos().y);
	if ((w <= 10 && h <= (GRID_SIZE)) || (h == 10 && w <= (GRID_SIZE))) {
		return true;
	}
	return false;
	//*/
	/*
	D3DXVECTOR2 dir[4] = {
		Game::UP,
		Game::DOWN,
		Game::LEFT,
		Game::RIGHT,
	};
	for (auto i : dir) {
		INT2 index = A.GetIndex() + Game::ConvertIndex(i);
		if (index.x < 0 || index.x >= SOIL_WIDTH) { continue; }
		if (index.y < 0 || index.y >= SOIL_HEIGHT) { continue; }
		if (index == B.GetIndex()) { return true; }
	}
	return false;
	*/
}
/*
	@brief	�����X�^�[�ƗE�҂̓����蔻��
*/
bool EnemyManager::Hit(Enemy & enemy)
{

	INT2 way[4] = {
		{ -1,0 },{ 1,0 },
		{ 0,-1 },{ 0,1 }
	};
	for (auto i : way) {
		INT2 pos = enemy.GetIndex() + i;
		auto hero = HeroManager::GetInstance().GetHero(pos);

		/*
			�E�҂����񂾂Ƃ��Ƀ����X�^�[��m_pHero��NULL�ɂȂ�悤�ɒ���
		*/
		enemy.SetHero(hero);
		if (hero) {
			CollisionManager::GetInstance().Add(enemy, *hero);/*!< �����X�^�[�̗E�҂ɑ΂���A�N�V���� */
			CollisionManager::GetInstance().Add(*hero, enemy);/*!< �E�҂̃����X�^�[�ɑ΂���A�N�V���� */
			return true;
		}
	}
	return false;
}

/*
	@brief	�����X�^�[���m�̏Փˏ��o�^
*/
void EnemyManager::CollisionEtoE()
{
	/*! ���X�g����Ȃ珈�����Ȃ� */
	if (m_HitList.empty()) { return; }

	/*! ���X�g�o�^������ */
	for (unsigned int i = 0; i < m_HitList.size(); i++) {
		/*! ���ʔ��� */
		auto dir = Game::ConvertIndex(m_HitList[i]->GetDirection());
		INT2 pos = m_HitList[i]->GetIndex() + dir;
		/*! �O���Q�� */
		if (pos.x < 0 || pos.x >= SOIL_WIDTH)	{ continue; }
		if (pos.y < 0 || pos.y >= SOIL_HEIGHT)	{ continue; }
		/*! �אڃL�����̒��o */
		Enemy* opponent = LocationManager::GetInstance().GetTopFront(pos);
		/*! �אڂ���L���������݂����CollisionList�֓o�^ */
		if (opponent) {
			CollisionManager::GetInstance().Add(*m_HitList[i], *opponent);
			CollisionManager::GetInstance().Add(*opponent, *m_HitList[i]);
			break;
		}
	}
}

/*
	@brief	�E�҂Ƃ̏Փˏ��o�^
*/
void EnemyManager::CollisionEtoH()
{
	auto p = m_pEnemyList->m_pHead->pNext;
	
	while (p != NULL&&p != m_pEnemyList->m_pTail) {
		Hit(*p->data);
		p = p->pNext;
	}
}

/*
	@brief	�����_�����O
*/
void EnemyManager::Render()
{
	ENEMY_NODE* p;
	p = m_pEnemyList->m_pHead->pNext;
	while (p != NULL&&p != m_pEnemyList->m_pTail) {
		IDirect3DTexture9* texture = TextureGetter(p->data->GetType());
		p->data->Render(texture);
		p = p->pNext;
	}
}

/*
	@brief	���
*/
void EnemyManager::Destroy()
{
	SAFE_DELETE(m_pEnemyList);
	SAFE_DELETE(m_MossTexture);
	SAFE_DELETE(m_BudsTexture);
	SAFE_DELETE(m_FlowerTexture);
	SAFE_DELETE(m_LarvaeTexture);
	SAFE_DELETE(m_ChrysalisTexture);
	SAFE_DELETE(m_AdultTexture);
	SAFE_DELETE(m_EggTexture);
	SAFE_DELETE(m_LizardTexture);
}

/*
	@brief	�{���E�����ɉ����������X�^�[�𐶐� �����󖂕��͖������̂��ߗ{�������X�^�[�𐶐�
*/
void EnemyManager::Produce(INT2 pos, INT2 component)
{
	/******************/
	/* �����X�^�[�o�� */
	/******************/

	/*! �{���E�����̔�r */
	auto val = component.x > component.y ? component.x : component.y;
	Enemy* enemy;
	/*! ���x���R */
	if (val > 16) {
		enemy = component.x > component.y ? new Lizard : new Lizard;
	}
	/*! ���x���Q */
	else if (val > 9) {
		enemy = component.x > component.y ? new Larvae : new Larvae;
	}
	/*! ���x���P */
	else {
		enemy = component.x > component.y ? new Moss : new Moss;
	}
	/*! �����X�^�[�̏����� */
	enemy->Init(pos, component);
	EnemyManager::GetInstance().AddEnemy(enemy);
}

/*
	@brief	�����X�^�[�̒ǉ�
*/
void EnemyManager::AddEnemy(Enemy * enemy)
{
	m_pEnemyList->AddNode(enemy);
}

/*
	@brief	�Փ˔��胊�X�g�ւ̒ǉ�
*/
void EnemyManager::HitListAdd(Enemy * enemy)
{
	m_HitList.push_back(enemy);
}

/*
	@brief	�Փ˔��胊�X�g�N���A
*/
void EnemyManager::HitListClear()
{
	m_HitList.clear();
	m_HitList.shrink_to_fit();
}

/*
	@brief	�����X�^�[�̃e�N�X�`���̓ǂݍ���
*/
void EnemyManager::LoadTexture()
{
	if (!m_MossTexture->Load(L"../Resource/Texture/moss.png"))	 MSG("�ǂݍ��ݎ��s");
	if (!m_BudsTexture->Load(L"../Resource/Texture/�Q.png"))	 MSG("�ǂݍ��ݎ��s");
	if (!m_FlowerTexture->Load(L"../Resource/Texture/��.png"))MSG("�ǂݍ��ݎ��s");
	if (!m_LarvaeTexture->Load(L"../Resource/Texture/196.png"))MSG("�ǂݍ��ݎ��s");
	if (!m_ChrysalisTexture->Load(L"../Resource/Texture/�.png"))MSG("�ǂݍ��ݎ��s");
	if (!m_AdultTexture->Load(L"../Resource/Texture/196-2.png"))MSG("�ǂݍ��ݎ��s");
	if (!m_EggTexture->Load(L"../Resource/Texture/egg.png"))	 MSG("�ǂݍ��ݎ��s");
	if (!m_LizardTexture->Load(L"../Resource/Texture/153.png"))MSG("�ǂݍ��ݎ��s");
}

/*
	@brief	�����X�^�[�ɑΉ������e�N�X�`����Ԃ�
*/
IDirect3DTexture9 * EnemyManager::TextureGetter(Enemy::Type type)
{
	if (type == Enemy::Type::MOSS)return m_MossTexture->GetTexture();
	if (type == Enemy::Type::BUDS)return m_BudsTexture->GetTexture();
	if (type == Enemy::Type::FLOWER)return m_FlowerTexture->GetTexture();
	if (type == Enemy::Type::LARVAE)return m_LarvaeTexture->GetTexture();
	if (type == Enemy::Type::CHRYSALIS)return m_ChrysalisTexture->GetTexture();
	if (type == Enemy::Type::ADULT)return m_AdultTexture->GetTexture();
	if (type == Enemy::Type::EGG)return m_EggTexture->GetTexture();
	if (type == Enemy::Type::LIZARD)return m_LizardTexture->GetTexture();
	return nullptr;
}
