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

/*Data for the table `config_anim_group` */

LOCK TABLES `config_anim_group` WRITE;

insert  into `config_anim_group`(`Name`,`movement`,`attack_left`,`attack_right`,`attack_power`,`block_hit`,`block_idle`,`block_bash`,`jump_start`,`jump_fall`,`jump_land`) values ('ag_1','/Game/meshes/actors/charactor/animations/female/female_move_mt.female_move_mt','','','','','','','/Game/meshes/actors/charactor/animations/mt_jumpfast.mt_jumpfast','/Game/meshes/actors/charactor/animations/mt_jumpfall.mt_jumpfall','/Game/meshes/actors/charactor/animations/mt_jumpland.mt_jumpland'),('ag_2','/Game/meshes/actors/charactor/animations/1hm_female_move.1hm_female_move','','','','/Game/meshes/actors/charactor/animations/1hm_blockhit_Montage.1hm_blockhit_Montage','/Game/meshes/actors/charactor/animations/1hm_blockidle.1hm_blockidle','/Game/meshes/actors/charactor/animations/1hm_blockbash_Montage.1hm_blockbash_Montage','/Game/meshes/actors/charactor/animations/mt_jumpfast.mt_jumpfast','/Game/meshes/actors/charactor/animations/mt_jumpfall.mt_jumpfall','/Game/meshes/actors/charactor/animations/mt_jumpland.mt_jumpland'),('ag_3','/Game/meshes/actors/charactor/animations/2hm_female_move.2hm_female_move','','','','/Game/meshes/actors/charactor/animations/2hm_blockhit_Montage.2hm_blockhit_Montage','/Game/meshes/actors/charactor/animations/2hm_blockidle.2hm_blockidle','/Game/meshes/actors/charactor/animations/2hm_blockbash_Montage.2hm_blockbash_Montage','/Game/meshes/actors/charactor/animations/mt_jumpfast.mt_jumpfast','/Game/meshes/actors/charactor/animations/mt_jumpfall.mt_jumpfall','/Game/meshes/actors/charactor/animations/mt_jumpland.mt_jumpland'),('ag_4','/Game/meshes/actors/charactor/animations/bow_female_move.bow_female_move','','','','/Game/meshes/actors/charactor/animations/bow_blockhit_Montage.bow_blockhit_Montage','/Game/meshes/actors/charactor/animations/bow_blockidle.bow_blockidle','/Game/meshes/actors/charactor/animations/bow_blockbash_Montage.bow_blockbash_Montage','/Game/meshes/actors/charactor/animations/mt_jumpfast.mt_jumpfast','/Game/meshes/actors/charactor/animations/mt_jumpfall.mt_jumpfall','/Game/meshes/actors/charactor/animations/mt_jumpland.mt_jumpland');

UNLOCK TABLES;

/*Table structure for table `config_armor_map` */

DROP TABLE IF EXISTS `config_armor_map`;

