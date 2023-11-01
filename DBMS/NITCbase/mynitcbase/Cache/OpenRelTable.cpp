// #include "OpenRelTable.h"
// #include <stdlib.h>
// #include <stdio.h>
// #include <cstring>
// #include <iostream>
// OpenRelTableMetaInfo OpenRelTable::tableMetaInfo[MAX_OPEN];
// OpenRelTable::OpenRelTable() {

//   // initialize relCache and attrCache with nullptr
//   for (int i = 0; i < MAX_OPEN; ++i) {
//     RelCacheTable::relCache[i] = nullptr;
//     AttrCacheTable::attrCache[i] = nullptr;
//     tableMetaInfo[i].free=true;
//   }

//   /************ Setting up Relation Cache entries ************/
//   // (we need to populate relation cache with entries for the relation catalog
//   //  and attribute catalog.)

//   /**** setting up Relation Catalog relation in the Relation Cache Table****/
//   RecBuffer relCatBlock(RELCAT_BLOCK);

//   Attribute relCatRecord[RELCAT_NO_ATTRS];
//   relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_RELCAT);

//   struct RelCacheEntry relCacheEntry;
//   RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry.relCatEntry);
//   relCacheEntry.recId.block = RELCAT_BLOCK;
//   relCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_RELCAT;

//   // allocate this on the heap because we want it to persist outside this function
//   RelCacheTable::relCache[RELCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
//   *(RelCacheTable::relCache[RELCAT_RELID]) = relCacheEntry;



//   /**** setting up Attribute Catalog relation in the Relation Cache Table ****/

//   // set up the relation cache entry for the attribute catalog similarly
//   // from the record at RELCAT_SLOTNUM_FOR_ATTRCAT

//   // set the value at RelCacheTable::relCache[ATTRCAT_RELID]


//   relCatBlock.getRecord(relCatRecord,RELCAT_SLOTNUM_FOR_ATTRCAT);

//   RelCacheTable::recordToRelCatEntry(relCatRecord,&relCacheEntry.relCatEntry);
//   relCacheEntry.recId.block=RELCAT_BLOCK;
//   relCacheEntry.recId.slot=RELCAT_SLOTNUM_FOR_ATTRCAT;

//   // allocate this on the heap because we want it to persist outside this function
//   RelCacheTable::relCache[ATTRCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
//   *(RelCacheTable::relCache[ATTRCAT_RELID]) = relCacheEntry;



//   relCatBlock.getRecord(relCatRecord,2);

//   RelCacheTable::recordToRelCatEntry(relCatRecord,&relCacheEntry.relCatEntry);
//   relCacheEntry.recId.block=RELCAT_BLOCK;
//   relCacheEntry.recId.slot=2;

//   // allocate this on the heap because we want it to persist outside this function
//   RelCacheTable::relCache[2] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
//   *(RelCacheTable::relCache[2]) = relCacheEntry;

  


//   /************ Setting up Attribute cache entries ************/
//   // (we need to populate attribute cache with entries for the relation catalog
//   //  and attribute catalog.)

//   /**** setting up Relation Catalog relation in the Attribute Cache Table ****/
//   RecBuffer attrCatBlock(ATTRCAT_BLOCK);

//   Attribute attrCatRecord[ATTRCAT_NO_ATTRS];

//   // iterate through all the attributes of the relation catalog and create a linked
//   // list of AttrCacheEntry (slots 0 to 5)
//   // for each of the entries, set
//   //    attrCacheEntry.recId.block = ATTRCAT_BLOCK;
//   //    attrCacheEntry.recId.slot = i   (0 to 5)
//   //    and attrCacheEntry.next appropriately
//   // NOTE: allocate each entry dynamically using malloc
//   AttrCacheEntry *head,*temp;
//   head= temp=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//   //temp=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//   //head=temp;
//   for(int i=0;i<5;i++){
//     temp->next=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//     temp=temp->next;
//   }
//   temp->next=NULL;
//   temp=head;
//   for(int i=0;i<6;i++){
//     attrCatBlock.getRecord(attrCatRecord,i);
//     AttrCacheTable::recordToAttrCatEntry(attrCatRecord,&temp->attrCatEntry);
//     temp->recId.block=ATTRCAT_BLOCK;
//     temp->recId.slot=i;
//     temp=temp->next;
//   }

//   // set the next field in the last entry to nullptr

