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

//========================================
// コンストラクタ
//========================================
CMotion3D::CMotion3D() {

	m_datas    = NULL;
	m_editData = NULL;
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

	// データの解放
	CMemory::ReleaseDouble(&m_datas, m_num);

	// エディットデータの解放
	CMemory::Release(&m_editData);
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

	if (CRegist::Load(loadPath, idx))
	{// 読み込み成功
		// データのメモリ再確保
		CMemory::ReAllocDouble(&m_datas, numOld, m_num);

		// データの破棄(番号指定の場合)
		if (idx != NONEDATA)
			m_datas[idx]->Release();

		// 読み込み実行
		if (!ExecutionLoad(loadPath, *m_datas[idx]))
		{// 失敗した時、
			// 読み込み済パスのメモリリセット
			ReAllocLoadPath(numOld);

			return NONEDATA;
		}
	}

	return idx;
}

//========================================
// 読み込み処理(エディットデータ)
//========================================
bool CMotion3D::LoadEditData(const char* loadPath) {

	CData* loadData = NULL;
	CMemory::Alloc(&loadData);

	// 読み込み実行
	if (!ExecutionLoad(loadPath, *loadData)) {
		CMemory::Release(&loadData);
		return false;
	}

	// エディットデータを解放
	CMemory::Release(&m_editData);

	// エディットデータを上書き
	m_editData = loadData;

	// 再セットアップ
	CDoll3D* doll = RNLib::Doll3DMgr().GetEditDoll();
	if (doll != NULL)
		doll->SetMotion(EDITDATA);

	return true;
}

