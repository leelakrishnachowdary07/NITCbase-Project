#include "BlockAccess.h"

#include <cstring>
RecId BlockAccess::linearSearch(int relId, char attrName[ATTR_SIZE], union Attribute attrVal, int op) {
    // get the previous search index of the relation relId from the relation cache
    // (use RelCacheTable::getSearchIndex() function)
    RecId prevRecId;
    RelCacheTable::getSearchIndex(relId,&prevRecId);

    // let block and slot denote the record id of the record being currently checked
    int block=-1;
    int slot=-1;

    // if the current search index record is invalid(i.e. both block and slot = -1)
    if (prevRecId.block == -1 && prevRecId.slot == -1)
    {
        // (no hits from previous search; search should start from the
        // first record itself)

        // get the first record block of the relation from the relation cache
        // (use RelCacheTable::getRelCatEntry() function of Cache Layer)
        RelCatEntry relcatentry;
        RelCacheTable::getRelCatEntry(relId,&relcatentry);
        // block = first record block of the relation
        // slot = 0
        block=relcatentry.firstBlk;
        slot=0;
    }
    else
    {
        // (there is a hit from previous search; search should start from
        // the record next to the search index record)

        // block = search index's block
        // slot = search index's slot + 1

        block =prevRecId.block;
        slot = prevRecId.slot+ 1;

    }

    /* The following code searches for the next record in the relation
       that satisfies the given condition
       We start from the record id (block, slot) and iterate over the remaining
       records of the relation
    */
    RelCatEntry relCatBuffer;
	RelCacheTable::getRelCatEntry(relId, &relCatBuffer);
    while (block != -1)
    {
        /* create a RecBuffer object for block (use RecBuffer Constructor for existing block) */
        RecBuffer blockbuffer(block);
        // get the record with id (block, slot) using RecBuffer::getRecord()
        Attribute attr[6];
        blockbuffer.getRecord(attr,slot);
        // get header of the block using RecBuffer::getHeader() function
        HeadInfo head;
        blockbuffer.getHeader(&head);
        // get slot map of the block using RecBuffer::getSlotMap() function
        unsigned char slotmap[head.numSlots];
        blockbuffer.getSlotMap(slotmap);

        // If slot >= the number of slots per block(i.e. no more slots in this block)
        if(slot>=relCatBuffer.numSlotsPerBlk)
        {
            // update block = right block of block
            block=head.rblock;
            // update slot = 0
            slot=0;
            continue;  // continue to the beginning of this while loop
        }

        // if slot is free skip the loop
        // (i.e. check if slot'th entry in slot map of block contains SLOT_UNOCCUPIED)
        if(slotmap[slot]==SLOT_UNOCCUPIED)
        {
            // increment slot and continue to the next record slot
            slot++;
            continue;
        }

        // compare record's attribute value to the the given attrVal as below:
        /*
            firstly get the attribute offset for the attrName attribute
            from the attribute cache entry of the relation using
            AttrCacheTable::getAttrCatEntry()
        */
       AttrCacheEntry attrcachebuff;
       AttrCacheTable::getAttrCatEntry(relId,attrName,&attrcachebuff.attrCatEntry);

        /* use the attribute offset to get the value of the attribute from
           current record */

        int cmpVal;  // will store the difference between the attributes
        // set cmpVal using compareAttrs()
        cmpVal=compareAttrs(attr[attrcachebuff.attrCatEntry.offset],attrVal,attrcachebuff.attrCatEntry.attrType);

        /* Next task is to check whether this record satisfies the given condition.
           It is determined based on the output of previous comparison and
           the op value received.
           The following code sets the cond variable if the condition is satisfied.
        */
        if (
            (op == NE && cmpVal != 0) ||    // if op is "not equal to"
            (op == LT && cmpVal < 0) ||     // if op is "less than"
            (op == LE && cmpVal <= 0) ||    // if op is "less than or equal to"
            (op == EQ && cmpVal == 0) ||    // if op is "equal to"
            (op == GT && cmpVal > 0) ||     // if op is "greater than"
            (op == GE && cmpVal >= 0)       // if op is "greater than or equal to"
        ) {
            /*
            set the search index in the relation cache as
            the record id of the record that satisfies the given condition
            (use RelCacheTable::setSearchIndex function)
            */
           RecId searchindex={block,slot};  
           RelCacheTable::setSearchIndex(relId,&searchindex);

            return RecId{block, slot};
        }

        slot++;
    }

    // no record in the relation with Id relid satisfies the given condition
    return RecId{-1, -1};
}
int BlockAccess::renameRelation(char oldName[ATTR_SIZE], char newName[ATTR_SIZE]){
    /* reset the searchIndex of the relation catalog using
       RelCacheTable::resetSearchIndex() */
    RelCacheTable::resetSearchIndex(0);
    Attribute newRelationName;    // set newRelationName with newName
    strcpy(newRelationName.sVal,newName);

    // search the relation catalog for an entry with "RelName" = newRelationName
    RecId search=linearSearch(0,RELCAT_ATTR_RELNAME,newRelationName,EQ);

    // If relation with name newName already exists (result of linearSearch
    //                                               is not {-1, -1})
    //    return E_RELEXIST;
    if(search.block!=-1 && search.slot!=-1){
        return E_RELEXIST;
    }

    /* reset the searchIndex of the relation catalog using
       RelCacheTable::resetSearchIndex() */
    RelCacheTable::resetSearchIndex(0);
    Attribute oldRelationName;    // set oldRelationName with oldName
    strcpy(oldRelationName.sVal,oldName);
    // search the relation catalog for an entry with "RelName" = oldRelationName
    search=linearSearch(0,RELCAT_ATTR_RELNAME,oldRelationName,EQ);

    // If relation with name oldName does not exist (result of linearSearch is {-1, -1})
    //    return E_RELNOTEXIST;
    if(search.block==-1 && search.slot==-1){
        return E_RELNOTEXIST;
    }
    /* get the relation catalog record of the relation to rename using a RecBuffer
       on the relation catalog [RELCAT_BLOCK] and RecBuffer.getRecord function
    */
    RecBuffer recbuff(RELCAT_BLOCK);
    Attribute rec[6];
    recbuff.getRecord(rec,search.slot);
    /* update the relation name attribute in the record with newName.
       (use RELCAT_REL_NAME_INDEX) */
    strcpy(rec[RELCAT_REL_NAME_INDEX].sVal,newName);
    // set back the record value using RecBuffer.setRecord
    recbuff.setRecord(rec,search.slot);

    /*
    update all the attribute catalog entries in the attribute catalog corresponding
    to the relation with relation name oldName to the relation name newName
    */

    /* reset the searchIndex of the attribute catalog using
       RelCacheTable::resetSearchIndex() */
    RelCacheTable::resetSearchIndex(1);
    //for i = 0 to numberOfAttributes :
    //    linearSearch on the attribute catalog for relName = oldRelationName
    //    get the record using RecBuffer.getRecord
    //
    //    update the relName field in the record to newName
    //    set back the record using RecBuffer.setRecord
    for(int i=0;i<rec[RELCAT_NO_ATTRIBUTES_INDEX].nVal;i++){
        search=linearSearch(1,ATTRCAT_ATTR_RELNAME,oldRelationName,EQ);
        RecBuffer attrcat(search.block);
        Attribute attrcatrec[6];
        attrcat.getRecord(attrcatrec,search.slot);

        strcpy(attrcatrec[0].sVal,newName);
        attrcat.setRecord(attrcatrec,search.slot);
    }

    return SUCCESS;
}
int BlockAccess::renameAttribute(char relName[ATTR_SIZE], char oldName[ATTR_SIZE], char newName[ATTR_SIZE]) {

    /* reset the searchIndex of the relation catalog using
       RelCacheTable::resetSearchIndex() */
    RelCacheTable::resetSearchIndex(0);
    Attribute relNameAttr;    // set relNameAttr to relName
    strcpy(relNameAttr.sVal,relName);

    // Search for the relation with name relName in relation catalog using linearSearch()
    RecId search=linearSearch(0,RELCAT_ATTR_RELNAME,relNameAttr,EQ);
    // If relation with name relName does not exist (search returns {-1,-1})
    //    return E_RELNOTEXIST;
    if(search.block==-1 && search.slot==-1){
        E_RELNOTEXIST;
    }
    /* reset the searchIndex of the attribute catalog using
       RelCacheTable::resetSearchIndex() */
    RelCacheTable::resetSearchIndex(1);

    /* declare variable attrToRenameRecId used to store the attr-cat recId
    of the attribute to rename */
    RecId attrToRenameRecId{-1, -1};
    Attribute attrCatEntryRecord[ATTRCAT_NO_ATTRS];

    /* iterate over all Attribute Catalog Entry record corresponding to the
       relation to find the required attribute */
    while (true) {
        // linear search on the attribute catalog for RelName = relNameAttr
        search=linearSearch(1,ATTRCAT_ATTR_RELNAME,relNameAttr,EQ);

        // if there are no more attributes left to check (linearSearch returned {-1,-1})
        //     break;
        if(search.block==-1 && search.slot==-1){
            break;
        }
        /* Get the record from the attribute catalog using RecBuffer.getRecord
          into attrCatEntryRecord */
        RecBuffer recbuff(search.block);
        recbuff.getRecord(attrCatEntryRecord,search.slot);
        // if attrCatEntryRecord.attrName = oldName
        //     attrToRenameRecId = block and slot of this record
        if(!strcmp(attrCatEntryRecord[ATTRCAT_ATTR_NAME_INDEX].sVal,oldName)){
            attrToRenameRecId=search;
        }
        // if attrCatEntryRecord.attrName = newName
        //     return E_ATTREXIST;
        if(!strcmp(attrCatEntryRecord[ATTRCAT_ATTR_NAME_INDEX].sVal,newName)){
            return E_ATTREXIST;
        }
    }

    // if attrToRenameRecId == {-1, -1}
    //     return E_ATTRNOTEXIST;
    if(attrToRenameRecId.block==-1 && attrToRenameRecId.slot==-1){
            return E_ATTRNOTEXIST;
    }

    // Update the entry corresponding to the attribute in the Attribute Catalog Relation.
    /*   declare a RecBuffer for attrToRenameRecId.block and get the record at
         attrToRenameRecId.slot */
    //   update the AttrName of the record with newName
    //   set back the record with RecBuffer.setRecord

    RecBuffer recbuff(attrToRenameRecId.block);
    Attribute attrcatrec[6];
    recbuff.getRecord(attrcatrec,attrToRenameRecId.slot);
    strcpy(attrcatrec[ATTRCAT_ATTR_NAME_INDEX].sVal,newName);
    recbuff.setRecord(attrcatrec,attrToRenameRecId.slot);

    return SUCCESS;
}
int BlockAccess::insert(int relId, Attribute *record) {
    // get the relation catalog entry from relation cache
    // ( use RelCacheTable::getRelCatEntry() of Cache Layer)
    RelCatEntry relcatbuff;
    RelCacheTable::getRelCatEntry(relId,&relcatbuff);

    int blockNum = /* first record block of the relation (from the rel-cat entry)*/relcatbuff.firstBlk;

    // rec_id will be used to store where the new record will be inserted
    RecId rec_id = {-1, -1};

    int numOfSlots = relcatbuff.numSlotsPerBlk/* number of slots per record block */;
    int numOfAttributes = relcatbuff.numAttrs/* number of attributes of the relation */;

    int prevBlockNum = -1/* block number of the last element in the linked list = -1 */;

    /*
        Traversing the linked list of existing record blocks of the relation
        until a free slot is found OR
        until the end of the list is reached
    */
    while (blockNum != -1) {
        // create a RecBuffer object for blockNum (using appropriate constructor!)
        RecBuffer blockbuffer(blockNum);

        // get header of block(blockNum) using RecBuffer::getHeader() function
        HeadInfo blockhead;
        blockbuffer.getHeader(&blockhead);

        // get slot map of block(blockNum) using RecBuffer::getSlotMap() function
        int slots=blockhead.numSlots;
        unsigned char smap[slots];
        blockbuffer.getSlotMap(smap);

        // search for free slot in the block 'blockNum' and store it's rec-id in rec_id
        // (Free slot can be found by iterating over the slot map of the block)
        /* slot map stores SLOT_UNOCCUPIED if slot is free and
           SLOT_OCCUPIED if slot is occupied) */

        /* if a free slot is found, set rec_id and discontinue the traversal
           of the linked list of record blocks (break from the loop) */

        for(int i=0;i<slots;i++){
            if(smap[i]==SLOT_UNOCCUPIED){
                rec_id=RecId{blockNum,i};
                break;
            }
        }
        if (rec_id.block!=-1 && rec_id.slot!=-1) break;


        /* otherwise, continue to check the next block by updating the
           block numbers as follows:
              update prevBlockNum = blockNum
              update blockNum = header.rblock (next element in the linked
                                               list of record blocks)
        */
        prevBlockNum = blockNum;
        blockNum = blockhead.rblock;
    }

    //  if no free slot is found in existing record blocks (rec_id = {-1, -1})
    if (rec_id.block==-1 && rec_id.slot==-1)
    {
        // if relation is RELCAT, do not allocate any more blocks
        //     return E_MAXRELATIONS;
        if(relId==0){
            return E_MAXRELATIONS;
        }
        // Otherwise,
        // get a new record block (using the appropriate RecBuffer constructor!)
        RecBuffer blockbuff;
        // get the block number of the newly allocated block
        // (use BlockBuffer::getBlockNum() function)
        blockNum=blockbuff.getBlockNum();
        // let ret be the return value of getBlockNum() function call
        if (blockNum == E_DISKFULL) {
            return E_DISKFULL;
        }

        // Assign rec_id.block = new block number(i.e. ret) and rec_id.slot = 0
        rec_id.block=blockNum;
        rec_id.slot=0;

        /*
            set the header of the new record block such that it links with
            existing record blocks of the relation
            set the block's header as follows:
            blockType: REC, pblock: -1
            lblock
                  = -1 (if linked list of existing record blocks was empty
                         i.e this is the first insertion into the relation)
                  = prevBlockNum (otherwise),
            rblock: -1, numEntries: 0,
            numSlots: numOfSlots, numAttrs: numOfAttributes
            (use BlockBuffer::setHeader() function)
        */
        HeadInfo blockhead;
        blockhead.blockType=REC;
        blockhead.pblock=-1;
        blockhead.lblock=prevBlockNum;
        blockhead.rblock=-1;
        blockhead.numEntries=0;
        blockhead.numAttrs=numOfAttributes;
        blockhead.numSlots=numOfSlots;

        blockbuff.setHeader(&blockhead);

        /*
            set block's slot map with all slots marked as free
            (i.e. store SLOT_UNOCCUPIED for all the entries)
            (use RecBuffer::setSlotMap() function)
        */
        unsigned char smap[numOfSlots];
        for(int i=0;i<numOfSlots;i++){
            smap[i]=SLOT_UNOCCUPIED;
        }
        blockbuff.setSlotMap(smap);
        // if prevBlockNum != -1
        if(prevBlockNum!=-1)
        {
            // create a RecBuffer object for prevBlockNum
            RecBuffer prevblockbuff(prevBlockNum);
            // get the header of the block prevBlockNum and
            // update the rblock field of the header to the new block
            // number i.e. rec_id.block
            HeadInfo prevblockhead;
            prevblockbuff.getHeader(&prevblockhead);
            prevblockhead.rblock=blockNum;
            // (use BlockBuffer::setHeader() function)
            prevblockbuff.setHeader(&prevblockhead);
        }
        else
        {
            // update first block field in the relation catalog entry to the
            // new block (using RelCacheTable::setRelCatEntry() function)
            relcatbuff.firstBlk=blockNum;
            RelCacheTable::setRelCatEntry(relId,&relcatbuff);
        }

        // update last block field in the relation catalog entry to the
        // new block (using RelCacheTable::setRelCatEntry() function)
        relcatbuff.lastBlk=blockNum;
        RelCacheTable::setRelCatEntry(relId,&relcatbuff);

    }

    // create a RecBuffer object for rec_id.block
    // insert the record into rec_id'th slot using RecBuffer.setRecord())
    RecBuffer blockbuff(rec_id.block);
    blockbuff.setRecord(record,rec_id.slot);

    /* update the slot map of the block by marking entry of the slot to
       which record was inserted as occupied) */
    // (ie store SLOT_OCCUPIED in free_slot'th entry of slot map)
    unsigned char smap[numOfSlots];
    blockbuff.getSlotMap(smap);
    smap[rec_id.slot]=SLOT_OCCUPIED;
    blockbuff.setSlotMap(smap);
    // (use RecBuffer::getSlotMap() and RecBuffer::setSlotMap() functions)

    // increment the numEntries field in the header of the block to
    // which record was inserted
    // (use BlockBuffer::getHeader() and BlockBuffer::setHeader() functions)
    HeadInfo blockhead;
    blockbuff.getHeader(&blockhead);
    blockhead.numEntries++;
    blockbuff.setHeader(&blockhead);

    // Increment the number of records field in the relation cache entry for
    // the relation. (use RelCacheTable::setRelCatEntry function)
    relcatbuff.numRecs++;
    RelCacheTable::setRelCatEntry(relId,&relcatbuff);

    return SUCCESS;
}
/*
NOTE: This function will copy the result of the search to the `record` argument.
      The caller should ensure that space is allocated for `record` array
      based on the number of attributes in the relation.
*/
// int BlockAccess::search(int relId, Attribute *record, char attrName[ATTR_SIZE], Attribute attrVal, int op) {
//     // Declare a variable called recid to store the searched record

