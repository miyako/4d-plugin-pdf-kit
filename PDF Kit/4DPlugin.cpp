/* --------------------------------------------------------------------------------
 #
 #	4DPlugin.cpp
 #	source generated by 4D Plugin Wizard
 #	Project : PDF Kit
 #	author : miyako
 #	2015/09/24
 #
 # --------------------------------------------------------------------------------*/


#include "4DPluginAPI.h"
#include "4DPlugin.h"

//PDFDocument - release seems to freeze in El Capitan

#ifndef NSAppKitVersionNumber10_10_Max
#define NSAppKitVersionNumber10_10_Max 1349
#endif

//https://developer.apple.com/library/mac/releasenotes/AppKit/RN-AppKit/

namespace pdfkit
{
	void retain(PDFDocument *document)
	{
		if (NSAppKitVersionNumber > NSAppKitVersionNumber10_10_Max)
		{
		[document retain];
		}
	}
	void retain(PDFPage *page)
	{
		if (NSAppKitVersionNumber > NSAppKitVersionNumber10_10_Max)
		{
		[page retain];
		}
	}
}

#pragma mark -

void PluginMain(PA_long32 selector, PA_PluginParameters params)
{
	try
	{
		PA_long32 pProcNum = selector;
		sLONG_PTR *pResult = (sLONG_PTR *)params->fResult;
		PackagePtr pParams = (PackagePtr)params->fParameters;

		CommandDispatcher(pProcNum, pResult, pParams); 
	}
	catch(...)
	{

	}
}

void CommandDispatcher (PA_long32 pProcNum, sLONG_PTR *pResult, PackagePtr pParams)
{
	switch(pProcNum)
	{
// --- Page

		case 1 :
			PDF_Count_pages(pResult, pParams);
			break;

		case 2 :
			PDF_GET_PAGE2(pResult, pParams);
			break;

		case 3 :
			PDF_SET_PAGE_ROTATION2(pResult, pParams);
			break;

		case 4 :
			PDF_REMOVE_PAGE2(pResult, pParams);
			break;

		case 5 :
			PDF_EXCHANGE_PAGES2(pResult, pParams);
			break;

		case 6 :
			PDF_INSERT_PAGES2(pResult, pParams);
			break;

		case 7 :
			PDF_GET_PAGE_ANNOTATION(pResult, pParams);
			break;

// --- Picture

		case 8 :
			PDF_Data_from_picture(pResult, pParams);
			break;

// --- Document

		case 9 :
			PDF_GET_DOCUMENT_ATTRIBUTES(pResult, pParams);
			break;

		case 10 :
			PDF_SET_DOCUMENT_ATTRIBUTES(pResult, pParams);
			break;

		case 11 :
			PDF_Get_document_version(pResult, pParams);
			break;

		case 12 :
			PDF_Get_document_text(pResult, pParams);
			break;

	}
}

#pragma mark -

// ------------------------------------- Page -------------------------------------



//void PDF_GET_PAGE(sLONG_PTR *pResult, PackagePtr pParams)
//{
//	C_BLOB ParamData;
//	C_LONGINT ParamPageNumber;
//	C_BLOB ParamPageData;
//	C_TEXT ParamLabel;
//	C_TEXT ParamString;
//	
//	ParamData.fromParamAtIndex(pParams, 1);
//	ParamPageNumber.fromParamAtIndex(pParams, 2);
//	
//	NSData *pdfData = [[NSData alloc]initWithBytes:ParamData.getBytesPtr() length:ParamData.getBytesLength()];
//	
//	if(pdfData){
//		
//		PDFDocument *pdf = [[PDFDocument alloc]initWithData:pdfData];
//		
//		if(pdf){
//			pdfkit::retain(pdf);
//			unsigned int countPages = [pdf pageCount];
//			unsigned int pageNumber = ParamPageNumber.getIntValue();
//			
//			if((pageNumber > 0) && (pageNumber <= countPages)){
//				
//				pageNumber--;//index is zero based
//				PDFPage *page = [pdf pageAtIndex:pageNumber];
//				ParamString.setUTF16String([page label]);
//				ParamString.setUTF16String([page string]);				
//				NSData *pdfPageData = [page dataRepresentation];
//				ParamPageData.setBytes((const uint8_t *)[pdfPageData bytes], [pdfPageData length]);
//				
//			}
//			
//			[pdf release];
//		}
//		
//		[pdfData release];
//	}
//	
//	ParamPageData.toParamAtIndex(pParams, 3);
//	ParamLabel.toParamAtIndex(pParams, 4);
//	ParamString.toParamAtIndex(pParams, 5);
//}

