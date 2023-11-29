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
class CEffect_Death : public CObject {
public:
	static const int CREATE_COUNTER = 30;

	//���
	enum class TYPE {
		NONE = 0,
		BALL, //�{�[��
		INK,  //�C���N
	};

	//���ȏ��
	struct Info
	{
		//�ݒ菈���ɂ���Čʂɐݒ肳��鍀��
		Vector3D pos    = INITVECTOR3D; // �ʒu
		Vector3D posOld = INITVECTOR3D; // �O��ʒu
		Vector3D move   = INITVECTOR3D; // �ړ���
		Vector3D rot    = INITVECTOR3D; // ����
		Vector3D spin   = INITVECTOR3D; // ��]��
		float    size   = 0.0f;         // �T�C�Y�i���a�j
		Color    color  = INITCOLOR;    // �F
		int      nLife  = NONEDATA;     // �����i�s���g�Ȃ�NONEDATA
		int      nIdx = NONEDATA;       // �e�N�X�`�� or ���f���ԍ�
		TYPE     type   = TYPE::NONE;   // ���

		//�ݒ菈����ʂ�΁A�F���ʂő������鍀��
		int  CreateCounter; // ��������Ă���̃J�E���^�[(���܂�Ă����̓v���C���[�Ƃ̓����蔻��𓭂����Ȃ�����
		bool bDeath;        // ���S�t���O
	};

	CEffect_Death();
	~CEffect_Death();

	void Update(void);

	//******************************
	//�ݒ菈���i���ȏ����ꊇ�ݒ�j
	//******************************
	void SetInfo(const Vector3D pos, const Vector3D posOld, const Vector3D move, const Vector3D rot, const Vector3D spin, const float size, const Color color, const int nLife, const int nIdx, const TYPE type);

	//******************************
	//�ݒ菈���i���ȏ����ʐݒ�j
	//******************************
	void SetPos(const Vector3D pos)       { m_Info.pos = pos; }
	void SetPosOld(const Vector3D posOld) { m_Info.posOld = posOld; }
	void SetMove(const Vector3D move)     { m_Info.move = move; }
	void SetRot(const Vector3D rot)       { m_Info.rot = rot; }
	void SetSpin(const Vector3D spin)     { m_Info.spin = spin; }
	void SetSize(const float size)        { m_Info.size = size; }
	void SetColor(const Color color)      { m_Info.color = color; }
	void SetLife(const int nLife)         { m_Info.nLife = nLife; }
	void SetIdx(const int nIdx)           { m_Info.nIdx = nIdx; }
	void SetType(const TYPE type)         { m_Info.type = type; }

private:
	static const float CREATE_SPREAD_POWER; //�������̊g�U��
	static const float PLAYER_COLLI_POWER;  //�v���C���[�ɓ��������Ƃ��̐�����ї�
	static const float MOVE_X_CORRECT;      //�]�x�N�g���̈ړ��␳�W��
	static const float GRAVITY_POWER;       //�d�͉����x
	static const float BOUND_POWER;         //�o�E���h�W��
	static const short BALL_ALPHA_DECREASE; //�{�[���̃��l�����ʁi�����蔻���Unknown���o���ۂɎg�p

	//��ʍX�V����
	void UpdateType_Ball(void);
	void UpdateType_Ink(void);

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

	Info m_Info;
};