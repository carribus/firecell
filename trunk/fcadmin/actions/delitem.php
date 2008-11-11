<?php
/*
 *  Script to delete an item from the database
 */
require_once dirname(__FILE__)."/../config.php";
require_once dirname(__FILE__)."/../db/mysqldb.php";

session_start();
if ( !isset($_SESSION["account_id"]) ) die("You must be logged in to use this function!");
if ( !isset($_SESSION["acctype"]) ) die("You must be logged in to use this function!");
if ( isset($_SESSION["acctype"]) && $_SESSION["acctype"] != 3 ) die("You must be logged in to use this function!");

if ( !isset($_GET["iid"]) )
  die("No Item ID specified!");
  
$item_id = $_GET["iid"];
$itemtype_id = NULL;
$object_id = NULL;

$db = new MySQLDB();

if ( !$db->Connect($DB_SERVER, $DB_DATABASE, $DB_USER, $DB_PASSWORD) )
{
  die("Failed to connect to database");
}

$query = "select * from fc_items where item_id = $item_id";
$result = $db->Execute( $query );

if ( $result != NULL )
{
  if ( $line = mysql_fetch_array($result, MYSQL_ASSOC) )
  {
    $object_id = $line["object_id"];
    $itemtype_id = $line["itemtype_id"];
  }
}

// delete the item
$query = "DELETE FROM fc_items WHERE item_id = $item_id";
$db->Execute($query);

switch ( $itemtype_id )
{
  case  1:  // processor
    $query = "DELETE FROM fc_processors WHERE processor_id = $object_id";
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

// delete the object
$db->Execute($query);


header("location: ".$BASE_FOLDER."?a=items");
?>
