#include "RaidRubySanctumActions.h"
#include "Playerbots.h"

namespace
{
    // Spells reused in actions
    constexpr uint32 SPELL_ENERVATING_BRAND       = 74502;
    constexpr uint32 SPELL_FLAME_BEACON           = 74453;
    constexpr uint32 SPELL_FIERY_COMBUSTION       = 74562;
    constexpr uint32 SPELL_SOUL_CONSUMPTION       = 74792;

    // Creatures reused in actions
    constexpr uint32 NPC_BALTHARUS_CLONE          = 39899;
    constexpr uint32 NPC_ONYX_FLAMECALLER         = 39814;
    constexpr uint32 NPC_METEOR_STRIKE_MARK       = 40055;
    constexpr uint32 NPC_FIERY_COMBUSTION_MARK    = 40001;
    constexpr uint32 NPC_LIVING_INFERNO           = 40681;
    constexpr uint32 NPC_LIVING_EMBER             = 40683;
}

bool RubySanctumBaltharusBrandAction::Execute(Event /*event*/)
{
    // Move away from the group when branded
    // Keep movement short to avoid climbing terrain; only need ~10yd separation
    return MoveAway(bot, 10.0f);
}

bool RubySanctumBaltharusSplitAddAction::Execute(Event /*event*/)
{
    Unit* add = AI_VALUE2(Unit*, "find target", "baltharus clone");
    Unit* boss = AI_VALUE2(Unit*, "find target", "baltharus");
    if (!add)
    {
        GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
        for (ObjectGuid const& guid : npcs)
        {
            Unit* u = botAI->GetUnit(guid);
            if (u && u->GetEntry() == NPC_BALTHARUS_CLONE)
            {
                add = u;
                break;
            }
        }
    }
    if (!add)
        return false;

    // Keep tanks close to the pull spot; only nudge away from the main boss to avoid stacking
    if (botAI->IsTank(bot))
    {
        if (boss)
        {
            float dist = bot->GetDistance(boss);
            if (dist < 8.0f)
                MoveAway(boss, 8.0f);
        }
        else
            MoveFromGroup(8.0f);
    }
    return Attack(add);
}

bool RubySanctumSavianaEnrageAction::Execute(Event /*event*/)
{
    Unit* boss = AI_VALUE2(Unit*, "find target", "saviana");
    if (!boss)
        return false;

    // Try a tranquilize/soothe style dispel
    if (botAI->DoSpecificAction("tranquilizing shot", Event("rubysanctum"), true))
        return true;
    if (botAI->DoSpecificAction("soothe", Event("rubysanctum"), true))
        return true;
    if (botAI->DoSpecificAction("anesthetic poison", Event("rubysanctum"), true))
        return true;

    return false;
}

bool RubySanctumSavianaFlameBeaconAction::Execute(Event /*event*/)
{
    // Move away from raid to drop conflagration
    return MoveAway(bot, 15.0f);
}

bool RubySanctumZarithrianCleaveArmorSwapAction::Execute(Event /*event*/)
{
    Unit* boss = AI_VALUE2(Unit*, "find target", "zarithrian");
    if (!boss)
        return false;

    // Taunt swap
    if (botAI->DoSpecificAction("taunt", Event("rubysanctum"), true))
        return true;
    return botAI->DoSpecificAction("growl", Event("rubysanctum"), true);
}

bool RubySanctumZarithrianAddsAction::Execute(Event /*event*/)
{
    Unit* target = nullptr;
    GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
    for (ObjectGuid const& guid : npcs)
    {
        Unit* u = botAI->GetUnit(guid);
        if (u && u->GetEntry() == NPC_ONYX_FLAMECALLER)
        {
            target = u;
            break;
        }
    }
    if (!target)
        return false;

    // Main tank should hold the boss; let the other tank pick up adds
    Unit* boss = AI_VALUE2(Unit*, "find target", "zarithrian");
    if (botAI->IsTank(bot) && boss && boss->GetVictim() == bot)
        return false;

    // Encourage AoE while adds are up; healers re-engage healing after fear
    if (!botAI->IsTank(bot))
    {
        if (botAI->ContainsStrategy(STRATEGY_TYPE_DPS))
            botAI->ChangeStrategy("+dps aoe", BOT_STATE_COMBAT);
        if (botAI->IsHeal(bot))
            botAI->ChangeStrategy("+heal", BOT_STATE_COMBAT);
    }

    return Attack(target);
}

bool RubySanctumHalionCombustionAction::Execute(Event /*event*/)
{
    // Move out before dispel; prefer moving off the ground mark if we can see it
    Unit* mark = nullptr;
    GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
    for (ObjectGuid const& guid : npcs)
    {
        Unit* u = botAI->GetUnit(guid);
        if (u && u->GetEntry() == NPC_FIERY_COMBUSTION_MARK)
        {
            mark = u;
            break;
        }
    }

    // Small sidestep to drop combustion without kiting around the arena
    bool moved = mark ? MoveAway(mark, 10.0f) : MoveAway(bot, 10.0f);

    // Return to a safe position: tanks front, others on flank to avoid breath/tail
    if (Unit* boss = AI_VALUE2(Unit*, "find target", "halion"))
    {
        if (botAI->IsTank(bot))
            Follow(boss, 6.0f, 0.0f);
        else
            Follow(boss, 8.0f, ANGLE_90_DEG);
    }

    return moved;
}

bool RubySanctumHalionConsumptionAction::Execute(Event /*event*/)
{
    // Mirror combustion movement: short step keeps bots in range
    bool moved = MoveAway(bot, 10.0f);

    // Return to a safe position: tanks front, others on flank to avoid breath/tail
    if (Unit* boss = AI_VALUE2(Unit*, "find target", "halion"))
    {
        if (botAI->IsTank(bot))
            Follow(boss, 6.0f, 0.0f);
        else
            Follow(boss, 8.0f, ANGLE_90_DEG);
    }

    return moved;
}

bool RubySanctumHalionMeteorStrikeAction::Execute(Event /*event*/)
{
    GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
    Unit* mark = nullptr;
    for (ObjectGuid const& guid : npcs)
    {
        Unit* u = botAI->GetUnit(guid);
        if (u && u->GetEntry() == NPC_METEOR_STRIKE_MARK)
        {
            mark = u;
            break;
        }
    }
    if (mark)
    {
        bool moved = MoveAway(mark, 15.0f);

        // Reposition to Halion's flank after dodging to avoid breath/tail
        if (Unit* boss = AI_VALUE2(Unit*, "find target", "halion"))
            Follow(boss, 8.0f, ANGLE_90_DEG);

        return moved;
    }
    return false;
}

bool RubySanctumHalionInfernosAction::Execute(Event /*event*/)
{
    Unit* target = nullptr;
    GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
    for (ObjectGuid const& guid : npcs)
    {
        Unit* u = botAI->GetUnit(guid);
        if (u && (u->GetEntry() == NPC_LIVING_INFERNO || u->GetEntry() == NPC_LIVING_EMBER))
        {
            target = u;
            break;
        }
    }
    if (!target)
        return false;
    return Attack(target);
}

bool RubySanctumHalionTwilightCutterAction::Execute(Event /*event*/)
{
    // Simple avoidance: reposition away from current location
    return MoveAway(bot, 10.0f);
}

bool RubySanctumHalionCorporealityBalanceAction::Execute(Event /*event*/)
{
    // No direct control; acknowledge action
    return true;
}
