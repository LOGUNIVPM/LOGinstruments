#include "LOGinstruments.hpp"


Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	plugin->slug = "LOGinstruments";
#ifdef VERSION
	p->version = TOSTRING(VERSION);
#endif

	p->website = "https://github.com/leopard86/LOGinstruments";
	p->manual = "https://github.com/leopard86/LOGinstruments/blob/master/README.md";
	p->addModel(createModel<constantWidget>("LOGinstruments", "Constant", "DC Offset Gen", UTILITY_TAG));
	p->addModel(createModel<constant2Widget>("LOGinstruments", "Constant2", "Precise DC Gen", UTILITY_TAG));
	p->addModel(createModel<SpeckWidget>("LOGinstruments", "Speck", "Spectrum Analyzer", VISUAL_TAG, UTILITY_TAG));
	p->addModel(createModel<BritixWidget>("LOGinstruments", "Britix", "Matrix Modulator", LOGIC_TAG, MIXER_TAG, UTILITY_TAG));
	p->addModel(createModel<CompaWidget>("LOGinstruments", "Compa", "Comparator", DIGITAL_TAG, QUANTIZER_TAG));
	p->addModel(createModel<LessMessWidget>("LOGinstruments", "LessMess", "Tidy Up Cables", UTILITY_TAG, VISUAL_TAG));
	p->addModel(createModel<VelvetWidget>("LOGinstruments", "Velvet", "Velvet Noise Gen", NOISE_TAG, RANDOM_TAG));
}
