#include "Scene.h"

#include "TransformSystem.h"
#include "RenderSystem.h"

#include "LoadFNT.h"

#include <Math/Vectors.h>
#include <Events/EventsManager.h>

#include <fstream>

Scene::Scene() {
	entities = new EntityManager;
	systems = new SystemManager(entities);
	scripts = new ScriptSystem(entities);
}

Scene::Scene(const std::string & filepath) : filepath(filepath) {
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
	entities->GetComponent<Transform>(mainCamera)->translation.z = 1.f;
	Camera * camera = entities->AddComponent<Camera>(mainCamera);
	camera->SetActive(true);
	camera->clearColor = vec4f(0.f);

	if (!filepath.empty()) Load(filepath);
}

void Scene::Enter() {
	Events::EventsManager::GetInstance()->SubscribeContext(this);
	systems->Start();
	scripts->Start();
}

void Scene::FixedUpdate(const float& dt) {
	scripts->FixedUpdate();
}

void Scene::Update(const float& dt) {
	systems->Update(dt);
	scripts->Update(dt);
}

void Scene::Exit() {
	scripts->Stop();
	systems->Stop();
	Events::EventsManager::GetInstance()->UnsubscribeContext(this);
}

void Scene::Destroy() { 
	scripts->Destroy();
}

void Scene::PrepareForSegue(Scene * destination) { }

void Scene::Load(const std::string & filepath) {
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
				auto text = entities->AddComponent<Text>(entity);
				auto courierNew = Load::FNT("Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga");
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
