/*
 * llist.c
 *
 *  Created on: 13 de abr de 2016
 *      Author: Cristóvão Rufino <cristovaozr@gmail.com>
 *		Co-author: Estêvão Rufino <estevaozr@gmail.com>
 */

#include "llist.h"
#include <string.h>
#include <stdlib.h>

void llist_init(llist_root_t *root) {
	root->llist_count = 0;
	root->head = root->tail = NULL;
}

llist_node_t *llist_new(void *data) {
	llist_node_t *ret = NULL;

	ret = (llist_node_t *)malloc(sizeof(llist_node_t));
	if(ret == NULL) goto exit;

	ret->data = data;
	ret->next = NULL;
	ret->prev = NULL;

exit:
	return ret;
}

void *llist_free(llist_node_t *node) {
	void *ret = NULL;

	if(node == NULL) goto exit;
	ret = node->data;
	free(node);

exit:
	return ret;
}

unsigned long llist_get_size(llist_root_t *root) {
	return root->llist_count;
}

llist_node_t *llist_walk_right(llist_node_t *root) {
	return root->next;
}

llist_node_t *llist_walk_left(llist_node_t *root) {
	return root->prev;
}

llist_node_t *llist_remove_tail(llist_root_t *root) {
	llist_node_t *ret = NULL;

	if(root->tail == NULL) goto exit;

	ret = root->tail;

	if(ret == root->head) { root->head = NULL; }
	root->tail = ret->prev;
	if(ret->prev) {
		ret->prev->next = NULL;
		ret->prev = NULL;
	}

	root->llist_count--;

exit:
	return ret;
}

llist_node_t *llist_remove_head(llist_root_t *root) {
	llist_node_t *ret = NULL;

	if(root->head == NULL) goto exit;

	ret = root->head;

	if(ret == root->tail) { root->tail = NULL; }
	root->head = ret->next;
	if(ret->next) {
		ret->next->prev = NULL;
		ret->next = NULL;
	}

	root->llist_count--;

exit:
	return ret;
}

llist_node_t *llist_remove_idx(llist_root_t *root, unsigned long idx) {
	unsigned long i;
	llist_node_t *ret = NULL;

	for(i = 0, ret = root->head; i < idx && ret->next; i++, ret = ret->next);

	if(ret) {
		if(ret == root->tail) { root->tail = ret->prev; }
		if(ret == root->head) { root->head = ret->next; }
		if(ret->next) { ret->next->prev = ret->prev; }
		if(ret->prev) { ret->prev->next = ret->next; }

		root->llist_count--;
	}

	return ret;
}

llist_node_t *llist_remove_node(llist_root_t *root, llist_node_t *node) {
	if(node == NULL)
		return NULL;

	if(root->head == node) { root->head = node->next; }
	if(root->tail == node) { root->tail = node->prev; }

	if(node->next) { node->next->prev = node->prev; }
	if(node->prev) { node->prev->next = node->next; }

	root->llist_count--;

	node->next = node->prev = NULL;

	return node;
}

void llist_insert_tail(llist_root_t *root, llist_node_t *node) {

	if(root->head == NULL) root->head = node;

	if(root->tail) {
		root->tail->next = node;
		node->prev = root->tail;
	}

	root->tail = node;
	root->llist_count++;
}

void llist_insert_head(llist_root_t *root, llist_node_t *node) {

	if(root->tail == NULL) root->tail = node;

	if(root->head) {
		root->head->prev = node;
		node->next = root->head;
	}

	root->head = node;
	root->llist_count++;
}

void llist_insert_idx(llist_root_t *root, llist_node_t *node, unsigned long idx) {
	unsigned long i;
	llist_node_t *it;

	if(idx == 0) {
		llist_insert_head(root, node);
		return;
	}

	if(idx >= root->llist_count) {
		llist_insert_tail(root, node);
		return;
	}

	for(i = 0, it = root->head; i < idx && it->next; i++, it = it->next);

	if(it) {
		node->prev = it->prev;
		it->prev->next = node;
		node->next = it;
		it->prev = node;
		root->llist_count++;
	}
}

void *llist_get_data(llist_node_t *node) {
	return node->data;
}
void llist_set_data(llist_node_t *node, void *data) {
	node->data = data;
}

