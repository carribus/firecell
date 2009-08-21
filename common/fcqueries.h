/*
    FireCell Server - The server code for the firecell multiplayer game
    Copyright (C) 2008  Peter M. Mares

		Contact: carribus@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _FCQUERIES_H_
#define _FCQUERIES_H_

/**
 *  @file fcqueries.h
 *
 *  This file contains the string definitions of query names that are used
 *  by the FireCell data services. 
 *  FCDatabase passes these defines into its respectively loaded interface
 *  engine. The engine would, at this stage, have loaded a list of supported
 *  queries from its respective query file (named as follows:
 *
 *  queries_<engineName>.conf   (where engineName is 'mysql', or 'pgsql' or 'mssql' etc).
 *
 *  So if you add a new query definition name to this file, make sure that 
 *  you also provide the matching query for all supported database types (and vice versa)
 */

////////////////////////////////////////
// ITEM LOADING QUERIES
////////////////////////////////////////
#define DBQ_LOAD_ITEM_TYPES                             "load_item_types"
#define DBQ_LOAD_ITEM_DEFS                              "load_items"
#define DBQ_LOAD_PROCESSOR_ITEM                         "load_item_processor"
#define DBQ_LOAD_OS_ITEM                                "load_item_os"
#define DBQ_LOAD_MEMORY_ITEM                            "load_item_memory"
#define DBQ_LOAD_SOFTWARE_ITEM                          "load_item_software"

////////////////////////////////////////
// WORLD DATA QUERIES
////////////////////////////////////////
#define DBQ_LOAD_WORLD_GEOGRAPHY                        "load_world_geography"
#define DBQ_LOAD_COMPANIES                              "load_companies"
#define DBQ_LOAD_COMPANY_COMPUTERS                      "load_company_computers"
#define DBQ_LOAD_COMPANY_PORTS                          "load_company_ports"
#define DBQ_LOAD_MISSIONS                               "load_missions"

#define DBQ_LOAD_FORUM_CATEGORIES					              "load_forum_categories"
#define DBQ_LOAD_FORUM_POSTS							              "load_forum_posts"

////////////////////////////////////////
// ACCOUNT QUERIES
////////////////////////////////////////
#define DBQ_LOAD_ACCOUNT                                "load_account"
#define DBQ_LOAD_CHARACTER_INFO                         "load_characters"
#define DBQ_LOGIN_CHARACTER                             "login_character"
#define DBQ_LOGOUT_CHARACTER                            "logout_character"

////////////////////////////////////////
// CHARACTER LOADING QUERIES
////////////////////////////////////////
#define DBQ_LOAD_CHARACTER_COMPUTER                     "load_character_computer"
#define DBQ_LOAD_CHARACTER_PORTS                        "load_character_ports"
#define DBQ_LOAD_CHARACTER_ITEMS                        "load_character_items"
#define DBQ_LOAD_CHARACTER_MISSIONS                     "load_character_missions"
#define DBQ_LOAD_CHARACTER_BANKACCOUNT                  "load_character_bankaccount"

////////////////////////////////////////
// CHARACTER PERSISTANCE QUERIES
////////////////////////////////////////
#define DBQ_SAVE_CHARACTER_PORTS                        "save_character_port"
#define DBQ_SAVE_CHARACTER_MISSION_NEW                  "save_character_mission_new"
#define DBQ_SAVE_CHARACTER_MISSION_SUCCESS_COUNT        "save_character_mission_success_count"
#define DBQ_SAVE_CHARACTER_MISSION_COMPLETE_FLAG        "save_character_mission_complete_flag"
#define DBQ_INSERT_CHARACTER_ITEM_INFO                  "insert_character_item_info"
#define DBQ_UPDATE_CHARACTER_ITEM_INFO                  "save_character_item_info"
#define DBQ_DELETE_CHARACTER_ITEM_INFO                  "del_character_item_info"
#define DBQ_INSERT_CHARACTER_BANK_ACCOUNT               "insert_character_bank_account"
#define DBQ_UPDATE_CHARACTER_BANK_ACCOUNT               "save_character_bank_account"

////////////////////////////////////////
// FORUM QUERIES
////////////////////////////////////////
#define DBQ_SAVE_FORUM_POST                             "save_forum_post"

////////////////////////////////////////
// CHAT SERVER QUERIES
////////////////////////////////////////
#define DBQ_LOAD_CHAT_ROOMS                             "load_chat_rooms"

#endif//_FCQUERIES_H_