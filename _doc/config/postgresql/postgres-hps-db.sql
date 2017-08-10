-- Role: hps
-- DROP ROLE hps;
--
CREATE ROLE hps LOGIN
  PASSWORD 'password'
  SUPERUSER INHERIT CREATEDB NOCREATEROLE NOREPLICATION;
--
--
-- Database: "HPS"
-- DROP DATABASE "HPS";
--
CREATE DATABASE "HPS"
  WITH OWNER = hps
       ENCODING = 'UTF8'
       TABLESPACE = pg_default
       LC_COLLATE = 'en_US.utf8'
       LC_CTYPE = 'en_US.utf8'
       CONNECTION LIMIT = -1;