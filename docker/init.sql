CREATE DATABASE Clients

create  table Messages
(
	MessageID serial not null primary key,
	Sender varchar(50),
	message text,
	datetime timestamp
);