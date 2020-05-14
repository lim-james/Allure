#include "Scene.h"

// systems
#include "TransformSystem.h"
#include "RenderSystem.h"
// components
#include "Text.h"
// Utils
#include "LoadFNT.h"

#include <Math/Vectors.h>
#include <Events/EventsManager.h>

#include <fstream>

Scene::Scene() {
	entities = new EntityManager;
	systems = new SystemManager(entities);
	scripts = new ScriptSystem(entities);
}

Scene::Scene(std::string const& filepath) : filepath(filepath) {
	entities = new EntityManager;
	systems = new SystemManager(entities);
	scripts = new ScriptSystem(entities);
}

Scene::~Scene() {
	delete entities;
	delete systems;
	delete scripts;
}

void Scene::Awake() {
	systems->Subscribe<TransformSystem>(0);
	systems->Subscribe<RenderSystem>(0);

	scripts->Awake();
}

void Scene::Create() {
	mainCamera = entities->Create();
	entities->GetComponent<Transform>(mainCamera)->translation.z = 10.f;
	Camera * camera = entities->AddComponent<Camera>(mainCamera);
	camera->SetActive(true);
	camera->clearColor = 0.f;

	if (!filepath.empty()) Load(filepath);
}

void Scene::Enter() {
	EventsManager::Get()->SubscribeContext(this);
	systems->Start();
	scripts->Start();
}

void Scene::FixedUpdate(float const& dt) {
	scripts->FixedUpdate(dt);
	systems->FixedUpdate(dt);
}

void Scene::Update(float const& dt) {
	scripts->Update(dt);
	systems->Update(dt);
}

void Scene::Exit() {
	scripts->Stop();
	systems->Stop();
	EventsManager::Get()->UnsubscribeContext(this);
}

void Scene::Destroy() { 
	scripts->Destroy();
}

void Scene::PrepareForSegue(Scene * destination) { }

void Scene::Load(std::string const& filepath) {
	std::ifstream ifs(filepath);

	if (!ifs.is_open()) {
		Debug::Error << "Scene file \"" << filepath << "\" not found.\n";
		return;
	}

	std::string result = "";
	std::string buffer;

	while (!ifs.eof()) {
		std::getline(ifs, buffer);

		if (buffer == ">>>") {
			const AENotation notation(result);
			const unsigned entity = notation.Get<unsigned>("entity");

			while (entity >= entities->PoolCount()) {
				entities->Create();
			}
 
			AEObject* object = nullptr;

			if (notation.Get("type") == "Transform") {
				object = static_cast<AEObject*>(entities->AddComponent<Transform>(entity));
			} else if (notation.Get("type") == "Text") {
				Text* const text = entities->AddComponent<Text>(entity);
				Font* const courierNew = Load::FNT("Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga");
				text->SetFont(courierNew);
				object = static_cast<AEObject*>(text);
			} 

			object->Parse(notation);

			result = "";
		} else {
			result += buffer + '\n';
		}
	}
}
