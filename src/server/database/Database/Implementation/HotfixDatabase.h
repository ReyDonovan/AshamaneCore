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

// DO NOT EDIT!
// Autogenerated from DB2Structure.h

#ifndef _HOTFIXDATABASE_H
#define _HOTFIXDATABASE_H

#include "MySQLConnection.h"

enum HotfixDatabaseStatements : uint32
{
    /*  Naming standard for defines:
        {DB}_{SEL/INS/UPD/DEL/REP}_{Summary of data changed}
        When updating more than one field, consider looking at the calling function
        name for a suiting suffix.
    */

    HOTFIX_SEL_ACHIEVEMENT,
    HOTFIX_SEL_ACHIEVEMENT_LOCALE,

    HOTFIX_SEL_ADVENTURE_JOURNAL,
    HOTFIX_SEL_ADVENTURE_JOURNAL_LOCALE,

    HOTFIX_SEL_ANIM_KIT,

    HOTFIX_SEL_AREA_GROUP_MEMBER,

    HOTFIX_SEL_AREA_TABLE,
    HOTFIX_SEL_AREA_TABLE_LOCALE,

    HOTFIX_SEL_AREA_TRIGGER,

    HOTFIX_SEL_ARMOR_LOCATION,

    HOTFIX_SEL_ARTIFACT,
    HOTFIX_SEL_ARTIFACT_LOCALE,

    HOTFIX_SEL_ARTIFACT_APPEARANCE,
    HOTFIX_SEL_ARTIFACT_APPEARANCE_LOCALE,

    HOTFIX_SEL_ARTIFACT_APPEARANCE_SET,
    HOTFIX_SEL_ARTIFACT_APPEARANCE_SET_LOCALE,

    HOTFIX_SEL_ARTIFACT_CATEGORY,

    HOTFIX_SEL_ARTIFACT_POWER,

    HOTFIX_SEL_ARTIFACT_POWER_LINK,

    HOTFIX_SEL_ARTIFACT_POWER_PICKER,

    HOTFIX_SEL_ARTIFACT_POWER_RANK,

    HOTFIX_SEL_ARTIFACT_QUEST_XP,

    HOTFIX_SEL_ARTIFACT_TIER,

    HOTFIX_SEL_ARTIFACT_UNLOCK,

    HOTFIX_SEL_AUCTION_HOUSE,
    HOTFIX_SEL_AUCTION_HOUSE_LOCALE,

    HOTFIX_SEL_BANK_BAG_SLOT_PRICES,

    HOTFIX_SEL_BANNED_ADDONS,

    HOTFIX_SEL_BARBER_SHOP_STYLE,
    HOTFIX_SEL_BARBER_SHOP_STYLE_LOCALE,

    HOTFIX_SEL_BATTLE_PET_ABILITY,
    HOTFIX_SEL_BATTLE_PET_ABILITY_LOCALE,

    HOTFIX_SEL_BATTLE_PET_ABILITY_EFFECT,

    HOTFIX_SEL_BATTLE_PET_ABILITY_STATE,

    HOTFIX_SEL_BATTLE_PET_ABILITY_TURN,

    HOTFIX_SEL_BATTLE_PET_BREED_QUALITY,

    HOTFIX_SEL_BATTLE_PET_BREED_STATE,

    HOTFIX_SEL_BATTLE_PET_SPECIES,
    HOTFIX_SEL_BATTLE_PET_SPECIES_LOCALE,

    HOTFIX_SEL_BATTLE_PET_SPECIES_STATE,

    HOTFIX_SEL_BATTLE_PET_SPECIES_X_ABILITY,

    HOTFIX_SEL_BATTLEMASTER_LIST,
    HOTFIX_SEL_BATTLEMASTER_LIST_LOCALE,

    HOTFIX_SEL_BROADCAST_TEXT,
    HOTFIX_SEL_BROADCAST_TEXT_LOCALE,

    HOTFIX_SEL_CFG_REGIONS,

    HOTFIX_SEL_CHARACTER_FACIAL_HAIR_STYLES,

    HOTFIX_SEL_CHAR_BASE_SECTION,

