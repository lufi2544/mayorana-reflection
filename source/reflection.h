/* date = January 8th 2026 7:19 pm */


//////////////////////
/*
 * This is a reflection system that is created for my personal use, so I can inspect any struct or class fields and use them as
 * generated metadata for different pruposes.
*/
//////////////////////


#define MY_CLASS(params)
#define MY_STRUCT(params)
#define MY_ENUM(params)



// TODO: for now lets add this by hand, but reflection should generate the code for the different types in a file, so then the code can use it.
enum enum_meta_type
{
	MetaType_u8,
	MetaType_u16,
	MetaType_u32,
	MetaType_u64,
	MetaType_s8,
	MetaType_s16,
	MetaType_s32,
	MetaType_s64,
	MetaType_f32,
	MetaType_f64,	
};

// TODO: this I think must be inside the project itself, so hm... I have to think about it.
struct member_definition
{
	enum_meta_type type;
	char* name;
	u32 offset;
};


enum enum_token_type
{
	Token_Unknown,
	
	Token_OpenParen,
	Token_Colon,
	Token_CloseParen,
	Token_Semicolon,
	Token_Asterisk,
	Token_OpenBracket,
	Token_CloseBracket,
	Token_OpenBraces,
	Token_CloseBraces,
	
	Token_String,
	Token_Identifier,
	
	Token_EndOfStream,
};

struct token
{
	enum_token_type type;
	u32 text_len;
	char *text;
};

struct tokenizer
{
	char *at;
};

inline bool 
is_white_space(char c)
{
	bool result = ((c == ' ') ||
				   (c == '\t') ||
				   (c == '\n') ||
				   (c == '\r'));
	
	return result;
}


// In this case the match has the \0 operator at the end, but not the _token->text data, since it is a buffer with a lenght based on a bigger text buffer.
global_f bool
token_equals(token _token, char *match)
{	
	char* at = match;
	for(u32 idx = 0;
		idx < _token.text_len;
		++idx, ++at)
	{
		if((*at == 0) ||
		   (_token.text[idx] != *at))
		{
			return false;
		}
	}
	
	bool result = (*at == 0);	
	return result;	
}


global_f bool
token_is_reflected(token _token)
{
	bool result = token_equals(_token, "MY_CLASS")
		|| token_equals(_token, "MY_STRUCT")
		|| token_equals(_token, "MY_ENUM");
	
	return result;
}


global_f token
get_token(tokenizer *_tokenizer);

internal_f bool
require_token(tokenizer *_tokenizer, enum_token_type _type)
{
	token this_token = get_token(_tokenizer);
	bool result = this_token.type == _type;
	
	return result;
}


internal_f void
parse_reflection_params(tokenizer *_tokenizer)
{
	for(;;)
	{
		// TODO Nothing for now here in the params
		token this_token = get_token(_tokenizer);
		if((this_token.type == Token_CloseParen)
		   || (this_token.type == Token_EndOfStream))
		{
			break;
		}
	}
}


internal_f void 
parsing_move_to(tokenizer *_tokenizer, enum_token_type target_token)
{
	bool moving = true;	
	while((_tokenizer->at != 0) && (moving))
	{
		token this_token = get_token(_tokenizer);
		if(this_token.type == target_token)
		{
			moving = false;
			break;
		}
	}
	
	
	if(moving)
	{
		fprintf(stderr, "ERROR: tried to go to %d token type but failed", target_token);
	}	
}

/**
* We don't support static const reflection, since I think it does not make sense for now? I have to investigate.
*/
internal_f void
parse_member(tokenizer *_tokenizer, token _struct_token, token _member_type_token)
{
	bool is_pointer = false;
	bool parsing = true;
	while(parsing)
	{
		token this_token = get_token(_tokenizer);
		
		if((token_equals(this_token, "static")) ||
		   (token_equals(this_token, "const")) || 
		   (token_equals(this_token, "constexpr")))
		{
			// baiscally if we encounter one of this identifers, we just return sice we are not parsing: "static", "consts" or "constexpr"
			parsing = false;
			parsing_move_to(_tokenizer, Token_Semicolon);			
			break;
		}
		
		switch(this_token.type)
		{
			
			case Token_Asterisk:
			{
				is_pointer = true;				
				
			}break;
			
			case Token_Identifier:
			{
				printf("{MetaType_%.*s, \"%.*s\", (u32)&((%.*s *)0)->%.*s}, \n",
					   _member_type_token.text_len, _member_type_token.text,
					   this_token.text_len, this_token.text, 
					   _struct_token.text_len, _struct_token.text, 
					    this_token.text_len, this_token.text);
				
			}break;
			
			case Token_Semicolon:
			case Token_EndOfStream:
			{
				parsing = false;
				
			}break;
			
		}
	}
}

