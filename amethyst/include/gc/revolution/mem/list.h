/*---------------------------------------------------------------------------*
  Project:     MEM library
  File:        list.h
  Programmers: Takano Makoto

  Copyright 2005 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
 *---------------------------------------------------------------------------*/

#ifndef MEM_LIST_H__
#define MEM_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>
#include <revolution/types.h>


/*---------------------------------------------------------------------------*
  Name:         MEMLink

  Description:  Node structure for two-way link list. This structure is stored in the list structure as a member of the structure to be linked.
                
 *---------------------------------------------------------------------------*/
typedef struct
{
    void*       prevObject;     // Pointer to the previous linked object
    void*       nextObject;     // Pointer to the next linked object

} MEMLink;


/*---------------------------------------------------------------------------*
  Name:         MEMList

  Description:  Two-way link list structure
 *---------------------------------------------------------------------------*/
typedef struct 
{
    void*       headObject;     // Pointer for the object linked to the head
    void*       tailObject;     // Pointer for the object linked to the end
    u16         numObjects;     // Number of objects linked to the list
    u16         offset;         // Offset for NNSFndLink type structure member

} MEMList;


/*---------------------------------------------------------------------------*
  Name:         MEM_INIT_LIST

  Description:  Macro to initialize list structure. Actual initialization is performed by the MEMInitList() function.
                

                This macro finds the offset using the offsetof macro based on the name of the specified structure and the link type member variable name and then passes that offset to the InitList function.
                
                

  Arguments:    list:       Pointer to the link structure.
                structName: Structure name of object you wish to link to the list.
                linkName:   The Link type member variable name used to link this object.
                            

  Returns:      None.
 *---------------------------------------------------------------------------*/

#define MEM_INIT_LIST(list, structName, linkName) \
           MEMInitList(list, offsetof(structName, linkName))


/*---------------------------------------------------------------------------*
    Function Prototypes

 *---------------------------------------------------------------------------*/

void    MEMInitList(
                MEMList*              list,
                u16                   offset);

void    MEMAppendListObject(
                MEMList*              list,
                void*                 object);

void    MEMPrependListObject(
                MEMList*              list,
                void*                 object);

void    MEMInsertListObject(
                MEMList*              list,
                void*                 target,
                void*                 object);

void    MEMRemoveListObject(
                MEMList*              list,
                void*                 object);

void*   MEMGetNextListObject(
                MEMList*              list,
                void*                 object);

void*   MEMGetPrevListObject(
                MEMList*              list,
                void*                 object);

void*   MEMGetNthListObject(
                MEMList*              list,
                u16                   index);


#ifdef __cplusplus
} /* extern "C" */
#endif

/* MEM_LIST_H__ */
#endif
