//================================================================================================
// 
// 会話処理[talk.cpp]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "../main.h"
#include "talk.h"

//会話イベントのファイルパス
const char *CTalk::EVENT_FILE[(int)EVENT::MAX] = {
	"",//イベント無し
	"data\\TALK\\talk000.txt", // 出発前（タイトル->ステージ選択の合間？
	"data\\TALK\\talk001.txt", // 不時着陸（1-1の開始イベント
	"data\\TALK\\talk002.txt", // ロケット発見(1-3クリア後
};

CTalk::Talk  *CTalk::s_pTalk = NULL;        //会話内容
CTalk::EVENT  CTalk::s_Event = EVENT::NONE; //イベント

//=======================================
// コンストラクタ
//=======================================
CTalk::CTalk()
{
	s_pTalk = NULL;        //会話内容
	s_Event = EVENT::NONE; //イベント
	m_bTalk = false;       //会話中かどうか
	m_nTalkNumAll = 0;     //最大会話数
	m_nTalkID = 0;         //会話番号
	m_bEndSpeak = false;   //発言終了（会話自体の終了ではない

	m_pText = NULL;

	m_pFont.col = COLOR_WHITE;   // 文字の色
	m_pFont.fTextSize  = 40.0f;  // 文字のサイズ
	m_pFont.nAppearTime = 5;     // 1文字目が表示されるまでの時間
	m_pFont.nStandTime = 8;      // 待機時間
	m_pFont.nEraseTime = 0;      // 消えるまでの時間

	m_pShadow.col = COLOR_BLACK; // 影の色
	m_pShadow.bShadow = true;                   // 影フラグ
	m_pShadow.AddPos = Pos3D(6.0f, 6.0f, 0.0f); // 文字の位置からずらす値 
	m_pShadow.AddSize = Pos2D(4.0f, 4.0f);      // 文字のサイズの加算値 

	DeleteLog(); //会話ログ削除
}

//=======================================
// デストラクタ
//=======================================
CTalk::~CTalk()
{
	//会話ログ削除
	DeleteLog();
}

//=======================================
// 会話イベント指定
//=======================================
CTalk *CTalk::Create(EVENT Event)
{
	//会話中なのでNULLを返す
	if (s_pTalk != NULL && s_Event != EVENT::NONE && Event == EVENT::NONE) return NULL;

	//インスタンス生成
	CTalk *pTalk = new CTalk;

	if (pTalk != NULL)
	{//生成成功
		//初期化処理
		pTalk->Init(Event);
	}

	//インスタンスを返す
	return pTalk;
}

//=======================================
// 会話ログ削除
//=======================================
void CTalk::DeleteLog(void)
{
	if (s_pTalk != NULL)
	{
		for (int nCntTalk = 0; nCntTalk < m_nTalkNumAll; nCntTalk++)
		{
			//会話ログ削除
			if (s_pTalk[nCntTalk].pLog != NULL)
			{
				delete[] s_pTalk[nCntTalk].pLog;
				s_pTalk[nCntTalk].pLog = NULL;
			}
		}

		//会話データ削除
		delete s_pTalk;
		s_pTalk = NULL;
	}

	s_Event = EVENT::NONE; //イベント
	m_bTalk = false;       //会話中かどうか
	m_nTalkNumAll = 0;     //最大会話数
	m_nTalkID = 0;         //会話番号
	m_bEndSpeak = false;   //発言終了（会話自体の終了ではない
	DeleteText();          //表示するテキストのメモリ開放
}

//=======================================
//会話イベント読込
//=======================================
void CTalk::LoadTalk(EVENT &Event)
{
	int nTalkCounter = 0;

	// ファイルを開く
	if (RNLib::File().OpenLoadFile(EVENT_FILE[(int)Event])) {
		while (RNLib::File().SearchLoop("END")) {
			if (nTalkCounter != 0 &&
				nTalkCounter >= m_nTalkNumAll) break;

			//会話数取得・会話ログ数確保
			if (RNLib::File().CheckIdentifier("NUM_TALK"))
			{
				RNLib::File().Scan(_RNC_File::SCAN::INT, &m_nTalkNumAll);
				s_pTalk = new Talk[m_nTalkNumAll];
			}

			//会話内容読み取り
			else if (RNLib::File().CheckIdentifier("SET_TALK")){

				//会話ログ保管用
				char LogTmp[TXT_MAX] = {};
				while (RNLib::File().SearchLoop("END_TALK")) {
					RNLib::File().Scan(_RNC_File::SCAN::STRING, &LogTmp[0], "TALK");
					RNLib::File().Scan(_RNC_File::SCAN::INT,    &s_pTalk[nTalkCounter].TalkerID, "PLAYER");
					RNLib::File().Scan(_RNC_File::SCAN::INT,    &s_pTalk[nTalkCounter].nTex, "TEXTURE");
				}

				//文字列代入
				CInt len = strlen(&LogTmp[0]) + 1;
				s_pTalk[nTalkCounter].pLog = new char[len];
				strcpy(&s_pTalk[nTalkCounter].pLog[0], &LogTmp[0]);

				//次の番号へ
				nTalkCounter++;
			}
		}

		// ファイルを閉じる
		RNLib::File().CloseFile();
	}
}

//=======================================
// 初期化処理
//=======================================
void CTalk::Init(EVENT &Event)
{
	//会話ログ読込
	LoadTalk(Event);

	//読み込めた
	if (s_pTalk != NULL)
	{
		//会話開始
		m_bTalk = true;
		m_nTalkID = -1;      //会話番号
		m_bEndSpeak = false; //発言終了（会話自体の終了ではない

		NextSpeak();         //次に表示するテキスト
	}
}

//=======================================
// 終了処理
//=======================================
void CTalk::Uninit(void)
{
	//データ開放
	DeleteLog();
}

//=======================================
// 更新処理
//=======================================
void CTalk::Update(void)
{
	//会話終了
	if (s_pTalk == NULL || !m_bTalk)
		return;

	//次の会話へ  or  会話終了
	else if (!m_bEndSpeak &&
			 (RNLib::Input().GetTrigger(DIK_T, _RNC_Input::BUTTON::Y) ||
			 RNLib::Input().GetTrigger(DIK_T, _RNC_Input::BUTTON::Y, 1)))
	{
		//次の発言へ
		NextSpeak();
	}
}

//=======================================
//表示するテキストのメモリ開放
//=======================================
void CTalk::DeleteText(void)
{
	//メモリ開放
	if (m_pText != NULL)
	{
		m_pText->Uninit();
		m_pText = NULL;
	}
}

//=======================================
//次にしゃべるテキストの設定
//=======================================
void CTalk::NextSpeak(void)
{
	//次の喋り手へ
	m_nTalkID++;

	//会話終了
	if (m_nTalkID == m_nTalkNumAll)
	{
		DeleteLog();
	}
	else
	{
		//次の会話の最大文字数
		m_bEndSpeak = false;
		DeleteText();

		m_pText = CFontText::Create(CFontText::BOX_NORMAL_GRAY, D3DXVECTOR3(330.0f, 700.0f, 0.0f), INITPOS2D,
									s_pTalk[m_nTalkID].pLog, CFont::FONT_07NIKUMARU, &m_pFont, false, false, &m_pShadow);
	}
}