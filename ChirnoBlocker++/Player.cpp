/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Player.cpp									 */
/*	DESCRIPTION: プレイヤーの情報を管理する				 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/06(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#include <fstream>
#include "Player.h"
using namespace std;

/*********************************************************/
/*	コンストラクタ										 */
/*********************************************************/
Player::Player(void)
{
	//コントロール方法取得
	ifstream config("config.dat", ifstream::binary);
	int i;
	char ch;
	for(i = 0; i < 2; i++)
		config >> ch;
	m_control = atoi(&ch);
	config.close();

	//ジョイスティック初期化
	if(m_control == 2)
		m_JoyStick = new JoyStick();

	//ビットマップ読み込み
	m_hBarBmp		=	(HBITMAP)LoadImage(NULL, TEXT("image\\Bar.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_hBarMask		=	(HBITMAP)LoadImage(NULL, TEXT("image\\BarMask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_hCharBmp		=	(HBITMAP)LoadImage(NULL, TEXT("image\\Char.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_hCharMask		=	(HBITMAP)LoadImage(NULL, TEXT("image\\CharMask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_hCreditBmp	=	(HBITMAP)LoadImage(NULL, TEXT("image\\ChirnoCredit.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_hCreditMask	=	(HBITMAP)LoadImage(NULL, TEXT("image\\ChirnoCreditMask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	//座標初期化
	m_barX = 197;
	m_charX = 245;
	m_hitRect.left = 253;
	m_hitRect.top = 556;
	m_hitRect.right = 269;
	m_hitRect.bottom = 572;
	m_BarHitRect.left = 197;
	m_BarHitRect.top = BAR_Y;
	m_BarHitRect.right = 620;
	m_BarHitRect.bottom = BAR_Y + BAR_HEIGHT;

	//残機初期化
	m_credit = 3;

	//可視フラグ初期化
	m_isVisible = TRUE;
}

/*********************************************************/
/*	デストラクタ										 */
/*********************************************************/
Player::~Player(void)
{
	//JoyStickクラス破棄
	if(m_control == 2)
		delete m_JoyStick;

	//ビットマップ破棄
	DeleteObject(m_hBarBmp);
	DeleteObject(m_hBarMask);
	DeleteObject(m_hCharBmp);
	DeleteObject(m_hCharMask);
	DeleteObject(m_hCreditBmp);
	DeleteObject(m_hCreditMask);
}

/*********************************************************/
/*	メソッド:Move										 */
/*	機能:プレイヤーの座標を移動する						 */
/*********************************************************/
void Player::Move()
{
	static FLOAT v = 5.0f;
	const FLOAT MAX_V = 9.0f;
	
	switch(m_control)
	{
	case 1:
		//左が押されてる
		if(GetKeyState(VK_LEFT) & 0x8000)
		{
			m_barX  -= static_cast<WORD>(v);	//速度分移動(バー)
			m_charX -= static_cast<WORD>(v);	//速度分移動(キャラ)
			v += 0.5f;		//速度上昇
		}
		//右が押されてる
		if(GetKeyState(VK_RIGHT) & 0x8000)
		{
			m_barX	+= static_cast<WORD>(v);	//速度分移動(バー)
			m_charX += static_cast<WORD>(v);	//速度分移動(キャラ)
			v += 0.5f;		//速度上昇
		}
		break;
	case 2:
		//左が押されてる
		if(m_JoyStick->CheckStick(JoyStick::JOY_VECTORLEFT))
		{
			m_barX  -= static_cast<WORD>(v);
			m_charX -= static_cast<WORD>(v);
			v += 0.5f;
		}
		//右が押されてる
		if(m_JoyStick->CheckStick(JoyStick::JOY_VECTORRIGHT))
		{
			m_barX  += static_cast<WORD>(v);
			m_charX += static_cast<WORD>(v);
			v += 0.5f;
		}

		break;
	}

	//速度チェック
	if(v > MAX_V) v = MAX_V;

	//領域チェック
	if(m_barX < DX)
	{
		m_barX = DX;
		m_charX = DX + (BAR_WIDTH / 2 - CHAR_WIDTH / 2);
	}
	if(m_barX + BAR_WIDTH > DX + DWIDTH)
	{
		m_barX = (DX + DWIDTH - BAR_WIDTH);
		m_charX = m_barX + (BAR_WIDTH / 2 - CHAR_WIDTH / 2);
	}
	
	click_flg &= 0x00;
}

/*********************************************************/
/*	メソッド:Collision									 */
/*	機能:ボス戦時の敵の弾との衝突判定を行う				 */
/*********************************************************/
BOOL Player::Collision()
{
	return TRUE;
}

/*********************************************************/
/*	メソッド:GetRect									 */
/*	機能:バーの当たり判定を取得する						 */
/*********************************************************/
RECT &Player::GetRect()
{
	m_BarHitRect.left = m_barX;
	m_BarHitRect.right = m_barX + BAR_WIDTH;
	return m_BarHitRect;
}