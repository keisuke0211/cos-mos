//========================================
// 
// 光3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
// [[[ light3D.cpp ]]]
//========================================
#include "../../RNlib.h"

//****************************************
// マクロ定義
//****************************************
#define AMBIENTLIGHTDATA_PATH "RNData\\AmbientLight.txt"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CLight3Dクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
// Author:RIKU NISHIMURA
//========================================
CLight3D::CLight3D() {
	m_pAmbient = NULL;
	m_col      = INITCOLOR;
	m_nSetNum  = 0;
}

//========================================
// デストラクタ
// Author:RIKU NISHIMURA
//========================================
CLight3D::~CLight3D() {

}

//========================================
// 初期化処理
// Author:RIKU NISHIMURA
//========================================
void CLight3D::Init(void) {

	if (!RNLib::File().OpenLoadFile(AMBIENTLIGHTDATA_PATH))
	{// 環境光データファイルが開けなかった場合、処理を終了する
		RNLib::Window().Message("環境光データファイルが開けませんでした。", "失敗");
		return;
	}

	// 読み込みループ
	while (RNLib::File().SearchLoop("END")) {
		if (false) {}
		else if (RNLib::File().CheckIdentifier("LIGHTSET{")) {
			// 設定数を読み込み、その分環境光設定情報のメモリを確保する
			RNLib::File().Scan(CFile::SCAN::INT,&m_nSetNum);
			m_pAmbient = new AmbientLight[m_nSetNum];

			// 光情報のカウント
			int cntLight = 0;

			while (RNLib::File().SearchLoop("}")) {
				if (false) {}
				else if (RNLib::File().CheckIdentifier("LIGHT{")) {
					m_pAmbient[cntLight] = {};

					while (RNLib::File().SearchLoop("}")) {
						if (false) {}
						else if (RNLib::File().CheckIdentifier("ROT:"))        { RNLib::File().Scan(CFile::SCAN::POS3D, &m_pAmbient[cntLight].rot); }
						else if (RNLib::File().CheckIdentifier("BRIGHTNESS:")) { RNLib::File().Scan(CFile::SCAN::FLOAT, &m_pAmbient[cntLight].fBrightness); }
					}

					// カウントの光を設定する
					SetLight(cntLight);

					cntLight++;	// カウントを加算
				}
			}
		}
		/*else if (RNLib::File().CheckIdentifier("FOGSET{")) {
			while (RNLib::File().SearchLoop("}")) {
				if (false) {}
				else if (RNLib::File().CheckIdentifier("DIST_START:")) { 
					float fDistStart = 0.0f;
					RNLib::File().Scan(CFile::SCAN::FLOAT, &fDistStart);
					RNLib::Fog().SetDistStart(fDistStart);
				}
				else if (RNLib::File().CheckIdentifier("DIST_END:"  )) {
					float fDistEnd = 0.0f;
					RNLib::File().Scan(CFile::SCAN::FLOAT, &fDistEnd);
					RNLib::Fog().SetDistEnd(fDistEnd);
				}
			}
		}*/
	}

	// ファイルを閉じる
	RNLib::File().CloseFile();
}

//========================================
// 環境光設定情報を保存
// Author:RIKU NISHIMURA
//========================================
void CLight3D::SaveAmbientLight(void) {
	
	if (!RNLib::File().OpenSaveFile(AMBIENTLIGHTDATA_PATH))
	{// 環境光データファイルが開けなかった場合、処理を終了する
		RNLib::Window().Message("環境光データファイルが開けませんでした。", "失敗");
		return;
	}

	FILE* pFile = RNLib::File().GetFile();
	fprintf(pFile, "LIGHTSET{ %d\n", m_nSetNum);
	for (int cntLight = 0; cntLight < m_nSetNum; cntLight++) {
		fprintf(pFile, "	LIGHT{\n");
		fprintf(pFile, "		ROT: %f %f %f\n", m_pAmbient[cntLight].rot.x, m_pAmbient[cntLight].rot.y, m_pAmbient[cntLight].rot.z);
		fprintf(pFile, "		BRIGHTNESS: %f\n", m_pAmbient[cntLight].fBrightness);
		fprintf(pFile, "	}\n");
	}
	fprintf(pFile, "}\n");

	fprintf(pFile, "FOGSET{\n");
	/*fprintf(pFile, "	DIST_START: %f\n", RNLib::Fog().GetDistStart());
	fprintf(pFile, "	DIST_END: %f\n", RNLib::Fog().GetDistEnd());*/
	fprintf(pFile, "}\n");

	fprintf(pFile, "END\n");

	// ファイルを閉じる
	RNLib::File().CloseFile();

	// 保存完了メッセージ
	RNLib::Window().Message("環境光データファイルを保存しました。", "");
}

//========================================
// 終了処理
// Author:RIKU NISHIMURA
//========================================
void CLight3D::Uninit(void) {
	// 環境光設定情報の破棄
	delete[] m_pAmbient;
	m_pAmbient = NULL;
}

//========================================
// 更新処理
// Author:RIKU NISHIMURA
//========================================
void CLight3D::Update(void) {

}

//========================================
// 色設定処理
// Author:RIKU NISHIMURA
//========================================
void CLight3D::SetCol(Color col) {
	m_col = col;

	for (int cntLight = 0; cntLight < m_nSetNum; cntLight++) {
		SetLight(cntLight);
	}
}

//========================================
// 数設定
// Author:RIKU NISHIMURA
//========================================
void CLight3D::SetNum(int nNum) {
	if (m_nSetNum == nNum)
		return;

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = RNLib::Window().GetD3DDevice();

	// もう使用されない光を全て無効にする
	for (int cntLight = nNum; cntLight < m_nSetNum; cntLight++) {
		pDevice->LightEnable(cntLight, FALSE);
	}
	
	// 過去の数として保存しておく
	int nNumOld = m_nSetNum;

	// 設定数を代入
	m_nSetNum = nNum;

	{// 環境光のメモリ再確保
		AmbientLight* pNew = new AmbientLight[m_nSetNum];
		int nCopySize = m_nSetNum < nNumOld ? m_nSetNum : nNumOld;
		if (nCopySize > 0) {
			memcpy(pNew, m_pAmbient, sizeof(AmbientLight) * nCopySize);
			delete[] m_pAmbient;
		}
		m_pAmbient = pNew;
	}
}

//========================================
// 光を設定する
// Author:RIKU NISHIMURA
//========================================
void CLight3D::SetLight(int nIdx) {
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = RNLib::Window().GetD3DDevice();

	// 光3Dの情報をクリアし、種類を設定
	D3DLIGHT9 light3D;
	ZeroMemory(&light3D, sizeof(D3DLIGHT9));
	light3D.Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光と方向を設定
	light3D.Diffuse = ColorToD3DXCOLOR(BrightnessToColor(m_col, m_pAmbient[nIdx].fBrightness));
	light3D.Direction = CGeometry::FindRotVec(m_pAmbient[nIdx].rot);

	// 光の情報を設定し、有効にする
	pDevice->SetLight(nIdx, &light3D);
	pDevice->LightEnable(nIdx, TRUE);
}