#include "RaidRubySanctumTriggers.h"
#include "Playerbots.h"

// Spell / aura and creature IDs used by triggers
namespace
{
    // Baltharus
    constexpr uint32 SPELL_ENERVATING_BRAND       = 74502;
    constexpr uint32 NPC_BALTHARUS_CLONE          = 39899;

    // Saviana
    constexpr uint32 SPELL_SAVIANA_ENRAGE         = 78722; // Tranquilizable enrage
    constexpr uint32 SPELL_FLAME_BEACON           = 74453;

    // Zarithrian
    constexpr uint32 SPELL_CLEAVE_ARMOR           = 74367;
    constexpr uint32 NPC_ONYX_FLAMECALLER         = 39814;

    // Halion fire side
    constexpr uint32 SPELL_FIERY_COMBUSTION       = 74562;
    constexpr uint32 SPELL_MARK_OF_COMBUSTION     = 74567;
    constexpr uint32 NPC_METEOR_STRIKE_MARK       = 40029;
    constexpr uint32 NPC_LIVING_INFERNO           = 40681;
    constexpr uint32 NPC_LIVING_EMBER             = 40683;
    // Halion shadow side
    constexpr uint32 SPELL_SOUL_CONSUMPTION       = 74792;
    constexpr uint32 SPELL_MARK_OF_CONSUMPTION    = 74795;
    // Cutter
    constexpr uint32 SPELL_TWILIGHT_CUTTER        = 74768;
    constexpr uint32 SPELL_TWILIGHT_CUTTER_TRIG   = 74769;
}

bool RubySanctumBaltharusSplitAddTrigger::IsActive()
{
    GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
    for (ObjectGuid const& guid : npcs)
    {
        Unit* u = botAI->GetUnit(guid);
        if (u && u->GetEntry() == NPC_BALTHARUS_CLONE)
            return true;
    }
    return false;
}

bool RubySanctumBaltharusBrandTrigger::IsActive()
{
    return bot->HasAura(SPELL_ENERVATING_BRAND);
}

bool RubySanctumSavianaEnrageTrigger::IsActive()
{
    Unit* boss = AI_VALUE2(Unit*, "find target", "saviana");
    return boss && boss->HasAura(SPELL_SAVIANA_ENRAGE);
}

bool RubySanctumSavianaFlameBeaconTrigger::IsActive()
{
    return bot->HasAura(SPELL_FLAME_BEACON);
}

bool RubySanctumZarithrianCleaveArmorTrigger::IsActive()
{
    Aura const* aura = bot->GetAura(SPELL_CLEAVE_ARMOR);
    return aura && aura->GetStackAmount() >= 2;
}

bool RubySanctumZarithrianAddsTrigger::IsActive()
{
    GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
    for (ObjectGuid const& guid : npcs)
    {
        Unit* u = botAI->GetUnit(guid);
        if (u && u->GetEntry() == NPC_ONYX_FLAMECALLER)
            return true;
    }
    return false;
}

bool RubySanctumHalionCombustionTrigger::IsActive()
{
    return bot->HasAura(SPELL_FIERY_COMBUSTION) || bot->HasAura(SPELL_MARK_OF_COMBUSTION);
}

bool RubySanctumHalionConsumptionTrigger::IsActive()
{
    return bot->HasAura(SPELL_SOUL_CONSUMPTION) || bot->HasAura(SPELL_MARK_OF_CONSUMPTION);
}

bool RubySanctumHalionMeteorTrigger::IsActive()
{
    GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
    for (ObjectGuid const& guid : npcs)
    {
        Unit* u = botAI->GetUnit(guid);
        if (u && u->GetEntry() == NPC_METEOR_STRIKE_MARK)
            return true;
    }
    return false;
}

bool RubySanctumHalionInfernosTrigger::IsActive()
{
    GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
    for (ObjectGuid const& guid : npcs)
    {
        Unit* u = botAI->GetUnit(guid);
        if (u && (u->GetEntry() == NPC_LIVING_INFERNO || u->GetEntry() == NPC_LIVING_EMBER))
            return true;
    }
    return false;
}

bool RubySanctumHalionCutterTrigger::IsActive()
{
    return bot->HasAura(SPELL_TWILIGHT_CUTTER) || bot->HasAura(SPELL_TWILIGHT_CUTTER_TRIG);
}

bool RubySanctumHalionCorporealityOffBalanceTrigger::IsActive()
{
    // If any corporeality aura is present and not neutral, consider off-balance
    static uint32 const corpAuras[] = { 74836,74835,74834,74833,74832,74826,74827,74828,74829,74830,74831 };
    for (uint32 auraId : corpAuras)
    {
        if (Aura const* aura = bot->GetAura(auraId))
        {
            // 74826/74827 roughly neutral; anything else is imbalance
            if (auraId != 74826 && auraId != 74827)
                return true;
        }
    }
    return false;
}
