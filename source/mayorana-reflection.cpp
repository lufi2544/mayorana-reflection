

// MAYORANA
#include "mayorana.h"

// Reflection project files
#include "reflection.h"


int main(int arg_num, char** args)
{
	mayorana_init();
	
	SCRATCH();
	
	buffer_t file_buffer = read_file_and_add_null_at_end(temp_arena, "game.h");
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
		switch(this_token.token_type)
		{
			case Token_EndOfStream:
			{
				parsing = false;
				
			}break;
			
			case Token_Unknown:
			{
				// not printing for now.
			}break;
			
			default:
			{
				printf("%d: %.*s\n", this_token.token_type, this_token.text_len, this_token.text);
				
			}break;
		}	
	}
	
	
	return 0;
}