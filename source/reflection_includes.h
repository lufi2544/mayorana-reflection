/* date = January 11th 2026 3:56 pm */


////////////////////
// Reflection System macros for generating metadata on fields
///

#define MY_CLASS(...)
#define MY_STRUCT(...)
#define MY_ENUM(params)
#define MY_PROPERTY(...)

struct type_definition;

struct member_definition
{
	char* name;
	const type_definition *type;
	u32 offset;
};


/** in this case we will generate the kind_type for each project, and we will compare agains that with the generated enum like 
* class_kind == ClassMeta_game_data, and so on
*/
struct type_definition
{
	u32 meta_type;
	u32 size;
	
	const member_definition *members;
	u32 member_count;
};

/*
MY_STRUCT()
struct game_data
{ 
	MY_PROPERTY()
		u64 players;
	
	MY_PROPERTY()
		u64 players_num;
};


MY_STRUCT()
struct player_data
{ 
	MY_PROPERTY()
		u64 players;
	
	MY_PROPERTY()
		u64 players_num;
};


MY_STRUCT()
struct map_data
{ 
	MY_PROPERTY()
		u64 players;
	
	MY_PROPERTY()
		u64 players_num;
};

MY_STRUCT()
struct render_data
{ 
	MY_PROPERTY()
		u64 players;
	
	MY_PROPERTY()
		u64 players_num;
};


const type_definition definition_of_u8 = 
{ 
	1, 
	sizeof(u8), 
	0, 
	0 
};

const type_definition definition_of_u16 = 
{ 
	2, 
	sizeof(u16), 
	0, 
	0 
};

const type_definition definition_of_u32 = 
{ 
	3, 
	sizeof(u32), 
	0, 
	0 
};

const type_definition definition_of_u64 = 
{ 
	4, 
	sizeof(u64), 
	0, 
	0 
};

const type_definition definition_of_s8 = 
{ 
	5, 
	sizeof(s8), 
	0, 
	0 
};

const type_definition definition_of_s16 = 
{ 
	6, 
	sizeof(s16), 
	0, 
	0 
};

const type_definition definition_of_s32 = 
{ 
	7, 
	sizeof(s32), 
	0, 
	0 
};

const type_definition definition_of_s64 = 
{ 
	8, 
	sizeof(s64), 
	0, 
	0 
};

const type_definition definition_of_f32 = 
{ 
	9, 
	sizeof(f32), 
	0, 
	0 
};

const type_definition definition_of_f64 = 
{ 
	10, 
	sizeof(f64), 
	0, 
	0 
};

const type_definition definition_of_bool = 
{ 
	11, 
	sizeof(bool), 
	0, 
	0 
};

const member_definition members_of_game_data[] = 
{
	{"players", &definition_of_u64, OFFSET_OF(game_data, players)}, 
	{"players_num", &definition_of_u64, OFFSET_OF(game_data, players_num)}, 
};

const type_definition definition_of_game_data  = 
{ 
	12,
	sizeof(game_data), 
	members_of_game_data, 
	ArrayCount(members_of_game_data) 
};

const member_definition members_of_player_data[] = 
{
	{"players", &definition_of_u64, OFFSET_OF(player_data, players)}, 
	{"players_num", &definition_of_u64, OFFSET_OF(player_data, players_num)}, 
};

const type_definition definition_of_player_data  = 
{ 
	13,
	sizeof(player_data), 
	members_of_player_data, 
	ArrayCount(members_of_player_data) 
};

const member_definition members_of_map_data[] = 
{
	{"players", &definition_of_u64, OFFSET_OF(map_data, players)}, 
	{"players_num", &definition_of_u64, OFFSET_OF(map_data, players_num)}, 
};

const type_definition definition_of_map_data  = 
{ 
	14,
	sizeof(map_data), 
	members_of_map_data, 
	ArrayCount(members_of_map_data) 
};

const member_definition members_of_render_data[] = 
{
	{"players", &definition_of_u64, OFFSET_OF(render_data, players)}, 
	{"players_num", &definition_of_u64, OFFSET_OF(render_data, players_num)}, 
};

const type_definition definition_of_render_data  = 
{ 
	15,
	sizeof(render_data), 
	members_of_render_data, 
	ArrayCount(members_of_render_data) 
};


enum meta_type : u32 
{
	MetaType_none, 
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
	MetaType_bool, 
	MetaType_game_data, 
	MetaType_player_data, 
	MetaType_map_data, 
	MetaType_render_data, 
}; 
*/