//   AttrCacheTable::attrCache[RELCAT_RELID] = /* head of the linked list */head;
//   head=NULL;
//   /**** setting up Attribute Catalog relation in the Attribute Cache Table ****/
//   head=temp=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//   for(int i=0;i<5;i++){
//     temp->next=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//     temp=temp->next;
//   }
//   temp->next=NULL;
//   temp=head;
//   for(int i=6;i<12;i++){
//     attrCatBlock.getRecord(attrCatRecord,i);
//     AttrCacheTable::recordToAttrCatEntry(attrCatRecord,&temp->attrCatEntry);
//     temp->recId.block=ATTRCAT_BLOCK;
//     temp->recId.slot=i;
//     temp=temp->next;
//   }

//   // set the next field in the last entry to nullptr

//   AttrCacheTable::attrCache[ATTRCAT_RELID] = /* head of the linked list */head;
//   head=NULL;

//   // set up the attributes of the attribute cache similarly.
//   // read slots 6-11 from attrCatBlock and initialise recId appropriately

//   // set the value at AttrCacheTable::attrCache[ATTRCAT_RELID]
//   //exercise
// head=temp=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//   for(int i=0;i<RelCacheTable::relCache[2]->relCatEntry.numAttrs;i++){
//     temp->next=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//     temp=temp->next;
//   }
//   temp->next=NULL;
//   temp=head;
//   for(int i=12;i<12+RelCacheTable::relCache[2]->relCatEntry.numAttrs;i++){
//     attrCatBlock.getRecord(attrCatRecord,i);
//     AttrCacheTable::recordToAttrCatEntry(attrCatRecord,&temp->attrCatEntry);
//     temp->recId.block=ATTRCAT_BLOCK;
//     temp->recId.slot=i;
//     temp=temp->next;
//   }

//   // set the next field in the last entry to nullptr

//   AttrCacheTable::attrCache[2] = /* head of the linked list */head;
//   head=NULL;
//   /************ Setting up tableMetaInfo entries ************/

//   // in the tableMetaInfo array
//   //   set free = false for RELCAT_RELID and ATTRCAT_RELID
//   //   set relname for RELCAT_RELID and ATTRCAT_RELID
//   tableMetaInfo[0].free=false;
//   tableMetaInfo[1].free=false;

//   strcpy(tableMetaInfo[0].relName,RELCAT_RELNAME);
//   strcpy(tableMetaInfo[1].relName,ATTRCAT_RELNAME);
  
// }

// // OpenRelTable::~OpenRelTable() {
// //   // free all the memory that you allocated in the constructor
// //   for (int i = 2; i < MAX_OPEN; ++i) {
// //     if (!tableMetaInfo[i].free) {
// //       OpenRelTable::closeRel(i); // we will implement this function later
// //     }
// //   }
// // }
// /* This function will open a relation having name `relName`.
// Since we are currently only working with the relation and attribute catalog, we
// will just hardcode it. In subsequent stages, we will loop through all the relations
// and open the appropriate one.
// */
// int OpenRelTable::getRelId(char relName[ATTR_SIZE]) {

//   /* traverse through the tableMetaInfo array,
//     find the entry in the Open Relation Table corresponding to relName.*/

//   // if found return the relation id, else indicate that the relation do not
//   // have an entry in the Open Relation Table.
//   for(int i=0;i<12;i++){
//     if((strcmp(tableMetaInfo[i].relName,relName)==0)){
//       return i;
//     }
//   }
//   return E_RELNOTOPEN;
// }
// int OpenRelTable::getFreeOpenRelTableEntry() {

//   /* traverse through the tableMetaInfo array,
//     find a free entry in the Open Relation Table.*/

//   // if found return the relation id, else return E_CACHEFULL.
//   for(int i=0;i<12;i++){
//     if(tableMetaInfo[i].free==true){
//       return i;
//     }
//   }
//   return E_CACHEFULL;
// }
// int OpenRelTable::openRel(char relName[ATTR_SIZE]) {

//   int rel=OpenRelTable::getRelId(relName);
//   if(rel >= 0){
//     // (checked using OpenRelTable::getRelId())
//     return rel;
//     // return that relation id;
//   }

//   /* find a free slot in the Open Relation Table
//      using OpenRelTable::getFreeOpenRelTableEntry(). */
//   int free=OpenRelTable::getFreeOpenRelTableEntry();
//   if (/* free slot not available */free==E_CACHEFULL){
//     return E_CACHEFULL;
//   }

//   // let relId be used to store the free slot.
//   int relId=free;

//   /****** Setting up Relation Cache entry for the relation ******/

//   /* search for the entry with relation name, relName, in the Relation Catalog using
//       BlockAccess::linearSearch().
//       Care should be taken to reset the searchIndex of the relation RELCAT_RELID
//       before calling linearSearch().*/
  
