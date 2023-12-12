//========================================
// 
// テキスト表示
// 
//========================================
// *** text.cpp ***
//========================================
#include "font-text.h"
#include "../../manager.h"

//========================================
// コンストラクタ
//========================================
CFontText::CFontText(int nPriority) : CFontObject(nPriority)
{
	m_Info.TexPos = INITD3DXVECTOR2;
	m_Info.TextBoxCol = INITCOLOR;
	m_Info.TexMove = INITD3DXVECTOR2;
	m_Info.FontCol = INITD3DCOLOR;
	m_Info.TextBoxColOld = INITD3DCOLOR;
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
	m_Info.nTexIdx = 0;

	m_Info.nStandTime = 0;
	m_Info.bStand = false;

	m_Info.nDisapTime = 0;
	m_Info.nDisapTimeMax = 0;
	m_Info.bRelease = false;

	m_Info.bTextBok = true;
	m_Info.bPause = false;
	m_Info.bSpace = false;

	m_Info.aShadow.col = INITD3DCOLOR;
	m_Info.aShadow.AddPos = INITD3DXVECTOR3;
	m_Info.aShadow.AddSize = INITD3DXVECTOR2;
	m_Info.aShadow.bShadow = false;
}

//========================================
// デストラクタ
//========================================
CFontText::~CFontText()
{

}

//================================================================================
//--------------------------------------------------------------------------------
//********************************** メイン処理 **********************************
//--------------------------------------------------------------------------------
//================================================================================

//========================================
// 初期化
//========================================
HRESULT CFontText::Init()
{
	m_Info.TextBoxCol = INITCOLOR;
	m_Info.FontCol = INITD3DCOLOR;
	m_Info.TextBoxColOld = INITD3DCOLOR;
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
	m_Info.bStand = false;

	m_Info.nDisapTime = 0;
	m_Info.nDisapTimeMax = 0;
	m_Info.bRelease = false;
	m_Info.bPause = false;

	return S_OK;
}

//========================================
// 終了
//========================================
void CFontText::Uninit()
{
	// 文字の削除
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

	Release();
}

//========================================
// 更新
//========================================
void CFontText::Update()
{
	m_Info.TexPos += m_Info.TexMove;

	if (m_Info.bTextBok)
	{
		RNLib::Polygon2D().Put(PRIORITY_TEXT, m_Info.TexPos, 0.0f, false)
			->SetSize(m_Info.TexSize.x, m_Info.TexSize.y)
			->SetCol(m_Info.TextBoxCol)
			->SetTex(m_Info.nTexIdx);

		/*RNLib::Polygon2D().Put(PRIORITY_TEXT, m_Info.TexPos, 0.0f, false)
			->SetSize(m_Info.TexSize.x, m_Info.TexSize.y)
			->SetCol(m_Info.TextBoxCol)
			->SetTex(m_Info.nTexIdx, 0, 1, 2);*/
	}

	m_Info.TexMove = INITD3DXVECTOR2;

	// テキスト生成
	if (!m_Info.bStand)
	{
		LetterForm();
	}

	// 待機処理
	StandTime();

	// 削除処理
	DisapTime();
}

//========================================
// 描画
//========================================
void CFontText::Draw()
{

}

