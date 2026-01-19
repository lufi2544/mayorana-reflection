/* date = January 8th 2026 7:19 pm */

#include "reflection_includes.h"
#include <cstring>

//////////////////////
/*
 * This is a reflection system that is created for my personal use, so I can inspect any struct or class fields and use them as
 * generated metadata for different pruposes.
*/
//////////////////////

//////////////
// Basic types the the reflection system supports
// IMPORTANT: This has to be equal to the primitive_meta_type enum @see: reflection_includes.h, since the fields has to match, we need to know about this primitives at this libs compile time
// to generically know how to print "mayorana.h" primitives and data structures.
//
char* basic_meta_types [] = 
{
	"u8", 
	"u16", 
	"u32", 
	"u64", 
	"s8", 
	"s16", 
	"s32", 
	"s64", 
	"f32", 
	"f64", 
	"bool", 
};

struct member_node
{
	char* type;
	char* name;
	u32 flags;
	
	member_node *next;
};

/// linked list to generate the ClassMeta_enum
struct meta_node
{
	char* name;
	member_node *member;
	
	meta_node* next;
	meta_node* prev;
};

meta_node* current_meta_node;
u32 meta_idx_counter = 0;

/////////////

struct flag_name
{
	u32 flag;
	const char *name;
};

global const flag_name MemberFlagNames[] =
{
	{ MemberFlag_IsPointer, "MemberFlag_IsPointer" },
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

/** TODO: This needs to be in the project itself.

global_f void
print_struct(u32 _member_count, member_definition *_struct_definition, void *struct_ptr)
{
	for(u32 member_idx = 0;
		member_idx < _member_count;
		++member_idx)
	{
		member_definition *member = _struct_definition + member_idx;
		char text_buffer[256];
		text_buffer[0] = 0;
		swtich(member->type)
		{
			case Metatype_u32:
			{
				
			}break;
			
			case MetaType_bool:
			{
				
			}break;
			
			case MetaType_bool:
			{
				
			}break;
			case MetaType_bool:
			{
				
			}break;
			case MetaType_bool:
			{
				
			}break;
			case MetaType_bool:
			{
				
			}break;
			case MetaType_bool:
			{
				
			}break;
			case MetaType_bool:
			{
				
			}break;
		}
	}
} 

*/

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


internal_f bool
token_is_reflected(token _token)
{
	bool result = token_equals(_token, "MY_CLASS")
		|| token_equals(_token, "MY_STRUCT")
		|| token_equals(_token, "MY_ENUM");
	
	return result;
}

internal_f bool 
token_is_property_reflected(token _token)
{
	bool result = token_equals(_token, "MY_PROPERTY");
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


internal_f void
parse_member_params(tokenizer *tokenizer)
{
	for(;;)
	{
		token this_token = get_token(tokenizer);
		if((this_token.type == Token_CloseParen)
		   || (this_token.type == Token_EndOfStream))
		{
			break;
		}
		else if(this_token.type == Token_Identifier)
		{
			// params parsing here
			
			
		}
	}
}


internal_f void 
skip_member(tokenizer *_tokenizer)
{
	parsing_move_to(_tokenizer, Token_Semicolon);
}

internal_f void
get_member_flags_string(u32 flags, char *buffer)
{
	buffer[0] = 0;
	char *at = buffer;
	
	for(int i = 0;
		i < ArrayCount(MemberFlagNames);
		++i)
	{
		if(flags & MemberFlagNames[i].flag)
		{
			if(at != buffer)
			{
				strcat(at, " || ");
			}
			
			strcat(at, MemberFlagNames[i].name);
		}
	}
}


// TODO: Change this to Parsing or somethig
internal_f void
generate_member_definition(tokenizer *_tokenizer, token _struct_type_token, meta_node *_struct_meta)
{	
	for(;;)
	{
		token this_token = get_token(_tokenizer);
		if(this_token.type == Token_CloseBraces)
		{
			break;
		}
		else
		{
			// Checking if the token we are about to parse is reflected, if it is, then we parse it
			if(token_is_property_reflected(this_token))
			{
				// getting the token type token in here.
				parse_member_params(_tokenizer);
				
				token member_type_token = get_token(_tokenizer);
				
				u32 member_flags = 0;
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
						skip_member(_tokenizer);
						break;
					}
					
					switch(this_token.type)
					{			
						case Token_Asterisk:
						{
							member_flags |= MemberFlag_IsPointer;						
						}break;
						
						case Token_Identifier:
						{
														
							// name cpy
							member_node *this_member = (member_node*)malloc(sizeof(member_node));
							this_member->next = 0;
							this_member->name = (char*)malloc(this_token.text_len + 1);
							snprintf(this_member->name, this_token.text_len + 1, "%s", this_token.text);
							
							
							// type cpy
							this_member->type = (char*)malloc(member_type_token.text_len + 1);
							snprintf(this_member->type, member_type_token.text_len + 1, "%s", member_type_token.text);
							
							this_member->flags = member_flags;							
							
							if(_struct_meta)
							{
								member_node *current_membe_node = _struct_meta->member;
								
								if(current_membe_node)
								{
									this_member->next = current_membe_node;
								}
								
								_struct_meta->member = this_member;								
							}
							
							
						}break;
						
						case Token_Semicolon:
						case Token_EndOfStream:
						{
							parsing = false;
							
						}break;
						
					}
				}													
			}
		}
	}	
	
}

