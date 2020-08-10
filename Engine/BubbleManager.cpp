#include "BubbleManager.h"

#include "InputEvents.h"
#include <Math/Math.hpp>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

BubbleManager::BubbleManager()
	: material(nullptr)
	, audioPrefab(nullptr)
	, source(nullptr)
	, controller(nullptr)
	, file(new AudioFile<int16_t>) {
}

void BubbleManager::FadeOut(Handler<void, void> const& completion) {
	if (controller) {
		controller->FadeOut(completion);
		controller = nullptr;
	} else {
		completion.Invoke();
	}
}

void BubbleManager::Play(SongData const& song) {
	t = bt = 0.f;
	current = song;

	file->Open(song.path);
	duration = file->GetDuration();
	bInv = Math::PI * static_cast<float>(song.bpm) / 60.f;

	{
		const unsigned entity = audioPrefab->Create()->entity;
		source = entities->GetComponent<AudioSource>(entity);
		controller = entities->GetComponent<AudioController>(entity);
		controller->FadeIn(song.path);
	}
}

void BubbleManager::Start() {
	t = bt = 0.f;
}

void BubbleManager::Update() {
	if (!source || source->IsPaused()) return;

	const float dt = time->dt * source->speed;
	t += dt, bt += dt;

	vec3f rotation = transform->GetLocalRotation();
	rotation.z += dt * rotationSpeed;
	transform->SetLocalRotation(rotation);

	material->multiplier = multiplier * source->volume;

	const float r = pow(1.f - fabs(sin(bt * bInv)), 3.f);
	material->minRadius = minRadius + (maxRadius - minRadius) * r;

	while (t > duration) t -= duration;
	material->values = file->Spectrum(t, audioDuration * 0.001f, startFrequency, endFrequency);
}

void BubbleManager::OnDestroy() {
	delete file;
}
