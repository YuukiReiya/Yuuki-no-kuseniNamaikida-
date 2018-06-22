/*
	@file	Camera2D.h
	@brief	�J����
	@author	�ԏ� �G�P
*/
#pragma once
#include "../Common/Common.h"

/*
	@def	�J�������W�ւ̕ϊ��}�N��
*/
#define ConvertView(Pos) Pos-Camera2D::GetInstance().GetPos()

/*
	@brief	�J�����̊Ǘ��N���X
*/
class Camera2D
{
public:
	/*! �f�X�g���N�^ */
	~Camera2D();

	/*! �V���O���g�� */
	static Camera2D& GetInstance() {
		static Camera2D instance;
		return instance;
	}

	/*! �Z�b�^�[ */
	void SetPos(D3DXVECTOR2 pos) { m_Pos = pos; }
	void SetPos(int x, int y) { m_Pos.x = (float)x, m_Pos.y = (float)y; }
	void SetZoom(float zoom) { m_Zoom = zoom; }
	/*! �Q�b�^�[ */
	const D3DXVECTOR2 GetPos()const { return m_Pos; }
	const float GetZoom()const { return m_Zoom; }
private:
	/*! �R���X�g���N�^ */
	Camera2D();

	D3DXVECTOR2 m_Pos;			/*!< �ʒu */
	float m_Zoom;				/*!< �Y�[��*/
};

