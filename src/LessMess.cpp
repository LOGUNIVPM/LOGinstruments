#include <string>
#include <math.h>
#include <float.h>
#include "LOGinstruments.hpp"
#include "LOGgui.hpp"

#define NUM_ROWS (9)

struct LessMessWidget;

struct LessMess : Module {
	enum ParamIds {
		NUM_PARAMS,
	};
	enum InputIds {

		NUM_INPUTS = NUM_ROWS,
	};
	enum OutputIds {

		NUM_OUTPUTS = NUM_ROWS,
	};

	enum LightIds {
		NUM_LIGHTS
	};

	std::string labels[NUM_ROWS];
	LessMessWidget *widget;

	LessMess() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}

	void process(const ProcessArgs &args) override;
	json_t *dataToJson() override;
	void dataFromJson(json_t*) override;

};

void LessMess::process(const ProcessArgs &args) {

	for (int i = 0; i < NUM_INPUTS; i++) {
		if (inputs[i].isConnected())
			outputs[i].setVoltage(inputs[i].getVoltage());
	}
}

#define V_SEP 35
struct LessMessWidget : ModuleWidget {
	TextField ** txtField;
	LessMessWidget(LessMess *module);
};

LessMessWidget::LessMessWidget(LessMess *module) {

	setModule(module);
	if (module) {
		module->widget = this;
	}
	
	txtField = new TextField*[LessMess::NUM_INPUTS];
	setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LessMess_nofonts.svg")));
	box.size = Vec(15*16, 380);

	for (int i = 0; i < LessMess::NUM_INPUTS; i++) {
		addInput(createInput<logPortIn>(Vec(10, 40 + i*V_SEP), module, i));

		txtField[i] = new TextField();
		txtField[i]->box.pos = Vec(40, 42 + i * V_SEP);
		txtField[i]->box.size.x = box.size.x-75;
		if (module) {
			txtField[i]->setText(module->labels[i]);
		}
		addChild(txtField[i]);

		addOutput(createOutput<logPortOut>(Vec(box.size.x-30, 40 + i * V_SEP), module, i));
	}

}


json_t *LessMess::dataToJson() {

	json_t *rootJ = json_object();
	std::string tmps;

	for (int i = 0; i < NUM_ROWS; i++) {
		tmps = widget->txtField[i]->getText();
		json_object_set_new(rootJ, ("label" + std::to_string(i)).c_str(), json_string( tmps.c_str() ));
	}
	return rootJ;
}

void LessMess::dataFromJson(json_t *rootJ) {

	for (int i = 0; i < NUM_ROWS; i++) {
		json_t *labJ = json_object_get(rootJ, ("label" + std::to_string(i)).c_str());
		if (labJ) {
			labels[i] = json_string_value(labJ);
		}
	}
}

Model *modelLessMess = createModel<LessMess, LessMessWidget>("LessMess");
