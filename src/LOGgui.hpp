/*
 * LOGgui.hpp
 *
 * GUI objects from LOGinstruments
 */

#ifndef SRC_LOGGUI_HPP_
#define SRC_LOGGUI_HPP_

#include <cstring>

#include "rack.hpp"

using namespace rack;

// custom ports

struct logPortIn : app::SvgPort {
	logPortIn() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/logPortIn.svg")));
	}
};

struct logPortOut : app::SvgPort {
	logPortOut() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/logPortOut.svg")));
	}
};

/*
struct VCSPin4State : SVGSwitch, ToggleSwitch {
	VCSPin4State() {
		addFrame(APP->window->loadSvg(asset::pluginInstance(pluginInstance, "res/VCSPinNone.svg")));
		addFrame(APP->window->loadSvg(asset::pluginInstance(pluginInstance, "res/VCSPinRed.svg")));
		addFrame(APP->window->loadSvg(asset::pluginInstance(pluginInstance, "res/VCSPinBlue.svg")));
		addFrame(APP->window->loadSvg(asset::pluginInstance(pluginInstance, "res/VCSPinBlack.svg")));
		sw->wrap();
		box.size = sw->box.size;
	}
};

struct VCSPin2State : SVGSwitch, ToggleSwitch {
	VCSPin2State() {
		addFrame(APP->window->loadSvg(asset::pluginInstance(pluginInstance, "res/VCSPinNone.svg")));
		addFrame(APP->window->loadSvg(asset::pluginInstance(pluginInstance, "res/VCSPinWhite.svg")));
		sw->wrap();
		box.size = sw->box.size;
	}
};
*/

/*
struct baseTxtLabel : Widget {
	std::shared_ptr<Font> font;
	NVGcolor txtCol;
	char text[128];
	const int fh = 14;

	baseTxtLabel(Vec pos) {
		box.pos = pos;
		box.size.y = fh;
		setColor(0x00, 0x00, 0x00, 0xFF);
		font = APP->window->loadFont(asset::plugin(pluginInstance, "res/DejaVuSansMono.ttf"));
		setText(" ");
	}

	baseTxtLabel(Vec pos, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		box.pos = pos;
		box.size.y = fh;
		setColor(r, g, b, a);
		font = APP->window->loadFont(asset::plugin(pluginInstance, "res/DejaVuSansMono.ttf"));
		setText(" ");
	}

	void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		txtCol.r = r;
		txtCol.g = g;
		txtCol.b = b;
		txtCol.a = a;
	}

	void setText(const char * txt) {
		strncpy(text, txt, sizeof(text));
		box.size.x = strlen(text) * 8;
	}


	void drawTxt(const DrawArgs &args, const char * txt) {

		Vec c = Vec(box.size.x/2, box.size.y);

		nvgFontSize(args.vg, fh);
		nvgFontFaceId(args.vg, font->handle);
		nvgTextLetterSpacing(args.vg, -2);
		nvgTextAlign(args.vg, NVG_ALIGN_CENTER);
		nvgFillColor(args.vg, nvgRGBA(txtCol.r, txtCol.g, txtCol.b, txtCol.a));

		nvgText(args.vg, c.x, c.y+fh, txt, NULL);
	}

	void draw(const DrawArgs &args) override {
		Widget::draw(args.vg);
		drawTxt(args, text);
	}

};

struct paperTxtLabel : baseTxtLabel {

	using baseTxtLabel::baseTxtLabel;

	void draw(const DrawArgs &args) override {
		Widget::draw(args.vg);
		drawPaperBG(args);
		drawTxt(args, text);
	}

	void drawPaperBG(const DrawArgs &args) {
		Vec c = Vec(box.size.x/2, box.size.y);
		const int whalf = box.size.x/2;

		// Draw rectangle
		nvgFillColor(args.vg, nvgRGBA(0xff, 0xff, 0xff, 0xF0));
		{
			nvgBeginPath(args.vg);
			nvgMoveTo(args.vg, c.x -whalf, c.y +2);
			nvgLineTo(args.vg, c.x +whalf, c.y +2);
			nvgQuadTo(args.vg, c.x +whalf +5, c.y +2+7, c.x +whalf, c.y+fh+2);
			nvgLineTo(args.vg, c.x -whalf, c.y+fh+2);
			nvgQuadTo(args.vg, c.x -whalf -5, c.y +2+7, c.x -whalf, c.y +2);
			nvgClosePath(args.vg);
		}
		nvgFill(args.vg);
		nvgFillColor(args.vg, nvgRGBA(0x00, 0x00, 0x00, 0x0F));
		{
			nvgBeginPath(args.vg);
			nvgMoveTo(args.vg, c.x -whalf, c.y +2);
			nvgLineTo(args.vg, c.x +whalf, c.y +2);
			nvgQuadTo(args.vg, c.x +whalf +5, c.y +2+7, c.x +whalf, c.y+fh+2);
			nvgLineTo(args.vg, c.x -whalf, c.y+fh+2);
			nvgQuadTo(args.vg, c.x -whalf -5, c.y +2+7, c.x -whalf, c.y +2);
			nvgClosePath(args.vg);
		}
		nvgStrokeWidth(args.vg, 0.5);
		nvgStroke(args.vg);
	}

};
*/

