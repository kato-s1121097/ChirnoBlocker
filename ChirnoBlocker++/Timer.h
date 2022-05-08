/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Timer.h									 */
/*	DESCRIPTION: タイマー機能							 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/11(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#ifndef __TIMER__
#define __TIMER__

#include <windows.h>
#pragma comment(lib, "winmm.lib")

class Timer
{
	DWORD m_BeginTime;
public:
	//コンストラクタ
	Timer(void) { m_BeginTime = timeGetTime(); }

	//メソッド
	DWORD GetInterval() { return (timeGetTime() - m_BeginTime); }
	void ResetTime()	{ m_BeginTime = timeGetTime(); }
};

#endif