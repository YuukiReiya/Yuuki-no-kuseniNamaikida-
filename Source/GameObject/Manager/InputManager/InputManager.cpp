/*
	@file	InputManager.cpp
	@brief	入力系統を管理
	@detail	シングルトンのマネージャークラス
	@author	番場 宥輝
*/
#include "InputManager.h"
#include "../../../Input/Keyboard/Keyboard.h"

/*
	@brief	名前空間
*/
using namespace Keyboard;


/*
	@brief	更新
*/
void InputManager::Update()
{
	Keyboard::Update();
}

/*
	@brief	ボタンが押されてる間
*/
bool InputManager::GetKey(Key key)
{
	switch (key)
	{
	case InputManager::DECIDE:
		return(GetButton(Keyboard::SPACE)) || (GetButton('X')) || (GetButton('J'));
		break;
	case InputManager::CANCEL:
		return(GetButton(Keyboard::BACK)) || (GetButton('Z'));
		break;
	case InputManager::DIG:
		return(GetButton('X')) || (GetButton('J'));
		break;
	case InputManager::SEARCH:
		return(GetButton(Keyboard::SPACE));
		break;
	case InputManager::LEFT:
		return(GetButton(Keyboard::LEFT)) || (GetButton('A'));
		break;
	case InputManager::RIGHT:
		return(GetButton(Keyboard::RIGHT)) || (GetButton('D'));
		break;
	case InputManager::UP:
		return(GetButton(Keyboard::UP)) || (GetButton('W'));
		break;
	case InputManager::DOWN:
		return(GetButton(Keyboard::DOWN)) || (GetButton('S'));
		break;
	default:
		break;
	}

	return false;
}

/*
	@brief	ボタンが押された瞬間
*/
bool InputManager::GetKeyDown(Key key)
{
	switch (key)
	{
	case InputManager::DECIDE:
		return(GetButtonDown(Keyboard::SPACE)) || (GetButtonDown('X')) || (GetButtonDown('J'));
		break;
	case InputManager::CANCEL:
		return(GetButtonDown(Keyboard::BACK)) || (GetButtonDown('Z'));
		break;
	case InputManager::DIG:
		return(GetButtonDown('X')) || (GetButtonDown('J'));
		break;
	case InputManager::SEARCH:
		return(GetButtonDown(Keyboard::SPACE));
		break;
	case InputManager::LEFT:
		return(GetButtonDown(Keyboard::LEFT)) || (GetButtonDown('A'));
		break;
	case InputManager::RIGHT:
		return(GetButtonDown(Keyboard::RIGHT)) || (GetButtonDown('D'));
		break;
	case InputManager::UP:
		return(GetButtonDown(Keyboard::UP)) || (GetButtonDown('W'));
		break;
	case InputManager::DOWN:
		return(GetButtonDown(Keyboard::DOWN)) || (GetButtonDown('S'));
		break;
	default:
		break;
	}

	return false;
}

/*
	@brief	ボタンが離された瞬間
*/
bool InputManager::GetKeyUp(Key key)
{
	switch (key)
	{
	case InputManager::DECIDE:
		return(GetButtonUp(Keyboard::SPACE)) || (GetButtonUp('X')) || (GetButtonUp('J'));
		break;
	case InputManager::CANCEL:
		return(GetButtonUp(Keyboard::BACK)) || (GetButtonUp('Z'));
		break;
	case InputManager::DIG:
		return(GetButtonUp('X')) || (GetButtonUp('J'));
		break;
	case InputManager::SEARCH:
		return(GetButtonUp(Keyboard::SPACE));
		break;
	case InputManager::LEFT:
		return(GetButtonUp(Keyboard::LEFT)) || (GetButtonUp('A'));
		break;
	case InputManager::RIGHT:
		return(GetButtonUp(Keyboard::RIGHT)) || (GetButtonUp('D'));
		break;
	case InputManager::UP:
		return(GetButtonUp(Keyboard::UP)) || (GetButtonUp('W'));
		break;
	case InputManager::DOWN:
		return(GetButtonUp(Keyboard::DOWN)) || (GetButtonUp('S'));
		break;
	default:
		break;
	}


	return false;
}

/*
	@brief	コンストラクタ
*/
InputManager::InputManager()
{
}

/*
	@brief	デストラクタ
*/
InputManager::~InputManager()
{
}