//   RelCacheTable::resetSearchIndex(RELCAT_RELID);

//   // relcatRecId stores the rec-id of the relation `relName` in the Relation Catalog.
//   Attribute attrval;
//   strcpy(attrval.sVal,relName);
//   RecId relcatRecId=BlockAccess::linearSearch(0,RELCAT_ATTR_RELNAME,attrval,EQ);

//   if (relcatRecId.block == -1 && relcatRecId.slot==-1) {
//     // (the relation is not found in the Relation Catalog.)
//     return E_RELNOTEXIST;
//   }

//   /* read the record entry corresponding to relcatRecId and create a relCacheEntry
//       on it using RecBuffer::getRecord() and RelCacheTable::recordToRelCatEntry().
//       update the recId field of this Relation Cache entry to relcatRecId.
//       use the Relation Cache entry to set the relId-th entry of the RelCacheTable.
//     NOTE: make sure to allocate memory for the RelCacheEntry using malloc()
//   */
//   RecBuffer recbuffer(relcatRecId.block);
  
//   RelCacheEntry *entry=nullptr;
//   entry=(RelCacheEntry*)malloc(sizeof(RelCacheEntry));
//   Attribute rec[6];

//   recbuffer.getRecord(rec,relcatRecId.slot);

//   RelCacheTable::recordToRelCatEntry(rec,&(entry->relCatEntry));

//   entry->recId.block=relcatRecId.block;
//   entry->recId.slot=relcatRecId.slot;

//   RelCacheTable::relCache[relId]=entry;

//   /****** Setting up Attribute Cache entry for the relation ******/

//   // let listHead be used to hold the head of the linked list of attrCache entries.
//   AttrCacheEntry* head;
//   AttrCacheEntry* temp;

//   int num=entry->relCatEntry.numAttrs;

//   head=temp=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));

//   for(int i=0;i<num;i++){
//     temp->next=(AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//     temp=temp->next;
//   }
//   temp->next=NULL;
//   temp=head;

//   RelCacheTable::resetSearchIndex(1);

//   Attribute attrcatrec[6];
//   /*iterate over all the entries in the Attribute Catalog corresponding to each
//   attribute of the relation relName by multiple calls of BlockAccess::linearSearch()
//   care should be taken to reset the searchIndex of the relation, ATTRCAT_RELID,
//   corresponding to Attribute Catalog before the first call to linearSearch().*/
//   for(int i=0;i<num;i++)
//   {
//       /* let attrcatRecId store a valid record id an entry of the relation, relName,
//       in the Attribute Catalog.*/
//       RecId attrcatRecId=BlockAccess::linearSearch(1,RELCAT_ATTR_RELNAME,attrval,EQ);

//       RecBuffer recbuf(attrcatRecId.block);

//       recbuf.getRecord(attrcatrec,attrcatRecId.slot);

//       AttrCacheTable::recordToAttrCatEntry(attrcatrec,&temp->attrCatEntry);

//       temp->recId.block=attrcatRecId.block;
//       temp->recId.slot=attrcatRecId.slot;

//       temp=temp->next;
//       /* read the record entry corresponding to attrcatRecId and create an
//       Attribute Cache entry on it using RecBuffer::getRecord() and
//       AttrCacheTable::recordToAttrCatEntry().
//       update the recId field of this Attribute Cache entry to attrcatRecId.
//       add the Attribute Cache entry to the linked list of listHead .*/
//       // NOTE: make sure to allocate memory for the AttrCacheEntry using malloc()
//   }
//   AttrCacheTable::attrCache[relId]=head;
//   // set the relIdth entry of the AttrCacheTable to listHead.

//   /****** Setting up metadata in the Open Relation Table for the relation******/

//   // update the relIdth entry of the tableMetaInfo with free as false and
//   // relName as the input.
//   tableMetaInfo[relId].free=false;
//   strcpy(tableMetaInfo[relId].relName,relName);

//   return relId;
// }

// int OpenRelTable::closeRel(int relId) {
//   if (/* rel-id corresponds to relation catalog or attribute catalog*/relId==0 || relId==1) {
//     return E_NOTPERMITTED;
//   }

//   if (relId<0 || relId >=MAX_OPEN) {
//     return E_OUTOFBOUND;
//   }

//   if (/* rel-id corresponds to a free slot*/tableMetaInfo[relId].free==true) {
//     return E_RELNOTOPEN;
//   }
//   if (/* RelCatEntry of the relId-th Relation Cache entry has been modified */RelCacheTable::relCache[relId]->dirty==true)
//   {

