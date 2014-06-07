#ifndef __NN_MONSTER_FLOCK_GROUP_H__
#define __NN_MONSTER_FLOCK_GROUP_H__

#include <map>
#include "util/SimpleFlock.h"

void clampInside(Vec2f &p);

class IMonsterAction;
class PlayerShape;

struct FlockInfo {
	FlockInfo() {
	}
	FlockInfo(const Vec2f &p) : pos(p) {
	}
	Vec2f pos;
	Vec2f target;
};

typedef std::map<IMonsterAction*, FlockInfo> FlockDict;

class MonsterFlock : public FlockGroup {

public:
	MonsterFlock();
	virtual ~MonsterFlock();

	static bool open(void);
	static bool close(void);
	static MonsterFlock* instance(void);

	virtual void begin(void);
	virtual void end(void);

	Vec2f get(IMonsterAction* m);
	void targetTo(PlayerShape* p);

private:
	static MonsterFlock* mSelf;
	FlockDict mNow;
	FlockDict mLast;

};

#endif // __NN_MONSTER_FLOCK_GROUP_H__
