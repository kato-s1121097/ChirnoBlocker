/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Define.h									 */
/*	DESCRIPTION:�萔���`����							 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/04(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#ifndef __DEFINE__
#define __DEFINE__

#include <windows.h>

/*********************************************************/
/* ��ʐ݌v�֘A											 */
/*********************************************************/
//�S��
const WORD CLIENT_WIDTH	 = 800;
const WORD CLIENT_HEIGHT = 600;
const WORD WINDOW_WIDTH  = (CLIENT_WIDTH + GetSystemMetrics(SM_CXFIXEDFRAME) * 2);
const WORD WINDOW_HEIGHT = (CLIENT_HEIGHT + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION));

//�Q�[�����
const WORD DX = 20;
const WORD DY = 20;
const WORD DWIDTH  = 512;
const WORD DHEIGHT = 560;
const WORD INTRO_X = 542;
const WORD INTRO_WIDTH = 268;
const WORD BLOCK_WIDTH = 64;
const WORD BLOCK_HEIGHT	= 32;

//�^�C�g�����
const WORD STARTRECT_LEFT = (CLIENT_WIDTH / 2 - 60);
const WORD STARTRECT_TOP  = (CLIENT_HEIGHT - 212);
const WORD STARTRECT_RIGHT = (STARTRECT_LEFT + 115);
const WORD STARTRECT_BOTTOM	= (STARTRECT_TOP + 38);
const WORD QUITRECT_LEFT	= STARTRECT_LEFT;
const WORD QUITRECT_TOP		= (CLIENT_HEIGHT - 149);
const WORD QUITRECT_RIGHT	= STARTRECT_RIGHT;
const WORD QUITRECT_BOTTOM	= (QUITRECT_TOP + 38);

/*********************************************************/
/*	�v���C���[�֘A										 */
/*********************************************************/
const WORD BAR_Y = 532;
const WORD BAR_WIDTH = 128;
const WORD BAR_HEIGHT = 16;
const WORD CHAR_Y = 548;
const WORD CHAR_WIDTH = 32;
const WORD CHAR_HEIGHT = 32;

/*********************************************************/
/*	�{�[���֘A											 */
/*********************************************************/
//�͈�
const BYTE BALL_WIDTH = 16;
const BYTE BALL_HEIGHT = 16;
//�ő�����x
const BYTE BALL_MAXV = 8;
//����
const BYTE VECTOR_RIGHT	= 0x01;
const BYTE VECTOR_LEFT	= 0x02;
const BYTE VECTOR_UP	= 0x04;
const BYTE VECTOR_DOWN	= 0x08;

/*********************************************************/
/*	�t���O�֘A											 */
/*********************************************************/
//���[�h�t���O
const BYTE LOADFLG_INITIALIZE		= 0x01;
const BYTE LOADFLG_FROMTITLE		= 0x02;
const BYTE LOADFLG_FROMSTAGECLEAR	= 0x04;
const BYTE LOADFLG_FROMGAMEOVER		= 0x08;
const BYTE LOADFLG_FROMCONTINUE		= 0x10;
const BYTE LOADFLG_ALLCLEAR			= 0x20;

//�N���b�N�t���O
const BYTE LEFT_CLICKED		= 0x01;
const BYTE RIGHT_CLICKED	= 0x02;

/*********************************************************/
/*	�W���C�X�e�B�b�N									 */
/*********************************************************/
const WORD BORDER_LOW	= (32768 - 13096);
const WORD BORDER_HIGH	= (32768 + 13096);

/*********************************************************/
/*	�G���[												 */
/*********************************************************/
const INT ERR_ALLOC		= -1;
const INT ERR_FILEOPEN	= -2;
const INT ERR_MCI		= -3;
const INT ERR_JOY		= -4;

#endif