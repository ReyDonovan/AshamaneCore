/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "OutdoorPvP.h"
#include "CellImpl.h"
#include "ChatPackets.h"
#include "DatabaseEnv.h"
#include "DB2Stores.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "Log.h"
#include "Map.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "OutdoorPvPMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"

class DefenseMessageBuilder
{
    public:
        DefenseMessageBuilder(uint32 zoneId, uint32 id)
            : _zoneId(zoneId), _id(id) { }

        WorldPackets::Chat::DefenseMessage* operator()(LocaleConstant locale) const
        {
            std::string text = sOutdoorPvPMgr->GetDefenseMessage(_zoneId, _id, locale);

            WorldPackets::Chat::DefenseMessage* defenseMessage = new WorldPackets::Chat::DefenseMessage();
            defenseMessage->ZoneID = _zoneId;
            defenseMessage->MessageText = text;
            return defenseMessage;
        }

    private:
        uint32 _zoneId; ///< ZoneId
        uint32 _id;     ///< BroadcastTextId
};

OPvPCapturePoint::OPvPCapturePoint(OutdoorPvP* pvp):
    m_capturePointSpawnId(), m_capturePoint(NULL), m_maxValue(0.0f), m_minValue(0.0f), m_maxSpeed(0),
    m_value(0), m_team(TEAM_NEUTRAL), m_OldState(OBJECTIVESTATE_NEUTRAL),
    m_State(OBJECTIVESTATE_NEUTRAL), m_neutralValuePct(0), m_PvP(pvp)
{ }

bool OPvPCapturePoint::HandlePlayerEnter(Player* player)
{
    if (m_capturePoint)
    {
        player->SendUpdateWorldState(m_capturePoint->GetGOInfo()->controlZone.worldState1, 1);
        player->SendUpdateWorldState(m_capturePoint->GetGOInfo()->controlZone.worldstate2, (uint32)ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f));
        player->SendUpdateWorldState(m_capturePoint->GetGOInfo()->controlZone.worldstate3, m_neutralValuePct);
    }
    return m_activePlayers[player->GetTeamId()].insert(player->GetGUID()).second;
}

void OPvPCapturePoint::HandlePlayerLeave(Player* player)
{
    if (m_capturePoint)
        player->SendUpdateWorldState(m_capturePoint->GetGOInfo()->controlZone.worldState1, 0);
    m_activePlayers[player->GetTeamId()].erase(player->GetGUID());
}

void OPvPCapturePoint::SendChangePhase()
{
    if (!m_capturePoint)
        return;

    // send this too, sometimes the slider disappears, dunno why :(
    SendUpdateWorldState(m_capturePoint->GetGOInfo()->controlZone.worldState1, 1);
    // send these updates to only the ones in this objective
    SendUpdateWorldState(m_capturePoint->GetGOInfo()->controlZone.worldstate2, (uint32)ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f));
    // send this too, sometimes it resets :S
    SendUpdateWorldState(m_capturePoint->GetGOInfo()->controlZone.worldstate3, m_neutralValuePct);
}

void OPvPCapturePoint::AddGO(uint32 type, ObjectGuid::LowType guid)
{
    GameObjectData const* data = sObjectMgr->GetGOData(guid);
    if (!data)
        return;

    m_Objects[type] = guid;
    m_ObjectTypes[m_Objects[type]] = type;
}

void OPvPCapturePoint::AddCre(uint32 type, ObjectGuid::LowType guid)
{
    CreatureData const* data = sObjectMgr->GetCreatureData(guid);
    if (!data)
        return;

    m_Creatures[type] = guid;
    m_CreatureTypes[m_Creatures[type]] = type;
}

bool OPvPCapturePoint::AddObject(uint32 type, uint32 entry, uint32 map, float x, float y, float z, float o, float rotation0, float rotation1, float rotation2, float rotation3)
{
    if (ObjectGuid::LowType guid = sObjectMgr->AddGOData(entry, map, x, y, z, o, 0, rotation0, rotation1, rotation2, rotation3))
    {
        AddGO(type, guid);
        return true;
    }

    return false;
}

bool OPvPCapturePoint::AddCreature(uint32 type, uint32 entry, uint32 map, float x, float y, float z, float o, TeamId /*teamId = TEAM_NEUTRAL*/, uint32 spawntimedelay /*= 0*/)
{
    if (ObjectGuid::LowType guid = sObjectMgr->AddCreatureData(entry, map, x, y, z, o, spawntimedelay))
    {
        AddCre(type, guid);
        return true;
    }

    return false;
}

