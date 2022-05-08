/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Player.cpp									 */
/*	DESCRIPTION: �v���C���[�̏����Ǘ�����				 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/06(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#include <fstream>
#include "Player.h"
using namespace std;

/*********************************************************/
/*	�R���X�g���N�^										 */
/*********************************************************/
Player::Player(void)
{
	//�R���g���[�����@�擾
	ifstream config("config.dat", ifstream::binary);
	int i;
	char ch;
	for(i = 0; i < 2; i++)
		config >> ch;
	m_control = atoi(&ch);
	config.close();

	//�W���C�X�e�B�b�N������
	if(m_control == 2)
		m_JoyStick = new JoyStick();

	//�r�b�g�}�b�v�ǂݍ���
	m_hBarBmp		=	(HBITMAP)LoadImage(NULL, TEXT("image\\Bar.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_hBarMask		=	(HBITMAP)LoadImage(NULL, TEXT("image\\BarMask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_hCharBmp		=	(HBITMAP)LoadImage(NULL, TEXT("image\\Char.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_hCharMask		=	(HBITMAP)LoadImage(NULL, TEXT("image\\CharMask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_hCreditBmp	=	(HBITMAP)LoadImage(NULL, TEXT("image\\ChirnoCredit.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_hCreditMask	=	(HBITMAP)LoadImage(NULL, TEXT("image\\ChirnoCreditMask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	//���W������
	m_barX = 197;
	m_charX = 245;
	m_hitRect.left = 253;
	m_hitRect.top = 556;
	m_hitRect.right = 269;
	m_hitRect.bottom = 572;
	m_BarHitRect.left = 197;
	m_BarHitRect.top = BAR_Y;
	m_BarHitRect.right = 620;
	m_BarHitRect.bottom = BAR_Y + BAR_HEIGHT;

	//�c�@������
	m_credit = 3;

	//���t���O������
	m_isVisible = TRUE;
}

/*********************************************************/
/*	�f�X�g���N�^										 */
/*********************************************************/
Player::~Player(void)
{
	//JoyStick�N���X�j��
	if(m_control == 2)
		delete m_JoyStick;

	//�r�b�g�}�b�v�j��
	DeleteObject(m_hBarBmp);
	DeleteObject(m_hBarMask);
	DeleteObject(m_hCharBmp);
	DeleteObject(m_hCharMask);
	DeleteObject(m_hCreditBmp);
	DeleteObject(m_hCreditMask);
}

/*********************************************************/
/*	���\�b�h:Move										 */
/*	�@�\:�v���C���[�̍��W���ړ�����						 */
/*********************************************************/
void Player::Move()
{
	static FLOAT v = 5.0f;
	const FLOAT MAX_V = 9.0f;
	
	switch(m_control)
	{
	case 1:
		//����������Ă�
		if(GetKeyState(VK_LEFT) & 0x8000)
		{
			m_barX  -= static_cast<WORD>(v);	//���x���ړ�(�o�[)
			m_charX -= static_cast<WORD>(v);	//���x���ړ�(�L����)
			v += 0.5f;		//���x�㏸
		}
		//�E��������Ă�
		if(GetKeyState(VK_RIGHT) & 0x8000)
		{
			m_barX	+= static_cast<WORD>(v);	//���x���ړ�(�o�[)
			m_charX += static_cast<WORD>(v);	//���x���ړ�(�L����)
			v += 0.5f;		//���x�㏸
		}
		break;
	case 2:
		//����������Ă�
		if(m_JoyStick->CheckStick(JoyStick::JOY_VECTORLEFT))
		{
			m_barX  -= static_cast<WORD>(v);
			m_charX -= static_cast<WORD>(v);
			v += 0.5f;
		}
		//�E��������Ă�
		if(m_JoyStick->CheckStick(JoyStick::JOY_VECTORRIGHT))
		{
			m_barX  += static_cast<WORD>(v);
			m_charX += static_cast<WORD>(v);
			v += 0.5f;
		}

		break;
	}

	//���x�`�F�b�N
	if(v > MAX_V) v = MAX_V;

	//�̈�`�F�b�N
	if(m_barX < DX)
	{
		m_barX = DX;
		m_charX = DX + (BAR_WIDTH / 2 - CHAR_WIDTH / 2);
	}
	if(m_barX + BAR_WIDTH > DX + DWIDTH)
	{
		m_barX = (DX + DWIDTH - BAR_WIDTH);
		m_charX = m_barX + (BAR_WIDTH / 2 - CHAR_WIDTH / 2);
	}
	
	click_flg &= 0x00;
}

/*********************************************************/
/*	���\�b�h:Collision									 */
/*	�@�\:�{�X�펞�̓G�̒e�Ƃ̏Փ˔�����s��				 */
/*********************************************************/
BOOL Player::Collision()
{
	return TRUE;
}

/*********************************************************/
/*	���\�b�h:GetRect									 */
/*	�@�\:�o�[�̓����蔻����擾����						 */
/*********************************************************/
RECT &Player::GetRect()
{
	m_BarHitRect.left = m_barX;
	m_BarHitRect.right = m_barX + BAR_WIDTH;
	return m_BarHitRect;
}