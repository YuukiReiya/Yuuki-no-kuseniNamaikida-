#include "Bugs.h"

#include "../../../../Debug/Debug.h"
Debug d;

Bugs::Bugs()
{
}


Bugs::~Bugs()
{
	Destroy();
}

void Bugs::Init(INT2 Pos,INT2 Component)
{
	m_pSprite = new Sprite;


	m_isDelete = false;
	m_Type = Type::BUGS;
	m_Hp = 23;
	m_Atk = 19;
	m_Pos = Game::ConvertIndexToPosition(Pos);
	m_Component = Component;
	m_Direction = Game::DOWN;
	m_MoveWeight = 0.6f;
	isMove = true;
	m_AtkCount = 0;
	m_GrowCount = 0;
	m_Grow = LARVAE;
	StackInit();

	m_pSprite->SetSpriteSize((float)(GRID_SIZE), (float)(GRID_SIZE));
	m_pSprite->SetSizeScaling(1.3f);
	m_pSprite->SetSplit(3, 4);
	m_AnimCount = m_AnimIndexU = m_AnimIndexV = 0;
}

void Bugs::Destroy()
{
	SAFE_DELETE(m_pSprite);
}

void Bugs::Update()
{
	AnimUpdate();

	switch (m_Grow)
	{
	case Bugs::LARVAE:
		LarvaeUpdate();
		break;
	case Bugs::CHRYSALIS:
		ChrysalisUpdate();
		break;
	case Bugs::ADULT:
		AdultUpdate();
		break;
	default:
		break;
	}

	Growing();
}

void Bugs::Render(IDirect3DTexture9 * texture)
{
	m_pSprite->SetPos(ConvertView(m_Pos));
	m_pSprite->RenderAtlas(texture, m_AnimIndexU, m_AnimIndexV);

	char str[255];
	sprintf(str, "HP:%d養分：%d",m_Hp,m_Component.x);
	//sprintf(str, "IndexPos=x:%d,y:%d/NextIndex=x:%d,y:%d", m_IndexPos.x, m_IndexPos.y, m_NextIndexPos.x, m_NextIndexPos.y);
	SetWindowTextA(Debug::m_hWnd, str);

}

void Bugs::Damage(int damage)
{
	m_Hp -= damage;
	if (m_Hp <= 0) {
		m_Hp = 0;
		m_isDelete = true;
	}
}

void Bugs::Collision(Character & character)
{
	character.Collision(*this);
}

void Bugs::Collision(Enemy & enemy)
{
	enemy.Collision(*this);
}

void Bugs::Collision(Moss & moss)
{
	/*捕食*/
	isMove = false;
	m_Hp += m_Atk < moss.GetHp() ? m_Atk : moss.GetHp();
	m_Component.x += moss.GetComponent().x;
	moss.Damage(m_Atk);
	if (moss.GetDeleteFlags()) {
		isMove = true;
	}
}

void Bugs::Collision(Lizard & lizard)
{
	isMove = false;
}

void Bugs::Growing()
{
	switch (m_Grow)
	{
	//幼虫
	case Bugs::LARVAE:
		//Hp20以上かつ養分20以上で蛹
		if (20 < m_Hp && 20 <= m_Component.x) {
			m_Hp -= 20;
			m_Grow = CHRYSALIS;
		}
		break;
	//蛹
	case Bugs::CHRYSALIS:
		//8秒経過で成虫
		if (FPS * 8 < m_GrowCount) {
			m_Hp = 38;
			m_GrowCount = 0;
			m_Grow = ADULT;
		}
		break;
	//成虫
	case Bugs::ADULT:
		//規定フレームに達したら
		if (60 < m_GrowCount) {
			//Hp30以上かつ養分11以上
			if (30 <= m_Hp && 11 <= m_Component.x) {
				Breeding();
			}
		}
		break;
	default:
		break;
	}
}

//繁殖
void Bugs::Breeding()
{
	m_Hp -= 10;
	m_Component.x -= 6;

	Bugs* child = new Bugs;
	INT2 halfComponent = {
		m_Component.x / 2,
		m_Component.y / 2,
	};
	child->Init(m_IndexPos, halfComponent);
	EnemyManager::GetInstance().AddEnemy(child);
}

