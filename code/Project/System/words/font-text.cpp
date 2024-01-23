//========================================
// 
// �e�L�X�g�\��
// 
//========================================
// *** font-text.cpp ***
//========================================
#include "font-text.h"
#include "../../manager.h"

//========================================
// �R���X�g���N�^
//========================================
CFontText::CFontText(int nPriority) : CFontObject(nPriority)
{
	m_Info.TxtBoxPos = INITPOS2D;
	m_Info.TxtBoxCol = COLOR_WHITE;
	m_Info.TxtBoxMove = INITPOS2D;
	m_Info.FontCol = INITD3DCOLOR;
	m_Info.TxtBoxColOld = INITD3DCOLOR;
	m_Info.FontColOld = INITD3DCOLOR;
	m_Info.bCol = false;

	m_Info.fTextSize = 0.0f;
	m_Info.nTextLength = 0;
	m_Info.nAppearTime = 0;
	m_Info.nAddCount = 0;
	m_Info.nAddLetter = 0;
	m_Info.nLetterPopCount = 0;
	m_Info.nLetterPopCountX = 0;
	m_Info.nNiCount = 0;

	m_Info.Tex.type = BOX_NONE;
	m_Info.Tex.Idx = 0;
	m_Info.Tex.PtnIdx = -1;
	m_Info.Tex.PtnX = 1;
	m_Info.Tex.PtnY = 1;

	m_Info.bLetterSkip = false;
	m_Info.bLetterEnd = false;

	m_Info.nStandTime = 0;

	m_Info.nDisapTime = 0;
	m_Info.nDisapTimeMax = 0;
	m_Info.bRelease = false;

	m_Info.bTextBox = true;
	m_Info.bPause = false;
	m_Info.bSpace = false;

	m_Info.aShadow.col = INITD3DCOLOR;
	m_Info.aShadow.AddPos = INITPOS3D;
	m_Info.aShadow.AddSize = INITPOS2D;
	m_Info.aShadow.bShadow = false;

	m_Info.aEdging.col = INITD3DCOLOR;
	m_Info.aEdging.bEdging = false;
	m_Info.aEdging.AddSize = INITPOS2D;
}

//========================================
// �f�X�g���N�^
//========================================
CFontText::~CFontText()
{

}

//================================================================================
//--------------------------------------------------------------------------------
//********************************** ���C������ **********************************
//--------------------------------------------------------------------------------
//================================================================================

//========================================
// ������
//========================================
HRESULT CFontText::Init()
{
	m_Info.TxtBoxCol = COLOR_WHITE;
	m_Info.FontCol = INITD3DCOLOR;
	m_Info.TxtBoxColOld = INITD3DCOLOR;
	m_Info.FontColOld = INITD3DCOLOR;
	m_Info.bCol = false;
	m_Info.fTextSize = 0.0f;
	m_Info.nTextLength = 0;
	m_Info.nAppearTime = 0;
	m_Info.nAddCount = 0;
	m_Info.nAddLetter = 0;
	m_Info.nLetterPopCount = 0;
	m_Info.nLetterPopCountX = 0;
	m_Info.nNiCount = 0;
	m_Info.sText = "";

	SetType(TYPE_FONT);
	
	m_Info.nStandTime = 0;
	m_Info.bLetterEnd = false;

	m_Info.nDisapTime = 0;
	m_Info.nDisapTimeMax = 0;
	m_Info.bRelease = false;
	m_Info.bPause = false;

	return S_OK;
}

//========================================
// �I��
//========================================
void CFontText::Uninit()
{
	// �����̍폜
	for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++)
	{
		if (m_Info.words[wordsCount] != NULL)
		{
			m_Info.words[wordsCount]->Uninit();
		}
	}

	if (m_Info.words != NULL)
	{
		delete[] m_Info.words;
		m_Info.words = NULL;
	}

	// �e
	if (m_Info.aShadow.bShadow)
	{
		for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++)
		{
			if (m_Info.aShadow.shadow[wordsCount] != NULL)
			{
				m_Info.aShadow.shadow[wordsCount]->Uninit();
			}
		}

		if (m_Info.aShadow.shadow != NULL)
		{
			delete[] m_Info.aShadow.shadow;
			m_Info.aShadow.shadow = NULL;
		}
	}

	// �����
	if (m_Info.aEdging.bEdging)
	{
		for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++)
		{
			if (m_Info.aEdging.edging[wordsCount] != NULL)
			{
				m_Info.aEdging.edging[wordsCount]->Uninit();
			}
		}

		if (m_Info.aEdging.edging != NULL)
		{
			delete[] m_Info.aEdging.edging;
			m_Info.aEdging.edging = NULL;
		}
	}

	Release();
}

