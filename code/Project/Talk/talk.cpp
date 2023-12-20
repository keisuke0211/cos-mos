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
		pTalk->Init();
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
}

//=======================================
//会話イベント読込
//=======================================
void CTalk::LoadTalk(EVENT &Event)
{
	//文字添削
	const char *CODE_TALK    = "TALK";
	const char *CODE_PLAYER  = "PLAYER";
	const char *CODE_TEXTURE = "TEXTURE";

	// ファイルを開く
	if (RNLib::File().OpenLoadFile(EVENT_FILE[(int)Event], "Talk")) {
		while (RNLib::File().SearchLoop("EOF")) {
			//会話数取得・会話ログ数確保
			if (RNLib::File().CheckIdentifier("NUM_TALK"))
			{
				RNLib::File().Scan(_RNC_File::SCAN::INT, &m_nTalkNumAll);
				s_pTalk = new Talk[m_nTalkNumAll];
			}

			//会話内容読み取り
			else if (RNLib::File().CheckIdentifier("SET_TALK")){
				while (RNLib::File().SearchLoop("END_TALK")) {
					//RNLib::File().Scan(_RNC_File::SCAN::BOOL, &m_isLoop, "isLoop");
					//RNLib::File().Scan(_RNC_File::SCAN::FLOAT, &m_scale, "scale");
				}
			}
		}

		// ファイルを閉じる
		RNLib::File().CloseFile();
	}

	//読み込めた
	if (s_pTalk != NULL)
	{
		//会話開始
		m_bTalk = true;
	}
}

//=======================================
// 初期化処理
//=======================================
void CTalk::Init(void)
{

}

//=======================================
// 終了処理
//=======================================
void CTalk::Uninit(void)
{

}

//=======================================
// 更新処理
//=======================================
void CTalk::Update(void)
{

}

//=======================================
// 文章表示
//=======================================
void CTalk::PopUpText(void)
{

}

//=======================================
// 次の文字を表示
//=======================================
void CTalk::NextChar(void)
{

}