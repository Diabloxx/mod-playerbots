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
    constexpr uint32 NPC_METEOR_STRIKE_MARK       = 40029;
    constexpr uint32 NPC_LIVING_INFERNO           = 40681;
    constexpr uint32 NPC_LIVING_EMBER             = 40683;
}

bool RubySanctumBaltharusBrandAction::Execute(Event /*event*/)
{
    // Move away from the group when branded
    return MoveAway(bot, 12.0f);
}

bool RubySanctumBaltharusSplitAddAction::Execute(Event /*event*/)
{
    Unit* add = AI_VALUE2(Unit*, "find target", "baltharus clone");
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
    return Attack(target);
}

bool RubySanctumHalionCombustionAction::Execute(Event /*event*/)
{
    // Move out before dispel
    return MoveAway(bot, 18.0f);
}

bool RubySanctumHalionConsumptionAction::Execute(Event /*event*/)
{
    return MoveAway(bot, 18.0f);
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
        return MoveAway(mark, 15.0f);
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
