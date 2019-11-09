#include "LOGinstruments.hpp"


Plugin *pluginInstance;

void init(rack::Plugin *p) {
	pluginInstance = p;
	p->addModel(modelSpeck);
	p->addModel(modelLessMess);
	p->addModel(modelVelvet);
	p->addModel(modelCrystal);
/*
	p->addModel(modelconstant);
	p->addModel(modelconstant2);
	p->addModel(modelBritix);
	p->addModel(modelCompa);
*/
}
