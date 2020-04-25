#ifndef LAYOUT_SYSTEM_H
#define LAYOUT_SYSTEM_H

#include "System.h"	
#include "Layout.h"

#include <vector>
#include <map>
#include <functional>

class LayoutSystem : public System {

	static std::vector<std::function<float(Constraint const&)>> getters;
	static std::vector<std::function<void(Transform* const, float const&, bool const&)>> setters;
	std::vector<Layout*> components;

public:

	void Initialize() override;
	void Update(float const& dt) override;

private:

	void ActiveHandler(Events::Event* event);

	// getters
	static float GetValue(Constraint const& constraint);
	// individual getters
	static float GetWidth(Constraint const& constraint);
	static float GetHeight(Constraint const& constraint);
	static float GetCenterX(Constraint const& constraint);
	static float GetCenterY(Constraint const& constraint);
	static float GetLeft(Constraint const& constraint);
	static float GetTop(Constraint const& constraint);
	static float GetRight(Constraint const& constraint);
	static float GetBottom(Constraint const& constraint);
	static float GetNA(Constraint const& constraint);

	// setters
	static void SetValue(Transform* const transform, short const& attribute, float const& value, bool const& warp);
	// individual getters
	static void SetWidth(Transform* const transform, float const& value, bool const& warp);
	static void SetHeight(Transform* const transform, float const& value, bool const& warp);
	static void SetCenterX(Transform* const transform, float const& value, bool const& warp);
	static void SetCenterY(Transform* const transform, float const& value, bool const& warp);
	static void SetLeft(Transform* const transform, float const& value, bool const& warp);
	static void SetTop(Transform* const transform, float const& value, bool const& warp);
	static void SetRight(Transform* const transform, float const& value, bool const& warp);
	static void SetBottom(Transform* const transform, float const& value, bool const& warp);

};

#endif
