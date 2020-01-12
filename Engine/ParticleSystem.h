#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "System.h"
#include "ParticleEmitter.h"
#include "Particle.h"

#include <Events/Event.h>
#include <vector>
#include <map>

class ParticleSystem : public System {

	std::map<ParticleEmitter*, std::vector<unsigned>> particleGroups;

public:

	void Initialize() override;
	void Update(const float& dt) override;

private:

	void EmitterActiveHandler(Events::Event* event);

	unsigned CreateParticle() const;


};

#endif