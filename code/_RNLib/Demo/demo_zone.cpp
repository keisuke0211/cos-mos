//========================================
// 
// デモゾーンの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"
#include "../RNmode.h"
#include "demo.h"
#include "demo_player.h"
#include "demo_zone.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]デモゾーンクラス
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 静的変数定義
//****************************************
const CDemoZone::TypeData CDemoZone::TYPE_DATAS[(int)TYPE2::MAX] = {
	{"DrawTest"      ,Pos3D(100.0f *  0.0f, 0.0f,100.0f * 0.5f),{90.0f,190.0f}},
	{"CameraTest"    ,Pos3D(100.0f *  1.0f, 0.0f,100.0f * 0.0f),{90.0f,90.0f }},
	{"DollTest"      ,Pos3D(100.0f *  1.0f, 0.0f,100.0f * 1.0f),{90.0f,90.0f }},
	{"EffectTest"    ,Pos3D(100.0f * -1.0f, 0.0f,100.0f * 0.0f),{90.0f,90.0f }},
	{"StaticMeshTest",Pos3D(100.0f * -1.0f, 0.0f,100.0f * 1.5f),{90.0f,190.0f}},
};
CDemoZone* CDemoZone::ms_active = NULL;

//========================================
// コンストラクタ
//========================================
CDemoZone::CDemoZone(const TYPE2& type2) : CDemoObject(TYPE::ZONE) {

	const Pos3D& pos = TYPE_DATAS[(int)type2].pos;

	m_type2    = type2;
	m_typeInfo = NULL;
	switch (m_type2) {
	case TYPE2::DRAW_TEST: {
		CMemory::Alloc((TypeInfo_DrawTest**)&m_typeInfo);
		*(TypeInfo_DrawTest*)m_typeInfo = {};
	}break;
	case TYPE2::CAMERA_TEST: {

	}break;
	case TYPE2::DOLL_TEST: {
		CMemory::Alloc((TypeInfo_DollTest**)&m_typeInfo);
		TypeInfo_DollTest* typeInfo = (TypeInfo_DollTest*)m_typeInfo;
		typeInfo->doll = new CDoll3D((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetSetUp3DIdx(CDefaultData::SETUP3D::CAPSULE));
	}break;
	case TYPE2::EFFECT_TEST: {

	}break;
	case TYPE2::STATICMESH_TEST: {
		CMemory::Alloc((TypeInfo_MatMeshTest**)&m_typeInfo);
		*(TypeInfo_MatMeshTest*)m_typeInfo = {};
	}break;
	}
}

//========================================
// デストラクタ
//========================================
CDemoZone::~CDemoZone() {

	switch (m_type2) {
	case TYPE2::DRAW_TEST: {

	}break;
	case TYPE2::CAMERA_TEST: {

	}break;
	case TYPE2::DOLL_TEST: {
		TypeInfo_DollTest* typeInfo = (TypeInfo_DollTest*)m_typeInfo;
		CMemory::Release(&typeInfo->doll);
	}break;
	case TYPE2::EFFECT_TEST: {

	}break;
	case TYPE2::STATICMESH_TEST: {

	}break;
	}

	CMemory::Release(&m_typeInfo);
}

//========================================
// 更新処理
//========================================
void CDemoZone::Update(void) {

	const Pos3D& pos = TYPE_DATAS[(int)m_type2].pos;

	UShort colorRGB = 128;
	if (ms_active == this) 
	{// 自身がアクティブの時、
		colorRGB = 255;

		// [[[ アクティブ時の更新処理 ]]]
		UpdateActive();

		// アクティブポインタをNULLにしておく(※このフレーム限りにする為)
		ms_active = NULL;
	}

	{// [[[ 床の描画 ]]]
		const float uvRight  = TYPE_DATAS[(int)m_type2].size.x / 10.0f;
		const float uvBottom = TYPE_DATAS[(int)m_type2].size.y / 10.0f;

		RNLib::Polygon3D().Put((UShort)RNMode::PRIORITY::STAGE3D, pos, Rot3D(D3DX_PI_HALF, 0.0f, 0.0f))
			->SetSize(TYPE_DATAS[(int)m_type2].size)
			->SetCol(Color{ colorRGB, colorRGB, colorRGB, (UShort)255 })
			->SetTexUV(
				RNLib::DefaultData().GetTextureIdx(CDefaultData::TEXTURE::TILE),
				Pos2D(0.0f, 0.0f),
				Pos2D(uvRight, 0.0f),
				Pos2D(0.0f, uvBottom),
				Pos2D(uvRight, uvBottom));
	}

	// [[[ 種類毎の処理 ]]]
	switch (m_type2) {
	case TYPE2::DRAW_TEST: {
		TypeInfo_DrawTest* typeInfo = (TypeInfo_DrawTest*)m_typeInfo;

		switch (typeInfo->putObjType) {
		case PUTOBJECT_TYPE::POLYGON3D: {

		}break;
		case PUTOBJECT_TYPE::TEXT3D: {

			RNLib::Text3D().Put((UShort)RNMode::PRIORITY::UI3D, "TEST", typeInfo->text3DInfo.alignment, 0, pos + Pos3D(0.0f, 5.0f, -45.0f), INITROT3D)
				->SetSize(Size2D(10.0f, 10.0f))
				->SetBillboard(true);

		}break;
		case PUTOBJECT_TYPE::MODEL: {

			RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::PRUFEN_HEAD), pos + Pos3D(0.0f, 5.0f, -50.0f), INITROT3D)
				->SetCol(typeInfo->modelInfo.col)
				->SetTex(typeInfo->modelInfo.isTex ? RNLib::DefaultData().GetTextureIdx(CDefaultData::TEXTURE::GRID) : NONEDATA)
				->SetZTest(typeInfo->modelInfo.isZTest)
				->SetLighting(typeInfo->modelInfo.isLighting)
				->SetOutLineIdx(typeInfo->modelInfo.m_outLineIdx)
				->SetBrightnessOfEmissive(typeInfo->modelInfo.m_brightnessOfEmissive);

		}break;
		}

		struct LocalFunc {
			static Pos3D FindPos(const UShort& putNum) {
				UShort x = putNum % 5;
				UShort y = putNum / 25;
				UShort z = (putNum / 5) % 5;

				return Pos3D(x * 10.0f, y * 10.0f, z * -10.0f);
			}
		};

		// 配置
		UShort putNum = 0;
		const Pos3D basePos = pos + Pos3D(-20.0f, 5.0f, 70.0f);
		for (int cntModel = 0; cntModel < typeInfo->modelInfo.drawNum; cntModel++, putNum++) {

			RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::PRUFEN_HEAD), basePos + LocalFunc::FindPos(putNum), Rot3D(0.0f,D3DX_PI,0.0f))
				->SetCol(typeInfo->modelInfo.col)
				->SetTex(typeInfo->modelInfo.isTex ? RNLib::DefaultData().GetTextureIdx(CDefaultData::TEXTURE::GRID) : NONEDATA)
				->SetZTest(typeInfo->modelInfo.isZTest)
				->SetLighting(typeInfo->modelInfo.isLighting)
				->SetOutLineIdx(typeInfo->modelInfo.m_outLineIdx)
				->SetBrightnessOfEmissive(typeInfo->modelInfo.m_brightnessOfEmissive);
		}

	}break;
	case TYPE2::CAMERA_TEST: {

		// 赤スフィア
		RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::SPHERE), pos + Pos3D(10.0f, 5.0f, 0.0f), INITROT3D)
			->SetCol(Color{ 255, 0, 0, 255 })
			->SetOutLineIdx(true)
			->SetClippingCamera(RNDemo::Get().GetPlayer(CDemo::PLAYER::BLUE).GetCamera());	// ※青プレイヤーのカメラにのみ描画

		// 白スフィア
		RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::SPHERE), pos + Pos3D(0.0f, 5.0f, 0.0f), INITROT3D)
			->SetCol(Color{ 255, 255, 255, 255 })
			->SetOutLineIdx(true);

		// 青スフィア
		RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::SPHERE), pos + Pos3D(-10.0f, 5.0f, 0.0f), INITROT3D)
			->SetCol(Color{ 0, 0, 255, 255 })
			->SetOutLineIdx(true)
			->SetClippingCamera(RNDemo::Get().GetPlayer(CDemo::PLAYER::RED).GetCamera());	// ※赤プレイヤーのカメラにのみ描画
		
	}break;
	case TYPE2::DOLL_TEST: {
		TypeInfo_DollTest* typeInfo = (TypeInfo_DollTest*)m_typeInfo;

		// 白いボール
		RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::SPHERE), pos + Pos3D(-20.0f, 5.0f, 0.0f), INITROT3D)
			->SetCol(Color{ 255, 255, 255, 255 });

		// ドール3D
		typeInfo->doll->SetPos(pos + Pos3D(0.0f, 0.0f, 0.0f));
	}break;
	case TYPE2::EFFECT_TEST: {

	}break;
	case TYPE2::STATICMESH_TEST: {

	}break;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]デモゾーンクラス
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 定数定義
//****************************************
const char* CDemoZone::PUTOBJECT_TYPE_NAME[(int)PUTOBJECT_TYPE::MAX] = {
	"Polygon3D",
	"Text3D",
	"Model",
};
const char* CDemoZone::ALIGNMENT_NAME[(int)CText::ALIGNMENT::MAX] = {
	"Center",
	"Left",
	"Right",
};

