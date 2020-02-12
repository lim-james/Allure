#include "EditorScene.h"

#include "Transform.h"
#include "Render.h"

#include "ButtonSystem.h"
#include "AnimationSystem.h"
#include "ParticleSystem.h"

#include "LoadTGA.h"
#include "LoadFNT.h"
#include "LuaScript.h"
#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <Helpers/FileHelpers.h>
#include <GLFW/glfw3.h>

void EditorScene::Awake() {

	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<AnimationSystem>(2);
	systems->Subscribe<ParticleSystem>(3);

	Scene::Awake();

	entities->GetComponent<Camera>(mainCamera)->SetSize(10.0f);

	const unsigned entity = entities->Create();
	emitter = entities->AddComponent<ParticleEmitter>(entity);
	emitter->SetActive(true);

	// fonts
	auto microsoft = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");
	auto courierNew = Load::FNT("Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga");

	// title
	{
		const unsigned label = entities->Create();

		auto transform = entities->GetComponent<Transform>(label);
		transform->translation.y = 7.75;

		titleText = entities->AddComponent<Text>(label);
		titleText->SetActive(true);
		titleText->SetFont(microsoft);
		titleText->text = "Allure 2D"; 
		titleText->scale = 0.5f;
		titleText->color.Set(1.f, 1.f, 1.f, 0.0f);

		auto animation = entities->AddComponent<Animation>(label);
		animation->SetActive(true);
	}

	{
		const unsigned entity = entities->Create();

		auto transform = entities->GetComponent<Transform>(entity);
		transform->translation.y = -7.25f;
		transform->scale.Set(7.f, 1.5f, 0.f);

		buttonRender = entities->AddComponent<Render>(entity);
		buttonRender->SetActive(true);
		buttonRender->tint.Set(1.f, 1.f, 1.f, 0.f);

		auto text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(microsoft);
		text->text = "Close editor";
		text->scale = 0.5f;
		text->color.Set(0.f, 0.f, 0.f, 1.f);

		auto button = entities->AddComponent<Button>(entity);
		button->BindHandler(MOUSE_OVER, &EditorScene::OnMouseOver, this);
		button->BindHandler(MOUSE_OUT, &EditorScene::OnMouseOut, this);
		button->BindHandler(MOUSE_DOWN, &EditorScene::OnMouseDown, this);
		button->BindHandler(MOUSE_UP, &EditorScene::OnMouseUp, this);
		button->BindHandler(MOUSE_CLICK, &EditorScene::OnClick, this);

		auto animation = entities->AddComponent<Animation>(entity);
		animation->SetActive(true);
	}

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &EditorScene::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("DROP_INPUT", &EditorScene::DropHandler, this);
}

void EditorScene::Start() {
	emitter->SetActive(true);

	{
		auto transform = entities->GetComponent<Transform>(titleText->entity);
		auto animation = entities->GetComponent<Animation>(titleText->entity);

		animation->Animate(
			AnimationBase(false, 0.5f, 1.f),
			titleText->color.a,
			1.f
		);

		animation->Animate(
			AnimationBase(false, 0.5f, 1.f),
			transform->translation.y,
			8.f
		);
	}

	{
		auto button = entities->GetComponent<Button>(buttonRender->entity);
		auto transform = entities->GetComponent<Transform>(buttonRender->entity);
		auto animation = entities->GetComponent<Animation>(buttonRender->entity);

		animation->Animate(
			AnimationBase(false, 0.5f, 2.f, [button]() {
				button->SetActive(true);
			}),
			buttonRender->tint.a,
			0.5f
		);

		animation->Animate(
			AnimationBase(false, 0.5f, 2.f),
			transform->translation.y,
			-7.f
		);
	}
}

void EditorScene::FixedUpdate(const float & dt) {
	auto info = Helpers::GetFileInfo(filepath);
	if (lastMod != info.st_mtime) {
		lastMod = info.st_mtime;
		UpdateEmitter();
	}

	if (!destination.empty()) {
		transitionDelay -= dt;
		if (transitionDelay <= 0.f) {
			Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::String(destination));
			destination = "";
			transitionDelay = 0.0f;
		}
	}
}

void EditorScene::SetFile(const std::string & _filepath) {
	filepath = _filepath;
	lastMod = Helpers::GetFileInfo(filepath).st_mtime;
	titleText->text = filepath;
	UpdateEmitter();
}

void EditorScene::KeyHandler(Events::Event * event) {
	auto input = static_cast<Events::KeyInput*>(event);
	if (input->action == GLFW_PRESS && input->key == GLFW_KEY_SPACE) {
		LuaScript* script = new LuaScript(filepath);

		script->Set("emitter.accelRad", 10.f, nullptr);

		delete script;
	}
}

void EditorScene::DropHandler(Events::Event * event) {
	auto drop = static_cast<Events::DropInput*>(event);
	SetFile(drop->paths[0]);
}

void EditorScene::OnMouseOver(unsigned target) {
	auto render = entities->GetComponent<Render>(target);
	auto animation = entities->GetComponent<Animation>(target);

	animation->Animate(
		AnimationBase(false, 0.2f),
		render->tint.a,
		1.f
	);
}