bool OPvPCapturePoint::SetCapturePointData(uint32 entry, uint32 map, float x, float y, float z, float o, float rotation0, float rotation1, float rotation2, float rotation3)
{
    TC_LOG_DEBUG("outdoorpvp", "Creating capture point %u", entry);

    // check info existence
    GameObjectTemplate const* goinfo = sObjectMgr->GetGameObjectTemplate(entry);
    if (!goinfo || goinfo->type != GAMEOBJECT_TYPE_CONTROL_ZONE)
    {
        TC_LOG_ERROR("outdoorpvp", "OutdoorPvP: GO %u is not capture point!", entry);
        return false;
    }

    m_capturePointSpawnId = sObjectMgr->AddGOData(entry, map, x, y, z, o, 0, rotation0, rotation1, rotation2, rotation3);
    if (!m_capturePointSpawnId)
        return false;

    // get the needed values from goinfo
    m_maxValue = (float)goinfo->controlZone.maxTime;
    m_maxSpeed = m_maxValue / (goinfo->controlZone.minTime ? goinfo->controlZone.minTime : 60);
    m_neutralValuePct = goinfo->controlZone.neutralPercent;
    m_minValue = CalculatePct(m_maxValue, m_neutralValuePct);

    return true;
}

bool OPvPCapturePoint::DelCreature(uint32 type)
{
    ObjectGuid::LowType spawnId = m_Creatures[type];
    if (!spawnId)
    {
        TC_LOG_DEBUG("outdoorpvp", "opvp creature type %u was already deleted", type);
        return false;
    }

    auto bounds = m_PvP->GetMap()->GetCreatureBySpawnIdStore().equal_range(spawnId);
    for (auto itr = bounds.first; itr != bounds.second;)
    {
        Creature* c = itr->second;
        ++itr;
        // Don't save respawn time
        c->SetRespawnTime(0);
        c->RemoveCorpse();
        c->AddObjectToRemoveList();
    }

    TC_LOG_DEBUG("outdoorpvp", "deleting opvp creature type %u", type);
    // explicit removal from map
    // beats me why this is needed, but with the recent removal "cleanup" some creatures stay in the map if "properly" deleted
    // so this is a big fat workaround, if AddObjectToRemoveList and DoDelayedMovesAndRemoves worked correctly, this wouldn't be needed
    //if (Map* map = sMapMgr->FindMap(cr->GetMapId()))
    //    map->Remove(cr, false);
    // delete respawn time for this creature
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CREATURE_RESPAWN);
    stmt->setUInt64(0, spawnId);
    stmt->setUInt16(1, m_PvP->GetMap()->GetId());
    stmt->setUInt32(2, 0);  // instance id, always 0 for world maps
    CharacterDatabase.Execute(stmt);

    sObjectMgr->DeleteCreatureData(spawnId);
    m_CreatureTypes[m_Creatures[type]] = 0;
    m_Creatures[type] = 0;
    return true;
}

bool OPvPCapturePoint::DelObject(uint32 type)
{
    if (!m_Objects[type])
        return false;

    ObjectGuid::LowType spawnId = m_Objects[type];
    auto bounds = m_PvP->GetMap()->GetGameObjectBySpawnIdStore().equal_range(spawnId);
    for (auto itr = bounds.first; itr != bounds.second;)
    {
        GameObject* go = itr->second;
        ++itr;
        // Don't save respawn time
        go->SetRespawnTime(0);
        go->Delete();
    }

    sObjectMgr->DeleteGOData(spawnId);
    m_ObjectTypes[m_Objects[type]] = 0;
    m_Objects[type] = 0;
    return true;
}

bool OPvPCapturePoint::DelCapturePoint()
{
    sObjectMgr->DeleteGOData(m_capturePointSpawnId);
    m_capturePointSpawnId = 0;

    if (m_capturePoint)
    {
        m_capturePoint->SetRespawnTime(0);                                 // not save respawn time
        m_capturePoint->Delete();
    }

    return true;
}

void OPvPCapturePoint::DeleteSpawns()
{
    for (std::map<uint32, ObjectGuid::LowType>::iterator i = m_Objects.begin(); i != m_Objects.end(); ++i)
        DelObject(i->first);
    for (std::map<uint32, ObjectGuid::LowType>::iterator i = m_Creatures.begin(); i != m_Creatures.end(); ++i)
        DelCreature(i->first);
    DelCapturePoint();
}

