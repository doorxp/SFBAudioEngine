/*
 *  Copyright (C) 2012, 2013 Stephen F. Booth <me@sbooth.org>
 *  All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    - Neither the name of Stephen F. Booth nor the names of its 
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CFErrorUtilities.h"
#include "CreateDisplayNameForURL.h"

CFErrorRef CreateError(CFStringRef domain, CFIndex code, CFStringRef description, CFStringRef failureReason, CFStringRef recoverySuggestion)
{
	if(nullptr == domain)
		return nullptr;
	
	CFMutableDictionaryRef errorDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	if(nullptr == errorDictionary)
		return nullptr;
	
	if(description)
		CFDictionarySetValue(errorDictionary, kCFErrorLocalizedDescriptionKey, description);
	
	if(failureReason)
		CFDictionarySetValue(errorDictionary, kCFErrorLocalizedFailureReasonKey, failureReason);
	
	if(recoverySuggestion)
		CFDictionarySetValue(errorDictionary, kCFErrorLocalizedRecoverySuggestionKey, recoverySuggestion);
	
	CFErrorRef error = CFErrorCreate(kCFAllocatorDefault, domain, code, errorDictionary);
	
	CFRelease(errorDictionary), errorDictionary = nullptr;
	
	return error;
}

CFErrorRef CreateErrorForURL(CFStringRef domain, CFIndex code, CFStringRef descriptionFormatStringForURL, CFURLRef url, CFStringRef failureReason, CFStringRef recoverySuggestion)
{
	if(nullptr == domain)
		return nullptr;
	
	CFMutableDictionaryRef errorDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	if(nullptr == errorDictionary)
		return nullptr;

	if(descriptionFormatStringForURL && url) {
		CFDictionarySetValue(errorDictionary, kCFErrorURLKey, url);

		CFStringRef displayName = CreateDisplayNameForURL(url);
		if(displayName) {
			CFStringRef description = CFStringCreateWithFormat(kCFAllocatorDefault, nullptr, descriptionFormatStringForURL, displayName);
			if(description) {
				CFDictionarySetValue(errorDictionary, kCFErrorLocalizedDescriptionKey, description);
				CFRelease(description), description = nullptr;
			}
			CFRelease(displayName), displayName = nullptr;
		}
	}

	if(failureReason)
		CFDictionarySetValue(errorDictionary, kCFErrorLocalizedFailureReasonKey, failureReason);

	if(recoverySuggestion)
		CFDictionarySetValue(errorDictionary, kCFErrorLocalizedRecoverySuggestionKey, recoverySuggestion);

	CFErrorRef error = CFErrorCreate(kCFAllocatorDefault, domain, code, errorDictionary);

	CFRelease(errorDictionary), errorDictionary = nullptr;

	return error;
}
