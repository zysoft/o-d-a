
-- -----------------------------------------------------
-- Table `Company`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `Company` (
  `cid` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `companyName` VARCHAR(255) NOT NULL ,
  PRIMARY KEY (`cid`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `User`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `User` (
  `uid` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `cid` INT UNSIGNED NOT NULL ,
  `fullName` VARCHAR(255) NOT NULL ,
  `login` VARCHAR(255) NOT NULL ,
  `pass` VARCHAR(255) NOT NULL ,
  PRIMARY KEY (`uid`) ,
  INDEX `fk_User_Company` (`cid` ASC) ,
  CONSTRAINT `fk_User_Company`
    FOREIGN KEY (`cid` )
    REFERENCES `Company` (`cid` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Tracker`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `Tracker` (
  `trid` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `trackerName` VARCHAR(255) NOT NULL ,
  `pluginName` VARCHAR(255) NOT NULL ,
  `dbHost` VARCHAR(255) NULL ,
  `dbName` VARCHAR(255) NULL ,
  `dbUser` VARCHAR(255) NULL ,
  `dbPass` VARCHAR(255) NULL ,
  `dbDriver` VARCHAR(50) NOT NULL DEFAULT 'QMYSQL' ,
  PRIMARY KEY (`trid`) )
ENGINE = InnoDB
COMMENT = 'This table describes all enabled trackers and their database connections';


-- -----------------------------------------------------
-- Table `Correspondence`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `Correspondence` (
  `trid` INT UNSIGNED NOT NULL ,
  `uid` INT UNSIGNED NOT NULL ,
  `utrid` INT UNSIGNED NOT NULL COMMENT 'This is user id (uid) in particular tracker' ,
  INDEX `fk_Participant_Tracker1` (`trid` ASC) ,
  INDEX `fk_Participant_User1` (`uid` ASC) ,
  UNIQUE INDEX `k_participant` (`uid` ASC, `trid` ASC) ,
  CONSTRAINT `fk_Participant_Tracker1`
    FOREIGN KEY (`trid` )
    REFERENCES `Tracker` (`trid` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Participant_User1`
    FOREIGN KEY (`uid` )
    REFERENCES `User` (`uid` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;




-- -----------------------------------------------------
-- Default dataset
-- -----------------------------------------------------
REPLACE INTO `Company` VALUES (1, 'Default company');
REPLACE INTO `User` VALUES (1, 1, 'Administrator', 'admin', 'd033e22ae348aeb5660fc2140aec35850c4da997');