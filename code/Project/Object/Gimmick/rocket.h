//========================================
// 
// ���P�b�g
// Author:KEISUKE OTONO
// 
//========================================
// *** rocket.h ***
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../stage-object.h"

//****************************************
// �N���X
//****************************************
class CRocket : public CStageObject {
public:
	//========== [[[ �񋓌^��` ]]]
	enum class ANIME_STATE {
		NONE = -1,// �����Ȃ�
		RIDE,	  // ���
		FLY,	  // ���
		MAX,
	};
	// ���ʏ��
	struct TexInfo
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 move;		// �ړ���
		D3DXVECTOR3 rot;		// ����
		Color col;				// ���_�J���[
		int TexIdx;				// �e�N�X�`���ԍ�
	};
	struct Info
	{
		TexInfo Firetex;	// ���e�N�X�`�����
		TexInfo Smoketex;	// ���e�N�X�`�����
		D3DXVECTOR3 move;		// �ړ���
		D3DXCOLOR col;			// ���_�J���[
		ANIME_STATE Animstate;	// �A�j���[�V�����̏��
		Scale3D scale;			// �X�P�[��
		Scale3D Maxscale;		// �ő�X�P�[��
		float SmallSpeed;		// �������Ȃ鑬�x
		float fScaleMag;		// �X�P�[���̔{��
		short nModelIdx;		// ���f���ԍ�
		int	nRideAnimeCounter;	// ���C�h�A�j���[�V�����J�E���^�[
		int	nFlyAnimeCounter;	// ��s�A�j���[�V�����J�E���^�[
		int nEffectAnimCounter;	// �G�t�F�N�g�A�j���[�V�����J�E���g
		bool bEffect;			// �G�t�F�N�g�̔���
	};
	
	CRocket();
	~CRocket();

	/* ������	*/HRESULT Init(void);
	/* �I��		*/void Uninit(void);
	/* �X�V		*/void Update(void);
	/* ����Ԃ̍X�V*/void UpdateState_Ride(void);
	/* ��ԏ�Ԃ̍X�V*/void UpdateState_Fly(void);

	// -- �ݒ� ---------------------------------------------
	/* �F			*/void SetColor(const D3DXCOLOR col) { m_Info.col = col; }
	/* ���f���ԍ�	*/void SetModelIdx(const int nIdx) { m_Info.nModelIdx = nIdx; }
	/* �A�j���[�V�����̏�Ԃ̐ݒ�*/void SetState(const ANIME_STATE state) { m_Info.Animstate = state; }
	/* ��鏀������*/	static void ReadyRide(void) { s_bReady = true; }
	/* ����Ă���		*/void Ride(void);
	/*���悵�Ă���v���C���[�����Z�b�g*/static void ResetCounter(void) { s_nCountPlayer = 0; }

	// -- ���� ---------------------------------------------
	/* �F		*/D3DXCOLOR GetColor(void) { return m_Info.col; }
	/*�ʒu		*/D3DXVECTOR3& GetFirePos(void) { return m_Info.Firetex.pos; }
	/*���݂̓���Ґ�*/static int GetCounter(void) { return s_nCountPlayer; }

private:
	// *** �ϐ� ***
	Info m_Info;				// ���ʏ��
	static int  s_nCountPlayer;	// �v���C���[�̃J�E���g
	static bool s_bReady;		// ��鏀������������

	static const int   s_AnimeMax;
	static const int   s_RideAnimeMax;
	static const int   s_RotAnimeMax;
	static const int   s_FadeModeCountMax;
	static const float s_RideAnimeMag;
	static const float s_RideAnimeShrink;
	static const float s_RotAdd;
	static const float s_MoveMag;
	static const float s_MoveAdd;
};