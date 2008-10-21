<?php
require_once dirname(__FILE__)."/../config.php";
require_once dirname(__FILE__)."/../ifcadminapp.php";

class ItemsManagerApp implements IFCAdminApp
{
  private $db = NULL;
  
  public function __construct()
  {
  }
  
  /////////////////////////////////////////////////////////////////////////////////
  
  public function __destruct()
  {
  }
  
  /////////////////////////////////////////////////////////////////////////////////
  
  public function init($db)
  {
    global $DB_SERVER, $DB_DATABASE, $DB_USER, $DB_PASSWORD;
    
    $this->db = $db;
    if ( !$this->db->Connect($DB_SERVER, $DB_DATABASE, $DB_USER, $DB_PASSWORD) )
    {
      echo "Failed to connect to the database!";
      return false;
    }
    
    return true;
  }
  
  /////////////////////////////////////////////////////////////////////////////////
  
  public function render()
  {
  
  }
  
}
?>
