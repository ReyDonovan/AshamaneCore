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

#include "ScriptMgr.h"
#include "Chat.h"
#include "DatabaseEnv.h"
#include "Group.h"
#include "GroupMgr.h"
#include "Language.h"
#include "LFGMgr.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "RBAC.h"

void GetPlayerInfo(ChatHandler* handler, Player* player)
{
    if (!player)
        return;

    ObjectGuid guid = player->GetGUID();
    lfg::LfgDungeonSet dungeons = sLFGMgr->GetSelectedDungeons(guid);

    std::string const& state = lfg::GetStateString(sLFGMgr->GetState(guid));
    handler->PSendSysMessage(LANG_LFG_PLAYER_INFO, player->GetName().c_str(),
        state.c_str(), uint8(dungeons.size()), lfg::ConcatenateDungeons(dungeons).c_str(),
        lfg::GetRolesString(sLFGMgr->GetRoles(guid)).c_str());
}

class lfg_commandscript : public CommandScript
{
public:
    lfg_commandscript() : CommandScript("lfg_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> lfgCommandTable =
        {
            {  "player", rbac::RBAC_PERM_COMMAND_LFG_PLAYER,  false, &HandleLfgPlayerInfoCommand, "" },
            {   "group", rbac::RBAC_PERM_COMMAND_LFG_GROUP,   false, &HandleLfgGroupInfoCommand,  "" },
            {   "queue", rbac::RBAC_PERM_COMMAND_LFG_QUEUE,   true,  &HandleLfgQueueInfoCommand,  "" },
            {   "clean", rbac::RBAC_PERM_COMMAND_LFG_CLEAN,   true,  &HandleLfgCleanCommand,      "" },
            { "options", rbac::RBAC_PERM_COMMAND_LFG_OPTIONS, true,  &HandleLfgOptionsCommand,    "" },
            {   "debug", rbac::RBAC_PERM_COMMAND_LFG_DEBUG,   true,  &HandleLfgDebugCommand,      "" },
            {    "join", rbac::RBAC_PERM_COMMAND_LFG,         false, &HandleLfgJoinCommand,       "" },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "lfg", rbac::RBAC_PERM_COMMAND_LFG, true, NULL, "", lfgCommandTable },
        };
        return commandTable;
    }

    static bool HandleLfgPlayerInfoCommand(ChatHandler* handler, char const* args)
    {
        Player* target = NULL;
        std::string playerName;
        if (!handler->extractPlayerTarget((char*)args, &target, NULL, &playerName))
            return false;

        GetPlayerInfo(handler, target);
        return true;
    }

    static bool HandleLfgGroupInfoCommand(ChatHandler* handler, char const* args)
    {
        Player* playerTarget;
        ObjectGuid guidTarget;
        std::string nameTarget;

        ObjectGuid parseGUID = ObjectGuid::Create<HighGuid::Player>(uint64(atoull(args)));

        if (sObjectMgr->GetPlayerNameByGUID(parseGUID, nameTarget))
        {
            playerTarget = ObjectAccessor::FindPlayer(parseGUID);
            guidTarget = parseGUID;
        }
        else if (!handler->extractPlayerTarget((char*)args, &playerTarget, &guidTarget, &nameTarget))
            return false;

        Group* groupTarget = NULL;

        if (playerTarget)
            groupTarget = playerTarget->GetGroup();
        else
        {
            CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GROUP_MEMBER);
            stmt->setUInt64(0, guidTarget.GetCounter());
            PreparedQueryResult resultGroup = CharacterDatabase.Query(stmt);
            if (resultGroup)
                groupTarget = sGroupMgr->GetGroupByDbStoreId((*resultGroup)[0].GetUInt32());
        }
        if (!groupTarget)
        {
            handler->PSendSysMessage(LANG_LFG_NOT_IN_GROUP, nameTarget.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        ObjectGuid guid = groupTarget->GetGUID();
        std::string const& state = lfg::GetStateString(sLFGMgr->GetState(guid));
        handler->PSendSysMessage(LANG_LFG_GROUP_INFO, groupTarget->isLFGGroup(),
            state.c_str(), sLFGMgr->GetDungeon(guid));

        Group::MemberSlotList const& members = groupTarget->GetMemberSlots();

        for (Group::MemberSlotList::const_iterator itr = members.begin(); itr != members.end(); ++itr)
        {
            Group::MemberSlot const& slot = *itr;
            Player* p = ObjectAccessor::FindPlayer((*itr).guid);
            if (p)
                GetPlayerInfo(handler, p);
            else
                handler->PSendSysMessage("%s is offline.", slot.name.c_str());
        }

        return true;
    }

    static bool HandleLfgOptionsCommand(ChatHandler* handler, char const* args)
    {
        int32 options = -1;
        if (char* str = strtok((char*)args, " "))
        {
            int32 tmp = atoi(str);
            if (tmp > -1)
                options = tmp;
        }

        if (options != -1)
        {
            sLFGMgr->SetOptions(options);
            handler->PSendSysMessage(LANG_LFG_OPTIONS_CHANGED);
        }
        handler->PSendSysMessage(LANG_LFG_OPTIONS, sLFGMgr->GetOptions());
        return true;
    }

    static bool HandleLfgQueueInfoCommand(ChatHandler* handler, char const* args)
    {
        handler->SendSysMessage(sLFGMgr->DumpQueueInfo(*args != '\0').c_str(), true);
        return true;
    }

    static bool HandleLfgCleanCommand(ChatHandler* handler, char const* /*args*/)
    {
        handler->PSendSysMessage(LANG_LFG_CLEAN);
        sLFGMgr->Clean();
        return true;
    }

    static bool HandleLfgDebugCommand(ChatHandler* handler, char const* /*args*/)
    {
        sLFGMgr->ToggleTesting();
        handler->PSendSysMessage(sLFGMgr->IsTesting() ? LANG_DEBUG_LFG_ON : LANG_DEBUG_LFG_OFF);
        return true;
    }

    static bool HandleLfgJoinCommand(ChatHandler* handler, char const* args)
    {
        CommandArgs cmdArgs = CommandArgs(handler, args, { CommandArgs::ARG_UINT, CommandArgs::ARG_UINT_OPTIONAL });

        if (!cmdArgs.ValidArgs())
            return false;

        lfg::LfgDungeonSet newDungeons;
        uint32 dungeon = cmdArgs.GetNextArg<uint32>();
        if (sLFGDungeonsStore.LookupEntry(dungeon))
            newDungeons.insert(dungeon);

        uint8 role = lfg::PLAYER_ROLE_DAMAGE;
        if (cmdArgs.Count() > 1)
            role = uint8(cmdArgs.GetNextArg<uint32>());

        sLFGMgr->JoinLfg(handler->getSelectedPlayerOrSelf(), role, newDungeons);
        return true;
    }
};

void AddSC_lfg_commandscript()
{
    new lfg_commandscript();
}
