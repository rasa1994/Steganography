include("../cmake/LoadImageHandler.cmake")

add_library(crypt STATIC Crypt.ixx)
add_library(imagecrypthandler STATIC ImageCryptHandler.ixx)
target_link_libraries(imagecrypthandler PRIVATE imagehandler crypt)
target_include_directories(imagecrypthandler PUBLIC ${imagehandler_SOURCE_DIR})