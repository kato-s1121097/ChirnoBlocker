/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: MCI.h										 */
/*	DESCRIPTION:MCIÇÃêßå‰ÇçsÇ§							 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/07(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#ifndef __MYMCI__
#define __MYMCI__

#include "Define.h"

class MCI
{
#define TYPE_SE  0
#define TYPE_BGM 1
	MCI_OPEN_PARMS mop;
public:
	MCI(HWND &hWnd, LPCWSTR filename, BYTE type);
	~MCI(void);

	void Play() { mciSendCommand(mop.wDeviceID, MCI_PLAY, 0, 0); }
	void Stop()
	{
		mciSendCommand(mop.wDeviceID, MCI_STOP, 0, 0);
		mciSendCommand(mop.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
	}
};

#endif