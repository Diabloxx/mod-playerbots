#ifndef _PLAYERBOT_RAIDRUBYSANCTUMACTIONS_H
#define _PLAYERBOT_RAIDRUBYSANCTUMACTIONS_H

#include "Action.h"

#include "AttackAction.h"
#include "MovementActions.h"
#include "ObjectAccessor.h"
#include "PlayerbotAI.h"
#include "Playerbots.h"
#include "ScriptedCreature.h"

#include <array>
#include <cmath>
#include <limits>

// Initial boss implementations for Ruby Sanctum raid.
// Keep behavior minimal but functional: tanks position bosses, DPS/healers assist and avoid simple hazards.

namespace RubySanctum
{
    // Boss entry ids (WotLK Ruby Sanctum)
    static constexpr uint32 NPC_BALTHARUS_THE_WARBORN = 39751;
    static constexpr uint32 NPC_SAVIANA_RAGEFIRE = 39747;
    static constexpr uint32 NPC_GENERAL_ZARITHRIAN = 39746;
    static constexpr uint32 NPC_HALION = 39863;

    // Common hazards (entries used by boss scripts)
    static constexpr uint32 NPC_FIRE_CYCLONE = 40091; // Halion (meteor/combustion visuals vary; this is a common hazard)

    // Spells (only those used for simple checks)
    static constexpr uint32 SPELL_BLADE_TEMPEST = 75125;      // Baltharus
    static constexpr uint32 SPELL_ENERVATING_BRAND = 74502;   // Baltharus

    // Saviana
    static constexpr uint32 SPELL_CONFLAGRATION = 74452;
    static constexpr char const* AURA_FLAME_BEACON_NAME = "Flame Beacon"; // DBC name-based lookup
    static constexpr uint32 SPELL_SAVIANA_ENRAGE = 78722;

    // General Zarithrian
    static constexpr uint32 NPC_ONYX_FLAMECALLER = 39814;
    static constexpr uint32 SPELL_CLEAVE_ARMOR = 74367;

    // Halion mechanics
    static constexpr uint32 NPC_METEOR_STRIKE_MARK = 40029;
    static constexpr uint32 NPC_ORB_CARRIER = 40081;

    static constexpr uint32 SPELL_FIERY_COMBUSTION = 74562;
    static constexpr uint32 SPELL_MARK_OF_COMBUSTION = 74567;
    static constexpr uint32 SPELL_SOUL_CONSUMPTION = 74792;
    static constexpr uint32 SPELL_MARK_OF_CONSUMPTION = 74795;
    static constexpr uint32 SPELL_TWILIGHT_CUTTER = 74768;

    static constexpr float HALION_ARENA_EDGE_RADIUS = 46.0f;

    inline Position GetHalionHomeCenter()
    {
        return Position(HALION_TANK_POS.GetPositionX(), HALION_TANK_POS.GetPositionY(), HALION_TANK_POS.GetPositionZ(), 0.0f);
    }

    inline bool IsNearArenaEdge(Player* bot, float edgeRadius)
    {
        Position center = GetHalionHomeCenter();
        return bot->GetExactDist2d(center.GetPositionX(), center.GetPositionY()) >= (edgeRadius - 3.0f);
    }

    inline bool MoveToArenaEdge(Player* bot, MovementAction* mover, float desiredRadius)
    {
        Position center = GetHalionHomeCenter();

        float dx = bot->GetPositionX() - center.GetPositionX();
        float dy = bot->GetPositionY() - center.GetPositionY();
        float dist = std::sqrt(dx * dx + dy * dy);
        if (dist < 0.001f)
        {
            dx = 1.0f;
            dy = 0.0f;
            dist = 1.0f;
        }

        dx /= dist;
        dy /= dist;

        float x = center.GetPositionX() + dx * desiredRadius;
        float y = center.GetPositionY() + dy * desiredRadius;
        float z = bot->GetPositionZ();
        bot->UpdateAllowedPositionZ(x, y, z);
        return mover->MoveTo(bot->GetMapId(), x, y, z, false, false, false, true, MovementPriority::MOVEMENT_COMBAT);
    }

    inline bool HasCombustionOrConsumption(PlayerbotAI* ai, Player* bot)
    {
        if (bot->HasAura(SPELL_FIERY_COMBUSTION) || bot->HasAura(SPELL_SOUL_CONSUMPTION))
            return true;

        if (ai->GetAura(SPELL_MARK_OF_COMBUSTION, bot, false, false))
            return true;

        if (ai->GetAura(SPELL_MARK_OF_CONSUMPTION, bot, false, false))
            return true;

        return false;
    }

