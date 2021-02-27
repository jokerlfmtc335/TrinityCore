#include "Script_Warlock.h"
#include "Pet.h"
#include "PetAI.h"
#include "Group.h"
#include "RobotManager.h"

Script_Warlock::Script_Warlock(Player* pmMe) :Script_Base(pmMe)
{
    curseType = WarlockCurseType::WarlockCurseType_Weakness;

    shadowfuryDelay = 300000;
    shadowburnDelay = 300000;
    chaosBoltDelay = 300000;
    conflagrateDelay = 300000;
    curseDelay = 300000;
    armorDelay = 300000;
}

void Script_Warlock::Update(uint32 pmDiff)
{
    Script_Base::Update(pmDiff);
    if (shadowfuryDelay < 300000)
    {
        shadowfuryDelay += pmDiff;
    }
    if (shadowburnDelay < 300000)
    {
        shadowburnDelay += pmDiff;
    }
    if (chaosBoltDelay < 300000)
    {
        chaosBoltDelay += pmDiff;
    }
    if (conflagrateDelay < 300000)
    {
        conflagrateDelay += pmDiff;
    }
    if (curseDelay < 300000)
    {
        curseDelay += pmDiff;
    }
    if (armorDelay < 300000)
    {
        armorDelay += pmDiff;
    }
}

void Script_Warlock::Reset()
{
    curseType = WarlockCurseType::WarlockCurseType_Weakness;

    shadowfuryDelay = 300000;
    shadowburnDelay = 300000;
    chaosBoltDelay = 300000;
    conflagrateDelay = 300000;
    curseDelay = 300000;
    armorDelay = 300000;
    Script_Base::Reset();
}

bool Script_Warlock::Heal(Unit* pmTarget)
{
    return false;
}

bool Script_Warlock::Tank(Unit* pmTarget, bool pmChase, bool pmAOE)
{
    return false;
}

bool Script_Warlock::DPS(Unit* pmTarget, bool pmChase, bool pmAOE)
{
    bool meResult = false;
    if (!me)
    {
        return false;
    }
    if (!me->IsAlive())
    {
        return false;
    }
    if ((me->GetPower(Powers::POWER_MANA) * 100 / me->GetMaxPower(Powers::POWER_MANA)) < 30)
    {
        UseManaPotion();
    }
    uint32 myLevel = me->GetLevel();
    if (myLevel >= 6)
    {
        if ((me->GetPower(Powers::POWER_MANA) * 100 / me->GetMaxPower(Powers::POWER_MANA)) < 20)
        {
            if (me->GetHealthPct() > 50)
            {
                if (CastSpell(me, "Life Tap"))
                {
                    return true;
                }
            }
        }
    }
    uint32 characterTalentTab = me->GetMaxTalentCountTab();
    switch (characterTalentTab)
    {
    case 0:
    {
        meResult = DPS_Affliction(pmTarget, pmChase, pmAOE);
        break;
    }
    case 1:
    {
        meResult = DPS_Demonology(pmTarget, pmChase, pmAOE);
        break;
    }
    case 2:
    {
        meResult = DPS_Destruction(pmTarget, pmChase, pmAOE);
        break;
    }
    default:
    {
        meResult = DPS_Common(pmTarget, pmChase, pmAOE);
        break;
    }
    }
    if (meResult)
    {
        PetAttack(pmTarget);
    }
    else
    {
        PetStop();
    }

    return meResult;
}

bool Script_Warlock::DPS_Common(Unit* pmTarget, bool pmChase, bool pmAOE)
{
    return DPS_Destruction(pmTarget, pmChase, pmAOE);
}

bool Script_Warlock::DPS_Affliction(Unit* pmTarget, bool pmChase, bool pmAOE)
{
    if (!pmTarget)
    {
        return false;
    }
    else if (!pmTarget->IsAlive())
    {
        return false;
    }
    if (!me)
    {
        return false;
    }
    else if (!me->IsValidAttackTarget(pmTarget))
    {
        return false;
    }
    float targetDistance = me->GetDistance(pmTarget);
    if (pmChase)
    {
        if (targetDistance > ATTACK_RANGE_LIMIT)
        {
            return false;
        }
        if (!Chase(pmTarget, FOLLOW_FAR_DISTANCE))
        {
            return false;
        }
    }
    else
    {
        if (targetDistance > RANGED_MAX_DISTANCE)
        {
            return false;
        }
        if (!me->isInFront(pmTarget, M_PI / 16))
        {
            me->SetFacingToObject(pmTarget);
        }
    }
    if ((me->GetPower(Powers::POWER_MANA) * 100 / me->GetMaxPower(Powers::POWER_MANA)) < 10)
    {
        if (!me->GetCurrentSpell(CURRENT_AUTOREPEAT_SPELL))
        {
            if (CastSpell(pmTarget, "Shoot", WARLOCK_RANGE_DISTANCE))
            {
                return true;
            }
        }
    }
    if (CastSpell(pmTarget, "Shadow Bolt", WARLOCK_RANGE_DISTANCE))
    {
        return true;
    }

    return true;
}

