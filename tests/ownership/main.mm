#include "4DPluginAPI.h"
#include "PrivateTypes.h"
#include "EntryPoints.h"
#include "C_TEXT.h"
#include "ARRAY_TEXT.h"
#include "C_BLOB.h"
#include "4DPlugin-JSON.h"
#import "Quartz/Quartz.h"
#import <objc/runtime.h>
#include <map>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <cstring>
#include <iostream>

extern "C" CFURLRef CFURLCreateWithFileSystemPath(CFAllocatorRef, CFStringRef,
                                                CFURLPathStyle, Boolean) {
    throw std::runtime_error("path operations prohibited");
}
extern "C" CFStringRef CFURLCopyFileSystemPath(CFURLRef, CFURLPathStyle) {
    throw std::runtime_error("path operations prohibited");
}
extern "C" void CGContextScaleCTM(CGContextRef, CGFloat, CGFloat) {
    throw std::runtime_error("drawing operations prohibited");
}
extern "C" void CGContextTranslateCTM(CGContextRef, CGFloat, CGFloat) {
    throw std::runtime_error("drawing operations prohibited");
}

static NSString *fixture = @"";
static bool boundaryUsed = false;
static bool pictureBoundaryUsed = false;
static int documentLive = 0;
static NSInteger observedRotation = -1;
static NSDictionary *observedAttributes = nil;
NSString *PDFDocumentTitleAttribute = @"title";
NSString *PDFDocumentAuthorAttribute = @"author";
NSString *PDFDocumentSubjectAttribute = @"subject";
NSString *PDFDocumentCreatorAttribute = @"creator";
NSString *PDFDocumentProducerAttribute = @"producer";
NSString *PDFDocumentKeywordsAttribute = @"keywords";
NSString *PDFDocumentCreationDateAttribute = @"created";
NSString *PDFDocumentModificationDateAttribute = @"modified";

@implementation OwnershipImage
- (id)initWithCGImage:(CGImageRef)image size:(NSSize)size {
    throw std::runtime_error("real graphics operation prohibited");
}
@end
@implementation PDFDocument
- (id)initWithData:(NSData *)data {
    if ([data length] != 3 || memcmp([data bytes], "PDF", 3))
        throw std::runtime_error("unexpected synthetic document");
    boundaryUsed = true;
    ++documentLive;
    return [super init];
}
- (void)dealloc { --documentLive; [super dealloc]; }
- (NSUInteger)pageCount { return 2; }
- (PDFPage *)pageAtIndex:(NSUInteger)index {
    if (index >= 2) throw std::runtime_error("synthetic page out of range");
    return [[[PDFPage alloc] init] autorelease];
}
- (void)removePageAtIndex:(NSUInteger)index {
    if (index >= 2) throw std::runtime_error("synthetic remove out of range");
}
- (void)exchangePageAtIndex:(NSUInteger)a withPageAtIndex:(NSUInteger)b {
    if (a >= 2 || b >= 2) throw std::runtime_error("synthetic exchange out of range");
}
- (void)insertPage:(PDFPage *)page atIndex:(NSUInteger)index {
    if (!page || index > 2) throw std::runtime_error("synthetic insert out of range");
}
- (NSData *)dataRepresentation { return [NSData dataWithBytes:"OUT" length:3]; }
- (NSDictionary *)documentAttributes {
    return [NSDictionary dictionaryWithObjectsAndKeys:fixture, PDFDocumentTitleAttribute,
            fixture, PDFDocumentAuthorAttribute, fixture, PDFDocumentSubjectAttribute,
            fixture, PDFDocumentCreatorAttribute, fixture, PDFDocumentProducerAttribute,
            [NSArray arrayWithObject:fixture], PDFDocumentKeywordsAttribute, nil];
}
- (void)setDocumentAttributes:(NSDictionary *)attributes {
    [observedAttributes release]; observedAttributes = [attributes copy];
}
- (NSInteger)majorVersion { return 1; }
- (NSInteger)minorVersion { return 7; }
- (NSString *)string { return fixture; }
@end
@implementation PDFPage
- (NSString *)label { return @"label"; }
- (NSString *)string { return fixture; }
- (NSData *)dataRepresentation { return [NSData dataWithBytes:"OUT" length:3]; }
- (NSArray *)annotations {
    return [NSArray arrayWithObjects:
            [[[PDFAnnotationText alloc] init] autorelease],
            [[[PDFAnnotationFreeText alloc] init] autorelease],
            [[[PDFAnnotationInk alloc] init] autorelease],
            [[[PDFAnnotationLine alloc] init] autorelease],
            [[[PDFAnnotationLink alloc] init] autorelease],
            [[[PDFAnnotationStamp alloc] init] autorelease],
            [[[PDFAnnotation alloc] init] autorelease], nil];
}
- (void)setRotation:(NSInteger)rotation { observedRotation = rotation; }
- (id)initWithImage:(OwnershipImage *)image {
    throw std::runtime_error("unexpected image operation");
}
@end
@implementation PDFAnnotation
- (NSString *)type { return @"Test"; }
- (NSRect)bounds { return NSMakeRect(1, 2, 3, 4); }
- (id)color { return @"synthetic color"; }
- (NSString *)contents { return fixture; }
@end
@implementation PDFAnnotationInk
- (NSArray *)paths { return [NSArray array]; }
@end
@implementation PDFAnnotationText @end
@implementation PDFAnnotationFreeText @end
@implementation PDFAnnotationLine
- (NSPoint)startPoint { return NSMakePoint(1, 2); }
- (NSPoint)endPoint { return NSMakePoint(3, 4); }
@end
@implementation PDFAnnotationLink
- (PDFDestination *)destination { return [[[PDFDestination alloc] init] autorelease]; }
- (NSURL *)URL { return nil; }
@end
@implementation PDFDestination
- (PDFPage *)page { return [[[PDFPage alloc] init] autorelease]; }
- (NSPoint)point { return NSMakePoint(1, 2); }
@end
@implementation PDFAnnotationStamp
- (NSString *)name { return fixture; }
@end

