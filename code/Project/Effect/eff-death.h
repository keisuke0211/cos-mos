#pragma once
//========================================================
// 
// ���S�p�[�e�B�N������
// Author:HIRASAWA SHION
// 
//========================================================
#include "../../_RNLib/RNlib.h"
#include "../collision.h"

// ���S�p�[�e�B�N���N���X
class CEffect_Death : public CStageObject {
public:
	//���
	enum class TYPE {
		NONE = 0,
		BALL, //�{�[��
		INK,  //�C���N
	};

	//�{�[���̃T�C�Y���x��
	enum class BALL_SIZE_LV {
		SMALL = 0, // �T�C�Y��
		MIDIUM,    // �T�C�Y��
		BIG,       // �T�C�Y��(�ő�)
		MAX
	};

	//���ȏ��
	struct Info
	{
		//�ݒ菈���ɂ���Čʂɐݒ肳��鍀��
		Vector3D move   = INITVECTOR3D; // �ړ���
		Vector3D spin   = INITVECTOR3D; // ��]��
		float    size   = 0.0f;         // �T�C�Y�i���a�j
		int      nLife  = NONEDATA;     // �����i�s���g�Ȃ�NONEDATA
		int      nIdx = NONEDATA;       // �e�N�X�`�� or ���f���ԍ�
		TYPE     type   = TYPE::NONE;   // ���

		//�{�[���p
		BALL_SIZE_LV *pLv = NULL; // �T�C�Y���x��
		int nBallID = NONEDATA;   // �{�[���ԍ�

		//�ݒ菈����ʂ�΁A�F���ʂő������鍀��
		int  ColliderInterval; // ����̓����蔻��𓭂����Ȃ��C���^�[�o���i�S�Ă̓����蔻��ɑ΂��Ăł͂Ȃ�
		bool bDeath;           // ���S�t���O
	};

	CEffect_Death();
	~CEffect_Death();

	void Uninit(void) { ; }
	void Update(void);

	//******************************
	//�ݒ菈���i���ȏ����ꊇ�ݒ�j
	//******************************
	void SetInfo(const Vector3D pos, const Vector3D posOld, const Vector3D move, const Vector3D rot, const Vector3D spin, const float size, const Color color, const int nLife, const int nIdx, const TYPE type);

	//******************************
	//�ݒ菈���i���ȏ����ʐݒ�j
	//******************************
	void SetMove(const Vector3D move)     { m_Info.move = move; }
	void SetSpin(const Vector3D spin)     { m_Info.spin = spin; }
	void SetSize(const float size)        { m_Info.size = size; }
	void SetLife(const int nLife)         { m_Info.nLife = nLife; }
	void SetIdx(const int nIdx)           { m_Info.nIdx = nIdx; }
	void SetType(const TYPE type)         { m_Info.type = type; }

	//******************************
	//�{�[���p�ݒ菈��
	//******************************
	void SetBallSize(BALL_SIZE_LV Lv);

private:
	static const int CREATE_INTERVAL = 30;  //�����C���^�[�o��
	static const int FUSION_INTERVAL = 120; //�Z���C���^�[�o��

	static const float CREATE_SPREAD_POWER; //�������̊g�U��
	static const float PLAYER_COLLI_POWER;  //�v���C���[�ɓ��������Ƃ��̐�����ї�
	static const float MOVE_X_CORRECT;      //�]�x�N�g���̈ړ��␳�W��
	static const float GRAVITY_POWER;       //�d�͉����x
	static const float BOUND_POWER;         //�o�E���h�W��
	static const short BALL_ALPHA_DECREASE; //�{�[���̃��l�����ʁi�����蔻���Unknown���o���ۂɎg�p
	static const float BALL_SIZE[(int)BALL_SIZE_LV::MAX];//�{�[���T�C�Y���i�[

	//��ʍX�V����
	void UpdateType_Ball(void);
	void UpdateType_Ink(void);

	//�{�[�����̏���
	void BallFusion(void);

	//���X�V����
	CCollision::ROT PlayerCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo, CPlayer::VECTOL vec);
	CCollision::ROT StgObjCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo, CPlayer::VECTOL vec, CStageObject::TYPE& type);
	void SetSelfInfo(CCollision::SelfInfo *pSelfInfo);
	void Move(CPlayer::VECTOL vec);
	void Spin(void);
	void PutPolygon(void);
	void PutModel(void);
	void Life(void);
	void Death(void);

	static int s_nNumAllBall;
	Info m_Info;
};