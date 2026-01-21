#ifndef _PLAYERBOT_RAIDRUBYSANCTUMSTRATEGY_H
#define _PLAYERBOT_RAIDRUBYSANCTUMSTRATEGY_H

#include "Strategy.h"
#include "Multiplier.h"

class RaidRubySanctumStrategy : public Strategy
{
public:
    explicit RaidRubySanctumStrategy(PlayerbotAI* botAI) : Strategy(botAI) {}

    std::string const getName() override { return "rubysanctum"; }
    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    void InitMultipliers(std::vector<Multiplier*>& multipliers) override;
};

#endif
