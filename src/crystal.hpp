#include <math.h>
#include <sys/time.h>
#include <dsp/digital.hpp>
#include "LOGinstruments.hpp"
#include "LOGgui.hpp"

#define SMALL		0.999 // for LkInt and DCB
#define CIRCB_MAX	192000
#define PAMP_SAW	1.0
#define PAMP_TRI	0.2
#define PAMP_SQU	1.0
#define PAMP_BIP	1.0
#define PAMP_PUL	1.0
#define PAMP_SINE	100.0
#define MAX_OSC		32.0
#define PER_VOICE_ATT 0.6
#define RST_HYSTE_SMP 1024 // must be >= MAX_OSC

Vec gemcoor[(int)MAX_OSC+1] = {
		Vec(0.0, 0.3),
		Vec(0.5, 1.0),
		Vec(0.1, 0.3),
		Vec(0.5, 1.0),
		Vec(0.22, 0.3),
		Vec(0.5, 1.0),
		Vec(0.33, 0.3),
		Vec(0.5, 1.0),
		Vec(0.44, 0.3),
		Vec(0.5, 1.0),
		Vec(0.56, 0.3),
		Vec(0.5, 1.0),
		Vec(0.67, 0.3),
		Vec(0.5, 1.0),
		Vec(0.78, 0.3),
		Vec(0.5, 1.0),
		Vec(0.9, 0.3),
		Vec(0.5, 1.0),
		Vec(1.0, 0.3),
		//Vec(0.5, 1.0),
		//20
		Vec(0.2, 0.0),
		Vec(0.8, 0.0),
		Vec(0.0, 0.3),
		Vec(1.0, 0.3),
		Vec(0.0, 0.3),
		Vec(0.2, 0.0),
		Vec(0.2, 0.3),
		Vec(0.3, 0.0),
		Vec(0.8, 0.3),
		Vec(0.7, 0.0),
		Vec(1.0, 0.3),
		Vec(0.8, 0.0),
		Vec(0.3, 0.0),
		Vec(0.5, 0.3),
};

float dturandTbl[] = { 0.0,
		0.2, -0.2, 0.5, -0.5, 0.1, -0.1, 0.6, -0.6,
		0.7, -0.7, 0.156, -0.156, 0.199, -0.199, 0.686, -0.686,
		0.808, -0.808, 0.432, -0.432, 0.111, -0.111, 0.343, -0.343,
		0.252, -0.252, 0.577, -0.577, 0.159, -0.159, 0.666 };


typedef enum {
		OSC_TYPE_SAW,
		OSC_TYPE_TRI,
		OSC_TYPE_SQU,
		OSC_TYPE_PUL,
		OSC_TYPE_BIP,
		OSC_TYPE_SINE,
		NUM_OSC_TYPES,
	} OSC_TYPE;

/* CIRCB */
template<typename T>
class circB {
public:
	circB(int iL, float pAmp) {
		int i;
		if (iL < 1 || iL > CIRCB_MAX) {
			iL = 100;
		}
		buffer = new T[CIRCB_MAX];
		for (i = 0; i < iL; i++) {
			buffer[i] = static_cast<T>(0.0);
		}
		bEnd = &buffer[iL];
		init(pAmp);
	}

	~circB() {
		delete[] buffer;
	}

	T process(T in) {
		incrpt(rp);
		T ret = *rp;
		incrpt(wp);
		*wp = in;

		return ret;
	}

	inline T read() {
		return *rp;
	}

	inline void inject(T in) {
		*wp += in;
	}

	void resize(int iL) {

		if (iL == bEnd - buffer) return; // same size
		if (iL < 1 || iL > CIRCB_MAX) return;

		while (bEnd < &buffer[iL]) {
			*(bEnd+1) = *bEnd; // replicate content
			bEnd++;
		}

		bEnd = &buffer[iL];
		if (rp > bEnd || wp > bEnd) init(0.0);

	}

	void reset() {
		T * pt = buffer;
		while(pt < bEnd)
			*(pt++) = static_cast<T>(0.0);
	}

	void init(float pAmp) {
		rp = buffer;
		wp = buffer;
		*rp += 1.0 * pAmp;
		decrpt(wp);
	}

	T* getBuffer() { return buffer; }

