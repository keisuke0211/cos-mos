//========================================
// 
// �u���b�N����
// Author:KEISUKE OTONO
// 
//========================================
#include "block.h"
#include "../../main.h"
#include "../../collision.h"
#include "../../stage.h"

//========================================
// �萔��`
//========================================
const CBlock::LooksData CBlock::LOOKS_DATAS[(int)LOOKS_TYPE::MAX] = {
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "data\\TEXTURE\\Material\\Soil.PNG"              , Color(255,255,255,255), MODEL_TYPE::MATERIAL_MESH, SET_TYPE::FORWARD_AND_BACKWARD,  0.0f  , 0.0f , },	  // �y�u���b�N
	{ "data\\MODEL\\StageObject\\SoilBlock_Grassy.x"          , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // ���̐������y�u���b�N
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "data\\TEXTURE\\Material\\StoneTile.PNG"         , Color(191,158,85 ,255), MODEL_TYPE::MATERIAL_MESH, SET_TYPE::FORWARD_AND_BACKWARD,  0.0f  , 0.0f , },	  // �Ñ�̐΃u���b�N
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "data\\TEXTURE\\Material\\StoneTile_PatternA.PNG", Color(191,158,85 ,255), MODEL_TYPE::MATERIAL_MESH, SET_TYPE::FORWARD_AND_BACKWARD,  0.0f  , 0.0f , },	  // �Ñ�̐΃u���b�NA
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "data\\TEXTURE\\Material\\StoneTile_PatternB.PNG", Color(191,158,85 ,255), MODEL_TYPE::MATERIAL_MESH, SET_TYPE::FORWARD_AND_BACKWARD,  0.0f  , 0.0f , },	  // �Ñ�̐΃u���b�NB
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "data\\TEXTURE\\Material\\StoneTile_PatternC.PNG", Color(191,158,85 ,255), MODEL_TYPE::MATERIAL_MESH, SET_TYPE::FORWARD_AND_BACKWARD,  0.0f  , 0.0f , },	  // �Ñ�̐΃u���b�NC
	{ "data\\MODEL\\StageObject\\AncientStonePillar_Up.x"     , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // ���̏�
	{ "data\\MODEL\\StageObject\\AncientStonePillar_Center.x" , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // ���̌q��
	{ "data\\MODEL\\StageObject\\AncientStonePillar_Down.x"   , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // ���̍��{
	{ "data\\MODEL\\StageObject\\Leaf.x"                      , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // ���u���b�N
	{ "data\\MODEL\\StageObject\\TreeTrunk.x"                 , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // �؂̊�
	{ "data\\MODEL\\StageObject\\TreeTrunk_Root.x"            , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // �؂̍���
	{ "NONEDATA"                                              , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             , -16.0f , 0.0f , },	  // �o�I�o�u�̖�(�w�i�p)
	{ "NONEDATA"                                              , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // ��
	{ "data\\MODEL\\coin.x"                                   , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -8.0f  , 0.0f , },	  // �R�ς݂̃R�C��
	{ "data\\MODEL\\stone_monument.x"                         , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // ��̐Δ�
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "data\\TEXTURE\\Material\\StoneBrick.PNG"        , Color(191,158,85 ,255), MODEL_TYPE::MATERIAL_MESH, SET_TYPE::FORWARD_AND_BACKWARD,  0.0f  , 0.0f , },	  // �Ñ�̐΃����K�u���b�N
	{ "data\\MODEL\\StageObject\\SoilAndAncientStoneBlock.x"  , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // �y���Ñ�̐΃u���b�N(���ԗp)
	{ "NONEDATA"                                              , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             , -8.0f  , 0.0f , },	  // ���V�̖�
	{ "data\\MODEL\\Three-eyes_block.x"                       , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // �O�ڂ̃u���b�N
	{ "data\\MODEL\\StageObject\\Leaf_Nuts.x"                 , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // ���u���b�N(�؂̎�)
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // �k�C�̉ƃu���b�N
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "data\\TEXTURE\\Material\\Asphalt.PNG"           , Color(255,255,255,255), MODEL_TYPE::MATERIAL_MESH, SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // �A�X�t�@���g�u���b�N
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "data\\TEXTURE\\Material\\Asphalt_Cracked.PNG"   , Color(255,255,255,255), MODEL_TYPE::MATERIAL_MESH, SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // �Ђъ��ꂽ�A�X�t�@���g�u���b�N
	{ "data\\MODEL\\StageObject\\IronBar.x"                   , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // �S�i�q(�����蔻�薳���z��)
	{ "data\\MODEL\\StageObject\\IronBar1.x"                  , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // �S�i�q1
	{ "data\\MODEL\\StageObject\\IronBar2.x"                  , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // �S�i�q2
	{ "data\\MODEL\\StageObject\\IronBar3.x"                  , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // �S�i�q3
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "data\\TEXTURE\\Material\\Mud.PNG"               , Color(255,255,255,255), MODEL_TYPE::MATERIAL_MESH, SET_TYPE::FORWARD_AND_BACKWARD,  0.0f  , 0.0f , },	  // �D�u���b�N
	{ "data\\MODEL\\StageObject\\SandAndAsphaltBlock.x"       , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // �����A�X�t�@���g�u���b�N(���ԗp)
	{ "data\\MODEL\\StageObject\\SandBlock.x"                 , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // ���u���b�N
	{ "data\\MODEL\\Torch.x"                                  , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 30.0f, },	  // ����
	{ "data\\MODEL\\Snake.x"                                  , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             , -2.0f  , 30.0f, },	  // �ւ̕ǉ�
	{ "data\\MODEL\\leaf_large.x"                             , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // �ΐF�̗t����(��)
	{ "data\\MODEL\\leaf_small.x"                             , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // �ΐF�̗t����(��)
	{ "data\\MODEL\\dry-leaf_large.x"                         , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // ���F�̗t����(��)
	{ "data\\MODEL\\dry-leaf_small.x"                         , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 0.0f , },	  // ���F�̗t����(��)
	{ "data\\MODEL\\StoneDragon.x"                            , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 30.0f, },	  // ���̐Α�
	{ "data\\MODEL\\StoneSword.x"                             , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::PUT          , SET_TYPE::FORWARD             ,  0.0f  , 30.0f, },	  // ���̐Α�
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "data\\TEXTURE\\Material\\Smooth.PNG"            , Color(73 ,109,111,255), MODEL_TYPE::MATERIAL_MESH, SET_TYPE::FORWARD_AND_BACKWARD,  0.0f  , 0.0f , },	  // �Ȃ߂炩�ȃR���N���[�g�u���b�N
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "data\\TEXTURE\\Material\\Smooth_Cracked.PNG"    , Color(73 ,109,111,255), MODEL_TYPE::MATERIAL_MESH, SET_TYPE::FORWARD_AND_BACKWARD,  0.0f  , 0.0f , },	  // �Ȃ߂炩�ȂЂъ��ꂽ�R���N���[�g�u���b�N
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "data\\TEXTURE\\Material\\StoneTile.PNG"         , Color(120,169,170,255), MODEL_TYPE::MATERIAL_MESH, SET_TYPE::FORWARD_AND_BACKWARD,  0.0f  , 0.0f , },	  // �R���N���[�g�u���b�N
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "data\\TEXTURE\\Material\\Smooth.PNG"            , Color(73 ,109,111,255), MODEL_TYPE::MATERIAL_MESH, SET_TYPE::BACKWARD            ,  0.0f  , 0.0f , },	  // (���)�Ȃ߂炩�ȃR���N���[�g�u���b�N
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "data\\TEXTURE\\Material\\Smooth_Cracked.PNG"    , Color(73 ,109,111,255), MODEL_TYPE::MATERIAL_MESH, SET_TYPE::BACKWARD            ,  0.0f  , 0.0f , },	  // (���)�Ȃ߂炩�ȂЂъ��ꂽ�R���N���[�g�u���b�N
	{ "data\\MODEL\\StageObject\\BaseBlock.x"                 , "data\\TEXTURE\\Material\\StoneTile.PNG"         , Color(120,169,170,255), MODEL_TYPE::MATERIAL_MESH, SET_TYPE::BACKWARD            ,  0.0f  , 0.0f , },	  // (���)�R���N���[�g�u���b�N
	{ "data\\MODEL\\sango_1.x"                                , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -12.0f , 30.0f, },	  // �T���S��1
	{ "data\\MODEL\\sango_2.x"                                , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -12.0f , 30.0f, },	  // �T���S��2
	{ "data\\MODEL\\sango_3.x"                                , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -8.0f  , 30.0f, },	  // �T���S��3
	{ "data\\MODEL\\stone_1.x"                                , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             ,  0.0f  , 30.0f, },	  // ��1
	{ "data\\MODEL\\stone_2.x"                                , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -8.0f  , 30.0f, },	  // ��2
	{ "data\\MODEL\\building000.x"                            , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -7.0f  , 60.0f, },	  // ����1
	{ "data\\MODEL\\building001.x"                            , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -7.0f  , 60.0f, },	  // ����2
	{ "data\\MODEL\\Wood_Fence.x"                             , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -10.0f , 30.0f, },	  // �؂̍�
	{ "data\\MODEL\\Old_House.x"                              , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -7.0f  , 60.0f, },	  // ����
	{ "data\\MODEL\\Power-Pole.x"						      , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -7.0f  , 30.0f, },	  // �d��(�d������)
	{ "data\\MODEL\\Power-Pole_LineL.x"						  , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -7.0f  , 30.0f, },	  // �d��(���ɓd��)
	{ "data\\MODEL\\Power-Pole_LineR.x"                       , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -7.0f  , 30.0f, },	  // �d��(�E�ɓd��)
	{ "data\\MODEL\\Power-Pole_LineB.x"                       , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -7.0f  , 30.0f, },	  // �d��(�����ɓd��)
	{ "data\\MODEL\\Bus_Stop.x"								  , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -7.0f  , 30.0f, },	  // �o�X��
	{ "data\\MODEL\\Street_Light.x"							  , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -7.0f  , 30.0f, },	  // �X��
	{ "data\\MODEL\\Sign.x"							          , "NONEDATA"                                       , Color(255,255,255,255), MODEL_TYPE::MESH         , SET_TYPE::FORWARD             , -7.0f  , 30.0f, },	  // �Ŕ�
};
const char* CBlock::OTHER_TEXTURE_PATHS[(int)OTHER_TEXTURE::MAX] = {
	"data\\TEXTURE\\Effect\\effect000.jpg",
};
const char* CBlock::OTHER_MODEL_PATHS[(int)OTHER_MODEL::MAX] = {
	"data\\MODEL\\StageObject\\SoilBlock_Grassy.x",					// ���̐������y�u���b�N

};
const char* CBlock::OTHER_SETUP3D_PATHS[(int)OTHER_SETUP3D::MAX] = {
	"data\\SETUP\\BaobabTree.txt",
	"data\\SETUP\\Chest.txt",
	"data\\SETUP\\PalmTree.txt",
};
const char* CBlock::OTHER_MOTION3D_PATHS[(int)OTHER_MOTION3D::MAX] = {
	"data\\MOTION\\ChestStepped.txt",
	"data\\MOTION\\PalmTree_Shake.txt",
};
const char* CBlock::OTHER_SOUND_PATHS[(int)OTHER_SOUND::MAX] = {
	"data\\SOUND\\SE\\coin.wav",
	"data\\SOUND\\SE\\lightA.wav",
	"data\\SOUND\\SE\\lightB.wav",
};

//========================================
// �ϐ���`
//========================================
UShort CBlock::m_num = 0;
Scale2D CBlock::m_eyescale = Scale2D(5.0f, 5.0f);
short CBlock::m_modelIdxes[(int)LOOKS_TYPE::MAX];
short CBlock::m_pasteTexIdxes[(int)LOOKS_TYPE::MAX];
short CBlock::m_otherTexIdxes[(int)OTHER_TEXTURE::MAX];
short CBlock::m_otherModelIdxes[(int)OTHER_MODEL::MAX];
short CBlock::m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::MAX];
short CBlock::m_otherMotion3DIdxes[(int)OTHER_MOTION3D::MAX];
short CBlock::m_otherSoundIdxes[(int)OTHER_SOUND::MAX];

//========================================
// �ǂݍ��ݏ���
//========================================
void CBlock::Load(void) {

	// �e�N�X�`���̓ǂݍ���
	for (int cnt = 0; cnt < (int)LOOKS_TYPE::MAX; cnt++) {
		m_pasteTexIdxes[cnt] = RNLib::Texture().Load(LOOKS_DATAS[cnt].pasteTexPath);
	}
	for (int cnt = 0; cnt < (int)OTHER_TEXTURE::MAX; cnt++) {
		m_otherTexIdxes[cnt] = RNLib::Texture().Load(OTHER_TEXTURE_PATHS[cnt]);
	}

	// ���f���̓ǂݍ���
	for (int cnt = 0; cnt < (int)LOOKS_TYPE::MAX; cnt++) {
		m_modelIdxes[cnt] = RNLib::Model().Load(LOOKS_DATAS[cnt].modelPath);
	}
	for (int cnt = 0; cnt < (int)OTHER_MODEL::MAX; cnt++) {
		m_otherModelIdxes[cnt] = RNLib::Model().Load(OTHER_MODEL_PATHS[cnt]);
	}

	// �Z�b�g�A�b�v3D�ǂݍ���
	for (int cnt = 0; cnt < (int)OTHER_SETUP3D::MAX; cnt++) {
		m_otherSetUp3DlIdxes[cnt] = RNLib::SetUp3D().Load(OTHER_SETUP3D_PATHS[cnt]);
	}

	// ���[�V����3D�ǂݍ���
	for (int cnt = 0; cnt < (int)OTHER_MOTION3D::MAX; cnt++) {
		m_otherMotion3DIdxes[cnt] = RNLib::Motion3D().Load(OTHER_MOTION3D_PATHS[cnt]);
	}

	// �T�E���h�ǂݍ���
	for (int cnt = 0; cnt < (int)OTHER_SOUND::MAX; cnt++) {
		m_otherSoundIdxes[cnt] = RNLib::Sound().Load(OTHER_SOUND_PATHS[cnt]);
	}
}

//========================================
// �R���X�g���N�^
//========================================
CBlock::CBlock(void) {

	Manager::StageObjectMgr()->AddList(this);
	for (int cnt = 0; cnt < 6; cnt++) {
		m_isHits[cnt] =
			m_isHitOlds[cnt] = false;
	}
	m_isCollision = true;
	m_doll = NULL;
	m_type = TYPE::BLOCK;
	m_width = SIZE_OF_1_SQUARE * 0.5f;
	m_height = SIZE_OF_1_SQUARE;
	m_targetAddPos = INITPOS3D;
	m_oldAddPos = INITPOS3D;
	m_addPos = INITPOS3D;
	m_counter = 0;
	m_counterMax = 0;
	m_nTexIdx = RNLib::Texture().Load("data\\TEXTURE\\Eye.png");
	m_num++;
}

//========================================
// �f�X�g���N�^
//========================================
CBlock::~CBlock() {

	RNLib::Memory().Release(&m_doll);
	m_num--;
}

//========================================
// ������
//========================================
HRESULT CBlock::Init(LOOKS_TYPE looksType) {
	m_looksType = looksType;

	m_pos.z += LOOKS_DATAS[(int)m_looksType].depth;
	m_pos.y += LOOKS_DATAS[(int)m_looksType].height * RNLib::Number().GetPlusMinus(m_pos.y);

	UShort priority = m_isCollision ? PRIORITY_OBJECT : PRIORITY_BACKGROUND;

	// �h�[���̐���
	switch (m_looksType) {
	case LOOKS_TYPE::BAOBAB_TREE: {
		m_doll = new CDoll3D(priority, m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::BAOBAB_TREE]);
		m_doll->SetPos(m_pos + Pos3D(0.0f, (((int)fabsf(m_pos.x) % 20) * (m_pos.y / fabsf(m_pos.y))), 30.0f + ((int)fabsf(m_pos.x) % 20)));
		if (m_pos.y < 0.0f) {
			m_doll->SetRot(Rot3D(0.0f, 0.0f, D3DX_PI));
		}
	}break;
	case LOOKS_TYPE::CHEST: {
		m_doll = new CDoll3D(priority, m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::CHEST]);
		m_doll->SetPos(m_pos);
		if (m_pos.y < 0.0f) {
			m_doll->SetRot(Rot3D(0.0f, 0.0f, D3DX_PI));
		}
	}break;
	case LOOKS_TYPE::PALMTREE: {
		m_doll = new CDoll3D(priority, m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::PALM_TREE]);
		m_doll->SetPos(m_pos);
		if (m_pos.y < 0.0f) {
			m_doll->SetRot(Rot3D(0.0f, 0.0f, D3DX_PI));
		}
	}break;
	}

	// �ʒu����
	switch (m_looksType) {
	case LOOKS_TYPE::SOIL_BLOCK:
	case LOOKS_TYPE::SOIL_BLOCK_GRASSY:
	case LOOKS_TYPE::ANCIENT_STONE_BLOCK:
	case LOOKS_TYPE::ANCIENT_STONE_BLOCK_PTN_A:
	case LOOKS_TYPE::ANCIENT_STONE_BLOCK_PTN_B:
	case LOOKS_TYPE::ANCIENT_STONE_BLOCK_PTN_C:
	case LOOKS_TYPE::LEAF_BLOCK:
	case LOOKS_TYPE::LEAF_LARGE:
	case LOOKS_TYPE::LEAF_SMALL:
	case LOOKS_TYPE::DRY_LEAF_LARGE:
	case LOOKS_TYPE::DRY_LEAF_SMALL:
		m_pos.z -= ((int)fabsf(m_pos.x + m_pos.y) % 20) * 0.5f;
		break;
	}

	if (LOOKS_DATAS[(int)m_looksType].modelType == MODEL_TYPE::MATERIAL_MESH || LOOKS_DATAS[(int)m_looksType].modelType == MODEL_TYPE::MESH) {

		if (LOOKS_DATAS[(int)m_looksType].setType == SET_TYPE::FORWARD ||
			LOOKS_DATAS[(int)m_looksType].setType == SET_TYPE::FORWARD_AND_BACKWARD) {
			if (LOOKS_DATAS[(int)m_looksType].modelType == MODEL_TYPE::MATERIAL_MESH)
				RNLib::StaticMesh().SetMaterialModel(priority, RNLib::Matrix().ConvPosRotToMtx(m_pos + Pos3D(0.0f, 0.0f, 0.0f), m_pos.y >= 0.0f ? INITROT3D : Rot3D(0.0f, 0.0f, D3DX_PI)), m_modelIdxes[(int)m_looksType], m_pasteTexIdxes[(int)m_looksType], LOOKS_DATAS[(int)m_looksType].col, false);
			else 
				RNLib::StaticMesh().SetModel(priority, RNLib::Matrix().ConvPosRotToMtx(m_pos + Pos3D(0.0f, 0.0f, 0.0f), m_pos.y >= 0.0f ? INITROT3D : Rot3D(0.0f, 0.0f, D3DX_PI)), m_modelIdxes[(int)m_looksType], LOOKS_DATAS[(int)m_looksType].col, false);
		}

		if (LOOKS_DATAS[(int)m_looksType].setType == SET_TYPE::BACKWARD ||
			LOOKS_DATAS[(int)m_looksType].setType == SET_TYPE::FORWARD_AND_BACKWARD) {
			Color setCol = LOOKS_DATAS[(int)m_looksType].col;
			setCol.r *= 0.7f;
			setCol.g *= 0.7f;
			setCol.b *= 0.7f;
			if (LOOKS_DATAS[(int)m_looksType].modelType == MODEL_TYPE::MATERIAL_MESH)
				RNLib::StaticMesh().SetMaterialModel(priority, RNLib::Matrix().ConvPosRotToMtx(m_pos + Pos3D(0.0f, 0.0f, 16.0f), m_pos.y >= 0.0f ? INITROT3D : Rot3D(0.0f, 0.0f, D3DX_PI)), m_modelIdxes[(int)m_looksType], m_pasteTexIdxes[(int)m_looksType], setCol, false);
			else
				RNLib::StaticMesh().SetModel(priority, RNLib::Matrix().ConvPosRotToMtx(m_pos + Pos3D(0.0f, 0.0f, 16.0f), m_pos.y >= 0.0f ? INITROT3D : Rot3D(0.0f, 0.0f, D3DX_PI)), m_modelIdxes[(int)m_looksType], setCol, false);
		}

		if (!m_isCollision)
			Delete();	// ����������s��Ȃ��̂ł���Δj������
	}

	return S_OK;
}

//========================================
// �I��
//========================================
void CBlock::Uninit(void) {

}

//========================================
// �X�V
//========================================
void CBlock::Update(void) {

	m_num;

	switch (m_looksType) {
	case LOOKS_TYPE::LEAF_BLOCK: {
		if (--m_counter <= 0) {
			m_counterMax = 60 + rand() % 60;
			m_counter = m_counterMax;
			m_oldAddPos = m_addPos;
			m_targetAddPos = RNLib::Geometry().GetRandomVec() * (1.0f + RNLib::Number().GetRandomFloat(1.0f));
		}

		float rate = RNLib::Ease().Easing(_RNC_Ease::TYPE::INOUT_SINE, m_counter, m_counterMax);
		m_addPos = (m_oldAddPos * rate) + (m_targetAddPos * (1.0f - rate));

		/*RNLib::Model().Put(PRIORITY_OBJECT, m_otherModelIdxes[(int)OTHER_MODEL::LEAF_INSIDE], m_pos - m_addPos * 0.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color);*/
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos + m_addPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color);
	}break;
	case LOOKS_TYPE::CHEST: {

		if (m_pos.y > 0.0f) {
			// �W�����v�ŗ��ꂽ�u��
			if (m_isHitOlds[(int)CCollision::ROT::OVER] && !m_isHits[(int)CCollision::ROT::OVER] && Stage::GetPlayer()->GetInfo(CPlayer::WORLD_SIDE::FACE)->bJump) {
				m_doll->SetMotion(m_otherMotion3DIdxes[(int)OTHER_MOTION3D::CHEST_STEPPED]);
			}
		}
		else {
			// �W�����v�ŗ��ꂽ�u��
			if (m_isHitOlds[(int)CCollision::ROT::UNDER] && !m_isHits[(int)CCollision::ROT::UNDER] && Stage::GetPlayer()->GetInfo(CPlayer::WORLD_SIDE::BEHIND)->bJump) {
				m_doll->SetMotion(m_otherMotion3DIdxes[(int)OTHER_MOTION3D::CHEST_STEPPED]);
			}
		}
	}break;
	case LOOKS_TYPE::PILE_OF_COINS: {

		if (m_pos.y > 0.0f) {
			// ���ꂽ�u��
			if (!m_isHitOlds[(int)CCollision::ROT::OVER] && m_isHits[(int)CCollision::ROT::OVER]) {
				for (int cnt = 0; cnt < 3; cnt++)
					Manager::EffectMgr()->ParticleCreate(CPlayer::GetParticleIdx(CPlayer::PARTI_TEX::SWAP_PARTI00), m_pos, Scale3D(8.0f, 8.0f, 0.0f), Color{ 255,200,0,255 });
				RNLib::Sound().Play(m_otherSoundIdxes[(int)OTHER_SOUND::COIN], _RNC_Sound::CATEGORY::SE, 1.0f, false);
			}
		}
		else {
			// ���ꂽ�u��
			if (!m_isHitOlds[(int)CCollision::ROT::UNDER] && m_isHits[(int)CCollision::ROT::UNDER]) {
				for (int cnt = 0; cnt < 3; cnt++)
					Manager::EffectMgr()->ParticleCreate(CPlayer::GetParticleIdx(CPlayer::PARTI_TEX::SWAP_PARTI00), m_pos, Scale3D(8.0f, 8.0f, 0.0f), Color{ 255,200,0,255 });

				RNLib::Sound().Play(m_otherSoundIdxes[(int)OTHER_SOUND::COIN], _RNC_Sound::CATEGORY::SE, 1.0f, false);
			}
		}

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			;
	}break;
	case LOOKS_TYPE::STONE_MONUMENT: {

		if (m_pos.y > 0.0f) {
			// ���ꂽ�u��
			if (!m_isHitOlds[(int)CCollision::ROT::OVER] && m_isHits[(int)CCollision::ROT::OVER]) {
				if (++m_counter == 1)
					RNLib::Sound().Play(m_otherSoundIdxes[rand() % 2 ? (int)OTHER_SOUND::LIGHT_A : (int)OTHER_SOUND::LIGHT_B], _RNC_Sound::CATEGORY::SE, 1.0f, false);
			}

			// ���ꂽ���Ƃ�����
			if (m_counter > 0) {
				if (++m_counter > 30) {
					m_counter = 30;
					if (rand() % 20 == 0) {
						Manager::EffectMgr()->ParticleCreate(m_otherTexIdxes[(int)OTHER_TEXTURE::EFFECT], m_pos + RNLib::Geometry().GetRandomVec() * RNLib::Number().GetRandomFloat(1.0f) * 16.0f, Scale3D(8.0f, 8.0f, 0.0f), Color{ (rand() % 40),(100 + rand() % 80),(100 + rand() % 80),255 }, CParticle::TYPE::TYPE_FIXED, 60 + rand() % 60);
					}
				}
			}
		}
		else {
			// ���ꂽ�u��
			if (!m_isHitOlds[(int)CCollision::ROT::UNDER] && m_isHits[(int)CCollision::ROT::UNDER]) {
				if (++m_counter == 1)
					RNLib::Sound().Play(m_otherSoundIdxes[rand() % 2 ? (int)OTHER_SOUND::LIGHT_A : (int)OTHER_SOUND::LIGHT_B], _RNC_Sound::CATEGORY::SE, 1.0f, false);
			}

			// ���ꂽ���Ƃ�����
			if (m_counter > 0) {
				if (++m_counter > 30) {
					m_counter = 30;
					if (rand() % 20 == 0) {
						Manager::EffectMgr()->ParticleCreate(m_otherTexIdxes[(int)OTHER_TEXTURE::EFFECT], m_pos + RNLib::Geometry().GetRandomVec() * RNLib::Number().GetRandomFloat(1.0f) * 16.0f, Scale3D(8.0f, 8.0f, 0.0f), Color{ (rand() % 40),(100 + rand() % 80),(100 + rand() % 80),255 }, CParticle::TYPE::TYPE_FIXED, 60 + rand() % 60);
					}
				}
			}
		}

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			->SetBrightnessOfEmissive((float)m_counter / 30);
	}break;
	case LOOKS_TYPE::PALMTREE: {
		if (++m_counter > 240)
		{
			m_doll->SetMotion(m_otherMotion3DIdxes[(int)OTHER_MOTION3D::PLAMTREE_SHAKE]);
			m_counter = 0;
		}
	}break;
	case LOOKS_TYPE::TREE_EYES_BLOCK: {
		D3DXVECTOR3 pos;
		if (m_pos.y > 0)
		{
			pos = Stage::GetPlayer()->GetInfo(CPlayer::WORLD_SIDE::FACE)->pos;
			float fatan = -RNLib::Geometry().FindAngleXY(m_pos, pos);

			RNLib::Polygon3D().Put(PRIORITY_OBJECT, D3DXVECTOR3(m_pos.x, m_pos.y + 4.0f, m_pos.z - 10.0f), D3DXVECTOR3(0.0f, 0.0f, fatan), false)
				->SetSize(m_eyescale)
				->SetTex(m_nTexIdx);
			RNLib::Polygon3D().Put(PRIORITY_OBJECT, D3DXVECTOR3(m_pos.x - 4.5f, m_pos.y - 4.0f, m_pos.z - 10.0f), D3DXVECTOR3(0.0f, 0.0f, fatan), false)
				->SetSize(m_eyescale)
				->SetTex(m_nTexIdx);
			RNLib::Polygon3D().Put(PRIORITY_OBJECT, D3DXVECTOR3(m_pos.x + 4.5f, m_pos.y - 4.0f, m_pos.z - 10.0f), D3DXVECTOR3(0.0f, 0.0f, fatan), false)
				->SetSize(m_eyescale)
				->SetTex(m_nTexIdx);
		}
		else
		{
			pos = Stage::GetPlayer()->GetInfo(CPlayer::WORLD_SIDE::BEHIND)->pos;
			float fatan = -RNLib::Geometry().FindAngleXY(m_pos, pos);
			RNLib::Polygon3D().Put(PRIORITY_OBJECT, D3DXVECTOR3(m_pos.x, m_pos.y - 4.0f, m_pos.z - 10.0f), D3DXVECTOR3(0.0f, 0.0f, fatan), false)
				->SetSize(m_eyescale)
				->SetTex(m_nTexIdx);
			RNLib::Polygon3D().Put(PRIORITY_OBJECT, D3DXVECTOR3(m_pos.x - 4.5f, m_pos.y + 4.0f, m_pos.z - 10.0f), D3DXVECTOR3(0.0f, 0.0f, fatan), false)
				->SetSize(m_eyescale)
				->SetTex(m_nTexIdx);
			RNLib::Polygon3D().Put(PRIORITY_OBJECT, D3DXVECTOR3(m_pos.x + 4.5f, m_pos.y + 4.0f, m_pos.z - 10.0f), D3DXVECTOR3(0.0f, 0.0f, fatan), false)
				->SetSize(m_eyescale)
				->SetTex(m_nTexIdx);

		}

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			;

	}break;
	case LOOKS_TYPE::LEAF_BLOCK_NUTS: {
		if (--m_counter <= 0) {
			m_counterMax = 60 + rand() % 60;
			m_counter = m_counterMax;
			m_oldAddPos = m_addPos;
			m_targetAddPos = RNLib::Geometry().GetRandomVec() * (1.0f + RNLib::Number().GetRandomFloat(1.0f));
		}

		float rate = RNLib::Ease().Easing(_RNC_Ease::TYPE::INOUT_SINE, m_counter, m_counterMax);
		m_addPos = (m_oldAddPos * rate) + (m_targetAddPos * (1.0f - rate));

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos + m_addPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color);
	}break;
	case LOOKS_TYPE::TORCH: {
		// ���̃G�t�F�N�g
		float m_ScaleTex = (float)(rand() % (int)(INIT_EFFECT_SCALE.x * 0.3) + 1.0f);

		D3DXVECTOR3 m_TexPos = D3DXVECTOR3(m_pos.x + (float)(rand() % (int)m_width * 0.5 - m_width * 0.25), m_pos.y > 0.0f ? m_pos.y + 5.0f : m_pos.y - 5.0f, m_pos.z);

		Manager::EffectMgr()->ParticleCreate(RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_steam_000.png"), m_TexPos, D3DXVECTOR3(m_ScaleTex, m_ScaleTex, 0.0f), Color{ 255,50,0,255 }, CParticle::TYPE::TYPE_FLOATUP, 200, m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI),D3DXVECTOR3(40.0f,40.0f,0.0f));

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			;
	}break;
	case LOOKS_TYPE::SNAKE: {
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], D3DXVECTOR3(m_pos.x, m_pos.y > 0.0f ? m_pos.y - 5.0f : m_pos.y + 5.0f, m_pos.z), m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			;
	}break;
	case LOOKS_TYPE::LEAF_LARGE: {

		RNLib::Model().Put(PRIORITY_OBJECT, m_otherModelIdxes[(int)OTHER_MODEL::SOIL_BLOCK_GRASSY], m_pos, m_pos.y >= 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			;
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			;
	}break;
	case LOOKS_TYPE::LEAF_SMALL: {
		RNLib::Model().Put(PRIORITY_OBJECT, m_otherModelIdxes[(int)OTHER_MODEL::SOIL_BLOCK_GRASSY], m_pos, m_pos.y >= 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			;

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], D3DXVECTOR3(m_pos.x, m_pos.y > 0.0f ? m_pos.y - 5.0f : m_pos.y + 5.0f, m_pos.z), m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			;
	}break;
	case LOOKS_TYPE::DRY_LEAF_LARGE: {
		RNLib::Model().Put(PRIORITY_OBJECT, m_otherModelIdxes[(int)OTHER_MODEL::SOIL_BLOCK_GRASSY], m_pos, m_pos.y >= 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			;

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], D3DXVECTOR3(m_pos.x, m_pos.y > 0.0f ? m_pos.y - 5.0f : m_pos.y + 5.0f, m_pos.z), m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			;
	}break;
	case LOOKS_TYPE::DRY_LEAF_SMALL: {
		RNLib::Model().Put(PRIORITY_OBJECT, m_otherModelIdxes[(int)OTHER_MODEL::SOIL_BLOCK_GRASSY], m_pos, m_pos.y >= 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			;

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], D3DXVECTOR3(m_pos.x, m_pos.y > 0.0f ? m_pos.y + 5.0f : m_pos.y - 5.0f, m_pos.z), m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			;
	}break;
	case LOOKS_TYPE::STONE_DRAGON: {
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], D3DXVECTOR3(m_pos.x, m_pos.y > 0.0f ? m_pos.y - 5.0f : m_pos.y + 5.0f, m_pos.z), m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color);
	}break;
	case LOOKS_TYPE::STONE_SWORD: {
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], D3DXVECTOR3(m_pos.x, m_pos.y > 0.0f ? m_pos.y - 5.0f : m_pos.y + 5.0f, m_pos.z), m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color);
	}break;
	default: {
		if (LOOKS_DATAS[(int)m_looksType].modelType == MODEL_TYPE::PUT) {
			RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, m_pos.y >= 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
				->SetCol(m_color);
		}
	}break;
	}

	// �ߋ��̓�����t���O�ۑ�
	for (int cnt = 0; cnt < 6; cnt++) {
		m_isHitOlds[cnt] = m_isHits[cnt];
		m_isHits[cnt] = false;
	}
}

//========================================
// �`��
//========================================
void CBlock::Draw(void) {

}