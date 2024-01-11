//========================================
// 
// ライト3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]ライト3Dクラス
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 静的変数定義
//****************************************
short  CLight3D::ms_IDCount           = 0;
UShort CLight3D::ms_linearLightNumOld = 0;

//========================================
// [静的]全て無効にする
//========================================
void CLight3D::AllDisable(Device& device) {

	// 画面のクリア
	device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);

	// 光を全て無効にする
	for (int cntLinearLight = 0; cntLinearLight < ms_linearLightNumOld; device->LightEnable(cntLinearLight, FALSE), cntLinearLight++);
	ms_linearLightNumOld = 0;

	// フォグを使用しない
	RNLib::DrawStateMgr().SetIsFog(device, false);
}

//========================================
// コンストラクタ
//========================================
CLight3D::CLight3D(const String& loadPath) {

	// リストに追加
	RNSystem::GetLight3DMgr().AddList(this);

	// IDを設定
	m_ID       = ms_IDCount;
	ms_IDCount = (ms_IDCount + 1) % SHRT_MAX;

	// クリア処理
	m_linearLights = NULL;
	m_fogInfo      = {};
	Clear();

	// 読み込み処理
	if (loadPath != NULL)
		Load(loadPath);
}

//========================================
// デストラクタ
//========================================
CLight3D::~CLight3D() {

	// クリア処理
	Clear();

	// リストから削除
	RNSystem::GetLight3DMgr().SubList(this);
}

//========================================
// クリア処理
//========================================
void CLight3D::Clear(void) {

	RNLib::Memory().Release(&m_linearLights);
	m_linearLightNum = 0;
	m_isFog          = false;
	m_fogInfo        = {};
	m_BGCol          = COLOR_BLACK;
	m_rot            = INITROT3D;
	m_col            = COLOR_WHITE;
}

//========================================
// 読み込み処理
//========================================
void CLight3D::Load(const String& loadPath) {

	// ファイルを開く
	if (RNLib::File().OpenLoadFile(loadPath, "Light3DFile")) {
		while (RNLib::File().SearchLoop("END")) {
			RNLib::File().Scan(_RNC_File::SCAN::COLOR, &m_BGCol, "BGCol");
			RNLib::File().Scan(_RNC_File::SCAN::BOOL , &m_isFog, "isFog");

			if (RNLib::File().CheckIdentifier("linearLights{")) {
				RNLib::File().Scan(_RNC_File::SCAN::USHORT, &m_linearLightNum);
				RNLib::Memory().Alloc(&m_linearLights, m_linearLightNum);

				UShort linearLightCount = 0;
				while (RNLib::File().SearchLoop("}")) {
					if (RNLib::File().CheckIdentifier("linearLight{")) {
						Rot3D rot = INITROT3D;

						while (RNLib::File().SearchLoop("}")) {
							RNLib::File().Scan(_RNC_File::SCAN::ROT3D, &rot, "rot");
							RNLib::File().Scan(_RNC_File::SCAN::COLOR, &m_linearLights[linearLightCount].col, "col");
						}
						m_linearLights[linearLightCount].rotMtx = RNLib::Matrix().ConvRotToMtx(rot);

						linearLightCount++;
					}
				}
			}
			else if (RNLib::File().CheckIdentifier("fogInfo{")) {
				while (RNLib::File().SearchLoop("}")) {
					RNLib::File().Scan(_RNC_File::SCAN::USHORT, &m_fogInfo.start, "start");
					RNLib::File().Scan(_RNC_File::SCAN::USHORT, &m_fogInfo.end, "end");
				}
			}
		}

		// ファイルを閉じる
		RNLib::File().CloseFile();
	}
}

