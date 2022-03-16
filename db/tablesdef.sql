use ms;
drop table if EXISTS `PurchaseRecord`;
drop table if EXISTS `Emergency`;
drop table if EXISTS `VIP`;
drop table if EXISTS `FeedBackToPatient`;
drop table if EXISTS `FeedBackToDoctor`;
drop table if EXISTS `SignIn`;
drop table if EXISTS `Info`;
drop table if exists `Reservation`;
drop table if EXISTS `VIsitRecord`;
drop table if EXISTS `MedicationRecord`;
drop table if EXISTS `NurseMemo`;
drop table if EXISTS `CashierRecord`;
drop table if EXISTS `Staff`;
drop table if EXISTS `Patient`;
create table Staff(
  username varchar(255) not null,
  password varchar(255) not null,
  type int not null,
  -- 0 admin 1 front 2 doctor 3 nurse 4 buyer
  primary key(username)
);
create table Patient(
  username varchar(255) not null,
  password varchar(255) not null,
  -- from table VisitRecord -> visited  tinyint(1) not null,
  situation varchar(1024) not null DEFAULT '',
  injected tinyint(1) not null DEFAULT 0,
  -- 0 for not 1 for yes
  primary key(username)
);
create table FeedBackToPatient(
  id int not null auto_increment,
  patient varchar(255) not null,
  doctor varchar(255) not null,
  advice varchar(1024) not null,
  primary key(id),
  foreign key (patient) references Patient(username) on delete cascade,
  foreign key (doctor) references Staff(username) on delete cascade
);
create table FeedBackToDoctor(
  id int not null auto_increment,
  patient varchar(255) not null,
  doctor varchar(255) not null,
  situation varchar(1024) not null,
  primary key(id),
  foreign key (patient) REFERENCES Patient(username) on delete cascade,
  foreign key (doctor) REFERENCES Staff(username) on delete cascade
);
create table SignIn(
  id int not null auto_increment,
  staffUsername varchar(255) not null,
  type int not null,
  -- 0 signin 1 signout
  primary key(id),
  foreign key (staffUsername) REFERENCES Staff(username) on delete cascade
);
create table Info(
  id int not null auto_increment,
  name varchar(255) not null,
  -- 医院名称
  primary key(id)
);
create table Reservation(
  id int not null auto_increment,
  patient varchar(255) not null,
  doctor varchar(255) not null,
  -- optional -> time varchar(255) not null,
  primary key(id),
  foreign key (patient) REFERENCES Patient(username) on delete cascade,
  foreign key (doctor) REFERENCES Staff(username) on delete cascade
);
create table VisitRecord(
  id int not null auto_increment,
  patient varchar(255) not null,
  doctor varchar(255) not null,
  -- optional -> time varchar(255) not null,
  primary key(id),
  foreign key (patient) REFERENCES Patient(username) on delete cascade,
  foreign key (doctor) REFERENCES Staff(username) on delete cascade
);
create table MedicationRecord(
  id int not null auto_increment,
  patient varchar(255) not null,
  doctor varchar(255) not null,
  dosage varchar(255) not null,
  primary key(id),
  foreign key (patient) REFERENCES Patient(username) on delete cascade,
  foreign key (doctor) REFERENCES Staff(username) on delete cascade
);
create table NurseMemo(
  id int not null auto_increment,
  message varchar(255) not null,
  nurseUsername varchar(255) not null,
  primary key(id),
  foreign key (nurseUsername) REFERENCES Staff(username) on delete cascade
);
create table CashierRecord(
  id int not null auto_increment,
  patient varchar(255) not null,
  amount float not null,
  createTime datetime not null DEFAULT CURRENT_TIMESTAMP,
  primary key(id),
  foreign key (patient) REFERENCES Patient(username) on delete cascade
);
create table PurchaseRecord(
  id int not null auto_increment,
  message varchar(255) not null,
  primary key(id)
);
create table Emergency(
  id int not null auto_increment,
  patient varchar(255) not null,
  message varchar(255) not null,
  primary key(id),
  foreign key (patient) REFERENCES Patient(username) on delete cascade
);
create table VIP(
  id int not null auto_increment,
  patient varchar(255) not null,
  points int not null DEFAULT 0,
  primary key(id),
  foreign key (patient) REFERENCES Patient(username) on delete cascade,
  unique(patient)
);