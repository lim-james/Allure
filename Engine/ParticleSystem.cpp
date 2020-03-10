#include "ParticleSystem.h"

#include "Transform.h"
#include "Render.h"

#include <Events/EventsManager.h>
#include <Math/Random.hpp>
#include <MACROS.h>

#include <thread>

void ParticleSystem::Initialize() {
	Events::EventsManager::GetInstance()->Subscribe("EMITTER_ACTIVE", &ParticleSystem::EmitterActiveHandler, this);
}

void ParticleSystem::Update(const float& dt) {
	for (auto& pair : particleGroups) {
		auto& emitter = pair.first;
		auto& group = pair.second;

		const auto& position = entities->GetComponent<Transform>(emitter->entity)->GetWorldTranslation();

		if (group.size() > 1000) {
			const unsigned threadCount = 8;
			const unsigned interval = group.size() / (threadCount + 1);
			std::vector<int> erase;

			std::thread threads[threadCount];

			for (unsigned t = 0; t < threadCount; ++t) {
				threads[t] = std::thread([t, interval, &group, emitter, dt, position, &erase, this]() {
					for (unsigned i = (t + 1) * interval; i < Math::Min((t + 2) * interval, group.size()); ++i) {
						const auto entity = group[i];
						auto particle = entities->GetComponent<Particle>(entity);
						auto transform = entities->GetComponent<Transform>(entity);
						auto render = entities->GetComponent<Render>(entity);

						transform->translation += particle->velocity * dt;
						particle->velocity += emitter->gravity * dt;

						const auto diff = transform->translation - position;
						const float length = Math::Length(diff);
						if (length > 0.f) {
							const float vAccelRad = Math::RandMinMax(-emitter->accelRadRange, emitter->accelRadRange);
							particle->velocity += (emitter->accelRad + vAccelRad) * dt * diff / length;
						}

						const float ratio = particle->age / particle->lifetime;

						const vec3f dSize = particle->endSize - particle->startSize;
						transform->scale = dSize * ratio + particle->startSize;

						const vec4f dColor = particle->endColor - particle->startColor;
						render->tint = dColor * ratio + particle->startColor;

						particle->age += dt;

						if (particle->age >= particle->lifetime) {
							erase.push_back(i);
						}
					}
				});
			}

			for (unsigned i = 0; i < interval; ++i) {
				const auto entity = group[i];
				auto particle = entities->GetComponent<Particle>(entity);
				auto transform = entities->GetComponent<Transform>(entity);
				auto render = entities->GetComponent<Render>(entity);

				transform->translation += particle->velocity * dt;
				particle->velocity += emitter->gravity * dt;

				const auto diff = transform->translation - position;
				const float length = Math::Length(diff);
				if (length > 0.f) {
					const float vAccelRad = Math::RandMinMax(-emitter->accelRadRange, emitter->accelRadRange);
					particle->velocity += (emitter->accelRad + vAccelRad) * dt * diff / length;
				}

				const float ratio = particle->age / particle->lifetime;

				const vec3f dSize = particle->endSize - particle->startSize;
				transform->scale = dSize * ratio + particle->startSize;

				const vec4f dColor = particle->endColor - particle->startColor;
				render->tint = dColor * ratio + particle->startColor;

				particle->age += dt;

				if (particle->age >= particle->lifetime) {
					erase.push_back(i);
				}
			}

			for (auto& t : threads) {
				t.join();
			}

			//std::sort(erase.begin(), erase.end());

			//std::cout << "Size " << erase.size() << '\n';
			for (int i = static_cast<int>(erase.size() - 1); i >= 0; --i) {
				entities->Destroy(group[i]);
				group.erase(group.begin() + i);
			}
		} else {
			auto end = static_cast<int>(group.size()) - 1;

			for (int i = end; i >= 0; --i) {
				const auto entity = group[i];
				auto particle = entities->GetComponent<Particle>(entity);
				auto transform = entities->GetComponent<Transform>(entity);
				auto render = entities->GetComponent<Render>(entity);

				transform->translation += particle->velocity * dt;
				particle->velocity += emitter->gravity * dt;

				const auto diff = transform->translation - position;
				const float length = Math::Length(diff);
				if (length > 0.f) {
					const float vAccelRad = Math::RandMinMax(-emitter->accelRadRange, emitter->accelRadRange);
					particle->velocity += (emitter->accelRad + vAccelRad) * dt * diff / length;
				}

				const float ratio = particle->age / particle->lifetime;

				const vec3f dSize = particle->endSize - particle->startSize;
				transform->scale = dSize * ratio + particle->startSize;

				const vec4f dColor = particle->endColor - particle->startColor;
				render->tint = dColor * ratio + particle->startColor;

				particle->age += dt;

				if (particle->age >= particle->lifetime) {
					entities->Destroy(entity);
					group.erase(group.begin() + i);
				}
			}
		}
		
		if (!emitter->IsActive()) continue;

		if (emitter->duration > 0) {
			if (emitter->age > emitter->duration) {
				if (emitter->loop && group.empty()) {
					emitter->age = 0.f;
				} else {
					if (group.empty()) {
						emitter->completion();
						emitter->completion.UnbindAll();
					}

					continue;
				}
			}

			emitter->age += dt;
		}

		emitter->spawnTimer -= dt;
		if (emitter->spawnTimer <= 0.f) {

			for (unsigned i = 0; i < emitter->burstAmount; ++i) {
				unsigned entity = CreateParticle();
				group.push_back(entity);

				auto particle = entities->GetComponent<Particle>(entity);
				auto transform = entities->GetComponent<Transform>(entity);
				auto render = entities->GetComponent<Render>(entity);

				particle->age = 0.f;
				particle->lifetime = emitter->lifetime + Math::RandMinMax(-emitter->lifetimeRange, emitter->lifetimeRange);

				transform->translation = position + emitter->offset + vec3f(
					Math::RandMinMax(-emitter->positionRange.x, emitter->positionRange.x),
					Math::RandMinMax(-emitter->positionRange.y, emitter->positionRange.y),
					Math::RandMinMax(-emitter->positionRange.z, emitter->positionRange.z)
				);

				const vec3f vAngle = Math::Rad(transform->rotation + emitter->angle + vec3f(
					Math::RandMinMax(-emitter->angleRange.x, emitter->angleRange.x),
					Math::RandMinMax(-emitter->angleRange.y, emitter->angleRange.y),
					Math::RandMinMax(-emitter->angleRange.z, emitter->angleRange.z)
				));

				const float vSpeed = Math::RandMinMax(-emitter->speedRange, emitter->speedRange);

				const vec3f worldUp(sin(vAngle.z), cos(vAngle.z), 0.f);

				// made for 2D
				particle->velocity = (emitter->speed + vSpeed) * worldUp;

				particle->startSize = emitter->startSize + vec3f(
					Math::RandMinMax(-emitter->startSizeRange.x, emitter->startSizeRange.x),
					Math::RandMinMax(-emitter->startSizeRange.y, emitter->startSizeRange.y),
					Math::RandMinMax(-emitter->startSizeRange.z, emitter->startSizeRange.z)
				);

				particle->endSize = emitter->endSize + vec3f(
					Math::RandMinMax(-emitter->endSizeRange.x, emitter->endSizeRange.x),
					Math::RandMinMax(-emitter->endSizeRange.y, emitter->endSizeRange.y),
					Math::RandMinMax(-emitter->endSizeRange.z, emitter->endSizeRange.z)
				);

				transform->scale = particle->startSize;

				// color
				particle->startColor = emitter->startColor + vec4f(
					Math::RandMinMax(-emitter->startColorRange.x, emitter->startColorRange.x),
					Math::RandMinMax(-emitter->startColorRange.y, emitter->startColorRange.y),
					Math::RandMinMax(-emitter->startColorRange.z, emitter->startColorRange.z),
					Math::RandMinMax(-emitter->startColorRange.w, emitter->startColorRange.w)
				);

				particle->endColor = emitter->endColor + vec4f(
					Math::RandMinMax(-emitter->endColorRange.x, emitter->endColorRange.x),
					Math::RandMinMax(-emitter->endColorRange.y, emitter->endColorRange.y),
					Math::RandMinMax(-emitter->endColorRange.z, emitter->endColorRange.z),
					Math::RandMinMax(-emitter->endColorRange.w, emitter->endColorRange.w)
				);

				render->tint = particle->startColor;
				render->SetTexture(emitter->texture);

				render->SetActive(true);
				particle->SetActive(true);
			}

			emitter->spawnTimer = emitter->spawnInterval;
		}
	}
}

void ParticleSystem::EmitterActiveHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<ParticleEmitter*>*>(event)->data;

	if (particleGroups.find(c) == particleGroups.end())
		particleGroups[c] = {};
}

unsigned ParticleSystem::CreateParticle() const {
	const unsigned id = entities->Create();
	entities->AddComponent<Render>(id); //->SetActive(true);
	entities->AddComponent<Particle>(id); //->SetActive(true);
	return id;
}
