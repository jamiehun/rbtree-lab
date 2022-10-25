#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));

  // TODO: initialize struct if needed
  new_node->color = RBTREE_BLACK;
  p->nil = new_node;
  p->root = p->nil;

  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) { // 트리와 insert할 값을 넣음
  // TODO: implement insert
  // CLRS pseudo code 참고
  node_t *y = (node_t *)calloc(1, sizeof(node_t));
  node_t *x = (node_t *)calloc(1, sizeof(node_t));
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t)); // z에 대한 새로운 노드
  new_node->key = key // z에 대한 key값 설정
  
  y = t->nil; // 구조체 안에 있는 멤버는 투포인터로 안찍어도 되나?
  x = t->root; 

  while ( x != t->nil )
  {
    y = x;
    
    if (key < x->key)
      x = x->left;
    else x = x->right;

  }

  // line 8: z.p = y
  new_node->parent = y;

  // line 9 ~ line 13
  if (y == t->nil)       // 처음부터 nil으로 빈 트리를 의미
    t->root = new_node;
  else if (new_node->key < y->key)







  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
