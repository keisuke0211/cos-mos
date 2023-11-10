//========================================
// 
// �w�i�G�f�B�^�[�̏���
// Author:RYUKI FUJIWARA
// 
//========================================
#include "BG-Editor.h"
#include "../Object/BackGround/BG-object.h"

CBGEditor::INFO CBGEditor::m_Info;
//==============================================
//�R���X�g���N�^
//==============================================
CBGEditor::CBGEditor()
{
	m_Info.side = WORLD_SIDE::NONE;
}
//==============================================
//�f�X�X�g���N�^
//==============================================
CBGEditor::~CBGEditor()
{

}
//==============================================
//TXT�̃��[�V�����Ǎ�����
//==============================================
void CBGEditor::Load(const char* filepath)
{
	FILE *pFile;		//�t�@�C���|�C���^��錾
	char cData[1000];	//�ꕶ�����m�F����ϐ�
	bool bLoad = false;

	// �t�@�C�����J��
	pFile = fopen(filepath, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		while (1)
		{
			//�t�@�C�����當���ǂݍ���
			fgets(&cData[0], 1000, pFile);

			//�R�����g�A�E�g�`�F�b�N
			char *pCharPos = strchr(&cData[0], '#');
			if (pCharPos != nullptr)
			{//strchr�̕Ԃ�l��NULL�ł͂Ȃ�
				*pCharPos = '\0';
			}

			//�^�u����
			while (cData[0] == '\t')
			{
				char cDataBackup[1000];
				strcpy(&cDataBackup[0], &cData[0]);
				strcpy(&cData[0], &cDataBackup[1]);
			}

			//������`�F�b�N
			if (strncmp(&cData[0], "SCRIPT", sizeof "SCRIPT" / sizeof(char) - 1) == 0)
			{//�ǂݎ��J�n
				bLoad = true;
			}
			else if (strncmp(&cData[0], "END_SCRIPT", sizeof "END_SCRIPT" / sizeof(char) - 1) == 0)
			{//�ǂݎ��I��
				bLoad = false;
				break;
			}
			else if (cData[0] == EOF)
			{//EOF�̔�����

				if (feof(pFile))
				{//EOF�Ɖ���

					bLoad = false;
					break;
				}
			}
			else if (bLoad == true)
			{//�ǂݎ��

				if (m_Info.side == WORLD_SIDE::NONE)
				{
					if (strncmp(&cData[0], "UP", sizeof "UP" / sizeof(char) - 1) == 0)
					{
						m_Info.side = WORLD_SIDE::UP;

						while (true) {

							fscanf(pFile, "%s", &cData[0]);

							if (strncmp(&cData[0], "END", sizeof "END" / sizeof(char) - 1) == 0)
							{//�ǂݎ��I��

								m_Info.side = WORLD_SIDE::NONE;
								break;
							}
							else if (strncmp(&cData[0], "POS", sizeof "POS" / sizeof(char) - 1) == 0) {
								fscanf(pFile, "%f,%f,%f ROT %f,%f,%f SPIN %f,%f,%f COLOR %d,%d,%d,%d PATH %s",
									&m_Info.pos.x, &m_Info.pos.y, &m_Info.pos.z,
									&m_Info.rot.x, &m_Info.rot.y, &m_Info.rot.z,
									&m_Info.spin.x, &m_Info.spin.y, &m_Info.spin.z,
									&m_Info.col.r, &m_Info.col.g, &m_Info.col.b, &m_Info.col.a,
									m_Info.ModelPath);

								m_Info.side = WORLD_SIDE::UP;

								//�������ăL���X�g
								CObject *pObj = new CBGObject;
								CBGObject *pBGObj = dynamic_cast<CBGObject*>(pObj);
								pBGObj->SetInfo(m_Info);
							}
						}
					}
					else if (strncmp(&cData[0], "DOWN", sizeof "DOWN" / sizeof(char) - 1) == 0)
					{
						m_Info.side = WORLD_SIDE::DOWN;

						while (true) {

							fscanf(pFile, "%s", &cData[0]);

							if (strncmp(&cData[0], "END", sizeof "END" / sizeof(char) - 1) == 0)
							{//�ǂݎ��I��

								m_Info.side = WORLD_SIDE::NONE;
								break;
							}
							else if (strncmp(&cData[0], "POS", sizeof "POS" / sizeof(char) - 1) == 0) {
								fscanf(pFile, "%f,%f,%f ROT %f,%f,%f SPIN %f,%f,%f COLOR %d,%d,%d,%d PATH %s",
									&m_Info.pos.x, &m_Info.pos.y, &m_Info.pos.z,
									&m_Info.rot.x, &m_Info.rot.y, &m_Info.rot.z,
									&m_Info.spin.x, &m_Info.spin.y, &m_Info.spin.z,
									&m_Info.col.r, &m_Info.col.g, &m_Info.col.b, &m_Info.col.a,
									m_Info.ModelPath);

								//�������ăL���X�g
								CObject *pObj = new CBGObject;
								CBGObject *pBGObj = dynamic_cast<CBGObject*>(pObj);
								pBGObj->SetInfo(m_Info);
							}
						}
					}
				}
			}
		}
		//�t�@�C�������
		fclose(pFile);
	}
}