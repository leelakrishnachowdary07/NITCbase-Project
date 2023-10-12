#include "BlockAccess.h"

#include <cstring>
int BlockAccess::deleteRelation(char relName[ATTR_SIZE]) {
    // if the relation to delete is either Relation Catalog or Attribute Catalog
	// (check if the relation names are either "RELATIONCAT" and "ATTRIBUTECAT".
	// you may use the following constants: RELCAT_NAME and ATTRCAT_NAME)
	if (strcmp(relName, RELCAT_RELNAME) == 0 || strcmp(relName, ATTRCAT_RELNAME) == 0)
        return E_NOTPERMITTED;

    /* reset the searchIndex of the relation catalog using
       RelCacheTable::resetSearchIndex() */
	RelCacheTable::resetSearchIndex(RELCAT_RELID);

    // assign relNameAttr.sVal = relName
    Attribute relNameAttr; // (stores relName as type union Attribute)
	strcpy((char*)relNameAttr.sVal,(const char*)relName);

    //  linearSearch on the relation catalog for RelName = relNameAttr
	RecId relCatRecId = BlockAccess::linearSearch(0, RELCAT_ATTR_RELNAME, relNameAttr ,EQ);

    // if the relation does not exist (linearSearch returned {-1, -1})
	if (relCatRecId.block==-1 && relCatRecId.slot==-1) 
	return E_RELNOTEXIST;

    // TODO: store the relation catalog record corresponding to the relation in
    // TODO: relCatEntryRecord using RecBuffer.getRecord

	RecBuffer relCatBlockBuffer (relCatRecId.block);

	// relCatEntryRecord : relation `relName` record in relation-catalog
    Attribute relCatEntryRecord[RELCAT_NO_ATTRS];
	relCatBlockBuffer.getRecord(relCatEntryRecord, relCatRecId.slot);

	// TODO: get the first record block & number of attributes of the relation 
	// TODO: (firstBlock) & (numAttrs) using the relation catalog entry record 
	
	int firstBlock = relCatEntryRecord[RELCAT_FIRST_BLOCK_INDEX].nVal;
	int numAttributes = relCatEntryRecord[RELCAT_NO_ATTRIBUTES_INDEX].nVal;

    // TODO: Delete all the record blocks of the relation
	int currentBlockNum = firstBlock;
    
	// for each record block of the relation:
	// Hint: to know if we reached the end, check if nextBlock = -1
	while (currentBlockNum != -1) {
		RecBuffer currentBlockBuffer (currentBlockNum);

    	// get block header using BlockBuffer.getHeader
		HeadInfo currentBlockHeader;
		currentBlockBuffer.getHeader(&currentBlockHeader);

		// get the next block from the header (rblock)
		currentBlockNum = currentBlockHeader.rblock;

		// release the block using BlockBuffer.releaseBlock
		currentBlockBuffer.releaseBlock();
	}

    /***
        Deleting attribute catalog entries corresponding the relation and index
        blocks corresponding to the relation with relName on its attributes
    ***/

    // reset the searchIndex of the attribute catalog
	RelCacheTable::resetSearchIndex(1);

    int numberOfAttributesDeleted = 0;

    while(true) {
        // attrCatRecId : `relname`'s entry in `ATTRCAT`
        RecId attrCatRecId = BlockAccess::linearSearch(1, RELCAT_ATTR_RELNAME, relNameAttr, EQ);

        // if no more attributes to iterate over (attrCatRecId == {-1, -1})
		if (attrCatRecId.block==-1 && attrCatRecId.slot==-1) break;

        numberOfAttributesDeleted++;

        // create a RecBuffer for attrCatRecId.block
        // get the header of the block
        // get the record corresponding to attrCatRecId.slot
		RecBuffer attrCatBlockBuffer (attrCatRecId.block);

		HeadInfo attrCatHeader;
		attrCatBlockBuffer.getHeader(&attrCatHeader);

		Attribute attrCatRecord [ATTRCAT_NO_ATTRS];
		attrCatBlockBuffer.getRecord(attrCatRecord, attrCatRecId.slot);

        // declare variable rootBlock which will be used to store the root
        // block field from the attribute catalog record.
        int rootBlock = attrCatRecord[ATTRCAT_ROOT_BLOCK_INDEX].nVal; // get root block from the record
        // (This will be used later to delete any indexes if it exists)
		
        // Update the Slotmap for the block by setting the slot as SLOT_UNOCCUPIED
        // Hint: use RecBuffer.getSlotMap and RecBuffer.setSlotMap
		unsigned char slotmap [attrCatHeader.numSlots];
		attrCatBlockBuffer.getSlotMap(slotmap);

		slotmap[attrCatRecId.slot] = SLOT_UNOCCUPIED;
		attrCatBlockBuffer.setSlotMap(slotmap);

        /* Decrement the numEntries in the header of the block corresponding to
           the attribute catalog entry and then set back the header
           using RecBuffer.setHeader */
		attrCatHeader.numEntries--;
		attrCatBlockBuffer.setHeader(&attrCatHeader);

        /* If number of entries become 0, releaseBlock is called after fixing
           the linked list.
        */
        if (attrCatHeader.numEntries == 0) {
            /* Standard DOUBLY Linked List Delete for a Block
               Get the header of the left block and set it's rblock to this
               block's rblock
            */

            // create a RecBuffer for lblock and call appropriate methods
			RecBuffer prevBlock (attrCatHeader.lblock);
			
			HeadInfo leftHeader;
			prevBlock.getHeader(&leftHeader);

			leftHeader.rblock = attrCatHeader.rblock;
			prevBlock.setHeader(&leftHeader);


            if (attrCatHeader.rblock != INVALID_BLOCKNUM) 
			{
                /* Get the header of the right block and set it's lblock to
                   this block's lblock */
                // create a RecBuffer for rblock and call appropriate methods
				RecBuffer nextBlock (attrCatHeader.rblock);
				
				HeadInfo rightHeader;
				nextBlock.getHeader(&rightHeader);

				rightHeader.lblock = attrCatHeader.lblock;
				nextBlock.setHeader(&rightHeader);

            } 
			else 
			{
                // (the block being released is the "Last Block" of the relation.)
                /* update the Relation Catalog entry's LastBlock field for this
                   relation with the block number of the previous block. */

				RelCatEntry relCatEntryBuffer;
				RelCacheTable::getRelCatEntry(1, &relCatEntryBuffer);

				relCatEntryBuffer.lastBlk = attrCatHeader.lblock;
            }

            // (Since the attribute catalog will never be empty(why?), we do not
            //  need to handle the case of the linked list becoming empty - i.e
            //  every block of the attribute catalog gets released.)

            // call releaseBlock()
			attrCatBlockBuffer.releaseBlock();
        }

		/*
        // (the following part is only relevant once indexing has been implemented)
        // if index exists for the attribute (rootBlock != -1), call bplus destroy
        if (rootBlock != -1) {
            // delete the bplus tree rooted at rootBlock using BPlusTree::bPlusDestroy()
        }
		*/
    }

    /*** Delete the entry corresponding to the relation from relation catalog ***/
    // Fetch the header of Relcat block
	// // relCatBlockBuffer = RecBuffer (RELCAT_BLOCK);

	HeadInfo relCatHeader;
	relCatBlockBuffer.getHeader(&relCatHeader);

    // TODO: Decrement the numEntries in the header of the block corresponding to the
    // TODO: relation catalog entry and set it back
	relCatHeader.numEntries--;
	relCatBlockBuffer.setHeader(&relCatHeader);

    /* Get the slotmap in relation catalog, update it by marking the slot as
       free(SLOT_UNOCCUPIED) and set it back. */
	unsigned char slotmap [relCatHeader.numSlots];
	relCatBlockBuffer.getSlotMap(slotmap);

	slotmap[relCatRecId.slot] = SLOT_UNOCCUPIED;
	relCatBlockBuffer.setSlotMap(slotmap);

    /*** Updating the Relation Cache Table ***/
    /** Update relation catalog record entry (number of records in relation
        catalog is decreased by 1) **/

	// Get the entry corresponding to relation catalog from the relation
    // cache and update the number of records and set it back
    // (using RelCacheTable::setRelCatEntry() function)

	RelCatEntry relCatEntryBuffer;
	RelCacheTable::getRelCatEntry(RELCAT_RELID, &relCatEntryBuffer);

	relCatEntryBuffer.numRecs--;
	RelCacheTable::setRelCatEntry(RELCAT_RELID, &relCatEntryBuffer);

    /** Update attribute catalog entry (number of records in attribute catalog
        is decreased by numberOfAttributesDeleted) **/
    // i.e., #Records = #Records - numberOfAttributesDeleted

    // Get the entry corresponding to attribute catalog from the relation
    // cache and update the number of records and set it back
    // (using RelCacheTable::setRelCatEntry() function)

	RelCacheTable::getRelCatEntry(ATTRCAT_RELID, &relCatEntryBuffer);
	relCatEntryBuffer.numRecs -= numberOfAttributesDeleted;
	RelCacheTable::setRelCatEntry(ATTRCAT_RELID, &relCatEntryBuffer);

    return SUCCESS;
}