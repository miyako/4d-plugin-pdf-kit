#pragma once
#import <Cocoa/Cocoa.h>

// Only these synthetic PDF classes are linked. The PDFKit framework is not.
extern NSString *PDFDocumentTitleAttribute;
extern NSString *PDFDocumentAuthorAttribute;
extern NSString *PDFDocumentSubjectAttribute;
extern NSString *PDFDocumentCreatorAttribute;
extern NSString *PDFDocumentProducerAttribute;
extern NSString *PDFDocumentKeywordsAttribute;
extern NSString *PDFDocumentCreationDateAttribute;
extern NSString *PDFDocumentModificationDateAttribute;

@interface OwnershipImage : NSObject
- (id)initWithCGImage:(CGImageRef)image size:(NSSize)size;
@end
#define NSImage OwnershipImage

@interface PDFPage : NSObject
- (NSString *)label;
- (NSString *)string;
- (NSData *)dataRepresentation;
- (NSArray *)annotations;
- (void)setRotation:(NSInteger)rotation;
- (id)initWithImage:(OwnershipImage *)image;
@end
@interface PDFDocument : NSObject
- (id)initWithData:(NSData *)data;
- (NSUInteger)pageCount;
- (PDFPage *)pageAtIndex:(NSUInteger)index;
- (void)removePageAtIndex:(NSUInteger)index;
- (void)exchangePageAtIndex:(NSUInteger)a withPageAtIndex:(NSUInteger)b;
- (void)insertPage:(PDFPage *)page atIndex:(NSUInteger)index;
- (NSData *)dataRepresentation;
- (NSDictionary *)documentAttributes;
- (void)setDocumentAttributes:(NSDictionary *)attributes;
- (NSInteger)majorVersion;
- (NSInteger)minorVersion;
- (NSString *)string;
@end
@interface PDFAnnotation : NSObject
- (NSString *)type;
- (NSRect)bounds;
- (id)color;
- (NSString *)contents;
@end
@interface PDFAnnotationInk : PDFAnnotation
- (NSArray *)paths;
@end
@interface PDFAnnotationText : PDFAnnotation @end
@interface PDFAnnotationFreeText : PDFAnnotation @end
@interface PDFAnnotationLine : PDFAnnotation
- (NSPoint)startPoint;
- (NSPoint)endPoint;
@end
@interface PDFDestination : NSObject
- (PDFPage *)page;
- (NSPoint)point;
@end
@interface PDFAnnotationLink : PDFAnnotation
- (PDFDestination *)destination;
- (NSURL *)URL;
@end
@interface PDFAnnotationStamp : PDFAnnotation
- (NSString *)name;
@end
