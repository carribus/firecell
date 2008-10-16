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
  PRIMARY KEY  (`account_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

/*Data for the table `fc_accounts` */

insert  into `fc_accounts`(`account_id`,`acc_name`,`acc_password`,`created`) values (1,'root','toor','2008-10-16 14:24:52');

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

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