//========================================
// �X�V
//========================================
void CFontText::Update()
{
	m_Info.TxtBoxPos += m_Info.TxtBoxMove;

	if (m_Info.bTextBox)
	{
		if (m_Info.Tex.PtnIdx >= 0){
			RNLib::Polygon2D().Put(PRIORITY_TEXT, m_Info.TxtBoxPos, 0.0f, false)
				->SetSize(m_Info.TxtBoxSize.x, m_Info.TxtBoxSize.y)
				->SetCol(m_Info.TxtBoxCol)
				->SetTex(m_Info.Tex.Idx, m_Info.Tex.PtnIdx, m_Info.Tex.PtnX, m_Info.Tex.PtnY);
		}
		else {
			RNLib::Polygon2D().Put(PRIORITY_TEXT, m_Info.TxtBoxPos, 0.0f, false)
				->SetSize(m_Info.TxtBoxSize.x, m_Info.TxtBoxSize.y)
				->SetCol(m_Info.TxtBoxCol)
				->SetTex(m_Info.Tex.Idx);
		}
	}

	m_Info.TxtBoxMove = INITPOS2D;

	// �e�L�X�g����
	if (!m_Info.bLetterEnd)
	{
		if (m_Info.nAppearTime <= 0 || m_Info.bLetterSkip)
		{
			while (1)
			{
				LetterForm();

				if (m_Info.bLetterEnd)
					break;
			}
		}
		else
		{
			LetterForm();
		}
	}

	// �ҋ@����
	StandTime();

	// �폜����
	DisapTime();
}

//========================================
// �`��
//========================================
void CFontText::Draw()
{

}

//========================================
// ����
//========================================
CFontText *CFontText::Create(Box type, Pos3D pos, Pos2D size, const char *Text, CFont::FONT FontType, FontFont *pFont, bool bBoxSize, bool bTextBox, FontShadow *Shadow, FontEdging *Edging, CShort SeIdx)
{
	CFontText *pText = new CFontText;

	if (pText != NULL)
	{
		pText->Init();

		// -- ���b�Z�[�W�{�b�N�X ----------------

		// �e�N�X�`���ݒ�
		pText->SetTxtBoxType(type);

		pText->m_Info.TxtBoxPos = Pos2D(pos.x, pos.y);
		pText->m_Info.TxtBoxSize = size;
		pText->m_Info.bTextBox = bTextBox;

		// -- �e�L�X�g -----------------------
		pText->m_Info.FontType = FontType;

		// -- �T�E���h -----------------------
		pText->m_Info.SeIdx = SeIdx;

		if (pFont != NULL)
		{
			pText->m_Info.FontCol = pFont->col;
			pText->SetTextSize(pFont->fTextSize);
			pText->SetStandTime(pFont->nStandTime);
			pText->EraseTime(pFont->nEraseTime);
			pText->TextLetter(Text, pFont->nAppearTime);
		}
		else if (pFont == NULL)
		{
			pText->m_Info.FontCol = INITD3DCOLOR;
			pText->SetTextSize(20.0f);
			pText->SetStandTime(10);
			pText->EraseTime(1);
			pText->TextLetter(Text, 1);
		}


		if (bBoxSize)
		{
			pText->m_Info.TxtBoxSize.x = pText->m_Info.fTextSize * (pText->m_Info.nTextLength * 0.5f + 1);
		}

		// �e
		if (Shadow == NULL)
		{
			pText->m_Info.aShadow.col = INITD3DCOLOR;
			pText->m_Info.aShadow.AddPos = INITPOS3D;
			pText->m_Info.aShadow.AddSize = INITPOS2D;
			pText->m_Info.aShadow.bShadow = false;
		}
		else if (Shadow != NULL)
		{
			if (Shadow->bShadow)
			{
				pText->m_Info.aShadow.shadow = new CWords*[pText->m_Info.nTextLength];

				for (int wordsCount = 0; wordsCount < pText->m_Info.nTextLength; wordsCount++)
				{
					pText->m_Info.aShadow.shadow[wordsCount] = NULL;
				}

				pText->m_Info.aShadow.col = Shadow->col;
				pText->m_Info.aShadow.AddPos = Shadow->AddPos;
				pText->m_Info.aShadow.AddSize = Shadow->AddSize;
				pText->m_Info.aShadow.bShadow = Shadow->bShadow;
			}
		}

		// �����
		if (Edging == NULL)
		{
			pText->m_Info.aEdging.col = INITD3DCOLOR;
			pText->m_Info.aEdging.AddSize = INITPOS2D;
			pText->m_Info.aEdging.bEdging = false;
		}
		else if (Edging != NULL)
		{
			pText->m_Info.aEdging.edging = new CWords*[pText->m_Info.nTextLength];

			for (int wordsCount = 0; wordsCount < pText->m_Info.nTextLength; wordsCount++) {
				pText->m_Info.aEdging.edging[wordsCount] = NULL;
			}

			pText->m_Info.aEdging.col = Edging->col;
			pText->m_Info.aEdging.AddSize = Edging->AddSize;
			pText->m_Info.aEdging.bEdging = Edging->bEdging;
		}
	}

	return pText;
}

