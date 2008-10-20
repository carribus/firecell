<?php
/*
 * Login form handler script
 */
require_once "../config.php";
require_once "../db/mysqldb.php";
require_once "../utils/utils.php";

session_start(); 

if ( !isset( $_POST ) )
	go_back();

if ( !isset( $_POST["name"] ) )
	go_back();
	
if ( !isset( $_POST["password"] ) )
	go_back();
	
$loginname = $_POST["name"];
$password = $_POST["password"];
$db = new MySQLDB();

if ( $db->Connect($DB_SERVER, $DB_DATABASE, $DB_USER, $DB_PASSWORD) == true )
{
	$query = "SELECT account_id, acc_name, acc_type ".
			 "FROM fc_accounts ".
			 "WHERE acc_name = '$loginname' AND acc_password = '$password'";
	$result = $db->Execute($query);
	
	if ( $result != NULL )
	{
		if ( $line = mysql_fetch_array($result, MYSQL_ASSOC) )
		{
			$accountid = $line["account_id"];
			$accname = $line["acc_name"];
			$acctype = $line["acc_type"];
			
			// update the session
      $_SESSION["account_id"] = $accountid;
      $_SESSION["accname"] = $accname;
      $_SESSION["acctype"] = $acctype;

			// strip the action from the HTTP_REFERER
			$referer = $_SERVER[HTTP_REFERER];
			$nPos = strrpos($referer, '?');
			if ( $nPos )
			{
				$referer = substr( $referer, 0, $nPos );
				$_SERVER[HTTP_REFERER] = $referer;
			}
		}
		else
		{
      $_SESSION["lasterror"] = "Invalid login or password";
		}
	}
}

go_back();
	
?>