/*
Navicat MySQL Data Transfer

Source Server         : mysql
Source Server Version : 80033
Source Host           : localhost:3306
Source Database       : test

Target Server Type    : MYSQL
Target Server Version : 80033
File Encoding         : 65001

Date: 2023-07-28 16:32:39
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for admins
-- ----------------------------
DROP TABLE IF EXISTS `admins`;
CREATE TABLE `admins` (
  `id` int NOT NULL AUTO_INCREMENT,
  `username` varchar(45) DEFAULT NULL,
  `password` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `userscol_UNIQUE` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb3;

-- ----------------------------
-- Records of admins
-- ----------------------------
INSERT INTO `admins` VALUES ('1', 'admin', 'aaaaa');
INSERT INTO `admins` VALUES ('3', null, null);
INSERT INTO `admins` VALUES ('4', null, null);
INSERT INTO `admins` VALUES ('5', 'admin1', '123');
INSERT INTO `admins` VALUES ('6', 'zyr', '111111');
INSERT INTO `admins` VALUES ('7', 'ssy', '111111');
INSERT INTO `admins` VALUES ('8', 'abc', 'abc');

-- ----------------------------
-- Table structure for members
-- ----------------------------
DROP TABLE IF EXISTS `members`;
CREATE TABLE `members` (
  `id` int NOT NULL AUTO_INCREMENT,
  `stuid` varchar(50) NOT NULL,
  `name` varchar(50) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `cardid` varchar(150) NOT NULL,
  `type` int NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`stuid`),
  UNIQUE KEY `cardid_UNIQUE` (`cardid`),
  UNIQUE KEY `num_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=55 DEFAULT CHARSET=utf8mb3;

-- ----------------------------
-- Records of members
-- ----------------------------
INSERT INTO `members` VALUES ('2', '1181001017', '单世芸', '16285729;', '201');
INSERT INTO `members` VALUES ('3', '1181001022', '孙雪', '6543409', '201');
INSERT INTO `members` VALUES ('4', '1181001026', '翟羽然', '8204177;6058259;', '201');
INSERT INTO `members` VALUES ('5', '1191001021', '任耀民', '3447871316', '201');
INSERT INTO `members` VALUES ('6', '1191001022', '沈书悦', '14313142', '201');
INSERT INTO `members` VALUES ('7', '1191002002', '陈奥运', '2966801354', '201');
INSERT INTO `members` VALUES ('8', '1191002005', '陈馨', '2967696410', '201');
INSERT INTO `members` VALUES ('9', '1191002033', '王梦雅', '2967003402', '201');
INSERT INTO `members` VALUES ('10', '1191002039', '徐扬帆', '2967444298', '201');
INSERT INTO `members` VALUES ('15', '5859', '任仲山', '1173228', '204');
INSERT INTO `members` VALUES ('16', '6027', '王国辉', '2030396', '204');
INSERT INTO `members` VALUES ('19', '111201', '测试用户', '101001', '201');
INSERT INTO `members` VALUES ('24', '1183008026', '张天天', '15011050', '201');
INSERT INTO `members` VALUES ('35', '1181001023', '王伊阳', '336158', '201');
INSERT INTO `members` VALUES ('46', '123456', '111', '2223', '201');
INSERT INTO `members` VALUES ('47', '12345', '小单', '10051459', '201');
INSERT INTO `members` VALUES ('48', '67890', '小世', '228243', '201');
INSERT INTO `members` VALUES ('50', '235', '小丁', '12345', '201');
INSERT INTO `members` VALUES ('52', '1201003028', '穆晓悦', '187188097059', '201');
INSERT INTO `members` VALUES ('53', '1201003023', '刘家彤', '058169098059', '201');

-- ----------------------------
-- Table structure for records
-- ----------------------------
DROP TABLE IF EXISTS `records`;
CREATE TABLE `records` (
  `id` int NOT NULL AUTO_INCREMENT,
  `stuid` varchar(50) DEFAULT NULL,
  `name` varchar(45) DEFAULT NULL,
  `type` varchar(50) DEFAULT NULL,
  `room` varchar(45) DEFAULT NULL,
  `time` varchar(45) DEFAULT NULL,
  `qd` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=393 DEFAULT CHARSET=utf8mb3;

-- ----------------------------
-- Records of records
-- ----------------------------
INSERT INTO `records` VALUES ('285', '1181001026', '翟羽然', '本科生', '330', '2021-08-16 13:18:21', '正常');
INSERT INTO `records` VALUES ('286', '12345', '小单', '本科生', '333', '2021-08-17 14:56:24', '正常');
INSERT INTO `records` VALUES ('287', '1181001017', '单世芸', '本科生', '333', '2021-08-17 14:56:32', '正常');
INSERT INTO `records` VALUES ('288', '1181001026', '翟羽然', '本科生', '330', '2021-08-17 14:56:38', '正常');
INSERT INTO `records` VALUES ('289', '67890', '小世', '本科生', '330', '2021-08-17 14:57:15', '正常');
INSERT INTO `records` VALUES ('290', '13579', '小芸', '教职工', '330', '2021-08-17 14:58:30', '正常');
INSERT INTO `records` VALUES ('291', '1181001017', '单世芸', '本科生', '330', '2021-08-17 14:58:52', '迟到');
INSERT INTO `records` VALUES ('292', '1181001026', '翟羽然', '本科生', '330', '2021-08-17 14:58:57', '迟到');
INSERT INTO `records` VALUES ('293', '13579', '小芸', '教职工', '330', '2021-08-17 14:59:12', '正常');
INSERT INTO `records` VALUES ('294', '67890', '小世', '本科生', '330', '2021-08-17 14:59:18', '正常');
INSERT INTO `records` VALUES ('295', '1181001026', '翟羽然', '本科生', '330', '2021-08-17 14:59:21', '正常');
INSERT INTO `records` VALUES ('296', '12345', '小单', '本科生', '330', '2021-08-18 09:21:01', '正常');
INSERT INTO `records` VALUES ('297', '67890', '小世', '本科生', '330', '2021-08-18 09:47:16', '正常');
INSERT INTO `records` VALUES ('298', '13579', '小芸', '教职工', '333', '2021-08-25 12:25:54', '迟到');
INSERT INTO `records` VALUES ('299', '1181001017', '单世芸', '本科生', '333', '2021-08-25 12:31:51', '迟到');
INSERT INTO `records` VALUES ('300', '1181001026', '翟羽然', '本科生', '333', '2021-08-25 12:33:12', '迟到');
INSERT INTO `records` VALUES ('301', '12345', '小单', '本科生', '333', '2021-08-26 00:55:09', '正常');
INSERT INTO `records` VALUES ('302', '67890', '小世', '本科生', '333', '2021-08-26 00:55:31', '正常');
INSERT INTO `records` VALUES ('303', '12345', '小单', '本科生', '333', '2021-08-26 00:55:51', '正常');
INSERT INTO `records` VALUES ('304', '1181001026', '翟羽然', '本科生', '333', '2021-08-26 00:56:24', '正常');
INSERT INTO `records` VALUES ('305', '13579', '小芸', '教职工', '333', '2021-08-26 00:56:28', '正常');
INSERT INTO `records` VALUES ('306', '1181001026', '翟羽然', '本科生', '333', '2021-08-26 00:57:51', '正常');
INSERT INTO `records` VALUES ('307', '67890', '小世', '本科生', '333', '2021-08-26 00:58:19', '正常');
INSERT INTO `records` VALUES ('308', '1201003028', '穆晓悦', '本科生', '331', '2023-07-17 11:02:39', '正常');
INSERT INTO `records` VALUES ('309', '1201003028', '穆晓悦', '本科生', '313', '2023-07-20 12:46:37', '正常');
INSERT INTO `records` VALUES ('310', '1181001026', '翟羽然', '本科生', '313', '2023-07-20 13:51:51', '正常');
INSERT INTO `records` VALUES ('311', '1201003028', '穆晓悦', '本科生', '313', '2023-07-20 13:52:45', '正常');
INSERT INTO `records` VALUES ('312', '1181001026', '翟羽然', '本科生', '313', '2023-07-20 13:53:02', '正常');
INSERT INTO `records` VALUES ('313', '1201003028', '穆晓悦', '本科生', '313', '2023-07-20 13:56:04', '正常');
INSERT INTO `records` VALUES ('314', '1181001026', '翟羽然', '本科生', '313', '2023-07-20 13:56:11', '正常');
INSERT INTO `records` VALUES ('315', '1201003028', '穆晓悦', '本科生', '313', '2023-07-20 14:03:08', '正常');
INSERT INTO `records` VALUES ('316', '1201003023', '刘家彤', '本科生', '313', '2023-07-20 14:07:21', '正常');
INSERT INTO `records` VALUES ('317', '1201003028', '穆晓悦', '本科生', '313', '2023-07-20 14:07:38', '正常');
INSERT INTO `records` VALUES ('318', '1201003023', '刘家彤', '本科生', '313', '2023-07-20 14:08:25', '正常');
INSERT INTO `records` VALUES ('319', '1201003028', '穆晓悦', '本科生', '313', '2023-07-20 14:09:12', '正常');
INSERT INTO `records` VALUES ('320', '1201003023', '刘家彤', '本科生', '313', '2023-07-20 14:09:36', '正常');
INSERT INTO `records` VALUES ('321', '6666666666', '测试用户', '本科生', '313', '2023-07-20 14:09:39', '正常');
INSERT INTO `records` VALUES ('322', '1201003023', '刘家彤', '本科生', '313', '2023-07-20 14:13:38', '正常');
INSERT INTO `records` VALUES ('323', '6666666666', '测试用户', '本科生', '313', '2023-07-20 14:14:00', '正常');
INSERT INTO `records` VALUES ('324', '1201003028', '穆晓悦', '本科生', '313', '2023-07-20 14:14:04', '正常');
INSERT INTO `records` VALUES ('325', '6666666666', '测试用户', '本科生', '313', '2023-07-20 14:14:20', '正常');
INSERT INTO `records` VALUES ('326', '1201003023', '刘家彤', '本科生', '313', '2023-07-20 14:14:24', '正常');
INSERT INTO `records` VALUES ('327', '6666666666', '测试用户', '本科生', '313', '2023-07-20 14:17:57', '正常');
INSERT INTO `records` VALUES ('328', '1201003023', '刘家彤', '本科生', '313', '2023-07-20 14:23:07', '迟到');
INSERT INTO `records` VALUES ('329', '6666666666', '测试用户', '本科生', '313', '2023-07-20 14:24:41', '迟到');
INSERT INTO `records` VALUES ('330', '1201003023', '刘家彤', '本科生', '313', '2023-07-20 14:25:58', '迟到');
INSERT INTO `records` VALUES ('331', '6666666666', '测试用户', '本科生', '313', '2023-07-20 14:26:09', '迟到');
INSERT INTO `records` VALUES ('332', '1201003028', '穆晓悦', '本科生', '313', '2023-07-20 14:26:25', '迟到');
INSERT INTO `records` VALUES ('333', '1201003023', '刘家彤', '本科生', '313', '2023-07-20 14:26:53', '迟到');
INSERT INTO `records` VALUES ('334', '6666666666', '测试用户', '本科生', '313', '2023-07-20 14:27:12', '迟到');
INSERT INTO `records` VALUES ('335', '1201003023', '刘家彤', '本科生', '313', '2023-07-20 14:27:20', '正常');
INSERT INTO `records` VALUES ('336', '6666666666', '测试用户', '本科生', '313', '2023-07-20 15:15:48', '迟到');
INSERT INTO `records` VALUES ('337', '1201003028', '穆晓悦', '本科生', '313', '2023-07-20 15:17:35', '迟到');
INSERT INTO `records` VALUES ('338', '1201003023', '刘家彤', '本科生', '313', '2023-07-20 15:20:35', '正常');
INSERT INTO `records` VALUES ('339', '6666666666', '测试用户', '本科生', '313', '2023-07-20 15:20:40', '正常');
INSERT INTO `records` VALUES ('343', '6666666666', '测试用户', '本科生', '313', '2023-07-20 16:40:16', '正常');
INSERT INTO `records` VALUES ('344', '1201003028', '穆晓悦', '本科生', '313', '2023-07-20 16:41:22', '正常');
INSERT INTO `records` VALUES ('346', '1201003028', '穆晓悦', '本科生', '313', '2023-07-20 16:46:56', '正常');
INSERT INTO `records` VALUES ('349', '1201003023', '刘家彤', '本科生', '313', '2023-07-20 16:48:25', '正常');
INSERT INTO `records` VALUES ('351', '1201003028', '穆晓悦', '本科生', '313', '2023-07-20 16:53:16', '正常');
INSERT INTO `records` VALUES ('352', '6666666666', '测试用户', '本科生', '313', '2023-07-20 16:53:32', '正常');
INSERT INTO `records` VALUES ('353', '1201003023', '刘家彤', '本科生', '313', '2023-07-20 16:53:38', '正常');
INSERT INTO `records` VALUES ('354', '6666666666', '测试用户', '本科生', '313', '2023-07-20 17:00:46', '正常');
INSERT INTO `records` VALUES ('374', '1201003028', '穆晓悦', '本科生', '313', '2023-07-27 16:22:48', '正常');
INSERT INTO `records` VALUES ('376', '1201003028', '穆晓悦', '本科生', '313', '2023-07-27 16:28:51', '正常');
INSERT INTO `records` VALUES ('383', '1201003028', '穆晓悦', '本科生', '313', '2023-07-27 16:42:00', '正常');
INSERT INTO `records` VALUES ('384', '1201003023', '刘家彤', '本科生', '313', '2023-07-27 16:42:06', '正常');
INSERT INTO `records` VALUES ('387', '1201003023', '刘家彤', '本科生', '313', '2023-07-28 10:13:37', '正常');
INSERT INTO `records` VALUES ('388', '1201003028', '穆晓悦', '本科生', '313', '2023-07-28 10:13:42', '正常');
INSERT INTO `records` VALUES ('389', '1201003023', '刘家彤', '本科生', '313', '2023-07-28 10:29:47', '正常');
INSERT INTO `records` VALUES ('390', '1201003028', '穆晓悦', '本科生', '313', '2023-07-28 10:29:58', '正常');
INSERT INTO `records` VALUES ('391', '1201003028', '穆晓悦', '本科生', '313', '2023-07-28 11:03:56', '迟到');
INSERT INTO `records` VALUES ('392', '1201003023', '刘家彤', '本科生', '313', '2023-07-28 11:04:05', '正常');

-- ----------------------------
-- Table structure for rooms
-- ----------------------------
DROP TABLE IF EXISTS `rooms`;
CREATE TABLE `rooms` (
  `id` int NOT NULL AUTO_INCREMENT,
  `num` int DEFAULT NULL,
  `mac` varchar(250) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=20 DEFAULT CHARSET=utf8mb3;

-- ----------------------------
-- Records of rooms
-- ----------------------------
INSERT INTO `rooms` VALUES ('1', '330', 'C4:5B:BE:61:46:51;C4:5B:BE:61:4E:7E;');
INSERT INTO `rooms` VALUES ('3', '331', 'BC:FF:4D:82:89:90');
INSERT INTO `rooms` VALUES ('6', '332', 'E8:68:E7:80:5D:67');
INSERT INTO `rooms` VALUES ('16', '333', '30:83:98:93:4D:2B');
INSERT INTO `rooms` VALUES ('17', '1', '111');
INSERT INTO `rooms` VALUES ('18', '323', '30:83:98:93:4D:CD');
INSERT INTO `rooms` VALUES ('19', '313', '98:CD:AC:0C:CC:65');

-- ----------------------------
-- Table structure for types
-- ----------------------------
DROP TABLE IF EXISTS `types`;
CREATE TABLE `types` (
  `num` int NOT NULL,
  `key` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

-- ----------------------------
-- Records of types
-- ----------------------------
INSERT INTO `types` VALUES ('201', '本科生');
INSERT INTO `types` VALUES ('204', '教职工');
