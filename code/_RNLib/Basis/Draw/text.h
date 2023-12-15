//========================================
// 
// �e�L�X�g�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../RNmain.h"

//****************************************
// �N���X��`
//****************************************
// �e�L�X�g�N���X
class CText {
public:
	//========== [[[ �񋓌^��` ]]]
	enum class ALIGNMENT { CENTER, LEFT, RIGHT, MAX, };

	//========== [[[ �\���̒�` ]]]
	// �t�H���g�f�[�^
	struct FontData {
		UShort startCode = 0;
		UShort ptnX      = 0;
		UShort ptnY      = 0;
		short  texIdx    = NONEDATA;	
	};

	//========== [[[ �֐��錾 ]]]
	CText                  ();
	~CText                 ();
	void      Init         (void);
	void      Uninit       (void);
	void      Update       (void);
	void      LoadFont     (const char* loadPath);
	FontData& GetFontData  (const short& fontIdx) { return fontIdx == NONEDATA ? m_defaultFontData : m_fontDatas[fontIdx]; }
	int       GetFontNum   (void) { return m_fontNum; }
	float     GetFontWidth (const short& fontIdx);
	float     GetFontHeight(const short& fontIdx);

private:
	//========== [[[ �ϐ��錾 ]]]
	FontData  m_defaultFontData;
	FontData* m_fontDatas;
	UShort    m_fontNum;
};