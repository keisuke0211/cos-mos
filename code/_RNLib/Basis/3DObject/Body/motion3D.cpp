//========================================
// 
// モーション3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]モーション3Dクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 定数定義
//****************************************
const UShort CMotion3D::COMMAND_DATA_NUM[] = {
	5,	// 移動 ( 目標位置X,Y,Z,移動時間,補間番号 )
	5,	// 回転 ( 目標向きX,Y,Z,回転時間,補間番号 )
	5,	// 拡縮 ( 拡大倍率X,Y,Z.拡縮時間,補間番号 )
	0,	// 足踏
};

//========================================
// コンストラクタ
//========================================
CMotion3D::CMotion3D() {

	m_datas = NULL;
}

//========================================
// デストラクタ
//========================================
CMotion3D::~CMotion3D() {

}

//========================================
// 初期化処理
//========================================
void CMotion3D::Init(void) {

}

//========================================
// 終了処理
//========================================
void CMotion3D::Uninit(void) {

	// データを解放
	CMemory::Release(&m_datas);
}

//========================================
// 更新処理
//========================================
void CMotion3D::Update(void) {

}

//========================================
// 読み込み処理
//========================================
short CMotion3D::Load(const char* loadPath, short idx) {

	const UShort numOld = m_num;
	const short  idxOld = idx;

	if (CRegist::Load(loadPath, idx))
	{// 読み込み成功
		// データのメモリ再確保
		CMemory::ReAlloc(&m_datas, numOld, m_num);

		// データの破棄(番号指定の場合)
		if (idxOld != NONEDATA)
			m_datas[idx].Release();

		// ファイルを開く
		if (!RNLib::File().OpenLoadFile(loadPath)) {
			while (RNLib::File().SearchLoop("END")) {
				RNLib::File().Scan(CFile::SCAN::INT, &m_datas[idx].loopTime, "loopTime");
				RNLib::File().Scan(CFile::SCAN::BOOL, &m_datas[idx].isLoop, "isLoop");

				// ボーンモーションデータを読み込み
				if (RNLib::File().CheckIdentifier("boneMotionDatas{")) {

					// ボーンの数を読み込み、
					// ボーンモーションデータのメモリ確保
					RNLib::File().Scan(CFile::SCAN::INT, &m_datas[idx].boneNum);
					CMemory::Alloc<BoneMotionData>(&m_datas[idx].boneMotionDatas, m_datas[idx].boneNum);

					int cntBone = 0;
					while (RNLib::File().SearchLoop("}")) {
						if (RNLib::File().CheckIdentifier("boneMotionData{")) {

							// エラーメッセージ
							if (cntBone >= m_datas[idx].boneNum) {
								RNLib::Window().Message_ERROR(CreateText("ボーンモーションデータの数がボーン数をオーバーしています。\n%s", loadPath));
								break;
							}

							// ボーンモーションデータ初期化
							BoneMotionData& boneMotionData = m_datas[idx].boneMotionDatas[cntBone];
							boneMotionData = {};

							// コマンド数読み込み
							RNLib::File().Scan(CFile::SCAN::INT, &boneMotionData.commandDataNum);

							// コマンド数が0を越えている時、
							if (boneMotionData.commandDataNum > 0) {
								boneMotionData.commandDatas = new CommandData[boneMotionData.commandDataNum];

								// コマンドのカウント
								int cntCommand = 0;

								// コマンドの読み込みを開始
								while (RNLib::File().SearchLoop("}")) {
									if (RNLib::File().CheckIdentifier("time")) {

										// エラーメッセージ
										if (cntCommand >= boneMotionData.commandDataNum) {
											RNLib::Window().Message_ERROR(CreateText("コマンドデータの数が指定数をオーバーしています。\n%s", loadPath));
											break;
										}

										// コマンド開始時間読み込み
										RNLib::File().Scan(CFile::SCAN::INT, &boneMotionData.commandDatas[cntCommand].time);

										// コマンドを読み取る
										COMMAND command = COMMAND::NONE;
										RNLib::File().Scan(CFile::SCAN::INT, &command);
										
										// 移動/回転/拡縮フラグを立てる
										if (boneMotionData.commandDatas[cntCommand].command == COMMAND::MOVE)
											boneMotionData.isMove = true;
										else if (boneMotionData.commandDatas[cntCommand].command == COMMAND::SPIN)
											boneMotionData.isSpin = true;
										else if (boneMotionData.commandDatas[cntCommand].command == COMMAND::SCALING)
											boneMotionData.isScale = true;

										// 引数のメモリを確保
										boneMotionData.commandDatas[cntCommand].datas = NULL;
										boneMotionData.commandDatas[cntCommand].datas = new float[COMMAND_DATA_NUM[(int)command]];

										// 引数の読み込み
										for (int cntData = 0; cntData < COMMAND_DATA_NUM[(int)command]; cntData++)
											RNLib::File().Scan(CFile::SCAN::FLOAT, &boneMotionData.commandDatas[cntCommand].datas[cntData]);

										// コマンドのカウント
										cntCommand++;	
									}
								}

								// エラーメッセージ
								if (cntCommand < boneMotionData.commandDataNum) {
									RNLib::Window().Message_ERROR(CreateText("コマンドデータの数が指定数に対して不足しています。\n%s", loadPath));
								}
							}
							else
							{// コマンド数が0以下だった時、
								boneMotionData.commandDatas = NULL;
								RNLib::File().NewLine();	// }
							}

							// ボーンのカウント
							cntBone++;
						}
					}
				}
			}

			// ファイルを閉じる
			RNLib::File().CloseFile();
		}
		else
		{// 読み込み失敗
			// エラーメッセージ
			RNLib::Window().Message_ERROR(CreateText("モーションデータファイルが存在しません。\n%s", loadPath));

			// 読み込み済パスのメモリリセット
			ReAllocLoadPath(numOld);

			return NONEDATA;
		}
	}

	return idx;
}

