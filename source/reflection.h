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
	u32 name_len;
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
	
	// Used to determine if this is  a class, struct or enum.
	u32 flags;
	
	// Special for Enums
	char* size_string;
	u32 size_string_len;
};


meta_node* current_meta_node; // this is the node that we will use to create the enum for all the MetaTypes later on when finished parsing.
u32 meta_idx_counter = 0;

/////////////

struct flag_name
{
	u32 flag;
	char *name;
};

global const flag_name MemberFlagNames[] =
{
	{ MemberFlag_IsPointer, "MemberFlag_IsPointer" },
	{ MemberFlag_IsEnum, "MemberFlag_IsEnum" },
	{ MemberFlag_IsEnumField, "MemberFlag_IsEnumField" },
};

global flag_name TypeFlagNames[] = 
{
	
	{TypeFlag_IsEnum, "TypeFlag_IsEnum"},
	{TypeFlag_IsStruct, "TypeFlag_IsStruct"},
	{TypeFlag_IsClass, "TypeFlag_IsClass"},
};


enum enum_token_type
{
	Token_Unknown,
	
	Token_OpenParen,
	Token_Coma,
	Token_Point,
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


global_f token
get_token(tokenizer *_tokenizer);  

internal_f bool 
token_is_property_reflected(token _token)
{
	bool result = token_equals(_token, "MY_PROPERTY");
	return result;
}


internal_f bool
token_is_reflected(token _token, tokenizer *_tokenizer)
{
	bool result = token_equals(_token, "MY_CLASS")
		|| token_equals(_token, "MY_STRUCT")
		|| token_equals(_token, "MY_ENUM");
	
	return result;
}

internal_f bool
require_token(tokenizer *_tokenizer, enum_token_type _type)
{
	token this_token = get_token(_tokenizer);
	bool result = this_token.type == _type;
	
	return result;
}


internal_f void
parse_reflection_params(tokenizer *_tokenizer, u32 *flags)
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
		else if(this_token.type == Token_Identifier)
		{
			if(token_equals(this_token, "CustomPrinter"))
			{
				(*flags) |= TypeFlag_HasCustomPrinter;
			}
		}
	}
}


internal_f void 
tokenizer_move_to(tokenizer *_tokenizer, enum_token_type target_token)
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
parse_member_params(tokenizer *tokenizer, u32 *member_flags)
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
			if(token_equals(this_token, "enum"))
			{
				(*member_flags) |= MemberFlag_IsEnum;
			}
			
		}
	}
}


