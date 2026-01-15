/* date = January 11th 2026 3:56 pm */


////////////////////
// Reflection System macros for generating metadata on fields
///

#define MY_CLASS(...)
#define MY_STRUCT(...)
#define MY_ENUM(params)
#define MY_PROPERTY(...)

enum type_kind
{
	Type_u8,
	Type_u16,
	Type_u32,
	Type_u64,
	Type_s8,
	Type_s16,
	Type_s32,
	Type_s64,
	Type_f32,
	Type_f64,
	Type_bool,
	
	Type_Struct,
};


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
	type_kind kind;
	u32 meta_type;
	u32 size;
	
	// this is temporary for structs
	const member_definition *members;
	u32 member_count;
};

/*

 <-----> 1- we first have to add the macros to the desired classes.

MY_STRUCT(all)
struct vec3
{
    float x, y, z;
};

 <-----> 2 - Generate the "base types" type definitions first, then the custom structs one.

 <-----> 
    - 

 
 <-----> 3 - then the Metadata_types are created

struct vec3
{
	float x, y, z;
};


const type_definition f32_Type_Def = {
    Type_f32,
    sizeof(vec3),
	0,
	0
};


const member_definition *vec3_Members[] = {
    { "x", &f32_Type_Def, (u64)&((vec3*)0)->x },
    { "y", &f32_Type_Def, OFFSET_OF(vec3, y) },
    { "z", &f32_Type_Def, OFFSET_OF(vec3, z) },
};

const type_definition TYPE_VEC3_DEF = {
    Type_Struct,
    sizeof(vec3),
	vec3_Members,
    ArrayCount(vec3_Members) 
};

struct GameData
{
	f32 health;
	vec3 location;
};

const member_definition *GameData_Members []= 
{
	{ "health", &TYPE_F32_DEF, OFFSET_OF(GameData, health)},
	{ "location", &TYPE_VEC3_DEF, OFFSET_OF(GameData, location)}
};


const type_definition GameData_Type_Definition = 
{
	Type_Struct,
	sizeof(GameData),
	GameData_Members,
	ArrayCount(GameData_Members)
};

*/


/*
MY_STRUCT()
struct game_data
{ 
	MY_PROPERTY()
		u64 players;
	
	MY_PROPERTY()
		u64 players_num;
};


// TODO Investigate the member_definition* stuff vs no-pointer
const type_definition definition_of_u64 = 
{
	Type_u64,
	sizeof(u64),
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
	Type_Struct, 
	sizeof(game_data), 
	members_of_game_data, 
	ArrayCount(members_of_game_data) 
};*/


const type_definition definition_of_u8 = 
{ 
	Type_u8, 
	0,
	0,
	0,
	0,
};

const type_definition definition_of_u16 = 
{ 
	Type_u16, 
	0,
	0,
	0,
	0,
};

const type_definition definition_of_u32 = 
{ 
	Type_u32, 
	0,
	0,
	0,
	0,
};

const type_definition definition_of_u64 = 
{ 
	Type_u64, 
	0,
	0,
	0,
	0,
};

const type_definition definition_of_s8 = 
{ 
	Type_s8, 
	0,
	0,
	0,
	0,
};

const type_definition definition_of_s16 = 
{ 
	Type_s16, 
	0,
	0,
	0,
	0,
};

const type_definition definition_of_s32 = 
{ 
	Type_s32, 
	0,
	0,
	0,
	0,
};

const type_definition definition_of_s64 = 
{ 
	Type_s64, 
	0,
	0,
	0,
	0,
};

const type_definition definition_of_f32 = 
{ 
	Type_f32, 
	0,
	0,
	0,
	0,
};

const type_definition definition_of_f64 = 
{ 
	Type_f64, 
	0,
	0,
	0,
	0,
};

const type_definition definition_of_bool = 
{ 
	Type_bool, 
	0,
	0,
	0,
	0,
};

const member_definition members_of_game_data[] = 
{
	{"players", &definition_of_u64, OFFSET_OF(game_data, players)}, 
	{"players_num", &definition_of_u64, OFFSET_OF(game_data, players_num)}, 
};

const type_definition definition_of_game_data  = 
{ 
	Type_Struct, 
	1,
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
	Type_Struct, 
	2,
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
	Type_Struct, 
	3,
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
	Type_Struct, 
	4,
	sizeof(render_data), 
	members_of_render_data, 
	ArrayCount(members_of_render_data) 
};


enum meta_type : u32 
{
	MetaType_none, 
	MetaType_game_data , 
	MetaType_player_data , 
	MetaType_map_data , 
	MetaType_render_data , 
}; 