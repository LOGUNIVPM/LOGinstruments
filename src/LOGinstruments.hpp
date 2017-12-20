#include "rack.hpp"

using namespace rack;


extern Plugin *plugin;

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

////////////////////
// module widgets
////////////////////

struct constantWidget : ModuleWidget {
	constantWidget();
};

struct constant2Widget : ModuleWidget {
	constant2Widget();
};

struct SpeckWidget : ModuleWidget {
	SpeckWidget();
};

struct BritixWidget : ModuleWidget {
	BritixWidget();
};

struct CompaWidget : ModuleWidget {
	CompaWidget();
};

struct LessMessWidget : ModuleWidget {
	TextField ** label;
	LessMessWidget();
	json_t *toJson() override;
	void fromJson(json_t *rootJ) override;
};


struct VelvetWidget : ModuleWidget {
	VelvetWidget();
};

////////////////////
// Additional GUI stuff
////////////////////

#ifdef DEBUG
  #define dbgPrint(a) printf a
#else
  #define dbgPrint(a) (void)0
#endif

