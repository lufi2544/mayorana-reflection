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


enum enum_token_type
{
	Token_Unknown,
	
	Token_OpenParam,
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
	enum_token_type token_type;
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


internal_f token
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
		case '\0':{ result.token_type = Token_EndOfStream; }break;
		case '(':{ result.token_type = Token_OpenParam; }break;
		case ')':{ result.token_type = Token_CloseParen; }break;
		case '{':{ result.token_type = Token_OpenBraces; }break;
		case '}':{ result.token_type = Token_CloseBraces; }break;
		case '[':{ result.token_type = Token_OpenBracket; }break;
		case ']':{ result.token_type = Token_CloseBracket; }break;
		case '*':{ result.token_type = Token_Asterisk; }break;
		case ':':{ result.token_type = Token_Colon; }break;
		case ';':{ result.token_type = Token_Semicolon; }break;
		
		
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
			
			result.token_type = Token_String;
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
				result.token_type = Token_Identifier;
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
				result.token_type = Token_Unknown;
			}
			break;
		}
		
	}
	
			
	return result;
}
