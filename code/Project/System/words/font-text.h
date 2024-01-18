//========================================
// 
// テキスト表示
// 
//========================================
// *** text.h *** www
//========================================
#pragma once

#include "object\font-object.h"
#include "words.h"

//****************************************
// 構造体
//****************************************

// テキスト情報(生成用)
struct FormFont
{
	D3DXCOLOR col;       // 文字の色
	float fTextSize;     // 文字のサイズ(初期値 20)
	int nAppearTime;     // 2文字目以降が表示されるまでの時間(初期値 1)
	int nStandTime;      // 待機時間(初期値 10)
	int nEraseTime;      // 消えるまでの時間(初期値 1) ※ 0 は消えない
};

// 影情報(生成用)
struct FormShadow
{
	D3DXCOLOR col;			// 影の色
	bool bShadow;			// 影フラグ
	D3DXVECTOR3 AddPos;		// 文字の位置からずらす値 (初期値 0,0,0)	(元の文字 + AddPos)
	D3DXVECTOR2 AddSize;	// 文字のサイズの加算値 (初期値 0,0)		(元の文字 + AddSize)
};

//****************************************
// 前方宣言
//****************************************
class CFont;

//****************************************
// クラス
//****************************************
class CFontText : public CFontObject
{
public:

	static const int BOX_SIZE = 60;		// １文字当たりのTxtSizeサイズ

	static const int SPACE = 0;			// 空白
	static const int SPACE_X = 1;		// 文字生成の開始位置（X）

	// ***** 列挙型 *****

	// テキストボックスの種類
	enum Box
	{
		BOX_MAINE_MENU = 0,		// メインメニュー
		BOX_SUB_TITLE,			// サブメニューのタイトル
		BOX_SUB_MENU00,			// サブメニュー00
		BOX_SUB_SELECTU00,		// サブメニュー00の選択時
		BOX_SUB_MENU01,			// サブメニュー00
		BOX_SUB_SELECTU01,		// サブメニュー00の選択時
		BOX_NONE,				// 無し
		BOX_MAX
	};

	// ***** 関数 *****
	CFontText(int nPriority = PRIO_TEXT);
	~CFontText();

	// -- メイン ------------------------------------------
	/* 初期化	*/HRESULT Init(void);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(void);

	//--------------------------------------------------
	// テキスト生成
	// 引数1  : Box type           / テキストボックスの画像
	// 引数2  : Pos3D pos          / テキストボックスの位置
	// 引数3  : Pos2D size         / テキストボックスのサイズ
	// 引数4  : const char *Text   / テキスト
	// 引数5  : CFont::FONT Type   / フォント種類
	// 引数6  : FormFont *pFont    / フォント関連の情報(色・時間など) 無くても大丈夫
	// 引数7  : bool bBoxSize      / メッセージボックスのサイズ調整の無効・有効（初期値 false）
	// 引数8  : bool bTextBox      / メッセージボックスの表示・非表示 (初期値 true)
	// 引数9  : FormShadow *Shadow / フォントの影の情報 無くても大丈夫
	//--------------------------------------------------
	static CFontText *CFontText::Create(Box type, Pos3D pos, Pos2D size, const char *Text, CFont::FONT FontType, FormFont *pFont = NULL, bool bBoxSize = false, bool bTextBox = true, FormShadow *Shadow = NULL);

	/* 削除 */void Disap(bool bDisap,int nTime);

	// -- 設定 ------------------------------------------
	/* 生成中の文字を一括で	*/void SetTexrSkip(bool skip) { m_Info.bLetterSkip = skip; }//www
	/* 移動量				*/void SetMove(Pos3D move);
	/* 空白表示				*/void SetSpace(bool bSpace) { m_Info.bSpace = bSpace; }
	/* ポーズ中の生成		*/void SetTxtPause(bool bPause);
	/* TextBoxの表示		*/void SetTxtBox(bool bTextBox) { m_Info.bTextBok = bTextBox; }
	/* TextBoxの位置		*/void SetTxtBoxPos(CFloat &X, CFloat &Y, bool bMoveWordsX = false, bool bMoveWordsY = false);
	/* TextBoxの位置		*/void SetTxtBoxPos(const Pos2D &pos,     bool bMoveWordsX = false, bool bMoveWordsY = false) { SetTxtBoxPos(pos.x, pos.y, bMoveWordsX, bMoveWordsY); }
	/* TextBoxの位置		*/void SetTxtBoxPos(const Pos3D &pos,     bool bMoveWordsX = false, bool bMoveWordsY = false) { SetTxtBoxPos(pos.x, pos.y, bMoveWordsX, bMoveWordsY); }
	// bSetAll : 既に配置している文字位置も設定するか  trueならSetWordPos関数を使用する
	/* TextBoxのサイズ		*/void SetTxtBoxSize(float width, float height) { m_Info.TxtBoxSize = Pos2D(width, height); }
	/* TextBoxの目標サイズ	*/void SetTxtBoxTgtSize(float width, float height) { m_Info.TxtBoxTgtSize = Pos2D(width, height); }
	/* TextBoxのテクスチャ	*/void SetTxtBoxTex(const char* Path = NULL, int PthIdx = -1, int PthX = 1, int PthY = 1);
	/* TextBoxのパターン番号*/void SetTxtBoxPthIdx(int PthIdx);
	/* TextBoxの色			*/void SetTxtBoxColor(Color col);
	/* TextBoxの種類		*/void SetTxtBoxType(Box type);
	/* テキストの色			*/bool SetTxtColor(D3DXCOLOR col);
	/* テキストのサイズ		*/bool SetTxtSize(float size);
	/* 出現時間				*/void SetAppearTime(CInt nAppearTime) { m_Info.nAppearTime = nAppearTime; }
	/* 文字変更(単体)		*/bool ChgWords(char* Text, int nIdx, D3DXCOLOR col);
	/* 文字変更(全体)		*/bool ChgHalfSizeText(char* Text, D3DXCOLOR col);// ※ 元のテキストより多いと使えない また半角英数のみ
	/* テキストの再生		*/void Regeneration(const char *Text, CFont::FONT FontType, FormFont *pFont = NULL, FormShadow *Shadow = NULL);


