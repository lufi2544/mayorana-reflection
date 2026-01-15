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

*/
