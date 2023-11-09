//========================================
// 
// �e�L�X�g�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// [[[ text.h ]]]
//========================================
#ifndef _TEXT_H_
#define _TEXT_H_

#include "../../RNmain.h"

//****************************************
// �N���X��`
//****************************************
// �e�L�X�g�N���X
class CText {
public:
	//========== [[[ �񋓌^��` ]]]
	// �\���`��
	enum class ALIGNMENT {
		CENTER,	// ��������
		LEFT,	// ������
		RIGHT,	// �E����
		MAX,
	};

	//========== [[[ �\���̒�` ]]]
	// �t�H���g���̏��\����
	typedef struct Font_ {
		int   nStartCode = 0;			// �J�n����
		int   nPtnWidth  = 0;			// �p�^�[����
		int   nPtnHeight = 0;			// �p�^�[������
		float fSpaceRate = 0.0f;		// �Ԋu�䗦
		int   nTexIdx    = NONEDATA;	// �e�N�X�`���ԍ�
	}FontData;

	//========== [[[ �֐��錾 ]]]
	CText();
	~CText();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void LoadFont(void);
	bool CheckFontExists(int nFont) {
		if (nFont < 0 || nFont >= m_nFontNum) {
			return false;
		}
		return true;
	}
	FontData GetFont(const int nFont) {
		if (nFont < 0 || nFont >= m_nFontNum) {
			FontData defFont = {};
			return defFont;
		}
		return m_pFont[nFont];
	};
	int GetFontNum(void) { return m_nFontNum; }
	float GetFontWidth   (int nFont);
	float GetFontWidth2D (int nFont);
	float GetFontHeight  (int nFont);
	float GetFontHeight2D(int nFont);

	//========== [[[ �ϐ��錾 ]]]
	FontData* m_pFont;	// �t�H���g���̃|�C���^
	int m_nFontNum;	// �t�H���g��
};

#endif