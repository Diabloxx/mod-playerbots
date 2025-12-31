#include "RaidRubySanctumActionContext.h"
#include "Action.h"
#include "Trigger.h"
#include "RaidRubySanctumActions.h"

void RaidRubySanctumActionContext::InitTriggers(std::list<TriggerNode*>& triggers)
{
    // Keep this ActionContext focused on non-boss events / generic mechanics.
    // Boss presence handling is wired in `RaidRubySanctumStrategy::InitTriggers`.

    // Xerestrasza event (intro / event movement)
    triggers.push_back(new TriggerNode("xerestrasza event",
        NextAction::array(0, new NextAction("xerestrasza move", ACTION_MOVE), nullptr)));

    // Generic support triggers for mechanics that require movement/dispelling
    triggers.push_back(new TriggerNode("fiery combustion",
        NextAction::array(0,
            new NextAction("move out of fire", ACTION_MOVE),
            new NextAction("remove debuff", ACTION_INTERRUPT + 1),
            nullptr)));
}
