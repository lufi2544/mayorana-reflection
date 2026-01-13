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
	type_definition *type;
	u32 offset;
};

/*

1- we first have to add the macros to the desired classes.

MY_STRUCT(all)
struct vec3
{
    float x, y, z;
};

2 - then the Metadata_types are created


const member_definition VEC3_MEMBERS[] = {
    { "x", offsetof(vec3, x), &TYPE_F32_DEF },
    { "y", offsetof(vec3, y), &TYPE_F32_DEF },
    { "z", offsetof(vec3, z), &TYPE_F32_DEF },
};

const type_definition TYPE_VEC3_DEF = {
    TYPE_STRUCT,
    sizeof(vec3),
    .struct_type = {
        VEC3_MEMBERS,
        3
    }
};


struct GameData
{
   f32 health;
   vec3 location;
};


const member_definition GAME_DATA_MEMBERS []= 
{
   { "health", offsetof(GameData, health), &TYPE_F32_DEF}
   { "location", offsetof(GameData, location), &TYPE_VEC3_DEF}
};


const type_definition GAME_DATA_TYPE = 
{
  TYPE_STRUCT,
  sizeof(GameData),
  GAME_DATA_MEMBERS,
2
}


*/

struct type_definition
{
	type_kind kind;
	u32 size;
	
	// this is temporary for structs
	member_definition *members;
	u32 member_count;		
};