//     /* Get the Relation Catalog entry from RelCacheTable::relCache
//     Then convert it to a record using RelCacheTable::relCatEntryToRecord(). */
//     Attribute relcatbuff[RELCAT_NO_ATTRS];
//     RelCacheTable::relCatEntryToRecord(&(RelCacheTable::relCache[relId]->relCatEntry),relcatbuff);

//     // declaring an object of RecBuffer class to write back to the buffer
//     RecId recId=RelCacheTable::relCache[relId]->recId;
//     RecBuffer relCatBlock(recId.block);

//     // Write back to the buffer using relCatBlock.setRecord() with recId.slot
//     relCatBlock.setRecord(relcatbuff,RelCacheTable::relCache[relId]->recId.slot);
//   }

//   // free the memory allocated in the relation and attribute caches which was
//   // allocated in the OpenRelTable::openRel() function
//   free(RelCacheTable::relCache[relId]);
//   AttrCacheEntry *head=AttrCacheTable::attrCache[relId];
//   AttrCacheEntry *next=head->next;

//   while(next){
//     free(head);
//     head=next;
//     next=next->next;

//   }
//   free(head);
//   // update `tableMetaInfo` to set `relId` as a free slot
//   tableMetaInfo[relId].free=true;
//   // update `relCache` and `attrCache` to set the entry at `relId` to nullptr
//   RelCacheTable::relCache[relId]=nullptr;
//   AttrCacheTable::attrCache[relId]=nullptr;

//   /****** Releasing the Relation Cache entry of the relation ******/


//   /****** Releasing the Attribute Cache entry of the relation ******/

//   // free the memory allocated in the attribute caches which was
//   // allocated in the OpenRelTable::openRel() function
//   // (because we are not modifying the attribute cache at this stage,
//   // write-back is not required. We will do it in subsequent
//   // stages when it becomes needed)


//   /****** Set the Open Relation Table entry of the relation as free ******/

//   // update `metainfo` to set `relId` as a free slot

//   return SUCCESS;
// }
// OpenRelTable::~OpenRelTable() {
//     int i;
//     for ( i=2;i<MAX_OPEN;i++);
//     {
//         //if (ith relation is still open)
//         if(OpenRelTable::tableMetaInfo[i].free==false)
//         {
//             // close the relation using openRelTable::closeRel().
//             OpenRelTable::closeRel(i);
//         }
//     }

//     /**** Closing the catalog relations in the relation cache ****/

//     //releasing the relation cache entry of the attribute catalog

//     if (/* RelCatEntry of the ATTRCAT_RELID-th RelCacheEntry has been modified */RelCacheTable::relCache[ATTRCAT_RELID]->dirty==true) {

//         /* Get the Relation Catalog entry from RelCacheTable::relCache
//         Then convert it to a record using RelCacheTable::relCatEntryToRecord(). */
//         RelCatEntry relcatbuff;
//         RelCacheTable::getRelCatEntry(ATTRCAT_RELID,&relcatbuff);
//         Attribute relcatentry[RELCAT_NO_ATTRS];

//         RelCacheTable::relCatEntryToRecord(&relcatbuff,relcatentry);

//         RecId recId=RelCacheTable::relCache[ATTRCAT_RELID]->recId;
//         // declaring an object of RecBuffer class to write back to the buffer
//         RecBuffer relCatBlock(recId.block);

//         // Write back to the buffer using relCatBlock.setRecord() with recId.slot
//         relCatBlock.setRecord(relcatentry,recId.slot);
//     }
//     // free the memory dynamically allocated to this RelCacheEntry
//     free(RelCacheTable::relCache[ATTRCAT_RELID]);


//     //releasing the relation cache entry of the relation catalog

//     if(/* RelCatEntry of the RELCAT_RELID-th RelCacheEntry has been modified */RelCacheTable::relCache[RELCAT_RELID]->dirty==true) {

//         /* Get the Relation Catalog entry from RelCacheTable::relCache
//         Then convert it to a record using RelCacheTable::relCatEntryToRecord(). */
//         RelCatEntry relcatbuff;
//         RelCacheTable::getRelCatEntry(0,&relcatbuff);

//         Attribute relcatentry[6];
//         RelCacheTable::relCatEntryToRecord(&relcatbuff,relcatentry);
//         RecId recId=RelCacheTable::relCache[RELCAT_RELID]->recId;

//         // declaring an object of RecBuffer class to write back to the buffer
//         RecBuffer relCatBlock(recId.block);

