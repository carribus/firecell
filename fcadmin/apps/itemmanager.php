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

class Processor
{
  public $id;
  public $core_speed;
  public $core_count;
}

class ItemsManagerApp implements IFCAdminApp
{
  const ITEMTYPE_PROCESSOR = 1;
  const ITEMTYPE_OPERATINGSYSTEM = 2;
  const ITEMTYPE_MEMORY = 3;
  const ITEMTYPE_NETWORK = 4;
  const ITEMTYPE_STORAGE = 5;
  const ITEMTYPE_SOFTWARE = 6;
  const ITEMTYPE_DATA = 7;
  const ITEMTYPE_MISC = 8;

  private $db = NULL;
  private $itemAction = NULL;
  private $itemFilter = NULL;
  private $itemTypes = NULL;
  private $itemData = NULL;
  private $itemType = NULL;                         // passed in 't' during 'Add' or 'Edit' functionality
  private $objectData = NULL;
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
    $this->LoadItemTypes();
    
    switch ( $this->itemAction )
    {
      case  NULL:
        {
          $this->itemFilter = $_GET["f"];
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
          $this->itemType = $_GET["t"];
          if ( $this->itemAction > -1 )
          {
            $this->LoadItem( $this->itemAction );
          }
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
        $this->render_itemform($this->itemAction);
        break;
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////////

  private function render_options()
  {
    echo "<center>";
    foreach ($this->itemTypes as $id => $type)
    {
      echo "<a href=\"?a=items&i=-1&t=$id\">Add ".$type->name."</a> |&nbsp;";
    }
    echo "</center><br/><br/>";
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
      echo "<td><a href=\"?a=items&i=".$item->id."&t=".$item->type_id."\">".$item->name."</a></td>";
      echo "<td>".$type->name."</td>";
      echo "<td>".$item->min_level." - ".$item->max_level."</td>";
      echo "<td>".$item->npc_value."</td>";
      echo "<td><a href=\"actions/delitem.php?iid=".$item->id."\">Delete</a>";
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
      if ( $type != NULL )
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
    switch ( $this->itemType )
    {
      case  constant("ItemsManagerApp::ITEMTYPE_PROCESSOR"):
        $this->render_processor_form($itemID);
        break;
        
      case  constant("ItemsManagerapp::ITEMTYPE_OPERATINGSYSTEM"):
        $this->render_os_form($itemID);
        break;
        
      case  constant("ItemsManagerapp::ITEMTYPE_MEMORY"):
        break;
        
      case  constant("ItemsManagerapp::ITEMTYPE_NETWORK"):
        break;
        
      case  constant("ItemsManagerapp::ITEMTYPE_STORAGE"):
        break;
        
      case  constant("ItemsManagerapp::ITEMTYPE_SOFTWARE"):
        break;
        
      case  constant("ItemsManagerapp::ITEMTYPE_DATA"):
        break;
        
      case  constant("ItemsManagerapp::ITEMTYPE_MISC"):
        break;
        
      default:
        echo "Unknown type (".$this->itemType.")<br/>";
        break;
    }
  }

  /////////////////////////////////////////////////////////////////////////////////

  private function render_processor_form($itemID)
  {
    $userAction = ($itemID == -1 ? "additem" : "edititem");
    $readOnly = ($itemID == -1 ? "" : "readonly=\"yes\"");
    
    $itemName = NULL;
    $minLevel = 1;
    $maxLevel = NULL;
    $npcValue = 0;
    $description = NULL;
    $objectID = -1;
    $coreCount = 0;
    $coreSpeed = 0;
    
    if ( $itemID != -1 )
    {
      $itemName = $this->itemData->name;
      $minLevel = $this->itemData->min_level;
      $maxLevel = ($this->itemData->max_level == "Max" ? NULL : $this->itemData->max_level);
      $npcValue = $this->itemData->npc_value;
      $description = $this->itemData->description;
      $coreCount = $this->objectData->core_count;
      $coreSpeed = $this->objectData->core_speed;
      $objectID = $this->objectData->id;
    }
    
    echo "<h2>".($itemID == -1 ? "Adding " : "Editing ")." processor</h2><br/>";
    
echo <<<FORMDEF
    <form name="$userAction" method="post" action="actions/$userAction.php">
      <input type="hidden" name="item_type" value="1" />
      <table>
        <tr>
          <td>Name:</td>
          <td><input type="text" name="item_name" value="$itemName" /></td>
        </tr>
        <tr>
          <td>Minimum Level:</td>
          <td><input type="text" name="min_level" value="$minLevel" /></td>
        </tr>
        <tr>
          <td>Maximum Level:</td>
          <td><input type="text" name="max_level" value="$maxLevel" /> (leave empty for max)</td>
        </tr>
        <tr>
          <td>NPC Value:</td>
          <td><input type="text" name="npc_value" value="$npcValue" /></td>
        </tr>
        <tr>
          <td>Core Count:</td>
          <td><input type="text" name="core_count" value="$coreCount" /></td>
        </tr>
        <tr>
          <td>Core Speed:</td>
          <td><input type="text" name="core_speed" value="$coreSpeed" /> Mhz</td>
        </tr>
        <tr>
          <td>Description:</td>
          <td><textarea name="description" cols="50" rows="5">$description</textarea></td>
        </tr>
        <tr>
          <td></td>
          <td><input type="submit" value="Submit" /></td>
        </tr>
      </table>
FORMDEF;

    if ( $itemID != -1 )
    {
      echo "<input type=\"hidden\" name=\"item_id\" value=\"$itemID\" />";          // the item ID
      echo "<input type=\"hidden\" name=\"object_id\" value=\"$objectID\" />";
    }

echo <<<FORMDEF
    </form>
FORMDEF;
  
  }
  
  /////////////////////////////////////////////////////////////////////////////////

  private function render_os_form($itemID)
  {
    $userAction = ($itemID == -1 ? "addos" : "editos");
    $readOnly = ($itemID == -1 ? "" : "readonly=\"yes\"");
    
    $itemName = NULL;
    $minLevel = 1;
    $maxLevel = NULL;
    $npcValue = 0;
    
    if ( $itemID != -1 )
    {
      $itemName = $this->itemData->name;
      $minLevel = $this->itemData->min_level;
      $maxLevel = ($this->itemData->max_level == "Max" ? NULL : $this->itemData->max_level);
      $npcValue = $this->itemData->npc_value;
    }
    
    echo "<h2>".($itemID == -1 ? "Adding " : "Editing ")." processor</h2><br/>";
    
echo <<<FORMDEF
    <form name="$userAction" method="post" action="actions/$userAction.php">
      <table>
        <tr>
          <td>Name:</td>
          <td><input type="text" name="item_name" value="$itemName" /></td>
        </tr>
        <tr>
          <td>Minimum Level:</td>
          <td><input type="text" name="min_level" value="$minLevel" /></td>
        </tr>
        <tr>
          <td>Maximum Level:</td>
          <td><input type="text" name="max_level" value="$maxLevel" /> (leave empty for max)</td>
        </tr>
        <tr>
          <td>NPC Value:</td>
          <td><input type="text" name="npc_value" value="$npcValue" /></td>
        </tr>
        <tr>
          <td></td>
          <td></td>
        </tr>
        <tr>
          <td>Description:</td>
          <td><textarea name="description" cols="50" rows="5"></textarea></td>
        </tr>
        <tr>
          <td></td>
          <td><input type="submit" value="Submit" /></td>
        </tr>
      </table>
    </form>
FORMDEF;
  
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
             "ON          types.itemtype_id = items.itemtype_id ".
             "WHERE       items.item_id = $itemID";
    $result = $this->db->Execute($query);
    if ( $result != NULL )
    {
      if ( $line = mysql_fetch_array($result, MYSQL_ASSOC) )
        $this->itemData = $this->CreateItemFromRecord($line);
    }
    
    // now that we have the item shell, we need to load the underlying object
    if ( $this->itemData != NULL )
    {
      // get the table for the object type
      $objType = $this->itemTypes[ $this->itemData->type_id ];
      $query = "SELECT * FROM ".$objType->dbTable." WHERE processor_id = ".$this->itemData->object_id;
      $result = $this->db->Execute($query);
      if ( $result != NULL )
      {
        if ( $line = mysql_fetch_array($result, MYSQL_ASSOC) )
        {
          $this->CreateObjectFromRecord($objType->id, $line);
        }
      }
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
  
  /////////////////////////////////////////////////////////////////////////////////

  private function CreateObjectFromRecord($objType, $record)
  {
    if ( $record == NULL )
      return false;
      
    switch ( $objType )
    {
      case  constant("ItemsManagerApp::ITEMTYPE_PROCESSOR"):
        $this->CreateProcessorFromRecord($record);
        break;
        
      case  constant("ItemsManagerapp::ITEMTYPE_OPERATINGSYSTEM"):
        $this->CreateOSFromRecord($record);
        break;
        
      case  constant("ItemsManagerapp::ITEMTYPE_MEMORY"):
        break;
        
      case  constant("ItemsManagerapp::ITEMTYPE_NETWORK"):
        break;
        
      case  constant("ItemsManagerapp::ITEMTYPE_STORAGE"):
        break;
        
      case  constant("ItemsManagerapp::ITEMTYPE_SOFTWARE"):
        break;
        
      case  constant("ItemsManagerapp::ITEMTYPE_DATA"):
        break;
        
      case  constant("ItemsManagerapp::ITEMTYPE_MISC"):
        break;
        
      default:
        echo "Unknown type (".$this->itemType.")<br/>";
    }
    
    return ($this->objectData != NULL);
  }

  /////////////////////////////////////////////////////////////////////////////////

  private function CreateProcessorFromRecord($record)
  {
    if ( $record == NULL )
     return false;
     
    $this->objectData = new Processor;
    $this->objectData->id = $record["processor_id"];
    $this->objectData->core_count = $record["core_count"];
    $this->objectData->core_speed = $record["core_speed"];    
  }
  
  /////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////

}
?>
