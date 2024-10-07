
set(GKIT_SOURCE_DIR "./Source")
set(GKIT_INCLUDE_DIR "./Include")
add_library(gkit STATIC ${GKIT_SOURCE_DIR}/app_camera.cpp
                        ${GKIT_SOURCE_DIR}/app_time.cpp
                        ${GKIT_SOURCE_DIR}/app.cpp
                        ${GKIT_SOURCE_DIR}/cgltf.cpp
                        ${GKIT_SOURCE_DIR}/color.cpp
                        ${GKIT_SOURCE_DIR}/draw.cpp
                        ${GKIT_SOURCE_DIR}/envmap.cpp
                        ${GKIT_SOURCE_DIR}/files.cpp
                        ${GKIT_SOURCE_DIR}/framebuffer.cpp
                        ${GKIT_SOURCE_DIR}/gamepads.cpp
                        ${GKIT_SOURCE_DIR}/gltf.cpp
                        ${GKIT_SOURCE_DIR}/image_hdr.cpp
                        ${GKIT_SOURCE_DIR}/image_io.cpp
                        ${GKIT_SOURCE_DIR}/image.cpp
                        ${GKIT_SOURCE_DIR}/mat.cpp
                        ${GKIT_SOURCE_DIR}/mesh.cpp
                        ${GKIT_SOURCE_DIR}/orbiter.cpp
                        ${GKIT_SOURCE_DIR}/program.cpp
                        ${GKIT_SOURCE_DIR}/rgbe.cpp
                        ${GKIT_SOURCE_DIR}/text.cpp
                        ${GKIT_SOURCE_DIR}/texture.cpp
                        ${GKIT_SOURCE_DIR}/uniforms.cpp
                        ${GKIT_SOURCE_DIR}/vec.cpp
                        ${GKIT_SOURCE_DIR}/wavefront_fast.cpp
                        ${GKIT_SOURCE_DIR}/wavefront.cpp
                        ${GKIT_SOURCE_DIR}/widgets.cpp
                        ${GKIT_SOURCE_DIR}/window.cpp

                        ${GKIT_INCLUDE_DIR}/app_camera.h
                        ${GKIT_INCLUDE_DIR}/app_time.h
                        ${GKIT_INCLUDE_DIR}/app.h
                        ${GKIT_INCLUDE_DIR}/cgltf.h
                        ${GKIT_INCLUDE_DIR}/color.h
                        ${GKIT_INCLUDE_DIR}/draw.h
                        ${GKIT_INCLUDE_DIR}/envmap.h
                        ${GKIT_INCLUDE_DIR}/files.h
                        ${GKIT_INCLUDE_DIR}/framebuffer.h                                       
                        ${GKIT_INCLUDE_DIR}/gamepads.h
                        ${GKIT_INCLUDE_DIR}/glcore.h
                        ${GKIT_INCLUDE_DIR}/gltf.h
                        ${GKIT_INCLUDE_DIR}/image_hdr.h
                        ${GKIT_INCLUDE_DIR}/image_io.h
                        ${GKIT_INCLUDE_DIR}/image.h
                        ${GKIT_INCLUDE_DIR}/mat.h
                        ${GKIT_INCLUDE_DIR}/materials.h
                        ${GKIT_INCLUDE_DIR}/mesh.h
                        ${GKIT_INCLUDE_DIR}/orbiter.h
                        ${GKIT_INCLUDE_DIR}/program.h
                        ${GKIT_INCLUDE_DIR}/quaternion.h
                        ${GKIT_INCLUDE_DIR}/rgbe.h
                        ${GKIT_INCLUDE_DIR}/text.h
                        ${GKIT_INCLUDE_DIR}/texture.h
                        ${GKIT_INCLUDE_DIR}/uniforms.h
                        ${GKIT_INCLUDE_DIR}/vec.h
                        ${GKIT_INCLUDE_DIR}/wavefront_fast.h
                        ${GKIT_INCLUDE_DIR}/wavefront.h
                        ${GKIT_INCLUDE_DIR}/widgets.h
                        ${GKIT_INCLUDE_DIR}/window.h
                        )

target_include_directories(gkit PUBLIC ${GKIT_INCLUDE_DIR})
target_link_libraries(gkit PUBLIC GL GLEW SDL2 SDL2_image)