#include "rbtree.h"

#include <stdlib.h>

/* NEW RB_TREE */
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

/* RBTREE INSERT */
// rbtree_insert 후 node_t가 return되어야 하므로 자료형에 node_t로 정의
node_t *rbtree_insert(rbtree *t, const key_t key) { // 트리와 insert할 값을 넣음
  // TODO: implement insert
  // CLRS pseudo code 참고
  node_t *y = t->nil;  // t는 인자로 들고오므로 node_t *y로 설정함
  node_t *x = t->root;
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t)); // z에 대한 새로운 노드
  new_node->key = key; // z에 대한 key값 설정

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
    y->left = new_node;
  else y->right = new_node;

  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->color = RBTREE_RED;
  
  insert_fixup(t, new_node); // insert_fixup 함수 설정
  return t->root;
}

/* Insert-FIXUP */ 
// Insert-Fixup의 경우 반환하는 값이 없고 insert에 대한 수정만 이루기 때문에 void로 설정함
// rbtree.h에도 똑같이 설정 필요
void insert_fixup(rbtree *t, node_t *new_node){
  // CLRS Terms ; (CLRS ; VS code)
  // (T ; t), (z ; new_node), (p ; parent), 
  while (new_node->parent->color == RBTREE_RED){
    if (new_node->parent == new_node->parent->parent->left){
      node_t *y = new_node->parent->parent->right; // line 3 make uncle
      if (y->color == RBTREE_RED){
        new_node->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = new_node->parent->parent;
      }
      else { // 여기에서 psuedo code는 else if로 되어 있는데 아래와 같이 바꿔줘야함
        if (new_node == new_node->parent->right) {
        new_node = new_node->parent;
        // LEFT - ROTATE (T, Z)
        left_rotate(t, new_node);
      }
      new_node->parent->color = RBTREE_BLACK;
      new_node->parent->parent->color = RBTREE_RED;
      // RIGHT - ROTATE (T, Z.P.P)
      right_rotate(t, new_node->parent->parent);
      }
  }
  else {
      node_t *y = new_node->parent->parent->left; // line 3 make uncle
      if (y->color == RBTREE_RED){
        new_node->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = new_node->parent->parent;
      }
      else { // 여기에서 psuedo code는 else if로 되어 있는데 아래와 같이 바꿔줘야함
      if (new_node == new_node->parent->left) {
        new_node = new_node->parent;
        // RIGHT - ROTATE (T, Z)
        right_rotate(t, new_node);
      }
      new_node->parent->color = RBTREE_BLACK;
      new_node->parent->parent->color = RBTREE_RED;
      // LEFT - ROTATE (T, Z.p.p)
      left_rotate(t, new_node->parent->parent);
      }
  }
  }
  t->root->color = RBTREE_BLACK;
}

/* LEFT-ROTATE (T, x) */
// LEFT-ROTATE의 경우에도 반환값이 void이기 때문에 void값 설정함
// T = rbtree *t , x = new_node
void left_rotate(rbtree *t, node_t *new_node) {
  node_t *y = new_node->right;               // y를 설정
  new_node->right = y->left;         // y의 왼쪽 서브트리를 new_node의 오른쪽 서브트리로 옮김
  if (y->left != t->nil){        
    y->left->parent = new_node;
  }
  y->parent = new_node->parent;      // new_node의 부모를 y로 연결

  if (new_node->parent == t->nil){
    t->root = y;
  }

  else if (new_node == new_node->parent->left){
    new_node->parent->left = y;
  }

  else new_node->parent->right = y;

  y->left = new_node;
  new_node->parent = y;
}

/* RIGHT-ROTATE (T, x) */
// T = rbtree *t , x = new_node
void right_rotate(rbtree *t, node_t *new_node) {
  node_t *y = new_node->left;               // y를 설정
  new_node->left = y->right;         // y의 왼쪽 서브트리를 new_node의 오른쪽 서브트리로 옮김
  if (y->right != t->nil){        
    y->right->parent = new_node;
  }
  y->parent = new_node->parent;      // new_node의 부모를 y로 연결

  if (new_node->parent == t->nil){
    t->root = y;
  }

  else if (new_node == new_node->parent->right){
    new_node->parent->right = y;
  }

  else new_node->parent->left = y;

  y->right = new_node;
  new_node->parent = y;
}

