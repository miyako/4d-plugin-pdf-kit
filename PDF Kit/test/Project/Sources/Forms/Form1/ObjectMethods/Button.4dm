filePath:=Get 4D folder:C485(Current resources folder:K5:16)+"Facture_EDF.pdf"
DOCUMENT TO BLOB:C525(filePath;pdfData)
BLOB TO PICTURE:C682(pdfData;vImage)
