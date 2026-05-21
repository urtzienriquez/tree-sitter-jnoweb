{
  "targets": [
    {
      "target_name": "tree_sitter_jnoweb_binding",
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "src"
      ],
      "sources": [
        "bindings/node/binding.cc",
        "src/parser.c",
        "src/scanner.c",
      ],
    }
  ]
}
