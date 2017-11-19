#include <string>
#include <math.h>
#include <float.h>
#include "LOGinstruments.hpp"

struct LessMess : Module {
	enum ParamIds {
		NUM_PARAMS,
	};
	enum InputIds {
		INPUT1,
		INPUT2,
		INPUT3,
		INPUT4,
		INPUT5,
		INPUT6,
		INPUT7,
		INPUT8,
		INPUT9,
		NUM_INPUTS,
	};
	enum OutputIds {
		OUTPUT1,
		OUTPUT2,
		OUTPUT3,
		OUTPUT4,
		OUTPUT5,
		OUTPUT6,
		OUTPUT7,
		OUTPUT8,
		OUTPUT9,
		NUM_OUTPUTS,
	};

	LessMess(LessMessWidget * p);
	void step() override;

	LessMessWidget * parent;

	json_t *toJson() override {
		json_t *rootJ = json_object();
		if (parent) {
			for (int i = 0; i < NUM_INPUTS; i++) {
				json_object_set_new(rootJ, ("label" + std::to_string(i)).c_str(), json_string( parent->label[i]->text.c_str() ));
			}
			return rootJ;
		} return NULL;
	}

	void fromJson(json_t *rootJ) override {
		for (int i = 0; i < NUM_INPUTS; i++) {
			if (parent) {
				json_t *labJ = json_object_get(rootJ, ("label" + std::to_string(i)).c_str());
				if (labJ) {
					parent->label[i]->insertText( json_string_value(labJ) );
				}
			}
		}
	}

	void reset() {
		;
	}
};


LessMess::LessMess(LessMessWidget * p) {
	parent = p;
	params.resize(NUM_PARAMS);
	inputs.resize(NUM_INPUTS);
	outputs.resize(NUM_OUTPUTS);
}


void LessMess::step() {

	for (int i = 0; i < NUM_INPUTS; i++) {
		if (inputs[i].active)
			outputs[i].value = inputs[i].value;
	}
}

#define V_SEP 35
LessMessWidget::LessMessWidget() {
	LessMess *module = new LessMess(this);
	label = new TextField*[LessMess::NUM_INPUTS];

	setModule(module);

	box.size = Vec(15*16, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/LessMess_nofonts.svg")));
		addChild(panel);
	}

	for (int i = 0; i < LessMess::NUM_INPUTS; i++) {
		addInput(createInput<PJ301MPort>(Vec(10, 30 + i*V_SEP), module, i));

		label[i] = new TextField();
		/*label[i]->text = "cable " + std::to_string(i);
		label[i]->placeholder = "plc:";*/
		label[i]->box.pos = Vec(40, 32 + i * V_SEP);
		label[i]->box.size.x = box.size.x-75;
		addChild(label[i]);

		addOutput(createOutput<PJ301MPort>(Vec(box.size.x-30, 30 + i * V_SEP), module, i));
	}

}