//void PDF_SET_PAGE_ROTATION(sLONG_PTR *pResult, PackagePtr pParams)
//{
//	C_BLOB ParamData;
//	C_LONGINT ParamPageNumber;
//	C_LONGINT ParamRotation;
//	
//	ParamData.fromParamAtIndex(pParams, 1);
//	ParamPageNumber.fromParamAtIndex(pParams, 2);
//	ParamRotation.fromParamAtIndex(pParams, 3);
//	
//	NSData *pdfData = [[NSData alloc]initWithBytes:ParamData.getBytesPtr() length:ParamData.getBytesLength()];
//	
//	if(pdfData){
//		
//		PDFDocument *pdf = [[PDFDocument alloc]initWithData:pdfData];
//		
//		if(pdf){
//			pdfkit::retain(pdf);
//			unsigned int countPages = [pdf pageCount];
//			unsigned int pageNumber = ParamPageNumber.getIntValue();
//			unsigned int pageRotation = ParamRotation.getIntValue();
//			
//			if((pageNumber > 0) && (pageNumber <= countPages)){
//				
//				if((pageRotation == 0) || (pageRotation == 90) || (pageRotation == 180) || (pageRotation == 270)){
//					
//					pageNumber--;//index is zero based
//					PDFPage *page = [pdf pageAtIndex:pageNumber];	
//					[page setRotation:pageRotation];
//					NSData *pdfDataModified = [pdf dataRepresentation];
//					ParamData.setBytes((const uint8_t *)[pdfDataModified bytes], [pdfDataModified length]);
//					ParamData.toParamAtIndex(pParams, 1);
//					
//				}
//			}
//			[pdf release];
//		}
//		[pdfData release];
//	}
//}





//void PDF_REMOVE_PAGE2(sLONG_PTR *pResult, PackagePtr pParams)
//{
//	C_BLOB ParamData;
//	C_LONGINT ParamPageNumber;
//    
//	ParamData.fromParamAtIndex(pParams, 1);
//	ParamPageNumber.fromParamAtIndex(pParams, 2);
//    
//	CGDataProviderRef dataProvider = CGDataProviderCreateWithData(NULL, ParamData.getBytesPtr(), ParamData.getBytesLength(), NULL);
//	if(dataProvider)
//	{
//		CGPDFDocumentRef pdf = CGPDFDocumentCreateWithProvider(dataProvider);
//		if(pdf)
//		{
//			unsigned int countPages = CGPDFDocumentGetNumberOfPages(pdf);
//			unsigned int pageNumber = ParamPageNumber.getIntValue();
//			if((pageNumber > 0) && (pageNumber <= countPages))
//			{
//				NSMutableData *mutableData = (NSMutableData *)CFDataCreateMutable(kCFAllocatorDefault, 0);
//				CGDataConsumerRef dataConsumer = CGDataConsumerCreateWithCFData((CFMutableDataRef)mutableData);
//				CGContextRef ctx = CGPDFContextCreate(dataConsumer, NULL, NULL);
//				
//				if(ctx)
//				{
//					for(size_t i = 1; i <= countPages; ++i)
//					{
//						PA_YieldAbsolute();
//						CGPDFPageRef page = CGPDFDocumentGetPage(pdf, i);
//						if(page)
//						{
//							if(i != pageNumber)
//							{
//								CGRect rect = CGPDFPageGetBoxRect(page, kCGPDFCropBox);
//								CGContextBeginPage(ctx, &rect);
//								CGContextDrawPDFPage(ctx, page);
//								CGContextEndPage(ctx);
//							}
//						}
//					}
//					CGPDFContextClose(ctx);
//					CGContextRelease(ctx);
//					ParamData.setBytes((const uint8_t *)[mutableData bytes], [mutableData length]);
//				}
//			}
//			CGPDFDocumentRelease(pdf);
//		}
//		CGDataProviderRelease(dataProvider);
//	}
//	ParamData.toParamAtIndex(pParams, 1);
//}

#pragma mark -



//void PDF_EXCHANGE_PAGES(sLONG_PTR *pResult, PackagePtr pParams)
//{
//	C_BLOB ParamData;
//	C_LONGINT ParamPageNumberA;
//	C_LONGINT ParamPageNumberB;
//	
//	ParamData.fromParamAtIndex(pParams, 1);
//	ParamPageNumberA.fromParamAtIndex(pParams, 2);
//	ParamPageNumberB.fromParamAtIndex(pParams, 3);
//	
//	NSData *pdfData = [[NSData alloc]initWithBytes:ParamData.getBytesPtr() length:ParamData.getBytesLength()];
//	
//	if(pdfData){
//		
//		PDFDocument *pdf = [[PDFDocument alloc]initWithData:pdfData];
//		
//		if(pdf){
//			pdfkit::retain(pdf);
//			unsigned int countPages = [pdf pageCount];
//			unsigned int pageNumberA = ParamPageNumberA.getIntValue();
//			unsigned int pageNumberB = ParamPageNumberB.getIntValue();
//			
//			if((pageNumberA <= countPages) && (pageNumberB <= countPages) && (pageNumberA != pageNumberB)){
//				
//				pageNumberA--;//index is zero based
//				pageNumberB--;//index is zero based				
//				[pdf exchangePageAtIndex:pageNumberA withPageAtIndex:pageNumberB];
//				NSData *pdfDataModified = [pdf dataRepresentation];
//				ParamData.setBytes((const uint8_t *)[pdfDataModified bytes], [pdfDataModified length]);
//				ParamData.toParamAtIndex(pParams, 1);				
//				
//			}
//			
//			[pdf release];
//		}
//		
//		[pdfData release];
//	}
//	
//
//}