llist_node_t *llist_get_idx(llist_root_t *root, unsigned long idx) {
	unsigned long i;
	llist_node_t *ret;

	for(i = 0, ret = root->head; i < idx && ret->next; i++, ret = ret->next);

	if(i != idx)
		ret = NULL;

	return ret;
}

llist_node_t *llist_get_head(llist_root_t *root) {
	return root->head;
}

llist_node_t *llist_get_tail(llist_root_t *root) {
	return root->tail;
}

void llist_swap_nodes(llist_node_t *a, llist_node_t *b) {
	void *t;

	t = a->data;
	a->data = b->data;
	b->data = t;
}

void llist_spush(llist_root_t *root, llist_node_t *node) {
	llist_insert_head(root, node);
}

llist_node_t *llist_spop(llist_root_t *root) {
	return llist_remove_head(root);
}

void llist_qpush(llist_root_t *root, llist_node_t *node) {
	llist_insert_tail(root, node);
}

llist_node_t *llist_qpop(llist_root_t *root) {
	return llist_remove_head(root);
}

static void quick_sort_iteration (llist_node_t *ll, llist_node_t *rr,
	int (*comp)(const void *v1, const void *v2))
{
	llist_node_t *p, *l, *r;
	int run;

	if(ll == NULL) return;
	if(rr == NULL) return;
	if(ll == rr) return;
	if(rr->next == ll) return;

	p = ll;
	l = p->next;
	r = rr;

	run = 1;
	while(run) {
		for(;;) {
			if(l == rr->next) {
				run = 0;
				break;
			}

			if(comp(p->data, l->data) < 0)
				break;

			l = l->next;
		}

		for(;;) {
			if(r == ll) {
				run = 0;
				break;
			}

			if(comp(p->data, r->data) >= 0)
				break;

			r = r->prev;
		}

		if(r->next == l)
			run = 0;

		if(run) {
			llist_swap_nodes(l, r);
		} else {
			llist_swap_nodes(p, r);
		}
	}

	quick_sort_iteration(p, r->prev, comp);
	quick_sort_iteration(l, rr, comp);
}

void llist_qsort(llist_root_t *root,
	int (*comp)(const void *v1, const void *v2))
{
	quick_sort_iteration(root->head, root->tail, comp);
}

llist_node_t *llist_bsearch(llist_root_t *root, const void *e,
	int (*comp)(const void *v1, const void *v2))
{
	int i;
	int low, nmid, cmid, high;
	int cp;
	llist_node_t *node;

	if(root->llist_count == 0)
		return NULL;

	low = 0;
	high = root->llist_count-1;

	node = root->head;
	cmid = 0;

	do {
		nmid = (low + high)/2;

		if(nmid > cmid)
			for(i=0; i<(nmid-cmid); i++)
				node = node->next;
		else
			for(i=0; i<(cmid-nmid); i++)
				node = node->prev;

		cmid = nmid;

		cp = comp(e, node->data);

		if(cp < 0)
			high = cmid - 1;
		else if (cp > 0)
			low = cmid + 1;
		else
			return node;
	} while (low <= high);

	return NULL;
}

void llist_sinsert(llist_root_t *root, llist_node_t *e,
	int (*comp)(const void *v1, const void *v2))
{
	llist_node_t *node;

	int i;
	int low, nmid, cmid, high;
	int cp;

	if(root->llist_count == 0) {
		llist_insert_head(root, e);
		return;
	}

	low = 0;
	high = root->llist_count-1;

	node = root->head;
	cmid = 0;

	do {
		nmid = (low + high)/2;

		if(nmid > cmid)
			for(i=0; i<(nmid-cmid); i++)
				node = node->next;
		else
			for(i=0; i<(cmid-nmid); i++)
				node = node->prev;

		cmid = nmid;

		cp = comp(e->data, node->data);

		if(cp < 0)
			high = cmid - 1;
		else if (cp > 0)
			low = cmid + 1;
		else {
			llist_insert_idx(root, e, cmid);
			return;
		}
	} while(low <= high);

	if(cp > 0)
		llist_insert_idx(root, e, cmid+1);
	else
		llist_insert_idx(root, e, cmid);
}
