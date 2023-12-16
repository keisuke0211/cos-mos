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

	// 光を全て無効にする
	for (int cntLinearLight = 0; cntLinearLight < ms_linearLightNumOld; device->LightEnable(cntLinearLight, FALSE), cntLinearLight++);
	ms_linearLightNumOld = 0;
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

	m_linearLights   = NULL;
	m_linearLightNum = 0;
	m_rot            = INITROT3D;
	m_col            = COLOR_WHITE;

	// 読み込み処理
	if (loadPath != NULL)
		Load(loadPath);
}

//========================================
// デストラクタ
//========================================
CLight3D::~CLight3D() {

	// リニアライトを解放
	RNLib::Memory().Release(&m_linearLights);

	// リストから削除
	RNSystem::GetLight3DMgr().SubList(this);
}

//========================================
// 読み込み処理
//========================================
void CLight3D::Load(const String& loadPath) {

	// ファイルを開く
	if (RNLib::File().OpenLoadFile(loadPath, "Light3DFile")) {
		while (RNLib::File().SearchLoop("END")) {
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
		}

		// ファイルを閉じる
		RNLib::File().CloseFile();
	}
}

//========================================
// セッティング処理
//========================================
void CLight3D::Setting(Device& device) {

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
			light3D.Direction = RNLib::Geometry().FindRotVec(RNLib::Matrix().ConvMtxToRot(RNLib::Matrix().MultiplyMtx(m_linearLights[cntLinearLight].rotMtx, rotMtx)));

			// 光の情報を設定し、有効にする
			device->SetLight(cntLinearLight, &light3D);
			device->LightEnable(cntLinearLight, TRUE);
		}

		// 過去の数として保存しておく
		ms_linearLightNumOld = m_linearLightNum;
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