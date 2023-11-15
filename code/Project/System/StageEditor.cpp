//========================================
// 
// �X�e�[�W�G�f�B�b�g�̓ǂݍ���
// Author:KEISUKE OTONO
// 
//========================================
#include "../manager.h"
#include "csv_file.h"
#include "../Object/stage-object.h"
#include "../Mode/mode_game.h"
#include "../Character/player.h"
#include "StageEditor.h"
#include "../Mode/mode_title.h"

//========================================
// �ÓI�ϐ�
//========================================
CStageEditor::PlanetType *CStageEditor::m_PlanetType = NULL;
CStageEditor::StageColor CStageEditor::m_StageColor = {NULL};
const char* CStageEditor::STAGE_INFO_FILE = "data\\GAMEDATA\\STAGE\\STAGE_FILE.txt";

//========================================
// �R���X�g���N�^
//========================================
CStageEditor::CStageEditor(void)
{
	if (m_PlanetType != NULL){
		Uninit();
	}
	else{
		m_PlanetType = NULL;
	}

	{
		m_StageColor.Set = INITCOLOR;
		m_StageColor.Player1 = INITCOLOR;
		m_StageColor.Player2 = INITCOLOR;
		m_StageColor.Block = INITCOLOR;
		m_StageColor.FillBlock = INITCOLOR;
	}

	m_Info.nRow = 0;
	m_Info.nLine = 0;
	m_Info.nRowMax = 0;
	m_Info.nLineMax = 0;
	m_Info.nPlanetIdx = 0;
	m_Info.aBgFile = NULL;
	m_Info.aSoundFile = NULL;

	// �ő�l
	m_Info.nPlanetMax = 0;
	m_Info.nLiftMax = 0;
	m_Info.nMateorMax = 0;
	m_Info.nLaserMax = 0;
	m_Info.nDogMax = 0;
	m_Info.nPileMax = 0;

	m_LiftInfo = NULL;
	m_MeteorInfo = NULL;
	m_LaserInfo = NULL;
	m_DogInfo = NULL;
	m_PileInfo = NULL;
}

//========================================
// �f�X�g���N�^
//========================================
CStageEditor::~CStageEditor()
{
	if (m_PlanetType != NULL){
		for (int nPlanet = 0; nPlanet < m_Info.nPlanetMax; nPlanet++)
		{
			if (m_PlanetType[nPlanet].StageType != NULL)
			{
				delete[] m_PlanetType[nPlanet].StageType;
				m_PlanetType[nPlanet].StageType = NULL;
			}
		}

		delete[] m_PlanetType;
		m_PlanetType = NULL;
	}

	if (m_LiftInfo != NULL){
		delete[] m_LiftInfo;
		m_LiftInfo = NULL;
	}

	if (m_MeteorInfo != NULL){
		delete[] m_MeteorInfo;
		m_MeteorInfo = NULL;
	}

	if (m_LaserInfo != NULL){
		delete[] m_LaserInfo;
		m_LaserInfo = NULL;
	}

	if (m_DogInfo != NULL){
		delete[] m_DogInfo;
		m_DogInfo = NULL;
	}

	if (m_PileInfo != NULL) {
		delete[] m_PileInfo;
		m_PileInfo = NULL;
	}

	if (m_Info.aBgFile != NULL){
		delete[] m_Info.aBgFile;
		m_Info.aBgFile;
	}

	if (m_Info.aSoundFile != NULL){
		delete[] m_Info.aSoundFile;
		m_Info.aSoundFile;
	}
}

//========================================
// �I������
// Author:KEISUKE OTONO
//========================================
void CStageEditor::Uninit(void)
{
	if (m_PlanetType != NULL)
	{
		for (int nPlanet = 0; nPlanet < m_Info.nPlanetMax; nPlanet++)
		{
			if (m_PlanetType[nPlanet].StageType != NULL)
			{
				delete[] m_PlanetType[nPlanet].StageType;
				m_PlanetType[nPlanet].StageType = NULL;
			}
		}

		delete[] m_PlanetType;
		m_PlanetType = NULL;
	}

	if (m_LiftInfo != NULL) {
		delete[] m_LiftInfo;
		m_LiftInfo = NULL;
	}

	if (m_MeteorInfo != NULL) {
		delete[] m_MeteorInfo;
		m_MeteorInfo = NULL;
	}

	if (m_LaserInfo != NULL) {
		delete[] m_LaserInfo;
		m_LaserInfo = NULL;
	}

	if (m_DogInfo != NULL) {
		delete[] m_DogInfo;
		m_DogInfo = NULL;
	}

	if (m_PileInfo != NULL) {
		delete[] m_PileInfo;
		m_PileInfo = NULL;
	}

	if (m_Info.aBgFile != NULL) {
		delete[] m_Info.aBgFile;
		m_Info.aBgFile = NULL;
	}

	if (m_Info.aSoundFile != NULL) {
		delete[] m_Info.aSoundFile;
		m_Info.aSoundFile = NULL;
	}
}

