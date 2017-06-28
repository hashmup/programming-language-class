#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/* この演習は，GC ライブラリを使っても使わなくても行えます.
   できることなら両者を比べてみてください．
   GCを使う場合，コンパイル時に -DUSE_GC=1 を指定する
   GCを使わない場合，コンパイル時に -DUSE_GC=0 を指定する
 */

#if USE_GC
#include <gc/gc.h>

/* sz バイト割り当て */
void * alloc(size_t sz) {
  void * p = GC_MALLOC(sz);
  printf("born: %ld %zu byte\n", p, sz);
  return p;
}

/* p を開放．だがGCありの場合は何もしない */
void reclaim(void * p) {
}

#else

/* sz バイト割り当て */
void * alloc(size_t sz) {
  void * p = malloc(sz);
  return p;
}

/* p を開放．だがGCありの場合は何もしない */
void reclaim(void * p) {
  free(p);
}

#endif

/* the usual binary search tree.
   to make a comparison fair, 
   we assume the content is int 
*/

typedef struct bs_tree {
  int x;
  struct bs_tree * left;
  struct bs_tree * right;
} bs_tree;

const double eps = 0.001;

/* 値がx, 左右の子供がl, rのノードを作る */
bs_tree * mk_node(int x, bs_tree * l, bs_tree * r) {
  bs_tree * n = (bs_tree *)alloc(sizeof(bs_tree));
  n->x = x;
  n->left = l;
  n->right = r;
  return n;
}

/* 2分探索木 t に x を挿入．関数型更新．
   つまり既存のノードを更新するのではなく，新しいノードを
   作って返す */
bs_tree * bs_tree_insert(int x, bs_tree * t) {
  if (!t) {
    /* t は空．-> leafノードを作って返す */
    return mk_node(x, 0, 0);
  } else {
    int y = t->x;
    bs_tree * l = t->left;
    bs_tree * r = t->right;
    if (x == y) {
      return t;
    } else {
      /* 左右どちらかに x を挿入.
	 このノード自身はここで用済み(dead) */
      reclaim(t);
      if (x < y) {
	return mk_node(y, bs_tree_insert(x, l), r);
      } else {
	return mk_node(y, l, bs_tree_insert(x, r));
      }
    }
  }
}

/* remove the maximum element from t
   t から最大値のノードを削除した新しい木を返す．
   返り値は新しい木のルート．
   削除されたノードに入っていた値を， *x_ に入れる．
 */
bs_tree * bs_tree_remove_max(bs_tree * t, int *x_) {
  assert(t);
  int x = t->x;
  bs_tree * l = t->left;
  bs_tree * r = t->right;

  /* t はここで不要になる */
  reclaim(t);
  if (!r) {
    /* 右の子がいない．
       -> t自身が削除されるノード
       -> 左の子を返す． */
    *x_ = x;
    return l;
  } else {
    /* 右の子がいる
       -> 右の子の最大値を返す */
    bs_tree * r_ = bs_tree_remove_max(r, x_);
    return mk_node(x, l, r_);
  }
}

/* tからx を削除 */
bs_tree * bs_tree_remove(int x, bs_tree * t) {
  if (!t) { 
    return 0;
  } else {
    int y = t->x;
    bs_tree * l = t->left;
    bs_tree * r = t->right;
    /* tはここで不要になる */
    reclaim(t);
    if (x == y) {
      /* tが削除される */
      if (!l) {
	/* 左の子がいない -> 右の子を返す */
	return r;
      } else {
	/* 左の子がいる -> 左の子から最大値を削除 */
	int y;
	bs_tree * l_ = bs_tree_remove_max(l, &y);
	return mk_node(y, l_, r);
      }
    } else if (x < y) {
      return mk_node(y, bs_tree_remove(x, l), r);
    } else {
      return mk_node(y, l, bs_tree_remove(x, r));
    }
  }
}

/* tに含まれるノード数を返す */
int bs_tree_count(bs_tree * t) {
  if (!t) return 0;
  else {
    return 1 + bs_tree_count(t->left) + bs_tree_count(t->right);
  }
}

/* (1) まず ランダム要素をn回挿入;
   (2) 次に (ランダム要素の挿入; ランダム要素の削除)を
   m回繰り返す．
   (3) 最後に全部の要素を削除

   (2)で，大体同じ木のサイズを保ちつつ挿入と削除を繰り返す
  */
bs_tree * grow_tree(int n, int m, 
		    unsigned short s0, 
		    unsigned short s1, 
		    unsigned short s2) {
  unsigned short xsubi[3] = { s0, s1, s2 };
  int i;
  bs_tree * t = 0;
  /* n回挿入 */
  for (i = 0; i < n; i++) {
    int x = nrand48(xsubi) % (2 * n);
    t = bs_tree_insert(x, t);
  }
  printf("%d nodes in the tree\n", bs_tree_count(t));
  /* m回, 挿入+削除 */
  for (i = 0; i < m; i++) {
    int x = nrand48(xsubi) % (2 * n);
    int y = nrand48(xsubi) % (2 * n);
    t = bs_tree_insert(x, t);
    t = bs_tree_remove(y, t);
  }
  /* 全部の値を削除 */
  for (i = 0; i < 2 * n; i++) {
    t = bs_tree_remove(i, t);
  }
  return t;
}

bs_tree * run(int n, int m) {
  return grow_tree(n, m, 1, 2, 3);
}

int main(int argc, char ** argv) {
  int n = (argc > 1 ? atoi(argv[1]) : 100);
  int m = (argc > 2 ? atoi(argv[2]) : 1000);
#if USE_GC
  int d = (argc > 3 ? atoi(argv[3]) : 3);
  GC_set_free_space_divisor(d);
#endif
  bs_tree * t = run(n, m);
  assert(!t);
  return 0;
}
