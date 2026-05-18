; Engine → inject engine language
(jchunk
  (jnv_sig_options
    (jnv_engine engine: (jnv_engine_key) @injection.language))
  (jnv_content) @injection.content)

; Chunk with options/label but no engine → Julia
(jchunk
  (jnv_sig_options !engine)
  (jnv_content) @injection.content
  (#set! injection.language "julia")
  (#set! injection.combined))

; Chunk with no options at all → Julia
(jchunk
  (jnv_content) @injection.content
  (#set! injection.language "julia")
  (#set! injection.combined))

; Inline \Sexpr{} → Julia
(jinline
  (jnv_content) @injection.content
  (#set! injection.language "julia")
  (#set! injection.combined))

; Everything else → LaTeX
((latex) @injection.content
  (#set! injection.language "latex")
  (#set! injection.combined))
