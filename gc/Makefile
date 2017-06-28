CFLAGS := -Wall -O3
LDFLAGS := -lgc

TARGETS = list.out gc_list.out huge_list.out bstree.out gc_bstree.out bstree2_gc.out bstree2_nogc.out

all: $(TARGETS)

list.out: list.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
gc_list.out: gc_list.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

huge_list.out: huge_list.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
bstree.out: bstree.c
	$(CC) $(CFLAGS) -o $@ -DUSE_GC=0 $^ $(LDFLAGS)
gc_bstree.out: gc_bstree.c
	$(CC) $(CFLAGS) -o $@ -DUSE_GC=1 $^ $(LDFLAGS)
bstree2_gc.out: bstree2.c
	$(CC) $(CFLAGS) -o $@ -DUSE_GC=1 $^ $(LDFLAGS)
bstree2_nogc.out: bstree2.c
	$(CC) $(CFLAGS) -o $@ -DUSE_GC=0 $^ $(LDFLAGS)

test: $(TARGETS)
	python test.py

plot:
	./iz.out 1000 1 > result.txt
	python ../utils/plot.py result.txt

clean:
	rm -rf *.out
