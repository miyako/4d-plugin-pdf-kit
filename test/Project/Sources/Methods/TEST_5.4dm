//%attributes = {}
$filePath:=Get 4D folder:C485(Current resources folder:K5:16)+"sample_annotation.pdf"
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