    inline Creature* FindNearestMeteorMark(PlayerbotAI* ai, Player* bot, float maxDist)
    {
        return FindNearestCreatureByEntry(ai, bot, NPC_METEOR_STRIKE_MARK, maxDist);
    }

    inline bool MoveAwayFromUnit2d(Player* bot, MovementAction* mover, Unit* source, float step, MovementPriority prio)
    {
        if (!source)
            return false;

        float dx = bot->GetPositionX() - source->GetPositionX();
        float dy = bot->GetPositionY() - source->GetPositionY();
        float mag = std::sqrt(dx * dx + dy * dy);
        if (mag < 0.001f)
        {
            dx = 1.0f;
            dy = 0.0f;
            mag = 1.0f;
        }

        dx /= mag;
        dy /= mag;

        float x = bot->GetPositionX() + dx * step;
        float y = bot->GetPositionY() + dy * step;
        float z = bot->GetPositionZ();
        bot->UpdateAllowedPositionZ(x, y, z);
        return mover->MoveTo(bot->GetMapId(), x, y, z, false, false, false, true, prio);
    }

    inline bool OrbitAround(Player* bot, MovementAction* mover, Unit* centerUnit, bool clockwise, float orbitRadius, float step)
    {
        if (!centerUnit)
            return false;

        float dx = bot->GetPositionX() - centerUnit->GetPositionX();
        float dy = bot->GetPositionY() - centerUnit->GetPositionY();
        float dist = std::sqrt(dx * dx + dy * dy);
        if (dist < 0.001f)
        {
            dx = 1.0f;
            dy = 0.0f;
            dist = 1.0f;
        }

        dx /= dist;
        dy /= dist;

        float tx = clockwise ? -dy : dy;
        float ty = clockwise ? dx : -dx;

        float x = centerUnit->GetPositionX() + dx * orbitRadius + tx * step;
        float y = centerUnit->GetPositionY() + dy * orbitRadius + ty * step;
        float z = bot->GetPositionZ();
        bot->UpdateAllowedPositionZ(x, y, z);

        return mover->MoveTo(bot->GetMapId(), x, y, z, false, false, false, true, MovementPriority::MOVEMENT_COMBAT);
    }

    inline bool MoveToward(Player* bot, MovementAction* mover, Position const& pos, float step, MovementPriority prio)
    {
        float dx = pos.GetPositionX() - bot->GetPositionX();
        float dy = pos.GetPositionY() - bot->GetPositionY();
        float dist2d = std::sqrt(dx * dx + dy * dy);

        if (dist2d < 0.1f)
            return false;

        float move = std::min(step, dist2d);
        dx /= dist2d;
        dy /= dist2d;

        float x = bot->GetPositionX() + dx * move;
        float y = bot->GetPositionY() + dy * move;
        float z = bot->GetPositionZ();
        bot->UpdateAllowedPositionZ(x, y, z);

        return mover->MoveTo(bot->GetMapId(), x, y, z, false, false, false, true, prio);
    }

    inline Unit* FindBossByEntry(PlayerbotAI* ai, Player* bot, uint32 entry)
    {
        // Prefer current target
        if (Unit* current = ai->GetUnit(bot->GetTarget()))
            if (current->IsAlive() && current->GetEntry() == entry)
                return current;

        // Search nearby hostile NPCs
        GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
        for (auto const& guid : npcs)
        {
            Unit* u = ai->GetUnit(guid);
            if (u && u->IsAlive() && u->GetEntry() == entry)
                return u;
        }

        // Fall back to generic find target by name if available in values
        return nullptr;
    }

    inline void MarkSkull(Player* bot, PlayerbotAI* ai, Unit* target)
    {
        if (!target)
            return;

        Group* group = bot->GetGroup();
        if (!group)
            return;

        static constexpr uint8 SKULL_ICON_INDEX = 7;
        ObjectGuid current = group->GetTargetIcon(SKULL_ICON_INDEX);
        Unit* currentUnit = ai->GetUnit(current);
        if (!currentUnit || !currentUnit->IsAlive() || currentUnit != target)
            group->SetTargetIcon(SKULL_ICON_INDEX, bot->GetGUID(), target->GetGUID());
    }

