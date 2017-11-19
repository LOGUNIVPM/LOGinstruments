#include "rack.hpp"


using namespace rack;


extern Plugin *plugin;

////////////////////
// module widgets
////////////////////

struct constantWidget : ModuleWidget {
	constantWidget();
};

struct SpeckWidget : ModuleWidget {
	SpeckWidget();
};

struct BritixWidget : ModuleWidget {
	BritixWidget();
};

struct BrexitWidget : ModuleWidget {
	BrexitWidget();
};

struct CompaWidget : ModuleWidget {
	CompaWidget();
};

struct LessMessWidget : ModuleWidget {
	TextField ** label;
	LessMessWidget();
};

struct VelvetWidget : ModuleWidget {
	VelvetWidget();
};

////////////////////
// Additional GUI stuff
////////////////////

struct VCSPin4State : SVGSwitch, ToggleSwitch {
	VCSPin4State() {
		addFrame(SVG::load(assetPlugin(plugin, "res/VCSPinNone.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/VCSPinRed.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/VCSPinBlue.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/VCSPinBlack.svg")));
		sw->wrap();
		box.size = sw->box.size;
	}
};

struct VCSPin2State : SVGSwitch, ToggleSwitch {
	VCSPin2State() {
		addFrame(SVG::load(assetPlugin(plugin, "res/VCSPinNone.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/VCSPinWhite.svg")));
		sw->wrap();
		box.size = sw->box.size;
	}
};
