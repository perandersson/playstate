#pragma once

#include "memory/memory.h"
#include "config.h"
#include "types.h"
#include "exception.h"
#include "singleton.h"
#include "functions.h"

#include "processor/processors/linked_list_update_processor.h"
#include "processor/processors/octree_render_processor.h"

#include "filesystem/file_system.h"
#include "script/script_system.h"
#include "window/window.h"
#include "game/game_runner.h"
#include "input/input_system.h"
#include "scene/scene.h"
#include "resources/resource_manager.h"
#include "rendering/render_system.h"

#include "model/model.h"

#include "component/scriptable_component.h"
#include "component/renderable/render_static_model.h"

#ifdef WIN32
#include "filesystem/win32/win32_filesystem.h"
#include "filesystem/win32/win32_file.h"
#include "filesystem/win32/win32_directory.h"

#include "input/win32/win32_input_state.h"
#include "input/win32/win32_input_system.h"

#include "rendering/win32/win32_render_context.h"
#include "rendering/win32/win32_graphics_driver.h"

#include "thread/win32/win32_lock.h"
#include "thread/win32/win32_thread.h"
#include "thread/win32/win32_thread_event.h"
#include "thread/win32/win32_thread_factory.h"

#include "window/win32/win32_window.h"

#endif