    inline bool AnyGroupMemberTooClose(PlayerbotAI* ai, Player* bot, float minDist)
    {
        GuidVector members = AI_VALUE(GuidVector, "group members");
        for (auto const& g : members)
        {
            Unit* u = ai->GetUnit(g);
            if (!u || !u->IsAlive() || u == bot)
                continue;

            if (bot->GetExactDist2d(u) < minDist)
                return true;
        }
        return false;
    }

    inline bool SpreadFromGroup(PlayerbotAI* ai, Player* bot, MovementAction* mover, float safeRadius, float step)
    {
        GuidVector members = AI_VALUE(GuidVector, "group members");

        float vx = 0.0f;
        float vy = 0.0f;
        int nearby = 0;

        for (auto const& g : members)
        {
            Unit* u = ai->GetUnit(g);
            if (!u || !u->IsAlive() || u == bot)
                continue;

            float d = bot->GetExactDist2d(u);
            if (d < safeRadius)
            {
                float dx = bot->GetPositionX() - u->GetPositionX();
                float dy = bot->GetPositionY() - u->GetPositionY();

                float w = (safeRadius - d) / safeRadius;
                vx += dx * w;
                vy += dy * w;
                nearby++;
            }
        }

        if (nearby <= 0)
            return false;

        float mag = std::sqrt(vx * vx + vy * vy);
        if (mag < 0.001f)
            return false;

        vx /= mag;
        vy /= mag;

        float x = bot->GetPositionX() + vx * step;
        float y = bot->GetPositionY() + vy * step;
        float z = bot->GetPositionZ();
        bot->UpdateAllowedPositionZ(x, y, z);

        return mover->MoveTo(bot->GetMapId(), x, y, z, false, false, false, true, MovementPriority::MOVEMENT_COMBAT);
    }

    inline bool IsSavianaAirPhase(Unit* saviana, Player* bot)
    {
        // Heuristic: during air phase she's typically not in melee range / not targetable for melee.
        // Use distance to decide we should spread (works even if melee is forced out).
        return saviana && saviana->IsAlive() && bot->GetDistance2d(saviana) > 15.0f;
    }

    inline bool HasFlameBeacon(PlayerbotAI* ai, Player* bot)
    {
        return ai->GetAura(AURA_FLAME_BEACON_NAME, bot, false, false) != nullptr;
    }

    inline bool IsSavianaEnraged(Unit* saviana)
    {
        return saviana && saviana->IsAlive() && saviana->HasAura(SPELL_SAVIANA_ENRAGE);
    }

    inline bool TryRemoveEnrage(PlayerbotAI* ai, Player* bot, Unit* boss)
    {
        if (!boss)
            return false;

        // Only attempt if we're a likely dispeller.
        // Hunter: Tranquilizing Shot (handled by generic dispel if present)
        // Rogue: Anesthetic Poison (passive; no cast)
        if (!(bot->IsClass(CLASS_HUNTER) || bot->IsClass(CLASS_ROGUE)))
            return false;

        // Prefer explicit tranquilizing shot usage if the bot has it; otherwise try generic dispel.
        // Some cores expose it as a named spell action, but keep it simple and robust.
        if (bot->IsClass(CLASS_HUNTER))
        {
            if (ai->CanCastSpell("Tranquilizing Shot", boss))
                return ai->CastSpell("Tranquilizing Shot", boss);
        }

        // Fallback: attempt generic dispel mechanics if available
        if (ai->CanCastSpell("remove enrage", boss))
            return ai->CastSpell("remove enrage", boss);

        return false;
    }

    inline bool IsInFrontOf(Unit* boss, Player* bot)
    {
        if (!boss)
            return false;

        // Use core facing check if available.
        // NOTE: `isInFront` is a Unit member used widely in core scripts.
        return boss->isInFront(bot, 2.5f);
    }

    inline bool MoveToBossSide(PlayerbotAI* ai, Player* bot, MovementAction* mover, Unit* boss, float lateralDist)
    {
        if (!boss)
            return false;

        // Move to a point on the boss's side relative to its facing.
        // Compute a perpendicular vector to boss forward (cos/sin).
        float o = boss->GetOrientation();
        float sideX = -std::sin(o);
        float sideY = std::cos(o);

        float x = boss->GetPositionX() + sideX * lateralDist;
        float y = boss->GetPositionY() + sideY * lateralDist;
        float z = bot->GetPositionZ();
        bot->UpdateAllowedPositionZ(x, y, z);

        // Small LOS check might be expensive; keep it direct.
        return mover->MoveTo(bot->GetMapId(), x, y, z, false, false, false, true, MovementPriority::MOVEMENT_COMBAT);
    }

