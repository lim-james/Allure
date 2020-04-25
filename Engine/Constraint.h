#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Transform.h"
#include "Camera.h"

#define WIDTH		0
#define HEIGHT		1	
#define CENTER_X	2	
#define CENTER_Y	3	
#define LEFT		4	
#define TOP			5	
#define RIGHT		6	
#define BOTTOM		7	
#define NA			8	

struct Constraint  {

	short self;
	Transform* item;
	short other;

	float multiplier, constant;

	Camera* view;

	Constraint(
		short const& self,
		Transform* const item,
		short const& other,
		float multiplier, 
		float constant,
		Camera* const view
	);

};


#endif
