/**
 * Loads .obj models into a vertex_array and materials_list.
 * Parses .obj files but does not consider groups of any sort.
 *
 * @file   obj_loader.h
 * @author David Gill
 * @date   24/04/2009
 */

#include <vector>
#include <string>
#include <istream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <cstdio>

#include <boost/lexical_cast.hpp>

#include "loci/platform/tstring.h"

#include "loci/video/resources/obj_loader.h"
#include "loci/video/resources/mesh.h"
#include "loci/video/resources/file_resource_cache.h"

namespace loci {
namespace video
{
    namespace // anonymous
    {
        void parse(const char *& obj, float & f)
        {
            char * end;
            f = static_cast<float>(std::strtod(obj, &end));
            obj += (end - obj);
        }

        void parse(const char *& obj, unsigned long & ul)
        {
            char * end;
            ul = std::strtoul(obj, &end, 10);
            obj += (end - obj);
        }

        void slash_separated_indices(const char *& obj, unsigned long & index1, unsigned long & index2, unsigned long & index3)
        {
            parse(obj, index1);
            ++obj;
            parse(obj, index2);
            ++obj;
            parse(obj, index3);
        }
    } // anonymous namespace

    void parse_obj(vertex_array & va, materials_list & ml, const std::vector<char> & source, bool invert_v_texcood)
    {
        const char * obj = &source.front();
        const char * end = obj + source.size();

        std::vector<vertex_xyz>  v_list;
        std::vector<vertex_uv>   vt_list;
        std::vector<vertex_nxyz> vn_list;

        // first-guess at memory requirements
        unsigned long amount = source.size() / 100;
        v_list.reserve(amount);
        vt_list.reserve(amount);
        vn_list.reserve(amount);
        va.reserve(amount / 3);

        // fallbacks
        v_list.push_back(vertex_xyz(0, 0, 0));
        vt_list.push_back(vertex_uv(0, 0));
        vn_list.push_back(vertex_nxyz(0, 1, 0));
        unsigned int duff_material_influence = 0;

        unsigned long vi = 0;
        unsigned long vti = 0;
        unsigned long vni = 0;
        unsigned int * material_influence = &duff_material_influence;
        vertex v1, v2;
        vertex_xyz pos;
        vertex_nxyz norm;
        vertex_uv tex;

        while (obj < end)
        {
            if (obj[0] == 'v')
            {          
                if (obj[1] == 't')
                {
                    obj += 2;

                    parse(obj, tex.u);
                    parse(obj, tex.v);
                    if (invert_v_texcood) {  tex.v = 1.0f - tex.v; }
                    vt_list.push_back(tex);
                }
                else if (obj[1] == 'n')
                {
                    obj += 2;

                    parse(obj, norm.x);
                    parse(obj, norm.y);
                    parse(obj, norm.z);
                    vn_list.push_back(norm);
                }
                else // == 'v'
                {
                    ++obj;

                    parse(obj, pos.x);
                    parse(obj, pos.y);
                    parse(obj, pos.z);
                    v_list.push_back(pos);
                }
            }
            else if (obj[0] == 'f')
            {
                ++obj;

                slash_separated_indices(obj, vi, vti, vni);
                va.push_back(vertex(v_list[vi], vt_list[vti], vn_list[vni]));

                slash_separated_indices(obj, vi, vti, vni);
                va.push_back(vertex(v_list[vi], vt_list[vti], vn_list[vni]));

                slash_separated_indices(obj, vi, vti, vni);
                va.push_back(vertex(v_list[vi], vt_list[vti], vn_list[vni]));
                
                ++(*material_influence);

                if (obj[0] == ' ')
                {
                    v1 = *(va.rbegin() + 2);
                    do
                    {
                        slash_separated_indices(obj, vi, vti, vni);
                        v2 = va.back();
                        va.push_back(v1);
                        va.push_back(v2);
                        va.push_back(vertex(v_list[vi], vt_list[vti], vn_list[vni]));
                        ++(*material_influence);
                    } while (obj[0] == ' ');
                }
            }
            else if (obj[0] == 'u' &&
                     obj[1] == 's' &&
                     obj[2] == 'e' &&
                     obj[3] == 'm' &&
                     obj[4] == 't' &&
                     obj[5] == 'l' &&
                     obj[6] == ' ')
            {
                obj += 7;

                unsigned int face_index = *material_influence;
                ml.push_back(material_range(face_index));
                material_influence = &ml.back().end;

                const char * mtl_end = obj;
                while (*mtl_end != '\n' && mtl_end < end) { ++mtl_end; }
                ml.back().material.assign(obj, mtl_end - 1);
            }
            else
            {
                while (*obj != '\n' && obj < end) { ++obj; } // consume the line
            }
            ++obj;
        }
    }

    void parse_mtl(file_resource_cache & texture_cache, const std::string & textures_dir, std::istream & is)
    {
        std::string material;
        std::string token;
        while (is >> token)
        {
            if (token == "newmtl")
            {
                is >> material;
            }
            else if (token == "map_Kd")
            {
                is >> token;
                texture_cache.cache(platform::to_tstr(textures_dir + token), material);
            }
            else
            {
                std::getline(is, token); // skip line
            }
        }
    }

    void parse_obj(vertex_array & va, materials_list & ml, file_resource_cache & texture_cache, const std::string & obj_path, const std::string & mtl_path, bool invert_v_texcood)
    {
		std::ifstream mtl_ifs(mtl_path.c_str());

        FILE * obj_file = std::fopen(obj_path.c_str(), "rb");
        if (!obj_file) { return; }

        std::fseek(obj_file, 0, SEEK_END);
        std::size_t file_size = std::ftell(obj_file);
        std::rewind(obj_file);

        std::vector<char> obj_data(file_size);
        std::fread(&obj_data.front(), 1, file_size, obj_file);

        std::fclose(obj_file);

        parse_obj(va, ml, obj_data, invert_v_texcood);

        std::string::size_type pos = mtl_path.find_last_of("/\\");
        if (pos == std::string::npos) { pos = 0; } else { ++pos; }
        parse_mtl(texture_cache, std::string(mtl_path, 0, pos), mtl_ifs);
    }
    
    void parse_obj(vertex_array & va, materials_list & ml, file_resource_cache & texture_cache, const std::string & path, bool invert_v_texcood)
    {
        std::string mtl_path(path);
        mtl_path.replace(mtl_path.find_last_of('.'), std::string::npos, ".mtl");
        parse_obj(va, ml, texture_cache, path, mtl_path, invert_v_texcood);
    }
} // namespace video
} // namespace loci