    inline Unit* FindNearestBossByEntry(PlayerbotAI* ai, Player* bot, uint32 entry)
    {
        Unit* nearest = nullptr;
        float best = std::numeric_limits<float>::max();

        GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
        for (auto const& guid : npcs)
        {
            Unit* u = ai->GetUnit(guid);
            if (!u || !u->IsAlive() || u->GetEntry() != entry)
                continue;

            float d = bot->GetDistance(u);
            if (d < best)
            {
                best = d;
                nearest = u;
            }
        }

        return nearest;
    }

    inline Unit* FindBaltharus(PlayerbotAI* ai, Player* bot)
    {
        // Baltharus clones share the same entry; to "ignore" clones we always pick the nearest one.
        return FindNearestBossByEntry(ai, bot, NPC_BALTHARUS_THE_WARBORN);
    }

    inline bool IsBaltharusBladeTempest(Unit* baltharus)
    {
        return baltharus && baltharus->IsAlive() && baltharus->HasAura(SPELL_BLADE_TEMPEST);
    }

    inline Creature* FindNearestCreatureByEntry(PlayerbotAI* ai, Player* bot, uint32 entry, float maxDist)
    {
        Creature* nearest = nullptr;
        float best = maxDist;

        GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
        for (auto const& guid : npcs)
        {
            Unit* u = ai->GetUnit(guid);
            if (!u || !u->IsAlive() || u->GetEntry() != entry)
                continue;

            float d = bot->GetDistance(u);
            if (d < best)
            {
                best = d;
                nearest = u->ToCreature();
            }
        }

        return nearest;
    }

    inline uint8 GetAuraStacks(PlayerbotAI* ai, Player* bot, uint32 spellId)
    {
        if (Aura* a = ai->GetAura(spellId, bot, false, false))
            return uint8(a->GetStackAmount());
        return 0;
    }

    inline bool TryClearCleaveArmor(PlayerbotAI* ai, Player* bot)
    {
        // Cleave Armor is a physical armor reduction debuff.
        // Paladin can clear with Divine Shield / Hand of Protection.
        uint8 stacks = GetAuraStacks(ai, bot, SPELL_CLEAVE_ARMOR);
        if (stacks < 4)
            return false;

        if (bot->IsClass(CLASS_PALADIN))
        {
            // Prefer Divine Shield if possible.
            if (ai->CanCastSpell("Divine Shield", bot))
                return ai->CastSpell("Divine Shield", bot);

            // Fallback: Hand of Protection if available.
            if (ai->CanCastSpell("Hand of Protection", bot))
                return ai->CastSpell("Hand of Protection", bot);
        }

        return false;
    }
}

// -------------------------
// Baltharus (boss 1)
// -------------------------
class BaltharusPositionAction : public MovementAction
{
public:
    BaltharusPositionAction(PlayerbotAI* ai) : MovementAction(ai, "baltharus position") {}

    bool Execute(Event) override
    {
        Unit* boss = RubySanctum::FindBaltharus(botAI, bot);
        if (!boss)
            return false;

        // Blade Tempest is in-front lethal damage. Only the tank should be in front.
        if (!botAI->IsTank(bot) && RubySanctum::IsInFrontOf(boss, bot))
        {
            botAI->Reset();
            return RubySanctum::MoveToBossSide(botAI, bot, this, boss, 7.0f);
        }

        // Tanks: keep boss near anchor unless boss is doing Blade Tempest (let the tank react normally)
        if (botAI->IsTank(bot))
        {
            if (RubySanctum::IsBaltharusBladeTempest(boss))
                return false;

            if (bot->GetExactDist2d(RubySanctum::BALTHARUS_TANK_POS) > 5.0f)
                return RubySanctum::MoveToward(bot, this, RubySanctum::BALTHARUS_TANK_POS, 3.0f,
                                               MovementPriority::MOVEMENT_COMBAT);

            bot->SetFacingToObject(boss);
            return false;
        }

        // Ranged/heal: keep some distance on ranged anchor
        if (botAI->IsRanged(bot) || botAI->IsHeal(bot))
        {
            if (bot->GetExactDist2d(RubySanctum::BALTHARUS_RANGED_POS) > 7.0f)
                return RubySanctum::MoveToward(bot, this, RubySanctum::BALTHARUS_RANGED_POS, 3.0f,
                                               MovementPriority::MOVEMENT_COMBAT);
        }

        return false;
    }
};

