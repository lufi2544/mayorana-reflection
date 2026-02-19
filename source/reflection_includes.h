/* date = January 11th 2026 3:56 pm */


////////////////////
// Reflection System macros for generating metadata on fields
///

#define MY_CLASS(...)
#define MY_STRUCT(...)
#define MY_ENUM(...)
#define MY_PROPERTY(...)


/**
 * ****IMPORTANT ****
 * 
 * As this reflection library is inteded to use among my projects, I don't want to specify in every of them
 * the way of printing and handling this base primitives from "mayorana.h", and in this case the intention is
 * to include this header and don't worrying about how the string_t or other "mayorana.h" is handled, we need to figure
 * out a way so at this lib's compile time, we know the metatype of what we intend to print, and to handle it.
 * 
 * My solution for this is to generate a MetaType_ enum in the reflection in the same order than this one here, so we don't break the 
 * enumeration in the types. As this is my personal projects reflection system is fine.CONS: whenever I add a reflected struct from "mayorana.h"
 * I need to be careful in the order.
*/
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



struct member_definition
{
	char* name;
	u32 meta_type;
	u32 offset;
	u32 flags;
};

struct type_definition
{
	char* name;
	u32 size;
	
	const member_definition *members;
	u32 member_count;
};

enum member_flag : u32
{		
	MemberFlag_None = 0,
	MemberFlag_IsPointer = 1 << 0,
	MemberFlag_IsEnum = 1 << 1,
	MemberFlag_IsEnumField = 1 << 2,
		
	MemberFlag_Num,
};


// TODO: should we pass the size for the table here?
global_f const type_definition*
get_type_definition(const type_definition** types_definition_table, u32 table_size, u32 _meta_type)
{
	if((!types_definition_table) || 
	   (table_size <= (_meta_type)))
	{
		return 0;
	}
	const type_definition* result = types_definition_table[_meta_type];	
	return result;
} 


global_f void
print_enum_value(char *member_name, u32 member_value, const type_definition *enum_definition)
{	
	if(member_value > enum_definition->member_count)
	{
		return;
	}
	
	for(u32 member_idx = 0;
		member_idx < enum_definition->member_count;
		++member_idx)		
	{
		if(member_idx == member_value)
		{						
			const member_definition *this_member_definition = enum_definition->members + member_idx;
			printf("%s : %s", member_name, this_member_definition->name);			
		}				
	}				
	
	printf("} \n");
}


global_f char*
get_enum_value_string(u32 enum_value, const type_definition *enum_definition)
{	
	if(enum_value > enum_definition->member_count)
	{
		return 0;
	}
	
	for(u32 member_idx = 0;
		member_idx < enum_definition->member_count;
		++member_idx)		
	{
		if(member_idx == enum_value)
		{						
			const member_definition *this_member_definition = enum_definition->members + member_idx;
			return this_member_definition->name;
		}				
	}				
	
	return 0;
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
		member_idx <  struct_definition->member_count;
		++member_idx)		
	{				
		const member_definition *this_member_definition = struct_definition->members + member_idx;
		const type_definition *this_member_type = get_type_definition(type_table, type_table_size, this_member_definition->meta_type);
		
		
		u8* struct_address = (u8*)struct_ptr;
		u8* member_ptr = struct_address + this_member_definition->offset;
		
		if(this_member_definition->flags & MemberFlag_IsEnum)
		{
			// TODO: Maybe encoding the size somewhere so we can cast to the proper size here.
			u32* enum_field_value = (u32*)member_ptr;
			print_enum_value(this_member_definition->name, *enum_field_value, this_member_type);
			continue;
		}
		
		
		// Dereferencing a ptr in this case, so we treat the ptr passed to this function as a ** .
		if(this_member_definition->flags & MemberFlag_IsPointer)
		{
			u8** member_ptr_ptr = (u8**)member_ptr;
			u8* real_ptr = *member_ptr_ptr;
			member_ptr = real_ptr;
		}
		
		primitive_meta_type this_meta_type = (primitive_meta_type)this_member_definition->meta_type;
		switch(this_meta_type)
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
				s8* value = (s8*)member_ptr;
				printf("%s: %d \n", this_member_definition->name, *value);	
			}break;
			
			case PrimitiveType_s16:
			{
				s16* value = (s16*)member_ptr;
				printf("%s: %d \n", this_member_definition->name, *value);	
			}break;
			
			case PrimitiveType_s32:
			{
				s32* value = (s32*)member_ptr;
				printf("%s: %d \n", this_member_definition->name, *value);	
			}break;
			
			case PrimitiveType_s64:
			{
				s64* value = (s64*)member_ptr;
				printf("%s: %llu \n", this_member_definition->name, *value);	
			}break;
			
			case PrimitiveType_f32:
			{
				f32* value = (f32*)member_ptr;
				printf("%s: %f \n", this_member_definition->name, *value);	
			}break;
			
			case PrimitiveType_f64:
			{
				f64* value = (f64*)member_ptr;
				printf("%s: %f \n", this_member_definition->name, *value);	
			}break;
			
			case PrimitiveType_bool:
			{
				bool* value = (bool*)member_ptr;
				printf("%s: %i \n", this_member_definition->name, *value);	
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
	
	printf("} \n");
	
}

/** Print a generic struct with the mayorana reflection system. */
#define MY_PRINT_STRUCT(debug_name, struct_name, ptr) \
print_struct(debug_name, all_type_definitions, ArrayCount(all_type_definitions), &GLUE(definition_of_, struct_name), ptr);

#define MY_ENUM_VALUE(enum_name, value) \
get_enum_value_string(value, &GLUE(definition_of_, enum_name));

