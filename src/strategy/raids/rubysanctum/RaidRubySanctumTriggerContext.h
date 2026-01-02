#ifndef _PLAYERBOT_RAIDRUBYSANCTUMTRIGGERCONTEXT_H
#define _PLAYERBOT_RAIDRUBYSANCTUMTRIGGERCONTEXT_H

#include "AiObjectContext.h"
#include "NamedObjectContext.h"
#include "RaidRubySanctumTriggers.h"

class RaidRubySanctumTriggerContext : public NamedObjectContext<Trigger>
{
public:
    RaidRubySanctumTriggerContext()
    {
        creators["baltharus"] = &RaidRubySanctumTriggerContext::baltharus;
        creators["saviana"] = &RaidRubySanctumTriggerContext::saviana;
        creators["general zarithrian"] = &RaidRubySanctumTriggerContext::zarithrian;

        creators["halion physical"] = &RaidRubySanctumTriggerContext::halion_physical;
        creators["halion twilight"] = &RaidRubySanctumTriggerContext::halion_twilight;
        creators["halion both"] = &RaidRubySanctumTriggerContext::halion_both;

        creators["fiery combustion"] = &RaidRubySanctumTriggerContext::fiery_combustion;
        creators["enervating brand"] = &RaidRubySanctumTriggerContext::enervating_brand;

        creators["cleave armor"] = &RaidRubySanctumTriggerContext::cleave_armor;
        creators["onyx flamecaller"] = &RaidRubySanctumTriggerContext::onyx_flamecaller;

        creators["flame beacon"] = &RaidRubySanctumTriggerContext::flame_beacon;
        creators["soul consumption"] = &RaidRubySanctumTriggerContext::soul_consumption;
        creators["twilight cutter"] = &RaidRubySanctumTriggerContext::twilight_cutter;
        creators["meteor strike"] = &RaidRubySanctumTriggerContext::meteor_strike;

        creators["saviana enrage"] = &RaidRubySanctumTriggerContext::saviana_enrage;
        creators["saviana conflagration"] = &RaidRubySanctumTriggerContext::saviana_conflagration;
    }

private:
    static Trigger* baltharus(PlayerbotAI* ai) { return new BaltharusTrigger(ai); }
    static Trigger* saviana(PlayerbotAI* ai) { return new SavianaTrigger(ai); }
    static Trigger* zarithrian(PlayerbotAI* ai) { return new ZarithrianTrigger(ai); }

    static Trigger* halion_physical(PlayerbotAI* ai) { return new HalionPhysicalTrigger(ai); }
    static Trigger* halion_twilight(PlayerbotAI* ai) { return new HalionTwilightTrigger(ai); }
    static Trigger* halion_both(PlayerbotAI* ai) { return new HalionBothTrigger(ai); }

    static Trigger* fiery_combustion(PlayerbotAI* ai) { return new RubySanctumFieryCombustionTrigger(ai); }
    static Trigger* enervating_brand(PlayerbotAI* ai) { return new RubySanctumEnervatingBrandTrigger(ai); }

    static Trigger* cleave_armor(PlayerbotAI* ai) { return new RubySanctumCleaveArmorTrigger(ai); }
    static Trigger* onyx_flamecaller(PlayerbotAI* ai) { return new RubySanctumFlamecallerPresentTrigger(ai); }

    static Trigger* flame_beacon(PlayerbotAI* ai) { return new RubySanctumFlameBeaconTrigger(ai); }
    static Trigger* soul_consumption(PlayerbotAI* ai) { return new RubySanctumSoulConsumptionTrigger(ai); }
    static Trigger* twilight_cutter(PlayerbotAI* ai) { return new RubySanctumTwilightCutterTrigger(ai); }
    static Trigger* meteor_strike(PlayerbotAI* ai) { return new RubySanctumMeteorStrikeTrigger(ai); }

    static Trigger* saviana_enrage(PlayerbotAI* ai) { return new RubySanctumSavianaEnrageTrigger(ai); }
    static Trigger* saviana_conflagration(PlayerbotAI* ai) { return new RubySanctumSavianaConflagrationTrigger(ai); }
};

#endif
