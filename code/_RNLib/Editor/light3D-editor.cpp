//========================================
// 
// ライト3Dエディターの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"
#include "../RNmode.h"
#include "light3D-editor.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]ライト3Dエディタークラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CLight3DEditor::CLight3DEditor() : m_editPath("NONEDATA") {

	m_light = new CLight3D(NULL);

	m_camera = new CCamera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight()));
	m_camera->SetGrabAirMouse(1.0f, 1.0f, 1.0f);
	m_camera->SetPosVAndPosR(Pos3D(0.0f, 100.0f, -100.0f), INITPOS3D);
	m_camera->SetLightID(m_light->GetID());

	m_selectLinearLightIdx = 0;
	m_isSaved              = true;
}

//========================================
// デストラクタ
//========================================
CLight3DEditor::~CLight3DEditor() {

}

//========================================
// 初期化処理
//========================================
void CLight3DEditor::Init(void) {

	// モデル設定
	RNLib::StaticMesh().SetModel((UShort)RNMode::PRIORITY::OBJECT3D, Pos3D(0.0f, 5.0f, 0.0f), Rot3D(0.0f, 0.0f, 0.0f), RNLib::DefaultData().GetModelIdx(_RNC_DefaultData::MODEL::CUBE), COLOR_WHITE);
	RNLib::StaticMesh().SetModel((UShort)RNMode::PRIORITY::OBJECT3D, Pos3D(0.0f, 15.0f, 0.0f), Rot3D(0.0f, 0.0f, 0.0f), RNLib::DefaultData().GetModelIdx(_RNC_DefaultData::MODEL::SPHERE), COLOR_WHITE);
}

