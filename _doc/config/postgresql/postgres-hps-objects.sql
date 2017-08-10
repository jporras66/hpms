--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET search_path = public, pg_catalog;
SET default_tablespace = '';
SET default_with_oids = false;

--
-- Name: messages; Type: TABLE; Schema: public; Owner: hps; Tablespace: 
--

CREATE TABLE messages (
    msg_id bigint NOT NULL,
    msg_uuid character varying(61),
    msg_mimetype character varying(51),
    msg_content bytea,
    msg_len numeric(4,0)
);

--
-- Name: timeout; Type: TABLE; Schema: public; Owner: hps; Tablespace: 
--

CREATE TABLE timeout (
    uuid character varying(61) not null,
    timestamp timestamp without time zone default now()
);


ALTER TABLE messages OWNER TO hps;
ALTER TABLE timeout  OWNER TO hps;

--
-- Name: seq_msg; Type: SEQUENCE; Schema: public; Owner: hps
--

CREATE SEQUENCE seq_msg
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE seq_msg OWNER TO hps;

--
-- Data for Name: emp; Type: TABLE DATA; Schema: public; Owner: hps
--


--
-- Name: seq_msg; Type: SEQUENCE SET; Schema: public; Owner: hps
--

SELECT pg_catalog.setval('seq_msg', 1, true);


--
-- Name: messages_msg_uuid_key, messages_pkey
--

ALTER TABLE ONLY messages
    ADD CONSTRAINT messages_msg_uuid_key UNIQUE (msg_uuid);

ALTER TABLE ONLY messages
    ADD CONSTRAINT messages_pkey PRIMARY KEY (msg_id);

ALTER TABLE ONLY timeout
    ADD CONSTRAINT timeout_pkey PRIMARY KEY (uuid);
--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

--REVOKE ALL ON SCHEMA public FROM PUBLIC;
--REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;
GRANT ALL ON SCHEMA public TO hps;


--
-- PostgreSQL database dump complete
--
