/*
SQLyog Enterprise - MySQL GUI v7.11 
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

insert  into `fc_accounts`(`account_id`,`acc_name`,`acc_password`,`created`,`acc_type`,`email`) values (1,'root','toor','2008-10-16 14:24:52',3,'root@fcell.org'),(2,'testuser','test','2008-10-21 13:26:00',1,'test@fcell.org');

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
  PRIMARY KEY  (`city_id`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin1;

/*Data for the table `fc_cities` */

insert  into `fc_cities`(`city_id`,`country_id`,`name`) values (1,1,'New York'),(2,2,'Moscow'),(3,3,'Amsterdam'),(4,4,'London'),(5,5,'Sydney'),(6,6,'Beijing'),(7,7,'Johannesburg'),(8,8,'Tokyo'),(9,9,'Abuja'),(10,10,'Paris'),(11,11,'Rio de Janeiro');

/*Table structure for table `fc_computers` */

DROP TABLE IF EXISTS `fc_computers`;

CREATE TABLE `fc_computers` (
  `computer_id` bigint(20) unsigned NOT NULL auto_increment,
  `character_id` bigint(20) unsigned NOT NULL COMMENT 'ID of the character this computer belongs ',
  `processor_id` int(10) unsigned NOT NULL COMMENT 'Processor installed in the computer',
  `name` varchar(32) default NULL COMMENT '[optional] Name of computer given by player',
  `memory_size` int(10) unsigned NOT NULL COMMENT 'Amount of Ram (in MB)',
  `os_id` int(10) unsigned NOT NULL COMMENT 'Operating system installed on machine',
  `harddrive_size` bigint(20) unsigned NOT NULL COMMENT 'Amount of storage space on machine (in MB)',
  `network_speed` int(10) unsigned NOT NULL COMMENT 'Speed of network connection (MBits)',
  PRIMARY KEY  (`computer_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

/*Data for the table `fc_computers` */

insert  into `fc_computers`(`computer_id`,`character_id`,`processor_id`,`name`,`memory_size`,`os_id`,`harddrive_size`,`network_speed`) values (1,1,11,'GM Computer',4096,1,1048576,100),(2,2,1,'TestUser PC',1024,2,81920,4),(3,3,1,NULL,1024,3,81920,4);

/*Table structure for table `fc_countries` */

DROP TABLE IF EXISTS `fc_countries`;

CREATE TABLE `fc_countries` (
  `country_id` int(10) unsigned NOT NULL auto_increment,
  `name` varchar(64) NOT NULL,
  PRIMARY KEY  (`country_id`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin1;

/*Data for the table `fc_countries` */

insert  into `fc_countries`(`country_id`,`name`) values (1,'USA'),(2,'Russia'),(3,'Netherlands'),(4,'United Kingdom'),(5,'Australia'),(6,'China'),(7,'South Africa'),(8,'Japan'),(9,'Nigeria'),(10,'France'),(11,'Brazil');

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
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=latin1;

/*Data for the table `fc_items` */

insert  into `fc_items`(`item_id`,`item_name`,`itemtype_id`,`object_id`,`description`,`min_level`,`max_level`,`npc_value`) values (1,'PowerCore SC12',1,1,'This is the entry level processor for computers. It will do what you want, when you want it, but not much else.',1,NULL,120),(2,'PowerCore SC15',1,2,NULL,3,NULL,250),(3,'PowerCore SC20',1,3,NULL,7,NULL,500),(4,'PowerCore SC24',1,4,NULL,10,NULL,800),(5,'PowerCore SC28',1,5,NULL,15,NULL,1500),(6,'PowerCore SC32',1,6,NULL,20,NULL,2000),(7,'PowerCore DC18',1,7,NULL,20,NULL,3500),(8,'PowerCore DC20',1,8,NULL,25,NULL,5000),(9,'PowerCore DC22',1,9,NULL,30,NULL,10000),(10,'PowerCore DC25',1,10,NULL,35,NULL,20000),(11,'PowerCore DC28',1,11,NULL,40,NULL,35000),(12,'RageOS',2,1,NULL,1,NULL,50),(13,'CritOS',2,2,NULL,1,NULL,50),(14,'ClarityOS',2,3,NULL,1,NULL,50);

/*Table structure for table `fc_itemtypes` */

DROP TABLE IF EXISTS `fc_itemtypes`;

CREATE TABLE `fc_itemtypes` (
  `itemtype_id` int(10) unsigned NOT NULL auto_increment,
  `itemtype_name` varchar(32) NOT NULL,
  `itemtype_table` varchar(32) NOT NULL COMMENT 'sql table that stores items of this type',
  PRIMARY KEY  (`itemtype_id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=latin1;

/*Data for the table `fc_itemtypes` */

insert  into `fc_itemtypes`(`itemtype_id`,`itemtype_name`,`itemtype_table`) values (1,'Processor','fc_processors'),(2,'Operating System','fc_operatingsystems'),(3,'Memory Module','fc_memorymodules'),(4,'Network Module','fc_networkmodules'),(5,'Storage Devices','fc_storagedevices'),(6,'Software','fc_software'),(7,'Data','fc_data'),(8,'Miscellaneous','fc_miscellaneous');

/*Table structure for table `fc_missions` */

DROP TABLE IF EXISTS `fc_missions`;

CREATE TABLE `fc_missions` (
  `mission_id` int(10) unsigned NOT NULL auto_increment,
  `parentmission_id` int(10) unsigned default NULL COMMENT 'Parent ID for sub quests of a main mission',
  `name` int(10) unsigned NOT NULL COMMENT 'ID Mission Name',
  `prelude` int(10) unsigned default NULL COMMENT 'ID of Optional mission prelude text',
  `description` int(10) unsigned NOT NULL COMMENT 'ID of mission text',
  `min_level` int(11) default NULL COMMENT 'minimum character level when mission becomes available',
  `max_level` int(11) default NULL COMMENT 'maximum character level before mission stops being available',
  `difficulty` smallint(5) unsigned NOT NULL COMMENT 'difficulty rating of the mission (1-10)',
  `success_event_id` int(10) unsigned NOT NULL COMMENT 'event that will indicate success of a mission',
  `failure_event_id` int(10) unsigned default NULL COMMENT '[optional] event that will cause mission to fail',
  `success_count` smallint(5) unsigned NOT NULL default '1' COMMENT 'Number of times the success event needs to fire to succeed in the mission',
  `failure_count` smallint(5) unsigned NOT NULL default '1' COMMENT 'number of times the failure event needs to fire to fail the mission',
  PRIMARY KEY  (`mission_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `fc_missions` */

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

insert  into `fc_oskernels`(`oskernel_id`,`name`) values (1,'Fury Kernal'),(2,'Havok Kernel'),(3,'CrystalOS');

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
