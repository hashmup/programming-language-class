#include <stdlib.h>
#include <stdio.h>

double cur_time() {
  struct timeval tp[1];
  gettimeofday(tp, 0);
  return tp->tv_sec + tp->tv_usec * 1.0e-6;
}

typedef struct list {
  volatile struct list * next;
} list;

void * alloc(long sz) {
  void * p = calloc(sz, 1);
  return p;
}

/* 長さ n のリストを作る．
   リストの各セルは，sz バイトとする */
volatile list * make_list(long sz, long n) {
  volatile list * q = 0;
  long i;
  for (i = 0; i < n; i++) {
    volatile list * p = alloc(sz);
    p->next = q;
    q = p;
  }
  return q;
}

/* セルサイズ sz バイト，長さ l のリストを m 回作る */
int main(int argc, char ** argv) {
  long sz = (argc > 1 ? atol(argv[1]) : 16); /* セルサイズ */
  long l = (argc > 2 ? atol(argv[2]) : 32); /* リストの長さ */
  long m = (argc > 3 ? atol(argv[3]) : 1024); /* リストの個数 */

  if (sz < sizeof(list)) sz = sizeof(list);
  if (l < 1) l = 1;

  printf("cell size = %ld bytes\n", sz);
  printf("list length = %ld\n", l);
  printf("iterations = %ld\n", m);
  printf("total bytes = %ld x %ld x %ld = %ld\n", sz, l, m, sz * l * m);

  double t0 = cur_time();
  long i;
  volatile list * p = 0;
  /* 長さ l のリストを m 回作る */
  for (i = 0; i < m; i++) {
    p = make_list(sz, l);
  }
  double t1 = cur_time();
  double dt = t1 - t0;
  printf("%f sec for %ld allocs (%f nsec per alloc)\n",
	 dt, l * m, dt * 1.0e9 / (l * m));
  return 0;
}
