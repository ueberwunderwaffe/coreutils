C_COMPILER = gcc
C_FLAGS = -Wall -Wextra -Werror

FU = file_utilities/
TU = text_utilities/
SU = shell_utilities/

### File utilities ###
chcon:
	$(C_COMPILER) $(FU)chcon.c -o chcon.out $(C_FLAGS)
chgrp:
	$(C_COMPILER) $(FU)chgrp.c -o chgrp.out $(C_FLAGS)
chown:
	$(C_COMPILER) $(FU)chown.c -o chown.out $(C_FLAGS)
chmod:
	$(C_COMPILER) $(FU)chmod.c -o chmod.out $(C_FLAGS)
cp:
	$(C_COMPILER) $(FU)cp.c -o cp.out $(C_FLAGS)
dd:
	$(C_COMPILER) $(FU)dd.c -o dd.out $(C_FLAGS)
df:
	$(C_COMPILER) $(FU)df.c -o df.out $(C_FLAGS)
dir:
	$(C_COMPILER) $(FU)dir.c -o dir.out $(C_FLAGS)
dircolors:
	$(C_COMPILER) $(FU)dircolors.c -o dircolors.out $(C_FLAGS)
install:
	$(C_COMPILER) $(FU)install.c -o install.out $(C_FLAGS)
ln:
	$(C_COMPILER) $(FU)ln.c -o ln.out $(C_FLAGS)
ls:
	$(C_COMPILER) $(FU)ls.c -o ls.out $(C_FLAGS)
mkdir:
	$(C_COMPILER) $(FU)mkdir.c -o mkdir.out $(C_FLAGS)
mkfifo:
	$(C_COMPILER) $(FU)mkfifo.c -o mkfifo.out $(C_FLAGS)
mknod:
	$(C_COMPILER) $(FU)mknod.c -o mknod.out $(C_FLAGS)
mktemp:
	$(C_COMPILER) $(FU)mktemp.c -o mktemp.out $(C_FLAGS)
mv:
	$(C_COMPILER) $(FU)mv.c -o mv.out $(C_FLAGS)
realpath:
	$(C_COMPILER) $(FU)realpath.c -o realpath.out $(C_FLAGS)
rm:
	$(C_COMPILER) $(FU)rm.c -o rm.out $(C_FLAGS)
rmdir:
	$(C_COMPILER) $(FU)rmdir.c -o rmdir.out $(C_FLAGS)
shred:
	$(C_COMPILER) $(FU)shred.c -o shred.out $(C_FLAGS)
sync:
	$(C_COMPILER) $(FU)sync.c -o sync.out $(C_FLAGS)
touch:
	$(C_COMPILER) $(FU)touch.c -o touch.out $(C_FLAGS)
truncate:
	$(C_COMPILER) $(FU)truncate.c -o truncate.out $(C_FLAGS)
vdir:
	$(C_COMPILER) $(FU)vdir.c -o vdir.out $(C_FLAGS)

### Text utilities ###
b2sum:
	$(C_COMPILER) $(TU)b2sum.c -o b2sum.out $(C_FLAGS)
base32:
	$(C_COMPILER) $(TU)base32.c -o base32.out $(C_FLAGS)
base64:
	$(C_COMPILER) $(TU)base64.c -o base64.out $(C_FLAGS)
cat:
	$(C_COMPILER) $(TU)cat.c -o cat.out $(C_FLAGS)
cksum:
	$(C_COMPILER) $(TU)cksum.c -o cksum.out $(C_FLAGS)
comm:
	$(C_COMPILER) $(TU)comm.c -o comm.out $(C_FLAGS)
csplit:
	$(C_COMPILER) $(TU)csplit.c -o csplit.out $(C_FLAGS)
cut:
	$(C_COMPILER) $(TU)cut.c -o cut.out $(C_FLAGS)
expand:
	$(C_COMPILER) $(TU)expand.c -o expand.out $(C_FLAGS)
fmt:
	$(C_COMPILER) $(TU)fmt.c -o fmt.out $(C_FLAGS)
fold:
	$(C_COMPILER) $(TU)fold.c -o fold.out $(C_FLAGS)
head:
	$(C_COMPILER) $(TU)head.c -o head.out $(C_FLAGS)
join:
	$(C_COMPILER) $(TU)join.c -o join.out $(C_FLAGS)
md5sum:
	$(C_COMPILER) $(TU)md5sum.c -o md5sum.out $(C_FLAGS)
nl:
	$(C_COMPILER) $(TU)nl.c -o nl.out $(C_FLAGS)
numfmt:
	$(C_COMPILER) $(TU)numfmt.c -o numfmt.out $(C_FLAGS)
od:
	$(C_COMPILER) $(TU)od.c -o od.out $(C_FLAGS)
paste:
	$(C_COMPILER) $(TU)paste.c -o paste.out $(C_FLAGS)
ptx:
	$(C_COMPILER) $(TU)ptx.c -o ptx.out $(C_FLAGS)
pr:
	$(C_COMPILER) $(TU)pr.c -o pr.out $(C_FLAGS)
sha1sum:
	$(C_COMPILER) $(TU)sha1sum.c -o sha1sum.out $(C_FLAGS)
shuf:
	$(C_COMPILER) $(TU)shuf.c -o shuf.out $(C_FLAGS)
sort:
	$(C_COMPILER) $(TU)sort.c -o sort.out $(C_FLAGS)
