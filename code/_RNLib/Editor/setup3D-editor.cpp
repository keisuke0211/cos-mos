//========================================
// 
// セットアップ3Dエディターの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"
#include "../RNmode.h"
#include "setup3D-editor.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]セットアップ3Dエディタークラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CSetUp3DEditor::CSetUp3DEditor() {

	m_camera = new CCamera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight()));
	m_camera->SetGrabAirMouse(1.0f, 1.0f, 1.0f);
	m_camera->SetPosVAndPosR(Pos3D(0.0f, 100.0f, -100.0f), INITPOS3D);
	m_camera->SetLightID((new CLight3D("RNData\\StandardLight.txt"))->GetID());

	m_doll = new CDoll3D((UShort)RNMode::PRIORITY::OBJECT3D, EDITDATA);
	m_doll->SetPos(Pos3D(0.0f, 0.0f, 0.0f));
	RNLib::Doll3DMgr().SetEditDoll(m_doll);
	RNLib::Doll3DMgr().SetEditCamera(m_camera);
	RNLib::Doll3DMgr().SetEditDollDrawModelVtxIdxNum(10);

	m_slither        = INITVECTOR3D;
	m_slitherCounter = 0;
	m_editPath       = NULL;
	m_motionPath     = NULL;
	m_messageType    = MESSAGE_TYPE::NONE;
	m_messageCounter = 0;
}

//========================================
// デストラクタ
//========================================
CSetUp3DEditor::~CSetUp3DEditor() {

	RNLib::Memory().Release(&m_camera);
	RNLib::Memory().Release(&m_doll);
	RNLib::Memory().Release(&m_editPath);
}

//========================================
// 初期化処理
//========================================
void CSetUp3DEditor::Init(void) {

}

