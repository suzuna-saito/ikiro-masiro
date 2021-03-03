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

	// �t�@�C������A�Z�b�g�z�u�f�[�^�ǂݍ���
	std::vector<LevelAssetElement> assets;
	readAssetJson(assets, "assets/dungeon.json");

	// ���b�V��������ƃ��b�V���ǂݍ���
	std::unordered_map<std::string, Mesh*> meshMap;
	meshMap.emplace("Mesh_Dungeons_Ground_02", GAMEINSTANCE.GetRenderer()->GetMesh("assets/dungeon/Mesh_Dungeons_Ground_02.gpmesh"));
	meshMap.emplace("Mesh_Dungeons_Ground_11", GAMEINSTANCE.GetRenderer()->GetMesh("assets/dungeon/Mesh_Dungeons_Ground_11.gpmesh"));
	meshMap.emplace("Mesh_Dungeons_Ground_12", GAMEINSTANCE.GetRenderer()->GetMesh("assets/dungeon/Mesh_Dungeons_Ground_12.gpmesh"));
	meshMap.emplace("Mesh_Dungeons_Wall_23"  , GAMEINSTANCE.GetRenderer()->GetMesh("assets/dungeon/Mesh_Dungeons_Wall_23.gpmesh"));
	meshMap.emplace("Mesh_Dungeons_Wall_12"  , GAMEINSTANCE.GetRenderer()->GetMesh("assets/dungeon/Mesh_Dungeons_Wall_12.gpmesh"));

	// �A�Z�b�g�z�u�f�[�^����ǂݍ���
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

// Tiled�`����Json�t�@�C����ǂݍ���
bool LevelManager::readTiledJson(std::vector<std::vector<int>>& mapData, const char* filename, const char* layerName)
{
	//RapidJson�h�L�������g�Ƃ��ĊJ���邩�H
	rapidjson::Document doc;
	if (!openJsonFile(doc, filename))
	{
		return false;
	}

	// �J����Docment��type �� map���H
	if (!IsExistMemberAndValue(doc, "type", "map"))
	{
		return false;
	}

	// �J����Document��layer�͂��邩�H
	if (!IsExistArrayName(doc, "layers"))
	{
		return false;
	}
	// layer���擾
	auto layer = doc["layers"].GetArray();
	int  layerSize = layer.Size();

	// ���C���[������Y�����C���[�̓Y�����l�𒲂ׂ�
	std::string layerNameString(layerName);
	int layerIndex = findLayerIndex(layer, layerNameString);
	if (layerIndex < 0)
	{
		return false;
	}

	// layer����data�͂���H    
	rapidjson::Value::ConstMemberIterator itr = layer[layerIndex].FindMember("data");
	if (itr == layer[layerIndex].MemberEnd())
	{
		printf("���C���[��:%s�Ƀ}�b�v�f�[�^������܂���\n", layerName);
		return false;
	}

	// ���C���[���rapidjson�z��쐬
	const rapidjson::Value& rapidArrayData = layer[layerIndex]["data"].GetArray();
	int width, height;
	width = layer[layerIndex]["width"].GetInt();
	height = layer[layerIndex]["height"].GetInt();

	// ���[�U�[�z��m�� �s�����A������̏��ԂɃT�C�Y�m��
	mapData.resize(height);
	for (auto& mapIter : mapData)
	{
		mapIter.resize(width);
	}

	// ���[�U�[�z��i2�����j�Ƀf�[�^���R�s�[����
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			mapData[y][x] = rapidArrayData[y * width + x].GetInt();
		}
	}
	return true;
}


// ���x���A�Z�b�g�`����JSON�t�@�C���ǂݍ���
bool LevelManager::readAssetJson(std::vector<LevelAssetElement>& assetList, const char* fileName)
{
	rapidjson::Document doc;
	// JSON�t�@�C�����J���邩���`�F�b�N
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

	// �A�Z�b�g�ǂݍ���
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

// ���C���[���Ɂ@layerName�̃��C���[���������A���̃C���f�b�N�X�l��Ԃ�
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
