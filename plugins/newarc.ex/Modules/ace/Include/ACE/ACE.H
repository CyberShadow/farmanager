#ifndef _ACEDLL_H
#define _ACEDLL_H

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

//様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�
//�===--                                                               --===様�
//様=====----    ACE-Archiver: dynamic link library for Win32    ----======様様
//様様様�====____                                            ____====様様様様様
//様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�
//
//      The use of this Dll within another program than the ACE-Archiver
//      itself has to be agreet upon contract. Any use of this Dll without
//      such a contract or any violation of such a contract is seen as an
//      attack against the copyright on the archiver ACE. This copyright is
//      holded by ACE Compression Software, the create of the ACE archiver.
//陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�


//様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�
//様様様様様様�=====---  Part 1: different structures ---====様様様様様様様様様
//様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�

#include "includes/strucs.h"

//様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�
//様様様様様様様様様=====---  Part 2: callbacks ---====様様様様様様様様様様様様
//様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�

#include "includes/callback.h"

//様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�
//様様様様=====---  Part 3: Functions supported by UnAceV2.Dll ---====様様様様�
//様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�

#include "includes/unacefnc.h"

//様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�
//様様様�=====---  Part 4: Functions supported by the AceV2.Dll ---====様様様様
//様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�

#include "includes/acefnc.h"



#ifdef __cplusplus
 }
#endif /* __cplusplus */

#endif /* _ACEDLL_H */