//========================================
// 書き出し処理
//========================================
void CMotion3D::Save(const char* savePath, const short& idx) {

	// ファイルを開くのに失敗した時、処理を終了
	if (!RNLib::File().OpenSaveFile(savePath))
		return;

	// ファイルポインタを取得
	FILE* file = RNLib::File().GetFile();

	fprintf(file, "loopTime: %d\n", m_datas[idx].loopTime);
	fprintf(file, "loop: %d\n", (int)m_datas[idx].isLoop);

	// ボーンモーションデータの書き出し
	for (int cntBone = 0; cntBone < m_datas[idx].boneNum; cntBone++) {
		BoneMotionData& boneMotionData = m_datas[idx].boneMotionDatas[cntBone];

		fprintf(file, "boneMotionDatas{ %d\n", boneMotionData.commandDataNum);

		// コマンドデータ書き出し
		for (int cntCmd = 0; cntCmd < boneMotionData.commandDataNum; cntCmd++) {
			CommandData& cmd = boneMotionData.commandDatas[cntCmd];

			fprintf(file, "		time %d ", cmd.time);
			switch (cmd.command) {
			case COMMAND::MOVE: {
				fprintf(file, "%d %f %f %f %d %d\n",
					(int)COMMAND::MOVE, cmd.datas[0], cmd.datas[1], cmd.datas[2], (int)cmd.datas[3], (int)cmd.datas[4]);
			}break;
			case COMMAND::SPIN: {
				fprintf(file, "%d %f %f %f %d %d\n",
					(int)COMMAND::SPIN, cmd.datas[0], cmd.datas[1], cmd.datas[2], (int)cmd.datas[3], (int)cmd.datas[4]);
			}break;
			case COMMAND::SCALING: {
				fprintf(file, "%d %f %f %f %d %d\n",
					(int)COMMAND::SCALING, cmd.datas[0], cmd.datas[1], cmd.datas[2], (int)cmd.datas[3], (int)cmd.datas[4]);
			}break;
			case COMMAND::STEP: {
				fprintf(file, "%d %d\n",
					(int)COMMAND::STEP, (int)cmd.datas[0]);
			}break;
			default: {
				// エラーメッセージ
				RNLib::Window().Message_ERROR(CreateText("コマンドが指定されていません。\n%s", savePath));
			}break;
			}
		}
		fprintf(file, "}\n");
	}

	fprintf(file, "END\n");

	// ファイルを閉じる
	RNLib::File().CloseFile();
}

//========================================
// メモリを指定数に初期化
//========================================
void CMotion3D::InitMemory(const UShort& num) {
	CRegist::InitMemory(num);

	// データのメモリ確保
	CMemory::Alloc(&m_datas, num);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]データクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CMotion3D::CData::CData() {

	loopTime        = 0;
	isLoop          = false;
	boneNum         = 0;
	boneMotionDatas = NULL;
}

//========================================
// デストラクタ
//========================================
CMotion3D::CData::~CData() {

	// 解放処理
	Release();
}

//========================================
// 解放処理
//========================================
void CMotion3D::CData::Release(void) {

	// ボーンコマンドデータを解放
	for (int cntParts = 0; cntParts < boneNum; cntParts++) {
		for (int cntCmd = 0; cntCmd < boneMotionDatas[cntParts].commandDataNum; cntCmd++) {
			CMemory::Release(&boneMotionDatas[cntParts].commandDatas[cntCmd].datas);
		}
		CMemory::Release(&boneMotionDatas[cntParts].commandDatas);
	}
	CMemory::Release(&boneMotionDatas);
}
