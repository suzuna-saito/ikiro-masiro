#pragma once
#include "Game.h"
#include "rapidjson/document.h"
#include "LevelAssetElement.h"

class LevelManager
{
public:
	LevelManager();
	~LevelManager();
	void SetCamera(class LookDownCamera* cam) { mCamera = cam; }
	void SetViewRange(float width, float height);
	void Update();

private:

	int findLayerIndex(rapidjson::GenericArray<false, rapidjson::Value>& layer, std::string& layerName);
	bool readTiledJson(std::vector<std::vector<int>>& mapData, const char* filename, const char* layerName);
	bool readAssetJson(std::vector<LevelAssetElement>& assetList, const char* fileName);

	Vector3 mLeftUpPos, mRightDownPos;
	Vector3 mNowRangeMin, mNowRangeMax;
	class LookDownCamera *mCamera;
	std::vector<class Mesh*> mBlockMeshs;
	friend class PhysicsWorld;

	int mMapXNum;
	int mMapYNum;

	float mBlockSize;
};