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

//画面下部表示の位置・サイズ
const Pos3D CTalk::TYPE_UNDER_POS = Pos3D(230.0f, 700.0f, 0.0f);
CFloat CTalk::TYPE_UNDER_FONT_SIZE = 40.0f;

/*  カーテンの幅  */ CFloat CTalk::TYPE_CURTAIN_WIDTH = 1280.0f;
/* カーテンの高さ */ CFloat CTalk::TYPE_CURTAIN_HEIGHT = 40.0f;
/*上部カーテン位置*/ CFloat CTalk::TYPE_CURTAIN_OVER_BEHIND_POS_Y = -TYPE_CURTAIN_HEIGHT;
/*下部カーテン位置*/ CFloat CTalk::TYPE_CURTAIN_BOTTOM_BEHIND_POS_Y = 720.0f + TYPE_CURTAIN_HEIGHT;
/* フォントサイズ */ CFloat CTalk::TYPE_CURTAIN_FONT_SIZE = 35.0f;

/*アニメーションカウンター*/int CTalk::s_CurtainCounter = 0;

CTalk::Talk  *CTalk::s_pTalk = NULL;        //会話内容
CTalk::EVENT  CTalk::s_Event = EVENT::NONE; //イベント

short CTalk::s_1P_Voice = NONEDATA;
short CTalk::s_2P_Voice = NONEDATA;

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
	m_nAutoCounter = 0;    //自動進行カウンター
	m_bAuto = true;        //自動進行フラグ

	m_pText = NULL;
	m_pTextSub = NULL;
	m_pos = TYPE_UNDER_POS;

	SetFontOption(SHOWTYPE::Under);

	//会話ログ削除
	DeleteLog();
}

//=======================================
// デストラクタ
//=======================================
CTalk::~CTalk()
{
	s_CurtainCounter = 0;

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

	//ボイス読込
	if (s_1P_Voice == NONEDATA) s_1P_Voice = RNLib::Sound().Load("data\\SOUND\\SE\\Voice\\cos.wav");
	if (s_2P_Voice == NONEDATA) s_2P_Voice = RNLib::Sound().Load("data\\SOUND\\SE\\Voice\\mos.wav");

	s_Event = EVENT::NONE; //イベント
	m_bTalk = false;       //会話中かどうか
	m_nTalkNumAll = 0;     //最大会話数
	m_nTalkID = 0;         //会話番号
	m_nAutoCounter = 0;    //自動進行カウンター
	m_bAuto = true;        //自動進行フラグ
	s_CurtainCounter = 0;  //アニメーションカウンター
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

				//会話ログ・表示方法保管用
				char LogTmp[TXT_MAX] = {};
				int nTypeTmp = 0;
				while (RNLib::File().SearchLoop("END_TALK")) {
					RNLib::File().Scan(_RNC_File::SCAN::STRING, &LogTmp[0], "TALK");
					RNLib::File().Scan(_RNC_File::SCAN::INT,    &s_pTalk[nTalkCounter].TalkerID, "PLAYER");
					RNLib::File().Scan(_RNC_File::SCAN::INT,    &nTypeTmp, "TYPE");
				}

				//文字列代入
				CInt len = strlen(&LogTmp[0]) + 1;
				s_pTalk[nTalkCounter].pLog = new char[len];
				strcpy(&s_pTalk[nTalkCounter].pLog[0], &LogTmp[0]);

				//表示方法代入
				s_pTalk[nTalkCounter].type = (SHOWTYPE)nTypeTmp;

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
		if (Event == EVENT::BEFORE_DEPARTURE) 
			Stage::SetIsCutIn(true);

		//会話開始
		m_bTalk = true;
		m_nTalkID = -1; //会話番号

		NextSpeak();    //次に表示するテキスト
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
	RNLib::Text2D().Put(PRIORITY_TEXT, "あたらしいふぉんと", _RNC_Text::ALIGNMENT::LEFT, NONEDATA, Pos2D(200.0f, 200.0f), 0.0f);

	//会話終了
	if (s_pTalk == NULL)
		return;

	if (!m_bTalk)
	{
		SetCurtain();
		return;
	}

	//表示方法を適用する
	ShowType();

	//自動進行
	Auto();

	//会話スキップ
	if (RNLib::Input().GetTrigger(DIK_U, _RNC_Input::BUTTON::START) ||
		RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::START, 1))
	{
		Skip();
	}

	//次の会話へ  or  会話終了
	else if (RNLib::Input().GetTrigger(DIK_T, _RNC_Input::BUTTON::Y) ||
			 RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::Y, 1) ||
			 m_nAutoCounter >= AUTO_COUNTER)
	{
		//次の発言へ
		NextSpeak();
	}

	//テキストボックスの位置設定
	if (m_pText != NULL && s_pTalk[m_nTalkID].type == SHOWTYPE::Curtain)
	{
		SetCurtain(false);
		m_pText->SetTxtBoxPos(m_pos, false, true);
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
	if (m_pTextSub != NULL)
	{
		m_pTextSub->Uninit();
		m_pTextSub = NULL;
	}
}

