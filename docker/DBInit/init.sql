create table Messages
(
	MessageID serial not null primary key,
	Sender varchar(50),
	Message text,
	Datetime timestamp
);