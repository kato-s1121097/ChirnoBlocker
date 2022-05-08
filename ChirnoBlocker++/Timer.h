/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Timer.h									 */
/*	DESCRIPTION: �^�C�}�[�@�\							 */
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
	//�R���X�g���N�^
	Timer(void) { m_BeginTime = timeGetTime(); }

	//���\�b�h
	DWORD GetInterval() { return (timeGetTime() - m_BeginTime); }
	void ResetTime()	{ m_BeginTime = timeGetTime(); }
};

#endif