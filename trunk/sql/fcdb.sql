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
  `acc_name` char(64) NOT NULL COMMENT 'account name',
  `acc_password` char(64) NOT NULL COMMENT 'account password',
  `created` datetime NOT NULL COMMENT 'datetime of account creation',
  `acc_type` smallint(5) unsigned NOT NULL default '0' COMMENT 'type of account (see fc_accounttypes)',
  PRIMARY KEY  (`account_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

/*Data for the table `fc_accounts` */

insert  into `fc_accounts`(`account_id`,`acc_name`,`acc_password`,`created`,`acc_type`) values (1,'root','toor','2008-10-16 14:24:52',0);

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
  PRIMARY KEY  (`character_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `fc_characters` */

/*Table structure for table `fc_items` */

DROP TABLE IF EXISTS `fc_items`;

CREATE TABLE `fc_items` (
  `item_id` bigint(20) unsigned NOT NULL auto_increment,
  `item_name` char(64) NOT NULL,
  `itemtype_id` int(10) unsigned NOT NULL COMMENT 'type of item (tb_itemtypes)',
  `description` text,
  `min_level` int(10) unsigned NOT NULL COMMENT 'minimum level that this item can be created for',
  `max_level` int(10) unsigned default NULL COMMENT 'maximum level that this item can be created for',
  PRIMARY KEY  (`item_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `fc_items` */

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

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
