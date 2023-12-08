//========================================
// 
// �X�e�[�W�I�u�W�F�N�g�}�l�[�W���[�̏���
// Author:KEISUKE OTONO
// Arrange:RIKU NISHIMURA
// 
//========================================
#include "../main.h"
#include "../UI/partsUI.h"

//========================================
// �ÓI�ϐ�
//========================================
CStageObjectMgr::BlockType *CStageObjectMgr::m_pBlockType = NULL;
const char* CStageObjectMgr::BLOCK_INFO_FILE = "data\\GAMEDATA\\BLOCK\\BLOCK_DATA.txt";

//========================================
// �R���X�g���N�^
//========================================
CStageObjectMgr::CStageObjectMgr(void):CObjectMgr("StageObjectMgr")
{
	
}

//========================================
// �f�X�g���N�^
//========================================
CStageObjectMgr::~CStageObjectMgr()
{
	delete m_pBlockType;
	m_pBlockType = NULL;
}

//========================================
// �X�V
//========================================
void CStageObjectMgr::Update(void)
{
	
}

//========================================
// �ǂݍ���
//========================================
void CStageObjectMgr::Load(void)
{
	int nCntModel = 0;
	char aDataSearch[TXT_MAX];	// �f�[�^�����p

	// �t�@�C���̓ǂݍ���
	FILE *pFile = fopen(BLOCK_INFO_FILE, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
	 //�������I������
		return;
	}

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if (!strcmp(aDataSearch, "END"))
		{// �ǂݍ��݂��I��
			fclose(pFile);
			break;
		}
		if (aDataSearch[0] == '#')
		{// �܂�Ԃ�
			continue;
		}

		if (!strcmp(aDataSearch, "NUM_MODEL"))
		{
			int nMaxType = -1;

			fscanf(pFile, "%s", &aDataSearch[0]);
			fscanf(pFile, "%d", &nMaxType);		// �ő吔
			m_pBlockType = new BlockType[nMaxType];
			assert(m_pBlockType != NULL);
		}
		else if (!strcmp(aDataSearch, "MODEL"))
		{
			fscanf(pFile, "%s", &aDataSearch[0]);
			fscanf(pFile, "%s", &m_pBlockType[nCntModel].aFileName[0]);	// �t�@�C����

			m_pBlockType[nCntModel].nModelIdx = RNLib::Model().Load(m_pBlockType[nCntModel].aFileName);	// ���f���ԍ�
			nCntModel++;
		}
	}
}

//========================================
// �e����
//========================================

//========================================
// �u���b�N
//========================================
CBlock *CStageObjectMgr::BlockCreate(D3DXVECTOR3 pos,CBlock::LOOKS_TYPE type, Color color, bool isCollision)
{
	CBlock *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CBlock;

	// ����������
	pObj->SetPos(pos);
	pObj->SetColor(color);
	pObj->SetCollision(isCollision);
	pObj->Init(type);

	return pObj;
}

//========================================
// �u���b�N(����@��)
//========================================
CFillBlock *CStageObjectMgr::FillBlockCreate(D3DXVECTOR3 pos,CFillBlock::BLOCKTYPE type, Color color)
{
	//�v���C���[�C���X�^���X�̐���
	CFillBlock *pObj = new CFillBlock;

	//����������
	pObj->Init(type);

	pObj->SetPos(pos);
	pObj->SetColor(color);

	//�v���C���[��Ԃ�
	return pObj;
}

//========================================
// ��
//========================================
CSpike *CStageObjectMgr::SpikeCreate(D3DXVECTOR3 pos, int nType)
{
	CSpike *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CSpike;

	// ����������
	pObj->Init();

	switch (nType)
	{
	case 0:
		if (pos.y <= 0) {
			pObj->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI));
			pos.y += CStageObject::SIZE_OF_1_SQUARE * 0.25f;
		}
		else {
			pos.y -= CStageObject::SIZE_OF_1_SQUARE * 0.25f;
		}
		break;
	case 1:
		pObj->SetRot(D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI/2));
		pos.x -= CStageObject::SIZE_OF_1_SQUARE * 0.25f;
		break;
	case 2:
		pObj->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI / 2));
		pos.x += CStageObject::SIZE_OF_1_SQUARE * 0.25f;
		break;
	}

	pObj->SetPos(pos);
	
	return pObj;
}

//========================================
// �}�O�}�u���b�N
//========================================
CMagmaBlock *CStageObjectMgr::MagmaBlockCreate(D3DXVECTOR3 pos, Color color)
{
	CMagmaBlock *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CMagmaBlock;

	// ����������
	pObj->Init();
	pObj->SetPos(pos);
	pObj->SetColor(color);

	return pObj;
}

//========================================
// �g�����|����
//========================================
CTrampoline *CStageObjectMgr::TrampolineCreate(D3DXVECTOR3 pos)
{
	CTrampoline *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CTrampoline;

	// ����������
	pObj->SetPos(pos);
	pObj->Init();

	return pObj;
}