split:
	$(C_COMPILER) $(TU)split.c -o split.out $(C_FLAGS)
sum:
	$(C_COMPILER) $(TU)sum.c -o sum.out $(C_FLAGS)
tac:
	$(C_COMPILER) $(TU)tac.c -o tac.out $(C_FLAGS)
tail:
	$(C_COMPILER) $(TU)tail.c -o tail.out $(C_FLAGS)
tr:
	$(C_COMPILER) $(TU)tr.c -o tr.out $(C_FLAGS)
tsort:
	$(C_COMPILER) $(TU)tsort.c -o tsort.out $(C_FLAGS)
unexpand:
	$(C_COMPILER) $(TU)unexpand.c -o unexpand.out $(C_FLAGS)
uniq:
	$(C_COMPILER) $(TU)uniq.c -o uniq.out $(C_FLAGS)
wc:
	$(C_COMPILER) $(TU)wc.c -o wc.out $(C_FLAGS)

### Shell utilities ###
arch:
	$(C_COMPILER) $(SU)arch.c -o arch.out $(C_FLAGS)
basename:
	$(C_COMPILER) $(SU)basename.c -o basename.out $(C_FLAGS)
chroot:
	$(C_COMPILER) $(SU)chroot.c -o chroot.out $(C_FLAGS)
date:
	$(C_COMPILER) $(SU)date.c -o date.out $(C_FLAGS)
dirname:
	$(C_COMPILER) $(SU)dirname.c -o dirname.out $(C_FLAGS)
du:
	$(C_COMPILER) $(SU)du.c -o du.out $(C_FLAGS)
echo:
	$(C_COMPILER) $(SU)echo.c -o echo.out $(C_FLAGS)
env:
	$(C_COMPILER) $(SU)env.c -o env.out $(C_FLAGS)
expr:
	$(C_COMPILER) $(SU)expr.c -o expr.out $(C_FLAGS)
factor:
	$(C_COMPILER) $(SU)factor.c -o factor.out $(C_FLAGS)
false:
	$(C_COMPILER) $(SU)false.c -o false.out $(C_FLAGS)
groups:
	$(C_COMPILER) $(SU)groups.c -o groups.out $(C_FLAGS)
hostid:
	$(C_COMPILER) $(SU)hostid.c -o hostid.out $(C_FLAGS)
id:
	$(C_COMPILER) $(SU)id.c -o id.out $(C_FLAGS)
link:
	$(C_COMPILER) $(SU)link.c -o link.out $(C_FLAGS)
logname:
	$(C_COMPILER) $(SU)logname.c -o logname.out $(C_FLAGS)
nice:
	$(C_COMPILER) $(SU)nice.c -o nice.out $(C_FLAGS)
nohup:
	$(C_COMPILER) $(SU)nohup.c -o nohup.out $(C_FLAGS)
nproc:
	$(C_COMPILER) $(SU)nproc.c -o npoc.out $(C_FLAGS)
pathchk:
	$(C_COMPILER) $(SU)pathchk.c -o pathchk.out $(C_FLAGS)
pinky:
	$(C_COMPILER) $(SU)pinky.c -o pinky.out $(C_FLAGS)
printenv:
	$(C_COMPILER) $(SU)printenv.c -o printenv.out $(C_FLAGS)
printf:
	$(C_COMPILER) $(SU)printf.c -o printf.out $(C_FLAGS)
pwd:
	$(C_COMPILER) $(SU)pwd.c -o pwd.out $(C_FLAGS)
readlink:
	$(C_COMPILER) $(SU)readlink.c -o readlink.out $(C_FLAGS)
runcon:
	$(C_COMPILER) $(SU)runcon.c -o runcon.out $(C_FLAGS)
seq:
	$(C_COMPILER) $(SU)seq.c -o seq.out $(C_FLAGS)
sleep:
	$(C_COMPILER) $(SU)sleep.c -o sleep.out $(C_FLAGS)
stat:
	$(C_COMPILER) $(SU)stat.c -o stat.out $(C_FLAGS)
stdbuf:
	$(C_COMPILER) $(SU)stdbuf.c -o stdbuf.out $(C_FLAGS)
stty:
	$(C_COMPILER) $(SU)stty.c -o stty.out $(C_FLAGS)
tee:
	$(C_COMPILER) $(SU)tee.c -o tee.out $(C_FLAGS)
test:
	$(C_COMPILER) $(SU)test.c -o test.out $(C_FLAGS)
timeout:
	$(C_COMPILER) $(SU)timeout.c -o timeout.out $(C_FLAGS)
true:
	$(C_COMPILER) $(SU)true.c -o true.out $(C_FLAGS)
tty:
	$(C_COMPILER) $(SU)tty.c -o tty.out $(C_FLAGS)
uname:
	$(C_COMPILER) $(SU)uname.c -o uname.out $(C_FLAGS)
unlink:
	$(C_COMPILER) $(SU)unlink.c -o unlink.out $(C_FLAGS)
uptime:
	$(C_COMPILER) $(SU)uptime.c -o uptime.out $(C_FLAGS)
users:
	$(C_COMPILER) $(SU)users.c -o users.out $(C_FLAGS)
who:
	$(C_COMPILER) $(SU)who.c -o who.out $(C_FLAGS)
whoami:
	$(C_COMPILER) $(SU)whoami.c -o whoami.out $(C_FLAGS)
yes:
	$(C_COMPILER) $(SU)yes.c -o yes.out $(C_FLAGS)