// TODO: in the future we will set this to be serializable only the MY_PROPERTY() fields
internal_f void
parse_struct(tokenizer *_tokenizer)
{
	token struct_type_token = get_token(_tokenizer);
	
	if(require_token(_tokenizer, Token_OpenBraces))
	{
		printf("member_definition *members_of_%.*s[] = \n", struct_type_token.text_len, struct_type_token.text);
		printf("{\n");
		for(;;)
		{
			token member_type_token = get_token(_tokenizer);
			
			if(member_type_token.type == Token_CloseBraces)
			{
				break;
			}
			else
			{
				// current struct member parsin with the member type
				parse_member(_tokenizer, struct_type_token, member_type_token);
			}
		}
		
		printf("};\n");
	}
}


/**
 * This is the reflection params and reflection identifier part.
*/
global_f void
parse_reflected(tokenizer *_tokenizer)
{
	if(require_token(_tokenizer, Token_OpenParen))
	{
		parse_reflection_params(_tokenizer);
		token type_token = get_token(_tokenizer);
		if(token_equals(type_token, "struct"))
		{
			parse_struct(_tokenizer);
			
		}else
		{			
			fprintf(stderr, "ERROR: Reflection is only supported for struct.");
		}
		
	}
	else
	{
		fprintf(stderr, "ERROR: Missing parenthesis after the reflection identifier.");
	}
}

inline bool
is_end_of_line(char c)
{
	bool result = ((c == '\n') ||
				   (c == '\r'));
	
	return result;
}

inline bool
is_alpha(char c)
{
	bool result = ((c <= 'z') && (c >= 'a')) || ((c <= 'Z') && (c >= 'A'));
	return result;
}

inline bool
is_numeric(char c)
{
	bool result = (c >= '0') && (c <= '9');	
	return result;
}


internal_f void
parse_c_style_comment(tokenizer *_tokenizer)
{
	_tokenizer->at += 2;
	while(_tokenizer->at[0] && !is_end_of_line(_tokenizer->at[0]))
	{
		_tokenizer->at++;
	}
}

internal_f void
parse_cpp_style_comment(tokenizer *_tokenizer)
{
	_tokenizer->at += 2;
	while((_tokenizer->at[0] && !((_tokenizer->at[0] == '*') && (_tokenizer->at[1] == '/'))))
	{
		_tokenizer->at++;
	}
	// if we don't have any comment in between the open and close part like: '/**/', we would not have enetered the while loop, so we skilp
	// that part manually
	if(_tokenizer->at[0] == '*')
	{
		_tokenizer->at += 2;
	}
}

internal_f void
eat_whitespaces(tokenizer *_tokenizer)
{
	for(;;)
	{		
		if(is_white_space(_tokenizer->at[0]))
		{
			++_tokenizer->at;
		}
		else if((_tokenizer->at[0] == '/') && (_tokenizer->at[1] == '/'))
		{		
			parse_c_style_comment(_tokenizer);
		}
		else if((_tokenizer->at[0] == '/') && (_tokenizer->at[1] == '*'))
		{
			parse_cpp_style_comment(_tokenizer);
		}
		else
		{
			
			break;
		}
		
	}		
}

internal_f void
parse_identifier(tokenizer *_tokenizer)
{
	while(is_alpha(_tokenizer->at[0]) ||
		  is_numeric(_tokenizer->at[0]) ||
		  _tokenizer->at[0] == '_')
	{
		_tokenizer->at++;
	}
}


global_f token
get_token(tokenizer *_tokenizer)
{
	eat_whitespaces(_tokenizer);
	
	token result;
	result.text_len = 1;
	result.text = _tokenizer->at;
	
	char C = _tokenizer->at[0];
	++_tokenizer->at;
	
	switch(C)
	{
		case '\0':{ result.type = Token_EndOfStream; }break;
		case '(':{ result.type = Token_OpenParen; }break;
		case ')':{ result.type = Token_CloseParen; }break;
		case '{':{ result.type = Token_OpenBraces; }break;
		case '}':{ result.type = Token_CloseBraces; }break;
		case '[':{ result.type = Token_OpenBracket; }break;
		case ']':{ result.type = Token_CloseBracket; }break;
		case '*':{ result.type = Token_Asterisk; }break;
		case ':':{ result.type = Token_Colon; }break;
		case ';':{ result.type = Token_Semicolon; }break;
		
		
		case '"':
		{
			result.text = _tokenizer->at;
			while(_tokenizer->at[0] && _tokenizer->at[0] != '"')
			{
				if((_tokenizer->at[0] == '\\' && _tokenizer->at[1]))
				{
					++_tokenizer->at;
				}
				++_tokenizer->at;
			}
			
			result.type = Token_String;
			result.text_len = _tokenizer->at - result.text;
			
			if(_tokenizer->at[0] == '"')
			{
				_tokenizer->at++;
			}
			
		}break;
		
		
		default:
		{
			if(is_alpha(C))
			{
				parse_identifier(_tokenizer);
				result.type = Token_Identifier;
				result.text_len = _tokenizer->at - result.text;
			}
#if 0
			else if(is_numeric(C))
			{
				parse_number();
			}
#endif
			else
			{
				result.type = Token_Unknown;
			}
			break;
		}
		
	}
	
			
	return result;
}
