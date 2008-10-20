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
  		<li>Version 0.1.01
  	</ul>
	</div>

  <div id="content">
	
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
      </ul>
    </div> <!-- /sidepanel !-->
  
		<!-- The application panel area (where all the functionality goes) !-->
    <div id="apppanel">
    <?php
      /*
       *  Load the relevant app panel per request
       */
       $appname = $_GET["a"];
       
        echo $appname;
       switch ( $appname )
       {
       case "items":
        break;
        
       case "missions":
        break;
        
       case "npc":
        break;
        
       case "users":
        break;
       }
    ?>
    </div> <!-- /APPPANEL !-->
		
		<!-- footer... !-->
    <div id="foot">
      FireCell Administration System by Peter Mares, 2008
    </div>
  </div> <!-- /CONTENT !-->
</div>  
</body>
</html>