//========================================
// 終了処理
//========================================
void CSetUp3DEditor::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CSetUp3DEditor::Update(void) {

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

	// 現在のボーン番号を取得
	short boneIdx = RNLib::Doll3DMgr().GetEditDollDrawModelVtxIdxBoneIdx();

	{// [[[ 操作 ]]]
		// 新規作成
		if (RNLib::Input().GetKeyTrigger(DIK_1)) {
			RNLib::Memory().Release(&m_editPath);
			if (RNLib::File().GetSelectSaveFileName(&m_editPath, "", ".txt")) {
				RNLib::SetUp3D().LoadEditData("RNData\\SetUp3D\\Capsule.txt");
				RNLib::SetUp3D().SaveEditData(m_editPath);
			}
		}

		// 読み込み
		if (RNLib::Input().GetKeyTrigger(DIK_2)) {
			RNLib::Memory().Release(&m_editPath);
			if (RNLib::File().GetSelectOpenFileName(&m_editPath, "", ".txt")) {
				RNLib::SetUp3D().LoadEditData(m_editPath);
			}
		}
		
		// 再読み込み
		if (RNLib::Input().GetKeyTrigger(DIK_3)) {
			if (m_editPath != NULL) {
				m_messageType = RNLib::SetUp3D().LoadEditData(m_editPath) ?
					MESSAGE_TYPE::SETUP_RELOAD_SUCCEEDED:
					MESSAGE_TYPE::SETUP_RELOAD_FAILED;
			}
			else {
				m_messageType = MESSAGE_TYPE::SETUP_RELOAD_FAILED;
			}
			m_messageCounter = 30;
		}

		// 新規作成
		if (RNLib::Input().GetKeyTrigger(DIK_4)) {
			RNLib::Memory().Release(&m_motionPath);
			if (RNLib::File().GetSelectSaveFileName(&m_motionPath, "", ".txt")) {
				RNLib::Motion3D().LoadEditData("RNData\\Motion3D\\CapsuleStretch.txt");
				RNLib::Motion3D().SaveEditData(m_motionPath);
			}
		}

		// 読み込み
		if (RNLib::Input().GetKeyTrigger(DIK_5)) {
			RNLib::Memory().Release(&m_motionPath);
			if (RNLib::File().GetSelectOpenFileName(&m_motionPath, "", ".txt")) {
				RNLib::Motion3D().LoadEditData(m_motionPath);
			}
		}

		// 再読み込み
		if (RNLib::Input().GetKeyTrigger(DIK_6)) {
			if (m_motionPath != NULL) {
				m_messageType = RNLib::Motion3D().LoadEditData(m_motionPath) ?
					MESSAGE_TYPE::MOTION_RELOAD_SUCCEEDED :
					MESSAGE_TYPE::MOTION_RELOAD_FAILED;
			}
			else {
				m_messageType = MESSAGE_TYPE::MOTION_RELOAD_FAILED;
			}
			m_messageCounter = 30;
		}
		
		// 頂点番号の描画ON/OFF
		if (RNLib::Input().GetKeyTrigger(DIK_7)) {
			RNLib::Doll3DMgr().SetEditDollIsDrawModelVtxIdx(!RNLib::Doll3DMgr().GetEditDollIsDrawModelVtxIdx());
		}

		// くねくね値
		if (RNLib::Input().GetKeyTrigger(DIK_Q))
			m_slither.x -= 0.1f;
		if (RNLib::Input().GetKeyTrigger(DIK_W))
			m_slither.x += 0.1f;
		if (RNLib::Input().GetKeyTrigger(DIK_A))
			m_slither.y -= 0.1f;
		if (RNLib::Input().GetKeyTrigger(DIK_S))
			m_slither.y += 0.1f;
		if (RNLib::Input().GetKeyTrigger(DIK_Z))
			m_slither.z -= 0.1f;
		if (RNLib::Input().GetKeyTrigger(DIK_X))
			m_slither.z += 0.1f;

		if (false) {}
		else if (m_slither.x < 0.0f) m_slither.x = 0.0f;
		else if (m_slither.x > 1.0f) m_slither.x = 1.0f;
		else if (m_slither.y < 0.0f) m_slither.y = 0.0f;
		else if (m_slither.y > 1.0f) m_slither.y = 1.0f;
		else if (m_slither.z < 0.0f) m_slither.z = 0.0f;
		else if (m_slither.z > 1.0f) m_slither.z = 1.0f;

		{// 頂点番号の描画ボーン番号切り替え
			if (RNLib::Input().GetKeyTrigger(DIK_E)) {
				if (--boneIdx < 0) {
					boneIdx = RNLib::SetUp3D().GetData(EDITDATA).m_boneDataNum - 1;
				}
			}
			else if (RNLib::Input().GetKeyTrigger(DIK_R)) {
				boneIdx = (boneIdx + 1) % RNLib::SetUp3D().GetData(EDITDATA).m_boneDataNum;
			}

			RNLib::Doll3DMgr().SetEditDollDrawModelVtxIdxBoneIdx(boneIdx);
		}

		{// 頂点番号の描画数
			short drawVtxNum = RNLib::Doll3DMgr().GetEditDollDrawModelVtxIdxNum();

			if (RNLib::Input().GetKeyTrigger(DIK_D)) {
				if (--drawVtxNum < NONEDATA)
					drawVtxNum = NONEDATA;
			}
			else if (RNLib::Input().GetKeyTrigger(DIK_F)) {
				drawVtxNum++;
			}

			RNLib::Doll3DMgr().SetEditDollDrawModelVtxIdxNum(drawVtxNum);
		}
	}

	{
		int countResult = 0;
		const int slitherTimeHalf = SLITHER_TIME * 0.5f;
		if (++m_slitherCounter == SLITHER_TIME) {
			m_slitherCounter = 0;
		}
		else if (m_slitherCounter > slitherTimeHalf) {
			countResult = slitherTimeHalf + (slitherTimeHalf - m_slitherCounter);
		}
		else {
			countResult = m_slitherCounter;
		}

		Rot3D rot = -m_slither + (m_slither * 2.0f * RNLib::Ease().Easing(_RNC_Ease::TYPE::INOUT_SINE, countResult, slitherTimeHalf));

		const _RNC_SetUp3D::CData& setUp = RNLib::SetUp3D().GetData(EDITDATA);
		if (&setUp != NULL) {
			for (short cntBone = 0; cntBone < setUp.m_boneDataNum; cntBone++) {
				m_doll->GetBoneState(cntBone).SetAddRot(rot);
			}
		}
	}

	// [[[ メッセージカウント ]]]
	if (m_messageCounter > 0) {
		if (--m_messageCounter == 0) {
			m_messageType = MESSAGE_TYPE::NONE;
		}
	}

	// [[[ ログ表示 ]]]
	RNLib::Text2D().PutDebugLog(String("-----Information"));
	RNLib::Text2D().PutDebugLog(String("EditPath:%s", m_editPath));
	RNLib::Text2D().PutDebugLog(String("-----Control"));
	RNLib::Text2D().PutDebugLog(String("SetUpCreateNew   [1]"));
	RNLib::Text2D().PutDebugLog(String("SetUpLoad        [2]"));
	RNLib::Text2D().PutDebugLog(String("SetUpReLoad      [3]%s", m_messageType == MESSAGE_TYPE::SETUP_RELOAD_SUCCEEDED ? " SUCCEEDED!" : m_messageType == MESSAGE_TYPE::SETUP_RELOAD_FAILED ? " FAILED!" : ""));
	RNLib::Text2D().PutDebugLog(String("MotionCreateNew  [4]"));
	RNLib::Text2D().PutDebugLog(String("MotionLoad       [5]"));
	RNLib::Text2D().PutDebugLog(String("MotionReLoad     [6]%s", m_messageType == MESSAGE_TYPE::MOTION_RELOAD_SUCCEEDED ? " SUCCEEDED!" : m_messageType == MESSAGE_TYPE::MOTION_RELOAD_FAILED ? " FAILED!" : ""));
	RNLib::Text2D().PutDebugLog(String("IsDrawVtxIdx     [7]   :%s", RNLib::Doll3DMgr().GetEditDollIsDrawModelVtxIdx() ? "TRUE" : "FALSE"));
	RNLib::Text2D().PutDebugLog(String("SlitherX         [Q][W]:%.1f", m_slither.x));
	RNLib::Text2D().PutDebugLog(String("SlitherY         [A][S]:%.1f", m_slither.y));
	RNLib::Text2D().PutDebugLog(String("SlitherZ         [Z][X]:%.1f", m_slither.z));
	RNLib::Text2D().PutDebugLog(String("FocusedBoneIdx   [E][R]:%d", RNLib::Doll3DMgr().GetEditDollDrawModelVtxIdxBoneIdx()));
	RNLib::Text2D().PutDebugLog(String("DrawVtxIdxNum    [D][F]:%d", RNLib::Doll3DMgr().GetEditDollDrawModelVtxIdxNum()));
	RNLib::Text2D().PutDebugLog(String("-----Materials"));

	// マテリアル情報の表示
	if (&RNLib::SetUp3D().GetData(EDITDATA) != NULL) {
		if (RNLib::SetUp3D().GetData(EDITDATA).m_boneDataNum > 0) {
			if (RNLib::SetUp3D().GetData(EDITDATA).m_boneDatas[boneIdx].modelIdx != NONEDATA) {

				// モデルデータを取得
				const _RNC_Model::CData& modelData = RNLib::Model().GetData(RNLib::SetUp3D().GetData(EDITDATA).m_boneDatas[boneIdx].modelIdx);

				// マテリアル情報に対するポインタを取得
				const D3DXMATERIAL* mats = (D3DXMATERIAL*)modelData.m_matBuff->GetBufferPointer();
				for (int cntMat = 0; cntMat < modelData.m_matNum; cntMat++) {
					const Color col = Color{
						(UShort)(mats[cntMat].MatD3D.Diffuse.r * 255),
						(UShort)(mats[cntMat].MatD3D.Diffuse.g * 255),
						(UShort)(mats[cntMat].MatD3D.Diffuse.b * 255),
						(UShort)(mats[cntMat].MatD3D.Diffuse.a * 255) 
					};

					RNLib::Text2D().PutDebugLog(String("Mat_%d", cntMat));
					RNLib::Polygon2D().Put(0, true)
						->SetSize(16.0f, 16.0f)
						->SetPos(RNLib::Text2D().PutDebugLog(String(" Color      :%d %d %d %d", col.r, col.g, col.b, col.a)) + Pos2D(8.0f, 0.0f))
						->SetCol(col);
					RNLib::Polygon2D().Put(0, true)
						->SetSize(16.0f, 16.0f)
						->SetPos(RNLib::Text2D().PutDebugLog(String(" TexturePath:%s", RNLib::Texture().GetLoadPath(modelData.m_texIdxes[cntMat]))) + Pos2D(8.0f, 0.0f))
						->SetTex(modelData.m_texIdxes[cntMat]);
				}
			}
			else {
				RNLib::Text2D().PutDebugLog(String("NONEDATA"));
			}
		}
		else {
			RNLib::Text2D().PutDebugLog(String("NONEDATA"));
		}
	}
	else {
		RNLib::Text2D().PutDebugLog(String("NONEDATA"));
	}
}