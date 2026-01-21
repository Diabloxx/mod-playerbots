#ifndef _PLAYERBOT_RAIDRUBYSANCTUMACTIONS_H
#define _PLAYERBOT_RAIDRUBYSANCTUMACTIONS_H

#include "Action.h"

// Baltharus
class RubySanctumBaltharusBrandAction : public Action
{
public:
    explicit RubySanctumBaltharusBrandAction(PlayerbotAI* botAI) : Action(botAI, "rubysanctum baltharus brand") {}
    bool Execute(Event /*event*/) override { return true; }
};

class RubySanctumBaltharusSplitAddAction : public Action
{
public:
    explicit RubySanctumBaltharusSplitAddAction(PlayerbotAI* botAI) : Action(botAI, "rubysanctum baltharus split add") {}
    bool Execute(Event /*event*/) override { return true; }
};

// Saviana
class RubySanctumSavianaEnrageAction : public Action
{
public:
    explicit RubySanctumSavianaEnrageAction(PlayerbotAI* botAI) : Action(botAI, "rubysanctum saviana enrage dispel") {}
    bool Execute(Event /*event*/) override { return true; }
};

class RubySanctumSavianaFlameBeaconAction : public Action
{
public:
    explicit RubySanctumSavianaFlameBeaconAction(PlayerbotAI* botAI) : Action(botAI, "rubysanctum saviana flame beacon") {}
    bool Execute(Event /*event*/) override { return true; }
};

// Zarithrian
class RubySanctumZarithrianCleaveArmorSwapAction : public Action
{
public:
    explicit RubySanctumZarithrianCleaveArmorSwapAction(PlayerbotAI* botAI) : Action(botAI, "rubysanctum zarithrian cleave armor swap") {}
    bool Execute(Event /*event*/) override { return true; }
};

class RubySanctumZarithrianAddsAction : public Action
{
public:
    explicit RubySanctumZarithrianAddsAction(PlayerbotAI* botAI) : Action(botAI, "rubysanctum zarithrian adds") {}
    bool Execute(Event /*event*/) override { return true; }
};

// Halion
class RubySanctumHalionCombustionAction : public Action
{
public:
    explicit RubySanctumHalionCombustionAction(PlayerbotAI* botAI) : Action(botAI, "rubysanctum halion combustion") {}
    bool Execute(Event /*event*/) override { return true; }
};

class RubySanctumHalionConsumptionAction : public Action
{
public:
    explicit RubySanctumHalionConsumptionAction(PlayerbotAI* botAI) : Action(botAI, "rubysanctum halion consumption") {}
    bool Execute(Event /*event*/) override { return true; }
};

class RubySanctumHalionMeteorStrikeAction : public Action
{
public:
    explicit RubySanctumHalionMeteorStrikeAction(PlayerbotAI* botAI) : Action(botAI, "rubysanctum halion meteor") {}
    bool Execute(Event /*event*/) override { return true; }
};

class RubySanctumHalionInfernosAction : public Action
{
public:
    explicit RubySanctumHalionInfernosAction(PlayerbotAI* botAI) : Action(botAI, "rubysanctum halion infernos") {}
    bool Execute(Event /*event*/) override { return true; }
};

class RubySanctumHalionTwilightCutterAction : public Action
{
public:
    explicit RubySanctumHalionTwilightCutterAction(PlayerbotAI* botAI) : Action(botAI, "rubysanctum halion cutter move") {}
    bool Execute(Event /*event*/) override { return true; }
};

class RubySanctumHalionCorporealityBalanceAction : public Action
{
public:
    explicit RubySanctumHalionCorporealityBalanceAction(PlayerbotAI* botAI) : Action(botAI, "rubysanctum halion balance") {}
    bool Execute(Event /*event*/) override { return true; }
};

#endif