    HOTFIX_SEL_CHAR_SECTIONS,

    HOTFIX_SEL_CHAR_START_OUTFIT,

    HOTFIX_SEL_CHAR_TITLES,
    HOTFIX_SEL_CHAR_TITLES_LOCALE,

    HOTFIX_SEL_CHAT_CHANNELS,
    HOTFIX_SEL_CHAT_CHANNELS_LOCALE,

    HOTFIX_SEL_CHR_CLASSES,
    HOTFIX_SEL_CHR_CLASSES_LOCALE,

    HOTFIX_SEL_CHR_CLASSES_X_POWER_TYPES,

    HOTFIX_SEL_CHR_RACES,
    HOTFIX_SEL_CHR_RACES_LOCALE,

    HOTFIX_SEL_CHR_SPECIALIZATION,
    HOTFIX_SEL_CHR_SPECIALIZATION_LOCALE,

    HOTFIX_SEL_CINEMATIC_CAMERA,

    HOTFIX_SEL_CINEMATIC_SEQUENCES,

    HOTFIX_SEL_CONVERSATION_LINE,

    HOTFIX_SEL_CREATURE_DIFFICULTY,

    HOTFIX_SEL_CREATURE_DISPLAY_INFO,

    HOTFIX_SEL_CREATURE_DISPLAY_INFO_EXTRA,

    HOTFIX_SEL_CREATURE_FAMILY,
    HOTFIX_SEL_CREATURE_FAMILY_LOCALE,

    HOTFIX_SEL_CREATURE_MODEL_DATA,

    HOTFIX_SEL_CREATURE_TYPE,
    HOTFIX_SEL_CREATURE_TYPE_LOCALE,

    HOTFIX_SEL_CRITERIA,

    HOTFIX_SEL_CRITERIA_TREE,
    HOTFIX_SEL_CRITERIA_TREE_LOCALE,

    HOTFIX_SEL_CURRENCY_TYPES,
    HOTFIX_SEL_CURRENCY_TYPES_LOCALE,

    HOTFIX_SEL_CURVE,

    HOTFIX_SEL_CURVE_POINT,

    HOTFIX_SEL_DESTRUCTIBLE_MODEL_DATA,

    HOTFIX_SEL_DIFFICULTY,
    HOTFIX_SEL_DIFFICULTY_LOCALE,

    HOTFIX_SEL_DUNGEON_ENCOUNTER,
    HOTFIX_SEL_DUNGEON_ENCOUNTER_LOCALE,

    HOTFIX_SEL_DURABILITY_COSTS,

    HOTFIX_SEL_DURABILITY_QUALITY,

    HOTFIX_SEL_EMOTES,

    HOTFIX_SEL_EMOTES_TEXT,

    HOTFIX_SEL_EMOTES_TEXT_SOUND,

    HOTFIX_SEL_FACTION,
    HOTFIX_SEL_FACTION_LOCALE,

    HOTFIX_SEL_FACTION_TEMPLATE,

    HOTFIX_SEL_GAMEOBJECT_DISPLAY_INFO,

    HOTFIX_SEL_GAMEOBJECTS,
    HOTFIX_SEL_GAMEOBJECTS_LOCALE,

    HOTFIX_SEL_GARR_ABILITY,
    HOTFIX_SEL_GARR_ABILITY_LOCALE,

    HOTFIX_SEL_GARR_BUILDING,
    HOTFIX_SEL_GARR_BUILDING_LOCALE,

    HOTFIX_SEL_GARR_BUILDING_PLOT_INST,

    HOTFIX_SEL_GARR_CLASS_SPEC,
    HOTFIX_SEL_GARR_CLASS_SPEC_LOCALE,

    HOTFIX_SEL_GARR_FOLLOWER,
    HOTFIX_SEL_GARR_FOLLOWER_LOCALE,

    HOTFIX_SEL_GARR_FOLLOWER_LEVEL_XP,

    HOTFIX_SEL_GARR_FOLLOWER_QUALITY,

    HOTFIX_SEL_GARR_FOLLOWER_TYPE,

