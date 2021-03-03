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
		printf("mesh�ǂݍ��ݎ��s : %s\n", gpmeshFileName);
		return;
	}
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mesh);

	rapidjson::Document doc;
	if (!openJsonFile(doc, collisionJsonFileName))
	{
		printf("JSON�t�@�C���ǂݍ��ݎ��s : %s\n", collisionJsonFileName);
		return;
	}

	if (!IsExistArrayName(doc, "collision"))
	{
		printf("JSON�t�@�C���p�[�X�G���[ : %s\n", collisionJsonFileName);
		return;
	}
	auto collisions = doc["collision"].GetArray();
	int collisionSize = collisions.Size();

	// JSON����ǂݎ�������e���� CollisionAABB�̈ʒu�E�X�P�[�������ׂĎ擾
	// BoxCollider�Ƃ��ēo�^
	for (int i = 0; i < collisionSize; i++)
	{
		BoxCollider* pEntryBox;
		AABB col;
		std::string itemName;
		Vector3 position, scale;

		// UE4�̊�{�{�b�N�X�̍ő�A�ŏ��l�̃f�t�H���g�l
		Vector3 max( 50, 50, 50);
		Vector3 min(-50,-50,-50);

		// JSON�����ƃX�P�[���ǂݍ���
		position.x = ForceGetFloat(collisions[i]["position_x"]);
		position.y = ForceGetFloat(collisions[i]["position_y"]);
		position.z = ForceGetFloat(collisions[i]["position_z"]);

		scale.x = ForceGetFloat(collisions[i]["scale_x"]);
		scale.y = ForceGetFloat(collisions[i]["scale_y"]);
		scale.z = ForceGetFloat(collisions[i]["scale_z"]);

		// �ǂݍ��񂾈ʒu�X�P�[�����瓖���蔻��{�b�N�X�̍ő�ŏ����߂�
		col.mMax = scale * max + position + offset;
		col.mMin = scale * min + position + offset;

		// �����蔻���Physics�ɓo�^
		pEntryBox = new BoxCollider(this,EnumPhysicsType::EnumBG);
		pEntryBox->SetObjectBox(col);
	}
}