namespace {
using Text = std::basic_string<PA_Unichar>;
std::map<PA_Unichar*, Text> strings;
std::map<PA_Handle, size_t> handles;
std::set<PA_Picture> pictures;
std::map<void*, int> refs;
PA_Variable property;
int failureCommand = 0;
bool failProperty = false, failResize = false, missingProperty = false;
int invalidResultInspections = 0, commandMode = 0;
int failures = 0, checks = 0;

void check(bool ok, const char* name) {
    ++checks;
    if (!ok) { ++failures; std::cerr << "FAIL " << name << "\n"; }
    else std::cout << "PASS " << name << "\n";
}
void require(bool ok) {
    if (!ok) throw std::runtime_error("invalid ownership or unexpected engine call");
}
PA_Unistring make(const PA_Unichar* p) {
    Text text(p ? p : reinterpret_cast<const PA_Unichar*>(u""));
    PA_Unistring s = {};
    s.fLength = static_cast<PA_long32>(text.size());
    s.fString = new PA_Unichar[text.size() + 1];
    std::copy(text.begin(), text.end(), s.fString);
    s.fString[text.size()] = 0;
    strings.emplace(s.fString, text);
    return s;
}
void dispose(PA_Unistring s) {
    if (s.fString) {
        require(strings.erase(s.fString) == 1);
        delete[] s.fString;
    }
}
void clear(PA_Variable& v) {
    switch (PA_GetVariableKind(v)) {
        case eVK_Unistring: dispose(v.uValue.fString); break;
        case eVK_Picture:
            if (v.uValue.fPicture) {
                require(pictures.erase(v.uValue.fPicture) == 1);
                delete static_cast<int*>(v.uValue.fPicture);
            }
            break;
        case eVK_Object: case eVK_Collection: {
            void* p = v.fType == eVK_Object ? v.uValue.fObject : v.uValue.fCollection;
            if (p) {
                require(refs.count(p) && refs[p] > 0);
                if (--refs[p] == 0) { refs.erase(p); delete static_cast<int*>(p); }
            }
            break;
        }
        case eVK_ArrayUnicode:
            if (v.uValue.fArray.fData) {
                auto p = reinterpret_cast<PA_Unistring*>(*v.uValue.fArray.fData);
                require(handles.at(v.uValue.fArray.fData) >=
                        (v.uValue.fArray.fNbElements + 1) * sizeof(*p));
                for (int i = 0; i <= v.uValue.fArray.fNbElements; ++i) dispose(p[i]);
                free(*v.uValue.fArray.fData);
                handles.erase(v.uValue.fArray.fData);
                delete v.uValue.fArray.fData;
            }
            break;
        default: break;
    }
    v = {};
    v.fType = eVK_Undefined;
}
PA_Variable copy(PA_Variable v) {
    if (PA_GetVariableKind(v) == eVK_Unistring)
        v.uValue.fString = make(v.uValue.fString.fString);
    if (v.fType == eVK_Object && v.uValue.fObject) ++refs.at(v.uValue.fObject);
    if (v.fType == eVK_Collection && v.uValue.fCollection) ++refs.at(v.uValue.fCollection);
    if (v.fType == eVK_Picture && v.uValue.fPicture) {
        v.uValue.fPicture = new int(0); pictures.insert(v.uValue.fPicture);
    }
    return v;
}
void engine(short selector, EngineBlock* eb) {
    eb->fError = eER_NoErr;
    switch (selector) {
        case EX_YIELD_ABSOLUTE: break;
        case EX_CREATE_UNISTRING:
            eb->fUniString1 = make(reinterpret_cast<PA_Unichar*>(eb->fHandle)); break;
        case EX_DISPOSE_UNISTRING: dispose(eb->fUniString1); break;
        case EX_SET_UNISTRING: {
            auto s = make(reinterpret_cast<PA_Unichar*>(eb->fHandle));
            dispose(eb->fUniString1); eb->fUniString1 = s; break;
        }
        case EX_CLEAR_VARIABLE: {
            auto v = static_cast<PA_Variable*>(eb->fPtr1);
            if ((failProperty || failureCommand) && v->fType == 0)
                ++invalidResultInspections;
            clear(*v); break;
        }
        case EX_VARIABLE_TO_STRING: {
            if (failProperty || failureCommand) {
                ++invalidResultInspections;
                eb->fUniString1 = {};
                break;
            }
            auto v = reinterpret_cast<PA_Variable*>(eb->fHandle);
            if (PA_GetVariableKind(*v) != eVK_Unistring) {
                ++invalidResultInspections; eb->fUniString1 = {}; break;
            }
            require(strings.count(v->uValue.fString.fString));
            eb->fUniString1 = v->uValue.fString; break;
        }
        case EX_HANDLE_MANAGER: {
            auto h = eb->fHandle;
            switch (eb->fParam1) {
                case 1:
                    h = new char*;
                    *h = static_cast<char*>(calloc(1, std::max<sLONG_PTR>(1, eb->fParam2)));
                    handles[h] = eb->fParam2; eb->fHandle = h; break;
                case 2:
                    require(handles.erase(h) == 1); free(*h); delete h; break;
                case 3: {
                    if (failResize) { eb->fError = -1; break; }
                    size_t old = handles.at(h);
                    *h = static_cast<char*>(realloc(*h, std::max<sLONG_PTR>(1, eb->fParam2)));
                    if (static_cast<size_t>(eb->fParam2) > old)
                        memset(*h + old, 0, eb->fParam2 - old);
                    handles[h] = eb->fParam2; break;
                }
                case 4: eb->fParam2 = handles.at(h); break;
                case 5: eb->fParam3 = reinterpret_cast<sLONG_PTR>(*h); break;
                case 6: break;
                default: throw std::runtime_error("unexpected handle operation");
            }
            break;
        }
        case EX_CREATE_PICTURE:
            eb->fPicture = new int(0); pictures.insert(eb->fPicture); break;
        case EX_DISPOSE_PICTURE:
            require(pictures.erase(eb->fPicture) == 1);
            delete static_cast<int*>(eb->fPicture); break;
        case EX_DUPLICATE_PICTURE:
            require(pictures.count(eb->fPicture));
            require(eb->fParam1 == 1); // independent copy, not retainOnly
            eb->fPicture = new int(0); pictures.insert(eb->fPicture); break;
        case EX_CREATE_NATIVE_PICTURE_FOR_SCREEN:
            require(pictures.count(eb->fPicture));
            pictureBoundaryUsed = true;
            eb->fHandle = nullptr; // never manufacture or execute native graphics
            break;
        case EX_GET_OBJ_VALUE:
            if (failProperty) {
                memset(eb->fPtr2, 0, sizeof(PA_Variable));
                eb->fError = -1;
            } else *static_cast<PA_Variable*>(eb->fPtr2) = copy(property);
            break;
        case EX_SET_OBJ_VALUE:
            clear(property);
            property = copy(*static_cast<PA_Variable*>(eb->fPtr2)); break;
        case EX_EXECUTE_COMMAND_BY_ID: {
            auto result = static_cast<PA_Variable*>(eb->fPtr2);
            if (failureCommand == eb->fParam1) {
                memset(result, 0, sizeof(*result));
                eb->fError = -1; break;
            }
            switch (eb->fParam1) {
                case 1231: PA_SetBooleanVariable(result, !missingProperty); break;
                case 1225: {
                    void* p = new int(0); refs[p] = 1;
                    PA_SetObjectVariable(result, p); break;
                }
                case 1217:
                    if (commandMode == 1) *result = copy(property);
                    else { auto s = make(reinterpret_cast<const PA_Unichar*>(u"{\"ok\":true}"));
                           PA_SetStringVariable(result, &s); }
                    break;
                default: throw std::runtime_error("unexpected command ID");
            }
            break;
        }
        default: throw std::runtime_error("unexpected selector");
    }
}
void reset() {
    clear(property);
    // Account for leaked owners, then isolate the next baseline case.
    for (auto& s : strings) delete[] s.first;
    strings.clear();
    for (auto p : pictures) delete static_cast<int*>(p);
    pictures.clear();
    for (auto p : refs) delete static_cast<int*>(p.first);
    refs.clear();
    require(handles.empty());
}
void balance(const char* name) {
    clear(property);
    check(strings.empty() && pictures.empty() && refs.empty() && handles.empty(), name);
    reset();
}
Text text(NSString* s) {
    Text out([s length], 0);
    if (!out.empty()) [s getCharacters:&out[0] range:NSMakeRange(0, out.size())];
    return out;
}
void dispatch(int command, std::initializer_list<void*> args, void* result = nullptr) {
    std::vector<uint8_t*> p;
    for (auto arg : args) p.push_back(static_cast<uint8_t*>(arg));
    PluginBlock block = {};
    block.fParameters = p.data(); block.fResult = result;
    PluginMain(command, &block);
}
PA_Handle blob(const char* data) {
    auto h = PA_NewHandle(3);
    memcpy(PA_LockHandle(h), data, 3); PA_UnlockHandle(h);
    return h;
}
void denyAppleBoundary(id, SEL, ...) { throw std::runtime_error("external Apple boundary denied"); }
void deny(Class cls, const char* selector, bool classMethod = false) {
    auto m = classMethod ? class_getClassMethod(cls, sel_registerName(selector)) :
                          class_getInstanceMethod(cls, sel_registerName(selector));
    require(m != nullptr);
    method_setImplementation(m, reinterpret_cast<IMP>(denyAppleBoundary));
}
}

