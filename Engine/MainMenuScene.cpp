#include "MainMenuScene.h"

// systems
#include "RenderSystem.h"
#include "ButtonSystem.h"
#include "AnimationSystem.h"

#include "LoadFNT.h"
#include "InputEvents.h"

#include <Math/Random.hpp>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void MainMenuScene::Awake()
{
	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<AnimationSystem>(2);

	Scene::Awake();

	//Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &MainMenuScene::CursorPositionHandler, this);
	//Events::EventsManager::GetInstance()->Subscribe("MOUSE_BUTTON_INPUT", &MainMenuScene::MouseButtonHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &MainMenuScene::KeyDownHander, this);

	camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetActive(true);
	camera->clearColor.Set(0.f);
	entities->GetComponent<Camera>(mainCamera)->SetSize(10.f);
	


	//{
	//	const unsigned label = entities->Create();
	//	entities->GetComponent<Transform>(label)->translation.Set(-17.f, -7.5f, 0.f);
	//	const auto text = entities->AddComponent<Text>(label);
	//	text->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	//	text->SetActive(true);
	//	text->paragraphAlignment = PARAGRAPH_LEFT;
	//	text->scale = 0.5f;
	//	text->text = "[Enter] to toggle lines.\nDrag nodes around.\nClick two nodes to connect them.\nClick on an empty area to create a node.\nRight click to select a node to path find from.";

	//}

	{
		const unsigned label = entities->Create();
		entities->GetComponent<Transform>(label)->scale.Set(7.f, 1.5f, 0.f);
		const auto button = entities->AddComponent<Button>(label); 
		const auto text = entities->AddComponent<Text>(label);
		text->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
		text->SetActive(true);
		text->scale = 0.5f;
		text->text = "Button";
		button->BindHandler(MOUSE_DOWN, &MainMenuScene::ButtonFunction, this);
	}

	{
		const unsigned label = entities->Create();
		entities->GetComponent<Transform>(label)->translation.Set(0.f, -2.f, 0.f);
		const auto render = entities->AddComponent<Render>(label);
		render->SetActive(true);
		render->tint.Set(1.f, 1.f, 1.f, 0.f);
		//render->SetCellRect(0.f, 0.f, 5.f, 2.f);
		const auto button = entities->AddComponent<Button>(label);
		const auto text = entities->AddComponent<Text>(label);
		text->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
		text->SetActive(true);
		text->scale = 0.5f;
		text->text = "Button";
		button->BindHandler(MOUSE_DOWN, &MainMenuScene::ButtonFunction, this);
	}

	
	//script = new LuaScript("Files/Scripts/States.lua");
}

void MainMenuScene::ButtonFunction(unsigned id) 
{
	Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::String("GAME"));
}

void MainMenuScene::Update(const float & dt)
{
	Scene::Update(dt);

	if (!mouseDown)
		mouseMoved = false;
}

void MainMenuScene::KeyDownHander(Events::Event* event)
{
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
}

//void MainMenuScene::CursorPositionHandler(Events::Event * event)
//{
//	Events::CursorPositionInput* input = static_cast<Events::CursorPositionInput*>(event);
//	mouseMoved = true;
//	mousePosition = input->position;
//
//	smPosition = camera->ScreenToWorldSpace(input->position);
//	smPosition.y = -smPosition.y;
//
//	if (mouseDown && selected) {
//		selected->translation = smPosition;
//		entityNodeMap[selected->entity]->position = smPosition;
//		if (start && end)
//			graph.PathFind(start, end);
//	}
//}
//
//void MainMenuScene::MouseButtonHandler(Events::Event * event)
//{
//	Events::MouseButtonInput* input = static_cast<Events::MouseButtonInput*>(event);
//	if (!mouseOver) {
//		if (input->button == GLFW_MOUSE_BUTTON_LEFT && input->action == GLFW_RELEASE) {
//			auto position = camera->ScreenToWorldSpace(mousePosition);
//			graph.CreateNode(position.x, -position.y, 0.f);
//			mouseOver = true;
//		}
//	}
//
//	if (input->button == GLFW_MOUSE_BUTTON_RIGHT && input->action == GLFW_RELEASE) {
//		if (start) {
//			if (end) {
//				entities->GetComponent<Render>(start->entity)->tint.Set(1.f);
//				entities->GetComponent<Render>(end->entity)->tint.Set(1.f);
//				start = end = nullptr;
//				graph.ClearPath();
//			}
//			else {
//				end = graph.GetNearestNode(smPosition);
//				entities->GetComponent<Render>(end->entity)->tint.Set(1.f, 1.f, 0.f, 1.f);
//				graph.PathFind(start, end);
//			}
//		}
//		else {
//			start = graph.GetNearestNode(smPosition);
//			entities->GetComponent<Render>(start->entity)->tint.Set(1.f, 1.f, 0.f, 1.f);
//		}
//	}
//}
//
//void MainMenuScene::OnMouseOverHandler(unsigned entity)
//{
//	entities->GetComponent<Animation>(entity)->Animate(
//		AnimationBase(false, 0.2f),
//		entities->GetComponent<Transform>(entity)->scale,
//		vec3f(1.25f)
//	);
//	mouseOver = true;
//}
//
//void MainMenuScene::OnMouseOutHandler(unsigned entity)
//{
//	entities->GetComponent<Animation>(entity)->Animate(
//		AnimationBase(false, 0.2f),
//		entities->GetComponent<Transform>(entity)->scale,
//		vec3f(1.f)
//	);
//	mouseOver = false;
//}
//
//void MainMenuScene::OnMouseDownHandler(unsigned entity)
//{
//	entities->GetComponent<Animation>(entity)->Animate(
//		AnimationBase(false, 0.2f),
//		entities->GetComponent<Transform>(entity)->scale,
//		vec3f(1.f)
//	);
//
//	mouseDown = true;
//
//	if (selected) {
//		graph.CreateEdge(entityNodeMap[selected->entity]->id, entityNodeMap[entity]->id);
//		selected = nullptr;
//	}
//	else {
//		selected = entities->GetComponent<Transform>(entity);
//	}
//}
//
//void MainMenuScene::OnMouseUpHandler(unsigned entity)
//{
//	entities->GetComponent<Animation>(entity)->Animate(
//		AnimationBase(false, 0.2f),
//		entities->GetComponent<Transform>(entity)->scale,
//		vec3f(1.f)
//	);
//
//	mouseDown = false;
//}
//
//void MainMenuScene::OnClick(unsigned entity)
//{
//	if (mouseMoved) {
//		selected = nullptr;
//	}
//	else {
//		if (selected) {
//		}
//	}
//}
