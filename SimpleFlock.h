#ifndef __SIMPLE_FLOCK_H__
#define __SIMPLE_FLOCK_H__

#include "CommonTypes.h"

class FlockGroup {

public:
	typedef Vec2f (* Adjuster)(const Vec2f &p, const Vec2f &t, int n, int l);

	struct Object {
		Object();
		Object(const Vec2f &o);
		Object(const Vec2f &o, const Object &t);
		void clear(void);
		Vec2f pos;
		float dist;
	};

public:
	FlockGroup();
	virtual ~FlockGroup();

	virtual void begin(void);
	virtual void end(void);

	Vec2f getTargetPosition(const Vec2f &o, Adjuster adj = NULL);

	void setTargetObject(const Vec2f &t);

public:
	Adjuster adjuster;

protected:
	Vec2f radius;
	Object target;
	Object nearest;
	int now;
	int last;

};

#endif // __SIMPLE_FLOCK_H__
