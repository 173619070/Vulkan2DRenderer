#version 450
#extension GL_KHR_vulkan_glsl : enable



// Defines position where user main function will be imported.
#define USER_SHADER_VERT_MAIN_CALL



// Vertex.
struct Vertex {
	vec2		coords;
	vec2		UVs;
	vec4		color;
	float		point_size;
	uint		single_texture_channel;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shader program interface.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Set 0: Window frame data.
layout(std140, set=0, binding=0) uniform			WindowFrameData {
	vec2		multiplier;
	vec2		offset;
} window_frame_data;

// Set 1: Transformation buffer.
layout(std430, set=1, binding=0) readonly buffer	TransformationBuffer {
	mat4		ssbo[];
} transformation_buffer;

// Set 3: Vertex buffer.
layout(std430, set=3, binding=0) readonly buffer	VertexBuffer {
	Vertex		ssbo[];
} vertex_buffer;

// Push constants.
layout(std140, push_constant) uniform PushConstants {
	uint		transformation_offset;			// Offset into the transformation buffer.
	uint		index_offset;					// Offset into the index buffer.
	uint		index_count;					// Amount of indices this shader should handle.
	uint		vertex_offset;					// Offset to first vertex in vertex buffer.
} push_constants;

// Output to fragment shader
layout(location=0) out		vec2	fragment_output_UV;
layout(location=1) out		vec4	fragment_output_color;
layout(location=2) out		vec2	fragment_output_original_coords;
layout(location=3) out flat	uint	fragment_output_vertex_index;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Entrypoint.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main()
{
	// These are always done.
	vec4 raw_vertex_coords			= vec4( vertex_buffer.ssbo[ gl_VertexIndex ].coords, 0.0, 1.0 );
	fragment_output_UV				= vertex_buffer.ssbo[ gl_VertexIndex ].UVs;
	fragment_output_color			= vertex_buffer.ssbo[ gl_VertexIndex ].color;
	fragment_output_original_coords	= raw_vertex_coords.xy;
	fragment_output_vertex_index	= gl_VertexIndex;

	mat4 transformation_matrix		= transformation_buffer.ssbo[ gl_InstanceIndex + push_constants.transformation_offset ];

	// User main call goes here.
	// These may be user provided. Current code is the fallback if user does not provide vertex shader. 
	// Does the user write to gl_Position or return the position from main?
	// - User may want to modify multiple values... So it's probably a good idea to let the user write to gl_Position and others directly.
	vec2 transformed_vertex_coords	= ( transformation_matrix * raw_vertex_coords ).xy;
	vec2 viewport_vertex_coords		= transformed_vertex_coords * window_frame_data.multiplier + window_frame_data.offset;

	gl_Position						= vec4( viewport_vertex_coords, 0.5, 1.0 );
	gl_PointSize					= vertex_buffer.ssbo[ gl_VertexIndex ].point_size;
}