    HOTFIX_SEL_GARR_FOLLOWER_X_ABILITY,

    HOTFIX_SEL_GARR_MISSION,
    HOTFIX_SEL_GARR_MISSION_LOCALE,

    HOTFIX_SEL_GARR_PLOT,
    HOTFIX_SEL_GARR_PLOT_LOCALE,

    HOTFIX_SEL_GARR_PLOT_BUILDING,

    HOTFIX_SEL_GARR_PLOT_INSTANCE,

    HOTFIX_SEL_GARR_SITE_LEVEL,

    HOTFIX_SEL_GARR_SITE_LEVEL_PLOT_INST,

    HOTFIX_SEL_GEM_PROPERTIES,

    HOTFIX_SEL_GLOBAL_STRINGS,
    HOTFIX_SEL_GLOBAL_STRINGS_LOCALE,

    HOTFIX_SEL_GLYPH_BINDABLE_SPELL,

    HOTFIX_SEL_GLYPH_PROPERTIES,

    HOTFIX_SEL_GLYPH_REQUIRED_SPEC,

    HOTFIX_SEL_GUILD_COLOR_BACKGROUND,

    HOTFIX_SEL_GUILD_COLOR_BORDER,

    HOTFIX_SEL_GUILD_COLOR_EMBLEM,

    HOTFIX_SEL_GUILD_PERK_SPELLS,

    HOTFIX_SEL_HEIRLOOM,
    HOTFIX_SEL_HEIRLOOM_LOCALE,

    HOTFIX_SEL_HOLIDAYS,

    HOTFIX_SEL_IMPORT_PRICE_ARMOR,

    HOTFIX_SEL_IMPORT_PRICE_QUALITY,

    HOTFIX_SEL_IMPORT_PRICE_SHIELD,

    HOTFIX_SEL_IMPORT_PRICE_WEAPON,

    HOTFIX_SEL_ITEM,

    HOTFIX_SEL_ITEM_APPEARANCE,

    HOTFIX_SEL_ITEM_ARMOR_QUALITY,

    HOTFIX_SEL_ITEM_ARMOR_SHIELD,

    HOTFIX_SEL_ITEM_ARMOR_TOTAL,

    HOTFIX_SEL_ITEM_BAG_FAMILY,
    HOTFIX_SEL_ITEM_BAG_FAMILY_LOCALE,

    HOTFIX_SEL_ITEM_BONUS,

    HOTFIX_SEL_ITEM_BONUS_LIST_LEVEL_DELTA,

    HOTFIX_SEL_ITEM_BONUS_TREE_NODE,

    HOTFIX_SEL_ITEM_CHILD_EQUIPMENT,

    HOTFIX_SEL_ITEM_CLASS,
    HOTFIX_SEL_ITEM_CLASS_LOCALE,

    HOTFIX_SEL_ITEM_CURRENCY_COST,

    HOTFIX_SEL_ITEM_DAMAGE_AMMO,

    HOTFIX_SEL_ITEM_DAMAGE_ONE_HAND,

    HOTFIX_SEL_ITEM_DAMAGE_ONE_HAND_CASTER,

    HOTFIX_SEL_ITEM_DAMAGE_TWO_HAND,

    HOTFIX_SEL_ITEM_DAMAGE_TWO_HAND_CASTER,

    HOTFIX_SEL_ITEM_DISENCHANT_LOOT,

    HOTFIX_SEL_ITEM_EFFECT,

    HOTFIX_SEL_ITEM_EXTENDED_COST,

    HOTFIX_SEL_ITEM_LEVEL_SELECTOR,

    HOTFIX_SEL_ITEM_LEVEL_SELECTOR_QUALITY,

    HOTFIX_SEL_ITEM_LEVEL_SELECTOR_QUALITY_SET,

    HOTFIX_SEL_ITEM_LIMIT_CATEGORY,
    HOTFIX_SEL_ITEM_LIMIT_CATEGORY_LOCALE,

    HOTFIX_SEL_ITEM_LIMIT_CATEGORY_CONDITION,

    HOTFIX_SEL_ITEM_MODIFIED_APPEARANCE,