/* FIND */
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *x = t->root;

  while((x != NULL)){
    if (x->key == key){
      return x;
    }
    else if (x->key < key){
      x = x->right;
    }
    else {
      x = x->left;
    }
  }
    return NULL;
}


/* MINIMUM */
node_t *rbtree_min(const rbtree *) {
  // TODO: implement find
  node_t* x = t->root;
  node_t* y = t->nil;

  while (x->left != NULL){
    y = x;
    x = x->left;
  }
  return y;
}

/* MAXIMUM */
node_t *rbtree_max(const rbtree *) {
  // TODO: implement find
  node_t *x = t->root;
  node_t *y = t->nil;

  while (x->right != NULL){
    y = x;
    x = x->right;
  }
  return y;
}

/* RBTREE_ERASE */
// ERASE P from rbtree t
// reference : CRLS
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  // CRLS pseudo code variable changed (CRLS : VSCODE)
  // (y : y), (z : p), (T : t), 
  
  node_t *x;
  node_t *y = p;                 // 삭제하려는 노드 저장
  color_t y_original_color = y->color;   // y의 기본 색 저장
  
  // 자식 노드가 right만 있을 경우
  if (p->left == t->nil){
    x = p->right;
    /* RB-TRANSPLANT(t, p, p->right)*/
    rbtree_transplant(t, p, p->right);
  }
  
  // 자식 노드가 left만 있을 경우
  else if (p->right == t->nil){
    x = p->left;
    /* RB-TRANSPLANT(t, p, p->left)*/
    rbtree_transplant(t, p, p->left);
  }

  // 자식 노드가 2개인 경우
  else {
    
    // p의 오른쪽 기준으로 최솟값 저장 (node_t 값으로 구현)
    node_t *m = t->root->right;
    node_t *n = t->nil;

    while (m->left != NULL) {
      n = m;
      n = n->left;
    }
    y = n;

    y_original_color = y->color;  // 최솟값의 색깔을 해당 변수에 저장시킴
    x = p->right;         // x는 p의 자녀
    
    // y의 부모가 p일 경우
    if (y->parent == p){
      x->parent = y;
    }
    
    // y의 부모가 p가 아닐 경우
    else {
      /* RB-TRANSPLANT(t, y, y->right) */ // y의 오른쪽 자식으로 transparent
      rbtree_transplant(t, y, y->right);
      y->right = p->right;   // y의 오른쪽 자식을 p의 오른쪽 자식으로 할당 후 
      y->right->parent = y;  // y->right의 부모로 y를 설정
    }

    /* RB-TRANSPLANT(t, p, y) */
    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (y_original_color == RBTREE_BLACK){
    rbtree_delete_fixup(t, x);
  }

  return 0;
}

/* RB-DELETE-FIXUP */
// reference : CRLS
void rbtree_delete_fixup(rbtree *t, node_t *x){
  // CRLS pseudo code variable changed (CRLS : VSCODE)
  // (x : x), (w : w)

  while ((x != t->root) && (x->color == RBTREE_BLACK)){
    if (x == x->parent->left){
      node_t *w = x->parent->right;
      
      // 경우 1 시작
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }

      // 경우 2 시작
      if ((w->left->color==RBTREE_BLACK) && (w->right->color==RBTREE_BLACK)){
        w->color = RBTREE_RED;
        x = x->parent;
      }

      // 경우 3 시작
      else {
        if (w->right->color == RBTREE_BLACK){
          w->left->color == RBTREE_BLACK;
          w->color == RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }

        // 경우 4 시작
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }

    // else
    else {
      node_t *w = x->parent->left;
      
      // else 경우 1 시작
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      // else 경우 2 시작
      if ((w->right->color==RBTREE_BLACK) && (w->left->color==RBTREE_BLACK)){
        w->color = RBTREE_RED;
        x = x->parent;
      }

      // else 경우 3 시작
      else {
        if (w->left->color == RBTREE_BLACK){
          w->right->color == RBTREE_BLACK;
          w->color == RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }

        // else 경우 4 시작
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }

  x->color = RBTREE_BLACK;
  }
}



/* RB-TRANSPARENT */ 
// reference : CRLS
// 노드 u가 루트인 서브트리 -> 노드 v가 루트인 서브트리
void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  // u의 부모가 없을 경우 v = root
  if (u->parent == t->nil){
    t->root = v;
  }
  else if (u == u->parent->left){
    u->parent->left = v;
  }

  else {u->parent->right = v;}

  v->parent = u->parent;
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