CREATE TABLE `config_armor_map` (
  `Name` char(125) NOT NULL,
  `valid` int(11) NOT NULL,
  `model` char(255) NOT NULL,
  `append_1` char(255) NOT NULL,
  `append_2` char(255) NOT NULL,
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

/*Data for the table `config_armor_map` */

LOCK TABLES `config_armor_map` WRITE;

insert  into `config_armor_map`(`Name`,`valid`,`model`,`append_1`,`append_2`) values ('item_21_race_1',1,'/Game/meshes/armor/iron/f/helmet.helmet','',''),('item_21_race_2',1,'/Game/meshes/armor/iron/f/helmet.helmet','',''),('item_22_race_1',1,'/Game/meshes/armor/iron/f/cuirasslight_0.cuirasslight_0','',''),('item_22_race_2',1,'/Game/meshes/armor/iron/f/cuirasslight_0.cuirasslight_0','',''),('item_23_race_1',1,'/Game/meshes/armor/iron/f/gauntlets_0.gauntlets_0','',''),('item_23_race_2',1,'/Game/meshes/armor/iron/f/gauntlets_0.gauntlets_0','',''),('item_24_race_1',1,'/Game/meshes/armor/iron/f/boost_0.boost_0','',''),('item_24_race_2',1,'/Game/meshes/armor/iron/f/boost_0.boost_0','',''),('item_27_race_1',1,'/Game/meshes/armor/steel/f/helmet_0.helmet_0','',''),('item_27_race_2',1,'/Game/meshes/armor/steel/f/helmet_0.helmet_0','',''),('item_28_race_1',1,'/Game/meshes/armor/steel/f/cuirasslight_0.cuirasslight_0','',''),('item_28_race_2',1,'/Game/meshes/armor/steel/f/cuirasslight_0.cuirasslight_0','',''),('item_29_race_1',1,'/Game/meshes/armor/steel/f/gauntlets_0.gauntlets_0','',''),('item_29_race_2',1,'/Game/meshes/armor/steel/f/gauntlets_0.gauntlets_0','',''),('item_30_race_1',1,'/Game/meshes/armor/steel/f/boots_0.boots_0','',''),('item_30_race_2',1,'/Game/meshes/armor/steel/f/boots_0.boots_0','','');

UNLOCK TABLES;

/*Table structure for table `config_effect` */

DROP TABLE IF EXISTS `config_effect`;

CREATE TABLE `config_effect` (
  `Name` char(125) NOT NULL,
  `valid` int(11) NOT NULL,
  `start_self_anim` char(255) NOT NULL,
  `start_weapon_anim` char(255) NOT NULL,
  `fly_body` char(255) NOT NULL,
  `hit_anim` char(255) NOT NULL,
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

/*Data for the table `config_effect` */

LOCK TABLES `config_effect` WRITE;

insert  into `config_effect`(`Name`,`valid`,`start_self_anim`,`start_weapon_anim`,`fly_body`,`hit_anim`) values ('skill_1_race_1',1,'/Game/meshes/actors/charactor/animations/1hm_am_attackleft.1hm_am_attackleft','','','/Game/meshes/actors/charactor/animations/1hm_staggerbacksmall_Montage.1hm_staggerbacksmall_Montage'),('skill_1_race_2',1,'/Game/meshes/actors/charactor/animations/1hm_am_attackleft.1hm_am_attackleft','','','/Game/meshes/actors/charactor/animations/1hm_staggerbacksmall_Montage.1hm_staggerbacksmall_Montage'),('skill_2_race_1',1,'/Game/meshes/actors/charactor/animations/1hm_am_attackright.1hm_am_attackright','','','/Game/meshes/actors/charactor/animations/1hm_staggerbacksmall_Montage.1hm_staggerbacksmall_Montage'),('skill_2_race_2',1,'/Game/meshes/actors/charactor/animations/1hm_am_attackright.1hm_am_attackright','','','/Game/meshes/actors/charactor/animations/1hm_staggerbacksmall_Montage.1hm_staggerbacksmall_Montage'),('skill_3_race_1',1,'/Game/meshes/actors/charactor/animations/1hm_am_attackpower.1hm_am_attackpower','','','/Game/meshes/actors/charactor/animations/1hm_staggerbacksmall_Montage.1hm_staggerbacksmall_Montage'),('skill_3_race_2',1,'/Game/meshes/actors/charactor/animations/1hm_am_attackpower.1hm_am_attackpower','','','/Game/meshes/actors/charactor/animations/1hm_staggerbacksmall_Montage.1hm_staggerbacksmall_Montage');

UNLOCK TABLES;

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
  `double_hand` int(11) NOT NULL,
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

/*Data for the table `config_equip` */

LOCK TABLES `config_equip` WRITE;

insert  into `config_equip`(`Name`,`valid`,`slot`,`patk_plus`,`matk_plus`,`hp_plus`,`pdef_plus`,`mdef_plus`,`double_hand`) values ('equip_1',1,0,0,0,800,300,300,0),('equip_10',1,3,0,0,600,400,600,0),('equip_11',1,4,2600,600,0,0,0,0),('equip_12',1,4,3600,0,0,0,0,1),('equip_2',1,1,0,0,1000,800,300,0),('equip_3',1,2,0,0,500,600,300,0),('equip_4',1,3,0,0,300,200,300,0),('equip_5',1,4,1300,600,0,0,0,0),('equip_6',0,0,0,0,0,0,0,0),('equip_7',1,0,0,0,1600,600,600,0),('equip_8',1,1,0,0,2000,1600,600,0),('equip_9',1,2,0,0,1000,1200,600,0),('equip_steel_bow',1,5,3300,0,0,0,0,1);

UNLOCK TABLES;

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

/*Data for the table `config_item` */

LOCK TABLES `config_item` WRITE;

insert  into `config_item`(`Name`,`valid`,`show_name`,`description`,`icon`,`type`,`child_key`,`arg1`,`arg2`,`arg3`,`arg4`,`max`,`can_sell`,`price`,`can_use`,`script`) values ('item_1',1,'金币','交易货币','',0,'',0,0,0,'',9999999,0,0,0,''),('item_10',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_11',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_12',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_13',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_14',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_15',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_16',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_17',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_18',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_19',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_2',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_20',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_21',1,'帝国头盔','','',2,'equip_1',0,0,0,'',99,0,0,0,''),('item_22',1,'帝国盔甲','','',2,'equip_2',0,0,0,'',99,0,0,0,''),('item_23',1,'帝国手套','','',2,'equip_3',0,0,0,'',99,0,0,0,''),('item_24',1,'帝国靴子','','',2,'equip_4',0,0,0,'',99,0,0,0,''),('item_25',1,'帝国剑','','',2,'equip_5',0,0,0,'',99,0,0,0,''),('item_26',0,'','','',2,'equip_0',0,0,0,'',99,0,0,0,''),('item_27',1,'钢盔','','',2,'equip_7',0,0,0,'',99,0,0,0,''),('item_28',1,'钢甲','','',2,'equip_8',0,0,0,'',99,0,0,0,''),('item_29',1,'钢手套','','',2,'equip_9',0,0,0,'',99,0,0,0,''),('item_3',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_30',1,'钢靴','','',2,'equip_10',0,0,0,'',99,0,0,0,''),('item_31',1,'钢剑','','',2,'equip_11',0,0,0,'',99,0,0,0,''),('item_32',1,'双手大剑','','',2,'equip_12',0,0,0,'',99,0,0,0,''),('item_33',1,'钢弓','','',2,'equip_steel_bow',0,0,0,'',99,0,0,0,''),('item_34',0,'','','',2,'equip_0',0,0,0,'',99,0,0,0,''),('item_35',0,'','','',2,'equip_0',0,0,0,'',99,0,0,0,''),('item_36',0,'','','',2,'equip_0',0,0,0,'',99,0,0,0,''),('item_37',0,'','','',2,'equip_0',0,0,0,'',99,0,0,0,''),('item_38',0,'','','',2,'equip_0',0,0,0,'',99,0,0,0,''),('item_39',0,'','','',2,'equip_0',0,0,0,'',99,0,0,0,''),('item_4',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_40',0,'','','',2,'equip_0',0,0,0,'',99,0,0,0,''),('item_41',0,'','','',2,'equip_0',0,0,0,'',99,0,0,0,''),('item_5',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_6',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_7',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_8',0,'','','',0,'',0,0,0,'',0,0,0,0,''),('item_9',0,'','','',0,'',0,0,0,'',0,0,0,0,'');

UNLOCK TABLES;

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

/*Data for the table `config_race` */

LOCK TABLES `config_race` WRITE;

insert  into `config_race`(`Name`,`valid`,`show_name`,`description`,`sex`,`anim_group`,`default_head`,`default_body`,`default_hand`,`default_feet`) values ('race_1',1,'人族','女性人族',0,'ag_1','/Game/meshes/actors/charactor/female_0/femalehead.femalehead','/Game/meshes/actors/charactor/female_0/femalebody_0.femalebody_0','/Game/meshes/actors/charactor/female_0/femalehands_0.femalehands_0','/Game/meshes/actors/charactor/female_0/femalefeet_0.femalefeet_0'),('race_2',1,'人族','男性人族',1,'ag_1','/Game/meshes/actors/charactor/female_0/femalehead.femalehead','/Game/meshes/actors/charactor/female_0/femalebody_0.femalebody_0','/Game/meshes/actors/charactor/female_0/femalehands_0.femalehands_0','/Game/meshes/actors/charactor/female_0/femalefeet_0.femalefeet_0');

UNLOCK TABLES;

/*Table structure for table `config_skill` */

DROP TABLE IF EXISTS `config_skill`;

CREATE TABLE `config_skill` (
  `Name` char(125) NOT NULL,
  `valid` int(11) NOT NULL,
  `show_name` char(255) NOT NULL,
  `description` char(255) NOT NULL,
  `cd` float NOT NULL,
  `patk_percent` float NOT NULL,
  `patk_plus` int(11) NOT NULL,
  `matk_percent` float NOT NULL,
  `matk_plus` int(11) NOT NULL,
  `target_maxhp_percent` float NOT NULL,
  `mp_cost` int(11) NOT NULL,
  `sp_cost` int(11) NOT NULL,
  `interrupt` float NOT NULL,
  `hit_recover` float NOT NULL,
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

/*Data for the table `config_skill` */

LOCK TABLES `config_skill` WRITE;

insert  into `config_skill`(`Name`,`valid`,`show_name`,`description`,`cd`,`patk_percent`,`patk_plus`,`matk_percent`,`matk_plus`,`target_maxhp_percent`,`mp_cost`,`sp_cost`,`interrupt`,`hit_recover`) values ('skill_1',1,'左劈斩','',2,1.2,0,0,0,0,0,0,0,0),('skill_2',1,'右劈斩','',2,1.2,0,0,0,0,0,0,0,0),('skill_3',1,'重击','',4,2,0,0,0,0,0,50,0,0),('skill_4',0,'','',0,0,0,0,0,0,0,0,0,0);

UNLOCK TABLES;

/*Table structure for table `config_weapon_map` */

DROP TABLE IF EXISTS `config_weapon_map`;

CREATE TABLE `config_weapon_map` (
  `Name` char(125) NOT NULL,
  `valid` int(11) NOT NULL,
  `anim_group` char(125) NOT NULL,
  `model` char(255) NOT NULL,
  `slot` char(255) NOT NULL,
  `append_1` char(255) NOT NULL,
  `slot1` char(255) NOT NULL,
  `open_weapon` char(255) NOT NULL,
  `close_weapon` char(255) NOT NULL,
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

/*Data for the table `config_weapon_map` */

LOCK TABLES `config_weapon_map` WRITE;

insert  into `config_weapon_map`(`Name`,`valid`,`anim_group`,`model`,`slot`,`append_1`,`slot1`,`open_weapon`,`close_weapon`) values ('item_25_race_1',1,'ag_2','/Game/meshes/weapons/iron/longsword/longsword_0.longsword_0','WEAPON','/Game/meshes/weapons/iron/longsword/longsword_Scb.longsword_Scb','WeaponSword','/Game/meshes/actors/charactor/animations/1hm_am_equip.1hm_am_equip','/Game/meshes/actors/charactor/animations/1hm_am_equip_close.1hm_am_equip_close'),('item_25_race_2',1,'ag_2','/Game/meshes/weapons/iron/longsword/longsword_0.longsword_0','WEAPON','/Game/meshes/weapons/iron/longsword/longsword_Scb.longsword_Scb','WeaponSword','/Game/meshes/actors/charactor/animations/1hm_am_equip.1hm_am_equip','/Game/meshes/actors/charactor/animations/1hm_am_equip_close.1hm_am_equip_close'),('item_31_race_1',1,'ag_2','/Game/meshes/weapons/steel/SteelSword/steelsword_0.steelsword_0','WEAPON','/Game/meshes/weapons/steel/SteelSword/steelsword_Scb.steelsword_Scb','WeaponSword','/Game/meshes/actors/charactor/animations/1hm_am_equip.1hm_am_equip','/Game/meshes/actors/charactor/animations/1hm_am_equip_close.1hm_am_equip_close'),('item_31_race_2',1,'ag_2','/Game/meshes/weapons/steel/SteelSword/steelsword_0.steelsword_0','WEAPON','/Game/meshes/weapons/steel/SteelSword/steelsword_Scb.steelsword_Scb','WeaponSword','/Game/meshes/actors/charactor/animations/1hm_am_equip.1hm_am_equip','/Game/meshes/actors/charactor/animations/1hm_am_equip_close.1hm_am_equip_close'),('item_32_race_1',1,'ag_3','/Game/meshes/weapons/steel/steelgreatsword/steelgreatsword.steelgreatsword','WEAPON','','WeaponBack','/Game/meshes/actors/charactor/animations/2hc_equip_Montage.2hc_equip_Montage','/Game/meshes/actors/charactor/animations/2hc_unequip_Montage.2hc_unequip_Montage'),('item_32_race_2',1,'ag_3','/Game/meshes/weapons/steel/steelgreatsword/steelgreatsword.steelgreatsword','WEAPON','','WeaponBack','/Game/meshes/actors/charactor/animations/2hc_equip_Montage.2hc_equip_Montage','/Game/meshes/actors/charactor/animations/2hc_unequip_Montage.2hc_unequip_Montage'),('item_33_race_1',1,'ag_4','/Game/meshes/weapons/steel/steelbow/steelbow.steelbow','SHIELD','/Game/meshes/weapons/steel/steelarrow/steelarrow.steelarrow','WeaponBow','/Game/meshes/actors/charactor/animations/bow_equip_Montage.bow_equip_Montage','/Game/meshes/actors/charactor/animations/bow_unequip_Montage.bow_unequip_Montage'),('item_33_race_2',1,'ag_4','/Game/meshes/weapons/steel/steelbow/steelbow.steelbow','SHIELD','/Game/meshes/weapons/steel/steelarrow/steelarrow.steelarrow','WeaponBow','/Game/meshes/actors/charactor/animations/bow_equip_Montage.bow_equip_Montage','/Game/meshes/actors/charactor/animations/bow_unequip_Montage.bow_unequip_Montage');

UNLOCK TABLES;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
