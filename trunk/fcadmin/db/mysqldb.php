<?php
class MySQLDB
{
	private $link = NULL;
	
	public function __construct()
	{
	}
	
	public function __destruct()
	{
		$this->Close();
	}
	
	public function Connect($server, $dbname, $user, $password)
	{	
		$result = false;
		
		$this->link = mysql_pconnect($server, $user, $password);
		if ( $this->link != NULL )
		{
			mysql_select_db($dbname);
			$result = true;
		}
		
		return $result;
	}
	
	public function Close()
	{
		if ( $this->link != NULL )
		{
			mysql_close($this->link);
		}
	}
	
	public function Execute($query)
	{
		$result = NULL;
		
		if ( $this->link != NULL )
		{
			$result = mysql_query($query, $this->link);	
		}
		
		return $result;
	}
}
?>