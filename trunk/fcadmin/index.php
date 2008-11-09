<?php
  require_once dirname(__FILE__)."/config.php";
  require_once dirname(__FILE__)."/ifcadminapp.php";
  require_once dirname(__FILE__)."/db/mysqldb.php";
  require_once dirname(__FILE__)."/apps/itemmanager.php";
  require_once dirname(__FILE__)."/apps/usermanager.php";
  
  session_start();
  
  $db = new MySQLDB();
  $app = NULL;
  
  if ( !$db->Connect($DB_SERVER, $DB_DATABASE, $DB_USER, $DB_PASSWORD) )
  {
    die("Failed to connect to database");
  }

echo <<<HTML
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"    
	"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html lang="en" xml:lang="en">
<head>
	<title>FireCell Administration</title>
	<meta http-equiv="Content-Type" content="txt/html; charset=utf-8" />
	<link rel="stylesheet" type="text/css" href="css/fcadmin.css" />
<!--	<link rel="shortcut icon" href="index.php?img=favicon" type="image/ico" /> !-->
</head>

<body>
<div id="bodyContainer">  
	<div id="head">
    FireCell Administration System
  	<ul class="utility">
  		<li>Version 0.1.03
  	</ul>
	</div>

  <div id="content">
HTML;
  $accountType= $_SESSION["acctype"];
  if ( $accountType < 2 || $accountType == NULL )
  {
echo <<<HTML
      <center>
      <br/>Login required:
      <form name="login" action="actions/login.php" method="post">
        <table id="logintable">
          <tr>
            <td align="right">Username:</td>
            <td><input class="fcedit" name="name" value="" type="text" /></td>
          </tr>
          <tr>
            <td align="right">Password:</td>
            <td><input class="fcedit" name="password" value="" type="password" /></td>
          </tr>
          <tr>
            <td></td>
            <td><input class="fcbutton" name="Login" value="Login" type="submit" /></td>
          </tr>
        </table>
      </form>
      </center>
HTML;
  }
  else
  {
echo <<<HTML
		<!-- The Navigation Panel !-->
    <div id="sidepanel">
      <h2><a href="index.php">Home</a></h2>
      <h2>World Tools</h2>
      <ul class="tools">
        <li><a href="?a=items">Items</a></li>
        <li><a href="?a=missions">Missions</a></li>
        <li><a href="?a=npc">NPCs</a></li>
      </ul>
    	<h2>Server Management</h2>
      <ul class="tools">
        <li><a href="?a=users">User Management</a></li>
        <li><a href="actions/logout.php">Logout</a></li>
      </ul>
    </div> <!-- /sidepanel !-->
  
		<!-- The application panel area (where all the functionality goes) !-->
    <div id="apppanel">
HTML;
      /*
       *  Load the relevant app panel per request
       */
       $appname = $_GET["a"];
       
       switch ( $appname )
       {
       case "items":
			  $app = new ItemsManagerApp();
        break;
        
       case "missions":
        break;
        
       case "npc":
        break;
        
       case "users":
        $app = new UserManagerApp();
        break;
       }
       
       if ( strlen($appname) > 0 )
       {
         if ( $app instanceof IFCAdminApp )
         {
           if ( $app->init($db) == true )
           {
             $app->render();
           }
           else
           {
             echo "Failed to initialise the requested application!";
           }
         }
         else
         {
           echo "Requested app class does not implement IFCAdminApp interface!";
         }
       }
       else
       {
         echo "No app found for this request!";
       }
    echo "</div> <!-- /APPPANEL !-->";
  }
echo <<<HTML
		<!-- footer... !-->
    <div id="foot">
      FireCell Administration System by Peter Mares, 2008
    </div>
  </div> <!-- /CONTENT !-->
</div>  
</body>
</html>
HTML;
?>