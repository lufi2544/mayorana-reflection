

// MAYORANA
#define MAYORANA_DISABLE_LOGS
#include "mayorana.h"

// Reflection files
#include "reflection.h"

#include "example.cpp"

#define MY_PRINT_STRUCT(debug_name, struct_name, ptr) \
print_struct(debug_name, all_type_definitions, ArrayCount(all_type_definitions), &GLUE(definition_of_, struct_name), ptr);


int main(int arg_num, char** args)
{
	mayorana_init();
			
	SCRATCH();
	
	
	//////// TEST
	char* enum_value = MY_ENUM_VALUE(enemy_type, 2);
	
	printf("PEPE %s \n", enum_value);
	
	string_t name = STRING_V(temp_arena, "ishak");
	u32 player_id = 32;	
	game_data game = { &name, player_id };	
	game.m_enemy_type = EnemyType_dragon;
	
	MY_PRINT_STRUCT("main_game_data", game_data, &game);
	////////// 
	
	
	// Generates internal data for reflected classes.
	gather_metadata_for_reflected("mayorana.h");	
	gather_metadata_for_reflected("game.h");	

	// Once the data is gathered, we generate it here in the correct oreder.
	generate_meta_enum_for_reflected();
	generate_basic_types_definition();
	generate_member_definition_for_reflected();
	generate_type_definition_for_reflected();
	generate_type_definition_table();
			
		
		
		return 0;
	}