	// -- 取得 ------------------------------------------
	/* 何文字表示したか */int GetPopCount() { return m_Info.nLetterPopCount; }
	/* TxtBotのTex番号	*/Box GetTxtBoxType() { return m_Info.Tex.type; }
	/* 位置				*/Pos2D GetTxtBoxPos() { return m_Info.TxtBoxPos; }
	/* Txtサイズ		*/Pos2D GetTxtBoxSize() { return m_Info.TxtBoxSize; }
	/* Txt目標サイズ	*/Pos2D GetTxtBoxTgtSize() { return m_Info.TxtBoxTgtSize; }
	/* 文字サイズ		*/float GetTxtSize() { return m_Info.fTextSize; }
	/* Textの生成フラグ	*/bool GetLetter() { return m_Info.bLetterEnd; }

private:

	// ***** 構造体 *****

	// テクスチャ情報
	struct Texture {
		Box type;   // TxtBoxの種類
		int Idx;    // テクスチャ番号
		int PtnIdx; // パターン番号
		int PtnX;   // Xの分割数
		int PtnY;   // Yの分割数
	};

	// 影
	struct Shadow
	{
		D3DXCOLOR col;   // 影の色
		bool bShadow;    // 影フラグ
		CWords** shadow; // 文字(影)
		Pos3D AddPos;    // 文字の位置からずらす値 (初期値 0,0,0) (元の文字 + AddPos)
		Pos2D AddSize;   // 文字のサイズの加算値   (初期値 0,0)   (元の文字 + AddSize)
	};

	// テキスト情報
	struct Info
	{
		Color TxtBoxCol;			// テキストボックスの色
		D3DXCOLOR FontCol;			// 文字の色
		D3DXCOLOR TxtBoxColOld;		// 前回のテキストボックスの色
		D3DXCOLOR FontColOld;		// 前回の文字の色
		bool bCol;					// 色変更フラグ(ポーズ切替)
		D3DXVECTOR2 TxtBoxPos;		// 位置
		D3DXVECTOR2 TxtBoxMove;		// 移動量
		D3DXVECTOR2 TxtBoxSize;		// サイズ
		D3DXVECTOR2 TxtBoxTgtSize;	// 目標のサイズ

		Texture Tex;			// テクスチャ情報

		float fTextSize;		// 文字のサイズ
		int nTextLength;		// 文字の長さ
		int nAppearTime;		// 1つの文字が加算されるまでの時間
		int nAddCount;			// 文字が加算されるまでのカウント
		int nAddLetter;			// 何文字目入れるか指定
		int nLetterPopCount;	// 文字を出した最大数
		int nLetterPopCountX;	// 横に文字を出した数
		int nNiCount;			// 改行の数
		bool bLetterSkip;		// 生成中なら残りの文字を一括生成する。
		bool bLetterEnd;		// 文字の生成終了したか

		int nStandTime;			// 待機時間

		int nDisapTime;			// 消える時間
		int nDisapTimeMax;		// 消える最大時間
		bool bRelease;			// 消すフラグ

		Shadow aShadow;			// 影

		bool bTextBok;			// テキストボックスの表示フラグ wwwwwwwwww
		bool bPause;			// ポーズ中でも動くか（false：動かない）
		bool bSpace;			// 空白表示フラグ(sprintf用)
		string sText;			// 表示するテキスト
		string sALLText;		// テキストの全体
		CWords** words;			// 文字
		CFont::FONT FontType;	// フォント種類
	};

	// ***** 関数 *****
	/* 文字生成	*/void LetterForm(void);
	/* 待機		*/void StandTime(void);
	/* 削除		*/void DisapTime(void);
	/* 文字判定	*/bool CheckLeadByte(int nLetter);

	// -- 設定 ------------------------------------------
	/* テキスト サイズ	*/void SetTextSize(float TextSize);
	/* 待機時間			*/void SetStandTime(int StandTime);
	/* 文字の消す時間	*/void EraseTime(int time);
	/* 文字の出現時間	*/void TextLetter(const char *Text, int nAppearTime);
	/* 配置した文字位置 */void SetWordPos(bool bMoveWordsX, bool bMoveWordsY);

	// ***** 変数 *****
	Info m_Info;
};