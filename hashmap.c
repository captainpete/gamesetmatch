#include "uthash.h"
#include "hashmap.h"

hashmap_t* hashmap_new(){
	hashmap_t *self = malloc(sizeof(hashmap_t));
	self->size       = 0;
	self->map       = NULL;
	return self;
}


void hashmap_add(hashmap_t* self, void* key, size_t keylen, void *value){
	hashmap_item_t *item = malloc(sizeof(hashmap_item_t));
	item->key   = key;
	item->value = value;
	self->size++;
	HASH_ADD_KEYPTR(hh, self->map, item->key, keylen, item);
}

hashmap_item_t* hashmap_find_item(hashmap_t* self, void* key, size_t keylen){
	hashmap_item_t *ret;
	HASH_FIND(hh, self->map, key, keylen, ret);
	return ret;
}

void* hashmap_find(hashmap_t* self, void* key, size_t keylen){
	hashmap_item_t *item = hashmap_find_item(self, key, keylen);
	if(item) return item->value;
	return NULL;
}

void hashmap_destroy(hashmap_t *self){
	free(self);
}

void hashmap_del_item(hashmap_t* self, hashmap_item_t *item){
	HASH_DEL(self->map, item);
	self->size--;
	free(item);
}

void* hashmap_del(hashmap_t* self, void* key, size_t keylen){
	hashmap_item_t *item = hashmap_find_item(self, key, keylen);
	void* ret = NULL;
	if(item){
		ret = item->value;
		hashmap_del_item(self, item);
	}
	return ret;
}