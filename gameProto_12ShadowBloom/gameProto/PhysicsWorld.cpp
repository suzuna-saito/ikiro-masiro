#include <typeinfo>
#include <algorithm>

#include "Renderer.h"
#include "Shader.h"
#include "PhysicsWorld.h"
#include "Actor.h"
#include "playerActor.h"
#include "EnemyActorBase.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "coin.h"

// PhysicsWorld�R���X�g���N�^
PhysicsWorld::PhysicsWorld()
	: mBoolDebugMode(false)
{
	// �����R���|�[�l���g�z��̊m��
	mBGBoxs.reserve(256);
	printf("PysicsWorld �쐬\n");

	InitBoxVertices();

	mLineShader = new Shader();
	mLineShader->Load("shaders/LineWorld.vert", "shaders/Line.frag");

}

// �f�X�g���N�^
PhysicsWorld::~PhysicsWorld()
{
	printf("PysicsWorld �j��\n");
	mBGBoxs.clear();
	delete mLineShader;
}

// BoxCollider�̒ǉ�
void PhysicsWorld::AddBoxCollider(EnumPhysicsType type, BoxCollider *box)
{
	// �v���[���[�����蔻��ǉ�
	if (type == EnumPhysicsType::EnumPlayer)
	{
		mPlayerBoxs.push_back(box);
	}

	// �w�i�����蔻��ǉ�
	if (type == EnumPhysicsType::EnumBG)
	{
		mBGBoxs.push_back(box);
	}

	// �R�C�������蔻��ǉ�
	if (type == EnumPhysicsType::EnumCoin)
	{
		mCoins.push_back(box);
	}

	// �G�����蔻��ǉ�
	if (type == EnumPhysicsType::EnumEnemy)
	{
		mEnemies.push_back(box);
	}

	// �w�i�Ƃ̓����蔻��p�g���K�[
	if (type == EnumPhysicsType::EnumBGTrigger)
	{
		mBGTriggers.push_back(box);
	}

	// �v���C���[�̍U������p
	if (type == EnumPhysicsType::EnumPlayerAttack)
	{
		mPlayerAttackBox.push_back(box);
	}
}

// boxCollider�̍폜
void PhysicsWorld::RemoveBoxCollider(BoxCollider* box)
{
	// �v���[���[�{�b�N�X���ɂ���H
	auto iterPlayer = std::find(mPlayerBoxs.begin(), mPlayerBoxs.end(), box);
	if (iterPlayer != mPlayerBoxs.end())
	{
		mPlayerBoxs.erase(iterPlayer);
		return;
	}

	// BackGround���ɂ���H
	auto iterBG = std::find(mBGBoxs.begin(), mBGBoxs.end(), box);
	if (iterBG != mBGBoxs.end())
	{
		mBGBoxs.erase(iterBG);
		return;
	}
	// BackGround���ɂ���H
	auto iterCoin = std::find(mCoins.begin(), mCoins.end(), box);
	if (iterCoin != mCoins.end())
	{
		mCoins.erase(iterCoin);
		return;
	}

	// �G�ɂ���H
	auto iterEnemy = std::find(mEnemies.begin(), mEnemies.end(), box);
	if (iterEnemy != mEnemies.end())
	{
		mEnemies.erase(iterEnemy);
		return;
	}

	// �g���K�[���ɂ���H
	auto iterTrig = std::find(mBGTriggers.begin(), mBGTriggers.end(), box);
	if(iterTrig != mBGTriggers.end())
	{
		mBGTriggers.erase(iterTrig);
		return;
	}

	// �v���C���[�U������ɂ���H
	auto iterPAttack = std::find(mPlayerAttackBox.begin(), mPlayerAttackBox.end(), box);
	if (iterPAttack != mPlayerAttackBox.end())
	{
		mPlayerAttackBox.erase(iterPAttack);
		return;
	}
}

// �f�o�b�O�p�@�{�b�N�X���X�g�\��
void PhysicsWorld::DebugShowBoxLists()
{

	if (mBGBoxs.size())
	{
		printf("\n-----------------PhysicsList--BGLists---------------\n");
		for (auto m : mBGBoxs)
		{
			Vector3 pos = m->GetOwner()->GetPosition();
			printf("%6d ", m->mGlobalID);
			printf("(% 7.2f,% 7.2f % 7.2f)-", pos.x, pos.y, pos.z);
			printf("[%p]\n", m->GetOwner());
		}
	}
}

void PhysicsWorld::Collision()
{
	// �����蔻�茟�o�O�ɂ��ׂẴg���K�[�����Z�b�g
	for (auto t : mBGTriggers)
	{
		t->SetHitTriggerFlag(false);
	}

	// �v���[���[�Ɣw�i�Փ�
	PlayerAndBGTest();

	// �G�Ɣw�i�Փ�
	EnemyAndBGTest();

	// �g���K�[�Ɣw�i�̃q�b�g���ׂ�
	TriggerAndBGTest();

	// �v���C���[�̍U���ƓG�Ƃ̃q�b�g���ׂ�
	PlayerAttackAndEnemyTest();

}

