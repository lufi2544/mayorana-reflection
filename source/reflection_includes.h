/* date = January 11th 2026 3:56 pm */


////////////////////
// Reflection System macros for generating metadata on fields
///

#define MY_CLASS(params)
#define MY_STRUCT(params)
#define MY_ENUM(params)

///////////////////////

// TODO: for now lets add this by hand, but reflection should generate the code for the different types in a file, so then the code can use it.
enum enum_meta_type
{
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
};

// TODO: this I think must be inside the project itself, so hm... I have to think about it.
struct member_definition
{
	enum_meta_type type;
	char* name;
	u32 offset;
};
