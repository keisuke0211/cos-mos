//========================================
// 
// �w�i�G�f�B�b�g�̓ǂݍ���
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once

#include "../main.h"

//****************************************
// �N���X��`
//****************************************
class CBGEditor {

public:

	enum class WORLD_SIDE {
		NONE = 0,
		UP,			//��
		DOWN,		//��
		MAX,
	};

	// �ړ����
	enum class MOVE_TYPE { NONE, AB, MAX, };
	struct MOVE_AB {
		D3DXVECTOR3 posB;
		int         time;
		int         counter;
		bool        isReturn;
	};

	// ���
	struct INFO {
		D3DXVECTOR3 pos        = INITD3DXVECTOR3;
		D3DXVECTOR3 posAdd     = INITD3DXVECTOR3;
		D3DXVECTOR3 rot        = INITD3DXVECTOR3;
		D3DXVECTOR3 move       = INITD3DXVECTOR3;
		short       life       = NONEDATA;
		short       lifeMax    = NONEDATA;
		D3DXVECTOR3 spin       = INITD3DXVECTOR3;
		D3DXVECTOR3 spinAdd    = INITD3DXVECTOR3;
		Color		col        = INITCOLOR;
		short       modelIdx   = NONEDATA;
		WORLD_SIDE  side       = WORLD_SIDE::NONE;
		void*       moveInfo   = NULL;
		MOVE_TYPE   moveType   = MOVE_TYPE::NONE;
		int         createTime = 0;
	};

	// *** �֐��錾 ***
	CBGEditor();
	~CBGEditor();

	static void Load(const char* filepath);	//�w�i�Ǎ�����

private:

	static INFO m_Info;
};
