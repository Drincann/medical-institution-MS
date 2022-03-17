delete from
  Staff;
delete from
  Patient;
delete from
  Info;
insert into
  `Patient`(username, password, situation, injected)
VALUES('patient1', 'patient1', '', 0);
insert into
  `Staff`(username, password, type)
VALUES('admin1', 'admin1', 0);
insert into
  `Staff`(username, password, type)
VALUES('front1', 'front1', 1);
insert into
  `Staff`(username, password, type)
VALUES('doctor1', 'doctor1', 2);
insert into
  `Staff`(username, password, type)
VALUES('nurse1', 'nurse1', 3);
insert INTO
  `Staff`(username, password, type)
VALUES('buyer1', 'buyer1', 4);
insert into
  Info (name)
values
  ('某医院');
insert into
  `CashierRecord`(patient, amount, createTIme)
values('patient1', '12.3', '2022-03-16');