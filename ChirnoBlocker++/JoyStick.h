/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: JoyStick.h									 */
/*	DESCRIPTION: �W���C�X�e�B�b�N�̏��𑀍삷��		 */
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
	//�O�����J�萔
	static const BYTE JOY_VECTORLEFT	= 0x01;
	static const BYTE JOY_VECTORRIGHT	= 0x02;
	static const BYTE JOY_VECTORUP		= 0x04;
	static const BYTE JOY_VECTORDOWN	= 0x08;

	//�R���X�g���N�^
	JoyStick(void);

	//�C�����C���֐�
	BOOL CheckSet() { return joyGetPosEx(JOYSTICKID1, &m_JoyInfo) == JOYERR_NOERROR ? TRUE : FALSE; }

	//�ʏ탁�\�b�h
	BOOL CheckStick(BYTE vector);
	BOOL CheckButton(DWORD button);
};

#endif