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
	"",                 //イベント無し
	"data\\TALK\\.txt", // 出発前（タイトル->ステージ選択の合間？
	"data\\TALK\\.txt", // 不時着陸（1-1の開始イベント
	"data\\TALK\\.txt", // ロケット発見(1-3クリア後
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
	m_nNextInterval = 0;   //次の文字・会話を表示するまでのインターバル
	m_nStringNumAll = 0;   //現在の会話の最大文字数
	m_nStringNum = 0;      //現在の会話の表示文字数
	m_bEndSpeak = false;   //発言終了（会話自体の終了ではない
	m_pPopText = NULL;     //表示するテキスト

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
	if (s_pTalk != NULL && s_Event != EVENT::NONE) return NULL;

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
	m_nNextInterval = 0;   //次の文字・会話を表示するまでのインターバル
	m_nStringNumAll = 0;   //現在の会話の最大文字数
	m_nStringNum = 0;      //現在の会話の表示文字数
	m_bEndSpeak = false;   //発言終了（会話自体の終了ではない
	m_pPopText = NULL;     //表示するテキスト
}

//=======================================
//会話イベント読込
//=======================================
void CTalk::LoadTalk(EVENT &Event)
{
	int nTalkCounter = 0;

	// ファイルを開く
	if (RNLib::File().OpenLoadFile("data\\TALK\\test.txt")) {
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
				//一時初期化
				s_pTalk[nTalkCounter].pLog = NULL;
				s_pTalk[nTalkCounter].TalkerID = NONEDATA;
				s_pTalk[nTalkCounter].nTex = NONEDATA;

				char LogTmp[TXT_MAX] = {};

				while (RNLib::File().SearchLoop("END_TALK")) {
					RNLib::File().Scan(_RNC_File::SCAN::STRING, &LogTmp[0], "TALK");
					RNLib::File().Scan(_RNC_File::SCAN::INT,    &s_pTalk[nTalkCounter].TalkerID, "PLAYER");
					RNLib::File().Scan(_RNC_File::SCAN::INT,    &s_pTalk[nTalkCounter].nTex, "TEXTURE");
				}

				CInt len = strlen(&LogTmp[0]);
				s_pTalk[nTalkCounter].pLog = new char[len + 1];
				strcpy(&s_pTalk[nTalkCounter].pLog[0], &LogTmp[0]);

				ZeroMemory(LogTmp, len);
				strcpy(&LogTmp[0], &s_pTalk[nTalkCounter].pLog[0]);
				CInt Talker = s_pTalk[nTalkCounter].TalkerID;
				CInt nTex = s_pTalk[nTalkCounter].nTex;

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
		m_nTalkID = 0;       //会話番号
		m_nNextInterval = 0; //次の文字・会話を表示するまでのインターバル
		m_nStringNum = 0;    //現在の会話の表示文字数
		m_bEndSpeak = false; //発言終了（会話自体の終了ではない
		m_pPopText = NULL;   //表示するテキスト

		m_nStringNumAll = strlen(s_pTalk[0].pLog) + 1; //現在の会話の最大文字数
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
	if (s_pTalk == NULL)
		return;

	//テキスト表示
	PopUpText();

	//会話終了
	if (!m_bTalk)
		return;

	//発言がまだ終わっていない
	else if (!m_bEndSpeak)
		NextChar();

	//次の会話へ  or  会話終了
	else if (RNLib::Input().GetTrigger(DIK_RETURN, _RNC_Input::BUTTON::A) ||
			 RNLib::Input().GetTrigger(DIK_RETURN, _RNC_Input::BUTTON::A, 1))
	{
		//次の発言へ
		m_nTalkID++;
		m_bEndSpeak = false;
		m_pPopText = NULL;

		//会話終了
		if (m_nTalkID == m_nTalkNumAll)
			m_bTalk = false;
	}
}

//=======================================
// 次の文字を表示
//=======================================
void CTalk::NextChar(void)
{
	//インターバル減少して終了
	if (--m_nNextInterval > 0) {
		m_nNextInterval--;
		return;
	}

	char Text[TXT_MAX] = {};

	//インターバルクリア
	m_nNextInterval = 0;

	//次の文字数へ（次の文字数が最大数を超えたら最大文字数に設定
	CInt NextTextNum = m_nStringNum + 2 <= m_nStringNumAll ? m_nStringNum + 2 : m_nStringNumAll;

	//次の文字まで接続
	for (int nCntText = 0; nCntText < NextTextNum; nCntText++)
	{
		sprintf(&Text[0], "%s%c", &Text[0], s_pTalk[m_nTalkID].pLog[nCntText]);
	}

	//m_pPopText開放・確保
	//文字列コピー
	//strcpy(&m_pPopText[0], &Text[0]);

	//文字数が最大数と一致なら発言終了
	if (NextTextNum == m_nStringNumAll)
		m_bEndSpeak = true;

	//インターバル設定
	m_nNextInterval = NEXT_INTERVAL;
}

//=======================================
// 文章表示
//=======================================
void CTalk::PopUpText(void)
{
	//テキストの背景
	RNLib::Polygon2D().Put(PRIORITY_UI, Pos2D(RNLib::Window().GetCenterX(), 600.0f), 0.0f)
		->SetCol(Color{ 10,10,10,200 })
		->SetSize(600.0f, 40.0f);

	//テキスト
	RNLib::Text2D().Put(PRIORITY_TEXT, m_pPopText, _RNC_Text::ALIGNMENT::LEFT, NONEDATA, Pos2D(60.0f, 600.0f), 0.0f)
		->SetSize(Size2D(10.0f, 10.0f));
}