<?php

function go_back($target = NULL)
{
  if ( $target != NULL && strlen($target) > 0 )
  {
    header("location: ".$target);
  }
  else
  {
    header("location: ".$_SERVER["HTTP_REFERER"]);
  }
  exit();
}

?>