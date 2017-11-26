#include "LOGinstruments.hpp"
#include <random>
#include <sys/time.h>

#define POSITIVE_VOLTAGE 1.0
#define NEGATIVE_VOLTAGE -1.0

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

/*
long int GetUsTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_usec;
}
*/

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

	velvet() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, 0) {};
	void step() override;
};

void velvet::step() {

	float thres = 0.0;

	if (inputs[INPUT_RATE].active) {
		thres = inputs[INPUT_RATE].value;
	}
	thres = MIN(MAX(thres + params[RATE].value, 0.0),1.0);

	int mode = (int) params[ENERGY_MODE].value;
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
	float r1 = static_cast <float> (/*better?*/drand48()) / (float)RAND_MAX;
	float r2 = static_cast <float> (drand48()) / (float)RAND_MAX;
#endif


	if (outputs[VELVET_OUT].active) {
		if ( r1 <= thres ) {
			if ( r2 >= 0.5 ) {
				outputs[VELVET_OUT].value = weight * POSITIVE_VOLTAGE;
			} else {
				outputs[VELVET_OUT].value = weight * NEGATIVE_VOLTAGE;
			}
		} else {
			outputs[VELVET_OUT].value = 0.0;
		}
	}

	if (outputs[WHITE_OUT].active)
		outputs[WHITE_OUT].value = r1 * 10.0;

}


VelvetWidget::VelvetWidget() {
	velvet *module = new velvet();
	setModule(module);
	box.size = Vec(15*10, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/velvet-nofonts.svg")));
		addChild(panel);
	}

	addParam(createParam<Davies1900hBlackKnob>(Vec(55, 95), module, velvet::RATE, 0.0, 1.0, 0.5));
	addParam(createParam<CKSS>(Vec(66, 200), module, velvet::ENERGY_MODE, 0.0, 1.0, 1.0));

	addInput(createInput<PJ301MPort>(Vec(8, 156), module, velvet::INPUT_RATE));

	addOutput(createOutput<PJ3410Port>(Vec(30, 260), module, velvet::VELVET_OUT));
	addOutput(createOutput<PJ3410Port>(Vec(90, 260), module, velvet::WHITE_OUT));
}

