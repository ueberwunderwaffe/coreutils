ls:
	gcc file_utilities/ls.c -o ls.out -Wall -Wextra
cat:
	gcc text_utilities/cat.c -o cat.out -Wall -Wextra
nl:
	gcc text_utilities/nl.c -o nl.out -Wall -Wextra
head:
	gcc text_utilities/head.c -o head.out -Wall -Wextra
tail:
	gcc text_utilities/tail.c -o tail.out -Wall -Wextra
expand:
	gcc text_utilities/expand.c -o expand.out -Wall -Wextra
unexpand:
	gcc text_utilities/unexpand.c -o unexpand.out -Wall -Wextra
sum:
	gcc text_utilities/sum.c -o sum.out -Wall -Wextra
wc:
	gcc text_utilities/wc.c -o wc.out -Wall -Wextra
sort:
	gcc text_utilities/sort.c -o sort.out -Wall -Wextra
