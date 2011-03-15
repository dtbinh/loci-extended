#ifndef LOCI_ANIM_ANIMTREE_H_
#define LOCI_ANIM_ANIMTREE_H_

/**
 * A convenience header to group components of the anim-tree.
 *
 * @file   animtree.h
 * @author David Gill
 * @date   27/02/2010
 */

#include <boost/shared_ptr.hpp>

#include "loci/anim/blend_info.h"
#include "loci/anim/time_info.h"
#include "loci/anim/blend_node.h"
#include "loci/anim/sequence_node.h"

#include "loci/anim/nodes/anim_graph.h"
#include "loci/anim/nodes/blend_sequence.h"
#include "loci/anim/nodes/clip.h"
#include "loci/anim/nodes/feather.h"
#include "loci/anim/nodes/tempo.h"
#include "loci/anim/nodes/speed_blend.h"
#include "loci/anim/nodes/duration.h"
#include "loci/anim/nodes/trim.h"
#include "loci/anim/nodes/time_offset.h"
#include "loci/anim/nodes/heading.h"
#include "loci/anim/nodes/sync.h"

namespace loci {
namespace anim
{
    typedef boost::shared_ptr<blend_node>       blend_node_ptr;
    typedef boost::shared_ptr<sequence_node>    seq_node_ptr;
    typedef boost::shared_ptr<anim_graph>       anim_graph_ptr;

    typedef boost::shared_ptr<blend_sequence>   blend_seq_ptr;
    typedef boost::shared_ptr<clip>             clip_ptr;
    typedef boost::shared_ptr<feather>          feather_ptr;
    typedef boost::shared_ptr<tempo>            tempo_ptr;
    typedef boost::shared_ptr<speed_transition> speed_transition_ptr;
    typedef boost::shared_ptr<apply_duration>   apply_duration_ptr;
    typedef boost::shared_ptr<alter_duration>   alter_duration_ptr;
    typedef boost::shared_ptr<trim>             trim_ptr;
    typedef boost::shared_ptr<time_offset>      time_offset_ptr;
    typedef boost::shared_ptr<fix_heading>      fix_heading_ptr;
    typedef boost::shared_ptr<follow_heading>   follow_heading_ptr;
    typedef boost::shared_ptr<sync_blend>       sync_blend_ptr;
} // anim namespace
} // loci namespace


#endif // LOCI_ANIM_ANIMTREE_H_