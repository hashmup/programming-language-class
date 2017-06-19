CFLAGS := -Wall -O3
LDFLAGS := -lgc

TARGETS = list.out huge_list.out

all: $(TARGETS)

list.out: list.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

huge_list.out: huge_list.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test: $(TARGETS)
	python test.py

plot:
	./iz.out 1000 1 > result.txt
	python ../utils/plot.py result.txt

clean:
	rm -rf *.out *.o calc_iz_c.s calc_hh_c.s
