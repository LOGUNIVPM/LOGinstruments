#include "LOGinstruments.hpp"
#include "LOGgui.hpp"
#include <random>
#include <sys/time.h>

#define POSITIVE_VOLTAGE 1.0
#define NEGATIVE_VOLTAGE -1.0


struct velvet: Module {
	enum ParamIds {
		RATE,
		ENERGY_MODE,
		NUM_PARAMS
	};

	enum InputIds {
		INPUT_RATE,
		NUM_INPUTS
	};

	enum OutputIds {
		VELVET_OUT,
		WHITE_OUT,
		NUM_OUTPUTS
	};

	enum LightIds {
		NUM_LIGHTS
	};

	velvet() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(RATE, 0.0, 1.0, 0.5);
		configParam(ENERGY_MODE, 0.0, 1.0, 1.0);
	};
	void process(const ProcessArgs &args) override;
};

void velvet::process(const ProcessArgs &args) {

	float thres = 0.0;

	if (inputs[INPUT_RATE].isConnected()) {
		thres = inputs[INPUT_RATE].getVoltage();
	}
	thres = clamp(thres + params[RATE].getValue(), 0.0, 1.0);

	int mode = (int) params[ENERGY_MODE].getValue();
	float weight = 1;
	if (mode == 1) {
		weight = (1 - thres) + 0.5;
		weight *= weight * weight;
	}
	weight *= 10.0;

	thres *= thres * thres; // cheap psychoacoustic curve


#ifdef MERSENNE_METHOD // approximately 3-4 times slower
	std::random_device rd;
	std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(0.0, 1.0);

	float r1 = dis(gen);
	float r2 = dis(gen);
#else
	float r1 = (random::uniform()) ;
	float r2 = (random::uniform()) ;
#endif


	if (outputs[VELVET_OUT].isConnected()) {
		if ( r1 <= thres ) {
			if ( r2 >= 0.5 ) {
				outputs[VELVET_OUT].setVoltage(weight * POSITIVE_VOLTAGE);
			} else {
				outputs[VELVET_OUT].setVoltage(weight * NEGATIVE_VOLTAGE);
			}
		} else {
			outputs[VELVET_OUT].setVoltage(0.0);
		}
	}

	if (outputs[WHITE_OUT].isConnected())
		outputs[WHITE_OUT].setVoltage((r1 -0.5) * 10.0);

}


struct VelvetWidget : ModuleWidget {
	VelvetWidget(velvet *module);
};

VelvetWidget::VelvetWidget(velvet *module) {

	setModule(module);
	setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/velvet-nofonts.svg")));


	addParam(createParam<Davies1900hBlackKnob>(Vec(40, 100), module, velvet::RATE));
	addParam(createParam<CKSS>(Vec(54, 240), module, velvet::ENERGY_MODE));

	addInput(createInput<logPortIn>(Vec(6, 164), module, velvet::INPUT_RATE));

	addOutput(createOutput<logPortOut>(Vec(5, 326), module, velvet::VELVET_OUT));
	addOutput(createOutput<logPortOut>(Vec(93, 326), module, velvet::WHITE_OUT));
}


Model *modelVelvet = createModel<velvet, VelvetWidget>("Velvet");
