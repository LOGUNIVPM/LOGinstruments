#include "LOGinstruments.hpp"


Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	plugin->slug = "LOGinstruments";
	plugin->name = "LOGinstruments";
	createModel<constantWidget>(plugin, "constant", "constant");
	createModel<SpeckWidget>(plugin, "Speck", "Speck");
	createModel<BritixWidget>(plugin, "Britix", "Britix");
	/*createModel<BrexitWidget>(plugin, "Brexit", "Brexit");*/
	createModel<CompaWidget>(plugin, "Compa", "Compa");
	createModel<LessMessWidget>(plugin, "LessMess", "LessMess");
	createModel<VelvetWidget>(plugin, "Velvet", "Velvet");
}
