#include "Bullet.h"
#include "BulletDelegate.h"
#include "../STGEngine.h"

BulletDelegate::BulletDelegate(Bullet* bullet)
{
	this->bullet = bullet;
}

BulletDelegate::~BulletDelegate()
{

}

////////////////////   Bullet    //////////////////////

Bullet::Bullet()
{
	bulletDelegate = nullptr;

	dirSame = true;
}

Bullet::~Bullet()
{
	TH_SAFE_RELEASE(bulletDelegate);
}

void Bullet::Update()
{	
	Sprite::Update();

	auto engine = STGEngine::GetInstance();

	auto player = engine->GetPlayer();
	if (player->IsInvincible())
	{
		if (Hit(player->GetPosition().x, player->GetPosition().y, player->GetRadius()))
		{
			MarkDestroy();
		}
	}
	else if (player->IsDamageable())
	{
		if (Hit(player->GetPosition().x, player->GetPosition().y, player->GetRadius()))
		{
			player->Hitten();
			MarkDestroy();
		}
	}

	if (dirSame)
	{
		SetRotation(angle - 90);
	}

	if (autoDelete)
	{
		if (position.x < -32 || position.x > 416 || position.y < -32 || position.y > 480)
		{
			MarkDestroy();
		}
	}
}

void Bullet::OnDie()
{
	MarkDestroy();

	auto engine = STGEngine::GetInstance();
	auto stgResources = STGResources::GetInstance();

	Particle3D* effect[1];
	for (int i = 0; i < 1; i++)
	{
		int effectLife = 40;

		effect[i] = new Particle3D();
		effect[i]->SetTexture(stgResources->texFourAngleStar);
		effect[i]->SetPosition(position);
		effect[i]->SetLife(effectLife);

		switch (bulletColor)
		{
		case RED:
			effect[i]->SetTexRect(Rect(0, 32, 0, 32));
			break;
		case PURPLE:
			effect[i]->SetTexRect(Rect(32, 64, 0, 32));
			break;
		case BLUE:
			effect[i]->SetTexRect(Rect(64, 96, 0, 32));
			break;
		case TEAL:
			effect[i]->SetTexRect(Rect(96, 128, 0, 32));
			break;
		}

		float effectAngle = ToRad(Random(0, 359));
		float dist = Random(0, 20);

		float effectScale = Random(20, 50) / 100.0f;
		effect[i]->SetScale(Vector3f(effectScale, effectScale, 1));

		effect[i]->SetRotatingAxis(Vector3f(Random(0, 100), Random(0, 100), Random(0, 100)));
		effect[i]->SetRotatingSpeed(Random(50, 100) / 10.0f);
		effect[i]->SetAlpha(0.6f);

		TweenSequence* sequence = new TweenSequence();
		sequence->AddTween(new Delay(effectLife / 2));
		sequence->AddTween(new ScaleTo(Vector3f(0, 0, 1), 30, Tweener::SIMPLE));
		effect[i]->AddTween(sequence);

		effect[i]->AddTween(new MoveBy(Vector3f(dist*cos(effectAngle), dist*sin(effectAngle), 0),
			effectLife, Tweener::EASE_OUT));

		engine->AddParticle(effect[i]);
	}
}

void Bullet::SetType(Type type)
{
	auto stgResources = STGResources::GetInstance();

	TH_SAFE_RELEASE(bulletDelegate);

	switch (type)
	{
	case LINDAN_RED:
		bulletDelegate = new LinDan(this);
		bulletDelegate->Retain();
		SetTexture(stgResources->texBullet01);
		SetTexRect(Rect(32, 48, 16, 32));
		SetBulletColor(RED);
		break;
	case LINDAN_PURPLE:
		bulletDelegate = new LinDan(this);
		bulletDelegate->Retain();
		SetTexture(stgResources->texBullet01);
		SetTexRect(Rect(64, 80, 16, 32));
		SetBulletColor(PURPLE);
		break;
	case LINDAN_BLUE:
		bulletDelegate = new LinDan(this);
		bulletDelegate->Retain();
		SetTexture(stgResources->texBullet01);
		SetTexRect(Rect(96, 112, 16, 32));
		SetBulletColor(BLUE);
		break;
	case LINDAN_TEAL:
		bulletDelegate = new LinDan(this);
		bulletDelegate->Retain();
		SetTexture(stgResources->texBullet01);
		SetTexRect(Rect(128, 144, 16, 32));
		SetBulletColor(TEAL);
		break;
	case LINDAN_GREEN:
		bulletDelegate = new LinDan(this);
		bulletDelegate->Retain();
		SetTexture(stgResources->texBullet01);
		SetTexRect(Rect(160, 176, 16, 32));
		SetBulletColor(GREEN);
		break;
	case LINDAN_YELLOW:
		bulletDelegate = new LinDan(this);
		bulletDelegate->Retain();
		SetTexture(stgResources->texBullet01);
		SetTexRect(Rect(192, 208, 16, 32));
		SetBulletColor(YELLOW);
		break;
	case LINDAN_ORANGE:
		bulletDelegate = new LinDan(this);
		bulletDelegate->Retain();
		SetTexture(stgResources->texBullet01);
		SetTexRect(Rect(224, 240, 16, 32));
		SetBulletColor(YELLOW);
		break;
	case LINDAN_GREY:
		bulletDelegate = new LinDan(this);
		bulletDelegate->Retain();
		SetTexture(stgResources->texBullet01);
		SetTexRect(Rect(240, 256, 16, 32));
		SetBulletColor(BRIGHT_GREY);
		break;
	}
}