#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tree_sitter/parser.h>

#define ESN 4
#define SEXPR 6

enum TokenType {
	_LATEX_WORD,
	JNV_INLINE,
	JNV_SIG_BEG,
	JNV_SIG_END,
	JNV_CONTENT,
	LATEX_COMMENT,
};

static bool prev_was_ws = false;

static bool ws(int32_t val)
{
	return(val == ' ' || val == '\t' || val == '\n' || val == '\r');
}

static void advance_ws(TSLexer* lexer)
{
	bool consumed = false;
	while (ws(lexer->lookahead))  { lexer->advance(lexer, true); consumed = true; }
	prev_was_ws = consumed;
};

static bool jnv_content(TSLexer* lexer)
{
	bool eof = lexer->eof(lexer);
	if (eof) return(false);

	int32_t col = lexer->get_column(lexer);
	int32_t val = lexer->lookahead;

	while (
		((lexer->get_column(lexer) != 0) || (lexer->lookahead != '@')) &&
		(! lexer->eof(lexer))
		) {
		lexer->mark_end(lexer);
		lexer->advance(lexer, false);
	}

	lexer->result_symbol = JNV_CONTENT;
	return(true);
};

static bool jnv_sig_end(TSLexer* lexer)
{
	if (lexer->eof(lexer)) return(false);

	// Check for >>=
	char tocheck[3] = {'>', '>', '='};
	for (int i = 0; i < 3; i++)
	{
		if (lexer->lookahead != tocheck[i])
			return(false);
		lexer->advance(lexer, false);
	}

	// Handle optional \r before \n (CRLF compatibility)
	if (lexer->lookahead == '\r')
		lexer->advance(lexer, false);
	if (lexer->lookahead != '\n')
		return(false);

	lexer->result_symbol = JNV_SIG_END;
	lexer->mark_end(lexer);
	return(true);
}

static bool word_or_sig(TSLexer* lexer)
{
	bool eof = lexer->eof(lexer);
	if (eof) return(false);

	lexer->mark_end(lexer);

	int32_t col = lexer->get_column(lexer);
	int32_t val  = lexer->lookahead;
	int32_t fval = lexer->lookahead;

	if ((col == 0) && (val != '\\')) {
		bool issig = true;
		char begcheck[2] = {'<', '<'};
		for (int i = 0; i < 2; i++)
		{
			if (val != begcheck[i])
			{
					issig = false;
					break;
			}
			lexer->advance(lexer, false);
			val = lexer->lookahead;
		}
		if (issig) {
			lexer->mark_end(lexer);
			lexer->result_symbol = JNV_SIG_BEG;
			return(true);
		}
	}

	if (lexer->lookahead == '%' && (col == 0 || prev_was_ws))
	{
		while ((val != '\n') && ! lexer->eof(lexer))
		{
			lexer->advance(lexer, false);
			val = lexer->lookahead;
		}
		lexer->mark_end(lexer);
		lexer->result_symbol = LATEX_COMMENT;
		return(true);
	}

	if (lexer->lookahead != '\\')
	{
		while ((!ws(val)) && (val != '\\') && ! lexer->eof(lexer))
		{
			lexer->advance(lexer, false);
			val = lexer->lookahead;
		}
		lexer->mark_end(lexer);
		lexer->result_symbol = _LATEX_WORD;
		return(true);
	}

	if (lexer->lookahead == '\\')
	{
		bool is_sexpr = true;
		char sexpr_check[SEXPR] = {'\\', 'S', 'e', 'x', 'p', 'r'};
		for (int i = 0; i < SEXPR; i++)
		{
			if (val != sexpr_check[i])
			{
				is_sexpr = false;
				break;
			}
			lexer->advance(lexer, false);
			val = lexer->lookahead;
		}

		if (is_sexpr) {
			lexer->mark_end(lexer);
			lexer->result_symbol = JNV_INLINE;
			return(true);
		}

		while ((!ws(val)) && (val != '\\') && ! lexer->eof(lexer))
		{
			lexer->advance(lexer, false);
			val = lexer->lookahead;
		}
		lexer->mark_end(lexer);
		lexer->result_symbol = _LATEX_WORD;
		return(true);
	}

	return(false);
}

void *tree_sitter_jnoweb_external_scanner_create()
{
	return(NULL);
};
void tree_sitter_jnoweb_external_scanner_destroy(void *payload)
{};
unsigned tree_sitter_jnoweb_external_scanner_serialize(
  void *payload,
  char *buffer
) {
	return 0;
};
void tree_sitter_jnoweb_external_scanner_deserialize(
  void *payload,
  const char *buffer,
  unsigned length
)
{};

bool tree_sitter_jnoweb_external_scanner_scan(
  void *payload,
  TSLexer *lexer,
  const bool *valid_symbols
)
{
	advance_ws(lexer);

	bool res = false;

	if (valid_symbols[_LATEX_WORD] || valid_symbols[JNV_INLINE] || valid_symbols[JNV_SIG_BEG] || valid_symbols[LATEX_COMMENT])
	{
		res = word_or_sig(lexer);
	} else if (valid_symbols[JNV_SIG_END]) {
		res = jnv_sig_end(lexer);
	} else if (valid_symbols[JNV_CONTENT]) {
		res = jnv_content(lexer);
	}

	return(res);
};