//========================================
// 生成
//========================================
CFontText *CFontText::Create(Box type, D3DXVECTOR3 pos, D3DXVECTOR2 size, const char *Text, CFont::FONT FontType, FormFont *pFont, bool bBoxSize, bool bTextBox, FormShadow *Shadow)
{
	CFontText *pText = new CFontText;

	if (pText != NULL)
	{
		pText->Init();

		// -- メッセージボックス ----------------

		// テクスチャ設定
		if(type == BOX_NORMAL_GRAY)
			pText->m_Info.nTexIdx = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox00.png");
		else if (type == BOX_NORMAL_BLUE)
			pText->m_Info.nTexIdx = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox01.png");
		else if (type == BOX_NORMAL_RED)
			pText->m_Info.nTexIdx = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox02.png");
		else if (type == BOX_NORMAL_GREEN)
			pText->m_Info.nTexIdx = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox03.png");
		else if(type == BOX_NONE || type == BOX_MAX)
			pText->m_Info.nTexIdx = -1;

		pText->m_Info.TexPos = Pos2D(pos.x, pos.y);
		pText->m_Info.TexSize = size;
		pText->m_Info.bTextBok = bTextBox;

		// -- テキスト -----------------------
		pText->m_Info.FontType = FontType;

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
			pText->m_Info.TexSize.x = pText->m_Info.fTextSize * (pText->m_Info.nTextLength * 0.5f + 1);
		}

		if (Shadow == NULL)
		{
			pText->m_Info.aShadow.col = INITD3DCOLOR;
			pText->m_Info.aShadow.AddPos = INITD3DXVECTOR3;
			pText->m_Info.aShadow.AddSize = INITD3DXVECTOR2;
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
	}

	return pText;
}

//================================================================================
//--------------------------------------------------------------------------------
//********************************** その他処理 **********************************
//--------------------------------------------------------------------------------
//================================================================================

//========================================
// 文字生成
//========================================
void CFontText::LetterForm(void)
{
	float fTxtSize = m_Info.fTextSize;

	if (++m_Info.nAddCount >= m_Info.nAppearTime)
	{// 表示する時間を上回ったら、

		if (m_Info.nAddLetter <= m_Info.nTextLength)
		{// テキストサイズを下回ってたら、

			m_Info.sText += m_Info.sALLText[m_Info.nAddLetter];
			string Text = m_Info.sText;
			D3DXVECTOR3 pos = m_Info.TexPos * 2;

			pos.x = pos.x - ((m_Info.TexSize.x * 2) / 2);

			if (Text != "" && m_Info.nAddLetter < m_Info.nTextLength)
			{// 空白じゃなかったら、 && テキストサイズを下回ってたら、

				if (CheckLeadByte(m_Info.sALLText[m_Info.nAddLetter]))
				{// 1バイト文字がシフトJISだったら、

					m_Info.nAddLetter++;
					m_Info.sText += m_Info.sALLText[m_Info.nAddLetter];
					m_Info.nAddLetter++;

					// 影
					if (m_Info.aShadow.bShadow)
					{
						D3DXVECTOR3 AddPos = m_Info.aShadow.AddPos;
						D3DXVECTOR2 AddSize = m_Info.aShadow.AddSize;

						m_Info.aShadow.shadow[m_Info.nLetterPopCount] = CWords::Create(m_Info.sText.c_str(),
							D3DXVECTOR3((pos.x + (SPACE + AddPos.x)) + ((fTxtSize * 2) * (m_Info.nLetterPopCountX + SPACE_X)), (pos.y + AddPos.y) + m_Info.nNiCount * 40.0f, pos.z),
							D3DXVECTOR3(fTxtSize + AddSize.x, fTxtSize + AddSize.y, 0.0f),
							m_Info.FontType, m_Info.aShadow.col);
					}

					m_Info.words[m_Info.nLetterPopCount] = CWords::Create(m_Info.sText.c_str(),
						D3DXVECTOR3((pos.x + SPACE) + ((fTxtSize * 2) * (m_Info.nLetterPopCountX + SPACE_X)), pos.y + m_Info.nNiCount*40.0f, pos.z),
						D3DXVECTOR3(fTxtSize, fTxtSize, 0.0f),
						m_Info.FontType, m_Info.FontCol);

					m_Info.nLetterPopCount++;
					m_Info.nLetterPopCountX++;
				}
				else
				{
					if (m_Info.sText != "\n")
					{// 終端文字なら、

						m_Info.nAddLetter++;

						if (m_Info.bSpace && m_Info.sText == " ")
						{
							m_Info.nLetterPopCount++;
							m_Info.nLetterPopCountX++;
							m_Info.sText = "";
							m_Info.nAddCount = 0;
							return;
						}

						// 影
						if (m_Info.aShadow.bShadow)
						{
							D3DXVECTOR3 AddPos = m_Info.aShadow.AddPos;
							D3DXVECTOR2 AddSize = m_Info.aShadow.AddSize;

							m_Info.aShadow.shadow[m_Info.nLetterPopCount] = CWords::Create(m_Info.sText.c_str(),
								D3DXVECTOR3((pos.x + (SPACE + AddPos.x)) + ((fTxtSize * 2) * (m_Info.nLetterPopCountX + SPACE_X)), (pos.y + AddPos.y) + m_Info.nNiCount * 40.0f, pos.z),
								D3DXVECTOR3(fTxtSize + AddSize.x, fTxtSize + AddSize.y, 0.0f),
								m_Info.FontType, m_Info.aShadow.col);
						}

						m_Info.words[m_Info.nLetterPopCount] = CWords::Create(m_Info.sText.c_str(),
							D3DXVECTOR3((pos.x + SPACE) + ((fTxtSize * 2) * (m_Info.nLetterPopCountX + SPACE_X)), pos.y + m_Info.nNiCount*40.0f, pos.z),
							D3DXVECTOR3(fTxtSize, fTxtSize, 0.0f),
							m_Info.FontType, m_Info.FontCol);

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
			}
			else
			{
				m_Info.bStand = true;
			}
		}

		m_Info.sText = "";
		m_Info.nAddCount = 0;
	}
}

//========================================
// 待機
//========================================
void CFontText::StandTime(void)
{
	if (m_Info.bStand && m_Info.nStandTime >= 1)
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
// 削除
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

		// 色の推移
		m_Info.TextBoxCol.a *= ((float)m_Info.nDisapTime / m_Info.nDisapTimeMax);
		m_Info.FontCol.a *= ((float)m_Info.nDisapTime / m_Info.nDisapTimeMax);

		// 文字色の推移
		for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++)
		{
			if (m_Info.words[wordsCount] != NULL)
			{
				m_Info.words[wordsCount]->SetColar(m_Info.FontCol);
			}
		}

		if (m_Info.aShadow.bShadow)
		{
			m_Info.aShadow.col.a *= ((float)m_Info.nDisapTime / m_Info.nDisapTimeMax);

			for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++)
			{
				if (m_Info.aShadow.shadow[wordsCount] != NULL)
				{
					m_Info.aShadow.shadow[wordsCount]->SetColar(m_Info.aShadow.col);
				}
			}
		}
	}
}

