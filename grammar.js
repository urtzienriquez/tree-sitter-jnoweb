const sepBy1 = (rule, sep) => seq(rule, repeat(seq(sep, rule)));

module.exports = grammar({
    name: 'jnoweb',

    externals: $ => [
        $._latex_word,
        $.command_name,
        $.jnv_sig_beg,
        $.jnv_sig_end,
        $.jnv_content,
    ],

    rules: {
        source_file: $ =>
            repeat(
                choice(
                    $.latex,
                    $.jchunk,
                    $.jinline,
                )
            ),

        jchunk: $ =>
            seq(
                $.jnv_sig_beg,
                optional($.jnv_sig_options),
                $.jnv_sig_end,
                optional($.jnv_content),
                '@'
            ),

        jinline: $ =>
            seq(
                field("Sexpr", $.command_name),
                "{",
                optional(alias(/[^}]+/, $.jnv_content)),
                '}',
            ),

        jnv_sig_options: $ =>
            choice(
                field("options", $._jnv_options),
                seq(
                    field("label", alias($.jnv_key, $.jnv_label)),
                    optional(seq(",", field("options", $._jnv_options)))
                ),
            ),

        _jnv_options: $ =>
            sepBy1(choice(
                $.jnv_key_pair,
                prec(2, $._jnv_engine)
            ), ","),

        _jnv_engine: $ =>
            seq(
                "engine",
                "=",
                alias($.jnv_engine_val, $.jnv_engine)
            ),

        jnv_engine_val: $ =>
            choice(
                seq(
                    "\"",
                    field("engine", $.jnv_engine_key),
                    "\""
                ),
                seq(
                    "'",
                    field("engine", $.jnv_engine_key),
                    "'"
                )
            ),

        jnv_key_pair: $ =>
            seq(
                field("key", $.jnv_key),
                /=[ ]*/,
                field("value", $.jnv_val)
            ),

        jnv_engine_key: $ =>
            /[a-zA-Z]+[a-zA-Z0-9]*/,

        jnv_key: $ =>
            /[a-zA-Z_]+[a-zA-Z0-9_.]*/,

        jnv_val: $ =>
            /[^,>]+/,

        latex: $ =>
            prec.right(repeat1(
                $._latex_word,
            )),
    },
});
