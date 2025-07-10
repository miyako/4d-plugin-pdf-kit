//%attributes = {}
$filePath:=Get 4D folder:C485(Current resources folder:K5:16)+"test.pdf"
DOCUMENT TO BLOB:C525($filePath; $pdfData)
//new: passing 0 for page rotates all pages
PDF SET PAGE ROTATION($pdfData; 0; 270)
$filePath:=System folder:C487(Desktop:K41:16)+"rotation_test.pdf"
BLOB TO DOCUMENT:C526($filePath; $pdfData)

$filePath:=Get 4D folder:C485(Current resources folder:K5:16)+"test.pdf"
DOCUMENT TO BLOB:C525($filePath; $pdfData)
PDF REMOVE PAGE($pdfData; 1)
$countPages:=PDF Count pages($pdfData)
PDF REMOVE PAGE($pdfData; $countPages)
$countPages:=PDF Count pages($pdfData)
$filePath:=System folder:C487(Desktop:K41:16)+"remove_test.pdf"
BLOB TO DOCUMENT:C526($filePath; $pdfData)

$filePath:=Get 4D folder:C485(Current resources folder:K5:16)+"test.pdf"
DOCUMENT TO BLOB:C525($filePath; $pdfData)
PDF EXCHANGE PAGES($pdfData; 1; 2)
$filePath:=System folder:C487(Desktop:K41:16)+"exchange_test.pdf"
BLOB TO DOCUMENT:C526($filePath; $pdfData)

$filePath:=Get 4D folder:C485(Current resources folder:K5:16)+"test.pdf"
DOCUMENT TO BLOB:C525($filePath; $pdfData)
PDF GET PAGE($pdfData; 1; $pageData; $pageLabel; $pageContent)
//insert 1 page
PDF INSERT PAGES($pdfData; 1; $pageData)
//insert many pages
PDF INSERT PAGES($pdfData; 11; $pdfData)
$filePath:=System folder:C487(Desktop:K41:16)+"insert_test.pdf"
BLOB TO DOCUMENT:C526($filePath; $pdfData)