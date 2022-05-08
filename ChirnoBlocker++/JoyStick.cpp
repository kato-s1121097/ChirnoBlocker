/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: JoyStick.cpp								 */
/*	DESCRIPTION: ジョイスティックの情報を操作する		 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/17(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#include "JoyStick.h"

/*********************************************************/
/*	コンストラクタ										 */
/*********************************************************/
JoyStick::JoyStick(void)
{
	//JOYINFOEX構造体初期化
	m_JoyInfo.dwSize  = sizeof(JOYINFOEX);
	m_JoyInfo.dwFlags = JOY_RETURNALL;
}

/*********************************************************/
/*	メソッド:CheckStick									 */
/*	機能:指定した方向にスティックが倒されているか確認する*/
/*	引数:vector = 調べたい方向							 */
/*	戻値:TRUE = 入力されている FALSE = 入力されていない	 */
/*********************************************************/
BOOL JoyStick::CheckStick(BYTE vector)
{
	//ジョイスティック情報取得
	if(joyGetPosEx(JOYSTICKID1, &m_JoyInfo) != JOYERR_NOERROR) return FALSE;

	//引数によって分岐して調べる
	switch(vector)
	{
	case JOY_VECTORLEFT:
		return (m_JoyInfo.dwXpos < BORDER_LOW)  ? TRUE : FALSE;
	case JOY_VECTORRIGHT:
		return (m_JoyInfo.dwXpos > BORDER_HIGH) ? TRUE : FALSE;
	case JOY_VECTORUP:
		return (m_JoyInfo.dwYpos < BORDER_LOW)  ? TRUE : FALSE;
	case JOY_VECTORDOWN:
		return (m_JoyInfo.dwYpos > BORDER_HIGH) ? TRUE : FALSE;
	default:
		return FALSE;
	}
}

/*********************************************************/
/*	メソッド:CheckButton								 */
/*	機能:指定したボタンが入力されているか確認する		 */
/*	引数:button	= 確認したいボタン						 */
/*	戻値:TRUE = 入力されている FALSE = 入力されていない	 */
/*********************************************************/
BOOL JoyStick::CheckButton(DWORD button)
{
	//ジョイスティック情報取得
	if(joyGetPosEx(JOYSTICKID1, &m_JoyInfo) != JOYERR_NOERROR) return FALSE;

	//引数に指定されたボタンの判定
	return (m_JoyInfo.dwButtons & button) ? TRUE : FALSE;
}