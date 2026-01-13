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
	type_definition *type;
	char* name;
	u32 offset;
};

/*
struct vec3
{
    float x, y, z;
};

const member_definition VEC3_MEMBERS[] = {
    { "x", offsetof(vec3, x), &TYPE_F32_DEF },
    { "y", offsetof(vec3, y), &TYPE_F32_DEF },
    { "z", offsetof(vec3, z), &TYPE_F32_DEF },
};

const type_definition VEC3_TYPE = {
    TYPE_STRUCT,
    sizeof(vec3),
    .struct_type = {
        VEC3_MEMBERS,
        3
    }
};
*/

struct type_definition
{
	type_kind kind;
	u32 size;
	
	// this is temporary for structs
	member_definition *members;
	u32 member_count;		
};