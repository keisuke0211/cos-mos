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
CLight3DEditor::CLight3DEditor() {

	m_light = new CLight3D(NULL);

	m_camera = new CCamera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight()));
	m_camera->SetBGCol(Color{ 128,128,128,255 });
	m_camera->SetGrabAirMouse(1.0f, 1.0f, 1.0f);
	m_camera->SetPosVAndPosR(Pos3D(0.0f, 100.0f, -100.0f), INITPOS3D);
	m_camera->SetLightID(m_light->GetID());

	m_isLockSubLinearLight = true;
	m_selectLinearLightIdx = 0;
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
		{// ライト3Dの回転
			Rot3D& lightRot = m_light->GetRot();
			if (RNLib::Input().GetKeyPress(DIK_UP   ))lightRot.x += 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_DOWN ))lightRot.x -= 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_LEFT ))lightRot.y += 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_RIGHT))lightRot.y -= 0.02f;
			RNLib::Number().LoopClamp(&lightRot.x, D3DX_PI, -D3DX_PI);
			RNLib::Number().LoopClamp(&lightRot.y, D3DX_PI, -D3DX_PI);
		}

		// リニアライトの加算
		if (RNLib::Input().GetKeyTrigger(DIK_C)) {
			m_light->AddLinearLight();
		}
	}

	// [[[ カメラ描画 ]]]
	RNLib::Polygon2D().Put(0, RNLib::Window().GetCenterPos(), 0.0f, true)
		->SetTex(m_camera)
		->SetSize(RNLib::Window().GetWidth(), RNLib::Window().GetHeight());

	// [[[ グリッド描画 ]]]
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
			RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(_RNC_DefaultData::MODEL::LINEAR_LIGHT), RNLib::Matrix().MultiplyMtx(m_light->GetLinearLight(m_selectLinearLightIdx).rotMtx, lightMtx))
				->SetCol(m_light->GetLinearLight(m_selectLinearLightIdx).col);
		}
	}

	// [[[ ログ表示 ]]]
	RNLib::Text2D().PutDebugLog(String("-----Control-----"));
	RNLib::Text2D().PutDebugLog(String("Light3DCreateNew    [1]"));
	RNLib::Text2D().PutDebugLog(String("Light3DLoad         [2]"));
	RNLib::Text2D().PutDebugLog(String("Light3DSave         [3]"));
	RNLib::Text2D().PutDebugLog(String("Light3DRot          [DIR]       :PI*%.2f,PI*%.2f", m_light->GetRot().x / D3DX_PI, m_light->GetRot().y / D3DX_PI));
	RNLib::Text2D().PutDebugLog(String("LinearLightNum      [Z][C]      :%d", m_light->GetLinearLightNum()));
	RNLib::Text2D().PutDebugLog(String("IsLockSubLinearLight[X]         :%s", m_isLockSubLinearLight ? "TRUE" : "FALSE"));
	RNLib::Text2D().PutDebugLog(String("SelectLinearLightIdx[Q][E]      :%d", m_selectLinearLightIdx));
	if (m_light->GetLinearLightNum() > 0) {
		RNLib::Text2D().PutDebugLog(String("SelectLinearLightCol[RT][FG][VB]:%d %d %d",
			m_light->GetLinearLight(m_selectLinearLightIdx).col.r,
			m_light->GetLinearLight(m_selectLinearLightIdx).col.g,
			m_light->GetLinearLight(m_selectLinearLightIdx).col.b));
		const Rot3D rot = RNLib::Matrix().ConvMtxToRot(m_light->GetLinearLight(m_selectLinearLightIdx).rotMtx);
		RNLib::Text2D().PutDebugLog(String("SelectLinearLightRot[WASD]      :PI*%.2f,PI*%.2f", rot.x / D3DX_PI, rot.y / D3DX_PI));
	}
}