//================================================================================
//--------------------------------------------------------------------------------
//********************************** ���̑����� **********************************
//--------------------------------------------------------------------------------
//================================================================================

//========================================
// ��������
//========================================
void CFontText::LetterForm(void)
{
	CFloat& fTxtSize = m_Info.fTextSize;

	if (++m_Info.nAddCount >= m_Info.nAppearTime)
	{// �\�����鎞�Ԃ���������A

		if (m_Info.nAddLetter <= m_Info.nTextLength)
		{// �e�L�X�g�T�C�Y��������Ă���A

			m_Info.sText += m_Info.sALLText[m_Info.nAddLetter];
			string Text = m_Info.sText;

			// �𑜓x�ɍ��킹��
			Pos3D pos = m_Info.TxtBoxPos * 2;

			pos.x = pos.x - m_Info.TxtBoxSize.x;
			pos.z = 0.0f;

			if (Text != "" && m_Info.nAddLetter < m_Info.nTextLength)
			{// �󔒂���Ȃ�������A && �e�L�X�g�T�C�Y��������Ă���A

				if (CheckLeadByte(m_Info.sALLText[m_Info.nAddLetter]))
				{// 1�o�C�g�������V�t�gJIS��������A

					m_Info.nAddLetter++;
					m_Info.sText += m_Info.sALLText[m_Info.nAddLetter];
					m_Info.nAddLetter++;

					// �e
					if (m_Info.aShadow.bShadow)
					{
						Pos3D AddPos = m_Info.aShadow.AddPos;
						Pos2D AddSize = m_Info.aShadow.AddSize;

						m_Info.aShadow.shadow[m_Info.nLetterPopCount] = CWords::Create(m_Info.sText.c_str(),
							Pos3D((pos.x + (SPACE + AddPos.x)) + ((fTxtSize * 2) * (m_Info.nLetterPopCountX + SPACE_X)), (pos.y + AddPos.y) + m_Info.nNiCount * 40.0f, pos.z),
							Pos3D(fTxtSize + AddSize.x, fTxtSize + AddSize.y, 0.0f),
							m_Info.FontType, m_Info.aShadow.col);

						m_Info.aShadow.shadow[m_Info.nLetterPopCount]->SetLetterPop(m_Info.nLetterPopCountX + SPACE_X);
					}

					m_Info.words[m_Info.nLetterPopCount] = CWords::Create(m_Info.sText.c_str(),
						Pos3D((pos.x + SPACE) + ((fTxtSize * 2) * (m_Info.nLetterPopCountX + SPACE_X)), pos.y + m_Info.nNiCount*40.0f, pos.z),
						Pos3D(fTxtSize, fTxtSize, 0.0f),
						m_Info.FontType, m_Info.FontCol);

					m_Info.words[m_Info.nLetterPopCount]->SetLetterPop(m_Info.nLetterPopCountX + SPACE_X);

					// �����
					if (m_Info.FontType == CFont::FONT::FONT_WAKUWAKU && m_Info.aEdging.bEdging)
					{
						Pos2D AddSize = m_Info.aEdging.AddSize;

						m_Info.aEdging.edging[m_Info.nLetterPopCount] = CWords::Create(m_Info.sText.c_str(),
							Pos3D((pos.x + SPACE) + ((fTxtSize * 2) * (m_Info.nLetterPopCountX + SPACE_X)), pos.y + m_Info.nNiCount * 40.0f, pos.z),
							Pos3D(fTxtSize + AddSize.x, fTxtSize + AddSize.y, 0.0f),
							CFont::FONT::FONT_CONVENIE, m_Info.aEdging.col);

						m_Info.aEdging.edging[m_Info.nLetterPopCount]->SetLetterPop(m_Info.nLetterPopCountX + SPACE_X);
					}

					m_Info.nLetterPopCount++;
					m_Info.nLetterPopCountX++;
				}
				else
				{
					if (m_Info.sText != "\n")
					{// �I�[�����Ȃ�A

						m_Info.nAddLetter++;

						if (m_Info.bSpace && m_Info.sText == " ")
						{
							m_Info.nLetterPopCount++;
							m_Info.nLetterPopCountX++;
							m_Info.sText = "";
							m_Info.nAddCount = 0;
							return;
						}

						// �e
						if (m_Info.aShadow.bShadow)
						{
							Pos3D AddPos = m_Info.aShadow.AddPos;
							Pos2D AddSize = m_Info.aShadow.AddSize;

							m_Info.aShadow.shadow[m_Info.nLetterPopCount] = CWords::Create(m_Info.sText.c_str(),
								Pos3D((pos.x + (SPACE + AddPos.x)) + ((fTxtSize * 2) * (m_Info.nLetterPopCountX + SPACE_X)), (pos.y + AddPos.y) + m_Info.nNiCount * 40.0f, pos.z),
								Pos3D(fTxtSize + AddSize.x, fTxtSize + AddSize.y, 0.0f),
								m_Info.FontType, m_Info.aShadow.col);
							m_Info.aShadow.shadow[m_Info.nLetterPopCount]->SetLetterPop(m_Info.nLetterPopCountX + SPACE_X);
						}

						m_Info.words[m_Info.nLetterPopCount] = CWords::Create(m_Info.sText.c_str(),
							Pos3D((pos.x + SPACE) + ((fTxtSize * 2) * (m_Info.nLetterPopCountX + SPACE_X)), pos.y + m_Info.nNiCount*40.0f, pos.z),
							Pos3D(fTxtSize, fTxtSize, 0.0f),
							m_Info.FontType, m_Info.FontCol);

						m_Info.words[m_Info.nLetterPopCount]->SetLetterPop(m_Info.nLetterPopCountX + SPACE_X);

						// �����
						if (m_Info.FontType == CFont::FONT::FONT_WAKUWAKU && m_Info.aEdging.bEdging)
						{
							Pos2D AddSize = m_Info.aEdging.AddSize;

							m_Info.aEdging.edging[m_Info.nLetterPopCount] = CWords::Create(m_Info.sText.c_str(),
								Pos3D((pos.x + SPACE) + ((fTxtSize * 2) * (m_Info.nLetterPopCountX + SPACE_X)), pos.y + m_Info.nNiCount * 40.0f, pos.z),
								Pos3D(fTxtSize + AddSize.x, fTxtSize + AddSize.y, 0.0f),
								CFont::FONT::FONT_CONVENIE, m_Info.aEdging.col);

							m_Info.aEdging.edging[m_Info.nLetterPopCount]->SetLetterPop(m_Info.nLetterPopCountX + SPACE_X);
						}

						m_Info.nLetterPopCount++;
						m_Info.nLetterPopCountX++;
					}
					else
					{
						m_Info.nLetterPopCountX = 0;
						m_Info.nAddLetter++;
						m_Info.nNiCount++;
					}
				}

				// �T�E���h�Đ�
				RNLib::Sound().Play(m_Info.SeIdx, _RNC_Sound::CATEGORY::SE, 0.5f, false);
			}
			else
			{
				m_Info.bLetterEnd = true;
			}
		}

		m_Info.sText = "";
		m_Info.nAddCount = 0;
	}
}

