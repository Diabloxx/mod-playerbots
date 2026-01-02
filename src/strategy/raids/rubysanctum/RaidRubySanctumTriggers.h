#ifndef _PLAYERBOT_RAIDRUBYSANCTUMTRIGGERS_H
#define _PLAYERBOT_RAIDRUBYSANCTUMTRIGGERS_H

#include "PlayerbotAI.h"
#include "Playerbots.h"
#include "Trigger.h"

namespace RubySanctum
{
    static constexpr uint32 NPC_BALTHARUS_THE_WARBORN = 39751;
    static constexpr uint32 NPC_SAVIANA_RAGEFIRE = 39747;
    static constexpr uint32 NPC_GENERAL_ZARITHRIAN = 39746;
    static constexpr uint32 NPC_HALION = 39863;
    static constexpr uint32 NPC_ONYX_FLAMECALLER = 39814;
    static constexpr uint32 NPC_ORB_CARRIER = 40081;
    static constexpr uint32 NPC_METEOR_STRIKE_MARK = 40029;

    static constexpr uint32 SPELL_ENERVATING_BRAND = 74502;
    static constexpr uint32 SPELL_CONFLAGRATION = 74452;
    static constexpr uint32 SPELL_CLEAVE_ARMOR = 74367;
    static constexpr uint32 SPELL_FLAME_BEACON = 74453;
    static constexpr uint32 SPELL_FIERY_COMBUSTION = 74562;
    static constexpr uint32 SPELL_SOUL_CONSUMPTION = 74792;
    static constexpr uint32 SPELL_TWILIGHT_CUTTER = 74768;
    static constexpr uint32 SPELL_METEOR_STRIKE_AOE_DAMAGE = 74648;

    // Baltharus
    static constexpr uint32 SPELL_REPELLING_WAVE = 74509;

    // Saviana
    static constexpr uint32 SPELL_FLAME_BREATH = 74403;
    static constexpr uint32 SPELL_ENRAGE = 78722;

    // Conflagration (Saviana)
    // Beacon is already present; Conflagration aura varies by difficulty, but we at least react to the common one.
    static constexpr uint32 SPELL_CONFLAGRATION_AURA = 74453;

    static constexpr char const* SPELLNAME_CLEAVE_ARMOR = "cleave armor";
    static constexpr char const* SPELLNAME_ENERVATING_BRAND = "enervating brand";
}

class RubySanctumBossBaseTrigger : public Trigger
{
public:
    RubySanctumBossBaseTrigger(PlayerbotAI* ai, std::string const& name, uint32 bossEntry)
        : Trigger(ai, name), bossEntry_(bossEntry) {}

protected:
    uint32 bossEntry_;

    Unit* GetBoss() const
    {
        GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
        for (auto const& guid : npcs)
        {
            Unit* u = botAI->GetUnit(guid);
            if (u && u->IsAlive() && u->GetEntry() == bossEntry_)
                return u;
        }
        return nullptr;
    }
};

// Boss presence triggers
class BaltharusTrigger : public RubySanctumBossBaseTrigger
{
public:
    BaltharusTrigger(PlayerbotAI* ai) : RubySanctumBossBaseTrigger(ai, "baltharus", RubySanctum::NPC_BALTHARUS_THE_WARBORN) {}
    bool IsActive() override { return GetBoss() != nullptr; }
};

class SavianaTrigger : public RubySanctumBossBaseTrigger
{
public:
    SavianaTrigger(PlayerbotAI* ai) : RubySanctumBossBaseTrigger(ai, "saviana", RubySanctum::NPC_SAVIANA_RAGEFIRE) {}
    bool IsActive() override { return GetBoss() != nullptr; }
};

class ZarithrianTrigger : public RubySanctumBossBaseTrigger
{
public:
    ZarithrianTrigger(PlayerbotAI* ai)
        : RubySanctumBossBaseTrigger(ai, "general zarithrian", RubySanctum::NPC_GENERAL_ZARITHRIAN) {}
    bool IsActive() override { return GetBoss() != nullptr; }
};

class HalionPhysicalTrigger : public RubySanctumBossBaseTrigger
{
public:
    HalionPhysicalTrigger(PlayerbotAI* ai) : RubySanctumBossBaseTrigger(ai, "halion physical", RubySanctum::NPC_HALION) {}
    bool IsActive() override { return GetBoss() != nullptr; }
};

class HalionTwilightTrigger : public RubySanctumBossBaseTrigger
{
public:
    HalionTwilightTrigger(PlayerbotAI* ai) : RubySanctumBossBaseTrigger(ai, "halion twilight", RubySanctum::NPC_HALION) {}
    bool IsActive() override { return GetBoss() != nullptr; }
};

class HalionBothTrigger : public RubySanctumBossBaseTrigger
{
public:
    HalionBothTrigger(PlayerbotAI* ai) : RubySanctumBossBaseTrigger(ai, "halion both", RubySanctum::NPC_HALION) {}
    bool IsActive() override { return GetBoss() != nullptr; }
};