#pragma mark Using CGPDF instead of PDF Kit

void PDF_GET_PAGE2(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT ParamPageNumber;
	C_BLOB ParamPageData;
	C_TEXT ParamLabel;
	C_TEXT ParamString;
	
	ParamPageNumber.fromParamAtIndex(pParams, 2);
	
	PA_Handle h = *(PA_Handle *)(pParams[0]);
	if(h)
	{
		CGDataProviderRef dataProvider = CGDataProviderCreateWithData(NULL, PA_LockHandle(h), PA_GetHandleSize(h), NULL);
		
		if(dataProvider)
		{
			CGPDFDocumentRef pdf = CGPDFDocumentCreateWithProvider(dataProvider);
			if(pdf)
			{
				unsigned int pageNumber = ParamPageNumber.getIntValue();
				unsigned int countPages = CGPDFDocumentGetNumberOfPages(pdf);
				if(pageNumber <= countPages)
				{
					CGPDFPageRef page = CGPDFDocumentGetPage(pdf, pageNumber);
					NSMutableData *mutableData = (NSMutableData *)CFDataCreateMutable(kCFAllocatorDefault, 0);
					CGDataConsumerRef dataConsumer = CGDataConsumerCreateWithCFData((CFMutableDataRef)mutableData);
					CGContextRef ctx = CGPDFContextCreate(dataConsumer, NULL, NULL);
					
					CGRect rect = CGPDFPageGetBoxRect(page, kCGPDFCropBox);
					CGContextBeginPage(ctx, &rect);
					CGContextDrawPDFPage(ctx, page);
					CGContextEndPage(ctx);
					CGPDFContextClose(ctx);
					CGContextRelease(ctx);
					
					PDFDocument *pdfDocument = [[PDFDocument alloc]initWithData:mutableData];
					if(pdfDocument)
					{
						PDFPage *page = [pdfDocument pageAtIndex:0];
						ParamString.setUTF16String([page string]);
						ParamLabel.setUTF16String([page label]);
						[pdfDocument release];
					}
					
					ParamPageData.setBytes((const uint8_t *)[mutableData bytes], [mutableData length]);
					CGDataConsumerRelease(dataConsumer);
					[mutableData release];
				}
				CGPDFDocumentRelease(pdf);
			}
			CGDataProviderRelease(dataProvider);
		}
		PA_UnlockHandle(h);
	}
	
	ParamPageData.toParamAtIndex(pParams, 3);
	ParamLabel.toParamAtIndex(pParams, 4);
	ParamString.toParamAtIndex(pParams, 5);
}

void PDF_SET_PAGE_ROTATION2(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB ParamData;
	C_LONGINT ParamPageNumber;
	C_LONGINT ParamRotation;
	
	ParamData.fromParamAtIndex(pParams, 1);
	ParamPageNumber.fromParamAtIndex(pParams, 2);
	ParamRotation.fromParamAtIndex(pParams, 3);
	
	PA_Handle h = *(PA_Handle *)(pParams[0]);
	if(h)
	{
		CGDataProviderRef dataProvider = CGDataProviderCreateWithData(NULL, PA_LockHandle(h), PA_GetHandleSize(h), NULL);
	
		if(dataProvider)
		{
			CGPDFDocumentRef pdf = CGPDFDocumentCreateWithProvider(dataProvider);
			if(pdf)
			{
				unsigned int countPages = CGPDFDocumentGetNumberOfPages(pdf);
				unsigned int pageNumber = ParamPageNumber.getIntValue();
				unsigned int pageRotation = ParamRotation.getIntValue();
				if(pageNumber <= countPages)
				{
					NSMutableData *mutableData = (NSMutableData *)CFDataCreateMutable(kCFAllocatorDefault, 0);
					CGDataConsumerRef dataConsumer = CGDataConsumerCreateWithCFData((CFMutableDataRef)mutableData);
					CGContextRef ctx = CGPDFContextCreate(dataConsumer, NULL, NULL);
					if(ctx)
					{
						for(size_t i = 1; i <= countPages; ++i)
						{
							PA_YieldAbsolute();
							CGPDFPageRef page = CGPDFDocumentGetPage(pdf, i);
							if(page)
							{
								if((i == pageNumber) || (0 == pageNumber))
								{
									CGRect rect = CGPDFPageGetBoxRect(page, kCGPDFCropBox);
									rect = CGRectApplyAffineTransform(rect, CGAffineTransformMakeRotation(M_PI * pageRotation / 180.0f));
									CGContextBeginPage(ctx, &rect);
									CGContextRotateCTM(ctx, M_PI * pageRotation / 180.0f);
									CGContextDrawPDFPage(ctx, page);
									CGContextEndPage(ctx);
								}else
								{
									CGRect rect = CGPDFPageGetBoxRect(page, kCGPDFCropBox);
									CGContextBeginPage(ctx, &rect);
									CGContextDrawPDFPage(ctx, page);
									CGContextEndPage(ctx);
								}
							}
						}
						CGPDFContextClose(ctx);
						CGContextRelease(ctx);
						ParamData.setBytes((const uint8_t *)[mutableData bytes], [mutableData length]);
					}
					CGDataConsumerRelease(dataConsumer);
					[mutableData release];
				}
				CGPDFDocumentRelease(pdf);
			}
			CGDataProviderRelease(dataProvider);
		}
		PA_UnlockHandle(h);
	}
	ParamData.toParamAtIndex(pParams, 1);
}

