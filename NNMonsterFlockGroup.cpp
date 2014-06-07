#include "NNMonsterFlockGroup.h"
#include "NNMonsterAction.h"
#include "../base/NNCommon.h"
#include "../player/NNPlayer.h"

static const float _NEAREST = 64.0f;
static const float _PULL = 5.0f;

static Vec2f _adjuster(const Vec2f &p, const Vec2f &t, int n, int l) {
	float s = (float)(n - 1) / (float)l;
	s = s * 3.0f + 1.0f;
	Vec2f r(p);
	r.y *= s;
	clampInside(r);

	return r;
}

void clampInside(Vec2f &p) {
	const float _D = DATA2R(10.0f, 20.0f);
	if(p.x < _D) p.x = _D;
	else if(p.x > Misc::instance()->getWinWidth() - _D) p.x = Misc::instance()->getWinWidth() - _D;
	if(p.y < _D) p.y = _D;
	else if(p.y > Misc::instance()->getWinHeight() - _D) p.y = Misc::instance()->getWinHeight() - _D;
}

MonsterFlock* MonsterFlock::mSelf = NULL;

MonsterFlock::MonsterFlock() {
	adjuster = _adjuster;
}

MonsterFlock::~MonsterFlock() {
}

bool MonsterFlock::open(void) {
	if(mSelf)
		return false;

	mSelf = new MonsterFlock;

	return true;
}

bool MonsterFlock::close(void) {
	if(!mSelf)
		return false;

	delete mSelf;
	mSelf = NULL;

	return true;
}

MonsterFlock* MonsterFlock::instance(void) {
	return mSelf;
}

void MonsterFlock::begin(void) {
	FlockGroup::begin();
}

void MonsterFlock::end(void) {
	FlockGroup::end();

	mLast = mNow;
	mNow.clear();

	Vec2f o(
		std::numeric_limits<float>::quiet_NaN(),
		std::numeric_limits<float>::quiet_NaN()
	);
	for(FlockDict::iterator it = mLast.begin(); it != mLast.end(); ++it) {
		FlockInfo &fi = it->second;
		fi.target = getTargetPosition(fi.pos);

		if(!o.isInvalid()) {
			Vec2f d = fi.target - o;
			float e = _NEAREST / (fabs(d.x) + fabs(d.y));
			if(e > 1.0f) {
				if(e > _PULL) e = _PULL;
				fi.target = o + d * e;
				clampInside(fi.target);
			}
		}
		o = fi.target;
	}
}

Vec2f MonsterFlock::get(IMonsterAction* m) {
	mNow[m] = FlockInfo(m->getFlockPos());

	FlockDict::iterator it = mLast.find(m);
	if(it != mLast.end())
		return it->second.target;

	return target.pos;
}

void MonsterFlock::targetTo(PlayerShape* p) {
	setTargetObject(p->getFlockPos());
}
