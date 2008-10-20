<?php
/*
 * Logout handler script
 */
require_once "../config.php";
require_once "../utils/utils.php";

session_start();

session_destroy();

go_back($BASE_FOLDER."index.php");
?>