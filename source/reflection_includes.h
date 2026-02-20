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
	u32 meta_type;
	u32 offset;
	u32 flags;
};

typedef void (*printer_fn)(char*, void*);

struct type_definition
{
	char* name;
	u32 size;
	
	const member_definition *members;
	u32 member_count;
	
	u32 flags;	
	printer_fn printer;
};

enum member_flag : u32
{		
	MemberFlag_None = 0,
	MemberFlag_IsPointer = 1 << 0,
	MemberFlag_IsEnum = 1 << 1,
	MemberFlag_IsEnumField = 1 << 2,
		
	MemberFlag_Num,
};

enum type_flag : u32
{	
	TypeFlag_IsPrimitive = 1 << 0,
	TypeFlag_IsStruct = 1 << 1,
	TypeFlag_IsClass = 1 << 2,
	TypeFlag_IsEnum = 1 << 3,
	TypeFlag_HasCustomPrinter = 1 << 4,
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
	if(member_value >= enum_definition->member_count)
	{
		return;
	}
	
	
	const member_definition *this_member_definition = enum_definition->members + member_value;
	printf("%s : %s", member_name, this_member_definition->name);			
	
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
		
		if(this_member_type->flags & TypeFlag_IsEnum)
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
		
		// All the primitive functions must have the printers, if we dont have a printer for this struct, we will print its members in stead.
		if(this_member_type->printer)
		{
			this_member_type->printer(this_member_definition->name, member_ptr);
		}
		else
		{
			print_struct(0, type_table, type_table_size, this_member_type, member_ptr);
		}
	}				
	
	printf("} \n");
	
}

/** Print a generic struct with the mayorana reflection system. */
#define MY_PRINT_STRUCT(debug_name, struct_name, ptr) \
print_struct(debug_name, all_type_definitions, ArrayCount(all_type_definitions), &GLUE(definition_of_, struct_name), ptr);

#define MY_ENUM_VALUE(enum_name, value) \
get_enum_value_string(value, &GLUE(definition_of_, enum_name));

