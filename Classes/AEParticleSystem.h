/**
 * @file	AEParticleSystem.h
 * @brief	Implementing AEro's particle system.
 *
 * @author	CaiyZ (Mr.IroN)
 * @version	0.1
 * @date	2013
*/

#pragma once

#define EMIT_TYPE_SINGLE			0
#define EMIT_TYPE_EXPLODE			1
#define EMIT_TYPE_SPIT				2

#define EMIT_MODE_ERUPT				0
#define EMIT_MODE_GRADUALLY			1


class AEParticleEmitter;


class AEParticle {

public:

	AEParticle();
	BOOLEAN isEnabled() { return enabled; }
	VOID setMatrixEmitter(AEParticleEmitter* _matrixEmit) { matrixEmit = _matrixEmit; }
	VOID setAttachEmitter(AEParticleEmitter* _attachEmit) { attachEmit = _attachEmit; }
	VOID disable() { enabled = 0; }
	VOID enable() { enabled = 1; }
	VOID setPosition(FLOAT _cx, FLOAT _cy, FLOAT _cz = 0.0) { cx = _cx;  cy = _cy;  cz = _cz; }
	VOID setDamping(FLOAT _damping) { damping = _damping; }
	VOID setVelocity(FLOAT _vx, FLOAT _vy, FLOAT _vz) { vx = _vx;  vy = _vy;  vz = _vz; }
	VOID setAcceleration(FLOAT _ax, FLOAT _ay, FLOAT _az) { ax = _ax;  ay = _ay;  az = _az; }
	VOID setFade(FLOAT _fade) { fade = _fade; }
	VOID setAngle(FLOAT _angle) { angle = _angle; }
	VOID setVRotate(FLOAT _vrotate) { vrotate = _vrotate; }
	VOID setRGB(FLOAT _r, FLOAT _g, FLOAT _b) { r = _r;  g = _g;  b = _b; }
	VOID setLife(FLOAT _life) { life = _life; }

	virtual VOID init() = 0;
	virtual VOID update() = 0;
	virtual VOID paint() = 0;
	virtual VOID reset() = 0;

protected:

	BOOLEAN enabled;
	AEParticleEmitter* matrixEmit;
	AEParticleEmitter* attachEmit;
	FLOAT cx, cy, cz, vx, vy, vz, ax, ay, az;
	FLOAT damping;
	FLOAT angle, vrotate;
	FLOAT r, g, b;
	FLOAT life, fade;

};


class AEParticleEmitter {

public:

	static const INT MAX_PARTICLE_COUNT			= 1000;

	AEParticleEmitter(FLOAT _x, FLOAT _y, FLOAT _z = 0.0, INT _count = 1, INT _life = 100);
	VOID setPosition(FLOAT _x, FLOAT _y, FLOAT _z = 0.0) { x = _x;  y = _y;  z = _z; }
	VOID setIndex(INT _index) { index = _index; }
	VOID resetTime() { time = 0; }
	FLOAT getX() { return x; }
	FLOAT getY() { return y; }
	FLOAT getZ() { return z; }
	INT getCount() { return count; }

	virtual VOID update() = 0;
	virtual VOID paint();

protected:

	INT index;
	AEParticle* group[MAX_PARTICLE_COUNT];
	INT count;
	FLOAT x, y, z;
	INT time, life;

};


struct AERO_PARTICLE_EMITTER_DESC {

	INT index;
	INT count;
	FLOAT x, y, z;
	INT time, life;

	AERO_PARTICLE_EMITTER_DESC() {
		
	}

};