#include "RaidRubySanctumActionContext.h"
#include "Action.h"
#include "Trigger.h"
#include "RaidRubySanctumActions.h"

void RaidRubySanctumActionContext::InitTriggers(std::list<TriggerNode*>& triggers)
{
    // Saviana Ragefire (air/land phases, flame beacon, conflagration)
    triggers.push_back(new TriggerNode("saviana",
        NextAction::array(0, new NextAction("saviana position", ACTION_MOVE), new NextAction("saviana attack", ACTION_RAID + 1), nullptr)));

    // Baltharus the Warborn (blade tempest, clone, enervating brand)
    triggers.push_back(new TriggerNode("baltharus",
        NextAction::array(0, new NextAction("baltharus position", ACTION_MOVE), new NextAction("baltharus attack", ACTION_RAID + 1), nullptr)));

    // General Zarithrian (cleave armor stacks, roar, flamecaller adds)
    triggers.push_back(new TriggerNode("general zarithrian",
        NextAction::array(0, new NextAction("general zarithrian position", ACTION_MOVE), new NextAction("general zarithrian attack", ACTION_RAID + 1), nullptr)));

    // Halion (multi-phase: physical, twilight, both)
    triggers.push_back(new TriggerNode("halion physical",
        NextAction::array(0, new NextAction("halion position", ACTION_MOVE), new NextAction("halion attack", ACTION_RAID + 2), nullptr)));
    triggers.push_back(new TriggerNode("halion twilight",
        NextAction::array(0, new NextAction("halion twilight position", ACTION_MOVE), new NextAction("halion twilight attack", ACTION_RAID + 2), nullptr)));
    triggers.push_back(new TriggerNode("halion both",
        NextAction::array(0, new NextAction("halion split position", ACTION_MOVE), new NextAction("halion split attack", ACTION_RAID + 2), nullptr)));

    // Xerestrasza event (intro / event movement)
    triggers.push_back(new TriggerNode("xerestrasza event",
        NextAction::array(0, new NextAction("xerestrasza move", ACTION_MOVE), nullptr)));

    // Generic support triggers for mechanics that require movement/dispelling
    triggers.push_back(new TriggerNode("fiery combustion",
        NextAction::array(0, new NextAction("move out of fire", ACTION_MOVE), new NextAction("remove debuff", ACTION_INTERRUPT + 1), nullptr)));
    triggers.push_back(new TriggerNode("soul consumption",
        NextAction::array(0, new NextAction("move out of fire", ACTION_MOVE), new NextAction("remove debuff", ACTION_INTERRUPT + 1), nullptr)));
}
