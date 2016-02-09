CC:=clang
CFLAGS:=-Wall -Wextra -O3 -std=c99 -Ofast -fPIC $(CFLAGS)
SRC_DIR:=.
SRC_FILES:=sc/dict_int_chr.c $(SRC_DIR)/*.c
DST_DIR:=.
LIB_PATH:=.
default:
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(DST_DIR)/a.out
r:
	$(CC) $(CFLAGS) -DNDEBUG $(SRC_FILES) -s -o $(DST_DIR)/a.out
c:
	cd $(DST_DIR); rm -f $(DST_DIR)/a.out *.o