//========================================
// �ҋ@
//========================================
void CFontText::StandTime(void)
{
	if (m_Info.bLetterEnd && m_Info.nStandTime >= 1)
	{
		if (--m_Info.nStandTime <= 0)
		{
			if (m_Info.nDisapTimeMax >= 1)
			{
				m_Info.bRelease = true;
			}
		}
	}
}

//========================================
// �폜
//========================================
void CFontText::DisapTime(void)
{
	if (m_Info.bRelease)
	{
		if (--m_Info.nDisapTime <= 0)
		{
			Uninit();
			return;
		}

		// �F�̐���
		m_Info.TxtBoxCol.a *= ((float)m_Info.nDisapTime / m_Info.nDisapTimeMax);
		m_Info.FontCol.a *= ((float)m_Info.nDisapTime / m_Info.nDisapTimeMax);

		// �����F�̐���
		for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++)
		{
			if (m_Info.words[wordsCount] != NULL)
			{
				m_Info.words[wordsCount]->SetColor(m_Info.FontCol);
			}
		}

		if (m_Info.aShadow.bShadow)
		{
			m_Info.aShadow.col.a *= ((float)m_Info.nDisapTime / m_Info.nDisapTimeMax);

			for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++)
			{
				if (m_Info.aShadow.shadow[wordsCount] != NULL)
				{
					m_Info.aShadow.shadow[wordsCount]->SetColor(m_Info.aShadow.col);
				}
			}
		}
	}
}

