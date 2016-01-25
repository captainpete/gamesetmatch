#ifndef HASHMAP_H
#define HASHMAP_H
#include "uthash.h"
typedef struct hashmap_item {
	void *key;
	void *value;
	UT_hash_handle hh;
} hashmap_item_t;

typedef struct hashmap {
	int size;
	hashmap_item_t *map;
} hashmap_t;

#define HASHMAP_TEMP(_item) _item ## _temp
#define HASHMAP_ITER(_item) _item ## _i
#define HASHMAP_MISC(_item) _item ## _misc

hashmap_t*      hashmap_new       (void);
void            hashmap_add       (hashmap_t *self, void *key, size_t keylen, void *value);
hashmap_item_t* hashmap_find_item (hashmap_t *self, void *key, size_t keylen);
void*           hashmap_find      (hashmap_t *self, void *key, size_t keylen);
void            hashmap_destroy   (hashmap_t *self);
void            hashmap_del_item  (hashmap_t *self, hashmap_item_t *item);
void*           hashmap_del       (hashmap_t *self, void *key, size_t keylen);

#define hashmap_each(_map, _item) \
for(hashmap_item_t* HASHMAP_ITER(_item) = (_map)->map, \
	*HASHMAP_TEMP(_item) = ((_map)->map ? (_map)->map->hh.next : NULL), \
	*HASHMAP_MISC(_item) = ((_item = HASHMAP_TEMP(_item) ? HASHMAP_TEMP(_item)->value : NULL), HASHMAP_TEMP(_item)); \
	HASHMAP_ITER(_item); \
	HASHMAP_ITER(_item) = HASHMAP_TEMP(_item), HASHMAP_TEMP(_item) =  HASHMAP_TEMP(_item) ?  HASHMAP_TEMP(_item)->hh.next : NULL, _item =  HASHMAP_ITER(_item) ? HASHMAP_ITER(_item)->value : NULL )



#endif
