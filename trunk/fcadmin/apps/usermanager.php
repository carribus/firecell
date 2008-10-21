<?php
require_once dirname(__FILE__)."/../config.php";
require_once dirname(__FILE__)."/../ifcadminapp.php";

class User
{
  public $account_id;
  public $account_name;
  public $account_password;
  public $account_created;
  public $account_type;
  public $account_email;
}

class UserManagerApp implements IFCAdminApp
{
  // attributes
  private $userAction = NULL;
  private $itemsPerPage = 50;
  private $db = NULL;
  private $currentPage = 0;
  private $numUsers = 0;
  private $numPagesAvailable = 0;
  private $userData;
  private $query_NumUsers = "SELECT count(*) as c FROM fc_accounts";
  private $query_GetUsers = "SELECT * FROM fc_accounts";
  
  public function __construct($itemsPerPage = 50)
  {
    $this->itemsPerPage = $itemsPerPage;
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
    
    $this->userAction = $_GET["u"];
    
    if ( $this->userAction == NULL )
    {
      // check if a specific page was requested (if not, set it to 0)
      $this->currentPage = $_GET["page"];
      if ( $this->currentPage == NULL )
        $this->currentPage = 0;

      // get the number of users so we can calculate the pagination
      $this->numUsers = $this->GetNumUsers();                    
      if ( $this->numUsers == -1 )
      {
        echo "Failed to retrieve number of users!";
        return false;
      }              
      // calculate the number of pages available
      $this->numPagesAvailable = ceil($this->numUsers / $this->itemsPerPage);
        
      // get the user data
      $this->userData = $this->GetUsers();
    }    
    else
    {
      switch ( $this->userAction )
      {
        case  -1:
          break;
          
        default:
          break;
      }
    }
      
    return true;
  }
  
  /////////////////////////////////////////////////////////////////////////////////
  
  public function render()
  {
    if ( $this->userAction == NULL )
    {
      $startIndex = $this->currentPage * $this->itemsPerPage;
      $this->render_options();
      $this->render_table($startIndex);    
      $this->render_pages();
    }
    else if ( $this->userAction == -1 )
    {
      // add a new user
      echo "Adding a new user not implemented yet";
    }
    else
    {
      // display the user's details for modification
      echo "Editing for users not implemented yet (would be editing user id ".$this->userAction;
    }
  }

  /////////////////////////////////////////////////////////////////////////////////
  
  private function render_options()
  {
    echo "<a href=\"?a=users&u=-1\">Add User</a>";
    echo "<br/><br/>";
  }

  /////////////////////////////////////////////////////////////////////////////////

  private function render_table($startIndex)
  {
    echo "<table class=\"tableUsers\" cellspacing=\"0\">";
    echo "<tr><th>ID</th><th>Account Name</th><th>Password</th><th>Type</th><th>Email</th><th>Created</th>";
    for ( $i = $startIndex; $i < $this->numUsers; $i++ )
    {
      $user = $this->userData[$i];
      echo "<tr>\n<td>".$user->account_id."</td>";
      echo "<td><a href=\"?a=users&u=".$user->account_id."\">".$user->account_name."</a></td>";
      echo "<td>******</td>";
      echo "<td>".$user->account_type."</td>";
      echo "<td>".$user->account_email."</td>";
      echo "<td>".$user->account_created."</td>";
      echo "</tr>";
    }
    echo "</table>";
    echo "<small>".($i-$startIndex)." users listed</small><br/><br/>";
  }
  
  /////////////////////////////////////////////////////////////////////////////////

  private function render_pages()
  {
    echo "<center><small>Pages</small><br/>";
    for ( $i = 0; $i < $this->numPagesAvailable; $i++ )
    {
      if ( $i != $this->currentPage )
      {
        echo "<a href=\"?a=users&page=$i\">";
      }
      echo ($i+1);
      if ( $i != $this->currentPage )
      {
        echo "</a>";
      }
      echo "&nbsp;&nbsp;";
    }
    echo "</center>";
  }
  
  /////////////////////////////////////////////////////////////////////////////////
  
  private function GetNumUsers()
  {
    if ( !$this->db )
      return -1;
      
    $resValue = 0;
    $result = $this->db->Execute( $this->query_NumUsers );
    if ( $result != NULL )
    {
      if ( $line = mysql_fetch_array($result, MYSQL_ASSOC) )
      {
        $resValue = $line["c"];
      }      
    }
    
    return $resValue;    
  }  
  
  /////////////////////////////////////////////////////////////////////////////////

  private function GetUsers()
  {
    if ( !$this->db )
      return -1;
      
    $userData = Array();
    $user = NULL;
    $result = $this->db->Execute( $this->query_GetUsers );
    if ( $result != NULL )
    {
      while ( $line = mysql_fetch_array($result, MYSQL_ASSOC) )
      {
        $user = new User();
        $user->account_id = $line["account_id"];
        $user->account_name = $line["acc_name"];
        $user->account_password = $line["acc_password"];
        $user->account_created = $line["created"];
        $user->account_type = $line["acc_type"];
        $user->account_email = $line["email"];
        
        array_push( $userData, $user );
      }      
    }
    
    return $userData;
  }
  
  /////////////////////////////////////////////////////////////////////////////////
  
}
?>
