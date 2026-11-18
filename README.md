![version](https://img.shields.io/badge/version-20%2B-E23089)
![platform](https://img.shields.io/static/v1?label=platform&message=mac-intel%20|%20mac-arm&color=blue)
[![license](https://img.shields.io/github/license/miyako/4d-plugin-pdf-kit)](LICENSE)
![downloads](https://img.shields.io/github/downloads/miyako/4d-plugin-pdf-kit/total)

4d-plugin-pdf-kit
=================

Perform basic PDF manipulation on macOS. The plugin wraps Apple's PDFKit
framework: you hand it a PDF as a 4D `Blob` and get back a `Blob`, `Text`,
`Longint`, or text arrays — no files, no windows, no UI.

| Command | Returns | Purpose |
|---|---|---|
| [PDF Count pages](#pdf-count-pages) | Longint | Number of pages in a PDF |
| [PDF GET PAGE](#pdf-get-page) | — | Extract one page as a single-page PDF blob, plus its label and text |
| [PDF SET PAGE ROTATION](#pdf-set-page-rotation) | — | Rotate one page, or all pages |
| [PDF REMOVE PAGE](#pdf-remove-page) | — | Delete one page |
| [PDF EXCHANGE PAGES](#pdf-exchange-pages) | — | Swap two pages |
| [PDF INSERT PAGES](#pdf-insert-pages) | — | Insert every page of one PDF into another |
| [PDF GET PAGE ANNOTATION](#pdf-get-page-annotation) | — | Read a page's annotations as four text arrays |
| [PDF Data from picture](#pdf-data-from-picture) | Blob | Wrap a picture into a single-page PDF |
| [PDF GET DOCUMENT ATTRIBUTES](#pdf-get-document-attributes) | — | Read document metadata as key/value text arrays |
| [PDF SET DOCUMENT ATTRIBUTES](#pdf-set-document-attributes) | — | Write document metadata from key/value text arrays |
| [PDF Get document version](#pdf-get-document-version) | Text | PDF version string such as `1.4` |
| [PDF Get document text](#pdf-get-document-text) | Text | All extractable text of the document |

**Platforms:** macOS only (PDFKit). The plugin is built universal
(arm64 + x86_64) for macOS 11.5 and later.

---

## Requirements & platform notes

- **macOS 11.5 or later.** The implementation uses PDFKit and Cocoa; there is
  no Windows implementation in this version.
- **Everything is a `Blob`.** Commands that modify a PDF write the result back
  into the *same* blob variable you passed in. Keep the original if you need it.
- **Page numbers are 1-based.** `0` and out-of-range values are handled
  per-command (see each section); they never raise a 4D error.
- **Failures are silent.** An unreadable blob, a missing page, or an
  unsupported annotation produces an empty result or an unchanged blob —
  not a 4D error. Check outputs before trusting them (see
  [Error handling & troubleshooting](#error-handling--troubleshooting)).

---

## PDF Count pages

### Syntax

```4d
PDF Count pages ( pdfData ) → pageCount
```

### Parameters

| Parameter | Type | Description |
|---|---|---|
| `pdfData` | Blob | PDF document data |
| Result | Longint | Number of pages; `0` if the blob is not a readable PDF |

### Description

Opens the blob as a PDFDocument and returns its page count. The blob is not
modified.

### Example

From the plugin's own test method (`TEST_3.4dm`):

```4d
$filePath:=Get 4D folder(Current resources folder:K5:16)+"test.pdf"
DOCUMENT TO BLOB($filePath; $pdfData)
PDF REMOVE PAGE($pdfData; 1)
$countPages:=PDF Count pages($pdfData)
PDF REMOVE PAGE($pdfData; $countPages)
```

---

## PDF GET PAGE

### Syntax

```4d
PDF GET PAGE ( pdfData ; pageNumber ; pageData ; pageLabel ; pageText )
```

### Parameters

| Parameter | Type | Description |
|---|---|---|
| `pdfData` | Blob | PDF document data (not modified) |
| `pageNumber` | Longint | 1-based page number |
| `pageData` | Blob | Receives the page re-serialized as a single-page PDF |
| `pageLabel` | Text | Receives the page's label (e.g. `1`, `iv`, `3.1`) |
| `pageText` | Text | Receives the page's extractable text |

### Description

Extracts one page. `pageData` is a complete single-page PDF — feed it back
into `PDF INSERT PAGES` or `BLOB TO PICTURE`/`PDF Data from picture` flows.
`pageLabel` and `pageText` are empty when the page has no label or no
extractable text. Out-of-range `pageNumber` leaves all three outputs empty.

*New in this version:* `pageLabel` is now populated. Earlier builds always
returned an empty label.

### Example

From the plugin's own test method (`TEST_3.4dm`):

```4d
$filePath:=Get 4D folder(Current resources folder:K5:16)+"test.pdf"
DOCUMENT TO BLOB:C525($filePath; $pdfData)
PDF GET PAGE($pdfData; 1; $pageData; $pageLabel; $pageContent)
//insert 1 page
PDF INSERT PAGES($pdfData; 1; $pageData)
//insert many pages
PDF INSERT PAGES($pdfData; 11; $pdfData)
```

---

## PDF SET PAGE ROTATION

### Syntax

```4d
PDF SET PAGE ROTATION ( pdfData ; pageNumber ; rotation )
```

### Parameters

| Parameter | Type | Description |
|---|---|---|
| `pdfData` | Blob | PDF document data, rewritten in place |
| `pageNumber` | Longint | 1-based page number; `0` rotates **all** pages |
| `rotation` | Longint | Rotation in degrees; `0`, `90`, `180`, `270` are the meaningful values (other values are passed to PDFKit unchanged) |

### Description

Sets the rotation of one page (`pageNumber > 0`) or of every page
(`pageNumber = 0`). The modified document is written back into `pdfData`.
Other `pageNumber` values do nothing.

### Example

From the plugin's own test method (`TEST_3.4dm`):

```4d
$filePath:=Get 4D folder(Current resources folder:K5:16)+"test.pdf"
DOCUMENT TO BLOB:C525($filePath; $pdfData)
//new: passing 0 for page rotates all pages
PDF SET PAGE ROTATION($pdfData; 0; 270)
$filePath:=System folder:C487(Desktop:K41:16)+"rotation_test.pdf"
BLOB TO DOCUMENT:C526($filePath; $pdfData)
```

---

## PDF REMOVE PAGE

### Syntax

```4d
PDF REMOVE PAGE ( pdfData ; pageNumber )
```

### Parameters

| Parameter | Type | Description |
|---|---|---|
| `pdfData` | Blob | PDF document data, rewritten in place |
| `pageNumber` | Longint | 1-based page number to delete |

### Description

Deletes one page and writes the document back into `pdfData`. An
out-of-range `pageNumber` leaves the blob unchanged.

### Example

From the plugin's own test method (`TEST_3.4dm`):

```4d
$filePath:=Get 4D folder(Current resources folder:K5:16)+"test.pdf"
DOCUMENT TO BLOB:C525($filePath; $pdfData)
PDF REMOVE PAGE($pdfData; 1)
$countPages:=PDF Count pages($pdfData)
PDF REMOVE PAGE($pdfData; $countPages)
$countPages:=PDF Count pages($pdfData)
$filePath:=System folder:C487(Desktop:K41:16)+"remove_test.pdf"
BLOB TO DOCUMENT:C526($filePath; $pdfData)
```

---

## PDF EXCHANGE PAGES

### Syntax

```4d
PDF EXCHANGE PAGES ( pdfData ; pageNumberA ; pageNumberB )
```

### Parameters

| Parameter | Type | Description |
|---|---|---|
| `pdfData` | Blob | PDF document data, rewritten in place |
| `pageNumberA` | Longint | 1-based page number |
| `pageNumberB` | Longint | 1-based page number |

### Description

Swaps two pages. Both numbers must be different, in range, and `>= 1`;
anything else leaves the blob unchanged. *Fixed in this version:* page
number `0` used to crash the plugin.

### Example

From the plugin's own test method (`TEST_7.4dm`):

```4d
$filePath:=Get 4D folder(Current resources folder:K5:16)+"test.pdf"
DOCUMENT TO BLOB:C525($filePath;$pdfData)

For ($i;1;10)
	PDF EXCHANGE PAGES ($pdfData;1;2)
	$text:=PDF Get document text ($pdfData)
	$version:=PDF Get document version ($pdfData)
	$count:=PDF Count pages ($pdfData)
End for 

$filePath:=System folder:C487(Desktop:K41:16)+"exchange_test.pdf"
BLOB TO DOCUMENT:C526($filePath;$pdfData)
```

---

## PDF INSERT PAGES

### Syntax

```4d
PDF INSERT PAGES ( pdfData ; pageNumber ; pdfToInsert )
```

### Parameters

| Parameter | Type | Description |
|---|---|---|
| `pdfData` | Blob | PDF document data, rewritten in place |
| `pageNumber` | Longint | 1-based position; the inserted pages land *before* this page |
| `pdfToInsert` | Blob | PDF document whose **all** pages are inserted |

### Description

Inserts every page of `pdfToInsert` into `pdfData` before `pageNumber`,
then writes the result back into `pdfData`. `pdfToInsert` can be the same
blob as `pdfData` (the document is duplicated at that position). There is
no append-after-the-last-page form: `pageNumber` must be `>= 1` and
`<=` the current page count. An invalid `pdfToInsert` or out-of-range
`pageNumber` leaves `pdfData` unchanged.

### Example

From the plugin's own test method (`TEST_3.4dm`):

```4d
PDF GET PAGE($pdfData; 1; $pageData; $pageLabel; $pageContent)
//insert 1 page
PDF INSERT PAGES($pdfData; 1; $pageData)
//insert many pages
PDF INSERT PAGES($pdfData; 11; $pdfData)
```

---

## PDF GET PAGE ANNOTATION

### Syntax

```4d
PDF GET PAGE ANNOTATION ( pdfData ; pageNumber ; types ; coordinates ; colors ; contents )
```

### Parameters

| Parameter | Type | Description |
|---|---|---|
| `pdfData` | Blob | PDF document data (not modified) |
| `pageNumber` | Longint | 1-based page number |
| `types` | Text array | Receives each annotation's type (`"Ink"`, `"Text"`, `"Line"`, …) |
| `coordinates` | Text array | Receives each annotation's bounds as `x y width height` |
| `colors` | Text array | Receives each annotation's color as an `NSColor` description string |
| `contents` | Text array | Receives annotation content — format depends on `types` (see below) |

### Description

Reads every annotation on one page into four parallel text arrays, one
element per annotation, in page order. The four arrays stay aligned even
when an annotation has no color or content (an empty string is emitted).

`contents` format by annotation type:

- `Ink` — SVG-style path string (`M x y L x y C … Z`)
- `Line` — `startX startY endX endY`
- `Text`, `FreeText` — the annotation's contents string
- `Stamp` — the stamp's name
- `Link` — the absolute URL, or `pageLabel (x y)` of the destination
- anything else — empty string

An out-of-range `pageNumber` leaves all four arrays empty.

### Example

From the plugin's own test method (`TEST_5.4dm`):

```4d
$filePath:=Get 4D folder(Current resources folder:K5:16)+"sample_annotation.pdf"
DOCUMENT TO BLOB:C525($filePath;$pdfData)

  //if type=Ink; contents=SVG style path string
  //if type=Line; contents=x,y width,height
  //if type=Text|FreeText; contents=contents
  //if type=Stamp; contents=name
  //if type=Link; contents=URL|page label (x,y)

ARRAY TEXT:C222($types;0)
ARRAY TEXT:C222($coordinates;0)
ARRAY TEXT:C222($colors;0)
ARRAY TEXT:C222($contents;0)

PDF GET PAGE ANNOTATION ($pdfData;1;$types;$coordinates;$colors;$contents)
```

---

## PDF Data from picture

### Syntax

```4d
PDF Data from picture ( picture ) → pdfData
```

### Parameters

| Parameter | Type | Description |
|---|---|---|
| `picture` | Picture | Source image |
| Result | Blob | Single-page PDF containing the picture |

### Description

Wraps a 4D picture into a one-page PDF document and returns it as a blob.
The page size matches the image. Returns an empty blob if the picture
cannot be converted.

### Example

From the plugin's own test method (`TEST_1.4dm`):

```4d
$filePath:=Get 4D folder(Current resources folder:K5:16)+"sample.png"
  //read image file as single pdf page
READ PICTURE FILE:C678($filePath;$image)

$pdfImageData:=PDF Data from picture ($image)
```

---

## PDF GET DOCUMENT ATTRIBUTES

### Syntax

```4d
PDF GET DOCUMENT ATTRIBUTES ( pdfData ; keys ; values )
```

### Parameters

| Parameter | Type | Description |
|---|---|---|
| `pdfData` | Blob | PDF document data (not modified) |
| `keys` | Text array | Receives attribute names (see table below) |
| `values` | Text array | Receives attribute values, parallel to `keys` |

### Description

Reads the document's metadata dictionary. Only attributes that are present
are returned; `keys` and `values` always have the same size. Recognized
keys:

| Key | Content |
|---|---|
| `PDF_TITLE` | Title |
| `PDF_AUTHOR` | Author |
| `PDF_SUBJECT` | Subject |
| `PDF_CREATOR` | Creator application |
| `PDF_PRODUCER` | Producer application |
| `PDF_KEYWORDS` | Keywords joined with `,` |
| `PDF_CREATION_DATE` | Creation date as text |
| `PDF_MODIFICATION_DATE` | Modification date as text |

The date values use the textual `NSDate` description format; they can be
fed back to `PDF SET DOCUMENT ATTRIBUTES` unchanged.

### Example

From the plugin's own test method (`TEST_4.4dm`):

```4d
$filePath:=Get 4D folder(Current resources folder:K5:16)+"test.pdf"
DOCUMENT TO BLOB:C525($filePath;$pdfData)

ARRAY TEXT:C222($keys;0)
ARRAY TEXT:C222($values;0)

PDF GET DOCUMENT ATTRIBUTES ($pdfData;$keys;$values)
```

---

## PDF SET DOCUMENT ATTRIBUTES

### Syntax

```4d
PDF SET DOCUMENT ATTRIBUTES ( pdfData ; keys ; values )
```

### Parameters

| Parameter | Type | Description |
|---|---|---|
| `pdfData` | Blob | PDF document data, rewritten in place |
| `keys` | Text array | Attribute names — the same constants `PDF GET DOCUMENT ATTRIBUTES` returns |
| `values` | Text array | New values, parallel to `keys` |

### Description

Merges the given key/value pairs into the document's metadata and writes
the document back into `pdfData`. Only the eight keys listed under
`PDF GET DOCUMENT ATTRIBUTES` are recognized; unknown keys are ignored.
If `keys` and `values` have different sizes, nothing is applied.

- `PDF_KEYWORDS` — split on `,` into the keyword list.
- `PDF_CREATION_DATE`, `PDF_MODIFICATION_DATE` — the value is parsed as a
  date; a value that does not parse clears the attribute instead.
- All other keys are stored as text.

*Fixed in this version:* documents with no existing metadata dictionary no
longer crash the command.

### Example

From the plugin's own test method (`TEST_4.4dm`):

```4d
  //modifiable attributes; original value will be used if not specified
ARRAY TEXT:C222($keys;5)
ARRAY TEXT:C222($values;5)

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

---

## PDF Get document version

### Syntax

```4d
PDF Get document version ( pdfData ) → version
```

### Parameters

| Parameter | Type | Description |
|---|---|---|
| `pdfData` | Blob | PDF document data (not modified) |
| Result | Text | Version as `major.minor` (e.g. `1.4`); empty if unreadable |

### Description

Returns the PDF header version reported by PDFKit.

### Example

From the plugin's own test method (`TEST_1.4dm`):

```4d
$version:=PDF Get document version ($pdfData)
```

---

## PDF Get document text

### Syntax

```4d
PDF Get document text ( pdfData ) → text
```

### Parameters

| Parameter | Type | Description |
|---|---|---|
| `pdfData` | Blob | PDF document data (not modified) |
| Result | Text | All text PDFKit can extract; empty for scanned/image-only PDFs |

### Description

Returns the document's extractable text in reading order. PDFs that are
pure scans or images return an empty string — there is no OCR.

### Example

From the plugin's own test method (`TEST_1.4dm`):

```4d
$text:=PDF Get document text ($pdfData)
```

---

## Error handling & troubleshooting

- **Failures are silent, not 4D errors.** An invalid blob, a missing page,
  or an unreadable PDF produces `0`, an empty string/blob, or an unchanged
  blob — the command never raises. Verify `PDF Count pages` or the outputs
  before using a result.
- **macOS only.** The commands are implemented with PDFKit; there is no
  supported Windows build in this version.
- **Keep the original blob.** Modifying commands overwrite `pdfData` in
  place. Copy the blob first if you may need the original.
- **Page numbers are 1-based.** `0` is out of range everywhere except
  `PDF SET PAGE ROTATION`, where `0` means "all pages".
- **Insertion has no append mode.** `PDF INSERT PAGES` requires
  `pageNumber <=` page count, so pages always land *before* an existing
  page.
- **Annotation content is best-effort.** Only the annotation types listed
  above produce meaningful `contents`; everything else yields an empty
  string, and `colors` is an `NSColor` description, not a hex value.
- **Big documents are heavy.** Each command loads the whole PDF into
  memory and modifying commands re-serialize it. Very large files use
  substantial RAM and can take noticeable time.
- **Dates round-trip as text.** `PDF_GET_DOCUMENT_ATTRIBUTES` returns
  dates in `NSDate` description format; pass the same string back to
  `PDF SET DOCUMENT ATTRIBUTES` to preserve them.

---

## Quick reference

```4d
// load once, reuse the blob
DOCUMENT TO BLOB(path;$pdf)

PDF Count pages($pdf)                          // -> Longint
PDF Get document version($pdf)                 // -> "1.7"
PDF Get document text($pdf)                    // -> Text

PDF GET PAGE($pdf;1;$pageBlob;$label;$text)    // one page out
PDF INSERT PAGES($pdf;1;$pageBlob)             // ...or back in

PDF SET PAGE ROTATION($pdf;0;90)               // 0 = all pages
PDF EXCHANGE PAGES($pdf;1;2)
PDF REMOVE PAGE($pdf;1)

PDF GET PAGE ANNOTATION($pdf;1;$t;$c;$col;$x)  // 4 text arrays

PDF GET DOCUMENT ATTRIBUTES($pdf;$k;$v)
PDF SET DOCUMENT ATTRIBUTES($pdf;$k;$v)        // same arrays back

READ PICTURE FILE(path;$img)
$pdfPage:=PDF Data from picture($img)          // picture -> PDF blob
```

### Ownership regression

The [synthetic ownership audit](tests/ownership/README.md) documents bundled
SDK contracts, reachability, baseline/fixed evidence and platform limitations.
Run `python3 tests/ownership/run.py` on macOS to validate actual sources without
invoking the signing, packaging or notarization project phases.
