//%attributes = {}
$filePath:=Get 4D folder:C485(Current resources folder:K5:16)+"test.pdf"
DOCUMENT TO BLOB:C525($filePath;$pdfData)

$version:=PDF Get document version ($pdfData)
$text:=PDF Get document text ($pdfData)

$filePath:=Get 4D folder:C485(Current resources folder:K5:16)+"sample.png"
  //read image file as single pdf page
READ PICTURE FILE:C678($filePath;$image)

$pdfImageData:=PDF Data from picture ($image)

  //insert 1 page
PDF INSERT PAGES ($pdfData;1;$pdfImageData)

$filePath:=System folder:C487(Desktop:K41:16)+"image_test.pdf"
BLOB TO DOCUMENT:C526($filePath;$pdfData)