#include "LevelActor.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "BoxCollider.h"
#include "BGBlock.h"
#include "rapidjson/rapidjson.h"
#include "RapidJsonHelper.h"


LevelActor::LevelActor()
{
}

LevelActor::~LevelActor()
{
}

void LevelActor::UpdateActor(float deltaTime)
{
}

void LevelActor::LoadLevel(const char* gpmeshFileName, const char* collisionJsonFileName, Vector3& offset)
{
	Mesh* mesh = RENDERER->GetMesh(gpmeshFileName);
	if (!mesh)
	{
		printf("mesh読み込み失敗 : %s\n", gpmeshFileName);
		return;
	}
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mesh);

	rapidjson::Document doc;
	if (!openJsonFile(doc, collisionJsonFileName))
	{
		printf("JSONファイル読み込み失敗 : %s\n", collisionJsonFileName);
		return;
	}

	if (!IsExistArrayName(doc, "collision"))
	{
		printf("JSONファイルパースエラー : %s\n", collisionJsonFileName);
		return;
	}
	auto collisions = doc["collision"].GetArray();
	int collisionSize = collisions.Size();

	// JSONから読み取った内容から CollisionAABBの位置・スケールをすべて取得
	// BoxColliderとして登録
	for (int i = 0; i < collisionSize; i++)
	{
		BoxCollider* pEntryBox;
		AABB col;
		std::string itemName;
		Vector3 position, scale;

		// UE4の基本ボックスの最大、最小値のデフォルト値
		Vector3 max( 50, 50, 50);
		Vector3 min(-50,-50,-50);

		// JSONから一とスケール読み込み
		position.x = ForceGetFloat(collisions[i]["position_x"]);
		position.y = ForceGetFloat(collisions[i]["position_y"]);
		position.z = ForceGetFloat(collisions[i]["position_z"]);

		scale.x = ForceGetFloat(collisions[i]["scale_x"]);
		scale.y = ForceGetFloat(collisions[i]["scale_y"]);
		scale.z = ForceGetFloat(collisions[i]["scale_z"]);

		// 読み込んだ位置スケールから当たり判定ボックスの最大最小求める
		col.mMax = scale * max + position + offset;
		col.mMin = scale * min + position + offset;

		// 当たり判定をPhysicsに登録
		pEntryBox = new BoxCollider(this,EnumPhysicsType::EnumBG);
		pEntryBox->SetObjectBox(col);
	}
}


