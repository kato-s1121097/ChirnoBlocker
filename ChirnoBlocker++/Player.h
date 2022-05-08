/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Player.h									 */
/*	DESCRIPTION: プレイヤーの情報を管理する				 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/06(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#ifndef __PLAYER__
#define __PLAYER__

#include "Define.h"
#include "JoyStick.h"

class Player
{
	JoyStick *m_JoyStick;
	WORD m_barX, m_charX;
	RECT m_hitRect, m_BarHitRect;
	BOOL m_isVisible;
	BYTE m_credit;
	int m_control;
	HBITMAP m_hBarBmp, m_hBarMask, m_hCharBmp, m_hCharMask, m_hCreditBmp, m_hCreditMask;
public:
	BYTE click_flg;

	//コンストラクタ
	Player(void);

	//デストラクタ
	~Player(void);

	//インライン関数
	HBITMAP &GetBmp(BYTE type)
	{
		switch(type)
		{
		case 0:
			return m_hCharBmp;
		case 1:
			return m_hBarBmp;
		case 2:
			return m_hCreditBmp;
		default:
			return m_hCharBmp;
		}
	}

	HBITMAP &GetMask(BYTE type)
	{
		switch(type)
		{
		case 0:
			return m_hCharMask;
		case 1:
			return m_hBarMask;
		case 2:
			return m_hCreditMask;
		default:
			return m_hCharMask;
		}
	}
	void SetPoint(WORD barX)		{ m_barX = barX; m_charX = (m_barX + BAR_WIDTH / 2) - (CHAR_WIDTH / 2); }
	void SetCredit(BYTE credit)		{ m_credit = credit; }
	void SetVisible(BOOL visible)	{ m_isVisible = visible; }
	DWORD GetPoint()				{ return MAKELONG(m_charX, m_barX); }
	BYTE GetCredit()				{ return m_credit; }
	BOOL IsVisible()				{ return m_isVisible; }

	//通常メソッド
	void Move();
	BOOL Collision();
	RECT &GetRect();
};

#endif