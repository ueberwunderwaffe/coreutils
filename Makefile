CC = gcc
CFLAGS = -Wall -Wextra

FU = file_utilities/
TU = text_utilities/
SU = shell_utilities/

### File utilities ###
ls:
	$(CC) $(FU)ls.c -o ls.out $(CFLAGS)
chgrp:	
	$(CC) $(FU)chgrp.c -o chgrp.out $(CFLAGS)
chown:
	$(CC) $(FU)chown.c -o chown.out $(CFLAGS)
chmod:
	$(CC) $(FU)chmod.c -o chmod.out $(CFLAGS)
cp:
	$(CC) $(FU)cp.c -o cp.out $(CFLAGS)
dd:	
	$(CC) $(FU)dd.c -o dd.out $(CFLAGS)
df:
	$(CC) $(FU)df.c -o df.out $(CFLAGS)

### Text utilities ###
cat:
	$(CC) $(TU)cat.c -o cat.out $(CFLAGS)
nl:
	$(CC) $(TU)nl.c -o nl.out $(CFLAGS)
head:
	$(CC) $(TU)head.c -o head.out $(CFLAGS)
tail:
	$(CC) $(TU)tail.c -o tail.out $(CFLAGS)
expand:
	$(CC) $(TU)expand.c -o expand.out $(CFLAGS)
unexpand:
	$(CC) $(TU)unexpand.c -o unexpand.out $(CFLAGS)
sum:
	$(CC) $(TU)sum.c -o sum.out $(CFLAGS)
cksum:
	$(CC) $(TU)cksum.c -o cksum.out $(CFLAGS)
wc:
	$(CC) $(TU)wc.c -o wc.out $(CFLAGS)
sort:
	$(CC) $(TU)sort.c -o sort.out $(CFLAGS)
comm:
	$(CC) $(TU)comm.c -o comm.out $(CFLAGS)

### Shell utilities ###
arch:
	$(CC) $(SU)arch.c -o arch.out $(CFLAGS)
date:
	$(CC) $(SU)date.c -o date.out $(CFLAGS)
echo:
	$(CC) $(SU)echo.c -o echo.out $(CFLAGS)
id:
	$(CC) $(SU)id.c -o id.out $(CFLAGS)
pwd:
	$(CC) $(SU)pwd.c -o pwd.out $(CFLAGS)
yes:
	$(CC) $(SU)yes.c -o yes.out $(CFLAGS)
whoami:
	$(CC) $(SU)whoami.c -o whoami.out $(CFLAGS)
logname:
	$(CC) $(SU)logname.c -o logname.out $(CFLAGS)
true:
	$(CC) $(SU)true.c -o true.out $(CFLAGS)
false:
	$(CC) $(SU)false.c -o false.out $(CFLAGS)
hostid:
	$(CC) $(SU)hostid.c -o hostid.out $(CFLAGS)
nproc:
	$(CC) $(SU)nproc.c -o nproc.out $(CFLAGS)
stat:
	$(CC) $(SU)stat.c -o stat.out $(CFLAGS)
timeout:
	$(CC) $(SU)timeout.c -o timeout.out $(CFLAGS)
tty:
	$(CC) $(SU)tty.c -o tty.out $(CFLAGS)
test:
	$(CC) $(SU)test.c -o test.out $(CFLAGS)
tee:
	$(CC) $(SU)tee.c -o tee.out $(CFLAGS)
uname:
	$(CC) $(SU)uname.c -o uname.out $(CFLAGS)
unlink:
	$(CC) $(SU)unlink.c -o unlink.out $(CFLAGS)
uptime:
	$(CC) $(SU)uptime.c -o uptime.out $(CFLAGS)
users:
	$(CC) $(SU)users.c -o users.out $(CFLAGS)
who:
	$(CC) $(SU)who.c -o who.out $(CFLAGS)
seq:
	$(CC) $(SU)seq.c -o seq.out $(CFLAGS)
printf:
	$(CC) $(SU)printf.c -o printf.out $(CFLAGS)
nice:
	$(CC) $(SU)nice.c -o nice.out $(CFLAGS)
nohup:
	$(CC) $(SU)nohup.c -o nohup.out $(CFLAGS)
factor:
	$(CC) $(SU)factor.c -o factor.out $(CFLAGS)
