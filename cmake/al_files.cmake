option(USE_GLV "" OFF)

# Main Library
set(core_headers
  include/al/core/app/al_App.hpp
  include/al/core/app/al_AudioApp.hpp
  include/al/core/app/al_FPS.hpp
  include/al/core/app/al_WindowApp.hpp
  include/al/core/graphics/al_BufferObject.hpp
  include/al/core/graphics/al_DefaultShaders.hpp
  include/al/core/graphics/al_EasyFBO.hpp
  include/al/core/graphics/al_EasyVAO.hpp
  include/al/core/graphics/al_FBO.hpp
  include/al/core/graphics/al_GLEW.hpp
  include/al/core/graphics/al_GLFW.hpp
  include/al/core/graphics/al_GPUObject.hpp
  include/al/core/graphics/al_Graphics.hpp
  include/al/core/graphics/al_Lens.hpp
  include/al/core/graphics/al_Light.hpp
  include/al/core/graphics/al_Mesh.hpp
  include/al/core/graphics/al_OpenGL.hpp
  include/al/core/graphics/al_RenderManager.hpp
  include/al/core/graphics/al_Shader.hpp
  include/al/core/graphics/al_Shapes.hpp
  include/al/core/graphics/al_Texture.hpp
  include/al/core/graphics/al_VAO.hpp
  include/al/core/graphics/al_VAOMesh.hpp
  include/al/core/graphics/al_Viewpoint.hpp
  include/al/core/io/al_AudioIO.hpp
  include/al/core/io/al_AudioIOData.hpp
  include/al/core/io/al_ControlNav.hpp
  include/al/core/io/al_CSVReader.hpp
  include/al/core/io/al_File.hpp
  include/al/core/io/al_MIDI.hpp
  include/al/core/io/al_Window.hpp
  include/al/core/math/al_Constants.hpp
  include/al/core/math/al_Mat.hpp
  include/al/core/math/al_Matrix4.hpp
  include/al/core/math/al_Quat.hpp
  include/al/core/math/al_StdRandom.hpp
  include/al/core/math/al_Vec.hpp
  include/al/core/protocol/al_OSC.hpp
  include/al/core/sound/al_Ambisonics.hpp
  include/al/core/sound/al_AudioScene.hpp
  include/al/core/sound/al_Biquad.hpp
  include/al/core/sound/al_Crossover.hpp
  include/al/core/sound/al_Dbap.hpp
  include/al/core/sound/al_Reverb.hpp
  include/al/core/sound/al_Speaker.hpp
  include/al/core/sound/al_StereoPanner.hpp
  include/al/core/sound/al_Vbap.hpp
  include/al/core/spatial/al_Pose.hpp
  include/al/core/system/al_Printing.hpp
  include/al/core/system/al_Thread.hpp
  include/al/core/system/al_Time.hpp
  include/al/core/types/al_Color.hpp
)

set(core_sources
  src/core/app/al_AudioApp.cpp
  src/core/app/al_FPS.cpp
  src/core/app/al_WindowApp.cpp
  src/core/graphics/al_BufferObject.cpp
  src/core/graphics/al_DefaultShaders.cpp
  src/core/graphics/al_EasyFBO.cpp
  src/core/graphics/al_EasyVAO.cpp
  src/core/graphics/al_FBO.cpp
  src/core/graphics/al_GLEW.cpp
  src/core/graphics/al_GLFW.cpp
  src/core/graphics/al_GPUObject.cpp
  src/core/graphics/al_Graphics.cpp
  src/core/graphics/al_Lens.cpp
  src/core/graphics/al_Light.cpp
  src/core/graphics/al_Mesh.cpp
  src/core/graphics/al_OpenGL.cpp
  src/core/graphics/al_RenderManager.cpp
  src/core/graphics/al_Shader.cpp
  src/core/graphics/al_Shapes.cpp
  src/core/graphics/al_Texture.cpp
  src/core/graphics/al_VAO.cpp
  src/core/graphics/al_VAOMesh.cpp
  src/core/graphics/al_Viewpoint.cpp
  src/core/io/al_AudioIO.cpp
  src/core/io/al_AudioIOData.cpp
  src/core/io/al_ControlNav.cpp
  src/core/io/al_CSVReader.cpp
  src/core/io/al_File.cpp
  src/core/io/al_MIDI.cpp
  src/core/io/al_Window.cpp
  src/core/io/al_WindowGLFW.cpp
  src/core/math/al_StdRandom.cpp
  src/core/protocol/al_OSC.cpp
  src/core/sound/al_Ambisonics.cpp
  src/core/sound/al_AudioScene.cpp
  src/core/sound/al_Biquad.cpp
  src/core/sound/al_Dbap.cpp
  src/core/sound/al_Vbap.cpp
  src/core/spatial/al_Pose.cpp
  src/core/system/al_Printing.cpp
  src/core/system/al_ThreadNative.cpp
  src/core/system/al_Time.cpp
  src/core/types/al_Color.cpp
)

set(util_headers
  include/al/util/al_Array.h
  include/al/util/al_Array.hpp
)

set(util_sources
  src/util/al_Array_C.c
  src/util/al_Array.cpp
)

set(al_headers
  ${core_headers}
  ${util_headers}
)

set(al_sources
  ${core_sources}
  ${util_sources}
)

# OPTIONAL ---------------------------------------------------------------------

set(glv_headers
  include/al/glv/al_GLV.hpp
  include/al/glv/glv.h
  include/al/glv/glv_behavior.h
  include/al/glv/glv_buttons.h
  include/al/glv/glv_conf.h
  include/al/glv/glv_core.h
  include/al/glv/glv_font.h
  include/al/glv/glv_layout.h
  include/al/glv/glv_model.h
  include/al/glv/glv_notification.h
  include/al/glv/glv_rect.h
  include/al/glv/glv_sliders.h
  include/al/glv/glv_textview.h
  include/al/glv/glv_util.h
  include/al/glv/glv_widget.h
)

set(glv_sources
  src/glv/al_GLV_draw.cpp
  src/glv/al_GLV_wrapper.cpp
  src/glv/glv_buttons.cpp
  src/glv/glv_core.cpp
  src/glv/glv_font.cpp
  src/glv/glv_glv.cpp
  src/glv/glv_inputdevice.cpp
  src/glv/glv_layout.cpp
  src/glv/glv_model.cpp
  src/glv/glv_notification.cpp
  src/glv/glv_sliders.cpp
  src/glv/glv_textview.cpp
  src/glv/glv_view.cpp
  src/glv/glv_widget.cpp
)

if (USE_GLV)
  list(APPEND al_headers ${glv_headers})
  list(APPEND al_sources ${glv_sources})
endif()