#include "Leonardo.hpp"


Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	plugin->slug = "Leonardo";
	plugin->name = "Leonardo";
	createModel<constantWidget>(plugin, "constant", "constant");
	createModel<SpeckWidget>(plugin, "Speck", "Speck");
	createModel<BritixWidget>(plugin, "Britix", "Britix");
	createModel<BrexitWidget>(plugin, "Brexit", "Brexit");
	createModel<CompaWidget>(plugin, "Compa", "Compa");
}