//================================================================================
//--------------------------------------------------------------------------------
//*********************************** �ݒ菈�� ***********************************
//--------------------------------------------------------------------------------
//================================================================================

//========================================
// �ڕW�ʒu(�P��)
//========================================
void CFontText::SetTargetPos(D3DXVECTOR3 pos, int nWord, int Time)
{
	// �ʏ핶��
	if (m_Info.words[nWord] != NULL)
	{
		bool bMove = m_Info.words[nWord]->IsMove();

		if (!bMove)
			m_Info.words[nWord]->SetTargetPos(pos, Time);
	}

	// �e
	if (m_Info.aShadow.bShadow)
	{
		if (m_Info.aShadow.shadow[nWord] != NULL)
		{
			bool bMove = m_Info.aShadow.shadow[nWord]->IsMove();

			if (!bMove)
				m_Info.aShadow.shadow[nWord]->SetTargetPos(pos, Time);
		}
	}

	// �����
	if (m_Info.aEdging.bEdging)
	{
		if (m_Info.aEdging.edging[nWord] != NULL)
		{
			bool bMove = m_Info.aEdging.edging[nWord]->IsMove();

			if (!bMove)
				m_Info.aEdging.edging[nWord]->SetTargetPos(pos, Time);
		}
	}
}

//========================================
// �ڕW�ʒu(�S��)
//========================================
void CFontText::SetAllTargetPos(D3DXVECTOR3 pos, int Time)
{
	for (int nWords = 0; nWords < m_Info.nLetterPopCount; nWords++)
	{
		// �ʏ핶��
		if (m_Info.words[nWords] != NULL)
		{
			bool bMove = m_Info.words[nWords]->IsMove();

			if (!bMove)
				m_Info.words[nWords]->SetTargetPos(pos,Time);
		}

		// �e
		if (m_Info.aShadow.bShadow)
		{
			if (m_Info.aShadow.shadow[nWords] != NULL)
			{
				bool bMove = m_Info.aShadow.shadow[nWords]->IsMove();

				if (!bMove)
					m_Info.aShadow.shadow[nWords]->SetTargetPos(pos, Time);
			}
		}

		// �����
		if (m_Info.aEdging.bEdging)
		{
			if (m_Info.aEdging.edging[nWords] != NULL)
			{
				bool bMove = m_Info.aEdging.edging[nWords]->IsMove();

				if (!bMove)
					m_Info.aEdging.edging[nWords]->SetTargetPos(pos, Time);
			}
		}
	}
}

//========================================
// �ړ���
//========================================
void CFontText::SetMove(Pos3D move)
{
	m_Info.TxtBoxMove = Pos2D(move.x, move.y);

	for (int nWords = 0; nWords < m_Info.nLetterPopCount; nWords++)
	{
		if (m_Info.words[nWords] != NULL)
		{
			m_Info.words[nWords]->SetMove(move);
		}
	}
}