#pragma mark 2016-09-20

void PDF_SET_DOCUMENT_ATTRIBUTES(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB ParamData;
	ARRAY_TEXT ParamKeys;
	ARRAY_TEXT ParamValues;
	
	ParamKeys.fromParamAtIndex(pParams, 2);
	ParamValues.fromParamAtIndex(pParams, 3);

	PA_Handle h = *(PA_Handle *)(pParams[0]);
	if(h)
	{
		NSData *pdfData = [[NSData alloc]initWithBytes:(const void *)PA_LockHandle(h) length:PA_GetHandleSize(h)];
		
		if(pdfData)
		{
			PDFDocument *pdf = [[PDFDocument alloc]initWithData:pdfData];
			
			if(pdf)
			{
				NSMutableDictionary *attributes = [[NSMutableDictionary alloc]initWithDictionary:[pdf documentAttributes]];
				
				NSArray *documentAttributes = [NSArray arrayWithObjects:
																			 @"PDF_TITLE", @"PDF_AUTHOR",
																			 @"PDF_SUBJECT", @"PDF_CREATOR",
																			 @"PDF_PRODUCER" ,@"PDF_KEYWORDS",
																			 @"PDF_CREATION_DATE", @"PDF_MODIFICATION_DATE", nil];
				
				if(ParamKeys.getSize() == ParamValues.getSize()){
					
					for(unsigned int i = 0; i < ParamKeys.getSize(); ++i){
						
						CUTF8String key, value;
						ParamKeys.copyUTF8StringAtIndex(&key, i);
						ParamValues.copyUTF8StringAtIndex(&value, i);
						
						NSString *k = [[NSString alloc]initWithUTF8String:(const char *)key.c_str()];
						NSString *v = [[NSString alloc]initWithUTF8String:(const char *)value.c_str()];
						
						NSDate *dateFromString = [NSDate dateWithString:v];
						
						switch ([documentAttributes indexOfObject:k])
						{
							case 0://PDF_TITLE
								[attributes setObject:v forKey:PDFDocumentTitleAttribute];
								break;
							case 1://PDF_AUTHOR
								[attributes setObject:v forKey:PDFDocumentAuthorAttribute];
								break;
							case 2://PDF_SUBJECT
								[attributes setObject:v forKey:PDFDocumentSubjectAttribute];
								break;
							case 3://PDF_CREATOR
								[attributes setObject:v forKey:PDFDocumentCreatorAttribute];
								break;
							case 4://PDF_PRODUCER
								[attributes setObject:v forKey:PDFDocumentProducerAttribute];
								break;
							case 5://PDF_KEYWORDS
								[attributes setObject:[v componentsSeparatedByString:@","] forKey:PDFDocumentKeywordsAttribute];
								break;
							case 6://PDF_CREATION_DATE
								if(dateFromString){
									[attributes setObject:dateFromString forKey:PDFDocumentCreationDateAttribute];
								}else{
									[attributes setObject:[NSNull null] forKey:PDFDocumentCreationDateAttribute];
								}
								break;
							case 7://PDF_MODIFICATION_DATE
								if(dateFromString){
									[attributes setObject:dateFromString forKey:PDFDocumentModificationDateAttribute];
								}else{
									[attributes setObject:[NSNull null] forKey:PDFDocumentModificationDateAttribute];
								}
								break;
						}
						
						[k release];
						[v release];
					}
				}
				
				[pdf setDocumentAttributes:attributes];
				NSData *pdfDataModified = [pdf dataRepresentation];
				ParamData.setBytes((const uint8_t *)[pdfDataModified bytes], [pdfDataModified length]);
				
				[attributes release];
				[pdf release];
				
			}
			[pdfData release];
		}
		PA_UnlockHandle(h);
	}
	ParamData.toParamAtIndex(pParams, 1);
}

