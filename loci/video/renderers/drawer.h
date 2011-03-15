#ifndef LOCI_VIDEO_RENDERERS_DRAWER_H_
#define LOCI_VIDEO_RENDERERS_DRAWER_H_

/**
 * An interface for scene renderers
 * An interface for scene renderers that simply render something without
 * per-frame parameters.
 *
 * @file   drawer.h
 * @author David Gill
 * @date   24/04/2010
 */

namespace loci {
namespace video
{
    class scene_renderer
    {
    public:
        scene_renderer(bool reflective = false)
            : reflective_(reflective)
        {
        }

        void draw()
        {
            render();
        }

        bool reflective() { return reflective_; }

    protected:
        void reflections(bool flag) { reflective_ = flag; }

    private:
        virtual void render() = 0;

        bool reflective_;
    };
} // namespace video
} // namespace loci

#endif // LOCI_VIDEO_RENDERERS_DRAWER_H_