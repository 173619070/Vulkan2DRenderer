
// This program shows how to handle multiple windows.
//
// Examples to see before trying this example:
// - HelloWorld
// - EventHandler
//
//
// Windows are the final visual output, and you can create as many as you need.
//
// In this example we'll create a simple program to create new windows on the fly
// and destroy them either individually or all of them at once.
// Running this program will show a green and a red rectangle on a primary window,
// clicking on the green rectangle with the cursor will spawn a new window, clicking on the
// red rectangle will destroy all child windows. Closing the primary window closes the
// application. You can also destroy the child windows individually by pressing their
// "X" button.

#include <VK2D.h>
#include <list>



void AddWindow(
	vk2d::Instance				*	instance,
	std::list<vk2d::Window*>	*	window_list
)
{
	vk2d::WindowCreateInfo ci;
	ci.size = { 400, 400 };
	auto window = instance->CreateOutputWindow( ci );
	window_list->push_back( window );
}



class PrimaryWindowEventHandler : public vk2d::WindowEventHandler
{
public:
	PrimaryWindowEventHandler(
		vk2d::Instance				*	instance,
		glm::ivec2					*	mouse_position,
		vk2d::Rect2f				*	create_button_rect,
		vk2d::Rect2f				*	destroy_button_rect,
		std::list<vk2d::Window*>	*	window_list
	) :
		instance( instance ),
		mouse_position( mouse_position ),
		create_button_rect( create_button_rect ),
		destroy_button_rect( destroy_button_rect ),
		window_list( window_list )
	{}

	void							EventCursorPosition(
		vk2d::Window			*	window,
		glm::dvec2					position
	) override
	{
		*mouse_position = position;
	};

	void							EventMouseButton(
		vk2d::Window			*	window,
		vk2d::MouseButton			button,
		vk2d::ButtonAction			action,
		vk2d::ModifierKeyFlags		modifier_keys
	) override
	{
		if( action == vk2d::ButtonAction::PRESS ) {
			if( create_button_rect->IsPointInside( *mouse_position ) ) {
				AddWindow( instance, window_list );
			}
			if( destroy_button_rect->IsPointInside( *mouse_position ) ) {
				for( auto w : *window_list ) {
					instance->DestroyOutputWindow( w );
				}
				window_list->clear();
			}
		}
	};

	vk2d::Instance				*	instance			= {};

	glm::ivec2					*	mouse_position		= {};

	vk2d::Rect2f				*	create_button_rect	= {};
	vk2d::Rect2f				*	destroy_button_rect	= {};

	std::list<vk2d::Window*>	*	window_list			= {};
};



int main()
{
	vk2d::InstanceCreateInfo instance_create_info {};
	auto instance = vk2d::CreateInstance( instance_create_info );
	if( !instance ) return -1;

	auto filepath = std::filesystem::path("../../data/fonts/ubuntu-font-family-0.83/UbuntuMono-B.ttf");
	auto manager = instance->GetResourceManager();
	vk2d::FontResource* fontResource = manager->LoadFontResource(filepath);

	glm::ivec2 mouse_position {};
	vk2d::Rect2f create_button { 50.0f, 50.0f, 350.0f, 150.0f };
	vk2d::Rect2f destroy_button { 50.0f, 200.0f, 350.0f, 300.0f };
	std::list<vk2d::Window*> window_list {};
	PrimaryWindowEventHandler primary_window_event_handler(
		instance.get(),
		&mouse_position,
		&create_button,
		&destroy_button,
		&window_list
	);

	vk2d::WindowCreateInfo					window_create_info {};
	window_create_info.size					= { 400, 400 };
	window_create_info.event_handler		= &primary_window_event_handler;
	auto primary_window = instance->CreateOutputWindow( window_create_info );
	if( !primary_window ) return -1;

	while( instance->Run() && !primary_window->ShouldClose() ) {

		// Windows do not close automatically, once the user presses the "X" on the
		// window, it's only flagged that it should close.
		// Here we manually check each window and destroy the onces that should close.
		{
			auto it = window_list.begin();
			while( it != window_list.end() ) {
				if( ( *it )->ShouldClose() ) {
					instance->DestroyOutputWindow( *it );
					it = window_list.erase( it );
				} else {
					++it;
				}
			}
		}

		if( !primary_window->BeginRender() ) return -1;

		glm::vec2 scale = glm::vec2(0.9f, 0.3f);

		primary_window->DrawString(fontResource,
			{ 0,0 },
			"hello world",
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			0.0f,
			scale
		);

		/// ////////////

		glm::vec2 pos1 = { 0,40 };
		glm::vec2 pos2 = { 400,40 };
		primary_window->DrawLine(
			pos1, pos2,
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);

		scale = { 0.8f,0.4f };
		primary_window->DrawString(fontResource,
			{ 0,40 },
			"hello world",
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			0.0f,
			scale
		);
		/// ////////////

		 pos1 = { 0,80 };
		 pos2 = { 400,80 };
		primary_window->DrawLine(
			pos1, pos2,
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);
		scale = { 0.7f,0.5f };
		primary_window->DrawString(fontResource,
			{ 0,80 },
			"hello world",
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			0.0f,
			scale
		);

		/// ////////////

		 pos1 = { 0,120 };
		 pos2 = { 400,120 };
		primary_window->DrawLine(
			pos1, pos2,
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);

		scale = { 0.6f,0.6f };
		primary_window->DrawString(fontResource,
			{ 0,120 },
			"hello world",
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			0.0f,
			scale
		);
		/// ////////////

		pos1 = { 0,160 };
		pos2 = { 400,160 };
		primary_window->DrawLine(
			pos1, pos2,
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);
		scale = { 0.5f,0.7f };
		primary_window->DrawString(fontResource,
			{ 0,160 },
			"hello world",
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			0.0f,
			scale
		);

		/// ////////////

		pos1 = { 0,200 };
		pos2 = { 400,200 };
		primary_window->DrawLine(
			pos1, pos2,
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);

		scale = { 0.4f,0.8f };
		primary_window->DrawString(fontResource,
			{ 0,200 },
			"hello world",
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			0.0f,
			scale
		);
		/// ////////////

		pos1 = { 0,240 };
		pos2 = { 400,240 };
		primary_window->DrawLine(
			pos1, pos2,
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);
		scale = { 0.3f,0.9f };
		primary_window->DrawString(fontResource,
			{ 0,240 },
			"hello world",
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			0.0f,
			scale
		);


		//primary_window->DrawRectangle(
		//	destroy_button,
		//	true,
		//	{ 1.0f, 0.3f, 0.2f, 1.0f }
		//);

		if( !primary_window->EndRender() ) return -1;

		for( auto w : window_list ) {
			w->BeginRender();

			w->DrawEllipse(
				{ 50.0f, 50.0f,
				350.0f, 350.0f },
				true,
				64.0f,
				{ 0.3f, 0.6f, 1.0f, 1.0f }
			);

			w->EndRender();
		}
	}

	return 0;
}
