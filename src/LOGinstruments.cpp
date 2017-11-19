#include "LOGinstruments.hpp"


Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	plugin->slug = "LOGinstruments";
#ifdef LOGINSTRUVERSION
	p->version = TOSTRING(LOGINSTRUVERSION);
#endif

	p->addModel(createModel<constantWidget>("LOGinstruments", "LOGinstruments", "Constant", "Constant"));
	p->addModel(createModel<SpeckWidget>("LOGinstruments", "LOGinstruments", "Speck", "Speck"));
	p->addModel(createModel<BritixWidget>("LOGinstruments", "LOGinstruments", "Britix", "Britix"));
	/*p->addModel(createModel<BrexitWidget>("LOGinstruments", "LOGinstruments", "Brexit", "Brexit"));*/
	p->addModel(createModel<CompaWidget>("LOGinstruments", "LOGinstruments", "Compa", "Compa"));
	p->addModel(createModel<LessMessWidget>("LOGinstruments", "LOGinstruments", "LessMess", "LessMess"));
	/*p->addModel(createModel<DPWOscWidget>("LOGinstruments", "LOGinstruments", "DPWOsc", "DPWOsc"));*/
	p->addModel(createModel<VelvetWidget>("LOGinstruments", "LOGinstruments", "Velvet", "Velvet"));
}