void Bugs::IndexUpdate()
{
	auto before = Game::ConvertPositionToIndex(m_Pos);
	//前の場所と変わっていなかったら
	if (m_IndexPos == before) {
		return;
	}
	m_IndexPos = Game::ConvertPositionToIndex(m_Pos);

	//判定先
	D3DXVECTOR2 dirList[] = {
		Game::ConvertLocalDirection(m_Direction,Game::UP) ,	//前方
		Game::ConvertLocalDirection(m_Direction,Game::LEFT) ,	//左方
		Game::ConvertLocalDirection(m_Direction,Game::RIGHT)	//右方
	};

	int dirNum = sizeof(dirList) / sizeof(D3DXVECTOR2);
	//移動先候補を格納
	for (int i = 0; i < dirNum; i++) {
		INT2 temp = m_IndexPos + Game::ConvertIndex(dirList[i]);
		if (TerrainManager::GetInstance().GetBreakFlag(temp)) {
			StackPushDirection(dirList[i]);
		}
	}
	D3DXVECTOR2 dir;
	INT2 temp;
	//取り出し
	if (StackElementsNum() != 0) {
		int index = rand() % StackElementsNum();
		dir = m_StackDirection[index];
		temp = Game::ConvertIndex(dir);
		m_NextIndexPos = m_IndexPos + temp;
		m_Direction = dir;
	}
	else {
		dir = Game::ConvertLocalDirection(m_Direction, Game::DOWN);
		temp = Game::ConvertIndex(dir);
		m_NextIndexPos = m_IndexPos + temp;
		m_Direction = m_Direction*(-1);
	}
	StackInit();

	/*Hpを減らす*/
	Damage(1);
}

void Bugs::AnimUpdate()
{
	m_AnimCount++;

	if (m_AnimCount > 8) {
		m_AnimIndexU++;
		if (m_AnimIndexU > 2)m_AnimIndexU = 0;
		m_AnimCount = 0;
	}

	if (m_Direction == Game::UP) {
		m_AnimIndexV = 3;
	}
	else if (m_Direction == Game::DOWN) {
		m_AnimIndexV = 0;
	}
	else if (m_Direction == Game::LEFT) {
		m_AnimIndexV = 1;
	}
	else if (m_Direction == Game::RIGHT) {
		m_AnimIndexV = 2;
	}
	m_AnimIndexU %= 3;
}

void Bugs::Move()
{
	//移動先にブロックがあるか判定
	if (!TerrainManager::GetInstance().GetBreakFlag(m_NextIndexPos)) {
	}
	//移動先にブロックは無いので移動する
	else {
		if (isMove) {
			m_Pos += m_Direction*m_MoveWeight;
		}

		//座標が移動先を超えていたら補正してあげる
		{
			bool isOver = false;//座標の超過判定
			D3DXVECTOR2 temp = Game::ConvertIndexToPosition(m_NextIndexPos);
			//座標超過判定
			if (m_Direction == Game::UP&&temp.y >= m_Pos.y) { isOver = true; }	//上
			else if (m_Direction == Game::DOWN&&temp.y <= m_Pos.y) { isOver = true; }	//下
			else if (m_Direction == Game::LEFT&&temp.x >= m_Pos.x) { isOver = true; }	//左
			else if (m_Direction == Game::RIGHT&&temp.x <= m_Pos.x) { isOver = true; }	//右

			if (isOver)
				m_Pos = temp;
		}
	}
}

//スタックへのプッシュ
void Bugs::StackPushDirection(D3DXVECTOR2 dirction)
{
	m_StackDirection[m_StackIndex] = dirction;
	m_StackIndex++;
}

//スタックの初期化
void Bugs::StackInit()
{
	for (int i = 0; i < STACK_SIZE; i++)
		m_StackDirection[i] = INITIALIZE_STACK_ELEMENTS;
	m_StackIndex = 0;
}

//スタックに格納されたNULLでない要素の数を返す
int Bugs::StackElementsNum()
{
	for (int i = 0; i < STACK_SIZE;) {
		if (m_StackDirection[i] == INITIALIZE_STACK_ELEMENTS)
			return i;
		i++;
	}

	return STACK_SIZE;
}

void Bugs::LarvaeUpdate()
{
	if (Game::TheSamePositionToGrid(m_Pos))
		IndexUpdate();
	Move();
}

void Bugs::ChrysalisUpdate()
{
	m_GrowCount++;
}

void Bugs::AdultUpdate()
{
	m_GrowCount++;
	if (Game::TheSamePositionToGrid(m_Pos))
		IndexUpdate();
	Move();
}