//========================================
// �����T�C�Y
//========================================
void CFontText::SetTextSize(float TextSize)
{
	if (TextSize <= 1.0f)
	{
		TextSize = 1.0f;
	}
	m_Info.fTextSize = TextSize;
}

//========================================
// �ҋ@����
//========================================
void CFontText::SetStandTime(int StandTime)
{
	if (StandTime <= 0)
	{
		StandTime = 0;
	}
	m_Info.nStandTime = StandTime;
}

//========================================
// �����܂ł̎���
//========================================
void CFontText::EraseTime(int Time)
{
	if (Time <= 0)
	{
		Time = 0;
	}
	m_Info.nDisapTime = Time;
	m_Info.nDisapTimeMax = m_Info.nDisapTime;
}

//========================================
// �\������܂ł̎���
//========================================
void CFontText::TextLetter(const char *Text, int nAppearTime)
{
	m_Info.sALLText = Text;
	m_Info.nTextLength = m_Info.sALLText.size();
	m_Info.nAddCount = 0;
	m_Info.nAppearTime = nAppearTime;
	m_Info.nAddLetter = 0;
	m_Info.bLetterSkip = false;
	m_Info.bLetterEnd = false;
	m_Info.words = new CWords*[m_Info.nTextLength];

	for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++)
	{
		m_Info.words[wordsCount] = NULL;
	}
}

//========================================
// �폜�t���O
//========================================
void CFontText::Disap(bool bDisap, int nTime)
{
	m_Info.bRelease = bDisap;
	EraseTime(nTime);
}

//=============================================
// 1�o�C�g�������V�t�gJIS���ǂ������肷��֐�
//=============================================
bool CFontText::CheckLeadByte(int nLetter)
{
	return (((nLetter & 0xffu) ^ 0x20u) - 0xa1) < 94u / 2;
}

//========================================
// �|�[�Y���ł��e�L�X�g�������邩
//========================================
void CFontText::SetTxtPause(bool bPause)
{
	m_Info.bPause = bPause;
}

//========================================
// �����̐ݒ�
//========================================
void CFontText::SetEdging(FontEdging *Edging)
{
	if (Edging != NULL)
	{
		m_Info.aEdging.edging = new CWords*[m_Info.nTextLength];

		for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++) {
			m_Info.aEdging.edging[wordsCount] = NULL;
		}

		m_Info.aEdging.col = Edging->col;
		m_Info.aEdging.AddSize = Edging->AddSize;
		m_Info.aEdging.bEdging = Edging->bEdging;
	}
}

//========================================
// TextBox�̈ʒu
//�e�x�N�g�����ʂɐݒ�
//========================================
void CFontText::SetTxtBoxPos(CFloat &X, CFloat &Y, bool bMoveWordsX, bool bMoveWordsY)
{
	//�ʒu�ݒ�
	m_Info.TxtBoxPos = Pos2D(X, Y);

	//�����ʒu�ݒ�
	SetWordPos(bMoveWordsX, bMoveWordsY);
}

//========================================
// �z�u���������ʒu
//========================================
void CFontText::SetWordPos(bool bMoveWordsX, bool bMoveWordsY)
{
	//�����ݒ肵�Ȃ��Ȃ�A��
	if ((!bMoveWordsX && !bMoveWordsY) || *m_Info.words == NULL) return;

	//�z�u�ʒu�ݒ�
	for (int nCntWord = 0; nCntWord < m_Info.nLetterPopCount; nCntWord++)
	{
		//���݈ʒu���擾���A�ړ�������x�N�g���̒l����
		Pos3D SetPos = m_Info.words[nCntWord]->GetPos();
		Pos3D SetPos1 = INITD3DXVECTOR3;

		if (m_Info.aEdging.edging[nCntWord] != NULL)
			SetPos1 = m_Info.aEdging.edging[nCntWord]->GetPos();

		if (bMoveWordsX) {
			SetPos.x = m_Info.TxtBoxPos.x;
			SetPos1.x = m_Info.TxtBoxPos.x;
		}
		if (bMoveWordsY) {
			SetPos.y = m_Info.TxtBoxPos.y * 2;
			SetPos1.y = m_Info.TxtBoxPos.y * 2;
		}

		//�����ʒu���
		m_Info.words[nCntWord]->SetPos(SetPos);

		if (m_Info.aEdging.edging[nCntWord] != NULL)
			m_Info.aEdging.edging[nCntWord]->SetPos(SetPos);
	}
}

