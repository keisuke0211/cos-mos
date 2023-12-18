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
//========================================
// モーション情報読み込み処理
// Arrange:RIKU NISHIMURA
//========================================
void CBGEditor::Load(const char* loadPath) {

	struct LocalFunc {
		static CBGEditor::INFO LoadBGInfo(void) {
			CBGEditor::INFO info = {};
			while (RNLib::File().SearchLoop("}")) {
				RNLib::File().Scan(_RNC_File::SCAN::POS3D, &info.pos, "POS");
				RNLib::File().Scan(_RNC_File::SCAN::POS3D, &info.posAdd, "POSADD");
				RNLib::File().Scan(_RNC_File::SCAN::POS3D, &info.move, "MOVE");
				RNLib::File().Scan(_RNC_File::SCAN::ROT3D, &info.rot, "ROT");
				RNLib::File().Scan(_RNC_File::SCAN::ROT3D, &info.spin, "SPIN");
				RNLib::File().Scan(_RNC_File::SCAN::ROT3D, &info.spinAdd, "SPINADD");
				RNLib::File().Scan(_RNC_File::SCAN::SHORT, &info.life, "LIFE");
				RNLib::File().Scan(_RNC_File::SCAN::COLOR, &info.col, "COLOR");
				RNLib::File().Scan(_RNC_File::SCAN::MODELIDX, &info.modelIdx, "PATH");
				if (RNLib::File().CheckIdentifier("MOVE_AB{")) {
					RNLib::Memory().Alloc((MOVE_AB**)&info.moveInfo);
					info.moveType = MOVE_TYPE::AB;
					MOVE_AB* moveInfo = (MOVE_AB*)info.moveInfo;
					moveInfo->counter = 0;
					moveInfo->isReturn = false;

					while (RNLib::File().SearchLoop("}")) {
						RNLib::File().Scan(_RNC_File::SCAN::POS3D, &moveInfo->posB, "POSB");
						RNLib::File().Scan(_RNC_File::SCAN::INT, &moveInfo->time, "TIME");
					}
				}
			}
			return info;
		}
	};

	if (RNLib::File().OpenLoadFile(loadPath)) {
		char headString[TXT_MAX];
		RNLib::File().Scan(_RNC_File::SCAN::STRING, headString);
		if (strcmp(headString, "BG_FILE"))
			return;

		while (RNLib::File().SearchLoop("END")) {
			if (RNLib::File().CheckIdentifier("UP")) {
				while (RNLib::File().SearchLoop("END")) {
					if (RNLib::File().CheckIdentifier("OBJECT{")) {
						CBGEditor::INFO info = LocalFunc::LoadBGInfo();
						info.side = CBGEditor::WORLD_SIDE::UP;

						CBGObject* pBGObj = new CBGObject;
						pBGObj->SetInfo(info);
					}
					else if (RNLib::File().CheckIdentifier("GENERATOR{")) {
						int time = 0;
						CBGEditor::INFO info = {};

						while (RNLib::File().SearchLoop("}")) {
							if (RNLib::File().CheckIdentifier("OBJECT{")) {
								info = LocalFunc::LoadBGInfo();
								info.side = CBGEditor::WORLD_SIDE::UP;
							}
							RNLib::File().Scan(_RNC_File::SCAN::INT, &time, "CREATE_TIME");
						}

						info.createTime = time;
						CBGObject* pBGObj = new CBGObject;
						pBGObj->SetInfo(info);
					}
				}
			}
			else if (RNLib::File().CheckIdentifier("DOWN")) {
				while (RNLib::File().SearchLoop("END")) {
					if (RNLib::File().CheckIdentifier("OBJECT{")) {
						CBGEditor::INFO info = LocalFunc::LoadBGInfo();
						info.side = CBGEditor::WORLD_SIDE::DOWN;

						CBGObject* pBGObj = new CBGObject;
						pBGObj->SetInfo(info);
					}
					else if (RNLib::File().CheckIdentifier("GENERATOR{")) {
						int time = 0;
						CBGEditor::INFO info = {};

						while (RNLib::File().SearchLoop("}")) {
							if (RNLib::File().CheckIdentifier("OBJECT{")) {
								info = LocalFunc::LoadBGInfo();
								info.side = CBGEditor::WORLD_SIDE::DOWN;
							}
							RNLib::File().Scan(_RNC_File::SCAN::INT, &time, "CREATE_TIME");
						}

						info.createTime = time;
						CBGObject* pBGObj = new CBGObject;
						pBGObj->SetInfo(info);
					}
				}
			}
		}
		RNLib::File().CloseFile();
	}
}