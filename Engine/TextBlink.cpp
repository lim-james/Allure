#include "TextBlink.h"

void TextBlink::Awake() {
	text = GetComponent<Text>();
}

void TextBlink::Start() {
	bt = duration;
}

void TextBlink::Update() {
	bt -= time->dt;
	if (bt <= 0.f) {
		text->color.a = text->color.a < 0.5f ? 1.f : 0.f;
		bt = duration + bt;
	}
}