void PDF_GET_DOCUMENT_ATTRIBUTES(sLONG_PTR *pResult, PackagePtr pParams)
{
	ARRAY_TEXT ParamKeys;
	ARRAY_TEXT ParamValues;
	
	ParamKeys.setSize(1);
	ParamValues.setSize(1);
	
	PA_Handle h = *(PA_Handle *)(pParams[0]);
	if(h)
	{
		NSData *pdfData = [[NSData alloc]initWithBytes:(const void *)PA_LockHandle(h) length:PA_GetHandleSize(h)];
		
		if(pdfData)
		{
			PDFDocument *pdf = [[PDFDocument alloc]initWithData:pdfData];
			
			if(pdf)
			{
				NSDictionary *attributes = [pdf documentAttributes];
				
				NSString *title = [attributes objectForKey:PDFDocumentTitleAttribute];
				NSString *author = [attributes objectForKey:PDFDocumentAuthorAttribute];
				NSString *subject = [attributes objectForKey:PDFDocumentSubjectAttribute];
				NSString *creator = [attributes objectForKey:PDFDocumentCreatorAttribute];
				NSString *producer = [attributes objectForKey:PDFDocumentProducerAttribute];
				NSArray *keywords = [attributes objectForKey:PDFDocumentKeywordsAttribute];
				NSDate *creationDate = [attributes objectForKey:PDFDocumentCreationDateAttribute];
				NSDate *modificationDate = [attributes objectForKey:PDFDocumentModificationDateAttribute];
				
				if(title){
					ParamKeys.appendUTF16String(@"PDF_TITLE");
					ParamValues.appendUTF16String(title);
				}
				
				if(author){
					ParamKeys.appendUTF16String(@"PDF_AUTHOR");
					ParamValues.appendUTF16String(author);
				}
				
				if(subject){
					ParamKeys.appendUTF16String(@"PDF_SUBJECT");
					ParamValues.appendUTF16String(subject);
				}
				
				if(creator){
					ParamKeys.appendUTF16String(@"PDF_CREATOR");
					ParamValues.appendUTF16String(creator);
				}
				
				if(producer){
					ParamKeys.appendUTF16String(@"PDF_PRODUCER");
					ParamValues.appendUTF16String(producer);
				}
				
				if(keywords){
					ParamKeys.appendUTF16String(@"PDF_KEYWORDS");
					ParamValues.appendUTF16String([keywords componentsJoinedByString:@","]);
				}
				
				if(creationDate){
					ParamKeys.appendUTF16String(@"PDF_CREATION_DATE");
					ParamValues.appendUTF16String([creationDate description]);
				}
				
				if(modificationDate){
					ParamKeys.appendUTF16String(@"PDF_MODIFICATION_DATE");
					ParamValues.appendUTF16String([modificationDate description]);
				}
				
				[pdf release];
			}
			[pdfData release];
		}
		PA_UnlockHandle(h);
	}//h
	
	ParamKeys.toParamAtIndex(pParams, 2);
	ParamValues.toParamAtIndex(pParams, 3);
}

void PDF_Data_from_picture(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB returnValue;
	
	PA_Picture ParamPicture = *(PA_Picture *)(pParams[0]);
	CGImageRef cgImage = (CGImageRef)PA_CreateNativePictureForScreen(ParamPicture);
	if(cgImage)
	{
		NSImage *pageImage = [[NSImage alloc]initWithCGImage:cgImage size:NSZeroSize];
		CFRelease(cgImage);
		if(pageImage)
		{
			PDFPage *page = [(PDFPage *)[PDFPage alloc]initWithImage:pageImage];
			if(page)
			{
				NSData *pdfData = [page dataRepresentation];
				returnValue.setBytes((const uint8_t *)[pdfData bytes], [pdfData length]);
				[page release];
				
			}
			[pageImage release];
		}
	}
	
	returnValue.setReturn(pResult);
}

