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
	char* name;
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

enum primitive_meta_type : u32
{
	PrimitiveType_None,
	PrimitiveType_u8,
	PrimitiveType_u16,
	PrimitiveType_u32,
	PrimitiveType_u64,
	PrimitiveType_s8,
	PrimitiveType_s16,
	PrimitiveType_s32,
	PrimitiveType_s64,
	PrimitiveType_f32,
	PrimitiveType_f64,
	PrimitiveType_bool,	
	PrimitiveType_buffer_t,		
	PrimitiveType_string_t,	
	// Add as many as Mayorana supports from mayoran.h
};


// TODO: should we pass the size for the table here?
global_f const type_definition*
get_type_definition(const type_definition** types_definition_table, u32 table_size, u32 meta_type)
{
	if((!types_definition_table) || 
	   (table_size <= (meta_type)))
	{
		return 0;
	}
	const type_definition* result = types_definition_table[meta_type];	
	return result;
} 


global_f void
print_struct(char *struct_name, const type_definition **type_table, u32 type_table_size, const type_definition *struct_definition, void *struct_ptr)
{			
	if(struct_name)
	{
		printf(" %s::%s\n", struct_definition->name, struct_name);		
		printf("{ \n");
	}
	else
	{
		printf("{ \n");
	}
	
	for(u32 member_idx = 0;
		member_idx < struct_definition->member_count;
		++member_idx)		
	{				
		const member_definition *this_member_definition = struct_definition->members + member_idx;		
		const type_definition * this_member_type = get_type_definition(type_table, type_table_size, this_member_definition->type_definition_id);
		
		u8* struct_address = (u8*)struct_ptr;
		u8* member_ptr = struct_address + this_member_definition->offset;
		
		// Dereferencing a ptr in this case, so we treat the ptr passed to this function as a ** .
		if(this_member_definition->flags & MemberFlag_IsPointer)
		{
			u8** member_ptr_ptr = (u8**)member_ptr;
			u8* real_ptr = *member_ptr_ptr;
			member_ptr = real_ptr;
		}
		
		primitive_meta_type meta_type = (primitive_meta_type)this_member_type->meta_type;
		switch(meta_type)
		{							
			// TODO: Make sure if any reflected native type is added, we inticate that.
			case PrimitiveType_u8:
			{
				printf("%s: %u \n", this_member_definition->name, *member_ptr);
			}break;
			
			case PrimitiveType_u16:
			{
				u16* value = (u16*)member_ptr;
				printf("%s: %u \n", this_member_definition->name, *value);
			}break;
			
			case PrimitiveType_u32:
			{
				u32* value = (u32*)member_ptr;
				printf("%s: %u \n", this_member_definition->name, *value);
			}break;
			
			case PrimitiveType_u64:
			{
				u64* value = (u64*)member_ptr;
				printf("%s: %llu \n", this_member_definition->name, *value);	
			}break;
			
			case PrimitiveType_s8:
			{
				
			}break;
			
			case PrimitiveType_s16:
			{
				
			}break;
			
			case PrimitiveType_s32:
			{
				
			}break;
			
			case PrimitiveType_s64:
			{
				
			}break;
			
			case PrimitiveType_f32:
			{
				
			}break;
			
			case PrimitiveType_f64:
			{
				
			}break;
			
			case PrimitiveType_bool:
			{
				
			}break;
			
			case PrimitiveType_string_t:
			{
				string_t *value = (string_t*)member_ptr;
				printf("%s: %s \n", this_member_definition->name, *(*value));
				
			}break;						
			
			
			default:
			{			
				print_struct(0, type_table, type_table_size, this_member_type, member_ptr);
			}break;
		}
	}				
	
	printf(" } \n");
	
}



MY_STRUCT()
struct game_data
{
	MY_PROPERTY()
		string_t* player_name;
	
	MY_PROPERTY()
		u32 player_id;
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
	
	MetaType_buffer_t, 
	MetaType_string_t, 
	MetaType_game_data, 
	MetaType_num 
}; 

const type_definition definition_of_u8 = 
{ 
	"u8", 
	1, 
	sizeof(u8), 
	0, 
	0 
};

const type_definition definition_of_u16 = 
{ 
	"u16", 
	2, 
	sizeof(u16), 
	0, 
	0 
};

const type_definition definition_of_u32 = 
{ 
	"u32", 
	3, 
	sizeof(u32), 
	0, 
	0 
};

const type_definition definition_of_u64 = 
{ 
	"u64", 
	4, 
	sizeof(u64), 
	0, 
	0 
};

const type_definition definition_of_s8 = 
{ 
	"s8", 
	5, 
	sizeof(s8), 
	0, 
	0 
};

const type_definition definition_of_s16 = 
{ 
	"s16", 
	6, 
	sizeof(s16), 
	0, 
	0 
};

const type_definition definition_of_s32 = 
{ 
	"s32", 
	7, 
	sizeof(s32), 
	0, 
	0 
};

const type_definition definition_of_s64 = 
{ 
	"s64", 
	8, 
	sizeof(s64), 
	0, 
	0 
};

const type_definition definition_of_f32 = 
{ 
	"f32", 
	9, 
	sizeof(f32), 
	0, 
	0 
};

const type_definition definition_of_f64 = 
{ 
	"f64", 
	10, 
	sizeof(f64), 
	0, 
	0 
};

const type_definition definition_of_bool = 
{ 
	"bool", 
	11, 
	sizeof(bool), 
	0, 
	0 
};

const member_definition members_of_buffer_t[] = 
{
	{"size", MetaType_u64, OFFSET_OF(buffer_t, size), 0}, 
	{"data", MetaType_u8, OFFSET_OF(buffer_t, data), MemberFlag_IsPointer}, 
};

const member_definition members_of_string_t[] = 
{
	{"size", MetaType_u32, OFFSET_OF(string_t, size), 0}, 
	{"buffer", MetaType_buffer_t, OFFSET_OF(string_t, buffer), 0}, 
};

const member_definition members_of_game_data[] = 
{
	{"player_id", MetaType_u32, OFFSET_OF(game_data, player_id), 0}, 
	{"player_name", MetaType_string_t, OFFSET_OF(game_data, player_name), MemberFlag_IsPointer}, 
};

const type_definition definition_of_buffer_t 
{ 
	"buffer_t",
	12, 
	sizeof(buffer_t), 
	members_of_buffer_t, 
	ArrayCount(members_of_buffer_t) 
}; 

const type_definition definition_of_string_t 
{ 
	"string_t",
	13, 
	sizeof(string_t), 
	members_of_string_t, 
	ArrayCount(members_of_string_t) 
}; 

const type_definition definition_of_game_data 
{ 
	"game_data",
	14, 
	sizeof(game_data), 
	members_of_game_data, 
	ArrayCount(members_of_game_data) 
}; 


const type_definition* all_type_definitions[] = 
{
	0, 
	&definition_of_u8, 
	&definition_of_u16, 
	&definition_of_u32, 
	&definition_of_u64, 
	&definition_of_s8, 
	&definition_of_s16, 
	&definition_of_s32, 
	&definition_of_s64, 
	&definition_of_f32, 
	&definition_of_f64, 
	&definition_of_bool, 
	
	&definition_of_buffer_t, 
	&definition_of_string_t, 
	&definition_of_game_data, 
};