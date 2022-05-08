/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: WindowProc.h								 */
/*	DESCRIPTION: ���b�Z�[�W�������s��					 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/04(f::jndbcph)						 */
/*	UPDATE: 2010/01/05(f::jndbcph)						 */
/*********************************************************/
#ifndef __WINDOWPROC__
#define __WINDOWPROC__

#include <fstream>	//For_Log
#include <new>		//For_bad_allocation
#include "Game.h"
#include "Define.h"
#include "Thread.h"
using namespace std;

/*********************************************************/
/*	�֐��FWindowProc									 */
/*	�T�v�F�E�B���h�E�v���V�[�W��						 */
/*********************************************************/
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lparam);

#endif