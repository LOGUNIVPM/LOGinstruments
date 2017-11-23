#include "LOGinstruments.hpp"


Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	plugin->slug = "LOGinstruments";
#ifdef LOGINSTRUVERSION
	p->version = TOSTRING(LOGINSTRUVERSION);
#endif

	p->addModel(createModel<constantWidget>("LOGinstruments", "Constant", "DC Offset Gen", UTILITY_TAG));
	p->addModel(createModel<SpeckWidget>("LOGinstruments", "Speck", "Spectrum Analyzer", VISUAL_TAG, UTILITY_TAG));
	p->addModel(createModel<BritixWidget>("LOGinstruments", "Britix", "Matrix Modulator", LOGIC_TAG, MIXER_TAG, UTILITY_TAG));
	p->addModel(createModel<CompaWidget>("LOGinstruments", "Compa", "Comparator", DIGITAL_TAG, QUANTIZER_TAG));
	p->addModel(createModel<LessMessWidget>("LOGinstruments", "LessMess", "Tidy Up Cables", UTILITY_TAG, VISUAL_TAG));
	p->addModel(createModel<VelvetWidget>("LOGinstruments", "Velvet", "Velvet Noise Gen", NOISE_TAG, RANDOM_TAG));
}
