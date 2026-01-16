/* date = January 11th 2026 3:56 pm */


////////////////////
// Reflection System macros for generating metadata on fields
///

#define MY_CLASS(...)
#define MY_STRUCT(...)
#define MY_ENUM(...)
#define MY_PROPERTY(...)

struct member_definition
{
	char* name;
	u32 type_definition_id;
	u32 offset;
	u32 flags;
};

struct type_definition
{
	u32 meta_type;
	u32 size;
	
	const member_definition *members;
	u32 member_count;
};



enum member_flag : u32
{		
	MemberFlag_None = 0,
	MemberFlag_IsPointer = 1 << 0,
	
	
	MemberFlag_Num,
};