//========================================
// 終了処理
//========================================
void CLight3DEditor::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CLight3DEditor::Update(void) {

	{// [[[ 操作 ]]]
		// ライト3Dを新しく作成
		if (RNLib::Input().GetKeyPress(DIK_1)) {
			if (!m_isSaved) {
				if (RNLib::Window().Message_OKCANCEL(String("ドキュメントの変更が保存されていません。\n上書きしてよろしいですか？"), "警告")) {
					m_light->Clear();
					m_isSaved = true;
				}
			}
			else {
				m_light->Clear();
				m_isSaved = true;
			}
		}

		// ライト3Dの読み込み
		if (RNLib::Input().GetKeyPress(DIK_2)) {
			if (RNLib::File().GetSelectOpenFileName(&m_editPath.GetCharPointer(), "", ".txt")) {
				m_light->Load(m_editPath);
				m_isSaved = true;
			}
		}

		// ライト3Dの書き込み
		if (RNLib::Input().GetKeyPress(DIK_3)) {
			if (RNLib::File().GetSelectSaveFileName(&m_editPath.GetCharPointer(), "", ".txt")) {
				m_light->Save(m_editPath);
				m_isSaved = true;
			}
		}

		// ライト3Dの向きリセット
		if (RNLib::Input().GetKeyPress(DIK_P)) {
			m_light->SetRot(INITROT3D);
		}
		else
		{// ライト3Dの回転
			Rot3D& lightRot = m_light->GetRot();
			if (RNLib::Input().GetKeyPress(DIK_UP   ))lightRot.x += 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_DOWN ))lightRot.x -= 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_LEFT ))lightRot.y += 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_RIGHT))lightRot.y -= 0.02f;
			RNLib::Number().LoopClamp(&lightRot.x, D3DX_PI, -D3DX_PI);
			RNLib::Number().LoopClamp(&lightRot.y, D3DX_PI, -D3DX_PI);
		}

		if (RNLib::Count().GetCount() % 3 == 0)
		{// 背景の色
			if (RNLib::Input().GetKeyPress(DIK_Y)) { m_light->GetBGCol().r--; m_isSaved = false; }
			if (RNLib::Input().GetKeyPress(DIK_U)) { m_light->GetBGCol().r++; m_isSaved = false; }
			if (RNLib::Input().GetKeyPress(DIK_H)) { m_light->GetBGCol().g--; m_isSaved = false; }
			if (RNLib::Input().GetKeyPress(DIK_J)) { m_light->GetBGCol().g++; m_isSaved = false; }
			if (RNLib::Input().GetKeyPress(DIK_N)) { m_light->GetBGCol().b--; m_isSaved = false; }
			if (RNLib::Input().GetKeyPress(DIK_M)) { m_light->GetBGCol().b++; m_isSaved = false; }

			// 制御
			RNLib::Number().LoopClamp(&m_light->GetBGCol().r, 255, 0);
			RNLib::Number().LoopClamp(&m_light->GetBGCol().g, 255, 0);
			RNLib::Number().LoopClamp(&m_light->GetBGCol().b, 255, 0);
		}

		// フォグのON/OFF
		if (RNLib::Input().GetKeyTrigger(DIK_X)) {
			m_isSaved = false;
			m_light->SetIsFog(!m_light->GetIsFog());
		}

		// フォグのスタート/エンド
		if (RNLib::Input().GetKeyPress(DIK_I) && m_light->GetFogInfo().start > 0) { m_light->GetFogInfo().start--; m_isSaved = false; }
		if (RNLib::Input().GetKeyPress(DIK_O))                                    { m_light->GetFogInfo().start++; m_isSaved = false; }
		if (RNLib::Input().GetKeyPress(DIK_K) && m_light->GetFogInfo().end > 0)   { m_light->GetFogInfo().end--  ; m_isSaved = false; }
		if (RNLib::Input().GetKeyPress(DIK_L))                                    { m_light->GetFogInfo().end++  ; m_isSaved = false; }
		RNLib::Number().Clamp(&m_light->GetFogInfo().start, USHRT_MAX, 0);
		RNLib::Number().Clamp(&m_light->GetFogInfo().end, USHRT_MAX, m_light->GetFogInfo().start);

		// リニアライトの加算
		if (RNLib::Input().GetKeyTrigger(DIK_C)) {
			m_isSaved = false;
			m_light->AddLinearLight();
		}

		// リニアライトの減算
		if (RNLib::Input().GetKeyTrigger(DIK_Z) && m_light->GetLinearLightNum() > 0) {
			if (RNLib::Window().Message_OKCANCEL(String("リニアライト[%d]を削除します。よろしいですか？", m_selectLinearLightIdx), "警告")) {
				m_isSaved = false;
				m_light->SubLinearLight(m_selectLinearLightIdx);
			}
		}

		// 選択リニアライトの加算
		if (RNLib::Input().GetKeyTrigger(DIK_Q)) {
			m_selectLinearLightIdx--;
		}

		// 選択リニアライトの減算
		if (RNLib::Input().GetKeyTrigger(DIK_E)) {
			m_selectLinearLightIdx++;
		}

		if (m_light->GetLinearLightNum() > 0) {
		
			// 選択リニアライトの制御
			RNLib::Number().LoopClamp(&m_selectLinearLightIdx, m_light->GetLinearLightNum() - 1, 0);

			if (RNLib::Count().GetCount() % 3 == 0)
			{// 選択リニアライトの色
				if (RNLib::Input().GetKeyPress(DIK_R)) { m_light->GetLinearLight(m_selectLinearLightIdx).col.r--; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_T)) { m_light->GetLinearLight(m_selectLinearLightIdx).col.r++; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_F)) { m_light->GetLinearLight(m_selectLinearLightIdx).col.g--; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_G)) { m_light->GetLinearLight(m_selectLinearLightIdx).col.g++; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_V)) { m_light->GetLinearLight(m_selectLinearLightIdx).col.b--; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_B)) { m_light->GetLinearLight(m_selectLinearLightIdx).col.b++; m_isSaved = false; }

				// 制御
				RNLib::Number().LoopClamp(&m_light->GetLinearLight(m_selectLinearLightIdx).col.r, 255, 0);
				RNLib::Number().LoopClamp(&m_light->GetLinearLight(m_selectLinearLightIdx).col.g, 255, 0);
				RNLib::Number().LoopClamp(&m_light->GetLinearLight(m_selectLinearLightIdx).col.b, 255, 0);
			}

			{// 選択リニアライトの回転
				Rot3D lightRot = RNLib::Matrix().ConvMtxToRot(m_light->GetLinearLight(m_selectLinearLightIdx).rotMtx);
				bool isSpin = false;
				if (RNLib::Input().GetKeyPress(DIK_W)) { lightRot.x += 0.02f; isSpin = true; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_S)) { lightRot.x -= 0.02f; isSpin = true; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_A)) { lightRot.y += 0.02f; isSpin = true; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_D)) { lightRot.y -= 0.02f; isSpin = true; m_isSaved = false; }
				RNLib::Number().LoopClamp(&lightRot.x, D3DX_PI, -D3DX_PI);
				RNLib::Number().LoopClamp(&lightRot.y, D3DX_PI, -D3DX_PI);
				m_light->GetLinearLight(m_selectLinearLightIdx).rotMtx = RNLib::Matrix().ConvRotToMtx(lightRot);
			}
		}
		else {
			m_selectLinearLightIdx = 0;
		}
	}

	// [[[ カメラ描画 ]]]
	RNLib::Polygon2D().Put(0, RNLib::Window().GetCenterPos(), 0.0f, true)
		->SetTex(m_camera)
		->SetSize(RNLib::Window().GetWidth(), RNLib::Window().GetHeight());

	// [[[ グリッド描画 ]]]
	if (m_light->GetIsFog()) {
		RNLib::Polygon3D().Put((UShort)RNMode::PRIORITY::STAGE3D, Pos3D(0.0f, -50.0f, 0.0f), Rot3D(D3DX_PI_HALF, 0.0f, 0.0f))
			->SetSize(Size2D(100000.0f, 100000.0f))
			->SetCol(Color(0, 0, 0, 255));
	}
	RNLib::Polygon3D().Put((UShort)RNMode::PRIORITY::STAGE3D, INITPOS3D, Rot3D(D3DX_PI_HALF, 0.0f, 0.0f))
		->SetSize(Size2D(90.0f, 90.0f))
		->SetTexUV(
			RNLib::DefaultData().GetTextureIdx(_RNC_DefaultData::TEXTURE::GRID),
			Pos2D(0.0f, 0.0f),
			Pos2D(9.0f, 0.0f),
			Pos2D(0.0f, 9.0f),
			Pos2D(9.0f, 9.0f));
	
	{
		const float  dist = 50.0f;
		const Pos3D  lightRot = m_light->GetRot();
		const Pos3D  lightPos = -RNLib::Geometry().FindRotVec(lightRot) * dist;
		const Matrix lightMtx = RNLib::Matrix().ConvPosRotToMtx(lightPos, lightRot);

		// [[[ ライト3D描画 ]]]
		RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(_RNC_DefaultData::MODEL::SPHERE), lightPos, lightRot, Scale3D(0.25f, 0.25f, 0.25f))
			->SetLighting(false);

		// [[[ リニアライト描画 ]]]
		for (int cntLinearLight = 0; cntLinearLight < m_light->GetLinearLightNum(); cntLinearLight++) {
			RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(_RNC_DefaultData::MODEL::LINEAR_LIGHT), RNLib::Matrix().MultiplyMtx(m_light->GetLinearLight(cntLinearLight).rotMtx, lightMtx))
				->SetCol(m_light->GetLinearLight(cntLinearLight).col.GetAlphaApplied(m_selectLinearLightIdx == cntLinearLight ? 1.0f : 0.5f));
		}
	}

	// [[[ ログ表示 ]]]
	RNLib::Text2D().PutDebugLog(String("-----Information"));
	RNLib::Text2D().PutDebugLog(String("EditPath:"));
	RNLib::Text2D().PutDebugLog(String("%s", m_editPath));
	RNLib::Text2D().PutDebugLog(String("-----Control-----"));
	RNLib::Text2D().PutDebugLog(String("Light3DCreateNew    [1]"));
	RNLib::Text2D().PutDebugLog(String("Light3DLoad         [2]"));
	RNLib::Text2D().PutDebugLog(String("Light3DSave         [3]"));
	RNLib::Text2D().PutDebugLog(String("Light3DRot          [DIR]       :%.2fPI,%.2fPI", m_light->GetRot().x / D3DX_PI, m_light->GetRot().y / D3DX_PI));
	RNLib::Text2D().PutDebugLog(String("Light3DRotReset     [P]"));
	RNLib::Text2D().PutDebugLog(String("BackGroundCol       [YU][HJ][NM]:%d %d %d",
		m_light->GetBGCol().r,
		m_light->GetBGCol().g,
		m_light->GetBGCol().b));
	RNLib::Text2D().PutDebugLog(String("IsFog               [X]         :%s", m_light->GetIsFog() ? "TRUE" : "FALSE"));
	RNLib::Text2D().PutDebugLog(String("FogStart            [I][O]      :%d", m_light->GetFogInfo().start));
	RNLib::Text2D().PutDebugLog(String("FogEnd              [K][L]      :%d", m_light->GetFogInfo().end));
	RNLib::Text2D().PutDebugLog(String("AddLinearLightNum   [C]         :%d", m_light->GetLinearLightNum()));
	RNLib::Text2D().PutDebugLog(String("SubLinearLight      [Z]"));
	if (m_light->GetLinearLightNum() > 0) {
		RNLib::Text2D().PutDebugLog(String("SelectLinearLightIdx[Q][E]      :%d", m_selectLinearLightIdx));
		RNLib::Text2D().PutDebugLog(String("SelectLinearLightCol[RT][FG][VB]:%d %d %d",
			m_light->GetLinearLight(m_selectLinearLightIdx).col.r,
			m_light->GetLinearLight(m_selectLinearLightIdx).col.g,
			m_light->GetLinearLight(m_selectLinearLightIdx).col.b));
		const Rot3D rot = RNLib::Matrix().ConvMtxToRot(m_light->GetLinearLight(m_selectLinearLightIdx).rotMtx);
		RNLib::Text2D().PutDebugLog(String("SelectLinearLightRot[WASD]      :%.2fPI,%.2fPI", rot.x / D3DX_PI, rot.y / D3DX_PI));
	}
}