//========================================
// 書き込み処理
//========================================
void CLight3D::Save(const String& savePath) {

	// ファイルを開く
	if (RNLib::File().OpenSaveFile(savePath)) {
		fprintf(RNLib::File().GetFile(), "Light3DFile\n");
		fprintf(RNLib::File().GetFile(), "isFog %d\n", m_isFog);
		fprintf(RNLib::File().GetFile(), "BGCol %d %d %d %d\n", m_BGCol.r, m_BGCol.g, m_BGCol.b, m_BGCol.a);
		fprintf(RNLib::File().GetFile(), "linearLights{ %d\n", m_linearLightNum);
		for (int cntLinearLight = 0; cntLinearLight < m_linearLightNum; cntLinearLight++) {
			fprintf(RNLib::File().GetFile(), "	linearLight{\n");
			const Rot3D rot = RNLib::Matrix().ConvMtxToRot(m_linearLights[cntLinearLight].rotMtx);
			fprintf(RNLib::File().GetFile(), "		rot %f %f %f\n", rot.x, rot.y, rot.z);
			fprintf(RNLib::File().GetFile(), "		col %d %d %d %d\n", m_linearLights[cntLinearLight].col.r, m_linearLights[cntLinearLight].col.g, m_linearLights[cntLinearLight].col.b, m_linearLights[cntLinearLight].col.a);
			fprintf(RNLib::File().GetFile(), "	}\n");
		}
		fprintf(RNLib::File().GetFile(), "}\n");
		fprintf(RNLib::File().GetFile(), "fogInfo{\n");
		fprintf(RNLib::File().GetFile(), "	start %d\n", m_fogInfo.start);
		fprintf(RNLib::File().GetFile(), "	end %d\n", m_fogInfo.end);
		fprintf(RNLib::File().GetFile(), "}\n");
		fprintf(RNLib::File().GetFile(), "END");

		// ファイルを閉じる
		RNLib::File().CloseFile();
	}
}

//========================================
// セッティング処理
//========================================
void CLight3D::Setting(Device& device, const Color* ovrCol) {

	// 画面のクリア
	device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), ovrCol == NULL ? m_BGCol : *ovrCol, 1.0f, 0);

	{// リニアライトの設定
		// もう使用されない光を全て無効にする
		for (int cntLinearLight = m_linearLightNum; cntLinearLight < ms_linearLightNumOld; device->LightEnable(cntLinearLight, FALSE), cntLinearLight++);

		// 光3Dの情報をクリアし、種類を設定
		D3DLIGHT9 light3D;
		ZeroMemory(&light3D, sizeof(D3DLIGHT9));
		light3D.Type = D3DLIGHT_DIRECTIONAL;

		// 向きマトリックスを算出
		const Matrix rotMtx = RNLib::Matrix().ConvRotToMtx(m_rot);

		for (UShort cntLinearLight = 0; cntLinearLight < m_linearLightNum; cntLinearLight++) {

			// 拡散光と方向を設定
			light3D.Diffuse   = m_linearLights[cntLinearLight].col.GetMixed(m_col);
			light3D.Direction = RNLib::Matrix().ConvMtxToRotVec(RNLib::Matrix().MultiplyMtx(rotMtx, m_linearLights[cntLinearLight].rotMtx));

			// 光の情報を設定し、有効にする
			device->SetLight(cntLinearLight, &light3D);
			device->LightEnable(cntLinearLight, TRUE);
		}

		// 過去の数として保存しておく
		ms_linearLightNumOld = m_linearLightNum;
	}

	// フォグの設定
	if (m_isFog) {
		RNLib::DrawStateMgr().SetIsFog(device, true);
		RNLib::DrawStateMgr().SetFogParameter(device, m_BGCol, m_fogInfo.start, m_fogInfo.end);
	}
	else {
		RNLib::DrawStateMgr().SetIsFog(device, false);
	}
}

//========================================
// リニアライトを加算
//========================================
void CLight3D::AddLinearLight(void) {

	const UShort oldNum = m_linearLightNum++;
	RNLib::Memory().ReAlloc(&m_linearLights, oldNum, m_linearLightNum);
	m_linearLights[oldNum] = {};
	m_linearLights[oldNum].rotMtx = RNLib::Matrix().ConvRotToMtx(INITROT3D);
}

//========================================
// リニアライトを減算
//========================================
void CLight3D::SubLinearLight(const UShort& idx) {

	if (idx < 0 || idx >= m_linearLightNum)
		return;

	for (UShort cntLinearLight = idx; cntLinearLight < m_linearLightNum - 1; cntLinearLight++) {
		m_linearLights[cntLinearLight] = m_linearLights[cntLinearLight + 1];
	}
	const UShort oldNum = m_linearLightNum--;
	RNLib::Memory().ReAlloc(&m_linearLights, oldNum, m_linearLightNum);
}