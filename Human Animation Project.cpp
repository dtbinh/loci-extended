// Human Animation Project.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"

#include <iostream>
#include <exception>

#include "loci/loci.h"

/*
void run_variety()
{
    typedef loci::video::render_view app_window;

    boost::shared_ptr<loci::anim::sync_warp> syncher = boost::make_shared<loci::anim::sync_warp>();
    syncher->add_frame(0, 0);
    syncher->add_frame(0, 6000);
    syncher->add_frame(0, 6100);

    boost::shared_ptr<loci::anim::sync_warp> syncher2 = boost::make_shared<loci::anim::sync_warp>();
    syncher2->add_frame(0, 0);
    syncher2->add_frame(0, 1000);
    syncher2->add_frame(0, 2000);

    loci::mocaps::bvh_document::mocap bvh = loci::mocaps::import_bvh_file("female/Female1_B11_WalkTurnLeft135.bvh", true);
    loci::mocaps::bvh_document::mocap bvh2 = loci::mocaps::import_bvh_file("female/Female1_C12_RunTurnLeft45.bvh", true);

    boost::shared_ptr<loci::anim::clip> motion = boost::make_shared<loci::anim::clip>(bvh.motion(), syncher);
    //boost::shared_ptr<loci::anim::clip> motion2 = boost::make_shared<loci::anim::clip>(bvh2.motion(), syncher2);
    //motion2->debug_colour(0, 0, 1);

    /*boost::shared_ptr<loci::anim::blend_sequence> seq = boost::make_shared<loci::anim::blend_sequence>();
    seq->append(motion, 0);
    seq->append(motion2, 2000);

    boost::shared_ptr<loci::anim::speed_transition> speeder = boost::make_shared<loci::anim::speed_transition>(
        motion , 1, 1,
        motion2, 2, 1);

    boost::shared_ptr<loci::anim::sync_blend> sync = boost::make_shared<loci::anim::sync_blend>(seq);*/
/*
    boost::shared_ptr<loci::anim::follow_heading> fix = boost::make_shared<loci::anim::follow_heading>();
    fix->anim(motion);


    boost::shared_ptr<loci::anim::anim_graph> g = boost::make_shared<loci::anim::anim_graph>(fix, bvh.figure());
    g->speed(1.8);
    g->direction(loci::anim::anim_graph::direction_type(0, 1));

    loci::simple_crowd people;
    people.insert_actor(g);
    
    app_window canvas(LOCI_TSTR("Mocap Test App"), 800, 600);
    //app_window canvas(LOCI_TSTR("Mocap Test App"), 800, 600, "obj/regent/regent.obj", 1000);
    while (!loci::platform::win32::dispatch_messages())
    {
        //canvas.rotate_camera(1.0f);
        //canvas.render( people, 0, 10000, 200 );
        canvas.render( people.update(20) );
    }
}*/

/*void run_exporting()
{
    loci::control::xml_crowd_controller crowd("actorscript.act");
    
    loci::mocaps::record_to_bvh_file("exported.bvh", *crowd.animation(0), 0.0333333, 8.0);

    loci::mocaps::bvh_document::mocap bvh = loci::mocaps::import_bvh_file("exported.bvh", false);
    boost::shared_ptr<loci::anim::clip> motion = boost::make_shared<loci::anim::clip>(bvh.motion());
    std::cout << motion->duration();
    boost::shared_ptr<loci::anim::anim_graph> g = boost::make_shared<loci::anim::anim_graph>(motion, bvh.figure());
 
    loci::simple_crowd people;
    people.insert_actor(g);
    
    loci::video::render_view canvas(LOCI_TSTR("Script Test"), 800, 600);
    while (!loci::platform::win32::dispatch_messages())
    {
        canvas.render( crowd.update(10) );
    }
}*/

/*using namespace loci;

class spiral : public anim::blend_node
{
public:
    spiral(const blend_node_ptr & node, float amount) : input(connect(node)), amount(amount) { }

private:
    void master_sync_properties(const anim::time_info & sync_info, const anim::blend_info & blending)
    {
        // find the sync properties of the highest weighted node, in this case there is only one
        find_sync_properties(input, sync_info, blending);
    }

    void evaluate(const anim::time_info & timing, const anim::blend_info & info)
    {
        // contribute rotation around the vertical axis (0.0, 1.0, 0.0) to the rotation delta,
        // weight the contribution by the amount() property of the blend_info structure.
        alerp(info.rotation_delta(), numeric::quat_from_axisangle(0.0f, 1.0f, 0.0f, amount), info.amount());
        animate(input, timing, info); // animate child node
    }

    void contract_children(contractor & contract) const
    {
        // include child node in the contract
        contract.imbue(input); 
    }

    blend_node_slot input; // input node slot
    float amount;          // rotation amount
};

control::xml::parser::blend_node_ptr
  parse_spiral(
      const control::xml::xml_parse_tree            &  xml    ,
        control::xml::motion_set                    &  motions,
         const control::xml::anim_parse_dispatcher  &  node_parser)
{
    using namespace control::xml;
    float amount = 10.0f; // default
    try_get_attr(xml, amount, "by");

    return parser::blend_node_ptr(
        new spiral(parse_blend_node(xml, motions, node_parser), amount));
}


void run_customnode()
{
    script_view_application app;
    app.controller()->set_blend_node_parser("spiral", parse_spiral);
    app.start(LOCI_TSTR("Loci :: A Human Animation API"), 800, 600, "actorscript.act");
}*/

void run_script()
{
    loci::script_view_application(LOCI_TSTR("Loci :: A Human Animation API"), 800, 600, "actorscript.act"/*, "obj/regent/regent.obj", 1000*/);
}

int main()
{
    std::cout << "Debugging Session.\nN.B. Do not terminate "
                 "the application from this console window\n\n";

    try
    {
        run_script();
    }
    catch (std::exception & e)
    {
        std::cerr << "\n\n*** An Exception Occured ***\n\n"
                  << e.what()
                  << "\n\n****************************\n"
                  << "Press enter to exit.";
        std::cin.get();
    }
    catch (...)
    {
        std::cerr << "\n\n*** An Unknown Exception Occured ***\n\n"
                  << "\n\n****************************\n"
                  << "Press enter to exit.";
        std::cin.get();
    }
}