// Mechanic triggers
class RubySanctumConflagrationTrigger : public Trigger
{
public:
    RubySanctumConflagrationTrigger(PlayerbotAI* ai) : Trigger(ai, "fiery combustion") {}
    bool IsActive() override { return bot->HasAura(RubySanctum::SPELL_CONFLAGRATION); }
};

class RubySanctumEnervatingBrandTrigger : public Trigger
{
public:
    RubySanctumEnervatingBrandTrigger(PlayerbotAI* ai) : Trigger(ai, "enervating brand") {}

    bool IsActive() override
    {
        // Brand is the key mechanic that requires the affected player to move away.
        return bot->HasAura(RubySanctum::SPELL_ENERVATING_BRAND);
    }
};

class RubySanctumCleaveArmorTrigger : public Trigger
{
public:
    RubySanctumCleaveArmorTrigger(PlayerbotAI* ai) : Trigger(ai, "cleave armor") {}
    bool IsActive() override
    {
        // Tank swap / bubble behavior: only meaningful for tanks.
        if (!botAI->IsTank(bot))
            return false;

        // Trigger when stacks are getting high.
        Aura* a = botAI->GetAura(RubySanctum::SPELLNAME_CLEAVE_ARMOR, bot, false, false);
        return a && a->GetStackAmount() >= 4;
    }
};

class RubySanctumFlamecallerPresentTrigger : public Trigger
{
public:
    RubySanctumFlamecallerPresentTrigger(PlayerbotAI* ai) : Trigger(ai, "onyx flamecaller") {}

    bool IsActive() override
    {
        GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
        for (auto const& guid : npcs)
        {
            Unit* u = botAI->GetUnit(guid);
            if (u && u->IsAlive() && u->GetEntry() == RubySanctum::NPC_ONYX_FLAMECALLER)
                return true;
        }
        return false;
    }
};

class RubySanctumFlameBeaconTrigger : public Trigger
{
public:
    RubySanctumFlameBeaconTrigger(PlayerbotAI* ai) : Trigger(ai, "flame beacon") {}
    bool IsActive() override { return bot->HasAura(RubySanctum::SPELL_FLAME_BEACON); }
};

class RubySanctumFieryCombustionTrigger : public Trigger
{
public:
    RubySanctumFieryCombustionTrigger(PlayerbotAI* ai) : Trigger(ai, "fiery combustion") {}
    bool IsActive() override { return bot->HasAura(RubySanctum::SPELL_FIERY_COMBUSTION); }
};

class RubySanctumSoulConsumptionTrigger : public Trigger
{
public:
    RubySanctumSoulConsumptionTrigger(PlayerbotAI* ai) : Trigger(ai, "soul consumption") {}
    bool IsActive() override { return bot->HasAura(RubySanctum::SPELL_SOUL_CONSUMPTION); }
};

class RubySanctumTwilightCutterTrigger : public Trigger
{
public:
    RubySanctumTwilightCutterTrigger(PlayerbotAI* ai) : Trigger(ai, "twilight cutter") {}

    bool IsActive() override
    {
        // Either you are being hit by the beam, or you are dangerously close to an Orb Carrier.
        if (bot->HasAura(RubySanctum::SPELL_TWILIGHT_CUTTER))
            return true;

        // Use core API directly (no RubySanctum helper in this module)
        return bot->FindNearestCreature(RubySanctum::NPC_ORB_CARRIER, 12.0f) != nullptr;
    }
};

class RubySanctumMeteorStrikeTrigger : public Trigger
{
public:
    RubySanctumMeteorStrikeTrigger(PlayerbotAI* ai) : Trigger(ai, "meteor strike") {}

    bool IsActive() override
    {
        // If the countdown aura is active, or if a marker NPC is near, treat it as a high-priority movement hazard.
        if (bot->HasAura(RubySanctum::SPELL_METEOR_STRIKE_AOE_DAMAGE))
            return true;

        return bot->FindNearestCreature(RubySanctum::NPC_METEOR_STRIKE_MARK, 18.0f) != nullptr;
    }
};

class RubySanctumSavianaEnrageTrigger : public Trigger
{
public:
    RubySanctumSavianaEnrageTrigger(PlayerbotAI* ai) : Trigger(ai, "saviana enrage") {}

    bool IsActive() override
    {
        GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
        for (auto const& guid : npcs)
        {
            Unit* u = botAI->GetUnit(guid);
            if (u && u->IsAlive() && u->GetEntry() == RubySanctum::NPC_SAVIANA_RAGEFIRE)
                return u->HasAura(RubySanctum::SPELL_ENRAGE);
        }

        return false;
    }
};

class RubySanctumSavianaConflagrationTrigger : public Trigger
{
public:
    RubySanctumSavianaConflagrationTrigger(PlayerbotAI* ai) : Trigger(ai, "saviana conflagration") {}

    bool IsActive() override
    {
        // If we are beaconed, we must run out. If conflagration is already ticking, keep distance.
        if (bot->HasAura(RubySanctum::SPELL_FLAME_BEACON))
            return true;

        return bot->HasAura(RubySanctum::SPELL_CONFLAGRATION_AURA);
    }
};

#endif
