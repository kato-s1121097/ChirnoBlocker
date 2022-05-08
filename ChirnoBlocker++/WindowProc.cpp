/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: WindowProc.cpp								 */
/*	DESCRIPTION: ���b�Z�[�W�������s��					 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/04(f::jndbcph)						 */
/*	UPDATE: 2010/02/15(f::jndbcph)						 */
/*********************************************************/
#include "WindowProc.h"

/*********************************************************/
/*	�֐��FWindowProc									 */
/*	�T�v�F�E�B���h�E�v���V�[�W��						 */
/*********************************************************/
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	DEVMODE devmode;
	PAINTSTRUCT ps;
	DWORD dwID;
	ifstream config;
	int result, i;
	char ch;
	static THREAD_PARAM param;
	static HBITMAP hBitmap;
	static HFONT hFont;

	switch(uMsg)
	{
	case WM_CREATE:

		//������@�擾
		config.open("config.dat", ifstream::binary);
		for(i = 0; i < 2; i++)
			config >> ch;

		config.close();

		//�t���X�N���[���N���m�F
		result = MessageBox(hWnd, TEXT("�t���X�N���[���ŋN�����܂����H\n�͂�(�t���X�N���[��)\n������(�E�B���h�E)\n�L�����Z��(�I��)"), TEXT("�m�F"), MB_YESNOCANCEL | MB_ICONQUESTION);
		//�t���X�N���[���N��
		switch(result)
		{
		case IDYES:
			//�f�B�X�v���C�𑜓x�ݒ�
			devmode.dmSize = sizeof(DEVMODE);
			devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
			devmode.dmPelsWidth = CLIENT_WIDTH;
			devmode.dmPelsHeight = CLIENT_HEIGHT;

			//���s
			if(ChangeDisplaySettings(&devmode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				MessageBox(hWnd, TEXT("�f�B�X�v���C�̐ݒ��ύX�ł��܂���"), NULL, MB_OK);
			}
			//����
			else
			{
				SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
				SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, 0);
			}
			break;
		case IDNO:
			ShowWindow(hWnd, SW_SHOW);
			break;
		case IDCANCEL:
			SendMessage(hWnd, WM_QUIT, 0, 0);
			break;
		}

		//�o�b�N�o�b�t�@����
		hdc = GetDC(hWnd);
		hBitmap = CreateCompatibleBitmap(hdc, CLIENT_WIDTH, CLIENT_HEIGHT);
		param.hdc = CreateCompatibleDC(hdc);
		SelectObject(param.hdc, hBitmap);
		SelectObject(param.hdc, GetStockObject(BLACK_PEN));
		SelectObject(param.hdc, GetStockObject(BLACK_BRUSH));
		SelectObject(param.hdc, GetStockObject(SYSTEM_FONT));
		SetTextColor(param.hdc, RGB(0xFF, 0xFF, 0xFF));
		SetBkMode(param.hdc, TRANSPARENT);
		ReleaseDC(hWnd, hdc);

		//�t�H���g�쐬
		hFont = CreateFont(22, 0, 0, 0, FW_NORMAL, TRUE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_ROMAN, 0);

		//�X���b�h�p�����[�^�ݒ�
		param.hWnd		= hWnd;
		param.dwFPS		= 30;
		param.control	= atoi(&ch);
		lstrcpy(param.str, TEXT("FPS�v����..."));
		//Game�N���X�̈�m��
		try
		{
			param.game = new Game(param.hdc, hWnd);
		} catch(bad_alloc ba)
		{
			return ERR_ALLOC;
		}

		//�X���b�h�N��
		CreateThread(NULL, 0, ThreadFunction, (LPVOID)&param, 0, &dwID);
		return 0;
	case WM_QUIT:
		//�o�b�N�o�b�t�@���
		DeleteDC(param.hdc);

		//GDI�I�u�W�F�N�g�j��
		DeleteObject(hBitmap);
		DeleteObject(hFont);

		//�N���X�̈���
		delete param.game;

		//�G���[���b�Z�[�W�\��
		if(wParam != 0)
		{
			switch(wParam)
			{
			case ERR_ALLOC:
				MessageBox(NULL, TEXT("�G���[�����������ׁA�Q�[�����I�����܂���"), TEXT("Allocate Error"), MB_OK);
				break;
			case ERR_FILEOPEN:
				MessageBox(NULL, TEXT("�G���[�����������ׁA�Q�[�����I�����܂���"), TEXT("File open error"), MB_OK);
				break;
			case ERR_MCI:
				MessageBox(NULL, TEXT("�G���[�����������ׁA�Q�[�����I�����܂���"), TEXT("MCI error"), MB_OK);
			case ERR_JOY:
				MessageBox(NULL, TEXT("�G���[�����������ׁA�Q�[�����I�����܂���"), TEXT("Joy error"), MB_OK);
			}
		}

		SendMessage(hWnd, WM_DESTROY, 0, 0);

		return 0;
	case WM_DESTROY:
		//�I�����b�Z�[�W�|�X�g
		PostQuitMessage(wParam);
	
		return 0;
	case WM_MOUSEMOVE:
		//������@���}�E�X�Ŗ�����ΒE�o
		if(param.control) return 0;

		//�Q�[����ʂȂ�
		if(param.game->GetDisplay() == GAME)
		{
			param.game->player->SetPoint(LOWORD(lParam) - BAR_WIDTH / 2);
			//��ʒ[
			if(LOWORD(lParam) - (BAR_WIDTH / 2) < DX)
				param.game->player->SetPoint(DX);
			if(HIWORD(param.game->player->GetPoint()) + BAR_WIDTH > (DX + DWIDTH))
				param.game->player->SetPoint(DX + DWIDTH - BAR_WIDTH);
		}
		return 0;
	case WM_LBUTTONDOWN:
		//�N���b�N�t���O�𗧂Ă�
		param.game->Clicked(LEFT_CLICKED);
		if(param.game->GetDisplay() == GAME)
			param.game->player->click_flg |= LEFT_CLICKED;
		return 0;
	case WM_RBUTTONDOWN:
		//�N���b�N�t���O�𗧂Ă�
		param.game->Clicked(RIGHT_CLICKED);
		if(param.game->GetDisplay() == GAME)
			param.game->player->click_flg = RIGHT_CLICKED;
		return 0;
	case WM_KEYDOWN:
		switch(wParam)
		{
		//�G�X�P�[�v�L�[�������ꂽ��I��
		case VK_ESCAPE:
			SendMessage(hWnd, WM_QUIT, 0, 0);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//�Q�[����ʕ\��
		param.game->ShowDisplay(param.hdc, hWnd);
		SetTextColor(param.hdc, RGB(0xFF, 0xFF, 0xFF));
		SelectObject(param.hdc, hFont);
		TextOut(param.hdc, DX + DWIDTH + 100, CLIENT_HEIGHT - 24, param.str, lstrlen(param.str));
		BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, param.hdc, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}