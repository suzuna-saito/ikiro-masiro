#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "LevelManager.h"
#include "BGBlock.h"
#include "coin.h"
#include "RapidJsonHelper.h"

#include "LevelAssetElement.h"
#include <unordered_map>

LevelManager::LevelManager()
	:mCamera(nullptr)
{

	// ファイルからアセット配置データ読み込み
	std::vector<LevelAssetElement> assets;
	readAssetJson(assets, "assets/dungeon.json");

	// メッシュ文字列とメッシュ読み込み
	std::unordered_map<std::string, Mesh*> meshMap;
	meshMap.emplace("Mesh_Dungeons_Ground_02", GAMEINSTANCE.GetRenderer()->GetMesh("assets/dungeon/Mesh_Dungeons_Ground_02.gpmesh"));
	meshMap.emplace("Mesh_Dungeons_Ground_11", GAMEINSTANCE.GetRenderer()->GetMesh("assets/dungeon/Mesh_Dungeons_Ground_11.gpmesh"));
	meshMap.emplace("Mesh_Dungeons_Ground_12", GAMEINSTANCE.GetRenderer()->GetMesh("assets/dungeon/Mesh_Dungeons_Ground_12.gpmesh"));
	meshMap.emplace("Mesh_Dungeons_Wall_23"  , GAMEINSTANCE.GetRenderer()->GetMesh("assets/dungeon/Mesh_Dungeons_Wall_23.gpmesh"));
	meshMap.emplace("Mesh_Dungeons_Wall_12"  , GAMEINSTANCE.GetRenderer()->GetMesh("assets/dungeon/Mesh_Dungeons_Wall_12.gpmesh"));

	// アセット配置データから読み込み
	for (auto iter : assets)
	{
		BGBlock* block;
		block = new BGBlock;
		block->SetMesh(meshMap[iter.mFileName.c_str()]);
		block->SetPosition(iter.mPosition);
		block->SetRotation(iter.mRotation);
		block->SetScale(iter.mScale);
	}

}

LevelManager::~LevelManager()
{
	mBlockMeshs.clear();
}


void LevelManager::SetViewRange(float width, float height)
{
	mLeftUpPos.x    = -width  * 0.5f;
	mRightDownPos.x =  width  * 0.5f;
	mLeftUpPos.y    = -height * 0.5f;
	mRightDownPos.y =  height * 0.5f;
}

void LevelManager::Update()
{

}

// Tiled形式のJsonファイルを読み込む
bool LevelManager::readTiledJson(std::vector<std::vector<int>>& mapData, const char* filename, const char* layerName)
{
	//RapidJsonドキュメントとして開けるか？
	rapidjson::Document doc;
	if (!openJsonFile(doc, filename))
	{
		return false;
	}

	// 開いたDocmentのtype は mapか？
	if (!IsExistMemberAndValue(doc, "type", "map"))
	{
		return false;
	}

	// 開いたDocumentにlayerはあるか？
	if (!IsExistArrayName(doc, "layers"))
	{
		return false;
	}
	// layer数取得
	auto layer = doc["layers"].GetArray();
	int  layerSize = layer.Size();

	// レイヤー名から該当レイヤーの添え字値を調べる
	std::string layerNameString(layerName);
	int layerIndex = findLayerIndex(layer, layerNameString);
	if (layerIndex < 0)
	{
		return false;
	}

	// layer内にdataはある？    
	rapidjson::Value::ConstMemberIterator itr = layer[layerIndex].FindMember("data");
	if (itr == layer[layerIndex].MemberEnd())
	{
		printf("レイヤー名:%sにマップデータがありません\n", layerName);
		return false;
	}

	// レイヤーよりrapidjson配列作成
	const rapidjson::Value& rapidArrayData = layer[layerIndex]["data"].GetArray();
	int width, height;
	width = layer[layerIndex]["width"].GetInt();
	height = layer[layerIndex]["height"].GetInt();

	// ユーザー配列確保 行方向、列方向の順番にサイズ確保
	mapData.resize(height);
	for (auto& mapIter : mapData)
	{
		mapIter.resize(width);
	}

	// ユーザー配列（2次元）にデータをコピーする
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			mapData[y][x] = rapidArrayData[y * width + x].GetInt();
		}
	}
	return true;
}


// レベルアセット形式のJSONファイル読み込み
bool LevelManager::readAssetJson(std::vector<LevelAssetElement>& assetList, const char* fileName)
{
	rapidjson::Document doc;
	// JSONファイルが開けるかをチェック
	if (!openJsonFile(doc, fileName))
	{
		return false;
	}
	if (!IsExistArrayName(doc, "data"))
	{
		return false;
	}

	auto elements = doc["data"].GetArray();
	int elementSize = elements.Size();

	// アセット読み込み
	LevelAssetElement lae;
	for (int i = 0; i < elementSize; i++)
	{
		std::string assetName;
		Vector3 position, scale, rotation;
		lae.mFileName = elements[i]["gpmeshFileName"].GetString();

		lae.mPosition.x = ForceGetFloat(elements[i]["position_x"]);
		lae.mPosition.y = ForceGetFloat(elements[i]["position_y"]);
		lae.mPosition.z = ForceGetFloat(elements[i]["position_z"]);

		lae.mScale.x    = ForceGetFloat(elements[i]["scale_x"]);
		lae.mScale.y    = ForceGetFloat(elements[i]["scale_y"]);
		lae.mScale.z    = ForceGetFloat(elements[i]["scale_z"]);

		lae.mRotation.x = Math::ToRadians(ForceGetFloat(elements[i]["rot_x"]));
		lae.mRotation.y = Math::ToRadians(ForceGetFloat(elements[i]["rot_y"]));
		lae.mRotation.z = Math::ToRadians(ForceGetFloat(elements[i]["rot_z"]));

		assetList.emplace_back(lae);
	}
	return true;
}

// レイヤー内に　layerNameのレイヤーを検索し、そのインデックス値を返す
int LevelManager::findLayerIndex(rapidjson::GenericArray<false, rapidjson::Value>& layer, std::string& layerName)
{
	int  layerSize = layer.Size();

	int i;
	std::string layerNameString(layerName);
	for (i = 0; i < layerSize; i++)
	{
		std::string currentLayerName = layer[i]["name"].GetString();
		printf("%s\n", currentLayerName.c_str());
		if (layerNameString == currentLayerName)
		{
			break;
		}
	}
	if (i == layerSize)
	{
		return -1;
	}
	return i;
}