//========================================
// 覐�
//========================================
CMeteor *CStageObjectMgr::MeteorCreate(D3DXVECTOR3 pos,D3DXVECTOR3 move)
{
	CMeteor *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CMeteor;

	// ����������
	pObj->SetPos(pos);
	pObj->SetMove(move);
	pObj->Init();


	return pObj;
}

//========================================
// �ړ���
//========================================
CMoveBlock *CStageObjectMgr::MoveBlockCreate(D3DXVECTOR3 posV, D3DXVECTOR3 posL, D3DXVECTOR3 move, bool bxRetrurn)
{
	CMoveBlock *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CMoveBlock;

	// ����������
	pObj->SetPos(posV);
	pObj->SetMove(move);
	pObj->SetPosInfo(posV,posL);

	if (move.x <= 0)
		bxRetrurn = true;

	pObj->SetXReturn(bxRetrurn);

	pObj->Init();

	return pObj;
}

//========================================
// �Y
//========================================
CPile *CStageObjectMgr::PileCreate(D3DXVECTOR3 pos, int nNumPile, float fCaveIn)
{
	CPile *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CPile;

	// ����������
	pObj->Init();
	pObj->Set(pos, nNumPile, fCaveIn);

	return pObj;
}

//========================================
// �p�[�c
//========================================
CParts *CStageObjectMgr::PartsCreate(D3DXVECTOR3 pos)
{
	CParts *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CParts;

	// ����������
	pObj->Init();
	pObj->SetPos(pos);

	if (pos.y <= 0) {
		pObj->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI));
	}

	return pObj;
}

//========================================
// �R�C��
//========================================
CCoin *CStageObjectMgr::CoinCreate(D3DXVECTOR3 pos)
{
	CCoin *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CCoin;

	// ����������
	pObj->SetPos(pos);

	return pObj;
}

//========================================
// ���P�b�g
//========================================
CRocket *CStageObjectMgr::RocketCreate(D3DXVECTOR3 pos)
{
	CRocket *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CRocket;

	// ����������
	pObj->SetPos(pos);
	pObj->Init();

	return pObj;
}

//========================================
// �S�[���Q�[�g
//========================================
CGoalGate *CStageObjectMgr::GoalGateCreate(D3DXVECTOR3 pos,bool bStartGate)
{
	CGoalGate *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CGoalGate;

	// ����������
	pObj->Init();
	pObj->SetPos(pos);
	pObj->SetStartGate(bStartGate);

	return pObj;
}

//========================================
// �f��
//========================================
CPlanet *CStageObjectMgr::PlanetCreate(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CPlanet::STAR_TYPE type, Color col)
{
	CPlanet *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CPlanet;

	// ����������
	pObj->SetType(type);
	pObj->SetPos(pos);
	pObj->SetCol(col);
	pObj->SetRot(rot);
	pObj->Init();
	

	return pObj;
}

//========================================
// ��
//========================================
CStar *CStageObjectMgr::StarCreate(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CStar *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CStar;

	// ����������
	pObj->SetPos(pos);
	pObj->SetRot(rot);
	pObj->Init();


	return pObj;
}

//========================================
// �������[�U�[
//========================================
CRoadTripLaser *CStageObjectMgr::RoadTripLaserCreate(D3DXVECTOR3 posV, D3DXVECTOR3 posL, D3DXVECTOR3 move,bool bXReturn)
{
	CRoadTripLaser *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CRoadTripLaser;

	// ����������

	if (posV.y <= 0) {
		pObj->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI));
	}

	pObj->SetMove(move);
	pObj->SetPosInfo(posV,posL);

	if (move.x <= 0)
		bXReturn = true;

	pObj->SetXMoveReturn(bXReturn);

	pObj->Init();

	return pObj;
}

//========================================
//  ������
//========================================
CExtenddog	 *CStageObjectMgr::ExtenddogCreate(D3DXVECTOR3 fHeadpos, D3DXVECTOR3 fHippos,int fHeadheight, bool bShrink,bool bReturn)
{
	CExtenddog *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CExtenddog;

	// ����������
	pObj->SetPos(fHeadpos);
	pObj->SetShrink(bShrink);
	pObj->SetHead(fHeadpos);
	pObj->SetHip(fHippos);
	pObj->SetHeadHeight(fHeadheight);
	pObj->SetReturn(bReturn);
	pObj->Init();


	return pObj;
}

//========================================
//  覐΃W�F�l���[�^�[
//========================================
CMeteorGenerator *CStageObjectMgr::MeteorGeneratorCreate(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nSummonInterval)
{
	CMeteorGenerator *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CMeteorGenerator;

	// ����������
	pObj->SetPos(pos);
	pObj->SetMove(move);
	pObj->SetInterval(nSummonInterval);
	pObj->Init();

	return pObj;
}
//========================================
//  �R�E����
//========================================
CBat * CStageObjectMgr::BatCreate(D3DXVECTOR3 pos)
{
	CBat *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CBat;

	// ����������
	pObj->SetPos(pos);
	pObj->Init();

	return pObj;
}

