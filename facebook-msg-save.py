#!/usr/bin/env python
# facebook-msg-save.py - Download facebook messages into SQLite DB using FQL
# NOTE: slow, because FQL is very slow, especially when you query old data

import fbconsole as fb
import pprint
import sys
import traceback
import time
import sqlite3 as sql

### Configuration

# Output file. SQLite Database
output = 'log.db'

# Message thread ID. FQL is damn flawed so I gaveup automating this part.
thd_id = 1 / 0

# You might want to get a long-live token, which survives for 60 days rather
# than < 2 hours. Once you get the token, you should set this value to your
# facebook app ID.
#fb.APP_ID = 



time_begin = time.time()

fb.AUTH_SCOPE = ['read_mailbox']
fb.authenticate()

conn = sql.connect(output)
c = conn.cursor()


r = fb.fql('''
	SELECT message_count FROM thread WHERE thread_id="%s" AND folder_id=0
''' % thd_id)
thd_cnt = r[0]['message_count']
print >> sys.stderr, "Number of messages: %d" % thd_cnt

user_name = dict()
i = 0
step = 30

# check if the table already exists
r = c.execute('''
	SELECT name FROM sqlite_master WHERE type='table' AND name='messages'
''').fetchall()
if len(r) == 0:
	c.execute('''
		CREATE TABLE messages
		( id INTEGER PRIMARY KEY
		, date INTEGER
		, author_id INTEGER
		, message TEXT
		)
	''')
	c.execute('''
		CREATE TABLE users
		( id INTEGER PRIMARY KEY
		, name TEXT
		)
	''')
else:
	(i,) = c.execute('SELECT MAX(id) FROM messages').fetchone()
	i = i + 1
	for row in c.execute('SELECT id, name FROM users'):
		uid, uname = row
		user_name[uid] = uname

i_begin = i
while i < thd_cnt:
	try:
		msgs = fb.fql('''
			SELECT author_id,body,created_time FROM message WHERE thread_id="%s" LIMIT %d,%d
		''' % (thd_id, i, step))
	except Exception as e:
		print >> sys.stderr, "Exception while retrieving messages at %d" % i
		raise e

	for msg in msgs:
		uid = int(msg['author_id'])
		ctime = int(msg['created_time'])
		body = msg['body']
		try:
			uname = user_name[uid]
		except KeyError:
			try:
				uname = fb.fql('SELECT name FROM user WHERE uid="%s"' % uid)[0]['name']
			except Exception as e:
				uname = str(uid)
			user_name[uid] = uname
			c.execute('INSERT INTO users VALUES (?, ?)', (uid, uname))

		c.execute('INSERT INTO messages VALUES (?,?,?,?)', (i, ctime, uid, body))
		print >> sys.stderr, "%s|%s|%s|%s" % (i, ctime, uname, body)
		i = i + 1
	conn.commit()
	i_diff = i - i_begin
	time_diff = time.time() - time_begin
	print >> sys.stderr, "### committed... (messages: %d, time: %f, rate: %f )###" % (i_diff, time_diff, i_diff / time_diff)

conn.commit()
conn.close()