int main(int argc, char** argv) {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    // Install boundaries before executing any plugin or wrapper code.
    deny([NSURL class], "initWithString:");
    deny([NSData class], "dataWithContentsOfFile:", true);
    deny([NSData class], "dataWithContentsOfURL:", true);
    deny([NSData class], "initWithContentsOfFile:");
    deny([NSData class], "initWithContentsOfURL:");
    gCall4D = engine;
    property.fType = eVK_Undefined;
    if (argc == 2 && std::string(argv[1]) == "--resize-failure") {
        PA_Variable a = PA_CreateVariable(eVK_ArrayUnicode);
        uint8_t* args[] = {reinterpret_cast<uint8_t*>(&a)};
        ARRAY_TEXT t; t.setSize(9);
        failResize = true;
        bool reported = false;
        try { t.toParamAtIndex(args, 1); }
        catch (const std::runtime_error& e) {
            reported = std::string(e.what()).find("-1") != std::string::npos;
        }
        failResize = false;
        check(reported && a.uValue.fArray.fNbElements == 0,
              "failed resize reported before insertion and preserves owner metadata");
        PA_ClearVariable(&a);
        balance("failed resize caller ownership balance");
        [pool drain];
        std::cout << "checks=" << checks << " failures=" << failures << "\n";
        return failures ? 1 : 0;
    }
    check(!std::is_copy_constructible<C_TEXT>::value &&
          !std::is_copy_assignable<C_TEXT>::value, "C_TEXT rejects shallow owner copies");
    check(!std::is_copy_constructible<ARRAY_TEXT>::value &&
          !std::is_copy_assignable<ARRAY_TEXT>::value, "ARRAY_TEXT rejects shallow owner copies");
    check(!std::is_copy_constructible<C_BLOB>::value &&
          !std::is_copy_assignable<C_BLOB>::value, "C_BLOB rejects shallow owner copies");
    PA_ObjectRef obj = reinterpret_cast<void*>(1); // never dereferenced by engine
    const char* values[] = {"", "ASCII", "\xc3\xa9", "\xf0\x9f\x98\x80"};
    for (const char* value : values) {
        ob_set_s(obj, "key", value);
        balance("narrow setter isolated owner balance");
        ob_set_s(obj, "key", value);
        CUTF8String got;
        check(ob_get_s(obj, L"key", &got) &&
              std::string(reinterpret_cast<const char*>(got.data()), got.size()) == value,
              "narrow setter/getter exact UTF8 content");
        balance("narrow setter/getter owner balance");
        ob_set_s(obj, L"key", value);
        balance("wide-key setter owner balance");
    }
    ob_set_a(obj, L"key", L"wide \u00e9 \U0001f600");
    balance("wide-value setter owner balance");
    CUTF16String native(reinterpret_cast<const PA_Unichar*>(u"native \u00e9 \U0001f600"));
    ob_set_a(obj, L"key", &native);
    CUTF16String nativeGot;
    check(ob_get_a(obj, L"key", &nativeGot) && nativeGot == native,
          "native UTF16 getter exact content");
    balance("native UTF16 setter/getter owner balance");
    auto picture = PA_CreatePicture(nullptr, 0);
    ob_set_p(obj, L"key", picture);
    check(pictures.count(picture) == 0, "picture setter consumes supplied owner control");
    if (pictures.count(picture)) PA_DisposePicture(picture);
    balance("picture setter independent owner balance");
    for (int kind : {eVK_Object, eVK_Collection}) {
        void* p = new int(0); refs[p] = 1;
        if (kind == eVK_Object) ob_set_o(obj, L"key", p);
        else ob_set_c(obj, L"key", p);
        void* returned = kind == eVK_Object ? ob_get_o(obj, L"key") : ob_get_c(obj, L"key");
        clear(property);
        check(refs.count(returned) && refs[returned] == 1,
              "object/collection result independent owner");
        if (kind == eVK_Object) PA_DisposeObject(returned);
        else PA_DisposeCollection(returned);
        balance("object/collection getter handoff owners");
    }
    ob_set_b(obj, L"key", true);
    check(ob_get_b(obj, L"key"), "boolean getter content control");
    balance("boolean property owners");
    ob_set_n(obj, L"key", 3.5);
    check(ob_get_n(obj, L"key") == 3.5, "real getter content control");
    balance("real property owners");
    ob_set_i(obj, L"key", 42);
    check(PA_GetLongintVariable(property) == 42 && ob_is_defined(obj, L"key"),
          "integer property and existence control");
    balance("integer property owners");
    for (int kind : {eVK_Unistring, eVK_Object, eVK_Collection, eVK_Picture}) {
        if (kind == eVK_Unistring) {
            auto s = make(reinterpret_cast<const PA_Unichar*>(u"mismatch"));
            PA_SetStringVariable(&property, &s);
        } else if (kind == eVK_Picture) {
            PA_SetPictureVariable(&property, PA_CreatePicture(nullptr, 0));
        } else {
            void* p = new int(0); refs[p] = 1;
            if (kind == eVK_Object) PA_SetObjectVariable(&property, p);
            else PA_SetCollectionVariable(&property, p);
        }
        CUTF8String u8; CUTF16String u16;
        ob_get_s(obj, L"key", &u8); ob_get_a(obj, L"key", &u16);
        ob_get_b(obj, L"key"); ob_get_n(obj, L"key");
        auto o = ob_get_o(obj, L"key"); if (o) PA_DisposeObject(o);
        auto c = ob_get_c(obj, L"key"); if (c) PA_DisposeCollection(c);
        balance("all six getters success/mismatch owner balance");
    }
    missingProperty = true;
    CUTF8String unchanged(reinterpret_cast<const uint8_t*>("unchanged"));
    check(!ob_get_s(obj, L"absent", &unchanged) &&
          unchanged == CUTF8String(reinterpret_cast<const uint8_t*>("unchanged")),
          "absent property leaves output unchanged");
    missingProperty = false;
    for (int mode : {0, 1, 2}) {
        commandMode = mode == 0 ? 0 : 1;
        if (mode == 1) PA_SetBooleanVariable(&property, 1);
        if (mode == 2) {
            void* p = new int(0); refs[p] = 1; PA_SetObjectVariable(&property, p);
        }
        CUTF8String result;
        ob_stringify(obj, &result);
        check(mode != 0 || result == CUTF8String(reinterpret_cast<const uint8_t*>("{\"ok\":true}")),
              "stringify exact content control");
        balance("stringify success/mismatch owners");
    }
    for (int id : {1225, 1217}) {
        failureCommand = id; invalidResultInspections = 0;
        bool threw = false;
        try { CUTF8String s; ob_stringify(obj, &s); }
        catch (const std::runtime_error& e) {
            threw = std::string(e.what()).find("-1") != std::string::npos;
        }
        check(threw && invalidResultInspections == 0,
              "failed command reported before result adoption");
        failureCommand = 0; balance("failed command owners");
    }
    for (bool existence : {true, false}) {
        for (int getter = 0; getter < (existence ? 7 : 6); ++getter) {
            failureCommand = existence ? 1231 : 0; failProperty = !existence;
            invalidResultInspections = 0;
            bool threw = false;
            try {
                CUTF8String u8; CUTF16String u16;
                switch (getter) {
                    case 0: ob_get_s(obj, L"key", &u8); break;
                    case 1: ob_get_a(obj, L"key", &u16); break;
                    case 2: ob_get_b(obj, L"key"); break;
                    case 3: ob_get_n(obj, L"key"); break;
                    case 4: ob_get_o(obj, L"key"); break;
                    case 5: ob_get_c(obj, L"key"); break;
                    case 6: ob_is_defined(obj, L"key"); break;
                }
            } catch (const std::runtime_error& e) {
                threw = std::string(e.what()).find("-1") != std::string::npos;
            }
            check(threw && invalidResultInspections == 0,
                  existence ? "failed property-existence command reported" :
                              "failed property reported before result adoption");
            failureCommand = 0; failProperty = false; balance("failed property owners");
        }
    }
    for (int n : {0, 1, 8, 64}) {
        PA_Variable a = PA_CreateVariable(eVK_ArrayUnicode);
        PA_Variable b = PA_CreateVariable(eVK_ArrayUnicode);
        uint8_t* args[] = {reinterpret_cast<uint8_t*>(&a)};
        uint8_t* other[] = {reinterpret_cast<uint8_t*>(&b)};
        ARRAY_TEXT t; t.setSize(n + 1);
        for (int i = 0; i <= n; ++i) t.setUTF16StringAtIndex(&native, i);
        t.toParamAtIndex(args, 1); t.toParamAtIndex(args, 1); t.toParamAtIndex(other, 1);
        for (int i = 0; i < 128; ++i) dispose(make(native.c_str()));
        auto first = PA_GetStringInArray(a, 0);
        check(first.fString != PA_GetStringInArray(b, 0).fString,
              "array outputs independent owners");
        PA_SetStringInArray(a, 0, nullptr);
        check(PA_GetStringInArray(a, 0).fString == first.fString,
              "null array input preserves old owner");
        ARRAY_TEXT snapshot; snapshot.fromParamAtIndex(args, 1);
        CUTF16String got; snapshot.copyUTF16StringAtIndex(&got, n);
        PA_ClearVariable(&a); PA_ResizeArray(&b, 0); PA_ClearVariable(&b);
        check(got == native && snapshot.getSize() == static_cast<unsigned>(n + 1),
              "borrowed getter snapshot survives clear and index zero");
        balance("array resize clear owners");
    }
    for (int rejected : {-1, 1, 2, 3}) {
        PA_Variable a = PA_CreateVariable(eVK_ArrayUnicode), dest = a;
        auto s = make(native.c_str());
        int index = rejected;
        if (rejected == 2) { dest.fType = eVK_Boolean; index = 0; }
        if (rejected == 3) { dest.uValue.fArray.fData = nullptr; index = 0; }
        PA_SetStringInArray(dest, index, &s);
        check(strings.count(s.fString) == 1, "rejected array assignment retains caller owner");
        PA_DisposeUnistring(&s); PA_ClearVariable(&a);
        balance("rejected assignment owner control");
    }
    {
        PA_Variable a = {}; a.fType = eVK_ArrayUnicode;
        uint8_t* args[] = {reinterpret_cast<uint8_t*>(&a)};
        ARRAY_TEXT t; t.appendUTF16String(native.c_str());
        bool reported = false;
        try { t.toParamAtIndex(args, 1); }
        catch (const std::runtime_error&) { reported = true; }
        check(reported, "null array storage reported before allocating output");
        balance("null array storage rejected output owner balance");
    }
    for (const char* value : values) {
        for (size_t size : {size_t(0), size_t(1), size_t(15), size_t(16), size_t(127)}) {
            std::string bytes;
            for (size_t i = 0; i < size; ++i) bytes += value;
            C_TEXT scalar;
            for (int repeat = 0; repeat < 3; ++repeat) {
                scalar.setUTF8String(reinterpret_cast<const uint8_t*>(bytes.data()), bytes.size());
                CUTF8String roundtrip; scalar.copyUTF8String(&roundtrip);
                size_t units = size * (strlen(value) == 4 ? 2 : strlen(value) ? strlen(value) == 2 ? 1 : 5 : 0);
                check(std::string(reinterpret_cast<const char*>(roundtrip.data()),
                                  roundtrip.size()) == bytes &&
                      scalar.getUTF16Length() == units &&
                      scalar.getUTF16StringPtr()[units] == 0,
                      "C_TEXT UTF8/UTF16 units capacity termination reuse control");
            }
        }
    }
    // All 12 exported commands execute against synthetic in-memory PDF classes.
    for (NSString* value in @[@"", @"ASCII", @"\u00e9", @"\U0001f600"]) {
        fixture = value;
        auto h = blob("PDF");
        int count = 0;
        dispatch(1, {&h}, &count);
        check(count == 2, "dispatcher page count control");
        int page = 1;
        PA_Handle out = nullptr;
        auto label = make(reinterpret_cast<const PA_Unichar*>(u"stale"));
        auto content = make(reinterpret_cast<const PA_Unichar*>(u"stale"));
        dispatch(2, {&h, &page, &out, &label, &content});
        check(strings.at(content.fString) == text(value) && label.fLength == 0,
              "dispatcher page scalar exact content and baseline label behavior");
        PA_DisposeHandle(out); dispose(label); dispose(content);
        for (int cmd : {11, 12}) {
            auto s = make(reinterpret_cast<const PA_Unichar*>(u"stale"));
            auto result = &s;
            dispatch(cmd, {&h}, &result);
            check(strings.at(s.fString) == (cmd == 11 ? text(@"1.7") : text(value)) &&
                  s.fString[s.fLength] == 0, "dispatcher version/document exact UTF16 return");
            dispose(s);
        }
        PA_Variable keys = PA_CreateVariable(eVK_ArrayUnicode);
        PA_Variable vals = PA_CreateVariable(eVK_ArrayUnicode);
        dispatch(9, {&h, &keys, &vals});
        check(keys.uValue.fArray.fNbElements == 6 && vals.uValue.fArray.fNbElements == 6 &&
              Text(PA_GetStringInArray(keys, 1).fString) == text(@"PDF_TITLE") &&
              Text(PA_GetStringInArray(vals, 1).fString) == text(value),
              "dispatcher attributes exact array content");
        dispatch(10, {&h, &keys, &vals});
        check([[observedAttributes objectForKey:PDFDocumentTitleAttribute] isEqualToString:value],
              "dispatcher borrowed input consumers preserve attributes");
        PA_ClearVariable(&keys); PA_ClearVariable(&vals);
        PA_DisposeHandle(h);
        h = blob("PDF");
        PA_Variable arrays[4];
        for (auto& a : arrays) a = PA_CreateVariable(eVK_ArrayUnicode);
        dispatch(7, {&h, &page, &arrays[0], &arrays[1], &arrays[2], &arrays[3]});
        check(arrays[0].uValue.fArray.fNbElements == 7 &&
              Text(PA_GetStringInArray(arrays[3], 1).fString) == text(value) &&
              Text(PA_GetStringInArray(arrays[3], 6).fString) == text(value),
              "dispatcher annotation branches exact array content");
        for (auto& a : arrays) PA_ClearVariable(&a);
        PA_DisposeHandle(h);
        check(boundaryUsed && documentLive == 0, "synthetic document lifetime control");
        balance("dispatcher text and array ownership control");
    }
    for (int cmd : {3, 4, 5, 6}) {
        auto h = blob("PDF"), inserted = blob("PDF");
        int page = 1, other = cmd == 3 ? 90 : 2;
        if (cmd == 3) dispatch(cmd, {&h, &page, &other});
        if (cmd == 4) dispatch(cmd, {&h, &page});
        if (cmd == 5) dispatch(cmd, {&h, &page, &other});
        if (cmd == 6) dispatch(cmd, {&h, &page, &inserted});
        check(PA_GetHandleSize(h) == 3 && memcmp(PA_LockHandle(h), "OUT", 3) == 0,
              "dispatcher modifying command exact blob output");
        PA_UnlockHandle(h);
        PA_DisposeHandle(h); PA_DisposeHandle(inserted);
        balance("dispatcher modifying command owner control");
    }
    auto p = PA_CreatePicture(nullptr, 0);
    PA_Handle result = nullptr;
    dispatch(8, {&p}, &result);
    check(pictureBoundaryUsed && pictures.count(p) && PA_GetHandleSize(result) == 0,
          "dispatcher picture borrowed owner with denied native graphics");
    PA_DisposePicture(p); PA_DisposeHandle(result);
    balance("dispatcher picture owner control");
    check(observedRotation == 90 && documentLive == 0, "dispatcher rotation and lifetime control");
    [observedAttributes release]; observedAttributes = nil;
    [pool drain];
    std::cout << "checks=" << checks << " failures=" << failures << "\n";
    return failures ? 1 : 0;
}
