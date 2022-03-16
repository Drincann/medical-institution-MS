use ms;
/* DROP TABLE IF EXISTS `Staff`;
drop table if exists `Patient`;
drop table if exists `FeedBackToPatient`;
drop table if exists `FeedBackToDoctor`;
drop table if exists `SignIn`; */
create table Staff(
  id int not null auto_increment,
  username varchar(255) not null,
  password varchar(255) not null,
  type int not null,
  -- 0 admin 1 front 2 doctor 3 nurse 4 buyer
  primary key(id)
);
create table Patient(
  id int not null auto_increment,
  username varchar(255) not null,
  password varchar(255) not null,
  -- from table VisitRecord -> visited  tinyint(1) not null,
  situation varchar(1024) not null,
  injected tinyint(1) not null,
  primary key(id)
);
create table FeedBackToPatient(
  id int not null auto_increment,
  patientid int not null,
  doctorid int not null,
  advice varchar(1024) not null,
  primary key(id),
  foreign key (patientid) references Patient(id),
  foreign key (doctorid) references Staff(id)
);
create table FeedBackToDoctor(
  id int not null auto_increment,
  patientid int not null,
  doctorid int not null,
  advice varchar(1024) not null,
  primary key(id),
  foreign key (patientid) REFERENCES Patient(id),
  foreign key (doctorid) REFERENCES Staff(id)
);
create table SignIn(
  id int not null auto_increment,
  staffId int not null,
  type int not null,
  -- 0 signin 1 signout
  primary key(id),
  foreign key (staffId) REFERENCES Staff(id)
);
create table Info(
  id int not null auto_increment,
  name varchar(255) not null,
  -- 医院名称
  primary key(id)
);
create table Reservation(
  id int not null auto_increment,
  patientid int not null,
  doctorid int not null,
  -- optional -> time varchar(255) not null,
  primary key(id),
  foreign key (patientId) REFERENCES Patient(id),
  foreign key (doctorid) REFERENCES Staff(id)
);
create table VisitRecord(
  id int not null auto_increment,
  patientid int not null,
  doctorid int not null,
  -- optional -> time varchar(255) not null,
  primary key(id),
  foreign key (patientid) REFERENCES Patient(id),
  foreign key (doctorid) REFERENCES Staff(id)
);
create table MedicationRecord(
  id int not null auto_increment,
  patientid int not null,
  doctorid int not null,
  dosage varchar(255) not null,
  primary key(id),
  foreign key (patientid) REFERENCES Patient(id),
  foreign key (doctorid) REFERENCES Staff(id)
);
create table NurseMemo(
  id int not null auto_increment,
  message varchar(255) not null,
  nurseid int not null,
  primary key(id),
  foreign key (nurseid) REFERENCES Staff(id)
);
create table CashierRecord(
  id int not null auto_increment,
  patientid int not null,
  amount float not null,
  createTime datetime not null DEFAULT CURRENT_TIMESTAMP,
  primary key(id),
  foreign key (patientid) REFERENCES Patient(id)
);
create table VIP(
  id int not null auto_increment,
  patientid int not null,
  points int not null DEFAULT 0,
  primary key(id),
  foreign key (patientid) REFERENCES Patient(id)
);
create table Emergency(
  id int not null auto_increment,
  patientid int not null,
  message varchar(255) not null,
  primary key(id),
  foreign key (patientid) REFERENCES Patient(id)
);
create table PurchaseRecord(
  id int not null auto_increment,
  message varchar(255) not null,
  primary key(id)
);