class BaltharusAttackAction : public AttackAction
{
public:
    BaltharusAttackAction(PlayerbotAI* ai) : AttackAction(ai, "baltharus attack") {}

    bool Execute(Event) override
    {
        Unit* boss = RubySanctum::FindBaltharus(botAI, bot);
        if (!boss)
            return false;

        RubySanctum::MarkSkull(bot, botAI, boss);

        bot->SetTarget(boss->GetGUID());
        bot->SetFacingToObject(boss);

        // Enervating Brand: move away from allies quickly.
        if (bot->HasAura(RubySanctum::SPELL_ENERVATING_BRAND))
        {
            const float safeRadius = 12.0f;
            const float step = 6.0f;

            // If anyone is close, spread out (more important than DPS).
            if (RubySanctum::AnyGroupMemberTooClose(botAI, bot, safeRadius))
            {
                botAI->Reset();
                if (RubySanctum::SpreadFromGroup(botAI, bot, this, safeRadius, step))
                    return true;
            }

            // Fallback: also ensure we are not stacked on boss (brand checks caster distance).
            if (!botAI->IsTank(bot) && bot->GetExactDist2d(boss) < 10.0f)
            {
                botAI->Reset();
                return MoveAway(boss, 14.0f);
            }
        }

        return Attack(boss);
    }
};

// -------------------------
// Saviana Ragefire (boss 2)
// -------------------------
class SavianaPositionAction : public MovementAction
{
public:
    SavianaPositionAction(PlayerbotAI* ai) : MovementAction(ai, "saviana position") {}

    bool Execute(Event) override
    {
        Unit* boss = RubySanctum::FindBossByEntry(botAI, bot, RubySanctum::NPC_SAVIANA_RAGEFIRE);
        if (!boss)
            return false;

        // Flame Breath cone: should only hit the main tank.
        // If we're not a tank and we're in front of Saviana, move to her side.
        if (!botAI->IsTank(bot) && RubySanctum::IsInFrontOf(boss, bot))
        {
            botAI->Reset();
            return RubySanctum::MoveToBossSide(botAI, bot, this, boss, 8.0f);
        }

        // Tanks should keep boss faced away from raid.
        if (botAI->IsTank(bot) && botAI->HasAggro(boss))
        {
            bot->SetFacingToObject(boss);
        }

        // Saviana Enrage: Hunters should try to remove it.
        // While enraged, keep a bit more spacing to reduce Fire Nova splash damage.
        if (RubySanctum::IsSavianaEnraged(boss))
        {
            if (RubySanctum::TryRemoveEnrage(botAI, bot, boss))
                return true;

            if (!botAI->IsTank(bot))
            {
                // Back off slightly to reduce nova splash risk.
                if (bot->GetDistance2d(boss) < 12.0f)
                {
                    botAI->Reset();
                    return MoveAway(boss, 15.0f);
                }
            }
        }

        // Air phase handling: spread.
        // Flame Beacon: spread harder (avoid cleaving beacon splash into group).
        const bool isAirPhase = RubySanctum::IsSavianaAirPhase(boss, bot);
        const bool hasFlameBeacon = RubySanctum::HasFlameBeacon(botAI, bot);

        if (isAirPhase || hasFlameBeacon)
        {
            const float safeRadius = hasFlameBeacon ? 12.0f : 8.0f;
            const float step = 4.0f;

            // Only move if needed (prevents strafing constantly)
            if (RubySanctum::AnyGroupMemberTooClose(botAI, bot, safeRadius))
            {
                botAI->Reset();
                return RubySanctum::SpreadFromGroup(botAI, bot, this, safeRadius, step);
            }

            return false;
        }

        if (botAI->IsTank(bot))
        {
            if (bot->GetExactDist2d(RubySanctum::SAVIANA_TANK_POS) > 6.0f)
                return RubySanctum::MoveToward(bot, this, RubySanctum::SAVIANA_TANK_POS, 3.0f,
                                               MovementPriority::MOVEMENT_COMBAT);

            bot->SetFacingToObject(boss);
            return false;
        }

        // Spread a bit from boss if we have conflagration
        if (bot->HasAura(RubySanctum::SPELL_CONFLAGRATION))
        {
            botAI->Reset();
            return MoveAway(boss, 12.0f);
        }

        if (botAI->IsRanged(bot) || botAI->IsHeal(bot))
        {
            if (bot->GetExactDist2d(RubySanctum::SAVIANA_RANGED_POS) > 8.0f)
                return RubySanctum::MoveToward(bot, this, RubySanctum::SAVIANA_RANGED_POS, 3.0f,
                                               MovementPriority::MOVEMENT_COMBAT);
        }

        return false;
    }
};

