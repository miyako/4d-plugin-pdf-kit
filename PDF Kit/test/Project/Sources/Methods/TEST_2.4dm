//%attributes = {}
filePath:=Get 4D folder:C485(Current resources folder:K5:16)+"Facture_EDF.pdf"
DOCUMENT TO BLOB:C525(filePath;pdfData)
PDF SET PAGE ROTATION (pdfData;0;90)

PDF GET PAGE (pdfData;1;$pagePdfData;pageLabel;pageText)
BLOB TO PICTURE:C682($pagePdfData;vImage)

SET PICTURE TO PASTEBOARD:C521(vImage)