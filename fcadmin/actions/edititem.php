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
    UpdateProcessor($db, $_POST);
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

function UpdateProcessor($db, $dataArray)
{
  // update the processor
  $query = "UPDATE fc_processors set core_count = ".$dataArray["core_count"].", core_speed = ".$dataArray["core_speed"]." WHERE processor_id = ".$dataArray["object_id"];
  $result = $db->Execute($query);
  
  // update the item
  $query = "UPDATE fc_items SET item_name = '".mysql_escape_string($dataArray["item_name"]).
           "', description = '".mysql_escape_string($dataArray["description"])."', min_level = ".$dataArray["min_level"].", max_level = ".$dataArray["max_level"].
           ", npc_value = ".$dataArray["npc_value"]." WHERE item_id = ".$dataArray["item_id"];
  $result = $db->Execute($query);
}

?>
