#include "OpenRelTable.h"
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
OpenRelTable::OpenRelTable() {

  // initialize relCache and attrCache with nullptr
  for (int i = 0; i < MAX_OPEN; ++i) {
    RelCacheTable::relCache[i] = nullptr;
    AttrCacheTable::attrCache[i] = nullptr;
  }

  /************ Setting up Relation Cache entries ************/
  // (we need to populate relation cache with entries for the relation catalog
  //  and attribute catalog.)

  /**** setting up Relation Catalog relation in the Relation Cache Table****/
  RecBuffer relCatBlock(RELCAT_BLOCK);

  Attribute relCatRecord[RELCAT_NO_ATTRS];
  relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_RELCAT);

  struct RelCacheEntry relCacheEntry;
  RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry.relCatEntry);
  relCacheEntry.recId.block = RELCAT_BLOCK;
  relCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_RELCAT;

  // allocate this on the heap because we want it to persist outside this function
  RelCacheTable::relCache[RELCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[RELCAT_RELID]) = relCacheEntry;



  /**** setting up Attribute Catalog relation in the Relation Cache Table ****/

  // set up the relation cache entry for the attribute catalog similarly
  // from the record at RELCAT_SLOTNUM_FOR_ATTRCAT

  // set the value at RelCacheTable::relCache[ATTRCAT_RELID]


  relCatBlock.getRecord(relCatRecord,RELCAT_SLOTNUM_FOR_ATTRCAT);

  RelCacheTable::recordToRelCatEntry(relCatRecord,&relCacheEntry.relCatEntry);
  relCacheEntry.recId.block=RELCAT_BLOCK;
  relCacheEntry.recId.slot=RELCAT_SLOTNUM_FOR_ATTRCAT;

  // allocate this on the heap because we want it to persist outside this function
  RelCacheTable::relCache[ATTRCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[ATTRCAT_RELID]) = relCacheEntry;


  


  /************ Setting up Attribute cache entries ************/
  // (we need to populate attribute cache with entries for the relation catalog
  //  and attribute catalog.)

  /**** setting up Relation Catalog relation in the Attribute Cache Table ****/
  RecBuffer attrCatBlock(ATTRCAT_BLOCK);

  Attribute attrCatRecord[ATTRCAT_NO_ATTRS];

  // iterate through all the attributes of the relation catalog and create a linked
  // list of AttrCacheEntry (slots 0 to 5)
  // for each of the entries, set
  //    attrCacheEntry.recId.block = ATTRCAT_BLOCK;
  //    attrCacheEntry.recId.slot = i   (0 to 5)
  //    and attrCacheEntry.next appropriately
  // NOTE: allocate each entry dynamically using malloc
  AttrCacheEntry *head,*temp;
  head= temp=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
  //temp=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
  //head=temp;
  for(int i=0;i<5;i++){
    temp->next=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
    temp=temp->next;
  }
  temp->next=NULL;
  temp=head;
  for(int i=0;i<6;i++){
    attrCatBlock.getRecord(attrCatRecord,i);
    AttrCacheTable::recordToAttrCatEntry(attrCatRecord,&temp->attrCatEntry);
    temp->recId.block=ATTRCAT_BLOCK;
    temp->recId.slot=i;
    temp=temp->next;
  }

  // set the next field in the last entry to nullptr

  AttrCacheTable::attrCache[RELCAT_RELID] = /* head of the linked list */head;
  head=NULL;
  /**** setting up Attribute Catalog relation in the Attribute Cache Table ****/
  head=temp=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
  for(int i=0;i<5;i++){
    temp->next=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
    temp=temp->next;
  }
  temp->next=NULL;
  temp=head;
  for(int i=6;i<12;i++){
    attrCatBlock.getRecord(attrCatRecord,i);
    AttrCacheTable::recordToAttrCatEntry(attrCatRecord,&temp->attrCatEntry);
    temp->recId.block=ATTRCAT_BLOCK;
    temp->recId.slot=i;
    temp=temp->next;
  }

  // set the next field in the last entry to nullptr

  AttrCacheTable::attrCache[ATTRCAT_RELID] = /* head of the linked list */head;
  head=NULL;

  // set up the attributes of the attribute cache similarly.
  // read slots 6-11 from attrCatBlock and initialise recId appropriately

  // set the value at AttrCacheTable::attrCache[ATTRCAT_RELID]
  
}

OpenRelTable::~OpenRelTable() {
  // free all the memory that you allocated in the constructor
}