	int getSize() { return bEnd - buffer; }
	int getSizeBytes() { return sizeof(T) * (bEnd - buffer); }

private:
	T* buffer;
	T* bEnd;
	T* wp;
	T* rp;
	void incrpt(T*& pt) {
		pt++;
		if (pt > bEnd) pt = buffer;
	}
	void decrpt(T*& pt) {
		pt--;
		if (pt < buffer) pt = bEnd;
	}

};

/* SmCook Sine CLASS */
template<typename T>
class SCSine {
private:
	T k, kpl1, kmin1;
	T yz1, yz2;

public:

	SCSine(T f) {
		setf0(f);
		reset();
	}

	T process () {
		T y = kpl1 * yz1 - k * yz2;
		yz1 = - (kmin1 * yz2 + k * yz1);
		yz2 = y;
		return y;
	}

	void reset() {
		yz1 = PAMP_SINE;
		yz2 = 0.0;
	}

	void inject(float inj) {
		yz1 += inj;
		yz2 = 0.0;
	}

	void setf0(T f) {
		k = -cos(M_PI*f*APP->engine->getSampleTime());
		kpl1 = k + 1; kmin1 = 1 - k;
	}

};

/* BQUAD CLASS */
template<typename T>
class BQuad {
private:
	T b0, b1, b2, a1, a2;
	T z1, z2;

public:

	BQuad(T D) {
		setD(D);
		z1 = z2 = 0.0;
	}

	BQuad(T b0, T b1, T b2, T a1, T a2) : b0(b0), b1(b1), b2(b2), a1(a1), a2(a2) {
		z1 = z2 = 0.0;
	}

	T process (T x) {
		T y = x * b0 + z1;
		z1 = x * b1 - y * a1 + z2;
		z1 = x * b2 - y * a2;
		return y;
	}

	void setD(T D) {
		a1 = b1 = -(D-2.0) / (D+1.0);
		a2 = b0 = (D-1.0)*(D-2.0) / ((D+1.0)*(D+2.0));
		b2 = 1.0;
	}

};

template <typename T>
class DCB {
	T dxn, dyn, dxn1, dyn1;

public:

	DCB() {
		reset();
	}

	void reset() {
		dxn = dyn = dxn1 = dyn1 = 0.0;
	}

	T process(T xn) {
		dxn = xn;
		T dyn = dxn - dxn1 + SMALL * dyn1;
		dxn1 = dxn;
		dyn1 = xn = dyn;
		return dyn;
	}
};


template <typename T>
class LkInt {
	T yn, ixn1;

public:

	LkInt() {
		reset();
	}

	void reset() {
		yn = ixn1 = 0.0;
	}

	T process(T xn) {
		yn = xn + SMALL * ixn1;
		ixn1 = yn;
		return yn;
	}
};


class aliasFreeOsc {

private:
	int oscID;
	float fs;
	float f0, basef0, basePK; // f0 has detuning applied
	float dtubnded, dturand, origBound;
	float L;
	int iL;
	float frac;
	circB<float> * dly;
	int ect, timeToCheck;
	float nrg, avgNrg;
	float pAmp;
	float makeupG;
	float nrgThr;
	float polarity;
	OSC_TYPE type;
	int integrate;
	bool dcblock;
	float kscale;
	float dxn, dxn1, ixn, ixn1, iixn1, dyn1;
	BQuad<float> * APThiran;
	float xgain, xgainStep;
	long int xfade;
	DCB<float> *dcb;
	LkInt<float> *lki[2];
	SCSine<float> *scs;

public:

	aliasFreeOsc(float knob, OSC_TYPE type, int oscID) {
		this->oscID = oscID;
		dly = nullptr;
		fs = APP->engine->getSampleRate();
		pAmp = 10.0;
		iL = 100;
		frac = 1.5;
		APThiran = new BQuad<float>(frac);
		dly = new circB<float>(iL, pAmp);
		dcb = new DCB<float>();
		lki[0] = new LkInt<float>();
		lki[1] = new LkInt<float>();
		scs = new SCSine<float>(440.0);
		dturand = dturandTbl[oscID] / 12.0;
		dtubnded = dturand; // 0 <= dtu < 1 semitone
		setFreq(knob);
		setType(type);
		nrg = 0.0;
		nrgThr = 0.5 * pAmp; // energy threshold for reinit is half the initial pulse energy
		dxn = dxn1 = ixn = ixn1 = dyn1 = 0.0;
	}

