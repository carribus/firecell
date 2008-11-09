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

$account_id = $_POST["account_id"];
$account_name = $_POST["account_name"];
$account_email = $_POST["account_email"];
$account_pass = $_POST["account_pass"];
$account_type = $_POST["account_type"];

$db = new MySQLDB();

if ( !$db->Connect($DB_SERVER, $DB_DATABASE, $DB_USER, $DB_PASSWORD) )
{
  die("Failed to connect to database");
}

$query = "UPDATE fc_accounts SET acc_name = '".mysql_escape_string($account_name)."', email = '".mysql_escape_string($account_email)."', acc_password = '".mysql_escape_string($account_pass)."', acc_type = $account_type ".
         "WHERE account_id = $account_id";
$result = $db->Execute( $query );

header("location: ".$BASE_FOLDER."?a=users");
?>
