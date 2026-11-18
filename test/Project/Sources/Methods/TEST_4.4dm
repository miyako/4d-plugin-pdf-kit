//%attributes = {}
$filePath:=Get 4D folder:C485(Current resources folder:K5:16)+"test.pdf"
DOCUMENT TO BLOB:C525($filePath;$pdfData)

ARRAY TEXT:C222($keys;0)
ARRAY TEXT:C222($values;0)

PDF GET DOCUMENT ATTRIBUTES ($pdfData;$keys;$values)

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

If (False:C215)
	  //these attributes are set automatically, can't be modified by code
	$keys{0}:=PDF_PRODUCER
	$keys{0}:=PDF_CREATION_DATE
	$keys{0}:=PDF_MODIFICATION_DATE
End if 

PDF SET DOCUMENT ATTRIBUTES ($pdfData;$keys;$values)
