//================================================================================================
// 
// �Y����[pile.cpp]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "pile.h"
#include "../../main.h"

//�Y���߂荞�܂���Œፂ��
const float CPile::CAVEIN_DIFF_HEIGHT = 35.0f;

const int NUMTEX = 2;
const int RAND_TEX = rand() % NUMTEX;
//=======================================
// �R���X�g���N�^
//=======================================
CPile::CPile()
{
	Manager::StageObjectMgr()->AddList(this);

	//�������
	m_type = TYPE::PILE;
	m_width = SIZE_OF_1_SQUARE * 2;
	m_height = SIZE_OF_1_SQUARE;

	m_TrunkModelID = NONEDATA;
	m_PilePos = INITVECTOR3D;	//�{�̍��W
	m_NumTrunk = 0;			//���̐��i�Œ�R�j
	m_fEvenTrunkCorrHeight = 0.0f;//���̐��������������ꍇ�̕␳�l�i��Ȃ�0.0f
	m_TrunkHeight = 0.0f;	//�����W
	m_TrunkHeightOld = 0.0f;//�����W
	m_StartTrunkHeight = 0.0f;
}

//=======================================
// �f�X�g���N�^
//=======================================
CPile::~CPile()
{

}

//=======================================
// ����������
//=======================================
void CPile::Init(void)
{
	m_TrunkModelID = RNLib::Model().Load("data\\MODEL\\Trunk.x");

	m_nTex[0] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Smoke_000.png");
	m_nTex[1] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Smoke_001.png");

	//�O�����ۑ�
	SetOld(0.0f);
}

//=======================================
// �I������
//=======================================
void CPile::Uninit(void)
{

}

//=======================================
// �X�V����
//=======================================
void CPile::Update(void)
{
	//�߂荞�݃��Z�b�g
	if (RNLib::Input().GetKeyTrigger(DIK_L)) m_TrunkHeight = m_StartTrunkHeight;

	//�O�����ۑ�
	SetOld(m_TrunkHeight);

	//���f���z�u
	PutModel();
}

//===============================
//���f���z�u
//===============================
void CPile::PutModel(void)
{
	//�����W
	Pos3D PilePos = m_PilePos;

	//��ԏ�̊�����z�u���邽�߁A�z�u���鍂�����v�Z
	{
		//�����̔����i�؂�̂āj
		const int nNumHalf = m_NumTrunk / EVENPARITY;

		//�{�̃T�C�Y  �{  �T�C�Y �~ ��̊���  �{  ���̂߂荞�ݗ�
		PilePos.y = SIZE_OF_1_SQUARE * -0.5f + SIZE_OF_1_SQUARE * nNumHalf + m_TrunkHeight;

		RNLib::Text2D().PutDebugLog(CreateText(
			"�YY�ʒu:%.2f  �߂荞��:%.2f  ����:%.2f  MaxY:%.2f  MinY:%.2f  �␳�l:%.2f",
			PilePos.y,	 m_TrunkHeight, m_height, m_pos.y + m_height, m_pos.y - m_height, m_fEvenTrunkCorrHeight));
	}

	for (int nCntPile = 0; nCntPile < m_NumTrunk; nCntPile++)
	{
		//�����f��
		RNLib::Model().Put(PRIORITY_OBJECT, m_TrunkModelID, PilePos, INITD3DXVECTOR3);

		//�z�u���W��������
		PilePos.y -= SIZE_OF_1_SQUARE;
	}
}

//===============================
//�ݒ菈��
//�����ɕK�v�����i�Œ�3�j��z�u���W���w�肷��
//-------------------------------
//�����P  pos�F�z�u���W
//�����Q  NumTrunk�F���̐��i�Œ�R�j
//�����R  TrunkHeight�F���̂߂荞�ݍ��W�i�߂荞�݂Ȃ� = 0.0f�j
//===============================
void CPile::Set(Pos3D pos, int NumTrunk, float TrunkHeight)
{
	//�������Œᐔ��������Ă�����C��
	if (NumTrunk < MIN_TRUNK) NumTrunk = MIN_TRUNK;

	//���ݒ�
	m_PilePos = Initpos = pos ;
	m_NumTrunk = NumTrunk;
	m_TrunkHeight = TrunkHeight;
	m_StartTrunkHeight = TrunkHeight;
	m_fEvenTrunkCorrHeight = NumTrunk % EVENPARITY == 0 ? SIZE_OF_1_SQUARE * 0.5f : 0.0f;

	//�����蔻��̍������Đݒ�
	m_height = SIZE_OF_1_SQUARE * NumTrunk;

	//���f���z�u
	PutModel();
}

