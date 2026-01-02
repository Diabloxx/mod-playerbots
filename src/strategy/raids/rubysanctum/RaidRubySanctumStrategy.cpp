#include "RaidRubySanctumStrategy.h"
#include "Strategy.h"

void RaidRubySanctumStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    // Boss order: Baltharus -> Saviana -> Zarithrian -> Halion

    // Baltharus
    triggers.push_back(new TriggerNode("baltharus",
        NextAction::array(0,
            new NextAction("baltharus position", ACTION_MOVE),
            new NextAction("baltharus attack", ACTION_RAID + 1),
            nullptr)));

    // Saviana
    triggers.push_back(new TriggerNode("saviana",
        NextAction::array(0,
            new NextAction("saviana position", ACTION_MOVE),
            new NextAction("saviana attack", ACTION_RAID + 1),
            nullptr)));

    // General Zarithrian
    triggers.push_back(new TriggerNode("general zarithrian",
        NextAction::array(0,
            new NextAction("general zarithrian position", ACTION_MOVE),
            new NextAction("general zarithrian attack", ACTION_RAID + 1),
            nullptr)));

    // Halion (phases)
    triggers.push_back(new TriggerNode("halion physical",
        NextAction::array(0,
            new NextAction("halion position", ACTION_MOVE),
            new NextAction("halion attack", ACTION_RAID + 2),
            nullptr)));

    triggers.push_back(new TriggerNode("halion twilight",
        NextAction::array(0,
            new NextAction("halion twilight position", ACTION_MOVE),
            new NextAction("halion twilight attack", ACTION_RAID + 2),
            nullptr)));

    triggers.push_back(new TriggerNode("halion both",
        NextAction::array(0,
            new NextAction("halion split position", ACTION_MOVE),
            new NextAction("halion split attack", ACTION_RAID + 2),
            nullptr)));

    // Generic mechanics
    triggers.push_back(new TriggerNode("fiery combustion",
        NextAction::array(0,
            new NextAction("move out of fire", ACTION_MOVE),
            new NextAction("remove debuff", ACTION_INTERRUPT + 1),
            nullptr)));

    // Xerestrasza event handlers
    triggers.push_back(new TriggerNode("xerestrasza event",
        NextAction::array(0, new NextAction("xerestrasza move", ACTION_MOVE), nullptr)));

    // Baltharus: Enervating Brand spread (handled in attack action but also give it high movement priority)
    triggers.push_back(new TriggerNode("enervating brand",
        NextAction::array(0,
            new NextAction("baltharus position", ACTION_MOVE + 2),
            nullptr)));

    // Saviana: Conflagration / targeted mechanic -> spread out (in addition to moving)
    triggers.push_back(new TriggerNode("fiery combustion",
        NextAction::array(0,
            new NextAction("saviana spread", ACTION_MOVE + 4),
            new NextAction("saviana position", ACTION_MOVE + 2),
            new NextAction("move out of fire", ACTION_MOVE),
            nullptr)));

    // Zarithrian: tank stack handling
    triggers.push_back(new TriggerNode("cleave armor",
        NextAction::array(0,
            new NextAction("zarithrian taunt swap", ACTION_RAID + 3),
            new NextAction("general zarithrian position", ACTION_MOVE + 3),
            nullptr)));

    // Zarithrian: add waves
    triggers.push_back(new TriggerNode("onyx flamecaller",
        NextAction::array(0,
            new NextAction("general zarithrian position", ACTION_MOVE + 1),
            new NextAction("general zarithrian attack", ACTION_RAID + 1),
            nullptr)));

    // Saviana: Flame Beacon -> spread to reduce collateral
    triggers.push_back(new TriggerNode("flame beacon",
        NextAction::array(0,
            new NextAction("saviana spread", ACTION_MOVE + 5),
            new NextAction("saviana position", ACTION_MOVE + 3),
            nullptr)));

    // Halion: Fiery Combustion (physical realm) -> move out
    triggers.push_back(new TriggerNode("fiery combustion",
        NextAction::array(0,
            new NextAction("halion position", ACTION_MOVE + 3),
            new NextAction("move out of fire", ACTION_MOVE + 2),
            nullptr)));

    // Halion: Soul Consumption (twilight realm) -> move out
    triggers.push_back(new TriggerNode("soul consumption",
        NextAction::array(0,
            new NextAction("halion twilight position", ACTION_MOVE + 3),
            new NextAction("move out of fire", ACTION_MOVE + 2),
            nullptr)));

    // Halion: Meteor Strike ground hazard -> reposition (both realms share logic, keep it simple)
    triggers.push_back(new TriggerNode("meteor strike",
        NextAction::array(0,
            new NextAction("halion position", ACTION_MOVE + 4),
            new NextAction("halion twilight position", ACTION_MOVE + 4),
            nullptr)));

    // Halion: Twilight Cutter -> immediate movement/positioning
    triggers.push_back(new TriggerNode("twilight cutter",
        NextAction::array(0,
            new NextAction("halion twilight position", ACTION_MOVE + 5),
            nullptr)));

    // Baltharus: tank should keep boss faced away from raid
    triggers.push_back(new TriggerNode("baltharus",
        NextAction::array(0,
            new NextAction("baltharus face away", ACTION_MOVE + 6),
            nullptr)));

    // Baltharus: Enervating Brand -> affected player runs out
    triggers.push_back(new TriggerNode("enervating brand",
        NextAction::array(0,
            new NextAction("baltharus brand run out", ACTION_MOVE + 7),
            nullptr)));

    // Saviana: tank should keep boss faced away from raid
    triggers.push_back(new TriggerNode("saviana",
        NextAction::array(0,
            new NextAction("saviana face away", ACTION_MOVE + 6),
            nullptr)));

    // Saviana: Flame Beacon / Conflagration -> run out hard
    triggers.push_back(new TriggerNode("saviana conflagration",
        NextAction::array(0,
            new NextAction("saviana run out", ACTION_MOVE + 8),
            new NextAction("saviana spread", ACTION_MOVE + 5),
            nullptr)));

    // Saviana: Enrage should be dispelled (Tranquilizing Shot / Anesthetic Poison)
    // For now, we just increase spacing (reduces collateral from raid-wide pulses and movement overlap).
    triggers.push_back(new TriggerNode("saviana enrage",
        NextAction::array(0,
            new NextAction("saviana spread", ACTION_MOVE + 2),
            nullptr)));
}

void RaidRubySanctumStrategy::InitMultipliers(std::vector<Multiplier*>& multipliers)
{
    // No multipliers yet; placeholder for difficulty scaling
}
