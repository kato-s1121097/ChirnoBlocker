/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Ball.h										 */
/*	DESCRIPTION: ボールに関する処理を行う				 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/07(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#ifndef __BALL__
#define __BALL__

#include <cstdlib>
#include "Define.h"
#include "Stage.h"
#include "MCI.h"
using namespace std;

class Ball
{
	HBITMAP m_hBallBmp, m_hBallMask;
	POINT m_BallPoint;
	BOOL m_isShot;
	FLOAT m_BallVX, m_BallVY, m_MaxVX, m_MaxVY;
	SHORT m_vector;
public:
	MCI *m_reflect;

	//コンストラクタ
	Ball();

	//デストラクタ
	~Ball(void);

	//インライン関数
	HBITMAP &GetBmp()	{ return m_hBallBmp; }
	HBITMAP &GetMask()	{ return m_hBallMask; }
	DWORD GetPoint()	{ return MAKELONG(m_BallPoint.x, m_BallPoint.y); }
	BOOL IsFall()		{ return m_BallPoint.y + BALL_WIDTH > (DY + DHEIGHT) ? TRUE : FALSE; }
	BOOL IsShot()		{ return m_isShot; }
	void SetShotFlag(const BOOL isShot) { m_isShot = isShot; }
	void SetPoint(const DWORD point)	{ m_BallPoint.x = LOWORD(point); m_BallPoint.y = HIWORD(point); }

	//通常メンバ
	void Move(const RECT &bar_rect);
	BOOL Collision(Stage *stage);
};

#endif