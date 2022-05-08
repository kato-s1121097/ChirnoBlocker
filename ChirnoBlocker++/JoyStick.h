/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: JoyStick.h									 */
/*	DESCRIPTION: ジョイスティックの情報を操作する		 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/17(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#ifndef __JOYSTICK__
#define __JOYSTICK__

#include "Define.h"

class JoyStick
{
	JOYINFOEX m_JoyInfo;
public:
	//外部公開定数
	static const BYTE JOY_VECTORLEFT	= 0x01;
	static const BYTE JOY_VECTORRIGHT	= 0x02;
	static const BYTE JOY_VECTORUP		= 0x04;
	static const BYTE JOY_VECTORDOWN	= 0x08;

	//コンストラクタ
	JoyStick(void);

	//インライン関数
	BOOL CheckSet() { return joyGetPosEx(JOYSTICKID1, &m_JoyInfo) == JOYERR_NOERROR ? TRUE : FALSE; }

	//通常メソッド
	BOOL CheckStick(BYTE vector);
	BOOL CheckButton(DWORD button);
};

#endif