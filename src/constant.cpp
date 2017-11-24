#include "LOGinstruments.hpp"


struct constant : Module {
	enum ParamIds {
		VALUE_PARAM_1,
		VALUE_PARAM_10,
		VALUE_PARAM_100,
		NUM_PARAMS
	};
	enum InputIds {
		NUM_INPUTS
	};
	enum OutputIds {
		OUT_1,
		OUT_10,
		OUT_100,
		NUM_OUTPUTS
	};

	constant() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, 0) {}
	void step() override;
};

void constant::step() {

	// Set output
	if (outputs[OUT_1].active)
		outputs[OUT_1].value = params[VALUE_PARAM_1].value;
	if (outputs[OUT_10].active)
		outputs[OUT_10].value = params[VALUE_PARAM_10].value;
	if (outputs[OUT_100].active)
		outputs[OUT_100].value = params[VALUE_PARAM_100].value;

}


constantWidget::constantWidget() {
	constant *module = new constant();
	setModule(module);
	box.size = Vec(15*10, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/const-nofonts.svg")));
		addChild(panel);
	}

	addParam(createParam<Davies1900hLargeBlackKnob>(Vec(48, 60), module, constant::VALUE_PARAM_1, -1.0, 1.0, 0.0));
	addParam(createParam<Davies1900hLargeBlackKnob>(Vec(48, 130), module, constant::VALUE_PARAM_10, -10.0, 10.0, 0.0));
	addParam(createParam<Davies1900hLargeBlackKnob>(Vec(48, 200), module, constant::VALUE_PARAM_100, -100.0, 100.0, 0.0));

	addOutput(createOutput<PJ301MPort>(Vec(110, 70), module, constant::OUT_1));
	addOutput(createOutput<PJ301MPort>(Vec(110, 140), module, constant::OUT_10));
	addOutput(createOutput<PJ301MPort>(Vec(110, 210), module, constant::OUT_100));
}

