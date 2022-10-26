#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

/* NEW RB_TREE */
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));          // rbtree 구조체에 대한 메모리 할당
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));   // node 구조체에 대한 메모리 할당

  // TODO: initialize struct if needed
  new_node->color = RBTREE_BLACK;                           // new_node는 루트이자 nil이므로 black으로 설정 
  p->nil = new_node;                                        // sentinel으로 접근
  p->root = p->nil;

  return p;                                                 // rbtree 자체를 return
}

/* DELETE_NODE */
// 후위순회로 구성함 (postorder)
void delete_node(rbtree* t, node_t* rnode) {
  if (rnode == t->nil) {         // rnode가 t->nil에 접근하면 return
    return;
  }
  delete_node(t, rnode->left);   // 후위순회로 뒤에서부터 접근
  delete_node(t, rnode->right);
  free(rnode);                   // 재귀 종료 후 rnode 메모리 해제
}

/* DELETE_NODE */
// delete_node를 활용해서 노드 삭제
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t, t->root);
  free(t->nil);                 // t->nil에 대한 메모리 해제
  free(t);                      // t 구조체도 메모리 해제
}


/* RBTREE INSERT */
// reference : CLRS pseudo code
// rbtree_insert 후 node_t가 return되어야 하므로 자료형에 node_t로 정의
node_t *rbtree_insert(rbtree *t, const key_t key) { // 트리와 insert할 값을 넣음
  // TODO: implement insert
  node_t *y = t->nil;  // t는 인자로 들고오므로 node_t *y로 설정함
  node_t *x = t->root; // root에 대한 구조체 포인터 설정
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t)); // z에 대한 새로운 노드
  new_node->key = key; // z에 대한 key값 설정

  while ( x != t->nil )
  {
    y = x;             // y를 x의 부모로 만들어줘서 x의 값을 저장
    
    if (key < x->key)
      x = x->left;     // x의 값은 계속해서 내려줌
    else x = x->right;

  }

  // line 8: z.p = y
  new_node->parent = y; // 삽입할 값의 부모는 y로 설정

  // line 9 ~ line 13
  if (y == t->nil)       // 처음부터 nil으로 빈 트리를 의미
    t->root = new_node;  
  else if (new_node->key < y->key) // new_node의 키값이 y의 key값보다 적을시
    y->left = new_node;            // y->left가 new_node가 됨
  else y->right = new_node;

  new_node->left = t->nil;         // nil값 설정
  new_node->right = t->nil;
  new_node->color = RBTREE_RED;    // 초깃값은 red로 설정
  
  insert_fixup(t, new_node); // insert_fixup 함수 설정 (rb트리 속성을 만족시키기 위해)
  return t->root;
}