//         // Write back to the buffer using relCatBlock.setRecord() with recId.slot
//         relCatBlock.setRecord(relcatentry,recId.slot);
//     }
//     // free the memory dynamically allocated for this RelCacheEntry
//     free(RelCacheTable::relCache[RELCAT_RELID]);

//     // free the memory allocated for the attribute cache entries of the
//     // relation catalog and the attribute catalog
//     for(int i=1;i>=0;i--){
//       AttrCacheEntry* temp=AttrCacheTable::attrCache[i];
//       AttrCacheEntry* next=nullptr;
//       for(int j=0;j<6;j++){
//         next=temp->next;
//         if(temp->dirty==true){
//           AttrCatEntry attrcatbuff;
//           AttrCacheTable::getAttrCatEntry(i,j,&attrcatbuff);

//           Attribute attrcatrec[6];
//           AttrCacheTable::attrCatEntryToRecord(&attrcatbuff,attrcatrec);

//           RecId recId=temp->recId;
//           RecBuffer relCatBlock(recId.block);
//           relCatBlock.setRecord(attrcatrec,recId.slot);

//         }
//         free(temp);
//         temp=next;
//       }
//     }
// }

#include "OpenRelTable.h"

#include <cstring>
#include <cstdlib>

OpenRelTableMetaInfo OpenRelTable::tableMetaInfo[MAX_OPEN];

/*
  * Initializes the meta information of each entry of
    the Open Relation Table to initial empty conditions.
  * It also loads the entries of the Relation Catalog relation
    and Attribute Catalog relation to the Relation Cache Table
    and Attribute Cache Table.
*/
OpenRelTable::OpenRelTable() {

  // initialize relCache and attrCache with nullptr
  for (int i = 0; i < MAX_OPEN; ++i) {
    RelCacheTable::relCache[i] = nullptr;
    AttrCacheTable::attrCache[i] = nullptr;
    tableMetaInfo[i].free = true;
  }

  /************ Setting up Relation Cache entries ************/
  // (we need to populate relation cache with entries for the relation catalog
  //  and attribute catalog.)

  /**** setting up Relation Catalog relation in the Relation Cache Table ****/
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
  relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_ATTRCAT);

  // set the value at RelCacheTable::relCache[ATTRCAT_RELID]
  RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry.relCatEntry);
  relCacheEntry.recId.block = RELCAT_BLOCK;
  relCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_ATTRCAT;

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
  AttrCacheEntry * head , * curr;
  head = curr = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
  int i;
  for (i = 0; i < RELCAT_NO_ATTRS - 1; i++) {
    attrCatBlock.getRecord(attrCatRecord, i);
    AttrCacheTable::recordToAttrCatEntry(attrCatRecord, &curr->attrCatEntry);
    curr->recId.slot = i;
    curr->recId.block = ATTRCAT_BLOCK;
    curr->next = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
    curr = curr->next;
  }
  attrCatBlock.getRecord(attrCatRecord, i);
  AttrCacheTable::recordToAttrCatEntry(attrCatRecord, &curr->attrCatEntry);
  curr->next = nullptr;
  curr->recId.slot = i;
  curr->recId.block = ATTRCAT_BLOCK;
  i++;

  // set the next field in the last entry to nullptr

  AttrCacheTable::attrCache[RELCAT_RELID] = head;

  /**** setting up Attribute Catalog relation in the Attribute Cache Table ****/

  // set up the attributes of the attribute cache similarly.
  // read slots 6-11 from attrCatBlock and initialise recId appropriately
  head = curr = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
  for (; i < RELCAT_NO_ATTRS + ATTRCAT_NO_ATTRS - 1; i++) {
    attrCatBlock.getRecord(attrCatRecord, i);
    AttrCacheTable::recordToAttrCatEntry(attrCatRecord, &curr->attrCatEntry);
    curr->recId.slot = i;
    curr->recId.block = ATTRCAT_BLOCK;
    curr->next = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
    curr = curr->next;
  }
  attrCatBlock.getRecord(attrCatRecord, i);
  AttrCacheTable::recordToAttrCatEntry(attrCatRecord, &curr->attrCatEntry);
  curr->next = nullptr;
  curr->recId.slot = i;
  curr->recId.block = ATTRCAT_BLOCK;
  i++;

  // set the value at AttrCacheTable::attrCache[ATTRCAT_RELID]
  AttrCacheTable::attrCache[ATTRCAT_RELID] = head;

  /************ Setting up tableMetaInfo entries ************/

  // in the tableMetaInfo array
  //   set free = false for RELCAT_RELID and ATTRCAT_RELID
  //   set relname for RELCAT_RELID and ATTRCAT_RELID

  // for relcat relid
  tableMetaInfo[RELCAT_RELID].free = false;
  strcpy(tableMetaInfo[RELCAT_RELID].relName, RELCAT_RELNAME);

  // for attrcat relid
  tableMetaInfo[ATTRCAT_RELID].free = false;
  strcpy(tableMetaInfo[ATTRCAT_RELID].relName, ATTRCAT_RELNAME);
}