bool Script_Warlock::DPS_Demonology(Unit* pmTarget, bool pmChase, bool pmAOE)
{
    if (!pmTarget)
    {
        return false;
    }
    else if (!pmTarget->IsAlive())
    {
        return false;
    }
    if (!me)
    {
        return false;
    }
    else if (!me->IsValidAttackTarget(pmTarget))
    {
        return false;
    }
    float targetDistance = me->GetDistance(pmTarget);
    if (pmChase)
    {
        if (targetDistance > ATTACK_RANGE_LIMIT)
        {
            return false;
        }
        if (!Chase(pmTarget, FOLLOW_FAR_DISTANCE))
        {
            return false;
        }
    }
    else
    {
        if (targetDistance > RANGED_MAX_DISTANCE)
        {
            return false;
        }
        if (!me->isInFront(pmTarget, M_PI / 16))
        {
            me->SetFacingToObject(pmTarget);
        }
    }
    if ((me->GetPower(Powers::POWER_MANA) * 100 / me->GetMaxPower(Powers::POWER_MANA)) < 10)
    {
        if (!me->GetCurrentSpell(CURRENT_AUTOREPEAT_SPELL))
        {
            if (CastSpell(pmTarget, "Shoot", WARLOCK_RANGE_DISTANCE))
            {
                return true;
            }
        }
    }
    if (CastSpell(pmTarget, "Shadow Bolt", WARLOCK_RANGE_DISTANCE))
    {
        return true;
    }

    return true;
}

