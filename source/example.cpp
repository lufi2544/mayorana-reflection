


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
	sizeof(u8), 
	0, 
	0 
};

const type_definition definition_of_u16 = 
{ 
	"u16", 
	sizeof(u16), 
	0, 
	0 
};

const type_definition definition_of_u32 = 
{ 
	"u32", 
	sizeof(u32), 
	0, 
	0 
};

const type_definition definition_of_u64 = 
{ 
	"u64", 
	sizeof(u64), 
	0, 
	0 
};

const type_definition definition_of_s8 = 
{ 
	"s8", 
	sizeof(s8), 
	0, 
	0 
};

const type_definition definition_of_s16 = 
{ 
	"s16", 
	sizeof(s16), 
	0, 
	0 
};

const type_definition definition_of_s32 = 
{ 
	"s32", 
	sizeof(s32), 
	0, 
	0 
};

const type_definition definition_of_s64 = 
{ 
	"s64", 
	sizeof(s64), 
	0, 
	0 
};

const type_definition definition_of_f32 = 
{ 
	"f32", 
	sizeof(f32), 
	0, 
	0 
};

const type_definition definition_of_f64 = 
{ 
	"f64", 
	sizeof(f64), 
	0, 
	0 
};

const type_definition definition_of_bool = 
{ 
	"bool", 
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
	sizeof(buffer_t), 
	members_of_buffer_t, 
	ArrayCount(members_of_buffer_t) 
}; 

const type_definition definition_of_string_t 
{ 
	"string_t",
	sizeof(string_t), 
	members_of_string_t, 
	ArrayCount(members_of_string_t) 
}; 

const type_definition definition_of_game_data 
{ 
	"game_data",
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