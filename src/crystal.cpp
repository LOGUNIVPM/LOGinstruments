#include "LOGinstruments.hpp"
#include "LOGgui.hpp"
#include "crystal.hpp"

struct CrystalModule: Module {
	enum ParamIds {
		PARAM_F0,
		PARAM_VOICES,
		PARAM_OSC_TYPE,
		PARAM_DETUNE,
		PARAM_VOLUME,
		PARAM_PITCH_IN_AMT,
		PARAM_KEYB_SCALE,
		PARAM_MANUAL_RST,
		PARAM_ILIKETHIS,
		NUM_PARAMS
	};

	enum InputIds {
		IN_CV1,
		IN_CV2,
		IN_CV3,
		NUM_INPUTS
	};

	enum OutputIds {
		OUT_MAIN,
		OUT_LOW_DET,
		OUT_HIGH_DET,
		NUM_OUTPUTS
	};

	enum LightIds {
		NUM_LIGHTS
	};

	aliasFreeOsc * voices[32];
	int allocd_voices;
	DCB<float>* dcblockL, *dcblockR;
	dsp::SchmittTrigger M_RST, CV_RST;
	int progrReset;

	int algo;
	float pitch;
	int rst;
	int vcycl;

	CrystalModule() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(PARAM_VOICES, 1.0, MAX_OSC, 1.0);
		configParam(PARAM_DETUNE, 0.0, 1.0, 0.1);
		configParam(PARAM_VOLUME, -30, 6.0, 0.0);
		configParam(PARAM_MANUAL_RST, 0.0, 1.0, 0.0);
		configParam(PARAM_ILIKETHIS, 0.0, 1.0, 0.0);

		dcblockL = new DCB<float>();
		dcblockR = new DCB<float>();
		algo = OSC_TYPE_SQU;
		pitch = 440.0;
		allocd_voices = 0;
		progrReset = 0;
		rst = 0;
		vcycl = 0;
	};
	void process(const ProcessArgs &args) override;

};

void CrystalModule::process(const ProcessArgs &args) {

    if (params[PARAM_VOICES].getValue() < 1.0) params[PARAM_VOICES].setValue(1.0);

	if ( allocd_voices < floor(params[PARAM_VOICES].getValue()) ) {
		voices[allocd_voices] = new
				aliasFreeOsc(params[PARAM_F0].getValue(), (OSC_TYPE)floor(params[PARAM_OSC_TYPE].getValue()), allocd_voices); // allocate one at a time
		allocd_voices++;
	} else if ( allocd_voices > floor(params[PARAM_VOICES].getValue()) ) {
#if DEBUG_LIGHTS
		lights[allocd_voices-1].value = 0.0;
#endif
		delete voices[allocd_voices-1];
		allocd_voices--;
	}

    vcycl = (vcycl+1) % allocd_voices;

	float cv1 = inputs[IN_CV1].getVoltage();
	float cv2 = inputs[IN_CV2].getVoltage();
	float cv3 = inputs[IN_CV3].getVoltage();

	algo = (int)round(cv2) % (int)NUM_OSC_TYPES;

	pitch = clamp(12.0f*(cv3), -32.0f, 54.0f) ;

	if (params[PARAM_ILIKETHIS].getValue() != 1.0) {
		if (M_RST.process(params[PARAM_MANUAL_RST].getValue()) || CV_RST.process(cv1)) {
			rst = RST_HYSTE_SMP;
			progrReset = allocd_voices-1;
		} else if (rst == 0) {
			rst = RST_HYSTE_SMP * ((cv1 > cv2) > cv3);
			if (rst) progrReset = allocd_voices-1;
		} else {
			rst--;
		}
	}
#if DEBUG_LIGHTS
	lights[MAX_OSC-1].value = rst;
#endif
	if (rst) {
		if (progrReset >= 0) {
			voices[progrReset]->setFreq(pitch);
			voices[progrReset]->setType((OSC_TYPE)algo);
			voices[progrReset]->reset();
			progrReset--;
		}
	}


	if ( fabs(voices[0]->getDtuBound() - params[PARAM_DETUNE].getValue()) > 0.001 ) {
		for (int i = 0; i < allocd_voices; i++) {
			voices[i]->setDtuBound(params[PARAM_DETUNE].getValue());
		}
	}


	if (outputs[OUT_MAIN].isConnected() || outputs[OUT_LOW_DET].isConnected() || outputs[OUT_HIGH_DET].isConnected()) {
		float resultL = 0.0, resultH = 0.0, result = 0.0;
		for (int i = 1; i < allocd_voices; i+=2) {
			resultL += PER_VOICE_ATT * voices[i]->process();
#if DEBUG_LIGHTS
			lights[i].value = (voices[i]->getNrg(), 0.0f, 10.0f);
#endif
		}
		for (int i = 2; i < allocd_voices; i+=2) {
			resultH += PER_VOICE_ATT * voices[i]->process();
#if DEBUG_LIGHTS
			lights[i].value = clamp(voices[i]->getNrg(), 0.0f, 10.0f);
#endif
		}
		float vol = powf(10, params[PARAM_VOLUME].getValue()/10.0);
		resultL += dcblockL->process(vol * PER_VOICE_ATT * voices[0]->process());
		resultH += dcblockR->process(vol * PER_VOICE_ATT * voices[0]->process());
		result = resultL+resultH;
		if (std::isnan(result) || result > 15.0) {
			for (int i = 0; i < allocd_voices; i++) {
				voices[i]->reset();
				dcblockL->reset();
				dcblockR->reset();
			}
		}
		outputs[OUT_LOW_DET].setVoltage(clamp(2.0f* vol * resultL, -15.0f, 15.0f));
		outputs[OUT_HIGH_DET].setVoltage(clamp(2.0f* vol * resultH, -15.0f, 15.0f));
		outputs[OUT_MAIN].setVoltage(clamp(vol * result, -15.0f, 15.0f));
	}

}


