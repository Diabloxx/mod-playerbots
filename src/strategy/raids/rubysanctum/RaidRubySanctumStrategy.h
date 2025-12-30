#ifndef _PLAYERBOT_RAIDRUBYSANCTUMSTRATEGY_H
#define _PLAYERBOT_RAIDRUBYSANCTUMSTRATEGY_H

#include "AiObjectContext.h"
#include "Multiplier.h"
#include "Strategy.h"

class RaidRubySanctumStrategy : public Strategy
{
public:
    RaidRubySanctumStrategy(PlayerbotAI* ai) : Strategy(ai) {}
    virtual std::string const getName() override { return "wotlk-ruby-sanctum"; }
    virtual void InitTriggers(std::vector<TriggerNode*> &triggers) override;
    virtual void InitMultipliers(std::vector<Multiplier*> &multipliers) override;
};

#endif
