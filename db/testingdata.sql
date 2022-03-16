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
  Info (name)
values
  ('某医院');
insert into
  `CashierRecord`(patient, amount, createTIme)
values('patient1', '12.3', '2022-03-16');