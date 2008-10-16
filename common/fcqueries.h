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
// ACCOUNT QUERIES
////////////////////////////////////////
#define DBQ_LOAD_ACCOUNT                  "load_account"
#define DBQ_LOAD_CHARACTER_IDS            "load_character_ids"

#endif//_FCQUERIES_H_