//===============================
//�߂荞�ݗʔ��f����
//===============================
void CPile::CaveInTrunkHeight(float fCaveInHeight)
{
	//�����ۑ�
	float fCaveTemp = fCaveInHeight;

	//�ւ��ݗʔ��f
	fCaveInHeight += m_TrunkHeight;

	//�Y�̍ő�E�Œፂ�����v�Z
	const int nNumHalf = m_NumTrunk / EVENPARITY;
	const float fMaxHeight = SIZE_OF_1_SQUARE * nNumHalf - m_fEvenTrunkCorrHeight * 2.0f;
	const float fMinHeight = -SIZE_OF_1_SQUARE * nNumHalf;

	//�Y�������Ȃ��悤�ɒ���
	if (fCaveInHeight < fMinHeight)		fCaveInHeight = fMinHeight;
	else if (fCaveInHeight > fMaxHeight)fCaveInHeight = fMaxHeight;

	//�O�����ۑ�
	SetOld(fCaveInHeight);

	//���f���z�u
	PutModel();

	//�p�[�e�B�N��
	D3DXVECTOR3 rot = INITD3DXVECTOR3;
	float m_ScaleTex,world = 0.0f,side = 0.785f;
	int RandNum;

	if (fCaveTemp > 0.0f){
		world = -D3DX_PI;
		side = -0.785f;
	}

	for (int Cnt = 0; Cnt < 24; Cnt++){
		D3DXVECTOR3 m_TexPos = D3DXVECTOR3(Initpos.x + (float)(rand() % (int)m_width - m_width * 0.5), Initpos.y, Initpos.z);

		RandNum = rand() % 2;
		if (RandNum == 0)
			rot = D3DXVECTOR3(0.0f,0.0f, world + side);
		else
			rot = D3DXVECTOR3(0.0f, 0.0f, world - side);

		m_ScaleTex = (float)(rand() % (int)(INIT_EFFECT_SCALE.x * 0.6f) + INIT_EFFECT_SCALE.x * 0.6f);
		Manager::EffectMgr()->ParticleCreate(m_nTex[RAND_TEX], m_TexPos, D3DXVECTOR3(m_ScaleTex, m_ScaleTex, 0.0f), Color{ 255,255,155,30 }, CParticle::TYPE::TYPE_FLOATUP, 300, rot,16,CDrawState::ALPHA_BLEND_MODE::NORMAL);

		rot = INITD3DXVECTOR3;
	}
}

//===============================
//�O�����ۑ�
//===============================
void CPile::SetOld(float fCaveInHeight)
{
	//�߂荞�ݗʂ𔽉f������
	m_TrunkHeightOld = m_TrunkHeight;
	m_TrunkHeight = fCaveInHeight;

	m_pos = D3DXVECTOR3(m_PilePos.x, m_PilePos.y + m_TrunkHeight + m_fEvenTrunkCorrHeight, 0.0f);
	m_posOld = D3DXVECTOR3(m_PilePos.x, m_PilePos.y + m_TrunkHeightOld + m_fEvenTrunkCorrHeight, 0.0f);

	//D3DXVECTOR3 pos = m_pos;
	//pos.z = -30.0f;
	//Manager::EffectMgr()->EffectCreate(RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Star_000.png"), pos, INIT_EFFECT_SCALE, INITCOLOR, 1);
}

//===============================
//�ʒu�擾�i�߂荞�ݗʂ��܂߂�j
//===============================
D3DXVECTOR3 CPile::GetPosCaveIn(void)
{
	D3DXVECTOR3 ReturnPos = m_pos;	//���ݍ��W���i�[
	ReturnPos.y += m_fEvenTrunkCorrHeight;//�␳�l�𔽉f
	return m_pos;//���W��Ԃ�
}

//===============================
//�O��ʒu�擾�i�߂荞�ݗʂ��܂߂�j
//===============================
D3DXVECTOR3 CPile::GetPosOldCaveIn(void)
{
	D3DXVECTOR3 ReturnPos = m_pos;	//���ݍ��W���i�[
	ReturnPos.y += m_TrunkHeightOld;//�O��̂߂荞�ݗʂ𔽉f
	ReturnPos.y += m_fEvenTrunkCorrHeight;//�␳�l�𔽉f
	return ReturnPos;				//���W��Ԃ�
}