/* Returns `index` of an unoccupied entry in the Open Relation Table. */
int OpenRelTable::getFreeOpenRelTableEntry() {

  /* traverse through the tableMetaInfo array,
    find a free entry in the Open Relation Table.*/
  for (int i = ATTRCAT_RELID + 1; i < MAX_OPEN; i++) {
    if (tableMetaInfo[i].free == true)
    {
      return i;
    }

  }

  // if found return the relation id, else return E_CACHEFULL.
  return E_CACHEFULL;
}

/* Returns the relation id, that is, the index, of the entry
corresponding to the input relation in the Open Relation Table. */
int OpenRelTable::getRelId(char relName[ATTR_SIZE]) {

  /* traverse through the tableMetaInfo array,
    find the entry in the Open Relation Table corresponding to relName.*/
  for (int i = 0; i < MAX_OPEN; i++)
  {
    if (!tableMetaInfo[i].free && strcmp(tableMetaInfo[i].relName, relName) == 0) {
      return i;
    }
  }

  // if found return the relation id, else indicate that the relation do not
  // have an entry in the Open Relation Table.
  return E_RELNOTOPEN;
}


int OpenRelTable::openRel(char relName[ATTR_SIZE]) {

  int ret = getRelId(relName);
  if(ret >= 0){
    // (checked using OpenRelTable::getRelId())

    // return that relation id;
    return ret;
  }

  /* find a free slot in the Open Relation Table
     using OpenRelTable::getFreeOpenRelTableEntry(). */
  ret = getFreeOpenRelTableEntry();

  if (ret == E_CACHEFULL){
    return E_CACHEFULL;
  }

  // let relId be used to store the free slot.
  int relId = ret;

  /****** Setting up Relation Cache entry for the relation ******/

  /* search for the entry with relation name, relName, in the Relation Catalog using
      BlockAccess::linearSearch().
      Care should be taken to reset the searchIndex of the relation RELCAT_RELID
      before calling linearSearch().*/

  RelCatEntry relCatEntry;
  RelCacheTable::getRelCatEntry(RELCAT_RELID, &relCatEntry);

  AttrCatEntry attrCatEntry;
  AttrCacheTable::getAttrCatEntry(RELCAT_RELID, RELCAT_REL_NAME_INDEX, &attrCatEntry);
  
  RelCacheTable::resetSearchIndex(RELCAT_RELID);
  Attribute attrVal;
  strcpy(attrVal.sVal, relName);

  // relcatRecId stores the rec-id of the relation `relName` in the Relation Catalog.
  RecId relcatRecId = BlockAccess::linearSearch(RELCAT_RELID, attrCatEntry.attrName, attrVal, EQ);

  /* relcatRecId == {-1, -1} */
  if (relcatRecId.block == -1 && relcatRecId.slot == -1) {
    // (the relation is not found in the Relation Catalog.)
    return E_RELNOTEXIST;
  }

  /* read the record entry corresponding to relcatRecId and create a relCacheEntry
      on it using RecBuffer::getRecord() and RelCacheTable::recordToRelCatEntry().
      update the recId field of this Relation Cache entry to relcatRecId.
      use the Relation Cache entry to set the relId-th entry of the RelCacheTable.
    NOTE: make sure to allocate memory for the RelCacheEntry using malloc()
  */
  RecBuffer recBlock(relcatRecId.block);
  Attribute record[RELCAT_NO_ATTRS];
  recBlock.getRecord(record, relcatRecId.slot);
  struct RelCacheEntry relCacheEntry;
  RelCacheTable::recordToRelCatEntry(record, &relCacheEntry.relCatEntry);
  relCacheEntry.recId = relcatRecId;

  RelCacheTable::relCache[relId] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[relId]) = relCacheEntry;

  /****** Setting up Attribute Cache entry for the relation ******/

  // let listHead be used to hold the head of the linked list of attrCache entries.
  AttrCacheEntry* listHead, *curr;
  listHead = curr = (AttrCacheEntry *)malloc(sizeof(AttrCacheEntry));

  int numAttrs = relCacheEntry.relCatEntry.numAttrs;

  for (int i = 0; i < numAttrs - 1; i++) {
    curr->next = (AttrCacheEntry *)malloc(sizeof(AttrCacheEntry));
    curr = curr->next;
  }
  curr->next = nullptr;

  RelCacheTable::getRelCatEntry(ATTRCAT_RELID, &relCatEntry);
  AttrCacheTable::getAttrCatEntry(ATTRCAT_RELID, ATTRCAT_REL_NAME_INDEX, &attrCatEntry);
  strcpy(attrVal.sVal, relName);
  char attrName[ATTR_SIZE];
  strcpy(attrName, ATTRCAT_ATTR_RELNAME);

  /*iterate over all the entries in the Attribute Catalog corresponding to each
  attribute of the relation relName by multiple calls of BlockAccess::linearSearch()
  care should be taken to reset the searchIndex of the relation, ATTRCAT_RELID,
  corresponding to Attribute Catalog before the first call to linearSearch().*/

  RelCacheTable::resetSearchIndex(ATTRCAT_RELID);
  curr = listHead;

  for (int i = 0; i < numAttrs; i++)
  {
      /* let attrcatRecId store a valid record id an entry of the relation, relName,
      in the Attribute Catalog.*/
      RecId attrcatRecId;
      attrcatRecId = BlockAccess::linearSearch(ATTRCAT_RELID, attrName, attrVal, EQ);

      /* read the record entry corresponding to attrcatRecId and create an
      Attribute Cache entry on it using RecBuffer::getRecord() and
      AttrCacheTable::recordToAttrCatEntry().
      update the recId field of this Attribute Cache entry to attrcatRecId.
      add the Attribute Cache entry to the linked list of listHead .*/
      // NOTE: make sure to allocate memory for the AttrCacheEntry using malloc()
      RecBuffer recBlock(attrcatRecId.block);
      Attribute record[ATTRCAT_NO_ATTRS];
      recBlock.getRecord(record, attrcatRecId.slot);
      AttrCacheTable::recordToAttrCatEntry(record, &curr->attrCatEntry);
      curr->recId = attrcatRecId; 

      curr = curr->next;
  }

  // set the relIdth entry of the AttrCacheTable to listHead.
  AttrCacheTable::attrCache[relId] = listHead;

  /****** Setting up metadata in the Open Relation Table for the relation******/

  // update the relIdth entry of the tableMetaInfo with free as false and
  // relName as the input.
  tableMetaInfo[relId].free = false;
  strcpy(tableMetaInfo[relId].relName, relName);

  return relId;
}