void PDF_GET_PAGE_ANNOTATION(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB ParamData;
	C_LONGINT ParamPageNumber;
	ARRAY_TEXT ParamTypes;
	ARRAY_TEXT ParamCoordinates;
	ARRAY_TEXT ParamColors;
	ARRAY_TEXT ParamContents;
	
	ParamPageNumber.fromParamAtIndex(pParams, 2);
	ParamTypes.setSize(1);
	ParamCoordinates.setSize(1);
	ParamColors.setSize(1);
	ParamContents.setSize(1);
	
	PA_Handle h = *(PA_Handle *)(pParams[0]);
	if(h)
	{
		NSData *pdfData = [[NSData alloc]initWithBytes:(const void *)PA_LockHandle(h) length:PA_GetHandleSize(h)];
		
		if(pdfData)
		{
			PDFDocument *pdf = [[PDFDocument alloc]initWithData:pdfData];
			
			if(pdf)
			{
				unsigned int countPages = [pdf pageCount];
				unsigned int pageNumber = ParamPageNumber.getIntValue();
				
				if((pageNumber > 0) && (pageNumber <= countPages))
				{
					pageNumber--;//index is zero based
					PDFPage *page = [pdf pageAtIndex:pageNumber];
					NSArray *annotations = [page annotations];
					
					for(unsigned int i = 0;i < [annotations count]; ++i)
					{
						PDFAnnotation *annotation = [annotations objectAtIndex:i];
						ParamTypes.appendUTF16String([annotation type]);
						
						NSRect boundsRect = [annotation bounds];
						NSString *coordinatesString = [[NSString alloc]initWithFormat:@"%f %f %f %f", boundsRect.origin.x, boundsRect.origin.y, boundsRect.size.width, boundsRect.size.height];
						ParamCoordinates.appendUTF16String(coordinatesString);
						[coordinatesString release];
						
						ParamColors.appendUTF16String([[annotation color]description]);
						
						if([[annotation class]isSubclassOfClass:[PDFAnnotationInk class]])
						{
							NSArray *paths = [(PDFAnnotationInk *)annotation paths];
							NSMutableString *pathString = [[NSMutableString alloc]init];
							
							for(unsigned int p = 0;p < [paths count]; ++p){
								
								NSBezierPath *path = [paths objectAtIndex:p];
								
								for(int pathElement = 0; pathElement < [path elementCount];++pathElement){
									
									NSPoint points[3];
									NSBezierPathElement element = [path elementAtIndex:pathElement associatedPoints:points];
									
									switch (element){
										case NSMoveToBezierPathElement:
											[pathString appendFormat:@"M %f %f ", points[0].x, points[0].y];
											break;
										case NSLineToBezierPathElement:
											[pathString appendFormat:@"L %f %f ", points[0].x, points[0].y];
											break;
										case NSCurveToBezierPathElement:
											[pathString appendFormat:@"C %f %f %f %f %f %f ", points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y];
											break;
										case NSClosePathBezierPathElement:
											[pathString appendFormat:@"Z %f %f ", points[0].x, points[0].y];
											break;
										default:
											break;
									}
								}
								
							}
							ParamContents.appendUTF16String(pathString);
							[pathString release];
							continue;
						}
						
						if(([[annotation class]isSubclassOfClass:[PDFAnnotationText class]]) || ([[annotation class]isSubclassOfClass:[PDFAnnotationFreeText class]])){
							ParamContents.appendUTF16String([annotation contents]);
							continue;
						}
						
						if([[annotation class]isSubclassOfClass:[PDFAnnotationLine class]]){
							NSPoint startPoint = [(PDFAnnotationLine *)annotation startPoint];
							NSPoint endPoint = [(PDFAnnotationLine *)annotation endPoint];
							NSString *pathString = [[NSString alloc]initWithFormat:@"%f %f %f %f", startPoint.x, startPoint.y, endPoint.x, endPoint.y];
							ParamContents.appendUTF16String(pathString);
							[pathString release];
							continue;
						}
						
						if(([[annotation class]isSubclassOfClass:[PDFAnnotationLink class]])){
							NSString *linkString;
							PDFDestination *destination = [(PDFAnnotationLink *)annotation destination];
							NSURL *url = [(PDFAnnotationLink *)annotation URL];
							if(url){
								linkString = [url absoluteString];
							}else{
								linkString = [NSString stringWithFormat:@"%@ (%f %f)", [[destination page]label], [destination point].x, [destination point].y];
							}
							ParamContents.appendUTF16String(linkString);
							continue;
						}
						
						if([[annotation class]isSubclassOfClass:[PDFAnnotationStamp class]]){
							ParamContents.appendUTF16String([(PDFAnnotationStamp *)annotation name]);
							continue;
						}
						ParamContents.appendUTF16String(@"");
					}
					
				}
				[pdf release];
			}
			[pdfData release];
		}
		PA_UnlockHandle(h);
	}//h

	ParamTypes.toParamAtIndex(pParams, 3);
	ParamCoordinates.toParamAtIndex(pParams, 4);
	ParamColors.toParamAtIndex(pParams, 5);
	ParamContents.toParamAtIndex(pParams, 6);
}

//void PDF_INSERT_PAGES(sLONG_PTR *pResult, PackagePtr pParams)
//{
//	C_BLOB ParamData;
//	C_LONGINT ParamPageNumber;
//	C_BLOB ParamDataToInsert;
//	
//	ParamPageNumber.fromParamAtIndex(pParams, 2);
//	ParamDataToInsert.fromParamAtIndex(pParams, 3);
//	
//	PA_Handle h = *(PA_Handle *)(pParams[0]);
//	if(h)
//	{
//		NSData *pdfData = [[NSData alloc]initWithBytes:(const void *)PA_LockHandle(h) length:PA_GetHandleSize(h)];
//		
//		if(pdfData)
//		{
//			PDFDocument *pdf = [[PDFDocument alloc]initWithData:pdfData];
//			
//			if(pdf)
//			{
//				unsigned int countPages = [pdf pageCount];
//				unsigned int pageNumber = ParamPageNumber.getIntValue();
//				
//				if((pageNumber > 0) && (pageNumber <= countPages))
//				{
//					pageNumber--;
//					NSData *pdfDataToInsert = [[NSData alloc]initWithBytes:ParamDataToInsert.getBytesPtr() length:ParamDataToInsert.getBytesLength()];
//					
//					if(pdfDataToInsert)
//					{
//						PDFDocument *pdfToInsert = [[PDFDocument alloc]initWithData:pdfDataToInsert];
//						
//						if(pdfToInsert)
//						{
//							for(unsigned int i = 0;i < [pdfToInsert pageCount]; ++i)
//							{
//								[pdf insertPage:[pdfToInsert pageAtIndex:i] atIndex:pageNumber];
//								pageNumber++;
//							}
//							
//							NSData *pdfDataModified = [pdf dataRepresentation];
//							ParamData.setBytes((const uint8_t *)[pdfDataModified bytes], [pdfDataModified length]);
//							
//							[pdfToInsert release];
//						}
//						[pdfDataToInsert release];
//					}
//				}
//				[pdf release];
//			}//pdf
//			[pdfData release];
//		}//pdfData
//		PA_UnlockHandle(h);
//	}//h
//	ParamData.toParamAtIndex(pParams, 1);
//}