//================================================================================
//--------------------------------------------------------------------------------
//*********************************** 設定処理 ***********************************
//--------------------------------------------------------------------------------
//================================================================================

//========================================
// 移動量
//========================================
void CFontText::SetMove(D3DXVECTOR3 move)
{
	m_Info.TexMove = Pos2D(move.x, move.y);

	for (int nWords = 0; nWords < m_Info.nLetterPopCount; nWords++)
	{
		if (m_Info.words[nWords] != NULL)
		{
			m_Info.words[nWords]->SetMove(move);
		}
	}

}

//========================================
// 文字サイズ
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
// 待機時間
//========================================
void CFontText::SetStandTime(int StandTime)
{
	if (StandTime <= 0)
	{
		StandTime = 0;
	}
	m_Info.nStandTime = StandTime;
	m_Info.bStand = false;
}

//========================================
// 消すまでの時間
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
// 表示するまでの時間
//========================================
void CFontText::TextLetter(const char * Text, int AppearTime)
{
	m_Info.sALLText = Text;
	m_Info.nTextLength = m_Info.sALLText.size();
	m_Info.nAddCount = AppearTime;
	m_Info.nAppearTime = m_Info.nAddCount;
	m_Info.nAddLetter = 0;
	m_Info.words = new CWords*[m_Info.nTextLength];

	for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++)
	{
		m_Info.words[wordsCount] = NULL;
	}
}

