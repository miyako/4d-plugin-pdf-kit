//%attributes = {}
$filePath:=Get 4D folder:C485(Current resources folder:K5:16)+"2p.pdf"
DOCUMENT TO BLOB:C525($filePath;$pdfData)

PDF REMOVE PAGE ($pdfData;1)

$filePath:=System folder:C487(Desktop:K41:16)+"remove_test.pdf"
BLOB TO DOCUMENT:C526($filePath;$pdfData)

OPEN URL:C673($filePath)