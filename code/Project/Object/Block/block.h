//========================================
// 
// �u���b�N����
// Author:KEISUKE OTONO
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../stage-object.h"

//****************************************
// �N���X
//****************************************
class CBlock : public CStageObject {
public:
	//========== [[[ �񋓌^��` ]]]
	// �����ڂ̎��
	enum class LOOKS_TYPE {
		SOIL_BLOCK,
		SOIL_BLOCK_GRASSY,
		ANCIENT_STONE_BLOCK,
		ANCIENT_STONE_BLOCK_PTN_A,
		ANCIENT_STONE_BLOCK_PTN_B,
		ANCIENT_STONE_BLOCK_PTN_C,
		ANCIENT_STONE_PILLAR_UP,
		ANCIENT_STONE_PILLAR_CENTER,
		ANCIENT_STONE_PILLAR_DOWN,
		MAX,
	};

	//========== [[[ �\���̒�` ]]]
	// ���ʏ��
	struct Info {
		LOOKS_TYPE looksType;	// ���
		short      modelIdx;	// ���f���ԍ�
	};

	//========== [[[ �֐��錾 ]]]
	CBlock();
	~CBlock();
	HRESULT Init(LOOKS_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//========== [[[ �萔�錾 ]]]
	static const char* MODEL_PATHS[(int)LOOKS_TYPE::MAX];

	//========== [[[ �ϐ��錾 ]]]
	Info m_Info;

};