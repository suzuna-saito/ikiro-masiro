// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL/SDL_log.h>
#include "Math.h"
#include "RapidJsonHelper.h"

int LoadStageTextures(const rapidjson::Document& doc, TextureStage texStage, const char* stgString);

namespace
{
	union Vertex
	{
		float f;
		uint8_t b[4];
	};
}

Mesh::Mesh()
	: mVertexArray(nullptr)
	, mRadius(0.0f)
	, mSpecPower(100.0f)
	, mLuminance(1.0f)
{
	mStageDefTexture.emplace(TextureStage::DiffuseMap , 0);
	mStageDefTexture.emplace(TextureStage::NormalMap  , 0);
	mStageDefTexture.emplace(TextureStage::SpecularMap, 0);
	mStageDefTexture.emplace(TextureStage::EmissiveMap, 0);
}

Mesh::~Mesh()
{
}

//���b�V���̃��[�h
bool Mesh::Load(const std::string & fileName, Renderer* renderer)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		printf("File not found: Mesh %s", fileName.c_str());
		return false;
	}
	// JSON�̉�͂��s��
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	// JSON�I�u�W�F�N�g���H
	if (!doc.IsObject())
	{
		printf("Mesh %s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// �o�[�W�����`�F�b�N
	if (ver != 1)
	{
		printf("Mesh %s not version 1", fileName.c_str());
		return false;
	}

	mShaderName = doc["shader"].GetString();

	// ���_���C�A�E�g�ƃT�C�Y���t�@�C������Z�b�g
	VertexArray::Layout layout = VertexArray::PosNormTex;
	size_t vertSize = 8;

	std::string vertexFormat = doc["vertexformat"].GetString();
	if (vertexFormat == "PosNormSkinTex")
	{
		layout = VertexArray::PosNormSkinTex;
		// This is the number of "Vertex" unions, which is 8 + 2 (for skinning)s�@1�̒��_�̏W���̐��@�W�@�{�@�Q�i�X�L�j���O���j
		// 3 (�ʒuxyz) + 3(�@��xyz) + 2(Bone�Əd�݁j�{�@2(UV)  �̌v�@10���i40byte) �@
		vertSize = 10;
	}

	// �e�N�X�`���̃��[�h
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		printf("Mesh %s has no textures, there should be at least one", fileName.c_str());
		return false;
	}

	// �X�y�L�����[���x
	mSpecPower = static_cast<float>(doc["specularPower"].GetDouble());

	// �G�~�b�V�u���x�l�i��`����Ă����)
	if (IsExistMember(doc, "luminance"))
	{
		mLuminance = ForceGetFloat(doc["luminance"]);
	}

	// �e�N�X�`���ǂݍ��݁i���t�@�C���`��)
	for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		// ���̃e�N�X�`�����ɓǂ݂���ł�H
		std::string texName = textures[i].GetString();
		Texture* t = renderer->GetTexture(texName);
		if (t == nullptr)
		{
			// �e�N�X�`���ǂݍ��݂̃g���C
			t = renderer->GetTexture(texName);
			if (t == nullptr)
			{
				// NULL�̂܂܂ł���΃f�t�H���g�e�N�X�`�����Z�b�g
				t = renderer->GetTexture("Assets/Default.png");
			}
		}
		mTextures.emplace_back(t);

		if (textures.Size() == 1)
		{
			mStageDefTexture[TextureStage::DiffuseMap] = t->GetTextureID();
		}
	}

	// �e�N�X�`���ǂݍ���(�V�t�@�C���`��)
	if (IsExistMember(doc, "diffusemap"))
	{
		mStageDefTexture[TextureStage::DiffuseMap] = LoadStageTextures(doc, TextureStage::DiffuseMap, "diffusemap");
	}
	mStageDefTexture[TextureStage::NormalMap]   = LoadStageTextures(doc, TextureStage::NormalMap  , "normalmap");
	mStageDefTexture[TextureStage::SpecularMap] = LoadStageTextures(doc, TextureStage::SpecularMap, "specularmap");
	mStageDefTexture[TextureStage::EmissiveMap] = LoadStageTextures(doc, TextureStage::EmissiveMap, "emissivemap");

	// ���_�ǂݍ���
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		printf("Mesh %s has no vertices", fileName.c_str());
		return false;
	}

	// ���_�f�[�^
	std::vector<Vertex> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	mRadius = 0.0f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		// �����_�ŁA�W�̗v�f(�ʒuxyz �@��xyz �e�N�X�`��uv�̗v�f�j�������Ă���
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray())
		{
			printf("Unexpected vertex format for %s", fileName.c_str());
			return false;
		}

		// ���_�ʒu����@�o�E���f�B���O�X�t�B�A�̔��a���v�Z����i���_����̋������ő�̂��̂𔼌a�Ƃ���j
		Vector3 pos(static_cast<float>(vert[0].GetDouble()),
			        static_cast<float>(vert[1].GetDouble()),
			        static_cast<float>(vert[2].GetDouble()));
		mRadius = Math::Max(mRadius, pos.LengthSq());
		// �o�E���f�B���O�{�b�N�X���v�Z
		if (i == 0)
		{
			mBox.InitMinMax(pos);
		}
		mBox.UpdateMinMax(pos);

		// ���_���C�A�E�g�� PosNormTex�Ȃ�i�{�[���������j
		if (layout == VertexArray::PosNormTex)
		{
			Vertex v;
			// Add the floats�@float�l��ǉ�
			for (rapidjson::SizeType j = 0; j < vert.Size(); j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}
		}
		else // �{�[���f�[�^����Ȃ�@PosNormSkinTex�Ȃ�
		{
			Vertex v;
			// Add pos/normal�@���_�Ɩ@����ǉ��@6��
			for (rapidjson::SizeType j = 0; j < 6; j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}

			// Add skin information�@�X�L�����ǉ��i�{�[���ԍ�:unsigned char��1�o�C�g���j
			for (rapidjson::SizeType j = 6; j < 14; j += 4)  //���[�v�Ƃ��Ă�2��]����@1��]�ڂ̓{�[���ԍ��A2��]�ڂ̓{�[���E�F�C�g��int�Ƃ��Ď���Ă���i�g�p���ɕ���������������ۂ��j
			{
				v.b[0] = vert[j].GetUint();
				v.b[1] = vert[j + 1].GetUint();
				v.b[2] = vert[j + 2].GetUint();
				v.b[3] = vert[j + 3].GetUint();
				vertices.emplace_back(v);
			}

			// Add tex coords�@�e�N�X�`�����W
			for (rapidjson::SizeType j = 14; j < vert.Size(); j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}
		}
	}

	// We were computing length squared earlier�@�o�E���f�B���O�X�t�B�A�̔��a���v�Z
	mRadius = Math::Sqrt(mRadius);

	// Load in the indices�@���_�C���f�b�N�X�����[�h
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		printf("Mesh %s has no indices", fileName.c_str());
		return false;
	}

	//���_�C���f�b�N�X�����������Ă���
	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			printf("Invalid indices for %s", fileName.c_str());
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	// Now create a vertex array�@���_�z����쐬����
	mVertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
		layout, indices.data(), static_cast<unsigned>(indices.size()));
	return true;
}

void Mesh::Unload()
{
	delete mVertexArray;
	mVertexArray = nullptr;

}

Texture* Mesh::GetTexture(int index)
{
	if (index < static_cast<int>(mTextures.size()))
	{
		return mTextures[index];
	}
	else
	{
		return nullptr;
	}
}

int Mesh::GetTextureID(TextureStage stage)
{
	return mStageDefTexture[stage];
}

int LoadStageTextures(const rapidjson::Document& doc, TextureStage texStage, const char* stgString)
{
	std::string noneTexture("none");
	std::string texName;
	Texture* t;
	if (IsExistMember(doc, stgString))
	{
		texName = doc[stgString].GetString();
		if (texName != noneTexture)
		{
			t = RENDERER->GetTexture(texName);
			if (t == nullptr)
			{
				t = RENDERER->GetTexture("Assets/Default.png");
			}
			return t->GetTextureID();
		}
	}
	return 0;
}
