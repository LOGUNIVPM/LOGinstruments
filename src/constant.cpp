#include "Leonardo.hpp"


struct constant : Module {
	enum ParamIds {
		VALUE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		VOID_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		MAIN_OUTPUT,
		NUM_OUTPUTS
	};

	float lights[1] = {};


	constant();
	void step();
};

constant::constant() {
	params.resize(NUM_PARAMS);
	inputs.resize(NUM_INPUTS);
	outputs.resize(NUM_OUTPUTS);
}

void constant::step() {

	// Set output
	if (outputs[MAIN_OUTPUT].active)
		outputs[MAIN_OUTPUT].value = 1.0 * params[VALUE_PARAM].value;

	lights[0] = rescalef(params[VALUE_PARAM].value, -48.0, 48.0, -1.0, 1.0);
}


constantWidget::constantWidget() {
	constant *module = new constant();
	setModule(module);
	box.size = Vec(15*10, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/VCO.svg")));
		addChild(panel);
	}

	addChild(createScrew<ScrewSilver>(Vec(15, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x-30, 0)));
	addChild(createScrew<ScrewSilver>(Vec(15, 365)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x-30, 365)));

	addParam(createParam<Davies1900hLargeBlackKnob>(Vec(48, 61), module, constant::VALUE_PARAM, -54.0, 54.0, 0.0));

	addInput(createInput<PJ301MPort>(Vec(11, 276), module, constant::VOID_INPUT));

	addOutput(createOutput<PJ301MPort>(Vec(11, 320), module, constant::MAIN_OUTPUT));

	addChild(createValueLight<SmallLight<GreenRedPolarityLight>>(Vec(99, 41), &module->lights[0]));
}