    HOTFIX_SEL_ITEM_PRICE_BASE,

    HOTFIX_SEL_ITEM_RANDOM_PROPERTIES,
    HOTFIX_SEL_ITEM_RANDOM_PROPERTIES_LOCALE,

    HOTFIX_SEL_ITEM_RANDOM_SUFFIX,
    HOTFIX_SEL_ITEM_RANDOM_SUFFIX_LOCALE,

    HOTFIX_SEL_ITEM_SEARCH_NAME,
    HOTFIX_SEL_ITEM_SEARCH_NAME_LOCALE,

    HOTFIX_SEL_ITEM_SET,
    HOTFIX_SEL_ITEM_SET_LOCALE,

    HOTFIX_SEL_ITEM_SET_SPELL,

    HOTFIX_SEL_ITEM_SPARSE,
    HOTFIX_SEL_ITEM_SPARSE_LOCALE,

    HOTFIX_SEL_ITEM_SPEC,

    HOTFIX_SEL_ITEM_SPEC_OVERRIDE,

    HOTFIX_SEL_ITEM_UPGRADE,

    HOTFIX_SEL_ITEM_X_BONUS_TREE,

    HOTFIX_SEL_JOURNAL_ENCOUNTER,
    HOTFIX_SEL_JOURNAL_ENCOUNTER_LOCALE,

    HOTFIX_SEL_JOURNAL_ENCOUNTER_ITEM,

    HOTFIX_SEL_JOURNAL_INSTANCE,
    HOTFIX_SEL_JOURNAL_INSTANCE_LOCALE,

    HOTFIX_SEL_KEYCHAIN,

    HOTFIX_SEL_LFG_DUNGEONS,
    HOTFIX_SEL_LFG_DUNGEONS_LOCALE,

    HOTFIX_SEL_LIGHT,

    HOTFIX_SEL_LIQUID_TYPE,

    HOTFIX_SEL_LOCK,

    HOTFIX_SEL_MAIL_TEMPLATE,
    HOTFIX_SEL_MAIL_TEMPLATE_LOCALE,

    HOTFIX_SEL_MAP,
    HOTFIX_SEL_MAP_LOCALE,

    HOTFIX_SEL_MAP_CHALLENGE_MODE,
    HOTFIX_SEL_MAP_CHALLENGE_MODE_LOCALE,

    HOTFIX_SEL_MAP_DIFFICULTY,
    HOTFIX_SEL_MAP_DIFFICULTY_LOCALE,

    HOTFIX_SEL_MODIFIER_TREE,

    HOTFIX_SEL_MOUNT,
    HOTFIX_SEL_MOUNT_LOCALE,

    HOTFIX_SEL_MOUNT_CAPABILITY,

    HOTFIX_SEL_MOUNT_TYPE_X_CAPABILITY,

    HOTFIX_SEL_MOUNT_X_DISPLAY,

    HOTFIX_SEL_MOVIE,

    HOTFIX_SEL_NAME_GEN,

    HOTFIX_SEL_NAMES_PROFANITY,

    HOTFIX_SEL_NAMES_RESERVED,

    HOTFIX_SEL_NAMES_RESERVED_LOCALE,

    HOTFIX_SEL_OVERRIDE_SPELL_DATA,

    HOTFIX_SEL_PHASE,

    HOTFIX_SEL_PHASE_X_PHASE_GROUP,

    HOTFIX_SEL_PLAYER_CONDITION,
    HOTFIX_SEL_PLAYER_CONDITION_LOCALE,

    HOTFIX_SEL_POWER_DISPLAY,

    HOTFIX_SEL_POWER_TYPE,

    HOTFIX_SEL_PRESTIGE_LEVEL_INFO,
    HOTFIX_SEL_PRESTIGE_LEVEL_INFO_LOCALE,

    HOTFIX_SEL_PVP_DIFFICULTY,

    HOTFIX_SEL_PVP_ITEM,

    HOTFIX_SEL_PVP_REWARD,

    HOTFIX_SEL_PVP_TALENT,
    HOTFIX_SEL_PVP_TALENT_LOCALE,

