/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Thread.h									 */
/*	DESCRIPTION: �Q�[���p�X���b�h						 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/04(f::jndbcph)						 */
/*	UPDATE: 2010/01/16(f::jndbcph)						 */
/*********************************************************/
#ifndef __THREAD__
#define __THREAD__

#include "Define.h"
#include "Game.h"
#pragma comment(lib, "winmm.lib")

//�A�j���[�V�����X���b�h�p�\����
typedef struct _THREAD_PARAM
{
	HWND hWnd;
	HDC hdc;
	Game *game;
	DWORD dwFPS;
	TCHAR str[64];
	int control;
} THREAD_PARAM;

/*********************************************************/
/*	�X���b�h�t�@���N�V����								 */
/*********************************************************/
DWORD WINAPI ThreadFunction(LPVOID vdParam);

#endif