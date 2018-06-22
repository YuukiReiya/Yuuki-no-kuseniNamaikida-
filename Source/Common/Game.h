#include "Common.h"
#pragma once

/* 
	@brief	�Q�[���Ŏg���֐�
*/
namespace Game {
	/*
		@brief	���W���C���f�b�N�X�Ɗ��S��v���Ă��邩�`�F�b�N
		@return	����
	*/
	static bool TheSamePositionToGrid(D3DXVECTOR2 Pos) {

		INT2 index{
			(int)Pos.x / GRID_SIZE - 1,
			(int)Pos.y / GRID_SIZE - GROUND_HEIGHT
		};
		D3DXVECTOR2 temp{
			(float)GRID_SIZE + (float)GRID_SIZE / 2 + index.x*GRID_SIZE,
			(float)GRID_SIZE*GROUND_HEIGHT + (float)GRID_SIZE / 2 + index.y*GRID_SIZE
		};
		if (Pos.x != temp.x || Pos.y != temp.y) {
			return false;
		}
		return true;
	}

	/*
		@brief	���W����C���f�b�N�X�����߂�
		@detail	���W�����S��v���Ă���K�v������
		@return	���W���狁�߂��C���f�b�N�X
	*/
	static INT2 ConvertPositionToIndex(D3DXVECTOR2 Pos) {
		Pos.x -= (float)GRID_SIZE / 2;
		Pos.y -= (float)GRID_SIZE / 2;
		Pos /= (float)GRID_SIZE;
		INT2 temp;
		temp.x = (int)Pos.x;
		temp.y = (int)Pos.y;
		//���W�𒲐�
		temp.x = temp.x - 1;
		temp.y = temp.y - GROUND_HEIGHT;
		return temp;
	}

	/*
		@brief	�C���f�b�N�X������W�����߂�
		@return	�Z�o�������W
	*/
	static D3DXVECTOR2 ConvertIndexToPosition(INT2 IndexPos) {
		D3DXVECTOR2 Pos{
			(float)GRID_SIZE + (float)GRID_SIZE / 2 + IndexPos.x*(float)GRID_SIZE,
			(float)GRID_SIZE*GROUND_HEIGHT + (float)GRID_SIZE / 2 + IndexPos.y*(float)GRID_SIZE,
		};
		return Pos;
	}


	/*
		@def	�萔�錾
	*/
	const D3DXVECTOR2 UP{ 0,-1 };	//������̒P�ʃx�N�g��
	const D3DXVECTOR2 DOWN{ 0,1 };	//�������̒P�ʃx�N�g��
	const D3DXVECTOR2 LEFT{ -1,0 };	//�������̒P�ʃx�N�g��
	const D3DXVECTOR2 RIGHT{ 1,0 };	//�E�����̒P�ʃx�N�g��
	const D3DXVECTOR2 FORWARD{ 0,-1 };//�O���̒P�ʃx�N�g��
	const D3DXVECTOR2 BACK{ 0,1 };//����̒P�ʃx�N�g��
					
	/*
		@brief	�P�ʃx�N�g����INT2�^�ɕϊ�
	*/
	static INT2 ConvertIndex(const D3DXVECTOR2 direction) {
		if (direction == UP)		return INT2(0, -1);
		else if (direction == DOWN)	return INT2(0, 1);
		else if (direction == LEFT)	return INT2(-1, 0);
		else if (direction == RIGHT)return INT2(1, 0);
		return INT2(0, 0);
	}

	/*
		@brief	INT2�^��P�ʃx�N�g���ɕϊ�
	*/
	static D3DXVECTOR2 ConvertDirection(INT2 normal) 
	{
		if (normal == INT2(0, -1)) { return UP; }
		if (normal == INT2(0,  1)) { return DOWN; }
		if (normal == INT2(-1, 0)) { return LEFT; }
		if (normal == INT2( 1, 0)) { return RIGHT; }
		return D3DXVECTOR2(0, 0);
	}

	/*
		@brief	�������󂯎��A�΂��郍�[�J���ȕ�����Ԃ�
		@detail	�ǂ�����P�ʃx�N�g��
	*/
	static D3DXVECTOR2 ConvertLocalDirection(D3DXVECTOR2 direction, D3DXVECTOR2 LocalTargetDirection) {
		/*! �� */
		if (direction == UP) {
			/*! ���[�J���̑O�� */
			if (LocalTargetDirection == UP)			return UP;
			/*! ���[�J���̌�� */
			else if (LocalTargetDirection == DOWN)	return DOWN;
			/*! ���[�J���̍� */
			else if (LocalTargetDirection == LEFT)	return LEFT;
			/*! ���[�J���̉E */
			else if (LocalTargetDirection == RIGHT)	return RIGHT;
		}
		/*! �� */
		else if (direction == DOWN) {
			/*! ���[�J���̑O�� */
			if (LocalTargetDirection == UP)			return DOWN;
			/*! ���[�J���̌�� */
			else if (LocalTargetDirection == DOWN)	return UP;
			/*! ���[�J���̍� */
			else if (LocalTargetDirection == LEFT)	return RIGHT;
			/*! ���[�J���̉E */
			else if (LocalTargetDirection == RIGHT)	return LEFT;
		}
		/*! �� */
		else if (direction == LEFT) {
			/*! ���[�J���̑O�� */
			if (LocalTargetDirection == UP)			return LEFT;
			/*! ���[�J���̌�� */
			else if (LocalTargetDirection == DOWN)	return RIGHT;
			/*! ���[�J���̍� */
			else if (LocalTargetDirection == LEFT)	return DOWN;
			/*! ���[�J���̉E */
			else if (LocalTargetDirection == RIGHT)	return UP;
		}
		/*! �E */
		else if (direction == RIGHT) {
			/*! ���[�J���̑O�� */
			if (LocalTargetDirection == UP)			return RIGHT;
			/*! ���[�J���̌�� */
			else if (LocalTargetDirection == DOWN)	return LEFT;
			/*! ���[�J���̍� */
			else if (LocalTargetDirection == LEFT)	return UP;
			/*! ���[�J���̉E */
			else if (LocalTargetDirection == RIGHT)	return DOWN;
		}
		return D3DXVECTOR2(0, 0);
	}
}