    HOTFIX_SEL_PVP_TALENT_UNLOCK,

    HOTFIX_SEL_QUEST_FACTION_REWARD,

    HOTFIX_SEL_QUEST_MONEY_REWARD,
 
    HOTFIX_SEL_QUEST_V2_CLI_TASK,

    HOTFIX_SEL_QUEST_PACKAGE_ITEM,

    HOTFIX_SEL_QUEST_SORT,
    HOTFIX_SEL_QUEST_SORT_LOCALE,

    HOTFIX_SEL_QUEST_V2,

    HOTFIX_SEL_QUEST_XP,

    HOTFIX_SEL_QUEST_POI_BLOB,

    HOTFIX_SEL_QUEST_POI_POINT,

    HOTFIX_SEL_RAND_PROP_POINTS,

    HOTFIX_SEL_RESEARCH_BRANCH,
    HOTFIX_SEL_RESEARCH_BRANCH_LOCALE,

    HOTFIX_SEL_RESEARCH_PROJECT,
    HOTFIX_SEL_RESEARCH_PROJECT_LOCALE,

    HOTFIX_SEL_RESEARCH_SITE,
    HOTFIX_SEL_RESEARCH_SITE_LOCALE,

    HOTFIX_SEL_REWARD_PACK,

    HOTFIX_SEL_REWARD_PACK_X_CURRENCY_TYPE,

    HOTFIX_SEL_REWARD_PACK_X_ITEM,

    HOTFIX_SEL_RULESET_ITEM_UPGRADE,

    HOTFIX_SEL_SANDBOX_SCALING,

    HOTFIX_SEL_SCALING_STAT_DISTRIBUTION,

    HOTFIX_SEL_SCENARIO,
    HOTFIX_SEL_SCENARIO_LOCALE,

    HOTFIX_SEL_SCENARIO_STEP,
    HOTFIX_SEL_SCENARIO_STEP_LOCALE,

    HOTFIX_SEL_SCENE_SCRIPT,

    HOTFIX_SEL_SCENE_SCRIPT_GLOBAL_TEXT,

    HOTFIX_SEL_SCENE_SCRIPT_PACKAGE,

    HOTFIX_SEL_SCENE_SCRIPT_TEXT,

    HOTFIX_SEL_SKILL_LINE,
    HOTFIX_SEL_SKILL_LINE_LOCALE,

    HOTFIX_SEL_SKILL_LINE_ABILITY,

    HOTFIX_SEL_SKILL_RACE_CLASS_INFO,

    HOTFIX_SEL_SOUND_KIT,

    HOTFIX_SEL_SPECIALIZATION_SPELLS,
    HOTFIX_SEL_SPECIALIZATION_SPELLS_LOCALE,

    HOTFIX_SEL_SPELL,
    HOTFIX_SEL_SPELL_LOCALE,

    HOTFIX_SEL_SPELL_AURA_OPTIONS,

    HOTFIX_SEL_SPELL_AURA_RESTRICTIONS,

    HOTFIX_SEL_SPELL_CAST_TIMES,

    HOTFIX_SEL_SPELL_CASTING_REQUIREMENTS,

    HOTFIX_SEL_SPELL_CATEGORIES,

    HOTFIX_SEL_SPELL_CATEGORY,
    HOTFIX_SEL_SPELL_CATEGORY_LOCALE,

    HOTFIX_SEL_SPELL_CLASS_OPTIONS,

    HOTFIX_SEL_SPELL_COOLDOWNS,

    HOTFIX_SEL_SPELL_DURATION,

    HOTFIX_SEL_SPELL_EFFECT,

    HOTFIX_SEL_SPELL_EQUIPPED_ITEMS,

    HOTFIX_SEL_SPELL_FOCUS_OBJECT,
    HOTFIX_SEL_SPELL_FOCUS_OBJECT_LOCALE,

    HOTFIX_SEL_SPELL_INTERRUPTS,

    HOTFIX_SEL_SPELL_ITEM_ENCHANTMENT,
    HOTFIX_SEL_SPELL_ITEM_ENCHANTMENT_LOCALE,