class SavianaAttackAction : public AttackAction
{
public:
    SavianaAttackAction(PlayerbotAI* ai) : AttackAction(ai, "saviana attack") {}

    bool Execute(Event) override
    {
        Unit* boss = RubySanctum::FindBossByEntry(botAI, bot, RubySanctum::NPC_SAVIANA_RAGEFIRE);
        if (!boss)
            return false;

        RubySanctum::MarkSkull(bot, botAI, boss);
        bot->SetTarget(boss->GetGUID());
        bot->SetFacingToObject(boss);

        return Attack(boss);
    }
};

// -------------------------
// General Zarithrian (boss 3)
// -------------------------
class ZarithrianPositionAction : public MovementAction
{
public:
    ZarithrianPositionAction(PlayerbotAI* ai) : MovementAction(ai, "general zarithrian position") {}

    bool Execute(Event) override
    {
        Unit* boss = RubySanctum::FindBossByEntry(botAI, bot, RubySanctum::NPC_GENERAL_ZARITHRIAN);
        if (!boss)
            return false;

        // Adds: tanks should pick up Onyx Flamecallers.
        if (botAI->IsTank(bot))
        {
            // If we have dangerous stacks, try to clear them (paladin tools).
            if (RubySanctum::TryClearCleaveArmor(botAI, bot))
                return true;

            // If we are *not* currently tanking Zarithrian, actively look for a Flamecaller to pick up.
            if (!botAI->HasAggro(boss))
            {
                if (Creature* add = RubySanctum::FindNearestCreatureByEntry(botAI, bot, RubySanctum::NPC_ONYX_FLAMECALLER, 60.0f))
                {
                    // Only switch if the add is not already controlled by a tank (simple heuristic).
                    Unit* addVictim = add->GetVictim();
                    if (!addVictim || !addVictim->IsPlayer() || !botAI->IsTank(addVictim->ToPlayer()))
                    {
                        botAI->Reset();
                        return MoveTo(add->GetMapId(), add->GetPositionX(), add->GetPositionY(), add->GetPositionZ(), false, false, false, true, MovementPriority::MOVEMENT_COMBAT);
                    }
                }
            }

            // Default: keep boss near anchor.
            if (bot->GetExactDist2d(RubySanctum::ZARITHRIAN_TANK_POS) > 6.0f)
                return RubySanctum::MoveToward(bot, this, RubySanctum::ZARITHRIAN_TANK_POS, 3.0f,
                                               MovementPriority::MOVEMENT_COMBAT);

            bot->SetFacingToObject(boss);
            return false;
        }

        // Non-tanks: keep range and avoid stacking near Flamecallers (Blast Nova is 40 yards).
        if (Creature* add = RubySanctum::FindNearestCreatureByEntry(botAI, bot, RubySanctum::NPC_ONYX_FLAMECALLER, 45.0f))
        {
            botAI->Reset();
            return MoveAway(add, 50.0f);
        }

        if (botAI->IsRanged(bot) || botAI->IsHeal(bot))
        {
            if (bot->GetExactDist2d(RubySanctum::ZARITHRIAN_RANGED_POS) > 8.0f)
                return RubySanctum::MoveToward(bot, this, RubySanctum::ZARITHRIAN_RANGED_POS, 3.0f,
                                               MovementPriority::MOVEMENT_COMBAT);
        }

        return false;
    }
};

class ZarithrianAttackAction : public AttackAction
{
public:
    ZarithrianAttackAction(PlayerbotAI* ai) : AttackAction(ai, "general zarithrian attack") {}

