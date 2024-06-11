#include "CallToOtherLanguage.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC

void CallToOtherLanguage::copyToClipboard(const std::string& str)
{
    NSData  * data   = [NSData dataWithBytes:str.c_str() length:strlen(str.c_str())];
    NSString* string = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];

    [[NSPasteboard generalPasteboard] clearContents];
    [[NSPasteboard generalPasteboard] setString:string forType:NSPasteboardTypeString];

    showAlert("Copied to Clipboard!");
}

void CallToOtherLanguage::showAlert(const char* message)
{
    NSData  * data   = [NSData dataWithBytes:message length:strlen(message)];
    NSString* string = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];

    NSAlert* alert = [[NSAlert alloc] init];
    [alert setMessageText:string];
    [alert runModal];
}

bool CallToOtherLanguage::hasSafeArea()
{
    return false;
}

#endif
