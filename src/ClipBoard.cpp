/*
 *  ClipBoard.cpp
 *  ofxLuaLive
 *
 *  Created by Makira on 11/07/09.
 *  Copyright 2011 ・‥…―━━━―…‥・yesMAYBEno・‥…―━━━―…‥・. All rights reserved.
 *
 */

#include "ClipBoard.h"

char* ClipBoard::getTextFromPasteboard() {  
	
    OSStatus             err = noErr;  
    ItemCount            itemCount;  
    PasteboardSyncFlags  syncFlags;  
    static PasteboardRef inPasteboard = NULL;  
    PasteboardCreate( kPasteboardClipboard, &inPasteboard );  
    char* data;  
    data = NULL;  
	
    syncFlags = PasteboardSynchronize( inPasteboard );  
    err = badPasteboardSyncErr;  
	
    err = PasteboardGetItemCount( inPasteboard, &itemCount );  
    require_noerr( err, CantGetPasteboardItemCount );  
	
    for( int itemIndex = 1; itemIndex <= itemCount; itemIndex++ ) {  
        PasteboardItemID  itemID;  
        CFDataRef  flavorData;  
		
        err = PasteboardGetItemIdentifier( inPasteboard, itemIndex, &itemID );  
        require_noerr( err, CantGetPasteboardItemIdentifier );  
		
        err = PasteboardCopyItemFlavorData( inPasteboard, itemID, CFSTR("public.utf8-plain-text"), &flavorData );         
        data = (char*)CFDataGetBytePtr(flavorData);  
		
	CantGetPasteboardItemIdentifier:  
        ;  
    }  
	
CantGetPasteboardItemCount:  
	
    return data;  
}  


OSStatus ClipBoard::setTextToPasteboard(const char* byteArrayIndex) {  
	
    OSStatus                err = noErr;  
    static PasteboardRef    pasteboard = NULL;  
    PasteboardCreate( kPasteboardClipboard, &pasteboard );  
	
    err = PasteboardClear( pasteboard );  
    require_noerr( err, PasteboardClear_FAILED );  
	
    CFDataRef  data;  
	
    data = CFDataCreate(kCFAllocatorDefault, (UInt8*) byteArrayIndex, strlen(byteArrayIndex)+1);  
    err = PasteboardPutItemFlavor( pasteboard, (PasteboardItemID)1, kUTTypeUTF8PlainText, data, 0);   
    require_noerr( err, PasteboardPutItemFlavor_FAILED );  
	
PasteboardPutItemFlavor_FAILED:   
PasteboardClear_FAILED:  
    return err;  
}  