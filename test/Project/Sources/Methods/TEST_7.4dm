//%attributes = {}
$filePath:=Get 4D folder:C485(Current resources folder:K5:16)+"test.pdf"
DOCUMENT TO BLOB:C525($filePath;$pdfData)

For ($i;1;10)
	PDF EXCHANGE PAGES ($pdfData;1;2)
	$text:=PDF Get document text ($pdfData)
	$version:=PDF Get document version ($pdfData)
	$count:=PDF Count pages ($pdfData)
End for 

$filePath:=System folder:C487(Desktop:K41:16)+"exchange_test.pdf"
BLOB TO DOCUMENT:C526($filePath;$pdfData)
