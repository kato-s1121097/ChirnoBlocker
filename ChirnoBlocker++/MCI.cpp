/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: MCI.cpp									 */
/*	DESCRIPTION:MCIの制御を行う							 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/07(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#include "MCI.h"
#include <fstream>
using namespace std;

/*********************************************************/
/*	コンストラクタ										 */
/*********************************************************/
MCI::MCI(HWND &hWnd, LPCWSTR filename, BYTE type)
{
	mop.dwCallback = (DWORD)hWnd;
	mop.lpstrElementName = filename;

	switch(type)
	{
	case TYPE_SE:
		mop.lpstrDeviceType = reinterpret_cast<LPCWSTR>(MCI_DEVTYPE_WAVEFORM_AUDIO);
		break;
	case TYPE_BGM:
		mop.lpstrDeviceType = reinterpret_cast<LPCWSTR>(MCI_DEVTYPE_SEQUENCER);
		break;
	}
	if(mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD)&mop))
	{
		SendMessage(hWnd, WM_QUIT, ERR_MCI, 0);
	}
}


/*********************************************************/
/*	デストラクタ										 */
/*********************************************************/
MCI::~MCI(void)
{
	mciSendCommand(mop.wDeviceID, MCI_CLOSE, 0, 0);
}