//========================================
// 削除フラグ
//========================================
void CFontText::Disap(bool bDisap, int nTime)
{
	m_Info.bRelease = bDisap;
	EraseTime(nTime);
}

//=============================================
// 1バイト文字をシフトJISかどうか判定する関数
//=============================================
bool CFontText::CheckLeadByte(int nLetter)
{
	return (((nLetter & 0xffu) ^ 0x20u) - 0xa1) < 94u / 2;
}

//========================================
// ポーズ中でもテキスト生成するか
//========================================
void CFontText::SetTetPause(bool bPause)
{
	m_Info.bPause = bPause;
}

//========================================
// テキストボックスの色設定
//========================================
void CFontText::SetBoxColor(Color col)
{
	m_Info.TextBoxCol = col;
}

//========================================
// テキストボックスの種類設定
//========================================
void CFontText::SetBoxType(Box type)
{
	// -- メッセージボックス ----------------

	// テクスチャ設定
	if (type == BOX_NORMAL_GRAY)
		m_Info.nTexIdx = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox00.png");
	else if (type == BOX_NORMAL_BLUE)
		m_Info.nTexIdx = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox01.png");
	else if (type == BOX_NORMAL_RED)
		m_Info.nTexIdx = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox02.png");
	else if (type == BOX_NORMAL_GREEN)
		m_Info.nTexIdx = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox03.png");
	else if (type == BOX_NONE || type == BOX_MAX)
		m_Info.nTexIdx = -1;
}

//========================================
// テキストの色設定
//========================================
bool CFontText::SetTextColor(D3DXCOLOR col)
{

	for (int wordsCount = 0; wordsCount < m_Info.nTextLength; wordsCount++)
	{
		if (m_Info.words[wordsCount] != NULL)
		{
			m_Info.words[wordsCount]->SetColar(col);
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

//========================================
// 文字変更(単体)
//========================================
bool CFontText::ChgWords(char* Text, int nIdx, D3DXCOLOR col)
{
	if (m_Info.words[nIdx] != NULL)
	{
		float fTxtSize = m_Info.fTextSize;
		D3DXVECTOR3 pos = m_Info.words[nIdx]->GetPos();

		m_Info.words[nIdx]->Uninit();

		// 影の再生成
		if (m_Info.aShadow.bShadow)
		{
			D3DXVECTOR3 AddPos = m_Info.aShadow.AddPos;
			D3DXVECTOR2 AddSize = m_Info.aShadow.AddSize;
			D3DXCOLOR ShadowCol = m_Info.aShadow.col;

			m_Info.aShadow.shadow[nIdx]->Uninit();

			if (Text != NULL)
			{
				m_Info.aShadow.shadow[nIdx] = CWords::Create(Text,
					D3DXVECTOR3(pos.x + AddPos.x, pos.y + AddPos.y, pos.z + AddPos.z),
					D3DXVECTOR3(fTxtSize + AddSize.x, fTxtSize + AddSize.y, 0.0f),
					m_Info.FontType, ShadowCol);
			}
		}

		// テキストの再生成
		if (Text != NULL)
		{
			m_Info.words[nIdx] = CWords::Create(Text,
				pos,
				D3DXVECTOR3(fTxtSize, fTxtSize, 0.0f),
				m_Info.FontType, col);
		}

		return TRUE;
	}
	return FALSE;
}

//========================================
// 文字変更(全体)　半角のみ
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
// テキストの再生成
//========================================
void CFontText::Regeneration(const char *Text, CFont::FONT FontType, FormFont *pFont, FormShadow *Shadow)
{
	// -- 破棄 -----------------------
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

	// -- 生成 -----------------------
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
		m_Info.aShadow.AddPos = INITD3DXVECTOR3;
		m_Info.aShadow.AddSize = INITD3DXVECTOR2;
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
}