void PhysicsWorld::DebugShowBox()
{
	// �f�o�b�O���[�h���H
	if (!mBoolDebugMode)
	{
		return;
	}

	// AABB�`�揀��
	Matrix4 scale,trans,world,view, proj, viewProj;
	view = RENDERER->GetViewMatrix();
	proj = RENDERER->GetProjectionMatrix();
	viewProj = view * proj;
	mLineShader->SetActive();
	mLineShader->SetMatrixUniform("uViewProj", viewProj);

	// �����蔻��{�b�N�X�`��
	DrawBoxs(mBGBoxs    , Color::Red);
	DrawBoxs(mPlayerBoxs, Color::Blue);
	DrawBoxs(mCoins     , Color::LightPink);
	DrawBoxs(mEnemies   , Color::White);
	DrawBoxs(mBGTriggers, Color::LightGreen);
	DrawBoxs(mPlayerAttackBox, Color::Red);
}

void PhysicsWorld::DrawBoxs(std::vector<class BoxCollider*>& boxs, const Vector3& color)
{
	Matrix4 scaleMat, posMat, worldMat;
	Vector3 scale, pos;

		mLineShader->SetVectorUniform("uColor", color);
	for (auto item : boxs)
	{
		AABB box;
		Vector3 min, max;
		box = item->GetWorldBox();

		// �{�b�N�X�̃X�P�[���ƈʒu���擾
		min = box.mMin;
		max = box.mMax;
		scale = max - min;
		pos = min;
		
		scaleMat = Matrix4::CreateScale(scale);
		posMat = Matrix4::CreateTranslation(pos);

		worldMat = scaleMat * posMat;
		mLineShader->SetMatrixUniform("uWorld", worldMat);

		glBindVertexArray(mBoxVAO);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	}

}


void PhysicsWorld::PlayerAndBGTest()
{
	//�w�i�ƃv���[���[�̏Փˌ��o
	for (auto p : mPlayerBoxs)
	{
		for (auto b : mBGBoxs)
		{
			BoxCollider *player = p;
			BoxCollider *box = b;
			if (Intersect(player->GetWorldBox(), b->GetWorldBox()))
			{
				//�v���[���[�̕ǂ߂荞�ݏC��������
				dynamic_cast<PlayerActor *>(player->GetOwner())->FixCollision( player, box );
			}
		}
	}

	// �v���[���[�ƃR�C���̏Փˌ��o
	for (auto p : mPlayerBoxs)
	{
		for (auto c : mCoins)
		{
			// �R�C���ƃv���[���[���q�b�g�������H
			if (Intersect(p->GetWorldBox(), c->GetWorldBox()))
			{
				dynamic_cast<Coin*>(c->GetOwner())->GetCoin();
			}
		}
	}
}

// �G�Ɣw�i�����蔻��
void PhysicsWorld::EnemyAndBGTest()
{
	for (auto e : mEnemies)
	{
		for (auto bg : mBGBoxs)
		{
			if (Intersect(e->GetWorldBox(), bg->GetWorldBox()))
			{
				dynamic_cast<EnemyActorBase*>(e->GetOwner())->OnCollision(e, bg);
			}
		}
	}
}

// �g���K�[�Ɣw�i�Ƃ̏Փ˔���
void PhysicsWorld::TriggerAndBGTest()
{
	for (auto trigger : mBGTriggers)
	{
		for (auto bg : mBGBoxs)
		{
			// �Փ˂�����g���K�[�̃t���O��ON�ɂ���
			if (Intersect(trigger->GetWorldBox(), bg->GetWorldBox()))
			{
				trigger->SetHitTriggerFlag(true);
			}
		}
	}
}
void PhysicsWorld::PlayerAttackAndEnemyTest()
{
	for (auto pa : mPlayerAttackBox)
	{
		for (auto e : mEnemies)
		{
			if (Intersect(pa->GetWorldBox(), e->GetWorldBox()))
			{
				dynamic_cast<EnemyActorBase*>(e->GetOwner())->OnCollision(e, pa);
				dynamic_cast<PlayerActor*>(pa->GetOwner())->HitEnemy(pa, e);
			}
		}
	}
}


void PhysicsWorld::InitBoxVertices()
{
	// �{�b�N�X���_���X�g
	float vertices[] = {
		0.0f, 0.0f, 0.0f,  // min
		1.0f, 0.0f, 0.0f,  
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,  // max
		0.0f, 1.0f, 1.0f,
	};
	// �{�b�N�X�̃��C�����X�g
	unsigned int lineList[] = {
		0,1,
		1,2,
		2,3,
		3,0,
		4,5,
		5,6,
		6,7,
		7,4,
		0,4,
		1,5,
		2,6,
		3,7,
	};
	unsigned int vbo, ebo;
	glGenVertexArrays(1, &mBoxVAO);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(mBoxVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineList), lineList, GL_STATIC_DRAW);
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}



