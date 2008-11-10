<?php
require_once dirname(__FILE__)."/../config.php";
require_once dirname(__FILE__)."/../ifcadminapp.php";

class ItemType
{
  public $id;
  public $name;
  public $dbTable;
}

class Item
{
  public $id;
  public $name;
  public $type_id;
  public $object_id;
  public $description;
  public $min_level;
  public $max_level;
  public $npc_value;
}

class ItemsManagerApp implements IFCAdminApp
{
  private $db = NULL;
  private $itemAction = NULL;
  private $itemFilter = NULL;
  private $itemTypes = NULL;
  private $itemData = NULL;
  private $numItems = 0;
  private $itemsPerPage = 50;
  private $currentPage = 0;
  private $numPagesAvailable = 0;
  
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
    if ( !$this->db->Connect($DB_SERVER, $DB_DATABASE, $DB_USER, $DB_PASSWORD) )
    {
      echo "Failed to connect to the database!";
      return false;
    }
    
    $this->itemAction = $_GET["i"];
    $this->itemFilter = $_GET["f"];
    $this->LoadItemTypes();
    
    switch ( $this->itemAction )
    {
      case  NULL:
        {
          // check if a specific page was requested (if not, set it to 0)
          $this->currentPage = $_GET["page"];
          if ( $this->currentPage == NULL )
            $this->currentPage = 0;
            
          $this->numItems = $this->GetNumItems();
        
          // calculate the number of pages available
          $this->numPagesAvailable = ceil($this->numItems / $this->itemsPerPage);
          
          // get the user data
          $this->itemData = $this->LoadItems();
        }
        break;
          
      default:
        {
          if ( $this->itemAction > -1 )
            $this->LoadItem( $this->itemAction );
        }
        break;
    }
    
