#ifndef _PLAYERBOT_RAIDRUBYSANCTUMACTIONCONTEXT_H
#define _PLAYERBOT_RAIDRUBYSANCTUMACTIONCONTEXT_H

#include "ActionContext.h"
#include "RaidRubySanctumActions.h"

class RaidRubySanctumActionContext : public ActionContext
{
public:
    RaidRubySanctumActionContext(PlayerbotAI* ai) : ActionContext(ai)
    {
        // Baltharus
        creators["baltharus position"] = &RaidRubySanctumActionContext::baltharus_position;
        creators["baltharus attack"] = &RaidRubySanctumActionContext::baltharus_attack;

        // Saviana
        creators["saviana position"] = &RaidRubySanctumActionContext::saviana_position;
        creators["saviana attack"] = &RaidRubySanctumActionContext::saviana_attack;

        // General Zarithrian
        creators["general zarithrian position"] = &RaidRubySanctumActionContext::zarithrian_position;
        creators["general zarithrian attack"] = &RaidRubySanctumActionContext::zarithrian_attack;

        // Halion
        creators["halion position"] = &RaidRubySanctumActionContext::halion_position;
        creators["halion attack"] = &RaidRubySanctumActionContext::halion_attack;
        creators["halion twilight position"] = &RaidRubySanctumActionContext::halion_twilight_position;
        creators["halion twilight attack"] = &RaidRubySanctumActionContext::halion_twilight_attack;
        creators["halion split position"] = &RaidRubySanctumActionContext::halion_split_position;
        creators["halion split attack"] = &RaidRubySanctumActionContext::halion_split_attack;

        // Xerestrasza
        creators["xerestrasza move"] = &RaidRubySanctumActionContext::xerestrasza_move;

        // Generic mechanics
        creators["move out of fire"] = &RaidRubySanctumActionContext::move_out_of_fire;
        creators["remove debuff"] = &RaidRubySanctumActionContext::remove_debuff;
    }

    void InitTriggers(std::list<TriggerNode*>& triggers) override;

private:
    static Action* baltharus_position(PlayerbotAI* ai) { return new BaltharusPositionAction(ai); }
    static Action* baltharus_attack(PlayerbotAI* ai) { return new BaltharusAttackAction(ai); }

    static Action* saviana_position(PlayerbotAI* ai) { return new SavianaPositionAction(ai); }
    static Action* saviana_attack(PlayerbotAI* ai) { return new SavianaAttackAction(ai); }

    static Action* zarithrian_position(PlayerbotAI* ai) { return new ZarithrianPositionAction(ai); }
    static Action* zarithrian_attack(PlayerbotAI* ai) { return new ZarithrianAttackAction(ai); }

    static Action* halion_position(PlayerbotAI* ai) { return new HalionPositionAction(ai); }
    static Action* halion_attack(PlayerbotAI* ai) { return new HalionAttackAction(ai); }
    static Action* halion_twilight_position(PlayerbotAI* ai) { return new HalionTwilightPositionAction(ai); }
    static Action* halion_twilight_attack(PlayerbotAI* ai) { return new HalionTwilightAttackAction(ai); }
    static Action* halion_split_position(PlayerbotAI* ai) { return new HalionSplitPositionAction(ai); }
    static Action* halion_split_attack(PlayerbotAI* ai) { return new HalionSplitAttackAction(ai); }

    static Action* xerestrasza_move(PlayerbotAI* ai) { return new XerestraszaMoveAction(ai); }

    static Action* move_out_of_fire(PlayerbotAI* ai) { return new MoveOutOfFireAction(ai); }
    static Action* remove_debuff(PlayerbotAI* ai) { return new RemoveDebuffAction(ai); }
};

#endif
