#include "RaidRubySanctumStrategy.h"
#include "Strategy.h"

void RaidRubySanctumStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    // Basic placeholders. Fill with boss-specific triggers later.
    triggers.push_back(new TriggerNode("baltharus",
        NextAction::array(0, new NextAction("baltharus position", ACTION_MOVE), nullptr)));
    triggers.push_back(new TriggerNode("baltharus",
        NextAction::array(0, new NextAction("baltharus attack", ACTION_RAID + 1), nullptr)));

    // Xerestrasza event handlers
    triggers.push_back(new TriggerNode("xerestrasza event",
        NextAction::array(0, new NextAction("xerestrasza move", ACTION_MOVE), nullptr)));
}

void RaidRubySanctumStrategy::InitMultipliers(std::vector<Multiplier*>& multipliers)
{
    // No multipliers yet; placeholder for difficulty scaling
}