OpenRelTable::~OpenRelTable()
{

  for (int i = 2; i < MAX_OPEN; i++)
  {
    // if ith relation is still open
    if (tableMetaInfo[i].free == false)
    {
      // close the relation using openRelTable::closeRel().
      OpenRelTable::closeRel(i);
    }
  }

  /**** Closing the catalog relations in the relation cache ****/

  // releasing the relation cache entry of the attribute catalog

  //if (/* RelCatEntry of the ATTRCAT_RELID-th RelCacheEntry has been modified */)
  if(RelCacheTable::relCache[ATTRCAT_RELID]->dirty)
  {

    /* Get the Relation Catalog entry from RelCacheTable::relCache
    Then convert it to a record using RelCacheTable::relCatEntryToRecord(). */
    RelCatEntry attrCatRelCacheEntry;
		RelCacheTable::getRelCatEntry(ATTRCAT_RELID, &attrCatRelCacheEntry);
		Attribute record[RELCAT_NO_ATTRS];
		RelCacheTable::relCatEntryToRecord(&attrCatRelCacheEntry, record);
    RecId recId = RelCacheTable::relCache[ATTRCAT_RELID]->recId;
    // declaring an object of RecBuffer class to write back to the buffer
    RecBuffer relCatBlock(recId.block);

    // Write back to the buffer using relCatBlock.setRecord() with recId.slot
    relCatBlock.setRecord(record,recId.slot);
  }
  // free the memory dynamically allocated to this RelCacheEntry
  free(RelCacheTable::relCache[ATTRCAT_RELID]);
  RelCacheTable::relCache[ATTRCAT_RELID] = nullptr ;

  // releasing the relation cache entry of the relation catalog

  //if (/* RelCatEntry of the RELCAT_RELID-th RelCacheEntry has been modified */)
  if(RelCacheTable::relCache[RELCAT_RELID]->dirty)
  {

    /* Get the Relation Catalog entry from RelCacheTable::relCache
    Then convert it to a record using RelCacheTable::relCatEntryToRecord(). */
    RelCatEntry relCatRelCacheEntry;
		RelCacheTable::getRelCatEntry(RELCAT_RELID, &relCatRelCacheEntry);
		Attribute record[RELCAT_NO_ATTRS];
		RelCacheTable::relCatEntryToRecord(&relCatRelCacheEntry, record);
    RecId recId = RelCacheTable::relCache[RELCAT_RELID]->recId;
    // declaring an object of RecBuffer class to write back to the buffer
    RecBuffer relCatBlock(recId.block);

    // Write back to the buffer using relCatBlock.setRecord() with recId.slot
    relCatBlock.setRecord(record, recId.slot);
  }
  // free the memory dynamically allocated for this RelCacheEntry
  free(RelCacheTable::relCache[RELCAT_RELID]);
  RelCacheTable::relCache[RELCAT_RELID] = nullptr ;
  // free the memory allocated for the attribute cache entries of the
  // relation catalog and the attribute catalog
  for (int i = 0; i <= ATTRCAT_RELID; i++) {
		// free attrcache
		AttrCacheEntry* entry = AttrCacheTable::attrCache[i], * temp = nullptr;
		while (entry) {
			temp = entry;
			entry = entry->next;
			free(temp);
		}
	}
}

