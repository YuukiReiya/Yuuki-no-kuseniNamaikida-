/*
	@file	SceneBase.h
	@brief	�V�[���Ǘ��̊��N���X
	@date	2017/12/04
	@author	�ԏ� �G�P
*/
#pragma once

/*
	@brief	�V�[���̊��N���X
*/
class SceneBase
{
public:
	/*! ���z�f�X�g���N�^ */
	virtual ~SceneBase() {};

	/*! �������z�֐� */
	virtual void Init() = 0;						/*!< ������ */
	virtual SceneBase* Update(SceneBase* scene) = 0;/*!< �X�V */
	virtual void Render() = 0;						/*!< �����_�����O */
	virtual void Destroy() = 0;						/*!< ��� */
};