bool Script_Warlock::DPS_Destruction(Unit* pmTarget, bool pmChase, bool pmAOE)
{
    if (!pmTarget)
    {
        return false;
    }
    else if (!pmTarget->IsAlive())
    {
        return false;
    }
    if (!me)
    {
        return false;
    }
    else if (!me->IsValidAttackTarget(pmTarget))
    {
        return false;
    }
    float targetDistance = me->GetDistance(pmTarget);
    if (pmChase)
    {
        if (targetDistance > ATTACK_RANGE_LIMIT)
        {
            return false;
        }
        if (!Chase(pmTarget, FOLLOW_FAR_DISTANCE))
        {
            return false;
        }
    }
    else
    {
        if (targetDistance > RANGED_MAX_DISTANCE)
        {
            return false;
        }
        if (!me->isInFront(pmTarget, M_PI / 16))
        {
            me->SetFacingToObject(pmTarget);
        }
    }
    if ((me->GetPower(Powers::POWER_MANA) * 100 / me->GetMaxPower(Powers::POWER_MANA)) < 10)
    {
        if (!me->GetCurrentSpell(CURRENT_AUTOREPEAT_SPELL))
        {
            if (CastSpell(pmTarget, "Shoot", WARLOCK_RANGE_DISTANCE))
            {
                return true;
            }
        }
    }
    uint32 myLevel = me->GetLevel();
    if (curseDelay > 5000)
    {
        curseDelay = 0;
        switch (curseType)
        {
        case WarlockCurseType::WarlockCurseType_None:
        {
            break;
        }
        case WarlockCurseType::WarlockCurseType_Element:
        {
            if (myLevel >= 32)
            {
                if (CastSpell(pmTarget, "Curse of the Elements", WARLOCK_RANGE_DISTANCE, true))
                {
                    return true;
                }
            }
            else
            {
                curseType = WarlockCurseType::WarlockCurseType_None;
            }
            break;
        }
        case WarlockCurseType::WarlockCurseType_Agony:
        {
            if (myLevel >= 8)
            {
                if (CastSpell(pmTarget, "Curse of Agony", WARLOCK_RANGE_DISTANCE, true, true))
                {
                    return true;
                }
            }
            else
            {
                curseType = WarlockCurseType::WarlockCurseType_None;
            }
            break;
        }
        case WarlockCurseType::WarlockCurseType_Weakness:
        {
            if (myLevel >= 4)
            {
                if (CastSpell(pmTarget, "Curse of Weakness", WARLOCK_RANGE_DISTANCE, true))
                {
                    return true;
                }
            }
            else
            {
                curseType = WarlockCurseType::WarlockCurseType_None;
            }
            break;
        }
        case WarlockCurseType::WarlockCurseType_Tongues:
        {
            if (myLevel >= 26)
            {
                if (CastSpell(pmTarget, "Curse of Tongues", WARLOCK_RANGE_DISTANCE, true))
                {
                    return true;
                }
            }
            else
            {
                curseType = WarlockCurseType::WarlockCurseType_None;
            }
            break;
        }
        default:
        {
            break;
        }
        }
    }
    if (myLevel >= 20)
    {
        if (pmAOE)
        {
            if (Group* myGroup = me->GetGroup())
            {
                Player* mainTank = NULL;
                for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
                {
                    if (Player* member = groupRef->GetSource())
                    {
                        if (AI_Base* memberAI = member->robotAI)
                        {
                            if (memberAI->groupRole == GroupRole::GroupRole_Tank)
                            {
                                mainTank = member;
                                break;
                            }
                        }
                    }
                }
                if (mainTank)
                {
                    uint32 meleeCount = 0;
                    for (std::set<Unit*>::iterator gaIT = mainTank->getAttackers().begin(); gaIT != mainTank->getAttackers().end(); gaIT++)
                    {
                        if (Unit* eachAttacker = *gaIT)
                        {
                            if (eachAttacker->IsAlive())
                            {
                                if (me->IsValidAttackTarget(eachAttacker))
                                {
                                    float eachAttackerDistance = mainTank->GetDistance(eachAttacker);
                                    if (eachAttackerDistance < AOE_TARGETS_RANGE)
                                    {
                                        meleeCount++;
                                    }
                                }
                            }
                        }
                    }
                    if (meleeCount > 2)
                    {
                        if (myLevel >= 50)
                        {
                            if (shadowfuryDelay > 21000)
                            {
                                if (CastSpell(pmTarget, "Shadowfury", WARLOCK_RANGE_DISTANCE, true))
                                {
                                    shadowfuryDelay = 0;
                                    return true;
                                }
                            }
                        }
                        if (CastSpell(pmTarget, "Rain of Fire", WARLOCK_RANGE_DISTANCE))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    if (myLevel >= 3)
    {
        if (CastSpell(pmTarget, "Immolate", WARLOCK_RANGE_DISTANCE, true, true))
        {
            return true;
        }
    }
    if (myLevel >= 40)
    {
        if (conflagrateDelay > 10500)
        {
            if (CastSpell(pmTarget, "Conflagrate", WARLOCK_RANGE_DISTANCE))
            {
                conflagrateDelay = 0;
                return true;
            }
        }
    }
    if (!sRobotManager->HasAura(pmTarget, "Shadow Mastery"))
    {
        if (CastSpell(pmTarget, "Shadow Bolt", WARLOCK_RANGE_DISTANCE))
        {
            return true;
        }
    }
    if (myLevel >= 60)
    {
        if (chaosBoltDelay > 12500)
        {
            if (CastSpell(pmTarget, "Chaos Bolt", WARLOCK_RANGE_DISTANCE))
            {
                chaosBoltDelay = 0;
                return true;
            }
        }
    }
    if (myLevel >= 20)
    {
        if (shadowburnDelay > 15500)
        {
            if (CastSpell(pmTarget, "Shadowburn", WARLOCK_RANGE_DISTANCE))
            {
                shadowburnDelay = 0;
                return true;
            }
        }
    }
    if (myLevel >= 64)
    {
        if (CastSpell(pmTarget, "Incinerate", WARLOCK_RANGE_DISTANCE))
        {
            return true;
        }
    }
    if (CastSpell(pmTarget, "Shadow Bolt", WARLOCK_RANGE_DISTANCE))
    {
        return true;
    }

    return true;
}

bool Script_Warlock::Buff(Unit* pmTarget, bool pmCure)
{
    if (!pmTarget)
    {
        return false;
    }
    else if (!pmTarget->IsAlive())
    {
        return false;
    }
    if (!me)
    {
        return false;
    }
    if (me->GetGUID() == pmTarget->GetGUID())
    {
        if (armorDelay > 5000)
        {
            armorDelay = 0;
            uint32 myLevel = me->GetLevel();
            if (myLevel >= 62)
            {
                if (CastSpell(me, "Fel Armor", WARLOCK_RANGE_DISTANCE, true))
                {
                    return true;
                }
            }
            else if (myLevel >= 20)
            {
                if (CastSpell(me, "Demon Armor", WARLOCK_RANGE_DISTANCE, true))
                {
                    return true;
                }
            }
            else if (myLevel >= 10)
            {
                if (CastSpell(me, "Demon Skin", WARLOCK_RANGE_DISTANCE, true))
                {
                    return true;
                }
            }
        }
        if (petting)
        {
            Pet* myPet = me->GetPet();
            if (!myPet)
            {
                if (CastSpell(me, "Summon Imp"))
                {
                    return true;
                }
            }
        }
        else
        {
            if (Pet* myPet = me->GetPet())
            {
                myPet->DespawnOrUnsummon(500ms, 0s);
            }
        }
    }

    return false;
}
