#include "grammar.h"
#include "lexer.h"
#include "ast.h"
#include "token.h"

void init_grammar(grammar * g, lexer * l)
{
	g->l = l;
}

void destroy_grammar(grammar * g)
{
	// Nothing to do, yet
}

ast_base * build_ast(grammar * g)
{
	return read_function_ast_node(g);
}

ast_base * read_function_ast_node(grammar * g)
{
	ast_base * root = NULL;
	ast_base * body = NULL;
	token_base * token = NULL;
	token_function * function_name = NULL;

	token = next(g->l);
	if (token->type != T_INT_TYPE)
	{
		fprintf(stderr, "INT_TYPE not found at the beginning of a function signature");
		exit(EXIT_FAILURE);
	}
	free_token_int_type((token_int_type *)token);

	token = next(g->l);
	if (token->type != T_FUNCTION)
	{
		fprintf(stderr, "FUNCTION not found as second entry for reading a function");
		exit(EXIT_FAILURE);
	}
	function_name = (token_function *) token;

	token = next(g->l);
	if (token->type != T_OPAR)
	{
		fprintf(stderr, "OPAR not found after the name of the function");
		exit(EXIT_FAILURE);
	}
	free_token_opar((token_opar *)token);

	token = next(g->l);
	if (token->type != T_CPAR)
	{
		fprintf(stderr, "CPAR not found after the name of the function");
		exit(EXIT_FAILURE);
	}
	free_token_cpar((token_cpar *)token);

	body = read_function_body(g);
	root = (ast_base *) malloc(sizeof(node_function));

	init_node_function((node_function *)root, function_name->name, body);
	free_token_function(function_name);
	return root;
}

ast_base * read_function_body(grammar * g)
{
	ast_base * root = NULL;
	ast_base * return_ast_value = NULL;
	token_base * token = NULL;
	token_int_value * int_value = NULL;

	token = next(g->l);
	if (token->type != T_OBRA)
	{
		fprintf(stderr,	"OBRA not found at the beginning of the function body");
		exit(EXIT_FAILURE);
	}
	free_token_obra((token_obra *)token);

	token = next(g->l);
	if (token->type != T_RETURN)
	{
		fprintf(stderr, "RETURN not found just before the end of the function body");
		exit(EXIT_FAILURE);
	}
	free_token_return((token_return *)token);

	token = next(g->l);
	if (token->type != T_INT_VALUE)
	{
		fprintf(stderr, "INT_VALUE not found after return statement");
		exit(EXIT_FAILURE);
	}
	int_value = (token_int_value *) token;
	return_ast_value = (ast_base *) malloc(sizeof(node_int));
	init_node_int((node_int *)return_ast_value, int_value->value);
	free_token_int_value(int_value);

	root = (ast_base *) malloc(sizeof(node_return));
	init_node_return((node_return *)root, return_ast_value);

	token = next(g->l);
	if (token->type != T_SEMICOLON)
	{
		fprintf(stderr, "SEMICOLON not found at the end of the function body");
		exit(EXIT_FAILURE);
	}
	free_token_semicolon((token_semicolon *)token);

	token = next(g->l);
	if (token->type != T_CBRA)
	{
		fprintf(stderr, "CBRA not found at the end of the function body");
		exit(EXIT_FAILURE);
	}
	free_token_cbra((token_cbra *)token);
	return root;
}