//=======================================
//表示方法を適用する
//=======================================
void CTalk::ShowType(void)
{
	switch (s_pTalk[m_nTalkID].type)
	{
			//==========================
			// 画面下部に表示
		case SHOWTYPE::Under:
			m_pos = TYPE_UNDER_POS;
			break;

			//==========================
			// 画面上下に黒幕を用意してその上にセリフを表示
		case SHOWTYPE::Curtain:
			SetCurtain();
			break;
	}

	//フォント設定
	SetFontOption(s_pTalk[m_nTalkID].type);
}

//=======================================
//自動進行
//=======================================
void CTalk::Auto(void)
{
	// フラグがTRUEでカウンター増加
	if (m_bAuto && m_pText != NULL && m_pText->GetLetter())
		m_nAutoCounter++;

	// フラグ切替
	if (RNLib::Input().GetTrigger(DIK_Y, _RNC_Input::BUTTON::RIGHT_THUMB) ||
		RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::RIGHT_THUMB, 1))
		m_bAuto = !m_bAuto;

	// フラグがOFFでカウンタークリア
	if (!m_bAuto)
		m_nAutoCounter = 0;
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
		m_pText->SetTextSkip(true);
		m_bTalk = false;
	}
	else
	{
		//次の会話の最大文字数
		DeleteText();

		//フォント設定
		SetFontOption(s_pTalk[m_nTalkID].type);

		//サウンド設定　語り手番号がプレイヤー番号以外ならサウンド無し。　合致しているなら語り手番号代入
		CShort SeIdx = 
			s_pTalk[m_nTalkID].TalkerID < 0 || s_pTalk[m_nTalkID].TalkerID > 2 ? NONEDATA :
			s_pTalk[m_nTalkID].TalkerID % 2 == 0 ? s_1P_Voice : s_2P_Voice;

		m_pText = CFontText::Create(CFontText::BOX_NONE, m_pos, INITPOS2D, s_pTalk[m_nTalkID].pLog,
									CFont::FONT_WAKUWAKU, &m_pFont, false, false, NULL, NULL, SeIdx);

		//両方喋る場合
		if (s_pTalk[m_nTalkID].TalkerID == 2)
		{
			//2P設定
			m_pFont.col = GetTalkerColor(1);
			m_pos.y = GetTalkerPosY(1);
			m_pTextSub = CFontText::Create(CFontText::BOX_NONE, m_pos, INITPOS2D, s_pTalk[m_nTalkID].pLog,
										CFont::FONT_WAKUWAKU, &m_pFont, false, false, NULL, NULL, s_2P_Voice);
		}

		//テキストボックスの位置設定
		if (m_pText != NULL && s_pTalk[m_nTalkID].type == SHOWTYPE::Curtain)
		{
			SetCurtain(false);
			m_pText->SetTxtBoxPos(m_pos, false, true);
		}

		//自動進行カウンター
		m_nAutoCounter = 0;
	}
}

//=======================================
//会話スキップ
//=======================================
void CTalk::Skip(void)
{
	DeleteLog();
}

