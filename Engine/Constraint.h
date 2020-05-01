#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Transform.h"
#include "Camera.h"

#define WIDTH		0
#define HEIGHT		1	
#define CENTER_X_ANCHOR	2	
#define CENTER_Y_ANCHOR	3	
#define LEFT_ANCHOR		4	
#define TOP_ANCHOR		5	
#define RIGHT_ANCHOR	6	
#define BOTTOM_ANCHOR	7	
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