//void PDF_REMOVE_PAGE(sLONG_PTR *pResult, PackagePtr pParams)
//{
//	C_BLOB ParamData;
//	C_LONGINT ParamPageNumber;
//	
//	ParamPageNumber.fromParamAtIndex(pParams, 2);
//	
//	PA_Handle h = *(PA_Handle *)(pParams[0]);
//	if(h)
//	{
//		NSData *pdfData = [[NSData alloc]initWithBytes:(const void *)PA_LockHandle(h) length:PA_GetHandleSize(h)];
//		
//		if(pdfData)
//		{
//			PDFDocument *pdf = [[PDFDocument alloc]initWithData:pdfData];
//			if(pdf)
//			{
//				unsigned int countPages = [pdf pageCount];
//				unsigned int pageNumber = ParamPageNumber.getIntValue();
//				if((pageNumber > 0) && (pageNumber <= countPages))
//				{
//					pageNumber--;//index is zero based
//					[pdf removePageAtIndex:pageNumber];
//					
//					NSData *pdfDataModified = [pdf dataRepresentation];
//					ParamData.setBytes((const uint8_t *)[pdfDataModified bytes], [pdfDataModified length]);
//				}
//				[pdf release];
//			}//pdf
//			[pdfData release];
//		}//pdfData
//		PA_UnlockHandle(h);
//	}//h
//	ParamData.toParamAtIndex(pParams, 1);
//}

void PDF_Count_pages(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT returnValue;
	
	PA_Handle h = *(PA_Handle *)(pParams[0]);
	if(h)
	{
		NSData *pdfData = [[NSData alloc]initWithBytes:(const void *)PA_LockHandle(h) length:PA_GetHandleSize(h)];
		
		if(pdfData)
		{
			PDFDocument *pdf = [[PDFDocument alloc]initWithData:pdfData];
			if(pdf)
			{
				returnValue.setIntValue([pdf pageCount]);
				[pdf release];
			}//pdf
			[pdfData release];
		}//pdfData
		PA_UnlockHandle(h);
	}//h
	returnValue.setReturn(pResult);
}

void PDF_Get_document_version(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB ParamData;
	C_TEXT returnValue;
	
	PA_Handle h = *(PA_Handle *)(pParams[0]);
	if(h)
	{
		NSData *pdfData = [[NSData alloc]initWithBytes:(const void *)PA_LockHandle(h) length:PA_GetHandleSize(h)];
		
		if(pdfData)
		{
			PDFDocument *pdf = [[PDFDocument alloc]initWithData:pdfData];
			if(pdf)
			{
				returnValue.setUTF16String([NSString stringWithFormat:@"%d.%d", [pdf majorVersion], [pdf minorVersion]]);
				[pdf release];
			}//pdf
			[pdfData release];
		}//pdfData
		PA_UnlockHandle(h);
	}//h
	returnValue.setReturn(pResult);
}

void PDF_Get_document_text(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT returnValue;
	
	PA_Handle h = *(PA_Handle *)(pParams[0]);
	if(h)
	{
		NSData *pdfData = [[NSData alloc]initWithBytes:(const void *)PA_LockHandle(h) length:PA_GetHandleSize(h)];
		if(pdfData)
		{
			PDFDocument *pdf = [[PDFDocument alloc]initWithData:pdfData];
			if(pdf)
			{
				returnValue.setUTF16String([pdf string]);
				[pdf release];
			}//pdf
			[pdfData release];
		}//pdfData
		PA_UnlockHandle(h);
	}//h
	returnValue.setReturn(pResult);
}

void PDF_EXCHANGE_PAGES2(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB ParamData;
	C_LONGINT ParamPageNumberA;
	C_LONGINT ParamPageNumberB;
	
	ParamPageNumberA.fromParamAtIndex(pParams, 2);
	ParamPageNumberB.fromParamAtIndex(pParams, 3);
	
	PA_Handle h = *(PA_Handle *)(pParams[0]);
	if(h)
	{
		NSData *pdfData = [[NSData alloc]initWithBytes:(const void *)PA_LockHandle(h) length:PA_GetHandleSize(h)];
		if(pdfData)
		{
			PDFDocument *pdf = [[PDFDocument alloc]initWithData:pdfData];
			if(pdf)
			{
				unsigned int countPages = [pdf pageCount];
				unsigned int pageNumberA = ParamPageNumberA.getIntValue();
				unsigned int pageNumberB = ParamPageNumberB.getIntValue();
				
				if((pageNumberA <= countPages) && (pageNumberB <= countPages) && (pageNumberA != pageNumberB)){
					
					pageNumberA--;//index is zero based
					pageNumberB--;//index is zero based
					
					NSUInteger lowerPage, higherPage;
					
					if(pageNumberA < pageNumberB)
					{
						lowerPage = pageNumberA;
						higherPage = pageNumberB;
					}else{
						lowerPage = pageNumberB;
						higherPage = pageNumberA;
					}
					
					[pdf insertPage:[pdf pageAtIndex:lowerPage] atIndex:higherPage];
					[pdf removePageAtIndex:lowerPage];
					[pdf insertPage:[pdf pageAtIndex:higherPage] atIndex:lowerPage];
					[pdf removePageAtIndex:higherPage+1];
					
					NSData *pdfDataModified = [pdf dataRepresentation];
					ParamData.setBytes((const uint8_t *)[pdfDataModified bytes], [pdfDataModified length]);
				}
				[pdf release];
			}//pdf
			[pdfData release];
		}//pdfData
		PA_UnlockHandle(h);
	}//h
	ParamData.toParamAtIndex(pParams, 1);
}

