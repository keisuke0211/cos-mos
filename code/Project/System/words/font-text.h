//========================================
// 
// �e�L�X�g�\��
// 
//========================================
// *** text.h *** www
//========================================
#pragma once

#include "object\font-object.h"
#include "words.h"

//****************************************
// �\����
//****************************************

// �e�L�X�g���(�����p)
struct FormFont
{
	D3DXCOLOR col;       // �����̐F
	float fTextSize;     // �����̃T�C�Y(�����l 20)
	int nAppearTime;     // 2�����ڈȍ~���\�������܂ł̎���(�����l 1)
	int nStandTime;      // �ҋ@����(�����l 10)
	int nEraseTime;      // ������܂ł̎���(�����l 1) �� 0 �͏����Ȃ�
};

// �e���(�����p)
struct FormShadow
{
	D3DXCOLOR col;			// �e�̐F
	bool bShadow;			// �e�t���O
	D3DXVECTOR3 AddPos;		// �����̈ʒu���炸�炷�l (�����l 0,0,0)	(���̕��� + AddPos)
	D3DXVECTOR2 AddSize;	// �����̃T�C�Y�̉��Z�l (�����l 0,0)		(���̕��� + AddSize)
};

//****************************************
// �O���錾
//****************************************
class CFont;

//****************************************
// �N���X
//****************************************
class CFontText : public CFontObject
{
public:

	static const int BOX_SIZE = 60;		// �P�����������TxtSize�T�C�Y

	static const int SPACE = 0;			// ��
	static const int SPACE_X = 1;		// ���������̊J�n�ʒu�iX�j

	// ***** �񋓌^ *****

	// �e�L�X�g�{�b�N�X�̎��
	enum Box
	{
		BOX_MAINE_MENU = 0,		// ���C�����j���[
		BOX_SUB_TITLE,			// �T�u���j���[�̃^�C�g��
		BOX_SUB_MENU00,			// �T�u���j���[00
		BOX_SUB_SELECTU00,		// �T�u���j���[00�̑I����
		BOX_SUB_MENU01,			// �T�u���j���[00
		BOX_SUB_SELECTU01,		// �T�u���j���[00�̑I����
		BOX_NONE,				// ����
		BOX_MAX
	};

	// ***** �֐� *****
	CFontText(int nPriority = PRIO_TEXT);
	~CFontText();

	// -- ���C�� ------------------------------------------
	/* ������	*/HRESULT Init(void);
	/* �I��		*/void Uninit(void);
	/* �X�V		*/void Update(void);
	/* �`��		*/void Draw(void);

	//--------------------------------------------------
	// �e�L�X�g����
	// ����1  : Box type           / �e�L�X�g�{�b�N�X�̉摜
	// ����2  : Pos3D pos          / �e�L�X�g�{�b�N�X�̈ʒu
	// ����3  : Pos2D size         / �e�L�X�g�{�b�N�X�̃T�C�Y
	// ����4  : const char *Text   / �e�L�X�g
	// ����5  : CFont::FONT Type   / �t�H���g���
	// ����6  : FormFont *pFont    / �t�H���g�֘A�̏��(�F�E���ԂȂ�) �����Ă����v
	// ����7  : bool bBoxSize      / ���b�Z�[�W�{�b�N�X�̃T�C�Y�����̖����E�L���i�����l false�j
	// ����8  : bool bTextBox      / ���b�Z�[�W�{�b�N�X�̕\���E��\�� (�����l true)
	// ����9  : FormShadow *Shadow / �t�H���g�̉e�̏�� �����Ă����v
	//--------------------------------------------------
	static CFontText *CFontText::Create(Box type, Pos3D pos, Pos2D size, const char *Text, CFont::FONT FontType, FormFont *pFont = NULL, bool bBoxSize = false, bool bTextBox = true, FormShadow *Shadow = NULL);

	/* �폜 */void Disap(bool bDisap,int nTime);

	// -- �ݒ� ------------------------------------------
	/* �������̕������ꊇ��	*/void SetTexrSkip(bool skip) { m_Info.bLetterSkip = skip; }//www
	/* �ړ���				*/void SetMove(Pos3D move);
	/* �󔒕\��				*/void SetSpace(bool bSpace) { m_Info.bSpace = bSpace; }
	/* �|�[�Y���̐���		*/void SetTxtPause(bool bPause);
	/* TextBox�̕\��		*/void SetTxtBox(bool bTextBox) { m_Info.bTextBok = bTextBox; }
	/* TextBox�̈ʒu		*/void SetTxtBoxPos(CFloat &X, CFloat &Y, bool bMoveWordsX = false, bool bMoveWordsY = false);
	/* TextBox�̈ʒu		*/void SetTxtBoxPos(const Pos2D &pos,     bool bMoveWordsX = false, bool bMoveWordsY = false) { SetTxtBoxPos(pos.x, pos.y, bMoveWordsX, bMoveWordsY); }
	/* TextBox�̈ʒu		*/void SetTxtBoxPos(const Pos3D &pos,     bool bMoveWordsX = false, bool bMoveWordsY = false) { SetTxtBoxPos(pos.x, pos.y, bMoveWordsX, bMoveWordsY); }
	// bSetAll : ���ɔz�u���Ă��镶���ʒu���ݒ肷�邩  true�Ȃ�SetWordPos�֐����g�p����
	/* TextBox�̃T�C�Y		*/void SetTxtBoxSize(float width, float height) { m_Info.TxtBoxSize = Pos2D(width, height); }
	/* TextBox�̖ڕW�T�C�Y	*/void SetTxtBoxTgtSize(float width, float height) { m_Info.TxtBoxTgtSize = Pos2D(width, height); }
	/* TextBox�̃e�N�X�`��	*/void SetTxtBoxTex(const char* Path = NULL, int PthIdx = -1, int PthX = 1, int PthY = 1);
	/* TextBox�̃p�^�[���ԍ�*/void SetTxtBoxPthIdx(int PthIdx);
	/* TextBox�̐F			*/void SetTxtBoxColor(Color col);
	/* TextBox�̎��		*/void SetTxtBoxType(Box type);
	/* �e�L�X�g�̐F			*/bool SetTxtColor(D3DXCOLOR col);
	/* �e�L�X�g�̃T�C�Y		*/bool SetTxtSize(float size);
	/* �o������				*/void SetAppearTime(CInt nAppearTime) { m_Info.nAppearTime = nAppearTime; }
	/* �����ύX(�P��)		*/bool ChgWords(char* Text, int nIdx, D3DXCOLOR col);
	/* �����ύX(�S��)		*/bool ChgHalfSizeText(char* Text, D3DXCOLOR col);// �� ���̃e�L�X�g��葽���Ǝg���Ȃ� �܂����p�p���̂�
	/* �e�L�X�g�̍Đ�		*/void Regeneration(const char *Text, CFont::FONT FontType, FormFont *pFont = NULL, FormShadow *Shadow = NULL);


