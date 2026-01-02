#ifndef _PLAYERBOT_RAIDRUBYSANCTUMACTIONS_H
#define _PLAYERBOT_RAIDRUBYSANCTUMACTIONS_H

#include "Action.h"
#include "AttackAction.h"
#include "MovementActions.h"
#include "PlayerbotAI.h"
#include "Playerbots.h"

class RubySanctumSavianaSpreadAction : public MovementAction
{
public:
    RubySanctumSavianaSpreadAction(PlayerbotAI* ai) : MovementAction(ai, "saviana spread") {}

    bool Execute(Event event) override
    {
        return botAI->DoSpecificAction("disperse set", event, true);
    }
};

class RubySanctumRunOutAction : public MovementAction
{
public:
    RubySanctumRunOutAction(PlayerbotAI* ai, std::string const& name, float radius)
        : MovementAction(ai, name), radius(radius)
    {
    }

    bool Execute(Event event) override
    {
        // Move away from group center; keep it bounded so we don't outrange healers.
        float bounded = std::min(std::max(radius, 8.0f), 22.0f);
        return MoveFromGroup(bounded);
    }

private:
    float radius;
};

class RubySanctumFaceBossAwayAction : public MovementAction
{
public:
    RubySanctumFaceBossAwayAction(PlayerbotAI* ai, std::string const& name, std::string const& bossName)
        : MovementAction(ai, name), bossName(bossName)
    {
    }

    bool Execute(Event event) override
    {
        if (!botAI->IsTank(bot))
            return false;

        Unit* boss = AI_VALUE2(Unit*, "find target", bossName);
        if (!boss)
            return false;

        if (boss->GetVictim() != bot)
            return false;

        // Face the boss away from raid: turn the tank so it looks at the average raid position.
        // We do not rotate if we can't find group members.
        GuidVector members = AI_VALUE(GuidVector, "group members");
        if (members.empty())
            return false;

        float ax = 0.0f, ay = 0.0f;
        uint32 count = 0;
        for (auto const& g : members)
        {
            if (Player* m = ObjectAccessor::FindPlayer(g))
            {
                if (!m->IsAlive() || m == bot)
                    continue;
                ax += m->GetPositionX();
                ay += m->GetPositionY();
                ++count;
            }
        }

        if (!count)
            return false;

        ax /= count;
        ay /= count;

        // Desired facing: boss should face AWAY from raid center => tank faces towards raid center.
        // So we set tank facing to raid center; since boss faces tank, boss faces away.
        float desired = bot->GetAngle(ax, ay);
        bot->SetFacingTo(desired);
        return true;
    }

private:
    std::string bossName;
};

class RubySanctumPositionToBossAction : public MovementAction
{
public:
    RubySanctumPositionToBossAction(PlayerbotAI* ai, std::string const& name, std::string const& bossName, float dist)
        : MovementAction(ai, name), bossName(bossName), dist(dist)
    {
    }

    bool Execute(Event event) override
    {
        Unit* boss = AI_VALUE2(Unit*, "find target", bossName);
        if (!boss)
            return false;

        // Tanks: just keep contact distance.
        if (botAI->IsTank(bot))
            return MoveNear(boss, std::max(2.0f, dist), MovementPriority::MOVEMENT_COMBAT);

        // Non-tanks: never stand in front of dragons/bosses with frontal cleave/breath.
        // Use existing rear-flank movement which tries to position behind the target.
        if (botAI->IsMelee(bot))
        {
            RearFlankAction rear(botAI, 2.5f);
            if (rear.isUseful())
                return rear.Execute(event);
            return MoveNear(boss, 3.0f, MovementPriority::MOVEMENT_COMBAT);
        }

        // Ranged/healers: keep a wider ring to reduce stacking.
        float desired = std::max(dist, 15.0f);
        if (botAI->IsHeal(bot))
            desired = std::min(std::max(dist, 18.0f), 28.0f);

        return MoveNear(boss, desired, MovementPriority::MOVEMENT_COMBAT);
    }

private:
    std::string bossName;
    float dist;
};

class RubySanctumAttackBossAction : public AttackAction
{
public:
    RubySanctumAttackBossAction(PlayerbotAI* ai, std::string const& name, std::string const& bossName)
        : AttackAction(ai, name), bossName(bossName)
    {
    }

    bool Execute(Event event) override
    {
        Unit* boss = AI_VALUE2(Unit*, "find target", bossName);
        if (!boss)
            return false;

        return Attack(boss);
    }

private:
    std::string bossName;
};

// Ruby Sanctum actions are currently provided by `RaidRubySanctumActionContext.*`.
// Keep this header intentionally minimal until action implementations are finalized.

#endif