// TODO: in the future we will set this to be serializable only the MY_PROPERTY() fields
internal_f void
parse_struct(tokenizer *_tokenizer)
{
	token struct_type_token = get_token(_tokenizer);
	
	
	// Meta type enum generation
	meta_node* new_meta_node = new meta_node();
	new_meta_node->next = 0;
	new_meta_node->prev = current_meta_node;
	new_meta_node->name = (char*)malloc(struct_type_token.text_len + 1);
	new_meta_node->name[0] = 0;
	memcpy(new_meta_node->name, struct_type_token.text, struct_type_token.text_len);
	new_meta_node->name[struct_type_token.text_len] = 0;
	
	if(!current_meta_node)
	{
		current_meta_node = new_meta_node;
	}
	else
	{
		current_meta_node->next = new_meta_node;
		current_meta_node = new_meta_node;
	}
	
	if(require_token(_tokenizer, Token_OpenBraces))
	{
		// generating the member definitio and the type definition for this struct
		generate_member_definition(_tokenizer, struct_type_token, current_meta_node);
		//generate_type_definition(_tokenizer, struct_type_token);
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

internal_f void
generate_type_definition_for(char *name, int idx)
{
	printf("const type_definition definition_of_%s = \n", name);
	printf("{ \n");
	printf("\"%s\", \n", name);
	printf("sizeof(%s), \n", name);
//	printf("%d, \n", idx);
	printf("0, \n");
	printf("0 \n");
	printf("};\n");
	printf("\n");
}

internal_f void
generate_basic_types_definition()
{	
	// Type definition struct
	for(int idx = 0;
		idx < ArrayCount(basic_meta_types);
		++idx)
	{
		char* type = basic_meta_types[idx];
		generate_type_definition_for(type, idx + 1 /* we add up 1 as the 0 is MetaType_None */);
	}
}

internal_f void
generate_meta_enum_for(char *name)
{
	printf("MetaType_%s, \n", name);
}

internal_f void
generate_basic_types_meta()
{
	
	// MetaType enum definition.
	for(int idx = 0;
		idx < ArrayCount(basic_meta_types);
		++idx)
	{
		char* type = basic_meta_types[idx];
		generate_meta_enum_for(type);
	}
}

internal_f void
generate_basic_types_meta_type()
{
	for(int idx = 0;
		idx < ArrayCount(basic_meta_types);
		++idx)
	{
		char* type = basic_meta_types[idx];
		generate_meta_enum_for(type);
	}
}

meta_node* first_meta_node = 0;

global_f void
generate_meta_enum_for_reflected()
{
	if(!current_meta_node)
	{
		return;
	}
		
	printf("\n");
	printf("enum meta_type : u32 \n");
	printf("{\n");
	printf("MetaType_none, \n");	
		
	generate_basic_types_meta();
	
	// going back in he list to the first element to recreate the enum in the correct order
	meta_node* first_node = 0;
	
	for(meta_node* node_idx = current_meta_node; 
		node_idx;
		node_idx = node_idx->prev)
	{
		first_node = node_idx;
	}
	
	// first meta_node caching
	first_meta_node = first_node;
	
	printf("\n");
		
	for(meta_node* node_idx = first_node;
		node_idx; 
		node_idx = node_idx->next)
	{
		printf("MetaType_%s, \n", node_idx->name);
	}
	
	printf("MetaType_num \n");
	printf("}; \n");
	printf("\n");
}

internal_f void
generate_member_definition_for_reflected()
{
	if(!first_meta_node)
	{
		return;
	}
	
		
	for(meta_node *node_idx = first_meta_node;
		node_idx;
		node_idx = node_idx->next)
	{
		
		printf("const member_definition members_of_%s[] = \n", node_idx->name);
		printf("{\n");
		for(member_node *member_idx = node_idx->member;
			member_idx;
			member_idx = member_idx->next)
		{
			char member_flags_string [256];
			member_flags_string[0] = 0;			
			get_member_flags_string(member_idx->flags, member_flags_string);
			
			printf("{\"%s\", MetaType_%s, OFFSET_OF(%s, %s), %s}, \n",
				   member_idx->name,
				   member_idx->type,
				   node_idx->name,
				   member_idx->name,
				   member_flags_string[0] != 0 ? member_flags_string : "0");
		}
		
		printf("};\n");
		printf("\n");	
	}
	

}

internal_f void
generate_type_definition_for_reflected()
{
	if(!first_meta_node)
	{
		return;
	}
	
	for(meta_node *idx = first_meta_node;
		idx;
		idx = idx->next)
	{
		printf("const type_definition definition_of_%s \n", idx->name);
		printf("{ \n");			
		printf(" \"%s\",\n", idx->name);
		// the meta_type with the idx 0 is the MetaType_none, so we start at 1.
		u32 current_meta_idx = (++meta_idx_counter + ArrayCount(basic_meta_types));
//		printf("%d, \n", current_meta_idx);
		printf("sizeof(%s), \n", idx->name);
		printf("members_of_%s, \n", idx->name);
		printf("ArrayCount(members_of_%s) \n", idx->name);				
		
		printf("}; \n");
		printf("\n");
	}
}


//  we will have a table that for getting a certain type and access its members, we will have to check against the enum MetaType_xxx assigned to it.
internal_f void
generate_type_definition_table()
{
	printf("\n");
	
	printf("const type_definition* all_type_definitions[] = \n");
	printf("{\n");
	
	printf("0, \n");
	// Filll the array with the basic types
	for(u32 idx = 0; 
		idx < ArrayCount(basic_meta_types);
		++idx)
	{
		const char* basic_type = basic_meta_types[idx];
		printf("&definition_of_%s, \n", basic_type);
	}
	
	printf("\n");
	
	// Custom struct types
	for(meta_node *idx = first_meta_node;
		idx;
		idx = idx->next)
	{
		printf("&definition_of_%s, \n", idx->name);
	}
	
	printf("};\n");
}

