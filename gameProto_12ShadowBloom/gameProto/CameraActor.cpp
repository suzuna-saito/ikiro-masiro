#include "game.h"
#include "Game.h"
#include "CameraActor.h"

CameraActor::CameraActor(Actor *targetActor)
	:mTargetActor(targetActor)
{
	mPosition = Vector3(0, 100, 100);

	//デフォルトカメラ値セット
	mViewMatrix = Matrix4::CreateLookAt(Vector3(-100, 0, 100),
 		                                Vector3(0, 0, 0),
		                                Vector3(0, 0, 1));

	// ゲームシステム側にこれがアクティブカメラであることを通知
	SetActive();
}

CameraActor::~CameraActor()
{
	// ゲームシステム側にアクティブカメラがいなくなったことを通知
	// カメラアクター自体はActorでデストラクトされる
	GAMEINSTANCE.InActiveCamera(this);
}

// オーバーライドして使わない場合
void CameraActor::UpdateActor(float deltaTime)
{
}

void CameraActor::Update(float deltaTime)
{
	UpdateActor(deltaTime);

	// カメラ視線ベクトル、カメラ行列作成
	mViewVector = mViewTarget - mPosition;
	mViewMatrix = Matrix4::CreateLookAt(mPosition,
		                                mViewTarget,
		                                Vector3(0,0,1));
}

//ゲームシステム側にこれがアクティブカメラであることを手動で通知する
void CameraActor::SetActive()
{
	//ゲームシステム側にこれがアクティブカメラであることを通知
	GAMEINSTANCE.SetCameraActor(this);
}

// 位置方向初期化
void CameraActor::Init( const Vector3 & cameraPos, const Vector3 & targetPos, const Vector3 & upVec)
{
	mPosition = cameraPos;
	mViewTarget = targetPos;
	mViewVector = targetPos - cameraPos;

	mViewMatrix = Matrix4::CreateLookAt(cameraPos,
		                                targetPos,
		                                upVec);
}

