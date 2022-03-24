#include <gazebo/gazebo.hh>

namespace gazebo 
{
    class WorldPluginP1 : public WorldPlugin {
        public:
        WorldPluginP1() : WorldPlugin() {
            std::cout << "Welcome to Raul's World!" << std::endl;
        }

        void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf) { }
    };

    GZ_REGISTER_WORLD_PLUGIN(WorldPluginP1)
};
