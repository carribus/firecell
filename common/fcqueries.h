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

/*
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
#define DBQ_LOAD_ITEM_TYPES               "load_item_types"
#define DBQ_LOAD_ITEM_DEFS                "load_items"
#define DBQ_LOAD_PROCESSOR_ITEM           "load_item_processor"
#define DBQ_LOAD_OS_ITEM                  "load_item_os"
#define DBQ_LOAD_MEMORY_ITEM              "load_item_memory"

////////////////////////////////////////
// WORLD DATA QUERIES
////////////////////////////////////////
#define DBQ_LOAD_WORLD_GEOGRAPHY          "load_world_geography"
#define DBQ_LOAD_COMPANIES                "load_companies"
#define DBQ_LOAD_COMPANY_COMPUTERS        "load_company_computers"

////////////////////////////////////////
// ACCOUNT QUERIES
////////////////////////////////////////
#define DBQ_LOAD_ACCOUNT                  "load_account"
#define DBQ_LOAD_CHARACTER_INFO           "load_characters"
#define DBQ_LOGIN_CHARACTER               "login_character"
#define DBQ_LOGOUT_CHARACTER              "logout_character"

////////////////////////////////////////
// CHARACTER LOADING QUERIES
////////////////////////////////////////
#define DBQ_LOAD_CHARACTER_COMPUTER       "load_character_computer"


#endif//_FCQUERIES_H_