    return true;
  }
  
  /////////////////////////////////////////////////////////////////////////////////
  // Rendering functions
  /////////////////////////////////////////////////////////////////////////////////
  
  public function render()
  {
    switch ( $this->itemAction )
    {
      case  NULL:
        $startIndex = $this->currentPage * $this->itemsPerPage;
        $this->render_options();
        $this->render_list($startIndex);
        $this->render_pages();
        break;

      default:
        $this->render_itemform($this->userAction);
        break;
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////////

  private function render_options()
  {
    echo "<a href=\"?a=items&i=-1\">Add Item</a>";
    echo "<br/><br/>";
  }
  
  /////////////////////////////////////////////////////////////////////////////////
  
  private function render_list($startIndex = 0)
  {
    //$this->render_filter_options();
    
echo <<<LIST
    <table class="tableUsers" cellspacing="0">
      <tr>
        <th>ID</th>
        <th>Name</th>
        <th>Type</th>
        <!--
        <th width="200">Description</th>
        -->
        <th>Levels</th>
        <th>NPC Value</th>
        <th>Options </th>
      </tr>
LIST;
    $itemsRendered = 0;
    for ( $i = $startIndex; $i < count($this->itemData); $i++ )
    {
      $item = $this->itemData[$i];
      $type = $this->itemTypes[$item->type_id];
      
      echo "<tr>";
      echo "<td>".$item->id."</td>";
      echo "<td><a href=\"?a=items&i=".$item->id."\">".$item->name."</a></td>";
      echo "<td>".$type->name."</td>";
      echo "<td>".$item->min_level." - ".$item->max_level."</td>";
      echo "<td>".$item->npc_value."</td>";
      echo "<td><a href=\"actions/delitem.php?aid=".$item->id."\">Delete</a>";
      echo "</tr>";
      $itemsRendered++;
      if ( $itemsRendered >= $this->itemsPerPage )
        break;
    }
echo <<<LIST
    </table>
LIST;
    echo "<small>".($i-$startIndex)." items listed</small><br/><br/>";
    
  }

  /////////////////////////////////////////////////////////////////////////////////

  private function render_pages()
  {
    echo "<center><small>Pages</small><br/>";
    for ( $i = 0; $i < $this->numPagesAvailable; $i++ )
    {
      if ( $i != $this->currentPage )
      {
        echo "<a href=\"?a=items&page=$i\">";
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
  
  private function render_filter_options()
  {
echo <<<FILTERS
    <div class="itemfilters">
      <form method="get">
        <table>
          <tr>
            <td>
            Item Type:
              <select name="f">
                <option value="0">All</option>
FILTERS;
    // echo the item types into the selection list
    for ( $i = 0; $i < count($this->itemTypes); $i++ )
    {
      $type = $this->itemTypes[$i];
      echo "<option value=\"".$type->id."\">".$type->name."</option>";    
    }
    
echo <<<FILTERS
              </select>
            </td>
            <td>
              <input type="submit" value="filter" />            
            </td>
          </tr>
        </table>
      </form>
    </div>
FILTERS;
  }

  /////////////////////////////////////////////////////////////////////////////////

  private function render_itemform($itemID)
  {
    // if we have an item loaded
    if ( $this->itemData != NULL )
    {
    
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////////
  // Data Loading functions
  /////////////////////////////////////////////////////////////////////////////////
  
  private function LoadItemTypes()
  {
    $this->itemTypes = Array();
    $query = "SELECT * FROM fc_itemtypes";
    $result = $this->db->Execute( $query );
    if ( $result != NULL )
    {
      while ( $line = mysql_fetch_array($result, MYSQL_ASSOC) )
      {
        $type = new ItemType();
        $type->id = $line["itemtype_id"];
        $type->name = $line["itemtype_name"];
        $type->dbTable = $line["itemtype_table"];
        
        $this->itemTypes[ $type->id ] = $type;
//        array_push( $this->itemTypes, $type );
      }      
    }
    
    return count($this->itemTypes);
  }

  /////////////////////////////////////////////////////////////////////////////////

  private function GetNumItems()
  {
    $count = 0;
    $query = "SELECT count(*) as c from fc_items";
    $result = $this->db->Execute( $query );
    if ( $result != NULL )
    {
      if ( $line = mysql_fetch_array($result, MYSQL_ASSOC) )
      {
        $count = $line["c"];
      }
    }
    
    return $count;
  }  
  
  /////////////////////////////////////////////////////////////////////////////////

  private function LoadItems()
  {
    if ( !$this->db )
      return -1;
      
    $query = "select        items.item_id, items.item_name, types.itemtype_id, items.object_id, ".
             "              items.description, items.min_level, items.max_level, items.npc_value ".
             "from          fc_items items ".
             "inner join    fc_itemtypes types ".
             "on            types.itemtype_id = items.itemtype_id ";
             
    // apply a filter if needs be
    if ( $this->itemFilter != NULL )
    {
      $query .= "WHERE items.itemtype_id = $this->itemFilter";
    }             
             
    $itemData = Array();
    $item = NULL;
    $result = $this->db->Execute( $query );
    if ( $result != NULL )
    {
      while ( $line = mysql_fetch_array($result, MYSQL_ASSOC) )
      {
        $item = $this->CreateItemFromRecord($line);
        
        array_push( $itemData, $item);
      }      
    }
    
    return $itemData;
  }  
  
  /////////////////////////////////////////////////////////////////////////////////

  private function LoadItem($itemID)
  {
    if ( !$this->db )
      return false;
      
    $query = "SELECT      * ".
             "FROM        fc_items items ".
             "INNER JOIN  fc_itemtypes types ".
             "ON          type.itemtype_id = items.itemtype_id ".
             "WHERE       items.item_id = $itemID";
             
    $result = $this->db->Execute($query);
    if ( $result != NULL )
    {
      if ( $line = mysql_fetch_array($result, MYSQL_ASSOC) )
        $this->itemData = $this->CreateItemFromRecord($line);
    }
  }  

  /////////////////////////////////////////////////////////////////////////////////

  private function CreateItemFromRecord($record)
  {
    if ( $record == NULL )
      return NULL;
      
    $item = new Item();
    $item->id = $record["item_id"];
    $item->name = $record["item_name"];
    $item->type_id = $record["itemtype_id"];
    $item->object_id = $record["object_id"];
    $item->description = $record["description"];
    $item->min_level = $record["min_level"];
    $item->max_level = ($record["max_level"] == NULL ? "Max" : $record["max_level"]);
    $item->npc_value = $record["npc_value"];

    return $item;    
  }
}
?>
