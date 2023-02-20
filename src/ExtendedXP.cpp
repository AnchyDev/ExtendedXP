#include "ExtendedXP.h"

void ExtendedXPPlayer::OnGiveXP(Player* player, uint32& amount, Unit* victim)
{
    if (!sConfigMgr->GetOption<bool>("ExtendedXP.Enable", false))
    {
        return;
    }

    if (!sConfigMgr->GetOption<bool>("ExtendedXP.RareXP.Enable", false))
    {
        return;
    }

    if (!player)
    {
        return;
    }

    if (!victim)
    {
        return;
    }

    if (victim->IsPlayer())
    {
        return;
    }

    if (!victim->ToCreature())
    {
        return;
    }

    auto creature = victim->ToCreature();
    auto creatureProto = creature->GetCreatureTemplate();

    if (creatureProto->rank != CREATURE_ELITE_RARE && creatureProto->rank != CREATURE_ELITE_RAREELITE)
    {
        return;
    }

    uint32 rareExpMulti = sConfigMgr->GetOption<uint32>("ExtendedXP.RareXP.Multiplier", 3);
    amount = amount * rareExpMulti;
}

void ExtendedXPPlayer::OnAchiComplete(Player* player, AchievementEntry const* achievement)
{
    if (!sConfigMgr->GetOption<bool>("ExtendedXP.Enable", false))
    {
        return;
    }

    if (!sConfigMgr->GetOption<bool>("ExtendedXP.AchievementXP.Enable", false))
    {
        return;
    }

    if (!player)
    {
        return;
    }

    if (!achievement)
    {
        return;
    }

    float expPercent = sConfigMgr->GetOption<float>("ExtendedXP.AchievementXP.Percent", 1.5f);
    float expMultiplier = (expPercent * achievement->points) / 100;

    if (sConfigMgr->GetOption<bool>("ExtendedXP.AchievementXP.ScaleLevel", false))
    {
        auto pLevel = player->GetLevel();
        expMultiplier = ((expMultiplier * 100.0f) * (1.0f - (pLevel / 100.0f))) / 100.0f;
    }

    float xpMax = player->GetUInt32Value(PLAYER_NEXT_LEVEL_XP);
    float xpReward = xpMax * expMultiplier;

    player->GiveXP(xpReward, nullptr);
}

void SC_AddExtendedXPScripts()
{
    new ExtendedXPPlayer();
}