/*
struct txtKnob : RoundBlackKnob {
	std::shared_ptr<Font> font;
	NVGcolor txtCol;

	txtKnob() {
		setColor(0x00, 0x00, 0x00, 0xFF);
		font = APP->window->loadFont(asset::plugin(pluginInstance, "res/DejaVuSansMono.ttf"));
	}

	void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		txtCol.r = r;
		txtCol.g = g;
		txtCol.b = b;
		txtCol.a = a;
	}

	txtKnob(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		setColor(r, g, b, a);
		font = APP->window->loadFont(asset::plugin(pluginInstance, "res/DejaVuSansMono.ttf"));
	}

	void draw(const DrawArgs &args) override {
		char tbuf[128];

		//FramebufferWidget::draw(args.vg);
		//drawTxt(args.vg, "VAL", value); // this follows the cursor only if the param ranges from minAngle to maxAngle (e.g. +-0.83*PI)

		if ( maxValue - floor(maxValue)  == 0.0 ) {
			snprintf(tbuf, sizeof(tbuf), "%d", (int)maxValue);
		} else {
			snprintf(tbuf, sizeof(tbuf), "%.3G", maxValue);
		}
		drawTxt(args.vg, tbuf, maxAngle);

		if ( minValue - floor(minValue) == 0.0 ) {
			snprintf(tbuf, sizeof(tbuf), "%d", (int)minValue);
		} else {
			snprintf(tbuf, sizeof(tbuf), "%.3G", minValue);
		}
		drawTxt(args.vg, tbuf, minAngle);
	}

	void drawTxt(const DrawArgs &args, const char * txt, float angle) {
		int fh = 14;
		nvgFontSize(args.vg, fh);
		nvgFontFaceId(args.vg, font->handle);
		nvgTextLetterSpacing(args.vg, -2);
		nvgFillColor(args.vg, nvgRGBA(txtCol.r, txtCol.g, txtCol.b, txtCol.a));
		Vec c = Vec(box.size.x/2, box.size.y/2);
		float r = box.size.x / 2;
		float x = c.x + (r*sinf(angle) );
		float y = fh + c.y - (r*cosf(angle) );
		int xl = strlen(txt) * 10;
		int xs = 10; // a little spacing on the right
		nvgText(args.vg, angle > 0.0 ? (x + xs) : (x - xl), y, txt, NULL);

	}
};
*/

/*
struct valueKnob : RoundBlackKnob {
	std::shared_ptr<Font> font;
	NVGcolor txtCol;

	valueKnob() {
		setColor(0x00, 0x00, 0x00, 0xFF);
		font = APP->window->loadFont(asset::plugin(pluginInstance, "res/DejaVuSansMono.ttf"));
	}

	void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		txtCol.r = r;
		txtCol.g = g;
		txtCol.b = b;
		txtCol.a = a;
	}

	valueKnob(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		setColor(r, g, b, a);
		font = APP->window->loadFont(asset::plugin(pluginInstance, "res/DejaVuSansMono.ttf"));
	}

	void draw(const DrawArgs &args) override {
		char tbuf[128];

		FramebufferWidget::draw(args.vg);
		//drawTxt(args.vg, "VAL", value); // this follows the cursor only if the param ranges from minAngle to maxAngle (e.g. +-0.83*PI)

		snprintf(tbuf, sizeof(tbuf), "%.3G", value);
		drawValue(args.vg, tbuf);

	}

	void drawValue(const DrawArgs &args, const char * txt) {

		Vec c = Vec(box.size.x/2, box.size.y);
		const int fh = 14;
		const int whalf = 15;

		// Draw rectangle
		nvgFillColor(args.vg, nvgRGBA(0xff, 0xff, 0xff, 0xF0));
		{
			nvgBeginPath(args.vg);
			nvgMoveTo(args.vg, c.x -whalf, c.y +2);
			nvgLineTo(args.vg, c.x +whalf, c.y +2);
			nvgQuadTo(args.vg, c.x +whalf +5, c.y +2+7, c.x +whalf, c.y+fh+2);
			nvgLineTo(args.vg, c.x -whalf, c.y+fh+2);
			nvgQuadTo(args.vg, c.x -whalf -5, c.y +2+7, c.x -whalf, c.y +2);
			nvgClosePath(args.vg);
		}
		nvgFill(args.vg);
		nvgFillColor(args.vg, nvgRGBA(0x00, 0x00, 0x00, 0x0F));
		{
			nvgBeginPath(args.vg);
			nvgMoveTo(args.vg, c.x -whalf, c.y +2);
			nvgLineTo(args.vg, c.x +whalf, c.y +2);
			nvgQuadTo(args.vg, c.x +whalf +5, c.y +2+7, c.x +whalf, c.y+fh+2);
			nvgLineTo(args.vg, c.x -whalf, c.y+fh+2);
			nvgQuadTo(args.vg, c.x -whalf -5, c.y +2+7, c.x -whalf, c.y +2);
			nvgClosePath(args.vg);
		}
		nvgStrokeWidth(args.vg, 0.5);
		nvgStroke(args.vg);


		nvgFontSize(args.vg, fh);
		nvgFontFaceId(args.vg, font->handle);
		nvgTextLetterSpacing(args.vg, -2);
		nvgTextAlign(args.vg, NVG_ALIGN_CENTER);
		nvgFillColor(args.vg, nvgRGBA(txtCol.r, txtCol.g, txtCol.b, txtCol.a));

		nvgText(args.vg, c.x, c.y+fh, txt, NULL);

	}
};

struct valueSnapKnob : valueKnob {
	valueSnapKnob() {
		snap = true;
	}
};

*/


#endif /* SRC_LOGGUI_HPP_ */
