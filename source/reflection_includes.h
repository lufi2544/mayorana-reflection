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
	PrimitiveType_string_t,	
	// Add as many as Mayorana supports from mayoran.h
};


// TODO: should we pass the size for the table here?
global_f const type_definition*
get_type_definition(const type_definition** types_definition_table, u32 table_size, u32 meta_type)
{
	if((!types_definition_table) || 
	   (table_size <= (meta_type - 1)))
	{
		return 0;
	}
	const type_definition* result = types_definition_table[meta_type - 1];	
	return result;
} 


global_f void
print_struct(const type_definition **type_table, u32 type_table_size, const member_definition *struct_definition, void *struct_ptr)
{
	
	const type_definition *MemberTypeDefinition = get_type_definition(type_table, type_table_size, struct_definition->type_definition_id);
	if(!MemberTypeDefinition)
	{
		return;
	}
	
	// this is a primitive type
	if(MemberTypeDefinition->member_count == 0)
	{
		
		primitive_meta_type meta_type = (primitive_meta_type)MemberTypeDefinition->meta_type;
		u8* struct_address = (u8*)struct_ptr;		
		switch(meta_type)
		{			
			case PrimitiveType_u8:
			{
				u8* value = struct_address + struct_definition->offset;
				printf("%s: %u", struct_definition->name, *value);
			}break;
			
			case PrimitiveType_u16:
			{
				
			}break;
			
			case PrimitiveType_u32:
			{
				
			}break;
			
			case PrimitiveType_u64:
			{
				
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
				
			}break;						
		}
	}
	else
	{
		
		for(u32 member_idx = 0;
			member_idx < MemberTypeDefinition->member_count;
			++member_idx)		
		{
			const member_definition *MemberDefinition = MemberTypeDefinition->members + member_idx;
			
			// Dereferencing a ptr in this case.
			if(MemberDefinition->flags & MemberFlag_IsPointer)
			{
				void** struct_ptr_ptr = (void**)struct_ptr;
				void* real_ptr = *struct_ptr_ptr;
				struct_ptr = real_ptr;
			}
			
			print_struct(type_table, type_table_size, MemberDefinition, struct_ptr);			
		}
	}
}