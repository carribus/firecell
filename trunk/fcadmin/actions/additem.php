<?php
/*
 *  Script to update a Processor Item in the database
 */
require_once dirname(__FILE__)."/../config.php";
require_once dirname(__FILE__)."/../db/mysqldb.php";

session_start();
if ( !isset($_SESSION["account_id"]) ) die("You must be logged in to use this function!");
if ( !isset($_SESSION["acctype"]) ) die("You must be logged in to use this function!");
if ( isset($_SESSION["acctype"]) && $_SESSION["acctype"] != 3 ) die("You must be logged in to use this function!");

if ( $_POST["max_level"] == NULL )
  $_POST["max_level"] = "NULL";


$db = new MySQLDB();

if ( !$db->Connect($DB_SERVER, $DB_DATABASE, $DB_USER, $DB_PASSWORD) )
{
  die("Failed to connect to database");
}

switch ( $_POST["item_type"] )
{
  case  1:  // processor
    AddProcessor($db, $_POST);
    break;
    
  case  2:  // operating system
    break;
    
  case  3:
    break;
    
  case  4:
    break;
    
  case  5:
    break;
    
  case  6:
    break;
    
  case  7:
    break;
    
  case  8:
    break;
    
  case  9:
    break;
    
}

header("location: ".$BASE_FOLDER."?a=items");

///////////////////////////////////////////////////////////////////////////////////////////

function AddProcessor($db, $dataArray)
{
  // insert the processor
  $query = "INSERT INTO fc_processors (core_count, core_speed) VALUES (".$dataArray["core_count"].", ".$dataArray["core_speed"].")";
  $result = $db->Execute($query);
  $objectID = $db->GetLastInsertedID();  
  
  // insert the item
  $query = "INSERT INTO fc_items (item_name, itemtype_id, object_id, description, min_level, max_level, npc_value) ".
           "VALUES ".
           "( '".mysql_escape_string($dataArray["item_name"])."', ".
           $dataArray["item_type"].", $objectID, '".mysql_escape_string($dataArray["description"])."', ".
           $dataArray["min_level"].", ".$dataArray["max_level"].", ".$dataArray["npc_value"].")";
  $result = $db->Execute($query);
}

?>
