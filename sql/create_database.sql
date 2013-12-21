--
-- Table structure for table `account`
--

DROP TABLE IF EXISTS `account`;
CREATE TABLE `account` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `email` varchar(45) NOT NULL,
  `pass_hash` varchar(45) NOT NULL,
  `online` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

--
-- Dumping data for table `account`
--

LOCK TABLES `account` WRITE;
INSERT INTO `account` VALUES (1,'test@gmail.com','f7e79ca8eb0b31ee4d5d6c181416667ffee528ed',0),(2,'test2@gmail.com','f7e79ca8eb0b31ee4d5d6c181416667ffee528ed',0);
UNLOCK TABLES;