//=======================================
//フォント設定
//=======================================
void CTalk::SetFontOption(const SHOWTYPE& type)
{
	m_pFont.nAppearTime = 5; // 1文字目が表示されるまでの時間
	m_pFont.nStandTime = NEXT_POPUP_INTERVAL;  // 待機時間
	m_pFont.nEraseTime = 0;  // 消えるまでの時間

	switch (type) {
			//==========================
			// 画面下部に表示
		case SHOWTYPE::Under: m_pFont.fTextSize = TYPE_UNDER_FONT_SIZE; break;

			//==========================
			// 画面上下に黒幕を用意してその上にセリフを表示
		case SHOWTYPE::Curtain: 
			m_pFont.fTextSize = TYPE_CURTAIN_FONT_SIZE;

			if (m_pText != NULL)
			{
				//暗幕が表示しきるまで待機
				if (m_nTalkID == 0 && m_pText->GetPopCount() == 0)
					m_pFont.nAppearTime = TYPE_CURTAIN_APPEAR_INTERVAL;

				//出現時間を設定
				m_pText->SetAppearTime(m_pFont.nAppearTime);
			}
			break;
	}

	//色取得
	if (s_pTalk != NULL)
		m_pFont.col = GetTalkerColor(s_pTalk[m_nTalkID].TalkerID);
}

//=======================================
//暗幕設定
//=======================================
void CTalk::SetCurtain(const bool bSetCurtain)
{
	//カウンターの割合算出  ->  出現量算出
	CFloat rate = RNLib::Ease().Easing(EASE_TYPE::LINEAR, s_CurtainCounter, TYPE_CURTAIN_COUNTER);
	CFloat Move = rate * TYPE_CURTAIN_HEIGHT * 1.5f;

	//カウンター増減
	if (m_bTalk && s_CurtainCounter < TYPE_CURTAIN_COUNTER)
		s_CurtainCounter++;

	else if (!m_bTalk && 0 < s_CurtainCounter)
		s_CurtainCounter--;

	//画面・カーテン上下の中心位置
	CFloat CenterX = RNLib::Window().GetCenterX();
	CFloat CurtainOverPos = TYPE_CURTAIN_OVER_BEHIND_POS_Y + Move;
	CFloat CurtainBottomPos = TYPE_CURTAIN_BOTTOM_BEHIND_POS_Y - Move;
	const Color CURTAIN_COLOR = { 0,0,0,100 };

	//暗幕配置
	if (bSetCurtain)
	{
		//上の幕
		RNLib::Polygon2D().Put(PRIORITY_EFFECT, Pos2D(CenterX, CurtainOverPos), 0.0f)
			->SetSize(TYPE_CURTAIN_WIDTH, TYPE_CURTAIN_HEIGHT)
			->SetCol(CURTAIN_COLOR);

		//下の幕
		RNLib::Polygon2D().Put(PRIORITY_EFFECT, Pos2D(CenterX, CurtainBottomPos), 0.0f)
			->SetSize(TYPE_CURTAIN_WIDTH, TYPE_CURTAIN_HEIGHT)
			->SetCol(CURTAIN_COLOR);
	}

	//カウンターが０でセリフ削除
	if (s_CurtainCounter == 0)
	{
		DeleteLog();
		return;
	}

	//表示座標取得
	m_pos.y = GetTalkerPosY(s_pTalk[m_nTalkID].TalkerID);
}

//=======================================
//語り手別セリフカラー取得
//=======================================
Color CTalk::GetTalkerColor(CInt &Talker)
{
	// プレイヤーカラー設定
	if (Talker == 0 || Talker == 2)
		return CPlayer::P1_COLOR;
	else if (Talker == 1)
		return CPlayer::P2_COLOR;

	// 違えば白に
	else return COLOR_WHITE;
}

//=======================================
//語り手別セリフ表示座標取得
//=======================================
float CTalk::GetTalkerPosY(CInt &Talker)
{
	if (Talker <= 0 || Talker <= 2)
	{
		switch ((int)CPlayer::GetInfo(Talker)->side)
		{
			case (int)CPlayer::WORLD_SIDE::FACE:
			case 2:
				return TYPE_CURTAIN_OVER_BEHIND_POS_Y + TYPE_CURTAIN_HEIGHT * 1.5f; break;

			case (int)CPlayer::WORLD_SIDE::BEHIND:
				return TYPE_CURTAIN_BOTTOM_BEHIND_POS_Y - TYPE_CURTAIN_HEIGHT * 1.5f; break;
		}
	}

	//現在地を返す
	return m_pos.y;
}