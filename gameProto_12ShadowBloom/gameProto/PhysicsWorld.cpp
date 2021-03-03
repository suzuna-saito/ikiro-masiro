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

// PhysicsWorldコンストラクタ
PhysicsWorld::PhysicsWorld()
	: mBoolDebugMode(false)
{
	// 物理コンポーネント配列の確保
	mBGBoxs.reserve(256);
	printf("PysicsWorld 作成\n");

	InitBoxVertices();

	mLineShader = new Shader();
	mLineShader->Load("shaders/LineWorld.vert", "shaders/Line.frag");

}

// デストラクタ
PhysicsWorld::~PhysicsWorld()
{
	printf("PysicsWorld 破棄\n");
	mBGBoxs.clear();
	delete mLineShader;
}

// BoxColliderの追加
void PhysicsWorld::AddBoxCollider(EnumPhysicsType type, BoxCollider *box)
{
	// プレーヤーあたり判定追加
	if (type == EnumPhysicsType::EnumPlayer)
	{
		mPlayerBoxs.push_back(box);
	}

	// 背景あたり判定追加
	if (type == EnumPhysicsType::EnumBG)
	{
		mBGBoxs.push_back(box);
	}

	// コインあたり判定追加
	if (type == EnumPhysicsType::EnumCoin)
	{
		mCoins.push_back(box);
	}

	// 敵当たり判定追加
	if (type == EnumPhysicsType::EnumEnemy)
	{
		mEnemies.push_back(box);
	}

	// 背景との当たり判定用トリガー
	if (type == EnumPhysicsType::EnumBGTrigger)
	{
		mBGTriggers.push_back(box);
	}

	// プレイヤーの攻撃判定用
	if (type == EnumPhysicsType::EnumPlayerAttack)
	{
		mPlayerAttackBox.push_back(box);
	}
}

// boxColliderの削除
void PhysicsWorld::RemoveBoxCollider(BoxCollider* box)
{
	// プレーヤーボックス内にいる？
	auto iterPlayer = std::find(mPlayerBoxs.begin(), mPlayerBoxs.end(), box);
	if (iterPlayer != mPlayerBoxs.end())
	{
		mPlayerBoxs.erase(iterPlayer);
		return;
	}

	// BackGround内にいる？
	auto iterBG = std::find(mBGBoxs.begin(), mBGBoxs.end(), box);
	if (iterBG != mBGBoxs.end())
	{
		mBGBoxs.erase(iterBG);
		return;
	}
	// BackGround内にいる？
	auto iterCoin = std::find(mCoins.begin(), mCoins.end(), box);
	if (iterCoin != mCoins.end())
	{
		mCoins.erase(iterCoin);
		return;
	}

	// 敵にいる？
	auto iterEnemy = std::find(mEnemies.begin(), mEnemies.end(), box);
	if (iterEnemy != mEnemies.end())
	{
		mEnemies.erase(iterEnemy);
		return;
	}

	// トリガー内にいる？
	auto iterTrig = std::find(mBGTriggers.begin(), mBGTriggers.end(), box);
	if(iterTrig != mBGTriggers.end())
	{
		mBGTriggers.erase(iterTrig);
		return;
	}

	// プレイヤー攻撃判定にいる？
	auto iterPAttack = std::find(mPlayerAttackBox.begin(), mPlayerAttackBox.end(), box);
	if (iterPAttack != mPlayerAttackBox.end())
	{
		mPlayerAttackBox.erase(iterPAttack);
		return;
	}
}

// デバッグ用　ボックスリスト表示
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
	// 当たり判定検出前にすべてのトリガーをリセット
	for (auto t : mBGTriggers)
	{
		t->SetHitTriggerFlag(false);
	}

	// プレーヤーと背景衝突
	PlayerAndBGTest();

	// 敵と背景衝突
	EnemyAndBGTest();

	// トリガーと背景のヒット調べる
	TriggerAndBGTest();

	// プレイヤーの攻撃と敵とのヒット調べる
	PlayerAttackAndEnemyTest();

}

void PhysicsWorld::DebugShowBox()
{
	// デバッグモードか？
	if (!mBoolDebugMode)
	{
		return;
	}

	// AABB描画準備
	Matrix4 scale,trans,world,view, proj, viewProj;
	view = RENDERER->GetViewMatrix();
	proj = RENDERER->GetProjectionMatrix();
	viewProj = view * proj;
	mLineShader->SetActive();
	mLineShader->SetMatrixUniform("uViewProj", viewProj);

	// 当たり判定ボックス描画
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

		// ボックスのスケールと位置を取得
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
	//背景とプレーヤーの衝突検出
	for (auto p : mPlayerBoxs)
	{
		for (auto b : mBGBoxs)
		{
			BoxCollider *player = p;
			BoxCollider *box = b;
			if (Intersect(player->GetWorldBox(), b->GetWorldBox()))
			{
				//プレーヤーの壁めり込み修正処理へ
				dynamic_cast<PlayerActor *>(player->GetOwner())->FixCollision( player, box );
			}
		}
	}

	// プレーヤーとコインの衝突検出
	for (auto p : mPlayerBoxs)
	{
		for (auto c : mCoins)
		{
			// コインとプレーヤーがヒットしたか？
			if (Intersect(p->GetWorldBox(), c->GetWorldBox()))
			{
				dynamic_cast<Coin*>(c->GetOwner())->GetCoin();
			}
		}
	}
}

// 敵と背景当たり判定
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

// トリガーと背景との衝突判定
void PhysicsWorld::TriggerAndBGTest()
{
	for (auto trigger : mBGTriggers)
	{
		for (auto bg : mBGBoxs)
		{
			// 衝突したらトリガーのフラグをONにする
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
	// ボックス頂点リスト
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
	// ボックスのラインリスト
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