//========================================
// �t�@�C���ǂݍ���
// Author:KEISUKE OTONO
//========================================
void CStageEditor::FileLoad(void)
{
	int nCntPlanet = 0;
	int *pCntStage = NULL;
	char aDataSearch[TXT_MAX];	// �f�[�^�����p

	// �t�@�C���̓ǂݍ���
	FILE *pFile = fopen(STAGE_INFO_FILE, "r");

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

			if (pCntStage != NULL)
			{
				delete[] pCntStage;
				pCntStage = NULL;
			}

			break;
		}
		if (aDataSearch[0] == '#')
		{// �܂�Ԃ�
			continue;
		}

		if (!strcmp(aDataSearch, "NUM_PLANET"))
		{
			int nMax = -1;

			fscanf(pFile, "%s", &aDataSearch[0]);
			fscanf(pFile, "%d", &nMax);		// �ő吔

			if (nMax <= 0)
			{
				nMax = 0;
			}

			m_Info.nPlanetMax = nMax;
			m_PlanetType = new PlanetType[nMax];
			assert(m_PlanetType != NULL);

			pCntStage = new int[nMax];

			for (int nCnt = 0; nCnt < nMax; nCnt++)
			{
				pCntStage[nCnt] = 0;
			}

		}
		else if (!strcmp(aDataSearch, "PLANET"))
		{
			if (nCntPlanet < m_Info.nPlanetMax)
			{
				int StageMax = 0;

				fscanf(pFile, "%s", &aDataSearch[0]);
				fscanf(pFile, "%d", &StageMax);	// �X�e�[�W��

				m_PlanetType[nCntPlanet].nStageMax = StageMax;
				m_PlanetType[nCntPlanet].StageType = new StageType[StageMax];
				assert(m_PlanetType[nCntPlanet].StageType != NULL);

				fscanf(pFile, "%s", &m_PlanetType[nCntPlanet].aTexFile[0]);	// �e�N�X�`���p�X
				fscanf(pFile, "%s", &m_PlanetType[nCntPlanet].aName[0]);	// �f����

				nCntPlanet++;
			}
		}
		else if (!strcmp(aDataSearch, "STAGE"))
		{
			int nPlanet = -1;

			fscanf(pFile, "%s", &aDataSearch[0]);
			fscanf(pFile, "%d", &nPlanet);	// �f���ԍ�

			if (pCntStage[nPlanet] < m_PlanetType[nPlanet].nStageMax)
			{
				fscanf(pFile, "%s", &m_PlanetType[nPlanet].StageType[pCntStage[nPlanet]].aFileName[0]);	// �t�@�C����
				fscanf(pFile, "%s", &m_PlanetType[nPlanet].StageType[pCntStage[nPlanet]].aName[0]);		// �X�e�[�W��

				pCntStage[nPlanet]++;
			}
		}
	}
}