int OpenRelTable::closeRel(int relId) {
	/* rel-id corresponds to relation catalog or attribute catalog*/
	if (relId == ATTRCAT_RELID || relId == RELCAT_RELID) {
		return E_NOTPERMITTED;
	}

	/* 0 <= relId < MAX_OPEN */
	if (relId >= MAX_OPEN || relId < 0) {
		return E_OUTOFBOUND;
	}

	/* rel-id corresponds to a free slot*/
	if (tableMetaInfo[relId].free) {
		return E_RELNOTOPEN;
	}

	/****** Releasing the Relation Cache entry of the relation ******/

	/* RelCatEntry of the relId-th Relation Cache entry has been modified */
	if (RelCacheTable::relCache[relId]->dirty)
	{

		/* Get the Relation Catalog entry from RelCacheTable::relCache
		Then convert it to a record using RelCacheTable::relCatEntryToRecord(). */
		Attribute record[RELCAT_NO_ATTRS];
		RelCatEntry relCatEntry;
		RelCacheTable::getRelCatEntry(relId, &relCatEntry);
		RelCacheTable::relCatEntryToRecord(&relCatEntry, record);

		RecId recId = RelCacheTable::relCache[relId]->recId;
		// declaring an object of RecBuffer class to write back to the buffer
		RecBuffer relCatBlock(recId.block);

		// Write back to the buffer using relCatBlock.setRecord() with recId.slot
		relCatBlock.setRecord(record, recId.slot);
	}

	/****** Releasing the Attribute Cache entry of the relation ******/

	// free the memory allocated in the attribute caches which was
	// allocated in the OpenRelTable::openRel() function
	free(RelCacheTable::relCache[relId]);
	AttrCacheEntry* entry = AttrCacheTable::attrCache[relId], * temp = nullptr;
	while (entry) {
    //if the entry has been modified:
    if(entry->dirty)
    {
      /* Get the Attribute Catalog entry from attrCache
        Then convert it to a record using AttrCacheTable::attrCatEntryToRecord().
        Write back that entry by instantiating RecBuffer class. Use recId
        member field and recBuffer.setRecord() */
        AttrCatEntry attrcatEntry;
        AttrCacheTable::getAttrCatEntry(relId,entry->attrCatEntry.attrName,&attrcatEntry);
        Attribute record[ATTRCAT_NO_ATTRS];
        AttrCacheTable::attrCatEntryToRecord(&attrcatEntry,record);
        RecId recId = entry->recId;
        RecBuffer attrCatBlock(recId.block);
        attrCatBlock.setRecord(record,recId.slot);
    }
    // if(entry->dirty)
    // {
    //     Attribute record[ATTRCAT_NO_ATTRS];
    //     AttrCacheTable::attrCatEntryToRecord(&entry->attrCatEntry,record);
    //     RecBuffer attrCatBlock(entry->recId.block);
    //     attrCatBlock.setRecord(record,entry->recId.slot);
    // }
		temp = entry;
		entry = entry->next;
		free(temp);
	}
	// (because we are not modifying the attribute cache at this stage,
	// write-back is not required. We will do it in subsequent
	// stages when it becomes needed)

	/****** Set the Open Relation Table entry of the relation as free ******/
	RelCacheTable::relCache[relId] = nullptr;
	AttrCacheTable::attrCache[relId] = nullptr;

	// update `metainfo` to set `relId` as a free slot
	tableMetaInfo[relId].free = true;

	return SUCCESS;
}