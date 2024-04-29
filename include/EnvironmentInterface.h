#ifndef SPlat_EnvironmentInterface_h
#define SPlat_EnvironmentInterface_h

#include "model/Character.h"

namespace SPlat {

    class EnvironmentInterface {

    public:

        virtual Model::Character * get_controlled_asset() = 0;

        virtual long get_framerate_limit() = 0;

        virtual bool get_running() = 0;

        virtual float get_unit() = 0;

        virtual void set_controlled_asset(Model::Character *) = 0;

        virtual void set_framerate_limit(long) = 0;

        virtual void set_running(bool) = 0;

        virtual void set_unit(float) = 0;

    };

}

#endif