//========================================
// �X�e�[�W�ǂݍ���
// Author:KEISUKE OTONO
//========================================
void CStageEditor::StageLoad(int planet, int stage)
{
	//�v���C���[������
	CMode_Game::GetPlayer()->Init();

	CSVFILE *pFile = new CSVFILE;

	m_Info.nPlanetIdx = planet;
	m_PlanetType[planet].nStageIdx = stage;
	bool bSet = true;
	bool bEnd = false;

	IntControl(&m_PlanetType[planet].nStageIdx, m_PlanetType[planet].nStageIdx, 0);

	// �ǂݍ���
	pFile->FileLood(m_PlanetType[planet].StageType[stage].aFileName, false, false, ',');

	// �s���̎擾
	int nRowMax = pFile->GetRowSize();

	// �e�f�[�^�ɑ��
	for (int nRow = 0; nRow < nRowMax; nRow++)
	{
		// �z�u���̐���
		int nType = -1;				// ���

		// �񐔂̎擾
		int nLineMax = pFile->GetLineSize(nRow);

		for (int nLine = 0; nLine < nLineMax; nLine++)
		{
			if (bEnd) { break; }
			char *aDataSearch = NULL;	// �f�[�^�����p
			string sData = pFile->GetData(nRow, nLine);
			char* cstr = new char[sData.size() + 1]; // �������m��
			std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
			aDataSearch = cstr;
			

			if (!strcmp(aDataSearch, "End")) { bEnd = true;}
			else if (!strcmp(aDataSearch, "SetBg")) {
				nLine += 4;

				if (m_Info.aBgFile == NULL){
					string sData = pFile->GetData(nRow, nLine);
					m_Info.aBgFile = new char[sData.size() + 1]; // �������m��
					std::char_traits<char>::copy(m_Info.aBgFile, sData.c_str(), sData.size() + 1);
				}

			}
			else if (!strcmp(aDataSearch, "SetSound")) {
				nLine += 4;

				if (m_Info.aSoundFile == NULL) {
					string sData = pFile->GetData(nRow, nLine);
					m_Info.aSoundFile = new char[sData.size() + 1]; // �������m��
					std::char_traits<char>::copy(m_Info.aSoundFile, sData.c_str(), sData.size() + 1);
				}
				
			}
			else if (!strcmp(aDataSearch, "StageWidth")){
				nLine += 4;
				int nWidth;
				
				ToData(nWidth, pFile, nRow, nLine);
				m_Info.nLine = 0;
				m_Info.nLineMax = nWidth;
			}
			else if (!strcmp(aDataSearch, "StageHeight")){
				nLine += 4;
				int nHeight;

				ToData(nHeight, pFile, nRow, nLine);
				m_Info.nRow = 0;
				m_Info.nRowMax = nHeight;
			}
			else if (!strcmp(aDataSearch, "SetColor")){
				StgColor(pFile,nRow,nLine);
			}
			else if (!strcmp(aDataSearch, "SetStage")){
				nRow++;
				// �X�e�[�W����
				while (1)
				{
					char *aDataSearch = NULL;	// �f�[�^�����p
					string sData = pFile->GetData(nRow, nLine);
					char* cstr = new char[sData.size() + 1]; // �������m��
					std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
					aDataSearch = cstr;

					if (!strcmp(aDataSearch, "EndStage")) {
						if (cstr != NULL) {
							delete[] cstr;
							cstr = NULL;
						}
						break;
					}
					else
					{
						for (m_Info.nLine = 0; m_Info.nLine < m_Info.nLineMax; m_Info.nLine++)
						{
						int nType = -1;
							ToData(nType, pFile, nRow, m_Info.nLine);
							SetStage(nType);
						}
						nRow++;
						m_Info.nRow++;
					}

					if (cstr != NULL)
					{
						delete[] cstr;
						cstr = NULL;
					}
				}

			}
			else if (!strcmp(aDataSearch, "SetLiftInfo")) {
				SetLiftInfo(pFile, nRow, nLine);
			}
			else if (!strcmp(aDataSearch, "SetMeteorInfo")) {
				SetMeteorInfo(pFile, nRow, nLine);
			}
			else if (!strcmp(aDataSearch, "SetLaserInfo")) {
				SetLaserInfo(pFile, nRow, nLine);
			}
			else if (!strcmp(aDataSearch, "SetDogInfo")){
				SetDogInfo(pFile, nRow, nLine);
			}
			else if (!strcmp(aDataSearch, "SetPileInfo")) {
				SetPileInfo(pFile, nRow, nLine);
			}

			if (cstr != NULL){
				delete[] cstr;
				cstr = NULL;
			}
		}

		// �ő吔�ɒB������Ԃ�
		if (nRow == nRowMax || bEnd)
		{
			break;
		}
	}

	// �������J��
	delete pFile;
	pFile = NULL;
}

//========================================
// �F���
// Author:KEISUKE OTONO
//========================================
void CStageEditor::StgColor(CSVFILE *pFile, int nRow, int nLine)
{
	while (1)
	{
		nLine = 0;
		nRow++;
		char *aDataSearch = NULL;	// �f�[�^�����p
		string sData = pFile->GetData(nRow, nLine);
		char* cstr = new char[sData.size() + 1]; // �������m��
		std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
		aDataSearch = cstr;

		if (!strcmp(aDataSearch, "EndColor")) {
			if (cstr != NULL) {
				delete[] cstr;
				cstr = NULL;
			}
			break;
		}
		else if (!strcmp(aDataSearch, "1P"))
		{
			SetColor(pFile, nRow, nLine);
			m_StageColor.Player1 = m_StageColor.Set;
			CMode_Game::GetPlayer()->SetColor(0, m_StageColor.Player1);
		}
		else if (!strcmp(aDataSearch, "2P"))
		{
			SetColor(pFile, nRow, nLine);
			m_StageColor.Player2 = m_StageColor.Set;
			CMode_Game::GetPlayer()->SetColor(1, m_StageColor.Player2);
		}
		else if (!strcmp(aDataSearch, "BgUp"))
		{
			SetColor(pFile, nRow, nLine);
			CMode_Game::SetBgUpColor(m_StageColor.Set);
		}
		else if (!strcmp(aDataSearch, "BgDown"))
		{
			SetColor(pFile, nRow, nLine);
			CMode_Game::SetBgDownColor(m_StageColor.Set);
		}
		else if (!strcmp(aDataSearch, "Block"))
		{
			SetColor(pFile, nRow, nLine);
			m_StageColor.Block = m_StageColor.Set;
		}
		else if (!strcmp(aDataSearch, "FillBlock"))
		{
			SetColor(pFile, nRow, nLine);
			m_StageColor.FillBlock = m_StageColor.Set;
		}

		if (cstr != NULL)
		{
			delete[] cstr;
			cstr = NULL;
		}
	}
}

