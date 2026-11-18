//%attributes = {}
DOCUMENT TO BLOB:C525(Get 4D folder:C485(Dossier Resources courant:K5:16)+"test.pdf"; $blob)
For ($i; 1; PDF Count pages($blob))
	PDF GET PAGE($blob; $i; $pagePdfData; pageLabel; pageText)
End for 
