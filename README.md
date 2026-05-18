# tree-sitter-jnoweb

[tree-sitter](https://tree-sitter.github.io) grammar for **jnoweb** (Julia NoWeb) files — a lightweight literate programming format for Julia documents.

Inspired by [tree-sitter-rnoweb](https://github.com/tree-sitter-grammars/tree-sitter-rnoweb) and [knitr](https://yihui.org/knitr/).

## File type

`.jnw` files contain LaTeX prose interleaved with Julia code chunks:

```latex
\documentclass{article}
\begin{document}

<<chunk-label, echo=true>>=
x = randn(100)
println(mean(x))
@

The mean is \Sexpr{round(mean(x), digits=3)}.

\end{document}
```

## Integration

### Neovim

Add the parser to `~/.config/nvim/lua/pack.lua` or equivalent:

```lua
vim.treesitter.language.add("jnoweb")
require("nvim-treesitter.parsers").jnoweb = {
  install_info = {
    url = "~/path/to/tree-sitter-jnoweb",
    files = { "src/parser.c", "src/scanner.c" },
    requires_generate_from_grammar = false,
  },
  filetype = "jnoweb",
}
```

Query files (`highlights.scm`, `injections.scm`) are bundled under `queries/` and can be symlinked:

```bash
ln -s ~/path/to/tree-sitter-jnoweb/queries/highlights.scm ~/.config/nvim/queries/jnoweb/highlights.scm
ln -s ~/path/to/tree-sitter-jnoweb/queries/injections.scm ~/.config/nvim/queries/jnoweb/injections.scm
```