//========================================
// �F�ݒ�
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SetColor(CSVFILE *pFile, int nRow, int nLine)
{
	nLine += 4;
	ToData(m_StageColor.Set.r, pFile, nRow, nLine); nLine++;
	ToData(m_StageColor.Set.g, pFile, nRow, nLine); nLine++;
	ToData(m_StageColor.Set.b, pFile, nRow, nLine); nLine++;
	ToData(m_StageColor.Set.a, pFile, nRow, nLine); nLine++;
}

//========================================
// �X�e�[�W����
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SetStage(int nType)
{
	if (nType >= 0)
	{
		float fSizeX = CStageObject::SIZE_OF_1_SQUARE;
		float fSizeY = CStageObject::SIZE_OF_1_SQUARE;
		D3DXVECTOR3 pos = Manager::GetMainCamera()->GetPosR();

		pos.x += ((m_Info.nLineMax * -0.5f) + m_Info.nLine + 0.5f) * fSizeX;
		pos.y -= ((m_Info.nRowMax * -0.5f) + m_Info.nRow + 0.5f) * fSizeY;

		pos.z = 0.0f/* + fRand() * 4.0f*/;

		// �z�u
		ObjPlace(fSizeX,fSizeY,pos,nType);
	}
}

//========================================
// �X�e�[�W�؂�ւ�
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SwapStage(int nStageIdx)
{
	int planet = m_Info.nPlanetIdx;
	int stage = m_PlanetType[planet].nStageIdx;
	int NecstStage = nStageIdx;

	if (stage != NecstStage)
	{
		if (NecstStage >= m_PlanetType[planet].nStageMax)
		{
			planet++;
			NecstStage = 0;
		}

		if (planet < m_Info.nPlanetMax)
		{
			if (RNLib::Transition().GetState() == CTransition::STATE::NONE)
			{
				Manager::Transition(CMode::TYPE::GAME, CTransition::TYPE::FADE);
				CMode_Game::SetStage(planet, NecstStage);
			}
		}
		else
		{
			Manager::Transition(CMode::TYPE::TITLE, CTransition::TYPE::FADE);
			CMode_Title::SetSelect(true);
		}
	}
}