//========================================
// 書き出し処理
//========================================
void CMotion3D::SaveEditData(const char* savePath) {

	if (m_editData == NULL) {
		RNLib::Window().Message_ERROR(CreateText("モーション3Dデータが存在しません。"));
		return;
	}

	// ファイルを開く
	if (RNLib::File().OpenSaveFile(savePath)) {
		fprintf(RNLib::File().GetFile(), "Motion3DFile\n");
		fprintf(RNLib::File().GetFile(), "loopTime %d\n", m_editData->loopTime);
		fprintf(RNLib::File().GetFile(), "isLoop %d\n", (int)m_editData->isLoop);
		fprintf(RNLib::File().GetFile(), "boneMotionDatas{ %d\n", m_editData->boneNum);

		// ボーンモーションデータの書き出し
		for (int cntBone = 0; cntBone < m_editData->boneNum; cntBone++) {
			BoneMotionData& boneMotionData = m_editData->boneMotionDatas[cntBone];

			fprintf(RNLib::File().GetFile(), "	boneMotionData{ %d\n", boneMotionData.commandDataNum);

			// コマンドデータ書き出し
			for (int cntCmd = 0; cntCmd < boneMotionData.commandDataNum; cntCmd++) {
				CommandData& cmd = boneMotionData.commandDatas[cntCmd];

				fprintf(RNLib::File().GetFile(), "		time %d ", cmd.time);
				switch (cmd.command) {
				case COMMAND::MOVE: {
					const CommandData_Move& data = *(CommandData_Move*)cmd.data;
					fprintf(RNLib::File().GetFile(), "%d %f %f %f %d %d\n",
						(int)COMMAND::MOVE, data.targetPos.x, data.targetPos.y, data.targetPos.z, data.time, (int)data.easeType);
				}break;
				case COMMAND::SPIN: {
					const CommandData_Spin& data = *(CommandData_Spin*)cmd.data;
					fprintf(RNLib::File().GetFile(), "%d %f %f %f %d %d\n",
						(int)COMMAND::SPIN, data.targetRot.x, data.targetRot.y, data.targetRot.z, data.time, (int)data.easeType);
				}break;
				case COMMAND::SCALING: {
					const CommandData_Scaling& data = *(CommandData_Scaling*)cmd.data;
					fprintf(RNLib::File().GetFile(), "%d %f %f %f %d %d\n",
						(int)COMMAND::SCALING, data.targetScale.x, data.targetScale.y, data.targetScale.z, data.time, (int)data.easeType);
				}break;
				default: {
					// エラーメッセージ
					RNLib::Window().Message_ERROR(CreateText("コマンドが指定されていません。\n%s", savePath));
				}break;
				}
			}
			fprintf(RNLib::File().GetFile(), "	}\n");
		}
		fprintf(RNLib::File().GetFile(), "}\n");
		fprintf(RNLib::File().GetFile(), "END\n");

		// ファイルを閉じる
		RNLib::File().CloseFile();
	}
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
//==========| [非公開]モーション3Dクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 読み込み実行処理
//========================================
bool CMotion3D::ExecutionLoad(const char* loadPath, CData& data) {

	// ファイルを開く
	if (RNLib::File().OpenLoadFile(loadPath, "Motion3DFile")) {
		while (RNLib::File().SearchLoop("END")) {
			RNLib::File().Scan(CFile::SCAN::SHORT, &data.loopTime, "loopTime");
			RNLib::File().Scan(CFile::SCAN::BOOL, &data.isLoop, "isLoop");

			// ボーンモーションデータを読み込み
			if (RNLib::File().CheckIdentifier("boneMotionDatas{")) {

				// ボーンの数を読み込み、
				// ボーンモーションデータのメモリ確保
				RNLib::File().Scan(CFile::SCAN::USHORT, &data.boneNum);
				CMemory::Alloc<BoneMotionData>(&data.boneMotionDatas, data.boneNum);

				UShort cntBone = 0;
				while (RNLib::File().SearchLoop("}")) {
					if (RNLib::File().CheckIdentifier("boneMotionData{")) {

						// エラーメッセージ
						if (cntBone >= data.boneNum) {
							RNLib::Window().Message_ERROR(CreateText("ボーンモーションデータの数がボーン数をオーバーしています。\n%s", loadPath));
							break;
						}

						// ボーンモーションデータ初期化
						BoneMotionData& boneMotionData = data.boneMotionDatas[cntBone];
						boneMotionData = {};

						// コマンド数読み込み
						RNLib::File().Scan(CFile::SCAN::USHORT, &boneMotionData.commandDataNum);

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
										
										// ファイルを閉じる
										RNLib::File().CloseFile();

										return false;
									}

									// コマンド開始時間読み込み
									RNLib::File().Scan(CFile::SCAN::SHORT, &boneMotionData.commandDatas[cntCommand].time);

									{// コマンドを読み取る
										int commandNum;
										RNLib::File().Scan(CFile::SCAN::INT, &commandNum);
										boneMotionData.commandDatas[cntCommand].command = (COMMAND)commandNum;
									}
									
									// コマンド毎の処理
									boneMotionData.commandDatas[cntCommand].data = NULL;
									switch (boneMotionData.commandDatas[cntCommand].command) {
									case COMMAND::MOVE: {
										boneMotionData.isMove = true;
										RNLib::Memory().Alloc((CommandData_Move**)&boneMotionData.commandDatas[cntCommand].data);

										CommandData_Move& data = *(CommandData_Move*)boneMotionData.commandDatas[cntCommand].data;
										RNLib::File().Scan(CFile::SCAN::POS3D, &data.targetPos);
										RNLib::File().Scan(CFile::SCAN::USHORT, &data.time);

										int easeTypeIdx;
										RNLib::File().Scan(CFile::SCAN::INT, &easeTypeIdx);
										data.easeType = (CEase::TYPE)easeTypeIdx;
									}break;
									case COMMAND::SPIN: {
										boneMotionData.isSpin = true;
										RNLib::Memory().Alloc((CommandData_Spin**)&boneMotionData.commandDatas[cntCommand].data);

										CommandData_Spin& data = *(CommandData_Spin*)boneMotionData.commandDatas[cntCommand].data;
										RNLib::File().Scan(CFile::SCAN::ROT3D, &data.targetRot);
										RNLib::File().Scan(CFile::SCAN::USHORT, &data.time);

										int easeTypeIdx;
										RNLib::File().Scan(CFile::SCAN::INT, &easeTypeIdx);
										data.easeType = (CEase::TYPE)easeTypeIdx;
									}break;
									case COMMAND::SCALING: {
										boneMotionData.isScale = true;
										RNLib::Memory().Alloc((CommandData_Scaling**)&boneMotionData.commandDatas[cntCommand].data);

										CommandData_Scaling& data = *(CommandData_Scaling*)boneMotionData.commandDatas[cntCommand].data;
										RNLib::File().Scan(CFile::SCAN::SCALE3D, &data.targetScale);
										RNLib::File().Scan(CFile::SCAN::USHORT, &data.time);

										int easeTypeIdx;
										RNLib::File().Scan(CFile::SCAN::INT, &easeTypeIdx);
										data.easeType = (CEase::TYPE)easeTypeIdx;

									}break;
									}

									// コマンドのカウント
									cntCommand++;	
								}
							}

							// エラーメッセージ
							if (cntCommand < boneMotionData.commandDataNum) {
								RNLib::Window().Message_ERROR(CreateText("コマンドデータの数が指定数に対して不足しています。\n%s", loadPath));

								// ファイルを閉じる
								RNLib::File().CloseFile();

								return false;
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

		return true;
	}
	else
	{// 読み込み失敗
		// エラーメッセージ
		RNLib::Window().Message_ERROR(CreateText("モーションデータファイルが存在しません。\n%s", loadPath));
	}

	return false;
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
			switch (boneMotionDatas[cntParts].commandDatas[cntCmd].command) {
			case COMMAND::MOVE   :CMemory::Release((CommandData_Move   **)&boneMotionDatas[cntParts].commandDatas[cntCmd].data); break;
			case COMMAND::SPIN   :CMemory::Release((CommandData_Spin   **)&boneMotionDatas[cntParts].commandDatas[cntCmd].data); break;
			case COMMAND::SCALING:CMemory::Release((CommandData_Scaling**)&boneMotionDatas[cntParts].commandDatas[cntCmd].data); break;
			}
		}
		CMemory::Release(&boneMotionDatas[cntParts].commandDatas);
	}
	CMemory::Release(&boneMotionDatas);
}
