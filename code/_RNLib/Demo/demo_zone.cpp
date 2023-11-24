//========================================
// 
// �f���]�[���̏���
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
//==========| [���J]�f���]�[���N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// �ÓI�ϐ���`
//****************************************
const CDemoZone::TypeData CDemoZone::TYPE_DATAS[(int)TYPE2::MAX] = {
	{"DrawTest"  ,Pos3D(100.0f * 0, 0.0f,100.0f * 0.5f),{90.0f,190.0f}},
	{"CameraTest",Pos3D(100.0f * 1, 0.0f,100.0f * 0.0f),{90.0f,90.0f }},
	{"DollTest"  ,Pos3D(100.0f * 1, 0.0f,100.0f * 1.0f),{90.0f,90.0f }},
};
CDemoZone* CDemoZone::ms_active = NULL;

//========================================
// �R���X�g���N�^
//========================================
CDemoZone::CDemoZone(const TYPE2& type2) : CDemoObject(TYPE::ZONE) {

	m_type2    = type2;
	m_typeInfo = NULL;
	switch (m_type2) {
	case TYPE2::DRAW_TEST: {
		CMemory::Alloc((TypeInfo_DrawTest**)&m_typeInfo);
		TypeInfo_DrawTest* typeInfo = (TypeInfo_DrawTest*)m_typeInfo;
		*typeInfo = {};
	}break;
	case TYPE2::CAMERA_TEST: {

	}break;
	case TYPE2::DOLL_TEST: {
		CMemory::Alloc((TypeInfo_DollTest**)&m_typeInfo);
		TypeInfo_DollTest* typeInfo = (TypeInfo_DollTest*)m_typeInfo;
		typeInfo->doll = new CDoll3D((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetSetUp3DIdx(CDefaultData::SETUP3D::CAPSULE));
	}break;
	}
}

//========================================
// �f�X�g���N�^
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
	}

	CMemory::Release(&m_typeInfo);
}