//========================================
// �I�u�W�F�N�g�z�u
// Author:KEISUKE OTONO
//========================================
void CStageEditor::ObjPlace(float fSizeX, float fSizeY, D3DXVECTOR3 pos, int nType)
{
	switch (nType)
	{
	case TYPE_BLOCK:
		Manager::BlockMgr()->BlockCreate(pos,CBlock::BLOCK_TYPE::BLOCK_NORMAL, m_StageColor.Block);
		break;
	case TYPE_TRAMPOLINE:
		pos.x += fSizeX / 2;
		Manager::BlockMgr()->TrampolineCreate(pos);
		break;
	case TYPE_SPIKE:
		Manager::BlockMgr()->SpikeCreate(pos,0);
		break;
	case TYPE_SPIKE_L:
		Manager::BlockMgr()->SpikeCreate(pos, 1);
		break;
	case TYPE_SPIKE_R:
		Manager::BlockMgr()->SpikeCreate(pos, 2);
		break;
	case TYPE_LIFT:
		Manager::BlockMgr()->MoveBlockCreate(pos, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		break;
	case TYPE_Meteor:
		pos.x += fSizeX;
		pos.y -= fSizeY;
		Manager::BlockMgr()->MeteorCreate(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		break;
	case TYPE_Laser:
		Manager::BlockMgr()->RoadTripLaserCreate(pos, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		break;
	case TYPE_Extenddog:
		Manager::BlockMgr()->BlockCreate(pos,CBlock::BLOCK_TYPE::BLOCK_NUI);
		break;
	case TYPE_FILL_BLOCK_11:
		Manager::BlockMgr()->FillBlockCreate(pos, CFillBlock::FILL_TYPE::FILL_1x1, m_StageColor.FillBlock);
		break;
	case TYPE_FILL_BLOCK_12:
		pos.y -= fSizeX * 0.5f;
		Manager::BlockMgr()->FillBlockCreate(pos, CFillBlock::FILL_TYPE::FILL_1x2, m_StageColor.FillBlock);
		break;
	case TYPE_FILL_BLOCK_13:
		pos.y -= fSizeX;
		Manager::BlockMgr()->FillBlockCreate(pos, CFillBlock::FILL_TYPE::FILL_1x3, m_StageColor.FillBlock);
		break;
	case TYPE_FILL_BLOCK_21:
		pos.x += fSizeY * 0.5f;
		Manager::BlockMgr()->FillBlockCreate(pos, CFillBlock::FILL_TYPE::FILL_2x1, m_StageColor.FillBlock);
		break;
	case TYPE_FILL_BLOCK_22:
		pos.x += fSizeX * 0.5f;
		pos.y -= fSizeY * 0.5f;
		Manager::BlockMgr()->FillBlockCreate(pos, CFillBlock::FILL_TYPE::FILL_2x2, m_StageColor.FillBlock);
		break;
	case TYPE_FILL_BLOCK_31:
		pos.x += fSizeY;
		Manager::BlockMgr()->FillBlockCreate(pos, CFillBlock::FILL_TYPE::FILL_3x1, m_StageColor.FillBlock);
		break;
	case TYPE_FILL_BLOCK_33:
		pos.x += fSizeX;
		pos.y -= fSizeY;
		Manager::BlockMgr()->FillBlockCreate(pos, CFillBlock::FILL_TYPE::FILL_3x3, m_StageColor.FillBlock);
		break;
	case TYPE_FILL_BLOCK_44:
		pos.x += fSizeX * 1.5f;
		pos.y -= fSizeY * 1.5f;
		Manager::BlockMgr()->FillBlockCreate(pos, CFillBlock::FILL_TYPE::FILL_4x4, m_StageColor.FillBlock);
		break;
	case TYPE_PLAYER_0:
		pos.y += fSizeY * 0.5f;
		CMode_Game::GetPlayer()->SetPos(0, pos);
		break;
	case TYPE_PLAYER_1:
		pos.y += -fSizeY * 0.5f;
		CMode_Game::GetPlayer()->SetPos(1, pos);
		break;
	case TYPE_GOALGATE:
		Manager::BlockMgr()->GoalGateCreate(pos);
		break;
	case TYPE_PARTS:
		Manager::BlockMgr()->PartsCreate(pos);
		break;
	case TYPE_GOAL:
		pos.x += fSizeX;
		pos.y -= fSizeY;
		Manager::BlockMgr()->RocketCreate(pos);
		break;
	}
}

//========================================
// ���t�g�̏��ݒ�
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SetLiftInfo(CSVFILE *pFile, int nRow, int nLine)
{
	int nLift = 0;

	while (1)
	{
		nLine = 0;	nRow++;
		char *aDataSearch = NULL;	// �f�[�^�����p
		string sData = pFile->GetData(nRow, nLine);
		char* cstr = new char[sData.size() + 1]; // �������m��
		std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
		aDataSearch = cstr;

		if (!strcmp(aDataSearch, "EndLiftInfo")) {
			if (cstr != NULL) {
				delete[] cstr;
				cstr = NULL;
			}
			break;
		}
		else if (!strcmp(aDataSearch, "LiftMax")) {
			int Max = 0;
			nLine += 4;
			ToData(Max, pFile, nRow, nLine);

			m_Info.nLiftMax = Max;
			m_LiftInfo = new Liftinfo[Max];
			assert(m_LiftInfo != NULL);
		}
		else if (!strcmp(aDataSearch, "SetLift")) {
			while (1)
			{
				nLine = 0;	nRow++;				char *aDataSearch = NULL;	// �f�[�^�����p
				string sData = pFile->GetData(nRow, nLine);
				char* cstr = new char[sData.size() + 1]; // �������m��
				std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
				aDataSearch = cstr;

				if (!strcmp(aDataSearch, "EndLift")) {
					if (cstr != NULL) {
						delete[] cstr;
						cstr = NULL;
					}

					if (nLift < m_Info.nLiftMax){
						Manager::BlockMgr()->MoveBlockCreate(m_LiftInfo[nLift].posV, m_LiftInfo[nLift].posR, m_LiftInfo[nLift].move);
					}

					nLift++;
					break;
				}
				else if (!strcmp(aDataSearch, "PosV")) {
					int Row = 0; int Line = 4; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_LiftInfo[nLift].posV = GetPos(Row, Line);
				}
				else if (!strcmp(aDataSearch, "PosR")) {
					int Row = 0; int Line = 0; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_LiftInfo[nLift].posR = GetPos(Row, Line);
				}
				else if (!strcmp(aDataSearch, "Move")) {
					nLine += 4;
					ToData(m_LiftInfo[nLift].move.x, pFile, nRow, nLine); nLine++;
					ToData(m_LiftInfo[nLift].move.y, pFile, nRow, nLine); nLine++;
					m_LiftInfo[nLift].move.z = 0.0f;
				}

				if (cstr != NULL) {
					delete[] cstr;
					cstr = NULL;
				}
			}
		}

		if (cstr != NULL){
			delete[] cstr;
			cstr = NULL;
		}
	}
}

//========================================
// 覐΂̏��ݒ�
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SetMeteorInfo(CSVFILE *pFile, int nRow, int nLine)
{
	int nMeteor = 0;

	while (1)
	{
		nLine = 0;	nRow++;
		char *aDataSearch = NULL;	// �f�[�^�����p
		string sData = pFile->GetData(nRow, nLine);
		char* cstr = new char[sData.size() + 1]; // �������m��
		std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
		aDataSearch = cstr;

		if (!strcmp(aDataSearch, "EndMeteorInfo")) {
			if (cstr != NULL) {
				delete[] cstr;
				cstr = NULL;
			}
			break;
		}
		else if (!strcmp(aDataSearch, "MeteorMax")) {
			int Max = 0;
			nLine += 4;
			ToData(Max, pFile, nRow, nLine);

			m_Info.nMateorMax = Max;
			m_MeteorInfo = new MeteorInfo[Max];
			assert(m_MeteorInfo != NULL);
		}
		else if (!strcmp(aDataSearch, "SetMeteor")) {
			while (1)
			{
				nLine = 0;	nRow++;				char *aDataSearch = NULL;	// �f�[�^�����p
				string sData = pFile->GetData(nRow, nLine);
				char* cstr = new char[sData.size() + 1]; // �������m��
				std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
				aDataSearch = cstr;

				if (!strcmp(aDataSearch, "EndMeteor")) {
					if (cstr != NULL) {
						delete[] cstr;
						cstr = NULL;
					}

					if (nMeteor < m_Info.nMateorMax) {
						Manager::BlockMgr()->MeteorGeneratorCreate(m_MeteorInfo[nMeteor].pos, m_MeteorInfo[nMeteor].move, m_MeteorInfo[nMeteor].inteval);
					}

					nMeteor++;
					break;
				}
				else if (!strcmp(aDataSearch, "Pos")) {
					int Row = 0; int Line = 0; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_MeteorInfo[nMeteor].pos = GetPos(Row, Line);
					m_MeteorInfo[nMeteor].pos.x += CStageObject::SIZE_OF_1_SQUARE;
					m_MeteorInfo[nMeteor].pos.y -= CStageObject::SIZE_OF_1_SQUARE;
				}
				else if (!strcmp(aDataSearch, "Move")) {
					nLine += 4;
					ToData(m_MeteorInfo[nMeteor].move.x, pFile, nRow, nLine); nLine++;
					ToData(m_MeteorInfo[nMeteor].move.y, pFile, nRow, nLine); nLine++;
					m_MeteorInfo[nMeteor].move.z = 0.0f;
				}
				else if (!strcmp(aDataSearch, "Inteval")) {
					int inteval = 0; nLine += 4;
					ToData(inteval, pFile, nRow, nLine);

					m_MeteorInfo[nMeteor].inteval = inteval;
				}

				if (cstr != NULL) {
					delete[] cstr;
					cstr = NULL;
				}
			}
		}

		if (cstr != NULL) {
			delete[] cstr;
			cstr = NULL;
		}
	}
}

//========================================
// ���[�U�[�̏��ݒ�
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SetLaserInfo(CSVFILE *pFile, int nRow, int nLine)
{
	int nLaser = 0;

	while (1)
	{
		nLine = 0;	nRow++;
		char *aDataSearch = NULL;	// �f�[�^�����p
		string sData = pFile->GetData(nRow, nLine);
		char* cstr = new char[sData.size() + 1]; // �������m��
		std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
		aDataSearch = cstr;

		if (!strcmp(aDataSearch, "EndLaserInfo")) {
			if (cstr != NULL) {
				delete[] cstr;
				cstr = NULL;
			}
			break;
		}
		else if (!strcmp(aDataSearch, "LaserMax")) {
			int Max = 0;
			nLine += 4;
			ToData(Max, pFile, nRow, nLine);

			m_Info.nLaserMax = Max;
			m_LaserInfo = new LaserInfo[Max];
			assert(m_LaserInfo != NULL);
		}
		else if (!strcmp(aDataSearch, "SetLaser")) {
			while (1)
			{
				nLine = 0;	nRow++;				char *aDataSearch = NULL;	// �f�[�^�����p
				string sData = pFile->GetData(nRow, nLine);
				char* cstr = new char[sData.size() + 1]; // �������m��
				std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
				aDataSearch = cstr;

				if (!strcmp(aDataSearch, "EndLaser")) {
					if (cstr != NULL) {
						delete[] cstr;
						cstr = NULL;
					}

					if (nLaser < m_Info.nLaserMax) {
						Manager::BlockMgr()->RoadTripLaserCreate(m_LaserInfo[nLaser].posV, m_LaserInfo[nLaser].posR, m_LaserInfo[nLaser].move);
					}

					nLaser++;
					break;
				}
				else if (!strcmp(aDataSearch, "PosV")) {
					int Row = 0; int Line = 0; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_LaserInfo[nLaser].posV = GetPos(Row, Line);
				}
				else if (!strcmp(aDataSearch, "PosR")) {
					int Row = 0; int Line = 0; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_LaserInfo[nLaser].posR = GetPos(Row, Line);
				}
				else if (!strcmp(aDataSearch, "Move")) {
					nLine += 4;
					ToData(m_LaserInfo[nLaser].move.x, pFile, nRow, nLine); nLine++;
					ToData(m_LaserInfo[nLaser].move.y, pFile, nRow, nLine); nLine++;
					m_LaserInfo[nLaser].move.z = 0.0f;
				}

				if (cstr != NULL) {
					delete[] cstr;
					cstr = NULL;
				}
			}
		}

		if (cstr != NULL) {
			delete[] cstr;
			cstr = NULL;
		}
	}
}

//========================================
// ���̏��ݒ�
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SetDogInfo(CSVFILE *pFile, int nRow, int nLine)
{
	int nDog = 0;

	while (1)
	{
		nLine = 0;	nRow++;
		char *aDataSearch = NULL;	// �f�[�^�����p
		string sData = pFile->GetData(nRow, nLine);
		char* cstr = new char[sData.size() + 1]; // �������m��
		std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
		aDataSearch = cstr;

		if (!strcmp(aDataSearch, "EndDogInfo")) {
			if (cstr != NULL) {
				delete[] cstr;
				cstr = NULL;
			}
			break;
		}
		else if (!strcmp(aDataSearch, "DogMax")) {
			int Max = 0;
			nLine += 4;
			ToData(Max, pFile, nRow, nLine);

			m_Info.nDogMax = Max;
			m_DogInfo = new DogInfo[Max];
			assert(m_DogInfo != NULL);
		}
		else if (!strcmp(aDataSearch, "SetDog")) {
			while (1)
			{
				nLine = 0;	nRow++;				char *aDataSearch = NULL;	// �f�[�^�����p
				string sData = pFile->GetData(nRow, nLine);
				char* cstr = new char[sData.size() + 1]; // �������m��
				std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
				aDataSearch = cstr;

				if (!strcmp(aDataSearch, "EndDog")) {
					if (cstr != NULL) {
						delete[] cstr;
						cstr = NULL;
					}

					if (nDog < m_Info.nDogMax) {
						bool bReturn = false;

						if (m_DogInfo[nDog].HeadPos.y >= 0) {
							m_DogInfo[nDog].HipPos.y == m_DogInfo[nDog].HipPos.y + (CStageObject::SIZE_OF_1_SQUARE * 0.5f);
						}
						else if (m_DogInfo[nDog].HeadPos.y <= 0) {
							m_DogInfo[nDog].HipPos.y == m_DogInfo[nDog].HipPos.y - (CStageObject::SIZE_OF_1_SQUARE * 0.5f);
							bReturn = true;
						}

						Manager::BlockMgr()->ExtenddogCreate(m_DogInfo[nDog].HeadPos, m_DogInfo[nDog].HipPos, m_DogInfo[nDog].Height, true, bReturn);
					}

					nDog++;
					break;
				}
				else if (!strcmp(aDataSearch, "HeadPos")) {
					int Row = 0; int Line = 0; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_DogInfo[nDog].HeadPos = GetPos(Row, Line);
				}
				else if (!strcmp(aDataSearch, "HipPos")) {
					int Row = 0; int Line = 0; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_DogInfo[nDog].HipPos = GetPos(Row, Line);
				}
				else if (!strcmp(aDataSearch, "Height")) {
					nLine += 4;
					ToData(m_DogInfo[nDog].Height, pFile, nRow, nLine); nLine++;
				}
				else if (!strcmp(aDataSearch, "Shrink")) {
					nLine += 4; int nShrink = 0;
					ToData(nShrink, pFile, nRow, nLine); nLine++;

					if (nShrink <= 0)
						m_DogInfo[nDog].bShrink = false;
					else
						m_DogInfo[nDog].bShrink = true;
				}

				if (cstr != NULL) {
					delete[] cstr;
					cstr = NULL;
				}
			}
		}

		if (cstr != NULL) {
			delete[] cstr;
			cstr = NULL;
		}
	}
}

//========================================
// �Y�̏��ݒ�
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SetPileInfo(CSVFILE *pFile, int nRow, int nLine)
{
	int nPile = 0;

	while (1)
	{
		nLine = 0;	nRow++;
		char *aDataSearch = NULL;	// �f�[�^�����p
		string sData = pFile->GetData(nRow, nLine);
		char* cstr = new char[sData.size() + 1]; // �������m��
		std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
		aDataSearch = cstr;

		if (!strcmp(aDataSearch, "EndPileInfo")) {
			if (cstr != NULL) {
				delete[] cstr;
				cstr = NULL;
			}
			break;
		}
		else if (!strcmp(aDataSearch, "PileMax")) {
			int Max = 0;
			nLine += 4;
			ToData(Max, pFile, nRow, nLine);

			m_Info.nPileMax = Max;
			m_PileInfo = new PileInfo[Max];
			assert(m_PileInfo != NULL);
		}
		else if (!strcmp(aDataSearch, "SetPile")) {
			while (1)
			{
				nLine = 0;	nRow++;				char *aDataSearch = NULL;	// �f�[�^�����p
				string sData = pFile->GetData(nRow, nLine);
				char* cstr = new char[sData.size() + 1]; // �������m��
				std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
				aDataSearch = cstr;

				if (!strcmp(aDataSearch, "EndPile")) {
					if (cstr != NULL) {
						delete[] cstr;
						cstr = NULL;
					}

					if (nPile < m_Info.nPileMax) {
						bool bReturn = false;

						Manager::BlockMgr()->PileCreate(m_PileInfo[nPile].pos, m_PileInfo[nPile].nNumPile, m_PileInfo[nPile].fCaveIn);
					}

					nPile++;
					break;
				}
				else if (!strcmp(aDataSearch, "Pos")) {
					int Row = 0; int Line = 0; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_PileInfo[nPile].pos = GetPos(Row, Line);
				}
				else if (!strcmp(aDataSearch, "NumPile")) {
					nLine += 4;
					int nNumPile = 0;
					ToData(nNumPile, pFile, nRow, nLine); nLine++;

					if (nNumPile <= 3){
						nNumPile = 3;
					}

					m_PileInfo[nPile].nNumPile = nNumPile;
				}
				else if (!strcmp(aDataSearch, "CaveIn")) {
					nLine += 4;
					ToData(m_PileInfo[nPile].fCaveIn, pFile, nRow, nLine); nLine++;
				}

				if (cstr != NULL) {
					delete[] cstr;
					cstr = NULL;
				}
			}
		}

		if (cstr != NULL) {
			delete[] cstr;
			cstr = NULL;
		}
	}
}

//========================================
// ���W�̎擾
// Author:KEISUKE OTONO
//========================================
D3DXVECTOR3 CStageEditor::GetPos(int nRow, int nLine)
{
	float fSizeX = CStageObject::SIZE_OF_1_SQUARE;
	float fSizeY = CStageObject::SIZE_OF_1_SQUARE;
	D3DXVECTOR3 pos = Manager::GetMainCamera()->GetPosR();

	pos.x += ((m_Info.nLineMax * -0.5f) + nLine + 0.5f) * fSizeX;
	pos.y -= ((m_Info.nRowMax * -0.5f) + nRow + 0.5f) * fSizeY;
	pos.z = 0.0f;

	return pos;
}

//========================================
// �ϊ�
// Author:KEISUKE OTONO
//========================================

// int
bool CStageEditor::ToData(int &val, CSVFILE *pFile, int nRow, int nLine)
{
	try
	{
		string sData = pFile->GetData(nRow, nLine);
		val = stoi(sData);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

// float
bool CStageEditor::ToData(float &val, CSVFILE *pFile, int nRow, int nLine)
{
	try
	{
		string sData = pFile->GetData(nRow, nLine);
		val = stof(sData);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

// double
bool CStageEditor::ToData(double &val, CSVFILE *pFile, int nRow, int nLine)
{
	try
	{
		string sData = pFile->GetData(nRow, nLine);
		val = stod(sData);
		return true;
	}
	catch (...)
	{
		return false;
	}
}
