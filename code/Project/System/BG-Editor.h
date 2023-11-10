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

	//�v���C���[���
	struct INFO
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXVECTOR3 spin;
		Color		col;
		char		ModelPath[128];
		WORLD_SIDE  side;
	};

	// *** �֐��錾 ***
	CBGEditor();
	~CBGEditor();

	static void Load(const char* filepath);	//�w�i�Ǎ�����

private:

	static INFO m_Info;
};