void OutdoorPvP::DeleteSpawns()
{
    for (OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
    {
        itr->second->DeleteSpawns();
        delete itr->second;
    }
    m_capturePoints.clear();
}

OutdoorPvP::OutdoorPvP() : m_TypeId(0), m_sendUpdate(true), m_map(nullptr)
{
    _scriptType = ZONE_SCRIPT_TYPE_OUTDOORPVP;
}

OutdoorPvP::~OutdoorPvP()
{
    DeleteSpawns();
}

void OutdoorPvP::HandlePlayerEnterZone(Player* player, uint32 /*zone*/)
{
    m_players[player->GetTeamId()].insert(player->GetGUID());
}

void OutdoorPvP::HandlePlayerLeaveZone(Player* player, uint32 /*zone*/)
{
    // inform the objectives of the leaving
    for (OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        itr->second->HandlePlayerLeave(player);
    // remove the world state information from the player (we can't keep everyone up to date, so leave out those who are not in the concerning zones)
    if (!player->GetSession()->PlayerLogout())
        SendRemoveWorldStates(player);
    m_players[player->GetTeamId()].erase(player->GetGUID());
    TC_LOG_DEBUG("outdoorpvp", "Player %s left an outdoorpvp zone", player->GetName().c_str());
}

void OutdoorPvP::HandlePlayerResurrects(Player* /*player*/, uint32 /*zone*/) { }

bool OutdoorPvP::Update(uint32 diff)
{
    bool objective_changed = false;
    for (OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
    {
        if (itr->second->Update(diff))
            objective_changed = true;
    }
    return objective_changed;
}

bool OPvPCapturePoint::Update(uint32 diff)
{
    if (!m_capturePoint)
        return false;

    float radius = (float)m_capturePoint->GetGOInfo()->controlZone.radius;

    for (uint32 team = 0; team < 2; ++team)
    {
        for (GuidSet::iterator itr = m_activePlayers[team].begin(); itr != m_activePlayers[team].end();)
        {
            ObjectGuid playerGuid = *itr;
            ++itr;

            if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                if (!m_capturePoint->IsWithinDistInMap(player, radius) || !player->IsOutdoorPvPActive())
                    HandlePlayerLeave(player);
        }
    }

    std::list<Player*> players;
    Trinity::AnyPlayerInObjectRangeCheck checker(m_capturePoint, radius);
    Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(m_capturePoint, players, checker);
    Cell::VisitWorldObjects(m_capturePoint, searcher, radius);

    for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
    {
        Player* const player = *itr;
        if (player->IsOutdoorPvPActive())
        {
            if (m_activePlayers[player->GetTeamId()].insert(player->GetGUID()).second)
                HandlePlayerEnter(*itr);
        }
    }

    // get the difference of numbers
    float fact_diff = ((float)m_activePlayers[0].size() - (float)m_activePlayers[1].size()) * diff / OUTDOORPVP_OBJECTIVE_UPDATE_INTERVAL;
    if (!fact_diff)
        return false;

    uint32 Challenger = 0;
    float maxDiff = m_maxSpeed * diff;

    if (fact_diff < 0)
    {
        // horde is in majority, but it's already horde-controlled -> no change
        if (m_State == OBJECTIVESTATE_HORDE && m_value <= -m_maxValue)
            return false;

        if (fact_diff < -maxDiff)
            fact_diff = -maxDiff;

        Challenger = HORDE;
    }
    else
    {
        // ally is in majority, but it's already ally-controlled -> no change
        if (m_State == OBJECTIVESTATE_ALLIANCE && m_value >= m_maxValue)
            return false;

        if (fact_diff > maxDiff)
            fact_diff = maxDiff;

        Challenger = ALLIANCE;
    }

    float oldValue = m_value;
    TeamId oldTeam = m_team;

    m_OldState = m_State;

    m_value += fact_diff;

    if (m_value < -m_minValue) // red
    {
        if (m_value < -m_maxValue)
            m_value = -m_maxValue;
        m_State = OBJECTIVESTATE_HORDE;
        m_team = TEAM_HORDE;
    }
    else if (m_value > m_minValue) // blue
    {
        if (m_value > m_maxValue)
            m_value = m_maxValue;
        m_State = OBJECTIVESTATE_ALLIANCE;
        m_team = TEAM_ALLIANCE;
    }
    else if (oldValue * m_value <= 0) // grey, go through mid point
    {
        // if challenger is ally, then n->a challenge
        if (Challenger == ALLIANCE)
            m_State = OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE;
        // if challenger is horde, then n->h challenge
        else if (Challenger == HORDE)
            m_State = OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE;
        m_team = TEAM_NEUTRAL;
    }
    else // grey, did not go through mid point
    {
        // old phase and current are on the same side, so one team challenges the other
        if (Challenger == ALLIANCE && (m_OldState == OBJECTIVESTATE_HORDE || m_OldState == OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE))
            m_State = OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE;
        else if (Challenger == HORDE && (m_OldState == OBJECTIVESTATE_ALLIANCE || m_OldState == OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE))
            m_State = OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE;
        m_team = TEAM_NEUTRAL;
    }

    if (m_value != oldValue)
        SendChangePhase();

    if (m_OldState != m_State)
    {
        //TC_LOG_ERROR("outdoorpvp", "%u->%u", m_OldState, m_State);
        if (oldTeam != m_team)
            ChangeTeam(oldTeam);
        ChangeState();
        return true;
    }

    return false;
}

void OutdoorPvP::SendUpdateWorldState(uint32 field, uint32 value)
{
    if (m_sendUpdate)
        for (int i = 0; i < 2; ++i)
            for (GuidSet::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
                if (Player* const player = ObjectAccessor::FindPlayer(*itr))
                    player->SendUpdateWorldState(field, value);
}

void OPvPCapturePoint::SendUpdateWorldState(uint32 field, uint32 value)
{
    for (uint32 team = 0; team < 2; ++team)
    {
        // send to all players present in the area
        for (GuidSet::iterator itr = m_activePlayers[team].begin(); itr != m_activePlayers[team].end(); ++itr)
            if (Player* const player = ObjectAccessor::FindPlayer(*itr))
                player->SendUpdateWorldState(field, value);
    }
}

void OPvPCapturePoint::SendObjectiveComplete(uint32 id, ObjectGuid guid)
{
    uint32 team;
    switch (m_State)
    {
    case OBJECTIVESTATE_ALLIANCE:
        team = 0;
        break;
    case OBJECTIVESTATE_HORDE:
        team = 1;
        break;
    default:
        return;
    }

    // send to all players present in the area
    for (GuidSet::iterator itr = m_activePlayers[team].begin(); itr != m_activePlayers[team].end(); ++itr)
        if (Player* const player = ObjectAccessor::FindPlayer(*itr))
            player->KilledMonsterCredit(id, guid);
}

void OutdoorPvP::HandleKill(Player* killer, Unit* killed)
{
    if (Group* group = killer->GetGroup())
    {
        for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* groupGuy = itr->GetSource();

            if (!groupGuy)
                continue;

            // skip if too far away
            if (!groupGuy->IsAtGroupRewardDistance(killed))
                continue;

            // creature kills must be notified, even if not inside objective / not outdoor pvp active
            // player kills only count if active and inside objective
            if ((groupGuy->IsOutdoorPvPActive() && IsInsideObjective(groupGuy)) || killed->GetTypeId() == TYPEID_UNIT)
                HandleKillImpl(groupGuy, killed);
        }
    }
    else
    {
        // creature kills must be notified, even if not inside objective / not outdoor pvp active
        if ((killer->IsOutdoorPvPActive() && IsInsideObjective(killer)) || killed->GetTypeId() == TYPEID_UNIT)
            HandleKillImpl(killer, killed);
    }
}

bool OutdoorPvP::IsInsideObjective(Player* player) const
{
    for (OPvPCapturePointMap::const_iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        if (itr->second->IsInsideObjective(player))
            return true;

    return false;
}

bool OPvPCapturePoint::IsInsideObjective(Player* player) const
{
    GuidSet const &plSet = m_activePlayers[player->GetTeamId()];
    return plSet.find(player->GetGUID()) != plSet.end();
}

bool OutdoorPvP::HandleCustomSpell(Player* player, uint32 spellId, GameObject* go)
{
    for (OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        if (itr->second->HandleCustomSpell(player, spellId, go))
            return true;

    return false;
}

bool OPvPCapturePoint::HandleCustomSpell(Player* player, uint32 /*spellId*/, GameObject* /*go*/)
{
    if (!player->IsOutdoorPvPActive())
        return false;
    return true;
}

bool OutdoorPvP::HandleOpenGo(Player* player, GameObject* go)
{
    for (OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        if (itr->second->HandleOpenGo(player, go) >= 0)
            return true;

    return false;
}

bool OutdoorPvP::HandleGossipOption(Player* player, Creature* creature, uint32 id)
{
    for (OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        if (itr->second->HandleGossipOption(player, creature, id))
            return true;

    return false;
}

bool OutdoorPvP::CanTalkTo(Player* player, Creature* c, GossipMenuItems const& gso)
{
    for (OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        if (itr->second->CanTalkTo(player, c, gso))
            return true;

    return false;
}

bool OutdoorPvP::HandleDropFlag(Player* player, uint32 id)
{
    for (OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        if (itr->second->HandleDropFlag(player, id))
            return true;

    return false;
}

int32 OPvPCapturePoint::HandleOpenGo(Player* /*player*/, GameObject* go)
{
    std::map<ObjectGuid::LowType, uint32>::iterator itr = m_ObjectTypes.find(go->GetSpawnId());
    if (itr != m_ObjectTypes.end())
        return itr->second;

    return -1;
}

void OutdoorPvP::BroadcastPacket(WorldPacket const* data) const
{
    // This is faster than sWorld->SendZoneMessage
    for (uint32 team = 0; team < 2; ++team)
        for (GuidSet::const_iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
            if (Player* const player = ObjectAccessor::FindPlayer(*itr))
                player->SendDirectMessage(data);
}

void OutdoorPvP::AddCapturePoint(OPvPCapturePoint* cp)
{
    OPvPCapturePointMap::iterator i = m_capturePoints.find(cp->m_capturePointSpawnId);
    if (i != m_capturePoints.end())
    {
        TC_LOG_ERROR("outdoorpvp", "OutdoorPvP::AddCapturePoint: CapturePoint " UI64FMTD " already exists!", cp->m_capturePointSpawnId);
        delete i->second;
    }
    m_capturePoints[cp->m_capturePointSpawnId] = cp;
}

OPvPCapturePoint* OutdoorPvP::GetCapturePoint(ObjectGuid::LowType guid) const
{
    OutdoorPvP::OPvPCapturePointMap::const_iterator itr = m_capturePoints.find(guid);
    if (itr != m_capturePoints.end())
        return itr->second;
    return nullptr;
}

void OutdoorPvP::RegisterZone(uint32 zoneId)
{
    sOutdoorPvPMgr->AddZone(zoneId, this);
}

bool OutdoorPvP::HasPlayer(Player const* player) const
{
    GuidSet const &plSet = m_players[player->GetTeamId()];
    return plSet.find(player->GetGUID()) != plSet.end();
}

void OutdoorPvP::TeamCastSpell(TeamId team, int32 spellId)
{
    if (spellId > 0)
    {
        for (GuidSet::iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
            if (Player* const player = ObjectAccessor::FindPlayer(*itr))
                player->CastSpell(player, (uint32)spellId, true);
    }
    else
    {
        for (GuidSet::iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
            if (Player* const player = ObjectAccessor::FindPlayer(*itr))
                player->RemoveAura((uint32)-spellId); // by stack?
    }
}

void OutdoorPvP::TeamApplyBuff(TeamId team, uint32 spellId, uint32 spellId2)
{
    TeamCastSpell(team, spellId);
    TeamCastSpell(OTHER_TEAM(team), spellId2 ? -(int32)spellId2 : -(int32)spellId);
}

void OutdoorPvP::OnGameObjectCreate(GameObject* go)
{
    if (go->GetGoType() != GAMEOBJECT_TYPE_CONTROL_ZONE)
        return;

    if (OPvPCapturePoint *cp = GetCapturePoint(go->GetSpawnId()))
        cp->m_capturePoint = go;
}

void OutdoorPvP::OnGameObjectRemove(GameObject* go)
{
    if (go->GetGoType() != GAMEOBJECT_TYPE_CONTROL_ZONE)
        return;

    if (OPvPCapturePoint *cp = GetCapturePoint(go->GetSpawnId()))
        cp->m_capturePoint = NULL;
}

void OutdoorPvP::SendDefenseMessage(uint32 zoneId, uint32 id)
{
    DefenseMessageBuilder builder(zoneId, id);
    Trinity::LocalizedPacketDo<DefenseMessageBuilder> localizer(builder);
    BroadcastWorker(localizer, zoneId);
}

template<class Worker>
void OutdoorPvP::BroadcastWorker(Worker& _worker, uint32 zoneId)
{
    for (uint32 i = 0; i < BG_TEAMS_COUNT; ++i)
        for (GuidSet::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(*itr))
                if (player->GetZoneId() == zoneId)
                    _worker(player);
}

void OutdoorPvP::SetMapFromZone(uint32 zone)
{
    AreaTableEntry const* areaTable = sAreaTableStore.LookupEntry(zone);
    ASSERT(areaTable);
    Map* map = sMapMgr->CreateBaseMap(areaTable->ContinentID);
    ASSERT(!map->Instanceable());
    m_map = map;
}