//     /* search for the record id (recid) corresponding to the attribute with
//     attribute name attrName, with value attrval and satisfying the condition op
//     using linearSearch() */
//     RecId recid=BlockAccess::linearSearch(relId,attrName,attrVal,op);
//     // if there's no record satisfying the given condition (recId = {-1, -1})
//     //    return E_NOTFOUND;
//     if(recid.block==-1 && recid.slot==-1){
//         return E_NOTFOUND;
//     }
//     /* Copy the record with record id (recId) to the record buffer (record)
//        For this Instantiate a RecBuffer class object using recId and
//        call the appropriate method to fetch the record
//     */
//     RecBuffer blockbuff(recid.block);
//     blockbuff.getRecord(record,recid.slot);
//     return SUCCESS;
// }
int BlockAccess::search(int relId, Attribute *record, char attrName[ATTR_SIZE], Attribute attrVal, int op) {
    // Declare a variable called recid to store the searched record
    RecId recId;

    /* get the attribute catalog entry from the attribute cache corresponding
    to the relation with Id=relid and with attribute_name=attrName  */
    // if this call returns an error, return the appropriate error code
    // get rootBlock from the attribute catalog entry
    AttrCatEntry buff;
    int ret=AttrCacheTable::getAttrCatEntry(relId,attrName,&buff);
    if(ret!=SUCCESS){
        return ret;
    }
    int rootBlock=buff.rootBlock;
    /* if Index does not exist for the attribute (check rootBlock == -1) */ 
    if(rootBlock==-1){
        /* search for the record id (recid) corresponding to the attribute with
           attribute name attrName, with value attrval and satisfying the
           condition op using linearSearch()
        */
       recId=linearSearch(relId,attrName,attrVal,op);
    }

    else {
        // (index exists for the attribute)

        /* search for the record id (recid) correspoding to the attribute with
        attribute name attrName and with value attrval and satisfying the
        condition op using BPlusTree::bPlusSearch() */
        recId=BPlusTree::bPlusSearch(relId,attrName,attrVal,op);
    }


    // if there's no record satisfying the given condition (recId = {-1, -1})
    //     return E_NOTFOUND;
    if(recId.block==-1 && recId.slot==-1){
        return E_NOTFOUND;
    }
    /* Copy the record with record id (recId) to the record buffer (record).
       For this, instantiate a RecBuffer class object by passing the recId and
       call the appropriate method to fetch the record
    */
    RecBuffer blockbuff(recId.block);
    blockbuff.getRecord(record,recId.slot);
    return SUCCESS;
}
int BlockAccess::deleteRelation(char relName[ATTR_SIZE]) {
    // if the relation to delete is either Relation Catalog or Attribute Catalog,
    //     return E_NOTPERMITTED
        // (check if the relation names are either "RELATIONCAT" and "ATTRIBUTECAT".
        // you may use the following constants: RELCAT_NAME and ATTRCAT_NAME)
    if(!strcmp(relName,"RELATIONCAT") || !strcmp(relName,"ATTRIBUTECAT")){
        return E_NOTPERMITTED;
    }
    /* reset the searchIndex of the relation catalog using
       RelCacheTable::resetSearchIndex() */
    RelCacheTable::resetSearchIndex(0);
    Attribute relNameAttr; // (stores relName as type union Attribute)
    // assign relNameAttr.sVal = relName
    strcpy((char*)relNameAttr.sVal,relName);
    //  linearSearch on the relation catalog for RelName = relNameAttr
    RecId recid=BlockAccess::linearSearch(0,"RelName",relNameAttr,EQ);
    // if the relation does not exist (linearSearch returned {-1, -1})
    //     return E_RELNOTEXIST
    if(recid.block==-1 && recid.slot==-1){
        return E_RELNOTEXIST;
    }
    Attribute relCatEntryRecord[RELCAT_NO_ATTRS];
    /* store the relation catalog record corresponding to the relation in
       relCatEntryRecord using RecBuffer.getRecord */
    RecBuffer relcatbuff(recid.block);
    relcatbuff.getRecord(relCatEntryRecord,recid.slot);
    /* get the first record block of the relation (firstBlock) using the
       relation catalog entry record */
       int firstblk= (int)relCatEntryRecord[RELCAT_FIRST_BLOCK_INDEX].nVal;
    /* get the number of attributes corresponding to the relation (numAttrs)
       using the relation catalog entry record */
        int numattrs=(int)relCatEntryRecord[RELCAT_NO_ATTRIBUTES_INDEX].nVal;
    /*
     Delete all the record blocks of the relation
    */
    // for each record block of the relation:
    //     get block header using BlockBuffer.getHeader
    //     get the next block from the header (rblock)
    //     release the block using BlockBuffer.releaseBlock
    //
    //     Hint: to know if we reached the end, check if nextBlock = -1
    int temp=firstblk;
    while(temp!=-1){
        RecBuffer relcattemp(temp);

        HeadInfo temphead;
        relcattemp.getHeader(&temphead);

        temp=temphead.rblock;

        relcattemp.releaseBlock();
    }

    /***
        Deleting attribute catalog entries corresponding the relation and index
        blocks corresponding to the relation with relName on its attributes
    ***/

    // reset the searchIndex of the attribute catalog
    RelCacheTable::resetSearchIndex(1);

    int numberOfAttributesDeleted = 0;

    while(true) {
        RecId attrCatRecId=linearSearch(1,"RelName",relNameAttr,EQ);

        // attrCatRecId = linearSearch on attribute catalog for RelName = relNameAttr

        // if no more attributes to iterate over (attrCatRecId == {-1, -1})
        //     break;
        if(attrCatRecId.block==-1 && attrCatRecId.slot==-1){
            break;
        }
        numberOfAttributesDeleted++;

        // create a RecBuffer for attrCatRecId.block
        RecBuffer recbuff(attrCatRecId.block);
        // get the header of the block
        HeadInfo rechead;
        recbuff.getHeader(&rechead);
        // get the record corresponding to attrCatRecId.slot
        Attribute record[6];
        recbuff.getRecord(record,attrCatRecId.slot);

        // declare variable rootBlock which will be used to store the root
        // block field from the attribute catalog record.
        int rootBlock = /* get root block from the record */record[ATTRCAT_ROOT_BLOCK_INDEX].nVal;
        // (This will be used later to delete any indexes if it exists)

        // Update the Slotmap for the block by setting the slot as SLOT_UNOCCUPIED
        // Hint: use RecBuffer.getSlotMap and RecBuffer.setSlotMap
        unsigned char smap[rechead.numSlots];
        recbuff.getSlotMap(smap);
        smap[attrCatRecId.slot]=SLOT_UNOCCUPIED;
        recbuff.setSlotMap(smap);

        /* Decrement the numEntries in the header of the block corresponding to
           the attribute catalog entry and then set back the header
           using RecBuffer.setHeader */
        rechead.numEntries--;
        recbuff.setHeader(&rechead);
        /* If number of entries become 0, releaseBlock is called after fixing
           the linked list.
        */
       /*   header.numEntries == 0  */
        if (rechead.numEntries==0) {
            /* Standard Linked List Delete for a Block
               Get the header of the left block and set it's rblock to this
               block's rblock
            */
            // create a RecBuffer for lblock and call appropriate methods
            RecBuffer lblockbuff(rechead.lblock);

            HeadInfo lhead;
            lblockbuff.getHeader(&lhead);

            lhead.rblock=rechead.rblock;
            lblockbuff.setHeader(&lhead);
            /* header.rblock != -1 */
            if (rechead.rblock!= INVALID_BLOCKNUM) {
                /* Get the header of the right block and set it's lblock to
                   this block's lblock */
                RecBuffer rblockbuff(rechead.rblock);

                HeadInfo rhead;
                rblockbuff.getHeader(&rhead);

                rhead.lblock=rechead.lblock;
                rblockbuff.setHeader(&rhead);
                // create a RecBuffer for rblock and call appropriate methods

            } else {
                // (the block being released is the "Last Block" of the relation.)
                /* update the Relation Catalog entry's LastBlock field for this
                   relation with the block number of the previous block. */
                   RelCatEntry relcatentrybuff;
                   RelCacheTable::getRelCatEntry(1,&relcatentrybuff);

                   relcatentrybuff.lastBlk=rechead.lblock;
            }

            // (Since the attribute catalog will never be empty(why?), we do not
            //  need to handle the case of the linked list becoming empty - i.e
            //  every block of the attribute catalog gets released.)

            // call releaseBlock()
            recbuff.releaseBlock();
        }

        // (the following part is only relevant once indexing has been implemented)
        // if index exists for the attribute (rootBlock != -1), call bplus destroy
        //if (rootBlock != -1) {
            // delete the bplus tree rooted at rootBlock using BPlusTree::bPlusDestroy()
        //}
    }

    /*** Delete the entry corresponding to the relation from relation catalog ***/
    // Fetch the header of Relcat block
    HeadInfo relcathead;
    relcatbuff.getHeader(&relcathead);
    /* Decrement the numEntries in the header of the block corresponding to the
       relation catalog entry and set it back */
    relcathead.numEntries--;
    relcatbuff.setHeader(&relcathead);
    /* Get the slotmap in relation catalog, update it by marking the slot as
       free(SLOT_UNOCCUPIED) and set it back. */
    unsigned char smap[relcathead.numSlots];
    relcatbuff.getSlotMap(smap);

    smap[recid.slot]=SLOT_UNOCCUPIED;
    relcatbuff.setSlotMap(smap);
    /*** Updating the Relation Cache Table ***/
    /** Update relation catalog record entry (number of records in relation
        catalog is decreased by 1) **/
    RelCatEntry relcatentrybuff;
    RelCacheTable::getRelCatEntry(0,&relcatentrybuff);
    relcatentrybuff.numRecs--;
    RelCacheTable::setRelCatEntry(0,&relcatentrybuff);
    // Get the entry corresponding to relation catalog from the relation
    // cache and update the number of records and set it back
    // (using RelCacheTable::setRelCatEntry() function)

    /** Update attribute catalog entry (number of records in attribute catalog
        is decreased by numberOfAttributesDeleted) **/
    // i.e., #Records = #Records - numberOfAttributesDeleted

    // Get the entry corresponding to attribute catalog from the relation
    // cache and update the number of records and set it back
    // (using RelCacheTable::setRelCatEntry() function)
    RelCacheTable::getRelCatEntry(1,&relcatentrybuff);
    relcatentrybuff.numRecs-=numberOfAttributesDeleted;
    RelCacheTable::setRelCatEntry(1,&relcatentrybuff);

    return SUCCESS;
}
/*
NOTE: the caller is expected to allocate space for the argument `record` based
      on the size of the relation. This function will only copy the result of
      the projection onto the array pointed to by the argument.
*/
int BlockAccess::project(int relId, Attribute *record) {
    // get the previous search index of the relation relId from the relation
    // cache (use RelCacheTable::getSearchIndex() function)
    RecId prevRecId;
    RelCacheTable::getSearchIndex(relId,&prevRecId);

    // declare block and slot which will be used to store the record id of the
    // slot we need to check.
    int block, slot;

    /* if the current search index record is invalid(i.e. = {-1, -1})
       (this only happens when the caller reset the search index)
    */
    if (prevRecId.block == -1 && prevRecId.slot == -1)
    {
        // (new project operation. start from beginning)

        // get the first record block of the relation from the relation cache
        // (use RelCacheTable::getRelCatEntry() function of Cache Layer)
        RelCatEntry blkbuff;
        RelCacheTable::getRelCatEntry(relId,&blkbuff);

        block=blkbuff.firstBlk;
        slot=0;

        // block = first record block of the relation
        // slot = 0
    }
    else
    {
        // (a project/search operation is already in progress)

        // block = previous search index's block
        // slot = previous search index's slot + 1
        block=prevRecId.block;
        slot=prevRecId.slot+1;
    }


    // The following code finds the next record of the relation
    /* Start from the record id (block, slot) and iterate over the remaining
       records of the relation */
    while (block != -1)
    {
        // create a RecBuffer object for block (using appropriate constructor!)
        RecBuffer recbuff(block);

        // get header of the block using RecBuffer::getHeader() function
        // get slot map of the block using RecBuffer::getSlotMap() function
        HeadInfo rechead;
        recbuff.getHeader(&rechead);
        unsigned char smap[rechead.numSlots];
        recbuff.getSlotMap(smap);

        if(/* slot >= the number of slots per block*/slot>=rechead.numSlots)
        {
            // (no more slots in this block)
            // update block = right block of block
            block=rechead.rblock;
            // update slot = 0
            slot=0;
            // (NOTE: if this is the last block, rblock would be -1. this would
            //        set block = -1 and fail the loop condition )
        }
        else if (/* slot is free */ smap[slot]==SLOT_UNOCCUPIED)
        { // (i.e slot-th entry in slotMap contains SLOT_UNOCCUPIED)

            // increment slot
            slot++;
        }
        else {
            // (the next occupied slot / record has been found)
            break;
        }
    }

    if (block == -1){
        // (a record was not found. all records exhausted)
        return E_NOTFOUND;
    }

    // declare nextRecId to store the RecId of the record found
    RecId nextRecId{block, slot};

    // set the search index to nextRecId using RelCacheTable::setSearchIndex
    RelCacheTable::setSearchIndex(relId,&nextRecId);
    /* Copy the record with record id (nextRecId) to the record buffer (record)
       For this Instantiate a RecBuffer class object by passing the recId and
       call the appropriate method to fetch the record
    */
   RecBuffer recbuff(block);
   recbuff.getRecord(record,slot);

    return SUCCESS;
}