/*
 * llist.h
 *
 *  Created on: 13 de abr de 2016
 *      Author: Cristóvão Rufino <cristovaozr@gmail.com>
 *		Co-author: Estêvão Rufino <estevaozr@gmail.com>
 */

#ifndef LIB_LLIST_LLIST_H_
#define LIB_LLIST_LLIST_H_

typedef struct llist_node {
	struct llist_node *next, *prev;
	void *data;
} llist_node_t;

typedef struct llist_root {
	unsigned long llist_count;
	llist_node_t *head;
	llist_node_t *tail;
} llist_root_t;

/**
 * função que inicializa uma lista
 */
extern void llist_init(llist_root_t *root);

/**
 * funções que criam e destroem llist_node_t
 */
extern llist_node_t *llist_new(void *data);
extern void *llist_free(llist_node_t *node);

/**
 * funções miscelânea
 */
extern unsigned long llist_get_size(llist_root_t *root);

/**
 * funções que andam na lista
 */
extern llist_node_t *llist_walk_right(llist_node_t *root);
extern llist_node_t *llist_walk_left(llist_node_t *root);

/**
 * função que troca o dado de dois llist_node_t
 */
extern void llist_swap_nodes(llist_node_t *a, llist_node_t *b);

/**
 * funções que removem llist_node_t da lista
 */
extern llist_node_t *llist_remove_tail(llist_root_t *root);
extern llist_node_t *llist_remove_head(llist_root_t *root);
extern llist_node_t *llist_remove_idx(llist_root_t *root, unsigned long idx);
extern llist_node_t *llist_remove_node(llist_root_t *root, llist_node_t *node);

/**
 * funções que inserem na lista
 */
extern void llist_insert_tail(llist_root_t *root, llist_node_t *node);
extern void llist_insert_head(llist_root_t *root, llist_node_t *node);
extern void llist_insert_idx(llist_root_t *root, llist_node_t *node, unsigned long idx);

/**
 * funções que modificam e configuram os dados
 */
extern void *llist_get_data(llist_node_t *node);
extern void llist_set_data(llist_node_t *node, void *data);

/**
 * funções que pegam llist_node_t da lista (equivalente a peek())
 */
extern llist_node_t *llist_get_idx(llist_root_t *root, unsigned long idx);
extern llist_node_t *llist_get_head(llist_root_t *root);
extern llist_node_t *llist_get_tail(llist_root_t *root);

/**
 * llist_s(push/pop): funções para comportamento em pilha
 */
extern void llist_spush(llist_root_t *root, llist_node_t *node);
extern llist_node_t *llist_spop(llist_root_t *root);

/**
 * llist_q(push/pop): funções para comportamento em fila
 */
extern void llist_qpush(llist_root_t *root, llist_node_t *node);
extern llist_node_t *llist_qpop(llist_root_t *root);

/**
 * funções para operações ordenadas com a lista
 */

extern void llist_qsort(llist_root_t *root, int (*comp)(const void *v1, const void *v2));
extern llist_node_t *llist_bsearch(llist_root_t *root, const void *e, int (*comp)(const void *v1, const void *v2));
extern void llist_sinsert(llist_root_t *root, llist_node_t *node, int (*comp)(const void *v1, const void *v2));

#endif /* LIB_LLIST_LLIST_H_ */

