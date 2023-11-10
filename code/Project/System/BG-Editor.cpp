//========================================
// 
// 背景エディターの処理
// Author:RYUKI FUJIWARA
// 
//========================================
#include "BG-Editor.h"
#include "../Object/BackGround/BG-object.h"

CBGEditor::INFO CBGEditor::m_Info;
//==============================================
//コンストラクタ
//==============================================
CBGEditor::CBGEditor()
{
	m_Info.side = WORLD_SIDE::NONE;
}
//==============================================
//デスストラクタ
//==============================================
CBGEditor::~CBGEditor()
{

}
//==============================================
//TXTのモーション読込処理
//==============================================
void CBGEditor::Load(const char* filepath)
{
	FILE *pFile;		//ファイルポインタを宣言
	char cData[1000];	//一文字ずつ確認する変数
	bool bLoad = false;

	// ファイルを開く
	pFile = fopen(filepath, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		while (1)
		{
			//ファイルから文字読み込む
			fgets(&cData[0], 1000, pFile);

			//コメントアウトチェック
			char *pCharPos = strchr(&cData[0], '#');
			if (pCharPos != nullptr)
			{//strchrの返り値がNULLではない
				*pCharPos = '\0';
			}

			//タブ消去
			while (cData[0] == '\t')
			{
				char cDataBackup[1000];
				strcpy(&cDataBackup[0], &cData[0]);
				strcpy(&cData[0], &cDataBackup[1]);
			}

			//文字列チェック
			if (strncmp(&cData[0], "SCRIPT", sizeof "SCRIPT" / sizeof(char) - 1) == 0)
			{//読み取り開始
				bLoad = true;
			}
			else if (strncmp(&cData[0], "END_SCRIPT", sizeof "END_SCRIPT" / sizeof(char) - 1) == 0)
			{//読み取り終了
				bLoad = false;
				break;
			}
			else if (cData[0] == EOF)
			{//EOFの微レ存

				if (feof(pFile))
				{//EOFと仮定

					bLoad = false;
					break;
				}
			}
			else if (bLoad == true)
			{//読み取り

				if (m_Info.side == WORLD_SIDE::NONE)
				{
					if (strncmp(&cData[0], "UP", sizeof "UP" / sizeof(char) - 1) == 0)
					{
						m_Info.side = WORLD_SIDE::UP;

						while (true) {

							fscanf(pFile, "%s", &cData[0]);

							if (strncmp(&cData[0], "END", sizeof "END" / sizeof(char) - 1) == 0)
							{//読み取り終了

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

								//生成してキャスト
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
							{//読み取り終了

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

								//生成してキャスト
								CObject *pObj = new CBGObject;
								CBGObject *pBGObj = dynamic_cast<CBGObject*>(pObj);
								pBGObj->SetInfo(m_Info);
							}
						}
					}
				}
			}
		}
		//ファイルを閉じる
		fclose(pFile);
	}
}