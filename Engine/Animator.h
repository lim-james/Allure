#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Component.h"

#include "AnimationBase.h"

#include <vector>
#include <map>

struct Animator : Component {

	using base_type = Animator;

	std::map<int, std::vector<AnimationBase*>> animations;

	~Animator() override;

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

	template<typename T>
	void Queue(AnimationBase const& base, T* target, T const& outcome) {
		animations[(int)target].push_back(new AnimationData<T>(base, target, outcome));
	}

	template<typename T>
	unsigned  Count(T* target) const {
		return animations[(int)target].size();
	}

	template<typename T>
	void Pop(T* target) {
		animations[(int)target].pop_back();
	}

	template<typename T>
	void Clear(T* target) {
		animations[(int)target].clear();
	}
	
	void Clear();

};

#endif