	~aliasFreeOsc() {
		delete dly;
	}

	void setKscale(float ks) {
		kscale = ks;
	}

	void setD(float d) {
		APThiran->setD(d);
	}

	float getKscale() { return kscale; }

	/* expects a deviation in semitones from C4 */
	void setFreq(float pitchKnob) {
		basePK = pitchKnob;
		basef0 = 261.626 * powf(2.0, pitchKnob / 12.0);
		f0 = powf(2.0, dtubnded) * basef0;
		float Lprev = L;
		L = fs / f0;
		if (L == Lprev) return;

		int iLprev = iL;
		iL = (int)floor(L) - 1; // make it always >1 (best: 1.5-2.5)
		iL = std::max(2, iL); // never go below this
		frac = L - (float)iL;
		if (iL != iLprev) {
			if (abs(iL-iLprev) > 1) reset();
		}
		dly->resize(iL);
		APThiran->setD(frac);
		scs->setf0(f0);
		ect = timeToCheck = 10 * iL; // check energy every 10 periods
	}

	float getFreq() { return basef0; }
	float getPitchKn() { return basePK; }

	void setDtuBound(float b) {
		//if (b > 1.0 || b < 0.0) return;
		origBound = b;
		dtubnded = dturand * b;
		setFreq(basePK);
	}

	float getDtuBound() { return origBound; }

	void setType(OSC_TYPE t) {
		type = t;
		switch(type) {
		case OSC_TYPE_SINE:
			makeupG = 1.0;
			break;
		case OSC_TYPE_SAW:
			polarity = +1.0;
			integrate = 1;
			dcblock = true;
			pAmp = PAMP_SAW;
			makeupG = 1.0;
			break;
		case OSC_TYPE_SQU:
			polarity = -1.0;
			integrate = 1;
			dcblock = false;
			pAmp = PAMP_SQU;
			makeupG = 1.0;
			break;
		case OSC_TYPE_TRI:
			polarity = -1.0;
			integrate = 2;
			dcblock = false;
			pAmp = PAMP_TRI;
			makeupG = 0.25;
			break;
		case OSC_TYPE_PUL:
			polarity = +1.0;
			integrate = 0;
			dcblock = false;
			pAmp = PAMP_PUL;
			makeupG = 2.0;
			break;
		case OSC_TYPE_BIP:
			polarity = -1.0;
			integrate = 0;
			dcblock = false;
			pAmp = PAMP_BIP;
			makeupG = 2.0;
			break;
		default:
			break;
		}
#ifndef CRAZY_BEHAVIOR
		nrgThr = 0.5 * pAmp;
#endif
		dxn = dxn1 = ixn = ixn1 = dyn1 = 0.0;
		reset();

	}

	void reset() {
		dly->reset();
		dly->inject(pAmp);
		dcb->reset();
		lki[0]->reset();
		lki[1]->reset();
		scs->reset();
	}

	OSC_TYPE getType() {
		return type;
	}

	float process() {
		float xn;
		if (type == OSC_TYPE_SINE) {
			xn = scs->process();
			xn = 0.5 * ( (1-kscale)*xn + (kscale)*(xn*basef0/261.626) ); // keyboard scaling
		} else {
			xn = makeupG * dly->process(polarity * APThiran->process(dly->read()));

			if (integrate) {
				if (dcblock) {
					xn = dcb->process(xn);
				}
				for (int i = 0; i < integrate; i++) {
					xn = lki[i]->process(xn);
					xn = 0.5 * ( (1-kscale)*xn + (kscale)*(xn*basef0/261.626) ); // keyboard scaling
				}
			}
		}

		nrg += fabs(xn);
		ect--;
		if (ect == 0) {
			checkEnergy();
			ect = timeToCheck;
			nrg = 0.0;
		}
		return xn;
	}

	float getNrg() { return avgNrg; }

private:
	void checkEnergy() {
		avgNrg = nrg / timeToCheck;
		if (avgNrg < nrgThr) {
			reinit();
		}
	}

	void reinit() {
		if (type != OSC_TYPE_SINE) {
			dly->inject(pAmp * 0.5);
		} else {
			scs->inject(pAmp);
		}
	}

};