//========================================
// �e�L�X�g�{�b�N�X�̃e�N�X�`���ݒ�
//========================================
void CFontText::SetTxtBoxTex(const char* Path, int PthIdx, int PthX, int PthY)
{
	if (Path != NULL)
	{
		m_Info.Tex.Idx = RNLib::Texture().Load(Path);

		if (PthIdx >= 0)
		{
			m_Info.Tex.PtnIdx = PthIdx;
			m_Info.Tex.PtnX = PthX;
			m_Info.Tex.PtnY = PthY;
		}
	}
}

//========================================
// �e�L�X�g�{�b�N�X�̃p�^�[���ݒ�
//========================================
void CFontText::SetTxtBoxPthIdx(int PthIdx)
{
	if (PthIdx >= 0)
	{
		m_Info.Tex.PtnIdx = PthIdx;
	}
}

//========================================
// �e�L�X�g�{�b�N�X�̐F�ݒ�
//========================================
void CFontText::SetTxtBoxColor(Color col)
{
	m_Info.TxtBoxCol = col;
}

//========================================
// �e�L�X�g�{�b�N�X�̎�ސݒ�
//========================================
void CFontText::SetTxtBoxType(Box type)
{
	// -- ���b�Z�[�W�{�b�N�X ----------------
	m_Info.Tex.type = type;

	// �e�N�X�`���ݒ�
	if (type == BOX_MAINE_MENU)
		m_Info.Tex.Idx = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox10.png");
	else if (type == BOX_SUB_TITLE)
		m_Info.Tex.Idx = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox11.png");
	else if (type == BOX_SUB_MENU00)
		m_Info.Tex.Idx = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox13.png");
	else if (type == BOX_SUB_SELECTU00)
		m_Info.Tex.Idx = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox12.png");
	else if (type == BOX_SUB_MENU01)
		m_Info.Tex.Idx = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox15.png");
	else if (type == BOX_SUB_SELECTU01)
		m_Info.Tex.Idx = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox14.png");
	else if (type == BOX_NONE || type == BOX_MAX)
		m_Info.Tex.Idx = -1;
}

