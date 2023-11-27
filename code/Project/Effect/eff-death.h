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
	//���
	enum class TYPE {
		NONE = 0,
		BALL, //�{�[��
		INK,  //�C���N
	};

	//���ȏ��
	struct Info
	{
		Vector3D pos    = INITVECTOR3D; // �ʒu
		Vector3D posOld = INITVECTOR3D; // �O��ʒu
		Vector3D move   = INITVECTOR3D; // �ړ���
		Vector3D rot    = INITVECTOR3D; // ����
		Vector3D spin   = INITVECTOR3D; // ��]��
		Vector2D size   = INITVECTOR2D; // �T�C�Y�i���E�����j
		Color    color  = INITCOLOR;    // �F
		int      *pLife = NULL;         // �����i�s���g�Ȃ�NULL
		int      nTex   = NONEDATA;     // �e�N�X�`���ԍ�
		TYPE     type   = TYPE::NONE;   // ���
	};

	CEffect_Death();
	~CEffect_Death();

	void Update(void);

	//******************************
	//�ݒ菈���i���ȏ����ꊇ�ݒ�j
	//******************************
	void SetInfo(const Vector3D pos, const Vector3D posOld, const Vector3D move, const Vector3D rot, const Vector3D spin, const Vector2D size, const Color color, const int *pLife, const int nTex, const TYPE type);

	//******************************
	//�ݒ菈���i���ȏ����ʐݒ�j
	//******************************
	void SetPos(const Vector3D pos)       { m_Info.pos = pos; }
	void SetPosOld(const Vector3D posOld) { m_Info.posOld = posOld; }
	void SetMove(const Vector3D move)     { m_Info.move = move; }
	void SetRot(const Vector3D rot)       { m_Info.rot = rot; }
	void SetSpin(const Vector3D spin)     { m_Info.spin = spin; }
	void SetSize(const Vector2D size)     { m_Info.size = size; }
	void SetColor(const Color color)      { m_Info.color = color; }
	void SetLife(const int *pLife)        { *m_Info.pLife = *pLife; }
	void SetTex(const int nTex)           { m_Info.nTex = nTex; }
	void SetType(const TYPE type)         { m_Info.type = type; }

private:
	//��ʍX�V����
	void UpdateType_Ball(void);
	void UpdateType_Ink(void);

	//���X�V����
	CCollision::ROT PlayerCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo);
	CCollision::ROT StgObjCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo);
	void Move(void);
	void Spin(void);
	void Life(void);

	Info m_Info;
};