/* date = January 11th 2026 3:56 pm */


////////////////////
// Reflection System macros for generating metadata on fields
///

#define MY_CLASS(params)
#define MY_STRUCT(params)
#define MY_ENUM(params)
#define MY_PROPERTY(params)

enum type_kind
{
	Type_u8,
	Type_u16,
	Type_u32,
	Type_u64,
	Type_f32,
	Type_BOOL,
	
	Type_Struct,
};


struct type_definition;

struct member_definition
{
	char* name;
	const type_definition *type;
	u32 offset;
};


struct type_definition
{
	type_kind kind;
	u32 size;
	
	// this is temporary for structs
	const member_definition *members;
	u32 member_count;		
};

/*

1- we first have to add the macros to the desired classes.

MY_STRUCT(all)
struct vec3
{
    float x, y, z;
};

2 - then the Metadata_types are created

struct vec3
{
	float x, y, z;
};



const type_definition TYPE_F32_DEF = {
    Type_f32,
    sizeof(vec3),
	0,
	0
};


const member_definition VEC3_MEMBERS[] = {
    { "x", &TYPE_F32_DEF, (u64)&((vec3*)0)->x },
    { "y", &TYPE_F32_DEF, OFFSET_OF(vec3, y) },
    { "z", &TYPE_F32_DEF, OFFSET_OF(vec3, z) },
};

const type_definition TYPE_VEC3_DEF = {
    Type_Struct,
    sizeof(vec3),
	VEC3_MEMBERS,
    ArrayCount(VEC3_MEMBERS) 
};


struct GameData
{
	f32 health;
	vec3 location;
};


const member_definition GAME_DATA_MEMBERS []= 
{
	{ "health", &TYPE_F32_DEF, OFFSET_OF(GameData, health)},
	{ "location", &TYPE_VEC3_DEF, OFFSET_OF(GameData, location)}
};


const type_definition GAME_DATA_TYPE = 
{
	Type_Struct,
	sizeof(GameData),
	GAME_DATA_MEMBERS,
	ArrayCount(GAME_DATA_MEMBERS)
};

*/
