/*
SQLyog Enterprise - MySQL GUI v7.13 
MySQL - 5.0.67-community-nt : Database - firecell
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

CREATE DATABASE /*!32312 IF NOT EXISTS*/`firecell` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `firecell`;

/*Table structure for table `fc_accounts` */

DROP TABLE IF EXISTS `fc_accounts`;

CREATE TABLE `fc_accounts` (
  `account_id` bigint(20) unsigned NOT NULL auto_increment,
  `acc_name` varchar(64) NOT NULL COMMENT 'account name',
  `acc_password` varchar(64) NOT NULL COMMENT 'account password',
  `created` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP COMMENT 'datetime of account creation',
  `acc_type` smallint(5) unsigned NOT NULL default '0' COMMENT 'type of account (see fc_accounttypes)',
  `email` varchar(64) NOT NULL,
  PRIMARY KEY  (`account_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

/*Data for the table `fc_accounts` */

insert  into `fc_accounts`(`account_id`,`acc_name`,`acc_password`,`created`,`acc_type`,`email`) values (1,'root','toor','2008-10-16 13:24:52',3,'root@fcell.org'),(2,'testuser','test','2008-10-21 12:26:00',1,'test@fcell.org');

/*Table structure for table `fc_accounttypes` */

DROP TABLE IF EXISTS `fc_accounttypes`;

CREATE TABLE `fc_accounttypes` (
  `accounttype_level` smallint(5) unsigned NOT NULL,
  `name` varchar(32) NOT NULL,
  PRIMARY KEY  (`accounttype_level`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `fc_accounttypes` */

insert  into `fc_accounttypes`(`accounttype_level`,`name`) values (0,'Guest'),(1,'Normal'),(2,'GM'),(3,'Administrator');

/*Table structure for table `fc_characters` */

DROP TABLE IF EXISTS `fc_characters`;

CREATE TABLE `fc_characters` (
  `character_id` smallint(5) unsigned NOT NULL auto_increment,
  `account_id` bigint(20) unsigned NOT NULL COMMENT 'account id that this character belongs to',
  `name` char(32) NOT NULL COMMENT 'character''s name',
  `xp` bigint(20) unsigned NOT NULL default '0' COMMENT 'character''s experience points',
  `level` int(10) unsigned NOT NULL default '1' COMMENT 'character''s level',
  `fame_scale` int(11) NOT NULL default '0' COMMENT 'character''s fame/notoriety value',
  `country_id` int(10) unsigned NOT NULL COMMENT 'country that character is in',
  `city_id` int(10) unsigned NOT NULL COMMENT 'city that character is in',
  `is_logged_in` tinyint(1) NOT NULL default '0' COMMENT 'flag that is set when a player logs in and choose a character to play',
  PRIMARY KEY  (`character_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

/*Data for the table `fc_characters` */

insert  into `fc_characters`(`character_id`,`account_id`,`name`,`xp`,`level`,`fame_scale`,`country_id`,`city_id`,`is_logged_in`) values (1,1,'FireCell_GM',0,1,0,1,1,0),(2,1,'TestCharacter',0,1,0,2,2,0),(3,2,'TestUserCharacter',0,1,0,3,3,0);

/*Table structure for table `fc_cities` */

DROP TABLE IF EXISTS `fc_cities`;

CREATE TABLE `fc_cities` (
  `city_id` int(10) unsigned NOT NULL auto_increment,
  `country_id` int(10) unsigned NOT NULL COMMENT 'country that this city belongs to',
  `name` varchar(64) NOT NULL,
  `IP_groupB` smallint(6) unsigned NOT NULL COMMENT 'Group B IP range for city',
  PRIMARY KEY  (`city_id`,`IP_groupB`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin1;

/*Data for the table `fc_cities` */

insert  into `fc_cities`(`city_id`,`country_id`,`name`,`IP_groupB`) values (1,1,'New York',45),(2,2,'Moscow',232),(3,3,'Amsterdam',152),(4,4,'London',136),(5,5,'Sydney',221),(6,6,'Beijing',54),(7,7,'Johannesburg',23),(8,8,'Tokyo',97),(9,9,'Abuja',195),(10,10,'Paris',36),(11,11,'Rio de Janeiro',144);

/*Table structure for table `fc_companies` */

DROP TABLE IF EXISTS `fc_companies`;

CREATE TABLE `fc_companies` (
  `company_id` int(10) unsigned NOT NULL auto_increment,
  `company_name` varchar(64) NOT NULL,
  `city_id` smallint(5) unsigned NOT NULL COMMENT 'City where company exists (to get Group A + B IP classes)',
  `IP_groupC` smallint(5) unsigned default NULL COMMENT 'Group C IP Range (if these are null, the IP is not fixed)',
  `IP_groupD` smallint(5) unsigned default NULL COMMENT 'Group D IP Range (if these are null, the IP is not fixed)',
  PRIMARY KEY  (`company_id`)
) ENGINE=InnoDB AUTO_INCREMENT=33 DEFAULT CHARSET=latin1;

/*Data for the table `fc_companies` */

insert  into `fc_companies`(`company_id`,`company_name`,`city_id`,`IP_groupC`,`IP_groupD`) values (1,'Anion Devices',1,1,242),(2,'CON Development',5,8,231),(3,'Advan Design',2,12,200),(4,'Incoak Group',3,25,194),(5,'Equic Capital',9,29,184),(6,'Tass Consulting',7,36,175),(7,'COA Ventures',8,43,156),(8,'Mango Corporation',6,44,127),(9,'Quant Research',4,56,126),(10,'RAM Consulting',11,71,123),(11,'Micon Research',1,72,118),(12,'Vlsilinx Incorporated',5,75,114),(13,'Zilinett',2,76,109),(14,'Catmel Asia',6,82,108),(15,'Belluted Systems, Inc',3,95,104),(16,'Citas Asia',7,99,99),(17,'Cadvanducts Ventures',8,104,95),(18,'Softwart Capital',9,108,82),(19,'Klated USA',3,109,76),(20,'Soft Design',4,114,75),(21,'MIC Manufacturing',6,118,72),(22,'TIOTIO Corporation',10,123,71),(23,'Compute Incorporated',10,126,56),(24,'Sechnology Research',1,127,44),(25,'COR Technology',2,156,43),(26,'Tectoduct Partners',7,175,36),(27,'Miconatione Design',5,184,29),(28,'Cromed Corporation',2,194,25),(29,'Ants Consulting',5,200,12),(30,'Systro',7,231,8),(31,'Systems Corporation',9,242,1),(32,'Macrosoft',1,NULL,NULL);

/*Table structure for table `fc_computers` */

DROP TABLE IF EXISTS `fc_computers`;

CREATE TABLE `fc_computers` (
  `computer_id` bigint(20) unsigned NOT NULL auto_increment,
  `owner_id` bigint(20) unsigned NOT NULL COMMENT 'ID of the owner this computer belongs ',
  `ownertype_id` smallint(5) unsigned NOT NULL COMMENT 'Type of owner (refer to fc_ownertypes)',
  `processor_id` int(10) unsigned NOT NULL COMMENT 'Processor installed in the computer',
  `name` varchar(32) default NULL COMMENT '[optional] Name of computer given by player',
  `memory_id` int(10) unsigned NOT NULL COMMENT 'ID of the memory item that is installed in this machine',
  `os_id` int(10) unsigned NOT NULL COMMENT 'Operating system installed on machine',
  `harddrive_size` bigint(20) unsigned NOT NULL COMMENT 'Amount of storage space on machine (in MB)',
  `network_speed` int(10) unsigned NOT NULL COMMENT 'Speed of network connection (MBits)',
  PRIMARY KEY  (`computer_id`)
) ENGINE=InnoDB AUTO_INCREMENT=36 DEFAULT CHARSET=latin1;

/*Data for the table `fc_computers` */

insert  into `fc_computers`(`computer_id`,`owner_id`,`ownertype_id`,`processor_id`,`name`,`memory_id`,`os_id`,`harddrive_size`,`network_speed`) values (1,1,1,11,'GM Computer',19,12,1048576,100),(2,2,1,1,'TestUser PC',15,13,81920,4),(3,3,1,1,NULL,15,14,81920,4),(4,1,3,1,'Anion Gateway',15,13,81920,4),(5,2,3,1,'CON Development',15,13,81920,4),(6,3,3,1,'Advan Development',15,13,81920,4),(7,4,3,1,'Incoak Group',15,14,81920,4),(8,5,3,1,'Equic Capital',15,14,81920,4),(9,6,3,1,'Tass Consulting',15,14,81920,4),(10,7,3,1,'COA Ventures',15,14,81920,4),(11,8,3,1,'Mango Corporation',15,14,81920,4),(12,9,3,1,'Quant Research',15,14,81920,4),(13,10,3,1,'RAM Consulting',15,14,81920,4),(14,11,3,1,'Micon Research',15,14,81920,4),(15,12,3,1,'Visilinx Incorporated',15,14,81920,4),(16,13,3,1,'Zilinett',15,14,81920,4),(17,14,3,1,'Catmel Asia',15,14,81920,4),(18,15,3,1,'Belluted Systems',15,14,81920,4),(19,16,3,1,'Citas Asia',15,14,81920,4),(20,17,3,1,'Cadvanducts Ventures',15,14,81920,4),(21,18,3,1,'Softwart Capital',15,14,81920,4),(22,19,3,1,'Klated USA',15,14,81920,4),(23,20,3,1,'Soft Design',15,14,81920,4),(24,21,3,1,'MIC Manufacturing',15,14,81920,4),(25,22,3,1,'TIOTIO Corporation',15,14,81920,4),(26,23,3,1,'Compute Incorporated',15,14,81920,4),(27,24,3,1,'Sechnology Research',15,14,81920,4),(28,25,3,1,'COR Technology',15,14,81920,4),(29,26,3,1,'Tectoduct Partners',15,14,81920,4),(30,27,3,1,'Miconatione Design',15,14,81920,4),(31,28,3,1,'Cromed Corporation',15,14,81920,4),(32,29,3,1,'Ants Consulting',15,13,81920,4),(33,30,3,1,'Systro',15,13,81920,4),(34,31,3,1,'Systems Corporationg',15,13,81920,4),(35,32,3,1,'Macrosoft',15,13,81920,4);

/*Table structure for table `fc_countries` */

DROP TABLE IF EXISTS `fc_countries`;

CREATE TABLE `fc_countries` (
  `country_id` int(10) unsigned NOT NULL auto_increment,
  `name` varchar(64) NOT NULL,
  `IP_groupA` smallint(5) unsigned NOT NULL COMMENT 'Group A IP block for country',
  PRIMARY KEY  (`country_id`,`IP_groupA`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin1;

/*Data for the table `fc_countries` */

insert  into `fc_countries`(`country_id`,`name`,`IP_groupA`) values (1,'USA',216),(2,'Russia',65),(3,'Netherlands',163),(4,'United Kingdom',229),(5,'Australia',124),(6,'China',45),(7,'South Africa',196),(8,'Japan',86),(9,'Nigeria',176),(10,'France',168),(11,'Brazil',79);

/*Table structure for table `fc_forumcategories` */

DROP TABLE IF EXISTS `fc_forumcategories`;

CREATE TABLE `fc_forumcategories` (
  `forumcat_id` int(10) unsigned NOT NULL auto_increment,
  `parent_id` int(11) default NULL COMMENT '[Optional] Parent Category ID',
  `order` int(10) unsigned NOT NULL COMMENT 'Order of display',
  `name` varchar(32) NOT NULL COMMENT 'Name of the category',
  `description` varchar(256) default NULL COMMENT 'Description of category',
  `accounttype_required` int(10) unsigned NOT NULL COMMENT 'account type required to see the category',
  `min_level` int(10) unsigned NOT NULL COMMENT 'minimum level required to see this category',
  `max_level` int(11) default NULL COMMENT 'max level that could see this category',
  PRIMARY KEY  (`forumcat_id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=latin1;

/*Data for the table `fc_forumcategories` */

insert  into `fc_forumcategories`(`forumcat_id`,`parent_id`,`order`,`name`,`description`,`accounttype_required`,`min_level`,`max_level`) values (1,NULL,1,'General','All game related posts are made in these categories',1,1,100),(2,1,1,'Tutorial Missions','The introductory tutorial missions can be found here',1,1,5),(3,1,2,'Basic Missions','Basic Missions for the early levels..',1,1,15),(4,1,3,'General Missions','Missions here are available to all levels of players',1,1,100),(5,NULL,2,'Q&A',NULL,1,1,100),(6,5,1,'Beginner Questions','All beginner questions should be asked here...',1,1,100),(7,5,2,'General Questions','General game related questions go here',1,1,100),(8,1,4,'News','Game related news',1,1,100);

/*Table structure for table `fc_forumposts` */

DROP TABLE IF EXISTS `fc_forumposts`;

CREATE TABLE `fc_forumposts` (
  `forumpost_id` bigint(20) unsigned NOT NULL auto_increment,
  `parentpost_id` bigint(20) default NULL COMMENT 'id of the parent thread post',
  `forumcategory_id` bigint(20) unsigned NOT NULL COMMENT 'category that this post belongs to',
  `order` int(11) NOT NULL COMMENT 'this posts order in the thread',
  `title` varchar(255) default NULL COMMENT 'Title of the thread',
  `author_id` bigint(20) NOT NULL COMMENT 'ID of the author of the thread',
  `content` text COMMENT 'Content of the thread post',
  `date_created` datetime NOT NULL COMMENT 'Datetime of thread creation',
  `mission_id` bigint(20) unsigned default NULL COMMENT 'Set if this post is tied to a mission',
  PRIMARY KEY  (`forumpost_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

/*Data for the table `fc_forumposts` */

insert  into `fc_forumposts`(`forumpost_id`,`parentpost_id`,`forumcategory_id`,`order`,`title`,`author_id`,`content`,`date_created`,`mission_id`) values (1,NULL,2,1,'Your first mission',1,'This is your first mission','2008-12-11 11:44:41',1);

/*Table structure for table `fc_items` */

DROP TABLE IF EXISTS `fc_items`;

CREATE TABLE `fc_items` (
  `item_id` bigint(20) unsigned NOT NULL auto_increment,
  `item_name` char(64) default NULL,
  `itemtype_id` int(10) unsigned NOT NULL COMMENT 'type of item (tb_itemtypes)',
  `object_id` bigint(20) unsigned NOT NULL COMMENT 'ID of the object in the table corresponding to the item type',
  `description` text,
  `min_level` int(10) unsigned NOT NULL COMMENT 'minimum level that this item can be created for',
  `max_level` int(10) unsigned default NULL COMMENT 'maximum level that this item can be created for',
  `npc_value` bigint(20) unsigned NOT NULL COMMENT 'Items NPC value',
  PRIMARY KEY  (`item_id`)
) ENGINE=InnoDB AUTO_INCREMENT=20 DEFAULT CHARSET=latin1;

/*Data for the table `fc_items` */

insert  into `fc_items`(`item_id`,`item_name`,`itemtype_id`,`object_id`,`description`,`min_level`,`max_level`,`npc_value`) values (1,'PowerCore SC12',1,1,'This is the entry level processor for computers. It will do what you want, when you want it, but not much else.',1,NULL,150),(2,'PowerCore SC15',1,2,NULL,3,NULL,500),(3,'PowerCore SC20',1,3,NULL,7,NULL,1000),(4,'PowerCore SC24',1,4,NULL,10,NULL,3000),(5,'PowerCore SC28',1,5,NULL,15,NULL,8500),(6,'PowerCore SC32',1,6,NULL,20,NULL,15000),(7,'PowerCore DC18',1,7,NULL,20,NULL,30000),(8,'PowerCore DC20',1,8,NULL,25,NULL,60000),(9,'PowerCore DC22',1,9,NULL,30,NULL,100000),(10,'PowerCore DC25',1,10,NULL,35,NULL,500000),(11,'PowerCore DC28',1,11,NULL,40,NULL,1000000),(12,'RageOS',2,1,NULL,1,NULL,50),(13,'CritOS',2,2,NULL,1,NULL,50),(14,'ClarityOS',2,3,NULL,1,NULL,50),(15,'MegaRam 1GB',3,1,NULL,1,NULL,500),(16,'MegaRam 2GB',3,2,NULL,15,NULL,1000),(17,'MegaRam 4GB',3,3,NULL,25,NULL,5000),(18,'MegaRam 8GB',3,4,NULL,35,NULL,15000),(19,'MegaRam 16GB',3,5,NULL,50,NULL,35000);

/*Table structure for table `fc_itemtypes` */

DROP TABLE IF EXISTS `fc_itemtypes`;

CREATE TABLE `fc_itemtypes` (
  `itemtype_id` int(10) unsigned NOT NULL auto_increment,
  `itemtype_name` varchar(32) NOT NULL,
  `itemtype_table` varchar(32) NOT NULL COMMENT 'sql table that stores items of this type',
  PRIMARY KEY  (`itemtype_id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=latin1;

/*Data for the table `fc_itemtypes` */

insert  into `fc_itemtypes`(`itemtype_id`,`itemtype_name`,`itemtype_table`) values (1,'Processor','fc_processors'),(2,'Operating System','fc_operatingsystems'),(3,'Memory Module','fc_memory'),(4,'Network Module','fc_networkmodules'),(5,'Storage Devices','fc_storagedevices'),(6,'Software','fc_software'),(7,'Data','fc_data'),(8,'Miscellaneous','fc_miscellaneous');

/*Table structure for table `fc_memory` */

DROP TABLE IF EXISTS `fc_memory`;

CREATE TABLE `fc_memory` (
  `memory_id` smallint(5) unsigned NOT NULL auto_increment,
  `memory_size` int(10) unsigned NOT NULL COMMENT 'Size of Memory Module in MB',
  PRIMARY KEY  (`memory_id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=latin1;

/*Data for the table `fc_memory` */

insert  into `fc_memory`(`memory_id`,`memory_size`) values (1,1024),(2,2048),(3,4096),(4,8192),(5,16384);

/*Table structure for table `fc_missions` */

DROP TABLE IF EXISTS `fc_missions`;

CREATE TABLE `fc_missions` (
  `mission_id` int(10) unsigned NOT NULL,
  `parentmission_id` int(10) unsigned default NULL COMMENT 'Parent ID for sub quests of a main mission',
  `min_level` int(11) default '1' COMMENT 'minimum character level when mission becomes available',
  `max_level` int(11) default '100' COMMENT 'maximum character level before mission stops being available',
  `difficulty` smallint(5) unsigned NOT NULL COMMENT 'difficulty rating of the mission (1-10)',
  `success_event_id` varchar(32) NOT NULL COMMENT 'event that will indicate success of a mission',
  `failure_event_id` varchar(32) default NULL COMMENT '[optional] event that will cause mission to fail',
  `success_count` smallint(5) unsigned NOT NULL default '1' COMMENT 'Number of times the success event needs to fire to succeed in the mission',
  `failure_count` smallint(5) unsigned NOT NULL default '1' COMMENT 'number of times the failure event needs to fire to fail the mission',
  PRIMARY KEY  (`mission_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `fc_missions` */

insert  into `fc_missions`(`mission_id`,`parentmission_id`,`min_level`,`max_level`,`difficulty`,`success_event_id`,`failure_event_id`,`success_count`,`failure_count`) values (1,NULL,1,NULL,1,'(child).Mission.Complete',NULL,0,0),(2,1,1,NULL,1,'FileSystem.FileListing',NULL,1,1),(3,1,1,NULL,1,'FileSystem.ChangeDir',NULL,1,1),(4,1,1,NULL,1,'FileSystem.OSVersion',NULL,1,1);

/*Table structure for table `fc_npcs` */

DROP TABLE IF EXISTS `fc_npcs`;

CREATE TABLE `fc_npcs` (
  `npc_id` bigint(20) unsigned NOT NULL auto_increment,
  `name` char(64) NOT NULL COMMENT 'NPC''s name',
  `level` int(10) unsigned NOT NULL COMMENT 'level of the NPC',
  `email` char(64) default NULL COMMENT 'NPC''s email address (if any)',
  `phone_number` char(20) default NULL COMMENT 'NPC''s phone number (if any)',
  PRIMARY KEY  (`npc_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `fc_npcs` */

/*Table structure for table `fc_operatingsystems` */

DROP TABLE IF EXISTS `fc_operatingsystems`;

CREATE TABLE `fc_operatingsystems` (
  `os_id` int(10) unsigned NOT NULL auto_increment,
  `oskernel_id` smallint(5) unsigned NOT NULL COMMENT 'Kernel Type of the OS (this is for software compatibility)',
  PRIMARY KEY  (`os_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

/*Data for the table `fc_operatingsystems` */

insert  into `fc_operatingsystems`(`os_id`,`oskernel_id`) values (1,1),(2,2),(3,3);

/*Table structure for table `fc_oskernels` */

DROP TABLE IF EXISTS `fc_oskernels`;

CREATE TABLE `fc_oskernels` (
  `oskernel_id` smallint(5) unsigned NOT NULL auto_increment,
  `name` varchar(32) NOT NULL COMMENT 'Name of the operating system kernel',
  PRIMARY KEY  (`oskernel_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

/*Data for the table `fc_oskernels` */

insert  into `fc_oskernels`(`oskernel_id`,`name`) values (1,'Fury'),(2,'Havok'),(3,'Crystal');

/*Table structure for table `fc_ownertypes` */

DROP TABLE IF EXISTS `fc_ownertypes`;

CREATE TABLE `fc_ownertypes` (
  `ownertype_id` smallint(5) unsigned NOT NULL,
  `ownertype` varchar(32) NOT NULL COMMENT 'description of the owner type',
  PRIMARY KEY  (`ownertype_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `fc_ownertypes` */

insert  into `fc_ownertypes`(`ownertype_id`,`ownertype`) values (1,'Player'),(2,'NPC'),(3,'Company');

/*Table structure for table `fc_processors` */

DROP TABLE IF EXISTS `fc_processors`;

CREATE TABLE `fc_processors` (
  `processor_id` int(10) unsigned NOT NULL auto_increment,
  `core_count` smallint(5) unsigned NOT NULL COMMENT 'Number of cores on the processor',
  `core_speed` smallint(5) unsigned NOT NULL COMMENT 'Speed of cores (GHz)',
  PRIMARY KEY  (`processor_id`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin1;

/*Data for the table `fc_processors` */

insert  into `fc_processors`(`processor_id`,`core_count`,`core_speed`) values (1,1,1200),(2,1,1500),(3,1,2000),(4,1,2400),(5,1,2800),(6,1,3200),(7,2,1800),(8,2,2000),(9,2,2200),(10,2,2500),(11,2,2800);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;