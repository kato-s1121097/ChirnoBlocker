/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Ball.cpp									 */
/*	DESCRIPTION: �{�[���Ɋւ��鏈�����s��				 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/07(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#include "Ball.h"

/*********************************************************/
/*	�R���X�g���N�^										 */
/*********************************************************/
Ball::Ball()
{
	//�r�b�g�}�b�v�ǂݍ���
	m_hBallBmp = (HBITMAP)LoadImage(NULL, TEXT("image\\Ball.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_hBallMask = (HBITMAP)LoadImage(NULL, TEXT("image\\BallMask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//���W������
	m_BallPoint.x = 253;
	m_BallPoint.y = 516;

	//�t���O������
	m_isShot = FALSE;

	//�����x������
	m_BallVX = 0.5f;
	m_BallVY = 0.5f;
	m_MaxVX = BALL_MAXV;
	m_MaxVY = BALL_MAXV;

	//����������
	m_vector = VECTOR_UP;

	//�����n�񏉊���
	srand(timeGetTime());
}

/*********************************************************/
/*	�f�X�g���N�^										 */
/*********************************************************/
Ball::~Ball(void)
{
	//�r�b�g�}�b�v�j��
	DeleteObject(m_hBallBmp);
	DeleteObject(m_hBallMask);
}

/*********************************************************/
/*	���\�b�h:Move										 */
/*	�@�\:�{�[���̈ړ��������s��							 */
/*********************************************************/
void Ball::Move(const RECT &bar_rect)
{
	static BOOL reflect_flg = FALSE;
	static DWORD reflectTime;

	if(!m_isShot)
	{
		m_BallVX = m_BallVY = 0.3f;
		m_vector &= 0x00;
		m_vector |= VECTOR_UP;
		return;
	}

	/*****************************************************/
	/* ����												 */
	/*****************************************************/
	m_BallVX += 0.4f;
	m_BallVY += 0.4f;

	//�ő�����x���z����
	if(m_BallVX > m_MaxVX)
		m_BallVX = m_MaxVX;
	if(m_BallVY > m_MaxVY)
		m_BallVY = m_MaxVY;

	/*****************************************************/
	/* �{�[���ړ�										 */
	/*****************************************************/
	//��
	if(m_vector & VECTOR_LEFT)
		m_BallPoint.x -= (LONG)m_BallVX;
	//�E
	else if(m_vector & VECTOR_RIGHT)
		m_BallPoint.x += (LONG)m_BallVX;
	//��
	if(m_vector & VECTOR_UP)
		m_BallPoint.y -= (LONG)m_BallVY;
	//�o�[
	else if(m_vector & VECTOR_DOWN)
		m_BallPoint.y += (LONG)m_BallVY;

	/*****************************************************/
	/* ��ʒ[����										 */
	/*****************************************************/
	//���[
	if(m_BallPoint.x < DX)
	{
		m_BallPoint.x = DX;
		m_vector ^= VECTOR_LEFT;
		m_vector |= VECTOR_RIGHT;

		//���ʉ��Đ�
		reflect_flg = TRUE;
		reflectTime = timeGetTime();
		m_reflect->Play();
	}
	//�E�[
	if(m_BallPoint.x + BALL_WIDTH > DX + DWIDTH)
	{
		m_BallPoint.x = DX + DWIDTH - BALL_WIDTH;
		m_vector ^= VECTOR_RIGHT;
		m_vector |= VECTOR_LEFT;
		//���ʉ��Đ�
		reflect_flg = TRUE;
		reflectTime = timeGetTime();
		m_reflect->Play();
	}
	//��
	if(m_BallPoint.y < DY)
	{
		m_BallPoint.y = DY;
		m_vector ^= VECTOR_UP;
		m_vector |= VECTOR_DOWN;
		//���ʉ��Đ�
		reflect_flg = TRUE;
		reflectTime = timeGetTime();
		m_reflect->Play();
	}
	//�o�[
	if(m_BallPoint.y + BALL_HEIGHT > bar_rect.top && m_BallPoint.y < bar_rect.bottom && m_BallPoint.x + BALL_WIDTH > bar_rect.left && m_BallPoint.x < bar_rect.right)
	{
		//�����ŋ��̋O��������
		switch(rand() % 26)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			m_vector &= ~VECTOR_RIGHT;
			m_vector |= VECTOR_LEFT;
			m_MaxVX = m_MaxVY = BALL_MAXV;
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			m_vector &= ~VECTOR_LEFT;
			m_vector |= VECTOR_RIGHT;
			m_MaxVX = m_MaxVY = BALL_MAXV;
			break;
		case 8:
		case 9:
			m_vector &= ~(VECTOR_LEFT | VECTOR_RIGHT);
			break;
		case 10:
			m_vector &= ~VECTOR_RIGHT;
			m_vector |= VECTOR_LEFT;
			m_BallVX = 0;
			m_MaxVX = m_MaxVY = BALL_MAXV;
			break;
		case 11:
			m_vector &= ~VECTOR_LEFT;
			m_vector |= VECTOR_RIGHT;
			m_BallVX = 0;
			m_MaxVX = m_MaxVY = BALL_MAXV;
			break;
		case 12:
		case 13:
		case 14:
		case 15:
			m_MaxVX = 6;
			break;
		case 16:
		case 17:
		case 18:
		case 19:
			m_MaxVY = 6;
			break;
		case 20:
		case 21:
		case 22:
			m_MaxVX = 10;
			break;
		case 23:
		case 24:
		case 25:
			m_MaxVY = 10;
			break;
		}

		m_vector &= ~VECTOR_DOWN;
		m_vector |= VECTOR_UP;

		//���ʉ��Đ�
		reflect_flg = TRUE;
		reflectTime = timeGetTime();
		m_reflect->Play();
	}

	if(reflect_flg && timeGetTime() - reflectTime > 100)
	{
		m_reflect->Stop();
		reflect_flg = FALSE;
	}

}

/*********************************************************/
/*	���\�b�h:Collision									 */
/*	�@�\:�{�[���̏Փ˔�����s��							 */
/*	����:*stage = �X�e�[�W�̃f�[�^�ւ̃|�C���^			 */
/*	�ߒl:TRUE = �Փ˂��� FALSE = �Փ˂��Ȃ�����			 */
/*********************************************************/
BOOL Ball::Collision(Stage *stage)
{
	int i = 0;
	BOOL isCollision = FALSE;

	for(; i < 96; i++)
	{
		if(!stage->GetBlockState(i)) continue;

		if(m_BallPoint.x + BALL_WIDTH > DX + (i % 8) * BLOCK_WIDTH && m_BallPoint.x < DX + (i % 8) * BLOCK_WIDTH + BLOCK_WIDTH && m_BallPoint.y < DY + i / 8 * BLOCK_HEIGHT + BLOCK_HEIGHT && m_BallPoint.y + BALL_HEIGHT > DY + i / 8 * BLOCK_HEIGHT)
		{
			stage->SetBlockState(i, FALSE);

			//�����瓖�������킯�ł͖���
			if(m_BallPoint.y < DY + i / 8 * BLOCK_HEIGHT + BLOCK_HEIGHT - 16 && m_vector & VECTOR_UP)
			{
				//�����瓖�������킯�ł͖���
				if(m_BallPoint.x + BALL_WIDTH > DX + i % 8 * BLOCK_WIDTH + 16)
				{
					//�ォ�瓖������
					if(m_BallPoint.y + BALL_WIDTH < DY + i / 8 * BLOCK_HEIGHT + 16)
					{
						m_vector ^= VECTOR_DOWN;
						m_vector |= VECTOR_UP;
					}
					//�����瓖������
					else
					{
						m_vector ^= VECTOR_UP;
						m_vector |= VECTOR_DOWN;
					}
				}
				else
				{
					m_vector ^= VECTOR_RIGHT;
					m_vector |= VECTOR_LEFT;
				}

				//�E���瓖�������킯�ł͖���
				if(m_BallPoint.x < DX + i % 8 * BLOCK_WIDTH  + BLOCK_WIDTH - 16)
				{
					//�ォ�瓖������
					if(m_BallPoint.y + BALL_WIDTH < DY + i / 8 * BLOCK_HEIGHT + 16)
					{
						m_vector ^= VECTOR_DOWN;
						m_vector |= VECTOR_UP;
					}
					//�����瓖������
					else
					{
						m_vector ^= VECTOR_UP;
						m_vector |= VECTOR_DOWN;
					}
				}
				else
				{
					m_vector ^= VECTOR_LEFT;
					m_vector |= VECTOR_RIGHT;
				}					
			}
			else
			{
				//�㉺�����𔽓]
				m_vector ^= VECTOR_UP;
				m_vector |= VECTOR_DOWN;
			}
	
			//�ォ�瓖�������킯�ł͖���
			if(m_BallPoint.y + BALL_HEIGHT > DY + i / 8 * BLOCK_HEIGHT + 16 && m_vector & VECTOR_DOWN)
			{
				//�����瓖�������킯�ł͖���
				if(m_BallPoint.x + BALL_WIDTH > DX + i % 8 * BLOCK_WIDTH + 16)
				{
					//�ォ�瓖������
					if(m_BallPoint.y + BALL_WIDTH < DY + i / 8 * BLOCK_HEIGHT + 16)
					{
						m_vector ^= VECTOR_DOWN;
						m_vector |= VECTOR_UP;
					}
					//�����瓖������
					else
					{
						m_vector ^= VECTOR_UP;
						m_vector |= VECTOR_DOWN;
					}
				}
				else
				{
					m_vector ^= VECTOR_RIGHT;
					m_vector |= VECTOR_LEFT;
				}

				//�E���瓖�������킯�ł͖���
				if(m_BallPoint.x < DX + i % 8 * BLOCK_WIDTH  + BLOCK_WIDTH - 16)
				{
					//�ォ�瓖������
					if(m_BallPoint.y + BALL_WIDTH < DY + i / 8 * BLOCK_HEIGHT + 16)
					{
						m_vector ^= VECTOR_DOWN;
						m_vector |= VECTOR_UP;
					}
					//�����瓖������
					else
					{
						m_vector ^= VECTOR_UP;
						m_vector |= VECTOR_DOWN;
					}
				}
				else
				{
					m_vector ^= VECTOR_LEFT;
					m_vector |= VECTOR_RIGHT;
				}					
			
			}
			else
			{
				//�㉺�����𔽓]
				m_vector ^= VECTOR_DOWN;
				m_vector |= VECTOR_UP;
			}

			isCollision = TRUE;
			break;
		}
	}

	return isCollision;
}