void EditorScene::OnMouseOut(unsigned target) {
	auto render = entities->GetComponent<Render>(target);
	auto animation = entities->GetComponent<Animation>(target);

	animation->Animate(
		AnimationBase(false, 0.2f),
		render->tint.a,
		0.5f
	);
}

void EditorScene::OnMouseDown(unsigned target) {
	auto transform = entities->GetComponent<Transform>(target);
	auto animation = entities->GetComponent<Animation>(target);

	animation->Animate(
		AnimationBase(false, 0.2f),
		transform->scale,
		vec3f(7.f, 1.5f, 0.f) * 0.9f
	);
}

void EditorScene::OnMouseUp(unsigned target) {
	auto transform = entities->GetComponent<Transform>(target);
	auto animation = entities->GetComponent<Animation>(target);

	animation->Animate(
		AnimationBase(false, 0.2f),
		transform->scale,
		vec3f(7.f, 1.5f, 0.f)
	);
}

void EditorScene::OnClick(unsigned target) {
	FadeOut();
	destination = "TITLE";
	transitionDelay = emitter->lifetime + emitter->lifetimeRange;
}


void EditorScene::UpdateEmitter() {
	LuaScript* script = new LuaScript(filepath);

	emitter->age = script->Get<float>("emitter.age");
	emitter->duration = script->Get<float>("emitter.duration");

	emitter->spawnInterval = script->Get<float>("emitter.spawnInterval");
	emitter->spawnTimer = script->Get<float>("emitter.spawnTimer");
	emitter->burstAmount = script->Get<int>("emitter.burstAmount");

	emitter->loop = script->Get<bool>("emitter.loop");
	emitter->lifetime = script->Get<float>("emitter.lifetime");
	emitter->lifetimeRange = script->Get<float>("emitter.lifetimeRange");

	auto angle = script->GetVector<float>("emitter.angle");
	emitter->angle.Set(angle[0], angle[1], angle[2]);
	auto angleRange = script->GetVector<float>("emitter.angleRange");
	emitter->angleRange.Set(angleRange[0], angleRange[1], angleRange[2]);

	emitter->speed = script->Get<float>("emitter.speed");
	emitter->speedRange = script->Get<float>("emitter.speedRange");

	auto offset = script->GetVector<float>("emitter.offset");
	emitter->offset.Set(offset[0], offset[1], offset[2]);
	auto positionRange = script->GetVector<float>("emitter.positionRange");
	emitter->positionRange.Set(positionRange[0], positionRange[1], positionRange[2]);

	auto gravity = script->GetVector<float>("emitter.gravity");
	emitter->gravity.Set(gravity[0], gravity[1], gravity[2]);

	emitter->accelRad = script->Get<float>("emitter.accelRad");
	emitter->accelRadRange = script->Get<float>("emitter.accelRadRange");

	auto startSize = script->GetVector<float>("emitter.startSize");
	emitter->startSize.Set(startSize[0], startSize[1], startSize[2]);
	auto startSizeRange = script->GetVector<float>("emitter.startSizeRange");
	emitter->startSizeRange.Set(startSizeRange[0], startSizeRange[1], startSizeRange[2]);

	auto endSize = script->GetVector<float>("emitter.endSize");
	emitter->endSize.Set(endSize[0], endSize[1], endSize[2]);
	auto endSizeRange = script->GetVector<float>("emitter.endSizeRange");
	emitter->endSizeRange.Set(endSizeRange[0], endSizeRange[1], endSizeRange[2]);

	emitter->texture = Load::TGA(script->Get<std::string>("emitter.texture"));

	auto startColor = script->GetVector<float>("emitter.startColor");
	emitter->startColor.Set(startColor[0], startColor[1], startColor[2], startColor[3]);
	auto startColorRange = script->GetVector<float>("emitter.startColorRange");
	emitter->startColorRange.Set(startColorRange[0], startColorRange[1], startColorRange[2], startColorRange[3]);

	auto endColor = script->GetVector<float>("emitter.endColor");
	emitter->endColor.Set(endColor[0], endColor[1], endColor[2], endColor[3]);
	auto endColorRange = script->GetVector<float>("emitter.endColorRange");
	emitter->endColorRange.Set(endColorRange[0], endColorRange[1], endColorRange[2], endColorRange[3]);

	delete script;
}

void EditorScene::FadeOut() {
	entities->GetComponent<Button>(buttonRender->entity)->SetActive(false);
	emitter->SetActive(false);

	{
		auto transform = entities->GetComponent<Transform>(titleText->entity);
		auto animation = entities->GetComponent<Animation>(titleText->entity);

		animation->Animate(
			AnimationBase(false, 0.5f),
			titleText->color.a,
			0.f
		);

		animation->Animate(
			AnimationBase(false, 0.5f),
			transform->translation.y,
			7.75f
		);
	}

	{
		auto transform = entities->GetComponent<Transform>(buttonRender->entity);
		auto animation = entities->GetComponent<Animation>(buttonRender->entity);

		animation->Animate(
			AnimationBase(false, 0.5f),
			buttonRender->tint.a,
			0.f
		);

		animation->Animate(
			AnimationBase(false, 0.5f),
			transform->translation.y,
			-7.25f
		);
	}
}
