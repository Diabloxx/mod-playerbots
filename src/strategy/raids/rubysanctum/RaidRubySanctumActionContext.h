#ifndef _PLAYERBOT_RAIDRUBYSANCTUMACTIONCONTEXT_H
#define _PLAYERBOT_RAIDRUBYSANCTUMACTIONCONTEXT_H

#include "Action.h"
#include "NamedObjectContext.h"
#include "RaidRubySanctumActions.h"

class RaidRubySanctumActionContext : public NamedObjectContext<Action>
{
  public:
    RaidRubySanctumActionContext()
    {
        creators["saviana spread"] = &RaidRubySanctumActionContext::saviana_spread;

        creators["baltharus position"] = &RaidRubySanctumActionContext::baltharus_position;
        creators["baltharus attack"] = &RaidRubySanctumActionContext::baltharus_attack;
        creators["baltharus brand run out"] = &RaidRubySanctumActionContext::baltharus_brand_run_out;

        creators["saviana position"] = &RaidRubySanctumActionContext::saviana_position;
        creators["saviana attack"] = &RaidRubySanctumActionContext::saviana_attack;
        creators["saviana run out"] = &RaidRubySanctumActionContext::saviana_run_out;

        creators["general zarithrian position"] = &RaidRubySanctumActionContext::zarithrian_position;
        creators["general zarithrian attack"] = &RaidRubySanctumActionContext::zarithrian_attack;

        creators["halion position"] = &RaidRubySanctumActionContext::halion_position;
        creators["halion attack"] = &RaidRubySanctumActionContext::halion_attack;

        creators["halion twilight position"] = &RaidRubySanctumActionContext::halion_twilight_position;
        creators["halion twilight attack"] = &RaidRubySanctumActionContext::halion_twilight_attack;

        creators["halion split position"] = &RaidRubySanctumActionContext::halion_split_position;
        creators["halion split attack"] = &RaidRubySanctumActionContext::halion_split_attack;

        creators["baltharus face away"] = &RaidRubySanctumActionContext::baltharus_face_away;
        creators["saviana face away"] = &RaidRubySanctumActionContext::saviana_face_away;
    }

  private:
    static Action* saviana_spread(PlayerbotAI* ai) { return new RubySanctumSavianaSpreadAction(ai); }

    static Action* baltharus_position(PlayerbotAI* ai)
    {
        return new RubySanctumPositionToBossAction(ai, "baltharus position", "baltharus the warborn", 6.0f);
    }
    static Action* baltharus_attack(PlayerbotAI* ai)
    {
        return new RubySanctumAttackBossAction(ai, "baltharus attack", "baltharus the warborn");
    }
    static Action* baltharus_brand_run_out(PlayerbotAI* ai)
    {
        return new RubySanctumRunOutAction(ai, "baltharus brand run out", 15.0f);
    }

    static Action* saviana_position(PlayerbotAI* ai)
    {
        // ranged/heals generally spread; default to a mid-range anchor
        return new RubySanctumPositionToBossAction(ai, "saviana position", "saviana ragefire", 18.0f);
    }
    static Action* saviana_attack(PlayerbotAI* ai)
    {
        return new RubySanctumAttackBossAction(ai, "saviana attack", "saviana ragefire");
    }
    static Action* saviana_run_out(PlayerbotAI* ai)
    {
        // Beacon/conflagration requires getting 10y away; pad to reduce overlap.
        return new RubySanctumRunOutAction(ai, "saviana run out", 18.0f);
    }

    static Action* zarithrian_position(PlayerbotAI* ai)
    {
        return new RubySanctumPositionToBossAction(ai, "general zarithrian position", "general zarithrian", 8.0f);
    }
    static Action* zarithrian_attack(PlayerbotAI* ai)
    {
        return new RubySanctumAttackBossAction(ai, "general zarithrian attack", "general zarithrian");
    }

    static Action* halion_position(PlayerbotAI* ai)
    {
        return new RubySanctumPositionToBossAction(ai, "halion position", "halion", 18.0f);
    }
    static Action* halion_attack(PlayerbotAI* ai)
    {
        return new RubySanctumAttackBossAction(ai, "halion attack", "halion");
    }

    static Action* halion_twilight_position(PlayerbotAI* ai)
    {
        return new RubySanctumPositionToBossAction(ai, "halion twilight position", "halion", 18.0f);
    }
    static Action* halion_twilight_attack(PlayerbotAI* ai)
    {
        return new RubySanctumAttackBossAction(ai, "halion twilight attack", "halion");
    }

    static Action* halion_split_position(PlayerbotAI* ai)
    {
        return new RubySanctumPositionToBossAction(ai, "halion split position", "halion", 18.0f);
    }
    static Action* halion_split_attack(PlayerbotAI* ai)
    {
        return new RubySanctumAttackBossAction(ai, "halion split attack", "halion");
    }

    static Action* baltharus_face_away(PlayerbotAI* ai)
    {
        return new RubySanctumFaceBossAwayAction(ai, "baltharus face away", "baltharus the warborn");
    }

    static Action* saviana_face_away(PlayerbotAI* ai)
    {
        return new RubySanctumFaceBossAwayAction(ai, "saviana face away", "saviana ragefire");
    }
};

#endif
