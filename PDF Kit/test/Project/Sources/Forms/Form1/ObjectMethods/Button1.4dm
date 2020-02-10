PDF SET PAGE ROTATION (pdfData;0;90)
BLOB TO DOCUMENT:C526(filePath;pdfData)
PDF GET PAGE (pdfData;1;$pagePdfData;pageLabel;pageText)
BLOB TO PICTURE:C682($pagePdfData;vImage)