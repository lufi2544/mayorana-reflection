

// MAYORANA
#define MAYORANA_DISABLE_LOGS
#include "mayorana.h"

// Reflection files
#include "reflection.h"


int main(int arg_num, char** args)
{
	mayorana_init();
	
	SCRATCH();
	
	buffer_t file_buffer = read_file_and_add_null_at_end(temp_arena, "mayorana.h");
	tokenizer this_tokenizer = {};
	this_tokenizer.at = (char*)file_buffer.data;
	
	if(file_buffer.size == 0)
	{
		printf("Failed to load the file \n");
		return 1;
	}
	
	
	// In this step we get the relevant data from the code, so we can create a node list to later on create the generated data to a file for reflection.
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
				if(token_is_reflected(this_token))
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
	
	
	// Once the data is gathered, we generate it here in the correct oreder.
	generate_meta_enum_for_reflected();
	generate_basic_types_definition();
	generate_member_definition_for_reflected();
	generate_type_definition_for_reflected();
	generate_type_definition_table();
	
	
	
	/*	const type_definition* def = get_type_definition(all_type_definitions, ArrayCount(all_type_definitions), MetaType_buffer_t);
		if(def)
		{
			printf("META_TYPE: %i", def->meta_type);
			
		}
	
*/
		
		
		
		return 0;
	}