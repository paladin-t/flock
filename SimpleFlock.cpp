#include "SimpleFlock.h"
#include <limits>

#ifdef min
#	undef min
#endif
#ifdef max
#	undef max
#endif

#define DIST_SIMPLE 0
#define DIST_REAL 1
#define DIST_RULE DIST_SIMPLE

#define DATUM_NEAREST 0
#define DATUM_FIRST 1
#define DATUM_RULE DATUM_NEAREST

FlockGroup::Object::Object() {
	pos = Vec2f(
		std::numeric_limits<float>::quiet_NaN(),
		std::numeric_limits<float>::quiet_NaN()
	);
	dist = std::numeric_limits<float>::quiet_NaN();
}

FlockGroup::Object::Object(const Vec2f &o) : pos(o) {
	dist = std::numeric_limits<float>::quiet_NaN();
}

FlockGroup::Object::Object(const Vec2f &o, const Object &t) : pos(o) {
#if DIST_RULE == DIST_SIMPLE
	dist = fabs(pos.x - t.pos.x) + fabs(pos.y - t.pos.y);
#elif DIST_RULE == DIST_REAL
	dist = (pos - t.pos).length();
#else
#	error "Not defined."
#endif
}

void FlockGroup::Object::clear(void) {
	pos = Vec2f(
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max()
	);
	dist = std::numeric_limits<float>::max();
}

FlockGroup::FlockGroup() {
	adjuster = NULL;
	nearest.clear();
	radius = Vec2f(
		std::numeric_limits<float>::quiet_NaN(),
		std::numeric_limits<float>::quiet_NaN()
	);
	last = now = 0;
}

FlockGroup::~FlockGroup() {
}

void FlockGroup::begin(void) {
	last = now;
	now = 0;

	radius = nearest.pos - target.pos;
	float l = radius.normalize();
	l *= 0.5f;
	radius *= l;

	nearest.clear();
}

void FlockGroup::end(void) {
}

Vec2f FlockGroup::getTargetPosition(const Vec2f &o, Adjuster adj) {
	Object f(o, target);
	now++;
#if DATUM_RULE == DATUM_NEAREST
	if(f.dist < nearest.dist)
#elif DATUM_RULE == DATUM_FIRST
	if(now == 1)
#else
#	error "Not defined."
#endif
		nearest = f;

	if(radius.isInvalid()) {
		return target.pos;
	} else {
		float s = (float)(now - 1) / (float)last;
		float r = s * 3.14159265f * 2.0f;
		if(f.pos.x > target.pos.x)
			r = -r;
		Vec2f p = radius.rotate(r);
		if(adj)
			p = adj(p, target.pos, now, last);
		else if(adjuster)
			p = adjuster(p, target.pos, now, last);

		p += target.pos;

		return p;
	}
}

void FlockGroup::setTargetObject(const Vec2f &t) {
	target = Object(t);
}