    HOTFIX_SEL_SPELL_ITEM_ENCHANTMENT_CONDITION,

    HOTFIX_SEL_SPELL_LEARN_SPELL,

    HOTFIX_SEL_SPELL_LEVELS,

    HOTFIX_SEL_SPELL_MISC,

    HOTFIX_SEL_SPELL_POWER,

    HOTFIX_SEL_SPELL_POWER_DIFFICULTY,

    HOTFIX_SEL_SPELL_PROCS_PER_MINUTE,

    HOTFIX_SEL_SPELL_PROCS_PER_MINUTE_MOD,

    HOTFIX_SEL_SPELL_RADIUS,

    HOTFIX_SEL_SPELL_RANGE,
    HOTFIX_SEL_SPELL_RANGE_LOCALE,

    HOTFIX_SEL_SPELL_REAGENTS,

    HOTFIX_SEL_SPELL_SCALING,

    HOTFIX_SEL_SPELL_SHAPESHIFT,

    HOTFIX_SEL_SPELL_SHAPESHIFT_FORM,
    HOTFIX_SEL_SPELL_SHAPESHIFT_FORM_LOCALE,

    HOTFIX_SEL_SPELL_TARGET_RESTRICTIONS,

    HOTFIX_SEL_SPELL_TOTEMS,

    HOTFIX_SEL_SPELL_X_SPELL_VISUAL,

    HOTFIX_SEL_SUMMON_PROPERTIES,

    HOTFIX_SEL_TACT_KEY,

    HOTFIX_SEL_TALENT,
    HOTFIX_SEL_TALENT_LOCALE,

    HOTFIX_SEL_TAXI_NODES,
    HOTFIX_SEL_TAXI_NODES_LOCALE,

    HOTFIX_SEL_TAXI_PATH,

    HOTFIX_SEL_TAXI_PATH_NODE,

    HOTFIX_SEL_TOTEM_CATEGORY,
    HOTFIX_SEL_TOTEM_CATEGORY_LOCALE,

    HOTFIX_SEL_TOY,
    HOTFIX_SEL_TOY_LOCALE,

    HOTFIX_SEL_TRANSMOG_HOLIDAY,

    HOTFIX_SEL_TRANSMOG_SET,
    HOTFIX_SEL_TRANSMOG_SET_LOCALE,

    HOTFIX_SEL_TRANSMOG_SET_GROUP,
    HOTFIX_SEL_TRANSMOG_SET_GROUP_LOCALE,

    HOTFIX_SEL_TRANSMOG_SET_ITEM,

    HOTFIX_SEL_TRANSPORT_ANIMATION,

    HOTFIX_SEL_TRANSPORT_ROTATION,

    HOTFIX_SEL_UNIT_POWER_BAR,
    HOTFIX_SEL_UNIT_POWER_BAR_LOCALE,

    HOTFIX_SEL_VEHICLE,

    HOTFIX_SEL_VEHICLE_SEAT,

    HOTFIX_SEL_WMO_AREA_TABLE,
    HOTFIX_SEL_WMO_AREA_TABLE_LOCALE,

    HOTFIX_SEL_WORLD_EFFECT,

    HOTFIX_SEL_WORLD_MAP_AREA,

    HOTFIX_SEL_WORLD_MAP_OVERLAY,

    HOTFIX_SEL_WORLD_MAP_TRANSFORMS,

    HOTFIX_SEL_WORLD_SAFE_LOCS,
    HOTFIX_SEL_WORLD_SAFE_LOCS_LOCALE,

    MAX_HOTFIXDATABASE_STATEMENTS
};

class TC_DATABASE_API HotfixDatabaseConnection : public MySQLConnection
{
public:
    typedef HotfixDatabaseStatements Statements;

    //- Constructors for sync and async connections
    HotfixDatabaseConnection(MySQLConnectionInfo& connInfo);
    HotfixDatabaseConnection(ProducerConsumerQueue<SQLOperation*>* q, MySQLConnectionInfo& connInfo);
    ~HotfixDatabaseConnection();

    //- Loads database type specific prepared statements
    void DoPrepareStatements() override;
};

#endif
