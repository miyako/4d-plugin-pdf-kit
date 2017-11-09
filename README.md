4d-plugin-pdf-kit
=================

Perform basic PDF manipulation on OS X.

### Platform

| carbon | cocoa | win32 | win64 |
|:------:|:-----:|:---------:|:---------:|
|<img src="https://cloud.githubusercontent.com/assets/1725068/22371562/1b091f0a-e4db-11e6-8458-8653954a7cce.png" width="24" height="24" />|<img src="https://cloud.githubusercontent.com/assets/1725068/22371562/1b091f0a-e4db-11e6-8458-8653954a7cce.png" width="24" height="24" />|||

### Version

<img src="https://cloud.githubusercontent.com/assets/1725068/18940649/21945000-8645-11e6-86ed-4a0f800e5a73.png" width="32" height="32" /> <img src="https://cloud.githubusercontent.com/assets/1725068/18940648/2192ddba-8645-11e6-864d-6d5692d55717.png" width="32" height="32" />

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