struct CrystalDisplay : TransparentWidget {
	CrystalModule *module;
	int frame = 0;
	int randLight[(int)MAX_OSC+1];
	NVGpaint pnt[(int)MAX_OSC+1];

	CrystalDisplay(CrystalModule * m) {
		module = m;
		memset(randLight, 0, sizeof(int)*(int)MAX_OSC);
	}

	void draw(const DrawArgs &args) override {

		if (module == NULL) return;

		Vec p;
		nvgSave(args.vg);
		Rect b = Rect(Vec(12, 0), box.size.minus(Vec(26, 40)));

#ifndef CA
		for (int i = 0; i < module->allocd_voices; i++) {

			nvgBeginPath(args.vg);

			p = gemcoor[i].mult(b.size);
			p = p.plus(b.pos);
			nvgMoveTo(args.vg, p.x, p.y);

			randLight[i+1] +=  (int)(128 *(random::uniform()-0.5));
			unsigned char nrg = (unsigned char) clamp(module->voices[i]->getNrg()*256.0f, 20.0f, 256.0f);
			pnt[i+1] = nvgLinearGradient(args.vg, b.pos.x, b.pos.y, b.pos.x+b.size.x, b.pos.y+b.size.y,
					nvgRGBA(0x40, 0x75, 0xDB, nrg), nvgRGBA(0x8C, 0xAC, 0xEA, nrg+randLight[i+1]));
			nvgStrokePaint(args.vg, pnt[i+1]);
			p = gemcoor[i+1].mult(b.size);
			p = p.plus(b.pos);
			nvgLineTo(args.vg, p.x, p.y);
			nvgLineCap(args.vg, NVG_ROUND);
			nvgMiterLimit(args.vg, 2.0);
			nvgStrokeWidth(args.vg, 1.3);
			nvgStroke(args.vg);

		}
#else
		for (int i = 0; i < module->allocd_voices; i++) {
			randLight[i+1] +=  (int)(256 *(random::uniform()-0.5));
			NVGpaint pnt = nvgLinearGradient(args.vg, b.pos.x, b.pos.y, b.pos.x+b.size.x, b.pos.y+b.size.y,
					nvgRGBA(0x20, 0x60, 0xD0, randLight[i]), nvgRGBA(0x20, 0x60, 0xD0, randLight[i+1]));
			nvgStrokePaint(args.vg, pnt);
			p = gemcoor[i+1].mult(b.size);
			nvgLineTo(args.vg, p.x, p.y);
		}
#endif
		nvgGlobalCompositeOperation(args.vg, NVG_LIGHTER);
		nvgResetScissor(args.vg);
		nvgRestore(args.vg);

	}


};

struct CrystalWidget : ModuleWidget {
	CrystalWidget(CrystalModule *module) {

		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/crystal-nofonts.svg")));


		addParam(createParam<RoundBlackKnob>(Vec(140, 214), module, CrystalModule::PARAM_VOICES));
		addParam(createParam<RoundBlackKnob>(Vec(15, 214), module, CrystalModule::PARAM_DETUNE));
		addParam(createParam<RoundBlackKnob>(Vec(15, 268), module, CrystalModule::PARAM_VOLUME));

		addInput(createInput<logPortIn>(Vec(0, 80), module, CrystalModule::IN_CV1));
		addInput(createInput<logPortIn>(Vec(box.size.x-24, 80), module, CrystalModule::IN_CV2));
		addInput(createInput<logPortIn>(Vec(box.size.x/2-12, 160), module, CrystalModule::IN_CV3));

		addParam(createParam<CKD6>(Vec(77, 269), module, CrystalModule::PARAM_MANUAL_RST));
		addParam(createParam<CKSS>(Vec(144, 273), module, CrystalModule::PARAM_ILIKETHIS));

		addOutput(createOutput<logPortOut>(Vec(78, 326), module, CrystalModule::OUT_MAIN));
		addOutput(createOutput<logPortOut>(Vec(5, 326), module, CrystalModule::OUT_LOW_DET));
		addOutput(createOutput<logPortOut>(Vec(150, 326), module, CrystalModule::OUT_HIGH_DET));


	{
		CrystalDisplay *display = new CrystalDisplay(module);
		display->box.pos = Vec(10, 60);
		display->box.size = Vec(box.size.x-20, 140);
		addChild(display);
	}

#ifdef DEBUG_LIGHTS
	int i;
	for (i = 0; i < (int)MAX_OSC-1; i++) {
		addChild(createLight<TinyLight<GreenLight>>(Vec(5 + (i*5), 360), module, i));
	}
	addChild(createLight<TinyLight<GreenLight>>(Vec(50, 30), module, i));
#endif
	}
};

Model *modelCrystal = createModel<CrystalModule, CrystalWidget>("Crystal");
