//========================================
// 
// レール3Dエディターの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"
#include "../RNmode.h"
#include "rail3D-editor.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]レール3Dエディタークラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CRail3DEditor::CRail3DEditor() : m_camera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight())), m_rail3D("NONEDATA"), m_editPath("NONEDATA") {

	m_camera.SetBGCol       (Color{ 128,128,128,255 });
	m_camera.SetGrabAirMouse(1.0f, 1.0f, 1.0f);
	m_camera.SetPosVAndPosR (Pos3D(0.0f, 100.0f, -100.0f), INITPOS3D);
	m_camera.SetLightID     ((new CLight3D("RNData\\StandardLight.txt"))->GetID());

	m_counter        = 0;
	m_time           = 60;
	m_selectPointIdx = 0;
	m_isSaved        = true;
}

//========================================
// デストラクタ
//========================================
CRail3DEditor::~CRail3DEditor() {

}

//========================================
// 初期化処理
//========================================
void CRail3DEditor::Init(void) {

}

//========================================
// 終了処理
//========================================
void CRail3DEditor::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CRail3DEditor::Update(void) {

	// [[[ カメラ描画 ]]]
	RNLib::Polygon2D().Put(0, RNLib::Window().GetCenterPos(), 0.0f, true)
		->SetTex(&m_camera)
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

	{// [[[ 操作 ]]]
		// レール3Dを新しく作成
		if (RNLib::Input().GetKeyPress(DIK_1)) {
			if (!m_isSaved) {
				if (RNLib::Window().Message_OKCANCEL(String("ドキュメントの変更が保存されていません。\n上書きしてよろしいですか？"), "警告")) {
					m_rail3D.Clear();
					m_isSaved = true;
				}
			}
			else {
				m_rail3D.Clear();
				m_isSaved = true;
			}
		}

		// レール3Dの読み込み
		if (RNLib::Input().GetKeyPress(DIK_2)) {
			if (RNLib::File().GetSelectOpenFileName(&m_editPath.GetCharPointer(), "", ".txt")) {
				m_rail3D.Load(m_editPath);
				m_isSaved = true;
			}
		}

		// レール3Dの書き込み
		if (RNLib::Input().GetKeyPress(DIK_3)) {
			if (RNLib::File().GetSelectSaveFileName(&m_editPath.GetCharPointer(), "", ".txt")) {
				m_rail3D.Save(m_editPath);
				m_isSaved = true;
			}
		}

		// 時間の加算/減算
		if (RNLib::Count().GetCount() % 4 == 0) {
			if (RNLib::Input().GetKeyPress(DIK_T)) { m_time--; }
			if (RNLib::Input().GetKeyPress(DIK_Y)) { m_time++; }

			RNLib::Number().Clamp(&m_counter, SHRT_MAX, 0);
		}

		// ポイントの加算
		if (RNLib::Input().GetKeyTrigger(DIK_C)) {
			m_isSaved = false;
			m_rail3D.AddPoint();
			m_selectPointIdx = m_rail3D.GetPointNum() - 1;
		}

		// ポイントの減算
		if (RNLib::Input().GetKeyTrigger(DIK_Z) && m_rail3D.GetPointNum() > 0) {
			if (RNLib::Window().Message_OKCANCEL(String("リニアライト[%d]を削除します。よろしいですか？", m_selectPointIdx), "警告")) {
				m_isSaved = false;
				m_rail3D.SubPoint(m_selectPointIdx);
			}
		}

		if (m_rail3D.GetPointNum() > 0) {

			// ポイントの選択
			if (RNLib::Input().GetKeyTrigger(DIK_Q)) { m_selectPointIdx--; }
			if (RNLib::Input().GetKeyTrigger(DIK_E)) { m_selectPointIdx++; }
			RNLib::Number().LoopClamp(&m_selectPointIdx, m_rail3D.GetPointNum() - 1, 0);

			{// ポイントの移動
				Pos3D& point = m_rail3D.GetPoint(m_selectPointIdx);
				const float cameraAngle = m_camera.GetRot().y;
				float XZMoveAngle = 0.0f;
				bool  isXZMove    = true;

				if (RNLib::Input().GetKeyPress(DIK_W)) {
					if      (RNLib::Input().GetKeyPress(DIK_A)) { XZMoveAngle = D3DX_PI * -0.25f; }
					else if (RNLib::Input().GetKeyPress(DIK_D)) { XZMoveAngle = D3DX_PI * 0.25f;  }
					else                                        { XZMoveAngle = 0.0f;             }
				}
				else if (RNLib::Input().GetKeyPress(DIK_S)) {
					if      (RNLib::Input().GetKeyPress(DIK_A)) { XZMoveAngle = D3DX_PI * -0.75f; }
					else if (RNLib::Input().GetKeyPress(DIK_D)) { XZMoveAngle = D3DX_PI * 0.75f;  }
					else                                        { XZMoveAngle = D3DX_PI;          }
				}
				else if (RNLib::Input().GetKeyPress(DIK_A)) { XZMoveAngle = D3DX_PI * -0.5f; }
				else if (RNLib::Input().GetKeyPress(DIK_D)) { XZMoveAngle = D3DX_PI * 0.5f;  }
				else { isXZMove = false; }

				if (isXZMove) {
					point.x += (sinf(cameraAngle + XZMoveAngle) * 0.5f);
					point.z += (cosf(cameraAngle + XZMoveAngle) * 0.5f);
					m_isSaved = false;
				}

				if (RNLib::Input().GetKeyPress(DIK_R)) { point.y += 0.5f; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_F)) { point.y -= 0.5f; m_isSaved = false; }
			}
		}
	}

	// 矢印の描画
	RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(_RNC_DefaultData::MODEL::PYRAMID_ALLOW), m_rail3D.GetMtx((float)m_counter / m_time))
		->SetCol(Color(255, 0, 0, 255));

	if (m_rail3D.GetPointNum() >= 2)
	{// ラインの描画
		Matrix mtx      = m_rail3D.GetMtx(0.0f);
		Pos3D  oldPos   = RNLib::Matrix().ConvMtxToPos(mtx);
		float  oldAngle = RNLib::Matrix().ConvMtxToRot(mtx).y;

		const UShort divNum = (m_rail3D.GetPointNum() - 1) * 5;
		for (int cntDiv = 1; cntDiv <= divNum; cntDiv++) {
			mtx = m_rail3D.GetMtx((float)cntDiv / divNum);
			Pos3D pos = RNLib::Matrix().ConvMtxToPos(mtx);
			float angle = RNLib::Matrix().ConvMtxToRot(mtx).y;
			RNLib::Polygon3D().Put((UShort)RNMode::PRIORITY::UI3D, INITMATRIX)
				->SetVtxPos(
					Pos3D(pos.x + sinf(angle - D3DX_PI_HALF), pos.y, pos.z + cosf(angle - D3DX_PI_HALF)),
					Pos3D(pos.x + sinf(angle + D3DX_PI_HALF), pos.y, pos.z + cosf(angle + D3DX_PI_HALF)),
					Pos3D(oldPos.x + sinf(oldAngle - D3DX_PI_HALF), oldPos.y, oldPos.z + cosf(oldAngle - D3DX_PI_HALF)),
					Pos3D(oldPos.x + sinf(oldAngle + D3DX_PI_HALF), oldPos.y, oldPos.z + cosf(oldAngle + D3DX_PI_HALF)))
				->SetCol(Color(255, 0, 0, 100))
				->SetCullingMode(_RNC_DrawState::CULLING_MODE::BOTH_SIDES);

			oldPos = pos;
			oldAngle = angle;
		}
	}

	// ポイントの描画
	for (UShort cntPoint = 0; cntPoint < m_rail3D.GetPointNum(); cntPoint++) {
		RNLib::Text3D().Put((UShort)RNMode::PRIORITY::UI3D, String("%d", cntPoint), _RNC_Text::ALIGNMENT::CENTER, NONEDATA, RNLib::Matrix().ConvPosToMtx(m_rail3D.GetPoint(cntPoint)))
			->SetSize(Size2D(4.0f, 4.0f))
			->SetCol(cntPoint == m_selectPointIdx ? Color(255, 255, ((RNLib::Count().GetCount() % 30) / 30.0f) * 255, 255) : COLOR_WHITE)
			->SetZTest(false)
			->SetBillboard(true);
	}

	// カウンター処理
	m_counter++;
	RNLib::Number().LoopClamp(&m_counter, m_time, 0);
	
	// [[[ ログ表示 ]]]
	RNLib::Text2D().PutDebugLog(String("-----Information"));
	RNLib::Text2D().PutDebugLog(String("EditPath:"));
	RNLib::Text2D().PutDebugLog(String("Counter :%d/%d %d%%", m_counter, m_time, (int)(((float)m_counter / m_time) * 100.0f)));
	RNLib::Text2D().PutDebugLog(String("-----Control"));
	RNLib::Text2D().PutDebugLog(String("Rail3DCreateNew   [1]"));
	RNLib::Text2D().PutDebugLog(String("Rail3DLoad        [2]"));
	RNLib::Text2D().PutDebugLog(String("Rail3DSave        [3]"));
	RNLib::Text2D().PutDebugLog(String("Time              [T][Y]:%d", m_time));
	RNLib::Text2D().PutDebugLog(String("AddPointNum       [C]   :%d", m_rail3D.GetPointNum()));
	RNLib::Text2D().PutDebugLog(String("SubPoint          [Z]"));
	if (m_rail3D.GetPointNum() > 0) {
		RNLib::Text2D().PutDebugLog(String("SelectPointIdx    [Q][E]:%d", 0));
		RNLib::Text2D().PutDebugLog(String("SelectPointPos[WASD][RF]:%f %f %f",
			m_rail3D.GetPoint(m_selectPointIdx).x,
			m_rail3D.GetPoint(m_selectPointIdx).y,
			m_rail3D.GetPoint(m_selectPointIdx).z));
	}
}