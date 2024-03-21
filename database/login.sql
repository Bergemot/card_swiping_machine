/*
Navicat MySQL Data Transfer

Source Server         : mysql
Source Server Version : 80033
Source Host           : localhost:3306
Source Database       : login

Target Server Type    : MYSQL
Target Server Version : 80033
File Encoding         : 65001

Date: 2023-07-28 16:29:44
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for login
-- ----------------------------
DROP TABLE IF EXISTS `login`;
CREATE TABLE `login` (
  `username` varchar(45) COLLATE utf8mb4_bin DEFAULT NULL,
  `password` varchar(45) COLLATE utf8mb4_bin DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

-- ----------------------------
-- Records of login
-- ----------------------------
INSERT INTO `login` VALUES ('password', 'password');
INSERT INTO `login` VALUES ('mxy', '123');
INSERT INTO `login` VALUES ('ljt', '123');