internal_f void 
skip_member(tokenizer *_tokenizer)
{
	tokenizer_move_to(_tokenizer, Token_Semicolon);
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

internal_f void
get_type_flags_string(u32 flags, char* buffer)
{
	buffer[0] = 0;
	char *at = buffer;
	
	for(int i = 0;
		i < ArrayCount(TypeFlagNames);
		++i)
	{
		if(flags & TypeFlagNames[i].flag)
		{
			if(at != buffer)
			{
				strcat(at, " || ");
			}
			
			strcat(at, TypeFlagNames[i].name);
		}
	}	
}

internal_f bool
member_has_flag(member_node *node, member_flag flag)
{
	return node->flags & flag;
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
			// Checking if the field is reflected MY_PROPERTY 
			if(token_is_property_reflected(this_token))
			{
				// getting the token type token in here.
				u32 member_flags = 0;
				parse_member_params(_tokenizer, &member_flags);
				
				token member_type_token = get_token(_tokenizer);
				
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
						
						
						// MEMBER NODE ORDER IN MEMORY: member3->member2->member1
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


// TODO: Change this to Parsing or somethig
internal_f void
generate_enum_type_definition(tokenizer *_tokenizer, token enum_type_token, meta_node *_meta_node)
{	
	// we can support the :u32.. enums or not, for now we are flexible	
	
	bool b_default_enum_size = false;
	char default_enum_type_size [3] = "u8";
	if(!require_token(_tokenizer, Token_Colon))
	{
		printf("\n");
		printf( "Found the ENUM: \"%.*s\" without the type definition, adding u8 as default; \n", enum_type_token.text_len, enum_type_token.text);		
		b_default_enum_size = true;
		
		
		u8 string_size = sizeof(default_enum_type_size);
		_meta_node->size_string = (char*)malloc(string_size + 1);		
		_meta_node->size_string_len = string_size;
		bytes_copy(_meta_node->size_string, default_enum_type_size, string_size + 1);
		
		
		_meta_node->size_string[string_size] = 0;
	}
	
	
	// mabye for later sizeof(token_type)
	token enum_size_token = get_token(_tokenizer);
	
	if(!b_default_enum_size)
	{
		_meta_node->size_string = enum_size_token.text;
		_meta_node->size_string_len = enum_size_token.text_len;
	}
	
	
	
	bool parsing = true;
	while(parsing)
	{
		token this_token = get_token(_tokenizer);
		if(this_token.type == Token_CloseBraces)
		{
			parsing = false;
			break;
		}
		else
		{			
			// Checking if the token we are about to parse is reflected, if it is, then we parse it
			
			// getting the token type token in here.
			u32 member_flags = 0;
			
			// TODO: add member metadata.
			//	parse_member_params(_tokenizer, &member_flags);								
			switch(this_token.type)
			{															
				// MEMBER NODE ORDER IN MEMORY: member3->member2->member1
				case Token_Identifier:
				{
					
					member_flags |= MemberFlag_IsEnumField;
					// name cpy
					member_node *this_member = (member_node*)malloc(sizeof(member_node));
					this_member->next = 0;
					this_member->name = (char*)malloc(this_token.text_len + 1);
					snprintf(this_member->name, this_token.text_len + 1, "%s", this_token.text);
					
					
					// this is the type of the current member of the enum, we will treat it as the same size as the enum size
					this_member->type = (char*)malloc(b_default_enum_size ? sizeof(default_enum_type_size) : enum_type_token.text_len + 1);
					
					snprintf(this_member->type, 
							 b_default_enum_size ? sizeof(default_enum_type_size) : enum_size_token.text_len + 1,
							 "%s", 
							 b_default_enum_size ? default_enum_type_size : enum_size_token.text);
					
					this_member->flags = member_flags;							
					
					if(_meta_node)
					{
						member_node *current_membe_node = _meta_node->member;
						
						if(current_membe_node)
						{
							this_member->next = current_membe_node;
						}
						
						_meta_node->member = this_member;
					}
					
					
				}break;
				
				case Token_EndOfStream:
				{
					parsing = false;
					
				}break;				
			}																			
		}
	}	
	
}


internal_f void
parse_enum(tokenizer *this_tokenizer)
{
	token enum_type_token = get_token(this_tokenizer);
	
	meta_node* new_meta_node = new meta_node();
	new_meta_node->flags = 0;
	new_meta_node->size_string = 0;
	new_meta_node->next = 0;
	new_meta_node->prev = current_meta_node;
	new_meta_node->name = (char*)malloc(enum_type_token.text_len + 1);
	new_meta_node->name[0] = 0;
	memcpy(new_meta_node->name, enum_type_token.text, enum_type_token.text_len);
	new_meta_node->name[enum_type_token.text_len] = 0;
	
	new_meta_node->flags |= TypeFlag_IsEnum;
	
	if(!current_meta_node)
	{
		current_meta_node = new_meta_node;
	}
	else
	{
		current_meta_node->next = new_meta_node;
		current_meta_node = new_meta_node;
	}	
	
	generate_enum_type_definition(this_tokenizer, enum_type_token, current_meta_node);				
}




// TODO: in the future we will set this to be serializable only the MY_PROPERTY() fields
internal_f void
parse_struct(tokenizer *_tokenizer, u32 type_flags)
{
	token struct_type_token = get_token(_tokenizer);
	
	
	// Meta type enum generation
	meta_node* new_meta_node = new meta_node();
	new_meta_node->flags = type_flags;
	new_meta_node->size_string = 0;
	new_meta_node->next = 0;
	new_meta_node->prev = current_meta_node;
	new_meta_node->name = (char*)malloc(struct_type_token.text_len + 1);
	new_meta_node->name[0] = 0;
	memcpy(new_meta_node->name, struct_type_token.text, struct_type_token.text_len);
	new_meta_node->name[struct_type_token.text_len] = 0;
	
	new_meta_node->flags |= TypeFlag_IsStruct;
	
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
	if(require_token(_tokenizer, Token_OpenParen) &&
	   !require_token(_tokenizer, Token_Point)) 	// Check if we are trying to parse the definition of the void Macros for reflection MY_STRUCT(...), MY_CLASS(...), MY_ENUM(...)
	{
		u32 flags = 0;
		parse_reflection_params(_tokenizer, &flags);
		
		token type_token = get_token(_tokenizer);
		
		if(token_equals(type_token, "struct"))
		{
			parse_struct(_tokenizer, flags);
			
		}
		else if(token_equals(type_token, "enum"))
		{
			parse_enum(_tokenizer);
		}
		else
		{			
			fprintf(stderr, "ERROR: Reflection is only supported for struct \n.");
		}
		
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
		case ',':{ result.type = Token_Coma; }break;
		case '.':{ result.type = Token_Point; }break;
		
		
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
	printf("0, \n");
	printf("TypeFlag_IsPrimitive, \n");
	printf("printer_%s \n", name);
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
	
	printf("\n");
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
		
		// ordering the nodes by member as in memory
		member_node* first_node = 0;
		member_node* prev_member = 0;
		
		for(member_node* node = node_idx->member;
			node; 
			node = prev_member)
		{
			prev_member = node->next;
			node->next = first_node;
			first_node = node;						
		}				
		
		
		printf("const member_definition members_of_%s[] = \n", node_idx->name);
		printf("{\n");
		
		u32 member_node_idx = 0;
		for(member_node *member_idx = first_node;
			member_idx;
			member_idx = member_idx->next, ++member_node_idx)
		{
			char member_flags_string [256];
			member_flags_string[0] = 0;			
			get_member_flags_string(member_idx->flags, member_flags_string);
			
			
			if(member_has_flag(member_idx, MemberFlag_IsEnumField))
			{
				
				printf("{\"%s\", MetaType_%s, %i, %s}, \n",
					   member_idx->name,
					   member_idx->type,
					   member_node_idx,
					   member_flags_string[0] != 0 ? member_flags_string : "0");
			}
			else				
			{
				
				printf("{\"%s\", MetaType_%s, OFFSET_OF(%s, %s), %s}, \n",
					   member_idx->name,
					   member_idx->type,
					   node_idx->name,
					   member_idx->name,
					   member_flags_string[0] != 0 ? member_flags_string : "0");
			}
			
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
		
		if(idx->flags & TypeFlag_IsEnum)
		{
			// Set for enums
			printf("sizeof(%.*s), \n", idx->size_string_len, idx->size_string);
		}
		else
		{
			// set for struct or classes
			printf("sizeof(%s), \n", idx->name);
		}
		
		// TODO: Flags for determine if the calss is a struct, class, or enum.
		// Flags...
		
		printf("members_of_%s, \n", idx->name);
		printf("ArrayCount(members_of_%s), \n", idx->name);
		
		char flags_string[256];
		get_type_flags_string(idx->flags, flags_string);
		printf("%s, \n" ,flags_string);
		
		if(idx->flags & TypeFlag_HasCustomPrinter)
		{
			printf("printer_%s \n", idx->name);			
		}
		else
		{
			printf("0 \n");
		}
		
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


internal_f u32 
gather_metadata_for_reflected(char *file)
{
	// In this step we get the relevant data from the code, so we can create a node list to later on create the generated data to a file for reflection.
	
	SCRATCH();
	
	buffer_t file_buffer = read_file_and_add_null_at_end(temp_arena, file);
	tokenizer this_tokenizer = {};
	this_tokenizer.at = (char*)file_buffer.data;
	
	if(file_buffer.size == 0)
	{
		printf("Failed to load the file \n");
		return 1;
	}
	
	bool parsing = true;
	while(parsing)
	{
		token this_token = get_token(&this_tokenizer);
		switch(this_token.type)
		{
			case Token_EndOfStream:
			{
				parsing = false;
				
			}break;
			
			case Token_Unknown:
			{
				// not printing for now.
			}break;
			
			case Token_Identifier:
			{
				if(token_is_reflected(this_token, &this_tokenizer) )
				{
					parse_reflected(&this_tokenizer);
				}
			}break;
			
			default:
			{
				//				printf("%d: %.*s\n", this_token.type, this_token.text_len, this_token.text);
				
			}break;
		}	
	}
	
	
	return 0;
}
