//========================================
// 
// 当たり判定の処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]当たり判定クラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CHitTest::CHitTest() {

}

//========================================
// デストラクタ
//========================================
CHitTest::~CHitTest() {

}

//========================================
// 初期化処理
//========================================
void CHitTest::Init(void) {

}

//========================================
// 終了処理
//========================================
void CHitTest::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CHitTest::Update(void) {

}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]当たり判定クラス(XZ平面)
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// [静的]点から矩形の重なり判定
//========================================
bool CHitTest::XZ::OverlapPointToSquare(const Pos3D& basePos, const Pos3D& targetPos, const Scale2D& targetScale) {

	const float targetWidthHalf = targetScale.x * 0.5f;
	const float targetDepthHalf = targetScale.y * 0.5f;
	const float targetFront = targetPos.z - targetDepthHalf;
	const float targetBack  = targetPos.z + targetDepthHalf;
	const float targetLeft  = targetPos.x - targetWidthHalf;
	const float targetRight = targetPos.x + targetWidthHalf;

	if (basePos.x >= targetLeft && basePos.x <= targetRight &&
		basePos.z >= targetFront && basePos.z <= targetBack) {
		return true;
	}

	return false;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]当たり判定クラス(三次元空間)
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// [静的]球がカメラの描画範囲内にあるかどうか判定
//========================================
bool CHitTest::XYZ::InPointToCameraView(const D3DXVECTOR3& point, const D3DXVECTOR3& cameraPosition, const D3DXVECTOR3& cameraLookAt, float cameraWidth, float cameraHeight, float fov)
{
    // Calculate the view matrix
    D3DXMATRIX viewMatrix;
    D3DXMatrixLookAtLH(&viewMatrix, &cameraPosition, &cameraLookAt, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

    // Calculate the projection matrix
    D3DXMATRIX projectionMatrix;
    D3DXMatrixPerspectiveFovLH(&projectionMatrix, fov, cameraWidth / cameraHeight, 0.1f, 1000.0f);

    // Combine view and projection matrices
    D3DXMATRIX viewProjectionMatrix = viewMatrix * projectionMatrix;

    // Transform the point to clip space
    D3DXVECTOR4 clipSpace;
    D3DXVec3Transform(&clipSpace, &point, &viewProjectionMatrix);

    // Check if the point is within the clip space boundaries
    return (clipSpace.x >= -1.0f && clipSpace.x <= 1.0f &&
        clipSpace.y >= -1.0f && clipSpace.y <= 1.0f &&
        clipSpace.z >= 0.0f && clipSpace.z <= 1.0f);
}