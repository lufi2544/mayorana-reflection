/* date = January 11th 2026 3:56 pm */


////////////////////
// Reflection System macros for generating metadata on fields
///

#define MY_CLASS(params)
#define MY_STRUCT(params)
#define MY_ENUM(params)
#define MY_PROPERTY(params)

struct member_definition
{
	u8 type;
	char* name;
	u32 offset;
};