//========================================
// �X�V����
//========================================
void CDemoZone::Update(void) {

	const Pos3D& pos = TYPE_DATAS[(int)m_type2].pos;

	UShort colorRGB = 128;
	if (ms_active == this) 
	{// ���g���A�N�e�B�u�̎��A
		colorRGB = 255;

		// [[[ �A�N�e�B�u���̍X�V���� ]]]
		UpdateActive();

		// �A�N�e�B�u�|�C���^��NULL�ɂ��Ă���(�����̃t���[������ɂ����)
		ms_active = NULL;
	}

	{// [[[ ���̕`�� ]]]
		const float uvRight  = TYPE_DATAS[(int)m_type2].size.x / 10.0f;
		const float uvBottom = TYPE_DATAS[(int)m_type2].size.y / 10.0f;

		RNLib::Polygon3D().Put((UShort)RNMode::PRIORITY::STAGE3D, pos, Rot3D(D3DX_PI_HALF, 0.0f, 0.0f))
			->SetSize(TYPE_DATAS[(int)m_type2].size)
			->SetCol(Color{ colorRGB, colorRGB, colorRGB, 255 })
			->SetTexUV(
				RNLib::DefaultData().GetTextureIdx(CDefaultData::TEXTURE::TILE),
				Pos2D(0.0f, 0.0f),
				Pos2D(uvRight, 0.0f),
				Pos2D(0.0f, uvBottom),
				Pos2D(uvRight, uvBottom));
	}

	// [[[ ��ޖ��̏��� ]]]
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
				->SetOutLine(typeInfo->modelInfo.m_isOutLine)
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

		// �z�u
		UShort putNum = 0;
		const Pos3D basePos = pos + Pos3D(-20.0f, 5.0f, 70.0f);
		for (int cntModel = 0; cntModel < typeInfo->modelInfo.drawNum; cntModel++, putNum++) {

			RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::PRUFEN_HEAD), basePos + LocalFunc::FindPos(putNum), Rot3D(0.0f,D3DX_PI,0.0f))
				->SetCol(typeInfo->modelInfo.col)
				->SetTex(typeInfo->modelInfo.isTex ? RNLib::DefaultData().GetTextureIdx(CDefaultData::TEXTURE::GRID) : NONEDATA)
				->SetZTest(typeInfo->modelInfo.isZTest)
				->SetLighting(typeInfo->modelInfo.isLighting)
				->SetOutLine(typeInfo->modelInfo.m_isOutLine)
				->SetBrightnessOfEmissive(typeInfo->modelInfo.m_brightnessOfEmissive);
		}

	}break;
	case TYPE2::CAMERA_TEST: {

		// �ԃX�t�B�A
		RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::SPHERE), pos + Pos3D(10.0f, 5.0f, 0.0f), INITROT3D)
			->SetCol(Color{ 255, 0, 0, 255 })
			->SetOutLine(true)
			->SetClippingCamera(RNDemo::Get().GetPlayer(CDemo::PLAYER::BLUE).GetCamera());	// ���v���C���[�̃J�����ɂ̂ݕ`��

		// ���X�t�B�A
		RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::SPHERE), pos + Pos3D(0.0f, 5.0f, 0.0f), INITROT3D)
			->SetCol(Color{ 255, 255, 255, 255 })
			->SetOutLine(true);

		// �X�t�B�A
		RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::SPHERE), pos + Pos3D(-10.0f, 5.0f, 0.0f), INITROT3D)
			->SetCol(Color{ 0, 0, 255, 255 })
			->SetOutLine(true)
			->SetClippingCamera(RNDemo::Get().GetPlayer(CDemo::PLAYER::RED).GetCamera());	// ���ԃv���C���[�̃J�����ɂ̂ݕ`��
		
	}break;
	case TYPE2::DOLL_TEST: {
		TypeInfo_DollTest* typeInfo = (TypeInfo_DollTest*)m_typeInfo;

		// �����{�[��
		RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::SPHERE), pos + Pos3D(-20.0f, 5.0f, 0.0f), INITROT3D)
			->SetCol(Color{ 255, 255, 255, 255 });

		// �h�[��3D
		typeInfo->doll->SetPos(pos + Pos3D(0.0f, 0.0f, 0.0f));
	}break;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�f���]�[���N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// �萔��`
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
// �X�V����(�A�N�e�B�u��)
//========================================
void CDemoZone::UpdateActive(void) {
	
	// �]�[�����\��
	RNLib::Text2D().PutDebugLog(CreateText("ZoneName        :�w%s�x", TYPE_DATAS[(int)m_type2].name));

	// [[[ ��ޖ��̏��� ]]]
	switch (m_type2) {
	case TYPE2::DRAW_TEST: {
		TypeInfo_DrawTest* typeInfo = (TypeInfo_DrawTest*)m_typeInfo;

		if (RNLib::Input().GetKeyTrigger(DIK_1)) {
			UShort typeCount = (UShort)typeInfo->putObjType;
			typeInfo->putObjType = (PUTOBJECT_TYPE)((typeCount + 1) % (int)PUTOBJECT_TYPE::MAX);
		}
		
		if (RNLib::Input().GetKeyPress(DIK_Z)) {
		}

		if (RNLib::Input().GetKeyPress(DIK_X)) {
		}

		if (RNLib::Input().GetKeyPress(DIK_C)) {
			typeInfo->modelInfo.drawNum++;
		}

		RNLib::Text2D().PutDebugLog(CreateText("-----DrawManager-----"));
		RNLib::Text2D().PutDebugLog(CreateText("Polygon2D Num   :%-4d+%-4d", RNLib::DrawMgr().GetRegistInfoSum().m_polygon2DDrawNum           , RNLib::DrawMgr().GetRegistInfoSumScreen().m_polygon2DDrawNum));
		RNLib::Text2D().PutDebugLog(CreateText("Polygon3D Num[Z]:%-4d+%-4d", RNLib::DrawMgr().GetRegistInfoSum().m_polygon3DDrawNum           , RNLib::DrawMgr().GetRegistInfoSumScreen().m_polygon3DDrawNum));
		RNLib::Text2D().PutDebugLog(CreateText("Text2D    Num   :%-4d+%-4d", RNLib::DrawMgr().GetRegistInfoSum().m_text2DDrawNum              , RNLib::DrawMgr().GetRegistInfoSumScreen().m_text2DDrawNum));
		RNLib::Text2D().PutDebugLog(CreateText("Text3D    Num[X]:%-4d+%-4d", RNLib::DrawMgr().GetRegistInfoSum().m_text3DDrawNum              , RNLib::DrawMgr().GetRegistInfoSumScreen().m_text3DDrawNum));
		RNLib::Text2D().PutDebugLog(CreateText("Model     Num[C]:%-4d+%-4d", RNLib::DrawMgr().GetRegistInfoSum().m_modelDrawNum               , RNLib::DrawMgr().GetRegistInfoSumScreen().m_modelDrawNum));
		RNLib::Text2D().PutDebugLog(CreateText("-----PutObject-----"));
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
		
		{// �{�[��0�̉�]����
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

		{// �{�[��1�̉�]����
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

		RNLib::Text2D().PutDebugLog(CreateText("-----Bone0-----"));
		RNLib::Text2D().PutDebugLog(CreateText("SpinX[R][T]:%f", bone0.GetAddRot().x));
		RNLib::Text2D().PutDebugLog(CreateText("SpinY[F][G]:%f", bone0.GetAddRot().y));
		RNLib::Text2D().PutDebugLog(CreateText("SpinZ[V][B]:%f", bone0.GetAddRot().z));
		RNLib::Text2D().PutDebugLog(CreateText("-----Bone1-----"));
		RNLib::Text2D().PutDebugLog(CreateText("SpinX[Y][U]:%f", bone1.GetAddRot().x));
		RNLib::Text2D().PutDebugLog(CreateText("SpinY[H][J]:%f", bone1.GetAddRot().y));
		RNLib::Text2D().PutDebugLog(CreateText("SpinZ[N][M]:%f", bone1.GetAddRot().z));
	}break;
	}
}