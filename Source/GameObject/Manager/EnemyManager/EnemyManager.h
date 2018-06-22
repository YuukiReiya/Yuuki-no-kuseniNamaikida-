/*
	@file	EnemyManager.h
	@brief	�o�������X�^�[���Ǘ�
	@detail	�V���O���g���̃}�l�[�W���[�N���X
	@author	�ԏ� �G�P
*/
#pragma once
#include "../../List/EnemyList/EnemyList.h"
#include "../CollisionManager/CollisionManager.h"
#include "../LocationManager/LocationManager.h"
#include "../../List/Location/Location.h"
#include "../HeroManager/HeroManager.h"

/*
	@brief	�����X�^�[�̊Ǘ��N���X
*/
class EnemyManager
{
public:
	/*! �f�X�g���N�^ */
	~EnemyManager();

	/*! �V���O���g�� */
	static EnemyManager& GetInstance() {
		static EnemyManager instance;
		return instance;
	}

	void Init();
	void Update();
	void Render();
	void Destroy();
	void Produce(INT2 pos, INT2 component);
	void AddEnemy(Enemy* enemy);
	void HitListAdd(Enemy* enemy);
	void HitListClear();
private:
	bool Hit(Enemy& A, Enemy& B);
	bool Hit(Enemy& enemy);
	void CollisionEtoE();
	void CollisionEtoH();
	void LoadTexture();
	IDirect3DTexture9* TextureGetter(Enemy::Type type);


	/*! �R���X�g���N�^ */
	EnemyManager();

	/*! �ϐ� */
	Location* m_pLocation;				/*! �ʒu�f�[�^ */
	std::vector<Enemy*> m_HitList;		/*!< �����X�^�[�Փ˔��胊�X�g */
	EnemyList* m_pEnemyList;			/*!< �����X�^�[�̃��X�g */
	Texture* m_MossTexture;				/*!< �R�P�̃e�N�X�`�� */
	Texture* m_BudsTexture;				/*!< �Q�̃e�N�X�`�� */
	Texture* m_FlowerTexture;			/*!< �Ԃ̃e�N�X�`�� */
	Texture* m_LarvaeTexture;			/*!< �c���̃e�N�X�`�� */
	Texture* m_ChrysalisTexture;		/*!< 匂̃e�N�X�`�� */
	Texture* m_AdultTexture;			/*!< �����̃e�N�X�`�� */
	Texture* m_EggTexture;				/*!< ���̃e�N�X�`�� */
	Texture* m_LizardTexture;			/*!< �g�J�Q�̃e�N�X�`�� */
};

