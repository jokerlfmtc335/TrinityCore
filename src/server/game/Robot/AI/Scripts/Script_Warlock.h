#ifndef ROBOT_STRATEGIES_SCRIPT_WARLOCK_H
#define ROBOT_STRATEGIES_SCRIPT_WARLOCK_H

#ifndef WARLOCK_RANGE_DISTANCE
# define WARLOCK_RANGE_DISTANCE 30
#endif

#include "Script_Base.h"

enum WarlockCurseType :uint32
{
    WarlockCurseType_None = 0,
    WarlockCurseType_Element = 1,
    WarlockCurseType_Agony = 2,
    WarlockCurseType_Weakness = 3,
    WarlockCurseType_Tongues = 4,
};

class Script_Warlock :public Script_Base
{
public:
    Script_Warlock(Player* pmMe);
    void Update(uint32 pmDiff);
    void Reset();
    bool DPS(Unit* pmTarget, bool pmChase = true, bool pmAOE = false);
    bool Tank(Unit* pmTarget, bool pmChase, bool pmAOE = false);
    bool Heal(Unit* pmTarget);
    bool Buff(Unit* pmTarget, bool pmCure = true);

    bool DPS_Common(Unit* pmTarget, bool pmChase, bool pmAOE = false);
    bool DPS_Affliction(Unit* pmTarget, bool pmChase, bool pmAOE = false);
    bool DPS_Demonology(Unit* pmTarget, bool pmChase, bool pmAOE = false);
    bool DPS_Destruction(Unit* pmTarget, bool pmChase, bool pmAOE = false);

    uint32 curseType;

    uint32 shadowfuryDelay;
    uint32 shadowburnDelay;
    uint32 chaosBoltDelay;
    uint32 conflagrateDelay;
    uint32 curseDelay;
    uint32 armorDelay;
};
#endif