    bool Execute(Event) override
    {
        Unit* boss = RubySanctum::FindBossByEntry(botAI, bot, RubySanctum::NPC_GENERAL_ZARITHRIAN);
        if (!boss)
            return false;

        // Tanks that are not tanking boss should prioritize adds.
        if (botAI->IsTank(bot) && !botAI->HasAggro(boss))
        {
            if (Creature* add = RubySanctum::FindNearestCreatureByEntry(botAI, bot, RubySanctum::NPC_ONYX_FLAMECALLER, 60.0f))
            {
                RubySanctum::MarkSkull(bot, botAI, add);
                bot->SetTarget(add->GetGUID());
                bot->SetFacingToObject(add);
                return Attack(add);
            }
        }

        RubySanctum::MarkSkull(bot, botAI, boss);
        bot->SetTarget(boss->GetGUID());
        bot->SetFacingToObject(boss);
        return Attack(boss);
    }
};

// -------------------------
// Halion (boss 4)
// -------------------------
class HalionPositionAction : public MovementAction
{
public:
    HalionPositionAction(PlayerbotAI* ai) : MovementAction(ai, "halion position") {}

    bool Execute(Event) override
    {
        Unit* boss = RubySanctum::FindBossByEntry(botAI, bot, RubySanctum::NPC_HALION);
        if (!boss)
            return false;

        if (botAI->IsTank(bot))
        {
            if (bot->GetExactDist2d(RubySanctum::HALION_TANK_POS) > 7.0f)
                return RubySanctum::MoveToward(bot, this, RubySanctum::HALION_TANK_POS, 3.0f,
                                               MovementPriority::MOVEMENT_COMBAT);

            bot->SetFacingToObject(boss);
            return false;
        }

        if (botAI->IsRanged(bot) || botAI->IsHeal(bot))
        {
            if (bot->GetExactDist2d(RubySanctum::HALION_RANGED_POS) > 9.0f)
                return RubySanctum::MoveToward(bot, this, RubySanctum::HALION_RANGED_POS, 3.0f,
                                               MovementPriority::MOVEMENT_COMBAT);
        }

        return false;
    }
};

class HalionAttackAction : public AttackAction
{
public:
    HalionAttackAction(PlayerbotAI* ai) : AttackAction(ai, "halion attack") {}

    bool Execute(Event) override
    {
        Unit* boss = RubySanctum::FindBossByEntry(botAI, bot, RubySanctum::NPC_HALION);
        if (!boss)
            return false;

        RubySanctum::MarkSkull(bot, botAI, boss);
        bot->SetTarget(boss->GetGUID());
        bot->SetFacingToObject(boss);
        return Attack(boss);
    }
};

class HalionTwilightPositionAction : public HalionPositionAction
{
public:
    HalionTwilightPositionAction(PlayerbotAI* ai) : HalionPositionAction(ai)
    {
        setName("halion twilight position");
    }

    bool Execute(Event event) override
    {
        Unit* boss = RubySanctum::FindBossByEntry(botAI, bot, RubySanctum::NPC_HALION);
        if (!boss)
            return false;

        // Twilight Cutter: keep moving (simple orbit) to emulate raid rotation.
        // Trigger if cutter aura is present or if orb carrier is close.
        if (bot->HasAura(RubySanctum::SPELL_TWILIGHT_CUTTER)
            || RubySanctum::FindNearestCreatureByEntry(botAI, bot, RubySanctum::NPC_ORB_CARRIER, 16.0f))
        {
            bool clockwise = true;
            if (Group* group = bot->GetGroup())
                clockwise = (group->GetGUID().GetCounter() % 2) == 0;

            float orbitRadius = botAI->IsTank(bot) ? 11.0f : 18.0f;
            return RubySanctum::OrbitAround(bot, this, boss, clockwise, orbitRadius, 6.0f);
        }

        return HalionPositionAction::Execute(event);
    }
};

class HalionTwilightAttackAction : public HalionAttackAction
{
public:
    HalionTwilightAttackAction(PlayerbotAI* ai) : HalionAttackAction(ai)
    {
        setName("halion twilight attack");
    }
};

class HalionSplitPositionAction : public HalionPositionAction
{
public:
    HalionSplitPositionAction(PlayerbotAI* ai) : HalionPositionAction(ai)
    {
        setName("halion split position");
    }
};

class HalionSplitAttackAction : public HalionAttackAction
{
public:
    HalionSplitAttackAction(PlayerbotAI* ai) : HalionAttackAction(ai)
    {
        setName("halion split attack");
    }
};

