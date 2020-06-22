#ifndef ANIMATION_H
#define ANIMATION_H

#include "Component.h"

#include <Handler/Handler.h>

#include <vector>
#include <map>

class AnimationBase {

protected:

	bool loop;
	float duration;
	float delay;

	Handler<void, void> completion;

	float et;

public:

	AnimationBase(
		bool const& loop = false,
		float const& duration = 0.f,
		float const& delay = 0.f,
		Handler<void, void> completion = nullptr);

	virtual ~AnimationBase() {}

	virtual void Update(float const& dt);
	const bool IsActive() const;

	float const& GetDuration() const;
	float const& GetDelay() const;

};

template<typename T>
class AnimationData : public AnimationBase {

private:

	T* target;
	T outcome;
	T original;

public:

	AnimationData() {}
	~AnimationData() override {}

	AnimationData(AnimationBase base, T* target, T const& outcome)
		: AnimationBase(base)
		, target(target)
		, outcome(outcome)
		, original(*target) {}


	void Update(float const& dt) override {
		AnimationBase::Update(dt);

		if (et >= duration) {
			if (loop) {
				*target = original;
				et = 0.f;
			} else {
				*target = outcome;
				completion();
			}
		} else if (et > 0.f) {
			const T interval = outcome - *(target);
			const float ratio = dt / (duration - et);
			*(target) += interval * ratio;
		}
	}
};

struct Animation : Component {

	using base_type = Animation;

	std::map<int, std::vector<AnimationBase*>> animations;

	Animation();
	~Animation() override;

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

	template<typename T>
	void Queue(AnimationBase const& base, T* target, T const& outcome) {
		const int t = (int)target;
		animations[t].push_back(new AnimationData<T>(base, target, outcome));
	}

	void Clear();

};

#endif