//========================================
// �e�L�X�g�̐F�ݒ�
//========================================
bool CFontText::SetTxtColor(D3DXCOLOR col)
{

	for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++)
	{
		if (m_Info.words[wordsCount] != NULL)
		{
			m_Info.words[wordsCount]->SetColor(col);
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

//========================================
// �e�L�X�g�̃T�C�Y�ݒ�
//========================================
bool CFontText::SetTxtSize(float size)
{
	for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++)
	{
		if (m_Info.words[wordsCount] != NULL)
		{
			m_Info.words[wordsCount]->SetSize(Pos3D(size,size,0.0f));
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

//========================================
// �����ύX(�P��)
//========================================
bool CFontText::ChgWords(char* Text, int nIdx, D3DXCOLOR col)
{
	if (m_Info.words[nIdx] != NULL)
	{
		float fTxtSize = m_Info.fTextSize;
		Pos3D pos = m_Info.words[nIdx]->GetPos();

		m_Info.words[nIdx]->Uninit();

		// �e�̍Đ���
		if (m_Info.aShadow.bShadow)
		{
			Pos3D AddPos = m_Info.aShadow.AddPos;
			Pos2D AddSize = m_Info.aShadow.AddSize;
			D3DXCOLOR ShadowCol = m_Info.aShadow.col;

			m_Info.aShadow.shadow[nIdx]->Uninit();

			if (Text != NULL)
			{
				m_Info.aShadow.shadow[nIdx] = CWords::Create(Text,
					Pos3D(pos.x + AddPos.x, pos.y + AddPos.y, pos.z + AddPos.z),
					Pos3D(fTxtSize + AddSize.x, fTxtSize + AddSize.y, 0.0f),
					m_Info.FontType, ShadowCol);
			}
		}

		// �e�L�X�g�̍Đ���
		if (Text != NULL)
		{
			m_Info.words[nIdx] = CWords::Create(Text,
				pos,
				Pos3D(fTxtSize, fTxtSize, 0.0f),
				m_Info.FontType, col);
		}

		return TRUE;
	}
	return FALSE;
}

//========================================
// �����ύX(�S��)�@���p�̂�
//========================================
bool CFontText::ChgHalfSizeText(char* Text, D3DXCOLOR col)
{
	int nDigit = strlen(Text);
	char aString[TXT_MAX];

	sprintf(aString,Text);

	if (m_Info.words != NULL)
	{
		if (nDigit <= m_Info.nLetterPopCount)
		{
			for (int nIdx = 0; nIdx < nDigit; nIdx++)
			{
				if (!ChgWords(&aString[nIdx], nIdx, col))
				{
					return FALSE;
				}
			}

			int nRest = m_Info.nLetterPopCount - nDigit;

			if (nRest > 0)
			{
				for (int nIdx = 0; nIdx < nRest; nIdx++)
				{
					int nCntRest = nIdx + nDigit;

					if (!ChgWords("", nCntRest, col))
					{
						return FALSE;
					}
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}

//========================================
// �e�L�X�g�̍Đ���
//========================================
void CFontText::Regeneration(const char *Text, CFont::FONT FontType, FontFont *pFont, FontShadow *Shadow, FontEdging *Edging)
{
	// -- �j�� -----------------------
	{
		for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++) {
			if (m_Info.words[wordsCount] != NULL)
				m_Info.words[wordsCount]->Uninit();
		}

		if (m_Info.words != NULL) {
			delete[] m_Info.words;
			m_Info.words = NULL;
		}

		if (m_Info.aShadow.bShadow) {
			for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++) {
				if (m_Info.aShadow.shadow[wordsCount] != NULL)
					m_Info.aShadow.shadow[wordsCount]->Uninit();
			}

			if (m_Info.aShadow.shadow != NULL) {
				delete[] m_Info.aShadow.shadow;
				m_Info.aShadow.shadow = NULL;
			}
		}
	}

	// -- ���� -----------------------
	m_Info.FontType = FontType;

	m_Info.nLetterPopCount = 0;
	m_Info.nLetterPopCountX = 0;
	m_Info.nAddLetter = 0;
	m_Info.nNiCount = 0;

	if (pFont != NULL){
		m_Info.FontCol = pFont->col;
		SetTextSize(pFont->fTextSize);
		SetStandTime(pFont->nStandTime);
		EraseTime(pFont->nEraseTime);
		TextLetter(Text, pFont->nAppearTime);
	}
	else if (pFont == NULL){
		m_Info.FontCol = INITD3DCOLOR;
		SetTextSize(20.0f);
		SetStandTime(10);
		EraseTime(1);
		TextLetter(Text, 1);
	}

	if (Shadow == NULL){
		m_Info.aShadow.col = INITD3DCOLOR;
		m_Info.aShadow.AddPos = INITPOS3D;
		m_Info.aShadow.AddSize = INITPOS2D;
		m_Info.aShadow.bShadow = false;
	}
	else if (Shadow != NULL){
		if (Shadow->bShadow){
			m_Info.aShadow.shadow = new CWords*[m_Info.nTextLength];

			for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++){
				m_Info.aShadow.shadow[wordsCount] = NULL;
			}

			m_Info.aShadow.col = Shadow->col;
			m_Info.aShadow.AddPos = Shadow->AddPos;
			m_Info.aShadow.AddSize = Shadow->AddSize;
			m_Info.aShadow.bShadow = Shadow->bShadow;
		}
	}

	if (Edging == NULL)
	{
		m_Info.aEdging.col = INITD3DCOLOR;
		m_Info.aEdging.AddSize = INITPOS2D;
		m_Info.aEdging.bEdging = false;
	}
	else if (Edging != NULL)
	{
		m_Info.aEdging.edging = new CWords*[m_Info.nTextLength];

		for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++) {
			m_Info.aEdging.edging[wordsCount] = NULL;
		}

		m_Info.aEdging.col = Edging->col;
		m_Info.aEdging.AddSize = Edging->AddSize;
		m_Info.aEdging.bEdging = Edging->bEdging;
	}
}