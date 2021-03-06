#include "ReimuSubPlaneBullet.h"

ReimuSubPlaneBullet::ReimuSubPlaneBullet()
{
	auto stgResources = STGResources::GetInstance();

	SetTexture(stgResources->texReimu);
	SetTexRect(Rect(64, 80, 160, 176));
	SetAngle(90);
	SetSpeed(8.0f);
	SetDamage(9.0f);
	SetHitRange(15.0f);
}

ReimuSubPlaneBullet::~ReimuSubPlaneBullet()
{
}

void ReimuSubPlaneBullet::Update()
{
	PlayerBullet::Update();

	auto engine = STGEngine::GetInstance();
	auto enemyList = engine->GetEnemyList();

	if (position.y > 464 || position.y < -16 || position.x < -16 || position.x > 400)
	{
		MarkDestroy();
	}

	if (this->target == nullptr)
	{
		float minDist = 999999.0f;
		Ptr<Enemy> nearestEnemy = nullptr;
		auto iter = enemyList->GetIterator();
		while (iter->HasNext())
		{
			auto enemy = iter->Next();
			auto enemyPos = enemy->GetPosition();
			auto diffX = enemyPos.x - position.x;
			auto diffY = enemyPos.y - position.y;
			float dist = sqrt(diffX * diffX + diffY * diffY);
			if (dist < minDist)
			{
				nearestEnemy = enemy;
				minDist = dist;
			}
		}
		if (nearestEnemy)
		{
			this->target = nearestEnemy;
		}
	}

	if (this->target)
	{
		if (this->target->GetLife() <= 0)
		{
			this->target = nullptr;
		}
		else
		{
			auto enemyPos = this->target->GetPosition();
			auto diffX = enemyPos.x - position.x;
			auto diffY = enemyPos.y - position.y;
			float distSquare = diffX * diffX + diffY * diffY;

			auto wantAngle = Math::ToAngle(diffX, diffY);
			auto myAngle = Math::StandardizeAngle(angle);
			double diffAngle = wantAngle - myAngle;
			if (fabs(diffAngle) > 180)
			{
				if (diffAngle > 0)
				{
					diffAngle -= 360;
				}
				else
				{
					diffAngle += 360;
				}
			}
			if (fabs(diffAngle) > 5 && distSquare > 30)
			{
				this->angle += diffAngle / 5;
			}
			else
			{
				SetAngle(wantAngle);
			}
		}
	}

	SetRotation(GetAngle());
}