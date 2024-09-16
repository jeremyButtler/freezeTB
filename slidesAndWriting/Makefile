filters= \
   --filter pandoc-crossref \
   --citeproc

pdf:
	pandoc \
		--from markdown \
		--to pdf \
		$(filters) \
		-o freezeTB-methods.pdf \
		writeUp.md
		#--pdf-engine=xelatex \

docx:
	pandoc \
		--from markdown \
		--to docx \
		$(filters) \
		--reference-doc=template.docx \
		-o freezeTB-methods.docx \
		writeUp.md
