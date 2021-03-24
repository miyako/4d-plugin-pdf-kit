![version](https://img.shields.io/badge/version-16%2B-8331AE)
![platform](https://img.shields.io/static/v1?label=platform&message=mac-intel%20|%20mac-arm&color=blue)
[![license](https://img.shields.io/github/license/miyako/4d-plugin-pdf-kit)](LICENSE)
![downloads](https://img.shields.io/github/downloads/miyako/4d-plugin-pdf-kit/total)

**Note**: for v17 and earlier, move `manifest.json` to `Contents`

4d-plugin-pdf-kit
=================

Perform basic PDF manipulation on OS X.

* ``PDF INSERT PAGES`` is rewritten to internally duplicate trailing pages before inserting new pages then finally remove the duplicate pages.  

* ``PDF REMOVE PAGE`` is rewritten to internally exchange the target page before removing it. 

## New in 1.4

* Removed the "retain" patch for earlier El Capitan crash problem. 
* ``PDF REMOVE PAGE`` is restored to use original code.

**Note**: Most page commands (insert, remove, exchange) will not work if a page is rotated. The plugin is not using PDF Kit to simply rotate a page, but rather, calling CGPDF to re-draw such pages on a rotated context. You should rotate the page at the very end.

## New in 1.2

Passing 0 to ```PDF SET PAGE ROTATION``` will rotate all pages.

Fixed El Capitan issue for ```PDF SET PAGE ROTATION``` and ```PDF REMOVE PAGE```. 

##Commands

```c
// --- Page
PDF Count pages
PDF GET PAGE
PDF SET PAGE ROTATION
PDF REMOVE PAGE
PDF EXCHANGE PAGES
PDF INSERT PAGES
PDF GET PAGE ANNOTATION

//El Capitan
PDF SET PAGE ROTATION2
PDF REMOVE PAGE2

// --- Picture
PDF Data from picture

// --- Document
PDF GET DOCUMENT ATTRIBUTES
PDF SET DOCUMENT ATTRIBUTES
PDF Get document version
PDF Get document text
```

Examples
---

* Document

```
$filePath:=Get 4D folder(Current resources folder)+"sample.pdf"
DOCUMENT TO BLOB($filePath;$pdfData)

$version:=PDF Get ducument version ($pdfData)
$text:=PDF Get document text ($pdfData)

$filePath:=Get 4D folder(Current resources folder)+"sample.png"
  //read image file as single pdf page
READ PICTURE FILE($filePath;$image)

$pdfImageData:=PDF Data from picture ($image)

  //insert 1 page
PDF INSERT PAGES ($pdfData;1;$pdfImageData)

$filePath:=System folder(Desktop)+"image_test.pdf"
BLOB TO DOCUMENT($filePath;$pdfData)
```

* pages

```
$filePath:=Get 4D folder(Current resources folder)+"sample.pdf"
DOCUMENT TO BLOB($filePath;$pdfData)

$countPages:=PDF Count pages ($pdfData)

PDF REMOVE PAGE ($pdfData;1)
$countPages:=PDF Count pages ($pdfData)
PDF REMOVE PAGE ($pdfData;$countPages)

$filePath:=System folder(Desktop)+"remove_test.pdf"
BLOB TO DOCUMENT($filePath;$pdfData)

$filePath:=Get 4D folder(Current resources folder)+"sample.pdf"
DOCUMENT TO BLOB($filePath;$pdfData)

PDF EXCHANGE PAGES ($pdfData;1;2)

$filePath:=System folder(Desktop)+"exchange_test.pdf"
BLOB TO DOCUMENT($filePath;$pdfData)

$filePath:=Get 4D folder(Current resources folder)+"sample.pdf"
DOCUMENT TO BLOB($filePath;$pdfData)

PDF SET PAGE ROTATION ($pdfData;1;90)

$filePath:=System folder(Desktop)+"rotation_test.pdf"
BLOB TO DOCUMENT($filePath;$pdfData)

$filePath:=Get 4D folder(Current resources folder)+"sample.pdf"
DOCUMENT TO BLOB($filePath;$pdfData)

PDF GET PAGE ($pdfData;1;$pageData;$pageLabel;$pageContent)

  //insert 1 page
PDF INSERT PAGES ($pdfData;1;$pageData)

  //insert many pages
PDF INSERT PAGES ($pdfData;11;$pdfData)

$filePath:=System folder(Desktop)+"insert_test.pdf"
BLOB TO DOCUMENT($filePath;$pdfData)
```

* atrributes

```
$filePath:=Get 4D folder(Current resources folder)+"sample.pdf"
DOCUMENT TO BLOB($filePath;$pdfData)

ARRAY TEXT($keys;0)
ARRAY TEXT($values;0)

PDF GET DOCUMENT ATTRIBUTES ($pdfData;$keys;$values)

  //modifiable attributes; original value will be used if not specified
ARRAY TEXT($keys;5)
ARRAY TEXT($values;5)

$keys{1}:=PDF_AUTHOR
$values{1}:="miyako"

$keys{2}:=PDF_CREATOR
$values{2}:="keisuke"

$keys{3}:=PDF_KEYWORDS
$values{3}:="4D,keisuke,miyako"  //comma separated words

$keys{4}:=PDF_SUBJECT
$values{4}:="sample"

$keys{5}:=PDF_TITLE
$values{5}:="sample"

If (False)
  //these attributes are set automatically, can't be modified by code
$keys{0}:=PDF_PRODUCER
$keys{0}:=PDF_CREATION_DATE
$keys{0}:=PDF_MODIFICATION_DATE
End if 

PDF SET DOCUMENT ATTRIBUTES ($pdfData;$keys;$values)
```

* annotation

```
$filePath:=Get 4D folder(Current resources folder)+"sample_annotation.pdf"
DOCUMENT TO BLOB($filePath;$pdfData)

  //if type=Ink; contents=SVG style path string
  //if type=Line; contents=x,y width,height
  //if type=Text|FreeText; contents=contents
  //if type=Stamp; contents=name
  //if type=Link; contents=URL|page label (x,y)

ARRAY TEXT($types;0)
ARRAY TEXT($coordinates;0)
ARRAY TEXT($colors;0)
ARRAY TEXT($contents;0)

PDF GET PAGE ANNOTATION ($pdfData;1;$types;$coordinates;$colors;$contents)
```
