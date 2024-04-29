#ifndef SPlat_EnvironmentInterface_h
#define SPlat_EnvironmentInterface_h

namespace SPlat {

    class EnvironmentInterface {

    public:

        virtual long get_framerate_limit() = 0;

        virtual bool get_running() = 0;

        virtual void set_framerate_limit(long) = 0;

        virtual void set_running(bool) = 0;

    };

}

#endif