// -------------------------
// Xerestrasza / generic utilities
// -------------------------
class XerestraszaMoveAction : public MovementAction
{
public:
    XerestraszaMoveAction(PlayerbotAI* ai) : MovementAction(ai, "xerestrasza move") {}

    bool Execute(Event) override
    {
        // Placeholder: keep bots from wandering; just succeed.
        return false;
    }
};

class MoveOutOfFireAction : public MovementAction
{
public:
    MoveOutOfFireAction(PlayerbotAI* ai) : MovementAction(ai, "move out of fire") {}

    bool Execute(Event) override
    {
        // Halion: if you have Combustion/Consumption, go to the edge before expecting dispel.
        if (RubySanctum::HasCombustionOrConsumption(botAI, bot) && !RubySanctum::IsNearArenaEdge(bot, RubySanctum::HALION_ARENA_EDGE_RADIUS))
        {
            botAI->Reset();
            return RubySanctum::MoveToArenaEdge(bot, this, RubySanctum::HALION_ARENA_EDGE_RADIUS);
        }

        // Halion: Meteor Strike mark avoidance.
        if (Creature* mark = RubySanctum::FindNearestMeteorMark(botAI, bot, 10.0f))
        {
            botAI->Reset();
            return RubySanctum::MoveAwayFromUnit2d(bot, this, mark, 12.0f, MovementPriority::MOVEMENT_COMBAT);
        }

        // Generic small avoidance: if standing close to a likely fire hazard NPC, step away.
        GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
        for (auto const& guid : npcs)
        {
            Unit* u = botAI->GetUnit(guid);
            if (!u || !u->IsAlive())
                continue;

            if (u->GetEntry() == RubySanctum::NPC_FIRE_CYCLONE && bot->GetExactDist2d(u) < 6.0f)
            {
                botAI->Reset();
                return MoveAway(u, 10.0f);
            }
        }

        return false;
    }
};

class RemoveDebuffAction : public Action
{
public:
    RemoveDebuffAction(PlayerbotAI* ai) : Action(ai, "remove debuff") {}

    bool Execute(Event) override
    {
        // Generic placeholder: actual dispel logic is class/spec dependent.
        return false;
    }
};

class ZarithrianTauntSwapAction : public Action
{
public:
    ZarithrianTauntSwapAction(PlayerbotAI* ai) : Action(ai, "zarithrian taunt swap") {}

    bool Execute(Event& /*event*/) override
    {
        if (!botAI->IsTank(bot))
            return false;

        Unit* boss = RubySanctum::FindBossByEntry(botAI, bot, RubySanctum::NPC_GENERAL_ZARITHRIAN);
        if (!boss || !boss->IsAlive())
            return false;

        // Only taunt if we're not already tanking.
        if (boss->GetVictim() == bot)
            return false;

        // Only taunt if our current tank stacks are high enough; use self Cleave Armor as a heuristic
        // (when we're off-tank, we usually have lower stacks and should be ready to pick up).
        // If we have stacks high too, don't taunt and instead rely on existing clear logic.
        uint8 myStacks = RubySanctum::GetAuraStacks(botAI, bot, RubySanctum::SPELL_CLEAVE_ARMOR);
        if (myStacks >= 3)
            return false;

        // Find current MT and their stacks.
        Unit* victim = boss->GetVictim();
        if (!victim)
            return false;

        Aura* victimAura = botAI->GetAura(RubySanctum::SPELL_CLEAVE_ARMOR, victim, false, false);
        uint8 victimStacks = victimAura ? uint8(victimAura->GetStackAmount()) : 0;
        if (victimStacks < 4)
            return false;

        // Use explicit spell IDs (stable across locales).
        uint32 tauntSpell = 0;
        switch (bot->getClass())
        {
            case CLASS_WARRIOR: tauntSpell = RubySanctum::SPELL_WARRIOR_TAUNT; break;
            case CLASS_PALADIN: tauntSpell = RubySanctum::SPELL_PALADIN_HAND_OF_RECKONING; break;
            case CLASS_DRUID: tauntSpell = RubySanctum::SPELL_DRUID_GROWL; break;
            case CLASS_DEATH_KNIGHT: tauntSpell = RubySanctum::SPELL_DK_DARK_COMMAND; break;
            default: break;
        }

        if (!tauntSpell)
            return false;

        if (!botAI->CanCastSpell(tauntSpell, boss))
            return false;

        botAI->CastSpell(tauntSpell, boss);
        return true;
    }
};

#endif
