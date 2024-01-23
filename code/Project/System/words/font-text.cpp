//========================================
// 
// テキスト表示
// 
//========================================
// *** font-text.cpp ***
//========================================
#include "font-text.h"
#include "../../manager.h"

//========================================
// コンストラクタ
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

	// 影
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

	// 縁取り
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
// 更新
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

	// テキスト生成
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
CFontText *CFontText::Create(Box type, Pos3D pos, Pos2D size, const char *Text, CFont::FONT FontType, FontFont *pFont, bool bBoxSize, bool bTextBox, FontShadow *Shadow, FontEdging *Edging, CShort SeIdx)
{
	CFontText *pText = new CFontText;

	if (pText != NULL)
	{
		pText->Init();

		// -- メッセージボックス ----------------

		// テクスチャ設定
		pText->SetTxtBoxType(type);

		pText->m_Info.TxtBoxPos = Pos2D(pos.x, pos.y);
		pText->m_Info.TxtBoxSize = size;
		pText->m_Info.bTextBox = bTextBox;

		// -- テキスト -----------------------
		pText->m_Info.FontType = FontType;

		// -- サウンド -----------------------
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

		// 影
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

		// 縁取り
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
//********************************** その他処理 **********************************
//--------------------------------------------------------------------------------
//================================================================================

//========================================
// 文字生成
//========================================
void CFontText::LetterForm(void)
{
	CFloat& fTxtSize = m_Info.fTextSize;

	if (++m_Info.nAddCount >= m_Info.nAppearTime)
	{// 表示する時間を上回ったら、

		if (m_Info.nAddLetter <= m_Info.nTextLength)
		{// テキストサイズを下回ってたら、

			m_Info.sText += m_Info.sALLText[m_Info.nAddLetter];
			string Text = m_Info.sText;

			// 解像度に合わせる
			Pos3D pos = m_Info.TxtBoxPos * 2;

			pos.x = pos.x - m_Info.TxtBoxSize.x;
			pos.z = 0.0f;

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

					// 縁取り
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

						// 縁取り
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

				// サウンド再生
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
// 待機
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
		m_Info.TxtBoxCol.a *= ((float)m_Info.nDisapTime / m_Info.nDisapTimeMax);
		m_Info.FontCol.a *= ((float)m_Info.nDisapTime / m_Info.nDisapTimeMax);

		// 文字色の推移
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
//*********************************** 設定処理 ***********************************
//--------------------------------------------------------------------------------
//================================================================================

//========================================
// 目標位置(単体)
//========================================
void CFontText::SetTargetPos(D3DXVECTOR3 pos, int nWord, int Time)
{
	// 通常文字
	if (m_Info.words[nWord] != NULL)
	{
		bool bMove = m_Info.words[nWord]->IsMove();

		if (!bMove)
			m_Info.words[nWord]->SetTargetPos(pos, Time);
	}

	// 影
	if (m_Info.aShadow.bShadow)
	{
		if (m_Info.aShadow.shadow[nWord] != NULL)
		{
			bool bMove = m_Info.aShadow.shadow[nWord]->IsMove();

			if (!bMove)
				m_Info.aShadow.shadow[nWord]->SetTargetPos(pos, Time);
		}
	}

	// 縁取り
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
// 目標位置(全体)
//========================================
void CFontText::SetAllTargetPos(D3DXVECTOR3 pos, int Time)
{
	for (int nWords = 0; nWords < m_Info.nLetterPopCount; nWords++)
	{
		// 通常文字
		if (m_Info.words[nWords] != NULL)
		{
			bool bMove = m_Info.words[nWords]->IsMove();

			if (!bMove)
				m_Info.words[nWords]->SetTargetPos(pos,Time);
		}

		// 影
		if (m_Info.aShadow.bShadow)
		{
			if (m_Info.aShadow.shadow[nWords] != NULL)
			{
				bool bMove = m_Info.aShadow.shadow[nWords]->IsMove();

				if (!bMove)
					m_Info.aShadow.shadow[nWords]->SetTargetPos(pos, Time);
			}
		}

		// 縁取り
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
// 移動量
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
void CFontText::SetTxtPause(bool bPause)
{
	m_Info.bPause = bPause;
}

//========================================
// 縁取りの設定
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
// TextBoxの位置
//各ベクトルを個別に設定
//========================================
void CFontText::SetTxtBoxPos(CFloat &X, CFloat &Y, bool bMoveWordsX, bool bMoveWordsY)
{
	//位置設定
	m_Info.TxtBoxPos = Pos2D(X, Y);

	//文字位置設定
	SetWordPos(bMoveWordsX, bMoveWordsY);
}

//========================================
// 配置した文字位置
//========================================
void CFontText::SetWordPos(bool bMoveWordsX, bool bMoveWordsY)
{
	//両方設定しないなら帰る
	if ((!bMoveWordsX && !bMoveWordsY) || *m_Info.words == NULL) return;

	//配置位置設定
	for (int nCntWord = 0; nCntWord < m_Info.nLetterPopCount; nCntWord++)
	{
		//現在位置を取得し、移動させるベクトルの値を代入
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

		//文字位置代入
		m_Info.words[nCntWord]->SetPos(SetPos);

		if (m_Info.aEdging.edging[nCntWord] != NULL)
			m_Info.aEdging.edging[nCntWord]->SetPos(SetPos);
	}
}

//========================================
// テキストボックスのテクスチャ設定
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
// テキストボックスのパターン設定
//========================================
void CFontText::SetTxtBoxPthIdx(int PthIdx)
{
	if (PthIdx >= 0)
	{
		m_Info.Tex.PtnIdx = PthIdx;
	}
}

//========================================
// テキストボックスの色設定
//========================================
void CFontText::SetTxtBoxColor(Color col)
{
	m_Info.TxtBoxCol = col;
}

//========================================
// テキストボックスの種類設定
//========================================
void CFontText::SetTxtBoxType(Box type)
{
	// -- メッセージボックス ----------------
	m_Info.Tex.type = type;

	// テクスチャ設定
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
// テキストの色設定
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
// テキストのサイズ設定
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
// 文字変更(単体)
//========================================
bool CFontText::ChgWords(char* Text, int nIdx, D3DXCOLOR col)
{
	if (m_Info.words[nIdx] != NULL)
	{
		float fTxtSize = m_Info.fTextSize;
		Pos3D pos = m_Info.words[nIdx]->GetPos();

		m_Info.words[nIdx]->Uninit();

		// 影の再生成
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

		// テキストの再生成
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
void CFontText::Regeneration(const char *Text, CFont::FONT FontType, FontFont *pFont, FontShadow *Shadow, FontEdging *Edging)
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