//========================================
// 更新処理(アクティブ時)
//========================================
void CDemoZone::UpdateActive(void) {
	
	const Pos3D& pos = TYPE_DATAS[(int)m_type2].pos;

	// ゾーン名表示
	RNLib::Text2D().PutDebugLog(CreateText("----------%s", TYPE_DATAS[(int)m_type2].name));

	// [[[ 種類毎の処理 ]]]
	switch (m_type2) {
	case TYPE2::DRAW_TEST: {
		TypeInfo_DrawTest* typeInfo = (TypeInfo_DrawTest*)m_typeInfo;

		if (RNLib::Input().GetKeyTrigger(DIK_1)) {
			UShort typeCount = (UShort)typeInfo->putObjType;
			typeInfo->putObjType = (PUTOBJECT_TYPE)((typeCount + 1) % (int)PUTOBJECT_TYPE::MAX);
		}
		
		if (RNLib::Input().GetKeyPress(DIK_F) && typeInfo->modelInfo.drawNum > 0) {
			typeInfo->modelInfo.drawNum--;
		}

		if (RNLib::Input().GetKeyPress(DIK_G)) {
			typeInfo->modelInfo.drawNum++;
		}


		RNLib::Text2D().PutDebugLog(CreateText("Polygon3DNumChange[R][T]"));
		RNLib::Text2D().PutDebugLog(CreateText("Model    NumChange[F][G]"));
		RNLib::Text2D().PutDebugLog(CreateText("----------PutObject"));
		RNLib::Text2D().PutDebugLog(CreateText("Type     [1]:%s", PUTOBJECT_TYPE_NAME[(int)typeInfo->putObjType]));

		switch (typeInfo->putObjType) {
		case PUTOBJECT_TYPE::POLYGON3D: {

		}break;
		case PUTOBJECT_TYPE::TEXT3D: {
			if (RNLib::Input().GetKeyTrigger(DIK_2)) {
				UShort alignmentCount = (UShort)typeInfo->text3DInfo.alignment;
				typeInfo->text3DInfo.alignment = (CText::ALIGNMENT)((alignmentCount + 1) % (int)CText::ALIGNMENT::MAX);
			}

			RNLib::Text2D().PutDebugLog(CreateText("Alignment[2]:%s", ALIGNMENT_NAME[(int)typeInfo->text3DInfo.alignment]));
		}break;
		case PUTOBJECT_TYPE::MODEL: {

		}break;
		}
	}break;
	case TYPE2::CAMERA_TEST: {

	}break;
	case TYPE2::DOLL_TEST: {
		TypeInfo_DollTest* typeInfo = (TypeInfo_DollTest*)m_typeInfo;
		CDoll3D::CBoneState& bone0 = typeInfo->doll->GetBoneState(0);
		CDoll3D::CBoneState& bone1 = typeInfo->doll->GetBoneState(1);
		
		{// ボーン0の回転操作
			Vector3D addRot = INITVECTOR3D;

			if (RNLib::Input().GetKeyPress(DIK_R))
				addRot.x += 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_T))
				addRot.x -= 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_F))
				addRot.y += 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_G))
				addRot.y -= 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_V))
				addRot.z += 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_B))
				addRot.z -= 0.02f;

			bone0.AddAddRot(addRot);
		}

		{// ボーン1の回転操作
			Vector3D addRot = INITVECTOR3D;

			if (RNLib::Input().GetKeyPress(DIK_Y))
				addRot.x += 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_U))
				addRot.x -= 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_H))
				addRot.y += 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_J))
				addRot.y -= 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_N))
				addRot.z += 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_M))
				addRot.z -= 0.02f;

			bone1.AddAddRot(addRot);
		}

		RNLib::Text2D().PutDebugLog(CreateText("Bone0"));
		RNLib::Text2D().PutDebugLog(CreateText("SpinX[R][T]:%f", bone0.GetAddRot().x));
		RNLib::Text2D().PutDebugLog(CreateText("SpinY[F][G]:%f", bone0.GetAddRot().y));
		RNLib::Text2D().PutDebugLog(CreateText("SpinZ[V][B]:%f", bone0.GetAddRot().z));
		RNLib::Text2D().PutDebugLog(CreateText("Bone1"));
		RNLib::Text2D().PutDebugLog(CreateText("SpinX[Y][U]:%f", bone1.GetAddRot().x));
		RNLib::Text2D().PutDebugLog(CreateText("SpinY[H][J]:%f", bone1.GetAddRot().y));
		RNLib::Text2D().PutDebugLog(CreateText("SpinZ[N][M]:%f", bone1.GetAddRot().z));
	}break;
	case TYPE2::EFFECT_TEST: {

		if (RNLib::Input().GetKeyTrigger(DIK_1))
			RNLib::StandardEffect3D().CreateDustStormOnLanding(pos, INITROT3D, Color{ 214,209,176,255 }, 10.0f);

	}break;
	case TYPE2::STATICMESH_TEST: {
		TypeInfo_MatMeshTest* typeInfo = (TypeInfo_MatMeshTest*)m_typeInfo;

		const Pos3D basePos = pos + Pos3D(75.0f, 5.0f, -75.0f);
		struct LocalFunc {
			static Pos3D FindPos(const UShort& putNum) {
				UShort x = putNum % 5;
				UShort y = putNum / 25;
				UShort z = (putNum / 5) % 5;

				return Pos3D(x * 12.0f, y * 12.0f, z * -12.0f);
			}
		};

		if (RNLib::Input().GetKeyPress(DIK_1)) {
			for (int cnt = 0; cnt < 5; cnt++) {
				if (typeInfo->isMesh) {
					if ((typeInfo->setNum / 25) % 2 == 0) {
						RNLib::StaticMesh().SetModel(
							(UShort)RNMode::PRIORITY::OBJECT3D,
							pos + basePos + LocalFunc::FindPos(typeInfo->setNum), Rot3D(0.0f, D3DX_PI, 0.0f),
							RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::PRUFEN_HEAD),
							Color{ 255,255,255,255 },
							false);
					}
					else {
						RNLib::StaticMesh().SetMaterialModel(
							(UShort)RNMode::PRIORITY::OBJECT3D,
							CMatrix::ConvPosRotToMtx(pos + basePos + LocalFunc::FindPos(typeInfo->setNum), Rot3D(0.0f, D3DX_PI, 0.0f)),
							RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::PRUFEN_HEAD),
							RNLib::DefaultData().GetTextureIdx((CDefaultData::TEXTURE)(rand() % (int)CDefaultData::TEXTURE::MAX)),
							Color{ rand() % 255 ,rand() % 255,rand() % 255,255 },
							false);
					}
				}
				typeInfo->setNum++;
			}
		}

		if (RNLib::Input().GetKeyTrigger(DIK_2)) {
			typeInfo->isMesh = !typeInfo->isMesh;

			if (typeInfo->isMesh) {
				for (int cnt = 0; cnt < typeInfo->setNum; cnt++) {
					if ((typeInfo->setNum / 25) % 2 == 0) {
						RNLib::StaticMesh().SetModel(
							(UShort)RNMode::PRIORITY::OBJECT3D,
							pos + basePos + LocalFunc::FindPos(cnt), Rot3D(0.0f, D3DX_PI, 0.0f),
							RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::PRUFEN_HEAD),
							Color{ 255,255,255,255 },
							false);
					}
					else {
						RNLib::StaticMesh().SetMaterialModel(
							(UShort)RNMode::PRIORITY::OBJECT3D,
							CMatrix::ConvPosRotToMtx(pos + basePos + LocalFunc::FindPos(cnt), Rot3D(0.0f, D3DX_PI, 0.0f)),
							RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::PRUFEN_HEAD),
							RNLib::DefaultData().GetTextureIdx((CDefaultData::TEXTURE)(rand() % (int)CDefaultData::TEXTURE::MAX)),
							Color{ rand() % 255 ,rand() % 255,rand() % 255,255 },
							false);
					}
				}
			}
			else {
				RNLib::StaticMesh().Delete(false);
			}
		}

		// モデルを設置
		if (!typeInfo->isMesh) {
			for (int cnt = 0; cnt < typeInfo->setNum; cnt++)
				RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::PRUFEN_HEAD), pos + basePos + LocalFunc::FindPos(cnt), Rot3D(0.0f, D3DX_PI, 0.0f));
		}

		RNLib::Text2D().PutDebugLog(CreateText("SetModel[1]   :%d", typeInfo->setNum));
		RNLib::Text2D().PutDebugLog(CreateText("IsMesh  [2]   :%s", typeInfo->isMesh ? "TRUE" : "FALSE"));
		RNLib::Text2D().PutDebugLog(CreateText("MeshNum       :%d", RNLib::StaticMesh().GetMeshNum()));

	}break;
	}
}