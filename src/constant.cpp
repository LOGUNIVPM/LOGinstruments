#include "LOGinstruments.hpp"
#include "LOGgui.hpp"

struct constant : Module {
	enum ParamIds {
		VALUE_PARAM_1,
		VALUE_PARAM_10,
		VALUE_PARAM_100,
		VALUE_PARAM_1B,
		VALUE_PARAM_10B,
		VALUE_PARAM_100B,
		NUM_PARAMS
	};
	enum InputIds {
		NUM_INPUTS
	};
	enum OutputIds {
		OUT_1,
		OUT_10,
		OUT_100,
		OUT_1B,
		OUT_10B,
		OUT_100B,
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
	if (outputs[OUT_1B].active)
		outputs[OUT_1B].value = params[VALUE_PARAM_1B].value;
	if (outputs[OUT_10B].active)
		outputs[OUT_10B].value = params[VALUE_PARAM_10B].value;
	if (outputs[OUT_100B].active)
		outputs[OUT_100B].value = params[VALUE_PARAM_100B].value;

}

struct constant2 : Module {
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

	constant2() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, 0) {}
	void step() override;
};

void constant2::step() {

	// Set output
	if (outputs[OUT_1].active)
		outputs[OUT_1].value = params[VALUE_PARAM_1].value;
	if (outputs[OUT_10].active)
		outputs[OUT_10].value = params[VALUE_PARAM_10].value;
	if (outputs[OUT_100].active)
		outputs[OUT_100].value = params[VALUE_PARAM_100].value;

}

constant2Widget::constant2Widget() {
	constant2 *module = new constant2();
	setModule(module);
	box.size = Vec(15*8, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/const2-nofonts.svg")));
		addChild(panel);
	}

	addParam(createParam<valueKnob>(Vec(40, 60), module, constant2::VALUE_PARAM_1, -1.0, 1.0, 0.0));
	addParam(createParam<valueKnob>(Vec(40, 150), module, constant2::VALUE_PARAM_10, -10.0, 10.0, 0.0));
	addParam(createParam<valueKnob>(Vec(40, 240), module, constant2::VALUE_PARAM_100, -100.0, 100.0, 0.0));

	addOutput(createOutput<PJ301MPort>(Vec(85, 65), module, constant2::OUT_1));
	addOutput(createOutput<PJ301MPort>(Vec(85, 155), module, constant2::OUT_10));
	addOutput(createOutput<PJ301MPort>(Vec(85, 245), module, constant2::OUT_100));
}

constantWidget::constantWidget() {
	constant *module = new constant();
	setModule(module);
	box.size = Vec(15*8, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/const-nofonts.svg")));
		addChild(panel);
	}

	addParam(createParam<RoundBlackKnob>(Vec(40, 60), module, constant::VALUE_PARAM_1, -1.0, 1.0, 0.0));
	addParam(createParam<RoundBlackKnob>(Vec(40, 100), module, constant::VALUE_PARAM_1B, -1.0, 1.0, 0.0));
	addParam(createParam<RoundBlackKnob>(Vec(40, 150), module, constant::VALUE_PARAM_10, -10.0, 10.0, 0.0));
	addParam(createParam<RoundBlackKnob>(Vec(40, 190), module, constant::VALUE_PARAM_10B, -10.0, 10.0, 0.0));
	addParam(createParam<RoundBlackKnob>(Vec(40, 240), module, constant::VALUE_PARAM_100, -100.0, 100.0, 0.0));
	addParam(createParam<RoundBlackKnob>(Vec(40, 280), module, constant::VALUE_PARAM_100B, -100.0, 100.0, 0.0));

	addOutput(createOutput<PJ301MPort>(Vec(85, 65), module, constant::OUT_1));
	addOutput(createOutput<PJ301MPort>(Vec(85, 105), module, constant::OUT_1B));
	addOutput(createOutput<PJ301MPort>(Vec(85, 155), module, constant::OUT_10));
	addOutput(createOutput<PJ301MPort>(Vec(85, 195), module, constant::OUT_10B));
	addOutput(createOutput<PJ301MPort>(Vec(85, 245), module, constant::OUT_100));
	addOutput(createOutput<PJ301MPort>(Vec(85, 285), module, constant::OUT_100B));
}

