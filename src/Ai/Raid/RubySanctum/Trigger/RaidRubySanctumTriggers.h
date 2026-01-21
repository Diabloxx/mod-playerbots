#ifndef _PLAYERBOT_RAIDRUBYSANCTUMTRIGGERS_H
#define _PLAYERBOT_RAIDRUBYSANCTUMTRIGGERS_H

#include "Trigger.h"

// Baltharus
class RubySanctumBaltharusBrandTrigger : public Trigger
{
public:
    explicit RubySanctumBaltharusBrandTrigger(PlayerbotAI* botAI) : Trigger(botAI, "rubysanctum baltharus brand") {}
    bool IsActive() override { return false; }
};

class RubySanctumBaltharusSplitAddTrigger : public Trigger
{
public:
    explicit RubySanctumBaltharusSplitAddTrigger(PlayerbotAI* botAI) : Trigger(botAI, "rubysanctum baltharus split add") {}
    bool IsActive() override { return false; }
};

// Saviana
class RubySanctumSavianaEnrageTrigger : public Trigger
{
public:
    explicit RubySanctumSavianaEnrageTrigger(PlayerbotAI* botAI) : Trigger(botAI, "rubysanctum saviana enrage dispel") {}
    bool IsActive() override { return false; }
};

class RubySanctumSavianaFlameBeaconTrigger : public Trigger
{
public:
    explicit RubySanctumSavianaFlameBeaconTrigger(PlayerbotAI* botAI) : Trigger(botAI, "rubysanctum saviana flame beacon") {}
    bool IsActive() override { return false; }
};

// Zarithrian
class RubySanctumZarithrianCleaveArmorTrigger : public Trigger
{
public:
    explicit RubySanctumZarithrianCleaveArmorTrigger(PlayerbotAI* botAI) : Trigger(botAI, "rubysanctum zarithrian cleave armor swap") {}
    bool IsActive() override { return false; }
};

class RubySanctumZarithrianAddsTrigger : public Trigger
{
public:
    explicit RubySanctumZarithrianAddsTrigger(PlayerbotAI* botAI) : Trigger(botAI, "rubysanctum zarithrian adds") {}
    bool IsActive() override { return false; }
};

// Halion
class RubySanctumHalionCombustionTrigger : public Trigger
{
public:
    explicit RubySanctumHalionCombustionTrigger(PlayerbotAI* botAI) : Trigger(botAI, "rubysanctum halion combustion") {}
    bool IsActive() override { return false; }
};

class RubySanctumHalionConsumptionTrigger : public Trigger
{
public:
    explicit RubySanctumHalionConsumptionTrigger(PlayerbotAI* botAI) : Trigger(botAI, "rubysanctum halion consumption") {}
    bool IsActive() override { return false; }
};

class RubySanctumHalionMeteorTrigger : public Trigger
{
public:
    explicit RubySanctumHalionMeteorTrigger(PlayerbotAI* botAI) : Trigger(botAI, "rubysanctum halion meteor") {}
    bool IsActive() override { return false; }
};

class RubySanctumHalionInfernosTrigger : public Trigger
{
public:
    explicit RubySanctumHalionInfernosTrigger(PlayerbotAI* botAI) : Trigger(botAI, "rubysanctum halion infernos") {}
    bool IsActive() override { return false; }
};

class RubySanctumHalionCutterTrigger : public Trigger
{
public:
    explicit RubySanctumHalionCutterTrigger(PlayerbotAI* botAI) : Trigger(botAI, "rubysanctum halion cutter move") {}
    bool IsActive() override { return false; }
};

class RubySanctumHalionCorporealityOffBalanceTrigger : public Trigger
{
public:
    explicit RubySanctumHalionCorporealityOffBalanceTrigger(PlayerbotAI* botAI) : Trigger(botAI, "rubysanctum halion balance") {}
    bool IsActive() override { return false; }
};

#endif
