<?php
/*
 *  Script to update a user in the database
 */
require_once dirname(__FILE__)."/../config.php";
require_once dirname(__FILE__)."/../db/mysqldb.php";

session_start();
if ( !isset($_SESSION["account_id"]) ) die("You must be logged in to use this function!");
if ( !isset($_SESSION["acctype"]) ) die("You must be logged in to use this function!");
if ( isset($_SESSION["acctype"]) && $_SESSION["acctype"] != 3 ) die("You must be logged in to use this function!");

if ( !isset($_GET["aid"]) )
  die("No User ID specified!");
  
$account_id = $_GET["aid"];

$db = new MySQLDB();

if ( !$db->Connect($DB_SERVER, $DB_DATABASE, $DB_USER, $DB_PASSWORD) )
{
  die("Failed to connect to database");
}

$query = "DELETE FROM fc_accounts WHERE account_id = ".$account_id;
$result = $db->Execute( $query );

header("location: ".$BASE_FOLDER."?a=users");
?>
