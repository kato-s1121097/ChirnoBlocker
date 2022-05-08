/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: JoyStick.cpp								 */
/*	DESCRIPTION: �W���C�X�e�B�b�N�̏��𑀍삷��		 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/17(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#include "JoyStick.h"

/*********************************************************/
/*	�R���X�g���N�^										 */
/*********************************************************/
JoyStick::JoyStick(void)
{
	//JOYINFOEX�\���̏�����
	m_JoyInfo.dwSize  = sizeof(JOYINFOEX);
	m_JoyInfo.dwFlags = JOY_RETURNALL;
}

/*********************************************************/
/*	���\�b�h:CheckStick									 */
/*	�@�\:�w�肵�������ɃX�e�B�b�N���|����Ă��邩�m�F����*/
/*	����:vector = ���ׂ�������							 */
/*	�ߒl:TRUE = ���͂���Ă��� FALSE = ���͂���Ă��Ȃ�	 */
/*********************************************************/
BOOL JoyStick::CheckStick(BYTE vector)
{
	//�W���C�X�e�B�b�N���擾
	if(joyGetPosEx(JOYSTICKID1, &m_JoyInfo) != JOYERR_NOERROR) return FALSE;

	//�����ɂ���ĕ��򂵂Ē��ׂ�
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
/*	���\�b�h:CheckButton								 */
/*	�@�\:�w�肵���{�^�������͂���Ă��邩�m�F����		 */
/*	����:button	= �m�F�������{�^��						 */
/*	�ߒl:TRUE = ���͂���Ă��� FALSE = ���͂���Ă��Ȃ�	 */
/*********************************************************/
BOOL JoyStick::CheckButton(DWORD button)
{
	//�W���C�X�e�B�b�N���擾
	if(joyGetPosEx(JOYSTICKID1, &m_JoyInfo) != JOYERR_NOERROR) return FALSE;

	//�����Ɏw�肳�ꂽ�{�^���̔���
	return (m_JoyInfo.dwButtons & button) ? TRUE : FALSE;
}