	// -- �擾 ------------------------------------------
	/* �������\�������� */int GetPopCount() { return m_Info.nLetterPopCount; }
	/* TxtBot��Tex�ԍ�	*/Box GetTxtBoxType() { return m_Info.Tex.type; }
	/* �ʒu				*/Pos2D GetTxtBoxPos() { return m_Info.TxtBoxPos; }
	/* Txt�T�C�Y		*/Pos2D GetTxtBoxSize() { return m_Info.TxtBoxSize; }
	/* Txt�ڕW�T�C�Y	*/Pos2D GetTxtBoxTgtSize() { return m_Info.TxtBoxTgtSize; }
	/* �����T�C�Y		*/float GetTxtSize() { return m_Info.fTextSize; }
	/* Text�̐����t���O	*/bool GetLetter() { return m_Info.bLetterEnd; }

private:

	// ***** �\���� *****

	// �e�N�X�`�����
	struct Texture {
		Box type;   // TxtBox�̎��
		int Idx;    // �e�N�X�`���ԍ�
		int PtnIdx; // �p�^�[���ԍ�
		int PtnX;   // X�̕�����
		int PtnY;   // Y�̕�����
	};

	// �e
	struct Shadow
	{
		D3DXCOLOR col;   // �e�̐F
		bool bShadow;    // �e�t���O
		CWords** shadow; // ����(�e)
		Pos3D AddPos;    // �����̈ʒu���炸�炷�l (�����l 0,0,0) (���̕��� + AddPos)
		Pos2D AddSize;   // �����̃T�C�Y�̉��Z�l   (�����l 0,0)   (���̕��� + AddSize)
	};

	// �e�L�X�g���
	struct Info
	{
		Color TxtBoxCol;			// �e�L�X�g�{�b�N�X�̐F
		D3DXCOLOR FontCol;			// �����̐F
		D3DXCOLOR TxtBoxColOld;		// �O��̃e�L�X�g�{�b�N�X�̐F
		D3DXCOLOR FontColOld;		// �O��̕����̐F
		bool bCol;					// �F�ύX�t���O(�|�[�Y�ؑ�)
		D3DXVECTOR2 TxtBoxPos;		// �ʒu
		D3DXVECTOR2 TxtBoxMove;		// �ړ���
		D3DXVECTOR2 TxtBoxSize;		// �T�C�Y
		D3DXVECTOR2 TxtBoxTgtSize;	// �ڕW�̃T�C�Y

		Texture Tex;			// �e�N�X�`�����

		float fTextSize;		// �����̃T�C�Y
		int nTextLength;		// �����̒���
		int nAppearTime;		// 1�̕��������Z�����܂ł̎���
		int nAddCount;			// ���������Z�����܂ł̃J�E���g
		int nAddLetter;			// �������ړ���邩�w��
		int nLetterPopCount;	// �������o�����ő吔
		int nLetterPopCountX;	// ���ɕ������o������
		int nNiCount;			// ���s�̐�
		bool bLetterSkip;		// �������Ȃ�c��̕������ꊇ��������B
		bool bLetterEnd;		// �����̐����I��������

		int nStandTime;			// �ҋ@����

		int nDisapTime;			// �����鎞��
		int nDisapTimeMax;		// ������ő厞��
		bool bRelease;			// �����t���O

		Shadow aShadow;			// �e

		bool bTextBok;			// �e�L�X�g�{�b�N�X�̕\���t���O wwwwwwwwww
		bool bPause;			// �|�[�Y���ł��������ifalse�F�����Ȃ��j
		bool bSpace;			// �󔒕\���t���O(sprintf�p)
		string sText;			// �\������e�L�X�g
		string sALLText;		// �e�L�X�g�̑S��
		CWords** words;			// ����
		CFont::FONT FontType;	// �t�H���g���
	};

	// ***** �֐� *****
	/* ��������	*/void LetterForm(void);
	/* �ҋ@		*/void StandTime(void);
	/* �폜		*/void DisapTime(void);
	/* ��������	*/bool CheckLeadByte(int nLetter);

	// -- �ݒ� ------------------------------------------
	/* �e�L�X�g �T�C�Y	*/void SetTextSize(float TextSize);
	/* �ҋ@����			*/void SetStandTime(int StandTime);
	/* �����̏�������	*/void EraseTime(int time);
	/* �����̏o������	*/void TextLetter(const char *Text, int nAppearTime);
	/* �z�u���������ʒu */void SetWordPos(bool bMoveWordsX, bool bMoveWordsY);

	// ***** �ϐ� *****
	Info m_Info;
};