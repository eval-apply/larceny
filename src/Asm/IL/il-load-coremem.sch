(parameterize ((recognize-javadot-symbols? #f))
  (load "Asm/Common/link-lop.sch")
  (load "Asm/IL/il-jdot-aliases.sch"))
(parameterize ((recognize-javadot-symbols? #t))
  (load "Asm/IL/il-corememory.sch"))
