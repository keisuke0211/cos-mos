//========================================
// 
// セットアップ3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]セットアップ3Dクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CSetUp3D::CSetUp3D() {

	m_datas    = NULL;
	m_editData = NULL;
}

//========================================
// デストラクタ
//========================================
CSetUp3D::~CSetUp3D() {

}

//========================================
// 初期化処理
//========================================
void CSetUp3D::Init(void) {

}

//========================================
// 終了処理
//========================================
void CSetUp3D::Uninit(void) {

	// データの解放
	CMemory::ReleaseDouble(&m_datas, m_num);

	// エディットデータの解放
	CMemory::Release(&m_editData);
}

//========================================
// 更新処理
//========================================
void CSetUp3D::Update(void) {

}

//========================================
// 読み込み処理
//========================================
short CSetUp3D::Load(const char* loadPath, short idx) {

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
bool CSetUp3D::LoadEditData(const char* loadPath) {

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
		doll->SetUp(EDITDATA);

	return true;
}

//========================================
// 書き出し処理(エディットデータ)
//========================================
void CSetUp3D::SaveEditData(const char* savePath) {

	if (m_editData == NULL) {
		RNLib::Window().Message_ERROR(CreateText("セットアップ3Dデータが存在しません。"));
		return;
	}

	// ファイルを開く
	if (RNLib::File().OpenSaveFile(savePath)) {

		fprintf(RNLib::File().GetFile(), "SetUp3DFile\n");
		fprintf(RNLib::File().GetFile(), "boneDataSum{ %d\n", m_editData->m_boneDataNum);
		for (int cntBoneData = 0; cntBoneData < m_editData->m_boneDataNum; cntBoneData++) {
			fprintf(RNLib::File().GetFile(), "	boneData{\n");
			fprintf(RNLib::File().GetFile(), "		idx %d\n", m_editData->m_boneDatas[cntBoneData].idx);
			fprintf(RNLib::File().GetFile(), "		modelIdx %s\n", RNLib::Model().GetLoadPath(m_editData->m_boneDatas[cntBoneData].modelIdx));
			fprintf(RNLib::File().GetFile(), "		parentIdx %d\n", m_editData->m_boneDatas[cntBoneData].parentIdx);
			fprintf(RNLib::File().GetFile(), "		relativePos %.2f %.2f %.2f\n", 
				m_editData->m_boneDatas[cntBoneData].relativePos.x,
				m_editData->m_boneDatas[cntBoneData].relativePos.y,
				m_editData->m_boneDatas[cntBoneData].relativePos.z);
			fprintf(RNLib::File().GetFile(), "		relativeRot %.2f %.2f %.2f\n", 
				m_editData->m_boneDatas[cntBoneData].relativeRot.x / D3DX_PI,
				m_editData->m_boneDatas[cntBoneData].relativeRot.y / D3DX_PI,
				m_editData->m_boneDatas[cntBoneData].relativeRot.z / D3DX_PI);
			if (m_editData->m_boneDatas[cntBoneData].swaying != NULL) {
				fprintf(RNLib::File().GetFile(), "		swaying{\n");
				fprintf(RNLib::File().GetFile(), "			timeMin %d\n", m_editData->m_boneDatas[cntBoneData].swaying->timeMin);
				fprintf(RNLib::File().GetFile(), "			timeAdd %d\n", m_editData->m_boneDatas[cntBoneData].swaying->timeAdd);
				fprintf(RNLib::File().GetFile(), "			distMin %f\n", m_editData->m_boneDatas[cntBoneData].swaying->distMin);
				fprintf(RNLib::File().GetFile(), "			distAdd %f\n", m_editData->m_boneDatas[cntBoneData].swaying->distAdd);
				fprintf(RNLib::File().GetFile(), "		}\n");
			}
			fprintf(RNLib::File().GetFile(), "	}\n");
		}
		fprintf(RNLib::File().GetFile(), "}\n");

		fprintf(RNLib::File().GetFile(), "faceDataSum{ %d\n", m_editData->m_faceDataNum);
		for (int cntFaceData = 0; cntFaceData < m_editData->m_faceDataNum; cntFaceData++) {
			fprintf(RNLib::File().GetFile(), "	faceData{\n");
			fprintf(RNLib::File().GetFile(), "		texIdx %s\n", RNLib::Texture().GetLoadPath(m_editData->m_faceDatas[cntFaceData].texIdx));
			fprintf(RNLib::File().GetFile(), "		col %d %d %d %d\n",
				m_editData->m_faceDatas[cntFaceData].col.r,
				m_editData->m_faceDatas[cntFaceData].col.g,
				m_editData->m_faceDatas[cntFaceData].col.b,
				m_editData->m_faceDatas[cntFaceData].col.a);
			for (int cntVtx = 0; cntVtx < 4; cntVtx++) {
				fprintf(RNLib::File().GetFile(), "		vtx%d{\n", cntVtx);
				fprintf(RNLib::File().GetFile(), "			boneIdx %d\n", m_editData->m_faceDatas[cntFaceData].vtxs[cntVtx].boneIdx);
				fprintf(RNLib::File().GetFile(), "			vtxIdx %d\n", m_editData->m_faceDatas[cntFaceData].vtxs[cntVtx].vtxIdx);
				fprintf(RNLib::File().GetFile(), "			texPos %.2f %.2f\n",
					m_editData->m_faceDatas[cntFaceData].vtxs[cntVtx].texPos.x,
					m_editData->m_faceDatas[cntFaceData].vtxs[cntVtx].texPos.y);
				fprintf(RNLib::File().GetFile(), "		}\n");
			}
			fprintf(RNLib::File().GetFile(), "	}\n");
		}
		fprintf(RNLib::File().GetFile(), "}\n");
		fprintf(RNLib::File().GetFile(), "END\n");

		// ファイルを閉じる
		RNLib::File().CloseFile();
	}
	else
	{// 書き出し失敗
		// エラーメッセージ
		RNLib::Window().Message_ERROR(CreateText("セットアップ3Dデータファイルが見つかりませんでした。\n%s", savePath));
	}
}

//========================================
// メモリを指定数に初期化
//========================================
void CSetUp3D::InitMemory(const UShort& num) {
	CRegist::InitMemory(num);

	// データのメモリ確保
	CMemory::Alloc(&m_datas, num);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]セットアップ3Dクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 読み込み実行処理
//========================================
bool CSetUp3D::ExecutionLoad(const char* loadPath, CData& data) {

	// ファイルを開く
	if (RNLib::File().OpenLoadFile(loadPath, "SetUp3DFile")) {
		while (RNLib::File().SearchLoop("END")) {
			if (RNLib::File().CheckIdentifier("boneDataSum{")) {
				BoneData*& boneDatas = data.m_boneDatas;
				
				// ボーンデータ群を解放
				CMemory::Release(&boneDatas);

				// ボーンデータ数読み込み
				RNLib::File().Scan(CFile::SCAN::SHORT, &data.m_boneDataNum);

				// ボーンデータ群のメモリ確保
				CMemory::Alloc(&boneDatas, data.m_boneDataNum);

				// ボーンデータ群の読み込み
				int cntBoneData = 0;
				while (RNLib::File().SearchLoop("}")) {
					if (RNLib::File().CheckIdentifier("boneData{")) {

						// エラーメッセージ
						if (cntBoneData >= data.m_boneDataNum) {
							RNLib::Window().Message_ERROR(CreateText("ボーンデータの数が指定数をオーバーしています。\n%s", loadPath));

							// ファイルを閉じる
							RNLib::File().CloseFile();

							return false;
						}

						// ボーンデータ初期化
						BoneData& boneData = boneDatas[cntBoneData];
						boneData = {};

						// 部品情報の読み込みを開始
						while (RNLib::File().SearchLoop("}")) {
							RNLib::File().Scan(CFile::SCAN::SHORT, &boneData.idx, "idx");
							RNLib::File().Scan(CFile::SCAN::MODELIDX, &boneData.modelIdx, "modelIdx");
							RNLib::File().Scan(CFile::SCAN::SHORT, &boneData.parentIdx, "parentIdx");
							RNLib::File().Scan(CFile::SCAN::POS3D, &boneData.relativePos, "relativePos");
							RNLib::File().Scan(CFile::SCAN::ROT3D_CORRECT, &boneData.relativeRot, "relativeRot");
							if (RNLib::File().CheckIdentifier("swaying{")) {
								CMemory::Alloc(&boneData.swaying);

								while (RNLib::File().SearchLoop("}")) {
									RNLib::File().Scan(CFile::SCAN::USHORT, &boneData.swaying->timeMin, "timeMin");
									RNLib::File().Scan(CFile::SCAN::USHORT, &boneData.swaying->timeAdd, "timeAdd");
									RNLib::File().Scan(CFile::SCAN::FLOAT, &boneData.swaying->distMin, "distMin");
									RNLib::File().Scan(CFile::SCAN::FLOAT, &boneData.swaying->distAdd, "distAdd");
								}
							}
							else if (RNLib::File().CheckIdentifier("follow{")) {
								CMemory::Alloc(&boneData.follow);

								while (RNLib::File().SearchLoop("}")) {
									RNLib::File().Scan(CFile::SCAN::SHORT, &boneData.follow->followIdx, "followIdx");
									RNLib::File().Scan(CFile::SCAN::POS3D, &boneData.follow->posRate, "posRate");
									RNLib::File().Scan(CFile::SCAN::POS3D, &boneData.follow->rotRate, "rotRate");
									RNLib::File().Scan(CFile::SCAN::POS3D, &boneData.follow->scaleRate, "scaleRate");
								}
							}
						}

						// カウントを加算
						cntBoneData++;
					}
				}// ボーンデータ群の読み込み終了…

				// エラーメッセージ
				if (cntBoneData < data.m_boneDataNum) {
					RNLib::Window().Message_ERROR(CreateText("ボーンデータの数が指定数に対して不足しています。\n%s", loadPath));

					// ファイルを閉じる
					RNLib::File().CloseFile();

					return false;
				}
				
				// ボーンデータ1つ1つの親番号をカウントに変換していく
				for (cntBoneData = 0; cntBoneData < data.m_boneDataNum; cntBoneData++) {
					BoneData& boneData = boneDatas[cntBoneData];

					// 親がDATANONEの時、折り返す
					if (boneData.parentIdx == NONEDATA)
						continue;

					// 親番号と一致したボーンデータを親とし、
					// ボーンデータの親番号をカウントに変換する
					int cntBoneData2;
					for (cntBoneData2 = 0; cntBoneData2 < data.m_boneDataNum; cntBoneData2++) {
						if (boneData.parentIdx == boneDatas[cntBoneData2].idx) {
							boneData.parentIdx = cntBoneData2;
							break;
						}
					}

					// 親が見つからなかった時、親番号をDATANONEにする
					if (cntBoneData2 == data.m_boneDataNum)
						boneData.parentIdx = NONEDATA;
				}
			}
			else if (RNLib::File().CheckIdentifier("faceDataSum{")) {
				FaceData*& faceDatas = data.m_faceDatas;

				// フェイスデータ群を解放
				CMemory::Release(&faceDatas);

				// フェイスデータ数読み込み
				RNLib::File().Scan(CFile::SCAN::SHORT, &data.m_faceDataNum);

				// フェイスデータ群のメモリ確保
				CMemory::Alloc(&faceDatas, data.m_faceDataNum);

				// フェイスデータ群の読み込み
				int cntFaceData = 0;
				while (RNLib::File().SearchLoop("}")) {
					if (RNLib::File().CheckIdentifier("faceData{")) {

						// エラーメッセージ
						if (cntFaceData >= data.m_faceDataNum) {
							RNLib::Window().Message_ERROR(CreateText("フェイスデータの数が指定数をオーバーしています。\n%s", loadPath));

							// ファイルを閉じる
							RNLib::File().CloseFile();

							return false;
						}

						// フェイスデータ初期化
						FaceData& faceData = faceDatas[cntFaceData];
						faceData = {};

						// 頂点情報の読み込みを開始
						while (RNLib::File().SearchLoop("}")) {
							struct LocalFunc {
								static void LoadFaceVtxData(CSetUp3D::FaceVtxData& vtxData) {
									while (RNLib::File().SearchLoop("}")) {
										RNLib::File().Scan(CFile::SCAN::USHORT, &vtxData.boneIdx, "boneIdx");
										RNLib::File().Scan(CFile::SCAN::UINT, &vtxData.vtxIdx, "vtxIdx");
										RNLib::File().Scan(CFile::SCAN::POS2D, &vtxData.texPos, "texPos");
									}
								}
							};

							// 頂点情報の読み込み
							if (RNLib::File().CheckIdentifier("vtx0{"))
								LocalFunc::LoadFaceVtxData(faceData.vtxs[0]);
							else if (RNLib::File().CheckIdentifier("vtx1{"))
								LocalFunc::LoadFaceVtxData(faceData.vtxs[1]);
							else if (RNLib::File().CheckIdentifier("vtx2{"))
								LocalFunc::LoadFaceVtxData(faceData.vtxs[2]);
							else if (RNLib::File().CheckIdentifier("vtx3{"))
								LocalFunc::LoadFaceVtxData(faceData.vtxs[3]);

							RNLib::File().Scan(CFile::SCAN::TEXIDX, &faceData.texIdx, "texIdx");
							RNLib::File().Scan(CFile::SCAN::COLOR, &faceData.col, "col");
						}

						// カウントを加算
						cntFaceData++;
					}
				}// フェイスデータ群の読み込み終了…

				// エラーメッセージ
				if (cntFaceData < data.m_faceDataNum) {
					RNLib::Window().Message_ERROR(CreateText("フェイスデータの数が指定数に対して不足しています。\n%s", loadPath));

					// ファイルを閉じる
					RNLib::File().CloseFile();

					return false;
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
		RNLib::Window().Message_ERROR(CreateText("セットアップ3Dデータファイルが存在しません。\n%s", loadPath));
	}

	return false;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]データクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CSetUp3D::CData::CData() {

	m_boneDatas   = NULL;
	m_boneDataNum = 0;
	m_faceDatas   = NULL;
	m_faceDataNum = 0;
}

//========================================
// デストラクタ
//========================================
CSetUp3D::CData::~CData() {

	// 解放処理
	Release();
}

//========================================
// 解放処理
//========================================
void CSetUp3D::CData::Release(void) {

	for (int cntBone = 0; cntBone < m_boneDataNum; cntBone++) {
		CMemory::Release(&m_boneDatas[cntBone].swaying);
		CMemory::Release(&m_boneDatas[cntBone].follow);
	}
	CMemory::Release(&m_boneDatas);
	CMemory::Release(&m_faceDatas);
}