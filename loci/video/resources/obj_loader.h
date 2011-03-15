#ifndef LOCI_VIDEO_OBJ_LOADER_H_
#define LOCI_VIDEO_OBJ_LOADER_H_

/**
 * Loads .obj models into a vertex_array and materials_list.
 * Parses .obj files but does not consider groups of any sort.
 *
 * @file   obj_loader.h
 * @author David Gill
 * @date   24/04/2009
 */

#include <string>
#include <istream>
#include <vector>

#include "loci/video/resources/mesh.h"

namespace loci {
namespace video
{
    // forward declaration
    class file_resource_cache;

    void parse_obj(vertex_array & va, materials_list & ml, const std::vector<char> & source, bool invert_v_texcood);
    void parse_mtl(file_resource_cache & texture_cache, const std::string & textures_dir, std::istream & is);

    void parse_obj(vertex_array & va, materials_list & ml, file_resource_cache & texture_cache, const std::string & obj_path, const std::string & mtl_path, bool invert_v_texcood = false);
    void parse_obj(vertex_array & va, materials_list & ml, file_resource_cache & texture_cache, const std::string & path, bool invert_v_texcood = false);
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_OBJ_LOADER_H_