/* Insert-FIXUP */ 
// reference : CLRS pseudo code
// Insert-Fixup의 경우 반환하는 값이 없고 insert에 대한 수정만 이루기 때문에 void로 설정함
// rbtree.h에도 똑같이 설정 필요(?)
void insert_fixup(rbtree *t, node_t *new_node){
  // CLRS Terms ; (CLRS ; VS code)
  // (T ; t), (z ; new_node), (p ; parent), 
  while (new_node->parent->color == RBTREE_RED){
    if (new_node->parent == new_node->parent->parent->left){
      node_t *y = new_node->parent->parent->right; // line 3 make uncle

      // 경우 1 :
      // 삽입된 red 노드의 부모도 red & 삼촌도 red라면
      // 부모와 삼촌을 black으로 바꾸고 할아버지를 red로 바꾼 뒤
      // 할아버지에서 다시 확인을 시작함
      if (y->color == RBTREE_RED){
        new_node->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = new_node->parent->parent;
      }
      else { // 여기에서 psuedo code는 else if로 되어 있는데 아래와 같이 바꿔줘야함
        // 경우 2:
        // 삽입된 red 노드가 부모의 오른쪽(혹은 왼쪽)자녀 &
        // 부모도 red이고 할아버지의 왼쪽(혹은 오른쪽)자녀 &
        // 삼촌은 black 이라면
        // 부모를 기준으로 왼쪽 (혹은 오른쪽)으로 회전한 뒤
        // case3의 방식으로 해결
        if (new_node == new_node->parent->right) {
        new_node = new_node->parent;
        // LEFT - ROTATE (T, Z)
        left_rotate(t, new_node);
      }

      // 경우3:
      // 삽입된 red 노드가 부모의 왼쪽 (혹은 오른쪽) 자녀 &
      // 부모도 red이고 할아버지의 왼쪽 (혹은 오른쪽) 자녀 &
      // 삼촌은 black이라면
      // 부모와 할아버지의 색을 바꾼 후
      // 할아버지 기준으로 오른쪽(왼쪽)으로 회전한다.
      new_node->parent->color = RBTREE_BLACK;
      new_node->parent->parent->color = RBTREE_RED;
      // RIGHT - ROTATE (T, Z.P.P)
      right_rotate(t, new_node->parent->parent);
      }
  }

  // 위의 경우와 LEFT - RIGHT만 바꿔줌 (대칭)
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
// LEFT-ROTATE의 경우에도 반환값이 없어서 void값 설정함
// T = rbtree *t , x = new_node
void left_rotate(rbtree *t, node_t *new_node) {
  node_t *y = new_node->right;       // y를 설정
  new_node->right = y->left;         // y의 왼쪽 서브트리를 new_node의 오른쪽 서브트리로 옮김 (자식설정)
  if (y->left != t->nil){        
    y->left->parent = new_node;      // y의 왼쪽 서브트리의 부모를 new_node로 옮김 (부모설정)
  }
  y->parent = new_node->parent;      // new_node의 부모를 y로 연결 (서브트리의 루트를 만들기 위한 코드)

  if (new_node->parent == t->nil){   // new_node의 부모가 t->nil일 시에 t->root = y
    t->root = y;
  }

  else if (new_node == new_node->parent->left){ 
    new_node->parent->left = y;      // new_node의 부모의 왼쪽으로 자식을 설정
  }

  else new_node->parent->right = y;  // new_node의 부모의 오른쪽으로 자식을 설정

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
node_t *rbtree_min(const rbtree *t) {
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
node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *x = t->root;
  node_t *y = t->nil;

  while (x->right != NULL){
    y = x;
    x = x->right;
  }
  return y;
}


/* MINIMUM_num */
// 서브트리에서 Min값 찾는 함수
node_t *rbtree_min_num(const rbtree *t, node_t *min_node) {
  // TODO: implement find
  while (min_node->left != t->nil){
    min_node = min_node->left;
  }
  return min_node;
}

/* RBTREE_ERASE */
// ERASE P from rbtree t
// reference : CRLS
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  // CRLS pseudo code variable changed (CRLS : VSCODE)
  // (y : y), (z : p), (T : t), 
  
  node_t *x;
  node_t *y;
  y = p;                 // 삭제하려는 노드 저장
  color_t y_original_color = y->color;   // y의 기본 색 저장
  
  // 자식 노드가 right만 있을 경우
  if (p->left == t->nil){
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }
  
  // 자식 노드가 left만 있을 경우
  else if (p->right == t->nil){
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }

  // 자식 노드가 2개인 경우
  else {
    y = rbtree_min_num(t, p->right); // 삭제되는 값의 successor를 찾는 과정

    y_original_color = y->color;  // 최솟값의 색깔을 해당 변수에 저장시킴
    x = y->right;         // x는 p의 자녀
    
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

  if (y_original_color == RBTREE_BLACK) {
    rbtree_delete_fixup(t, x);
  }
  free(p);
  return 0;
}

/* RB-DELETE-FIXUP */
// reference : CRLS
void rbtree_delete_fixup(rbtree *t, node_t *x){
  // CRLS pseudo code variable changed (CRLS : VSCODE)
  // (x : x), (w : w)
  node_t *w;

  while ((x != t->root) && (x->color == RBTREE_BLACK)){
    if (x == x->parent->left){
      w = x->parent->right;
      
      // 경우 1 시작
      // doubly black의 형제가 red일 때
      // doubly black의 형제를 black으로 만든 후 case2, 3, 4 중 하나로 해결
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }

      // 경우 2 시작
      // doubly black의 형제가 black & 그 형제의 두 자녀 모두 black일 때 
      // doubly black과 그 형제의 black을 모아서 부모에게 전달해서 부모가 extra black을 해결하도록 위임
      if ((w->left->color==RBTREE_BLACK) && (w->right->color==RBTREE_BLACK)){
        w->color = RBTREE_RED;
        x = x->parent;
      }

      // 경우 3 시작
      // doubly black의 오른쪽형제가 black & 그 형제의 왼쪽자녀가 red & 
      // 그 형제의 오른쪽 자녀는 Black 
      else {
        if (w->right->color == RBTREE_BLACK){
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }

        // 경우 4 시작
        // doubly black의 오른쪽 형제가 black & 그 형제의 오른쪽 자녀가 red일 때 
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
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
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
  }
  x->color = RBTREE_BLACK;
  
}


/* RB-TRANSPARENT */ 
// reference : CRLS pseudo code
// 노드 u가 루트인 서브트리 -> 노드 v가 루트인 서브트리
void rbtree_transplant(rbtree *t, node_t *u, node_t *v){ 

  // u의 부모가 없을 경우 v = root
  if (u->parent == t->nil){   // 루트의 부모는 t->nil이므로
    t->root = v;              // v는 루트가 됨
  }
  else if (u == u->parent->left){ // 왼쪽에 자식이 있으면 왼쪽 자식을 새로 설정
    u->parent->left = v;
  }

  else {u->parent->right = v;}    // 오른쪽에 자식이 있으면 오른쪽 자식을 새로 설정

  v->parent = u->parent;          // 자식 기준에서 부모를 설정

  // u 메모리를 해제 안해줘도 되나? 떠다닐 것 같음
}


// 중위순회 함수 선언
int inorder(const rbtree *t, node_t *p, key_t *arr, int idx, int n){ 
  if (p == t->nil || idx >= n){
    return idx;
  }
  printf("%d", p->key);
  
  idx = inorder(t, p->left, arr, idx, n);
  arr[idx++] = p->key;
  idx = inorder(t, p->right, arr, idx, n);
  return idx;
}

// 중위순회, 배열의 값을 넣어야함
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  inorder(t, t->root, arr, 0, n);
  return 0;
}