void PDF_REMOVE_PAGE2(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB ParamData;
	C_LONGINT ParamPageNumber;
	
	ParamPageNumber.fromParamAtIndex(pParams, 2);
	
	PA_Handle h = *(PA_Handle *)(pParams[0]);
	if(h)
	{
		NSData *pdfData = [[NSData alloc]initWithBytes:(const void *)PA_LockHandle(h) length:PA_GetHandleSize(h)];
		
		if(pdfData)
		{
			PDFDocument *pdf = [[PDFDocument alloc]initWithData:pdfData];
			if(pdf)
			{
				unsigned int countPages = [pdf pageCount];
				unsigned int pageNumber = ParamPageNumber.getIntValue();
				if((pageNumber > 0) && (pageNumber <= countPages))
				{
					for(unsigned int i = pageNumber; i < countPages;++i)
					{
						[pdf insertPage:[pdf pageAtIndex:i] atIndex:i-1];
						[pdf removePageAtIndex:i];
					}
					[pdf removePageAtIndex:--countPages];
					
					NSData *pdfDataModified = [pdf dataRepresentation];
					ParamData.setBytes((const uint8_t *)[pdfDataModified bytes], [pdfDataModified length]);
				}
				[pdf release];
			}//pdf
			[pdfData release];
		}//pdfData
		PA_UnlockHandle(h);
	}//h
	ParamData.toParamAtIndex(pParams, 1);
}

void PDF_INSERT_PAGES2(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB ParamData;
	C_LONGINT ParamPageNumber;
	C_BLOB ParamDataToInsert;
	
	ParamPageNumber.fromParamAtIndex(pParams, 2);
	ParamDataToInsert.fromParamAtIndex(pParams, 3);
	
	PA_Handle h = *(PA_Handle *)(pParams[0]);
	if(h)
	{
		NSData *pdfData = [[NSData alloc]initWithBytes:(const void *)PA_LockHandle(h) length:PA_GetHandleSize(h)];
		
		if(pdfData)
		{
			PDFDocument *pdf = [[PDFDocument alloc]initWithData:pdfData];
			
			if(pdf)
			{
				unsigned int countPages = [pdf pageCount];
				unsigned int pageNumber = ParamPageNumber.getIntValue();
				
				if((pageNumber > 0) && (pageNumber <= countPages))
				{
					pageNumber--;
					NSData *pdfDataToInsert = [[NSData alloc]initWithBytes:ParamDataToInsert.getBytesPtr() length:ParamDataToInsert.getBytesLength()];
					
					if(pdfDataToInsert)
					{
						PDFDocument *pdfToInsert = [[PDFDocument alloc]initWithData:pdfDataToInsert];
						
						if(pdfToInsert)
						{
							NSUInteger lastPageIndex = countPages;
							NSUInteger numberOfPagesToDelete = countPages-pageNumber;
                            NSUInteger pageNumberToDelete = lastPageIndex + ([pdfToInsert pageCount]) -1;
							countPages--;
							for(signed int i = lastPageIndex;i > pageNumber; --i)
							{
								[pdf insertPage:[pdf pageAtIndex:i-1] atIndex:countPages];
//                                NSLog(@"insertPage:%i atIndex:%i", i-1, countPages);
							}
							for(unsigned int i = 0;i < [pdfToInsert pageCount]; ++i)
							{
								[pdf insertPage:[pdfToInsert pageAtIndex:i] atIndex:pageNumber];
								pageNumber++;
							}
                            
							for(unsigned int i = 0;i < numberOfPagesToDelete; ++i)
							{
								[pdf removePageAtIndex:pageNumberToDelete];
//                                NSLog(@"removePageAtIndex:%i", (unsigned int)pageNumberToDelete);
							}
							
							NSData *pdfDataModified = [pdf dataRepresentation];
							ParamData.setBytes((const uint8_t *)[pdfDataModified bytes], [pdfDataModified length]);
							
							[pdfToInsert release];
						}
						[pdfDataToInsert release];
					}
				}
				[pdf release];
			}//pdf
			[pdfData release];
		}//pdfData
		PA_UnlockHandle(h);
	}//h
	ParamData.toParamAtIndex(pParams, 1);
}
