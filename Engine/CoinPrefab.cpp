#include "CoinPrefab.h"

#include "SpriteRender.h"
#include "SpriteAnimation.h"
#include "SphereCollider.h"
#include "CoinHandler.h"
// utilities
#include "LoadTexture.h"
#include "LoadSAD.h"

CoinPrefab::CoinPrefab() {
	coinSheet = Load::Texture2D("Files/SpritePack/Others/Coin/coin-sheets.png");
	coinData = Load::SAD("Files/SpritePack/Others/Coin/coin-sheets.sad");

	pickupSheet = Load::Texture2D("Files/SpritePack/Others/Coin/coin-sheet-with-pickup.png");
	pickupData = Load::SAD("Files/SpritePack/Others/Coin/coin-sheet-with-pickup.sad");
}

Transform * CoinPrefab::Create() {
	const unsigned entity = entities->Create();

	Transform* const transform = entities->GetComponent<Transform>(entity);
	transform->SetScale(0.5f);

	SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
	render->SetActive(true);
	render->SetSprite(coinSheet);

	SpriteAnimation* const animation = entities->AddComponent<SpriteAnimation>(entity);
	animation->SetActive(true);
	animation->animations = coinData;
	animation->currentAnimation = "IDLE";

	SphereCollider* const collider = entities->AddComponent<SphereCollider>(entity);
	collider->SetActive(true);

	CoinHandler* const handler = entities->AddComponent<CoinHandler>(entity);
	handler->SetActive(true);
	handler->pickupSheet = pickupSheet;
	handler->pickupData = pickupData;

	collider->handlers[COLLISION_ENTER].Bind(&CoinHandler::OnCollisionEnter, handler);

	return transform;
}
