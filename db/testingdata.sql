delete from
  Staff;
delete from
  Patient;
delete from
  Info;
insert into
  `Staff`(username, password, type)
VALUES('admin', 'admin', 0);
insert into
  `Staff`(username, password, type)
VALUES('front', 'front', 1);
insert into
  `Staff`(username, password, type)
VALUES('doctor', 'doctor', 2);
insert into
  `Staff`(username, password, type)
VALUES('nurse', 'nurse', 3);
insert into
  `Staff`(username, password, type)
VALUES('buyer', 'buyer', 4);
insert into
  `Patient`(username, password, situation, injected)
VALUES('patient1', 'patient1', '', 0);
insert into
  `Patient`(username, password, situation, injected)
VALUES('patient2', 'patient2', '', 0);
insert into
  `Patient`(username, password, situation, injected)
VALUES('patient3', 'patient3', '', 0);
insert into
  Info (name)
values
  ('某医院');
insert into
  `CashierRecord`(patient, amount, createTIme)
values('patient1', '12.3', '2022-03-16');