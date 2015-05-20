/*
SQLyog Ultimate v11.24 (32 bit)
MySQL - 5.5.25-MariaDB : Database - world2
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`world2` /*!40100 DEFAULT CHARACTER SET utf8mb4 */;

USE `world2`;

/*Table structure for table `config_anim_group` */

DROP TABLE IF EXISTS `config_anim_group`;

CREATE TABLE `config_anim_group` (
  `Name` char(125) NOT NULL,
  `movement` char(255) NOT NULL,
  `attack_left` char(255) NOT NULL,
  `attack_right` char(255) NOT NULL,
  `attack_power` char(255) NOT NULL,
  `block_hit` char(255) NOT NULL,
  `block_idle` char(255) NOT NULL,
  `block_bash` char(255) NOT NULL,
  `jump_start` char(255) NOT NULL,
  `jump_fall` char(255) NOT NULL,
  `jump_land` char(255) NOT NULL,
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

/*Table structure for table `config_armor_map` */

DROP TABLE IF EXISTS `config_armor_map`;

CREATE TABLE `config_armor_map` (
  `Name` char(125) NOT NULL,
  `model` char(255) NOT NULL,
  `append_1` char(255) NOT NULL,
  `append_2` char(255) NOT NULL,
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

/*Table structure for table `config_equip` */

DROP TABLE IF EXISTS `config_equip`;

CREATE TABLE `config_equip` (
  `Name` char(125) NOT NULL,
  `valid` int(11) NOT NULL,
  `slot` int(11) NOT NULL,
  `patk_plus` int(11) NOT NULL,
  `matk_plus` int(11) NOT NULL,
  `hp_plus` int(11) NOT NULL,
  `pdef_plus` int(11) NOT NULL,
  `mdef_plus` int(11) NOT NULL,
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

/*Table structure for table `config_item` */

DROP TABLE IF EXISTS `config_item`;

CREATE TABLE `config_item` (
  `Name` char(125) NOT NULL,
  `valid` int(11) NOT NULL,
  `show_name` char(255) NOT NULL,
  `description` char(255) NOT NULL,
  `icon` char(255) NOT NULL,
  `type` int(11) NOT NULL,
  `child_key` char(125) NOT NULL,
  `arg1` int(11) NOT NULL,
  `arg2` int(11) NOT NULL,
  `arg3` int(11) NOT NULL,
  `arg4` char(255) NOT NULL,
  `max` int(11) NOT NULL,
  `can_sell` int(11) NOT NULL,
  `price` int(11) NOT NULL,
  `can_use` int(11) NOT NULL,
  `script` char(255) NOT NULL,
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

/*Table structure for table `config_race` */

DROP TABLE IF EXISTS `config_race`;

CREATE TABLE `config_race` (
  `Name` char(125) NOT NULL,
  `valid` int(11) NOT NULL,
  `show_name` char(255) NOT NULL,
  `description` char(255) NOT NULL,
  `sex` int(11) NOT NULL,
  `anim_group` char(125) NOT NULL,
  `default_head` char(255) NOT NULL,
  `default_body` char(255) NOT NULL,
  `default_hand` char(255) NOT NULL,
  `default_feet` char(255) NOT NULL,
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

/*Table structure for table `config_weapon_map` */

DROP TABLE IF EXISTS `config_weapon_map`;

CREATE TABLE `config_weapon_map` (
  `Name` char(125) NOT NULL,
  `anim_group` char(125) NOT NULL,
  `model` char(255) NOT NULL,
  `slot` char(255) NOT NULL,
  `append_1` char(255) NOT NULL,
  `slot1` char(255) NOT NULL,
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
