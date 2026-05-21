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

## Neovim setup

### 1. Symlink query files

```bash
mkdir -p ~/.config/nvim/queries/jnoweb/
ln -sf ~/path/to/tree-sitter-jnoweb/queries/highlights.scm ~/.config/nvim/queries/jnoweb/highlights.scm
ln -sf ~/path/to/tree-sitter-jnoweb/queries/injections.scm ~/.config/nvim/queries/jnoweb/injections.scm
```

Replace `~/path/to/tree-sitter-jnoweb` with the actual path to this repo.

### 2. Compile the parser

```bash
mkdir -p ~/.local/share/nvim/site/parser/
gcc -O2 -shared \
  -I src \
  src/parser.c src/scanner.c \
  -o ~/.local/share/nvim/site/parser/jnoweb.so
```

### 3. Configure Neovim

Add to your `init.lua`:

```lua
vim.filetype.add({
  extension = { jnw = "jnoweb" },
})

vim.treesitter.language.add("jnoweb", {
  path = vim.fn.expand("~/.local/share/nvim/site/parser/jnoweb.so"),
})
```

### 4. Verify

Open a `.jnw` file and run `:InspectTree`. You should see proper syntax nodes with no red `ERROR` tags.

## Installation from GitHub release

Download `tree-sitter-jnoweb.wasm` or `tree-sitter-jnoweb.tar.gz` from the [latest release](https://github.com/urtzienriquez/tree-sitter-jnoweb/releases).

## Queries

- `highlights.scm` — syntax highlighting for chunk delimiters, options, and `\Sexpr{}`
- `injections.scm` — injects Julia into code chunks and `\Sexpr{}`, LaTeX into prose

## Development

```bash
tree-sitter generate    # regenerate parser.c from grammar.js
tree-sitter test        # run corpus tests
tree-sitter build --wasm  # build WASM binary
```

To recompile the `.so` for local testing after changing the grammar:

```bash
tree-sitter generate
gcc -O2 -shared -I src src/parser.c src/scanner.c -o ~/.local/share/nvim/site/parser/jnoweb.so
```

## License

GPL-3.0-or-later
